/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#include "VRBClient.h"
#include <stdio.h>
#include <string.h>
#include <util/unixcompat.h>

#ifdef _WIN32
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

#include <config/CoviseConfig.h>
#include <net/covise_connect.h>
#include <net/covise_host.h>
#include <net/covise_socket.h>
#include <net/message.h>
#include <net/message_types.h>
#include <net/tokenbuffer.h>
#include <net/udpMessage.h>
#include <net/udp_message_types.h>
#include <util/common.h>

using namespace covise;

VRBClient::VRBClient(const char *n, const char *collaborativeConfigurationFile, bool slave)
    : VRBClient(n, readcollaborativeConfigurationFile(collaborativeConfigurationFile), slave)
    {}

VRBClient::VRBClient(const char *n, const vrb::VrbCredentials &credentials, bool slave)
    : sendDelay(0.1f)
	, isSlave(slave)
    , m_credentials(credentials)
    , name(n)
{
    if (!credentials.ipAddress.empty())
    {
        serverHost = new Host(credentials.ipAddress.c_str());
    }
}

VRBClient::~VRBClient()
{
    delete sConn;
	delete udpConn;
}

int VRBClient::getID()
{
    return ID;
}

int VRBClient::poll(Message *m)
{
    if (isSlave)
        return 0;
    if (sConn->check_for_input())
    {
        sConn->recv_msg(m);
        return 1;
    }
	return 0;


}
bool VRBClient::pollUdp(vrb::UdpMessage* m)
{
	if (!udpConn)
	{
		return false;
	}
	return udpConn->recv_udp_msg(m);
}
int VRBClient::wait(Message *m)
{
#ifdef MB_DEBUG
    std::cerr << "VRBCLIENT::MESSAGEWAIT: Message: " << m->type << std::endl;
    std::cerr << "VRBCLIENT::MESSAGEWAIT: Data: " << m->data << std::endl;
#endif
    if (isSlave)
        return 0;
    if (messageQueue.size())
    {
        *m = *(messageQueue.front()); // copy message
        messageQueue.remove(messageQueue.front());
        delete m;
        return 1;
    }

    return sConn->recv_msg(m);
}

int VRBClient::wait(Message *m, int messageType)
{
#ifdef MB_DEBUG
    if (m->type != COVISE_MESSAGE_EMPTY)
    {
        std::cerr << "VRBCLIENT::MESSAGEWAIT: Message: " << m->type << std::endl;
        std::cerr << "VRBCLIENT::MESSAGEWAIT: Data: " << m->data << std::endl;
        std::cerr << "VRBCLIENT::MESSAGEWAIT: Type: " << messageType << std::endl;
    }
#endif
    int ret = 1;
    while (ret > 0)
    {
        ret = sConn->recv_msg(m);
        if (m->type == messageType)
        {
            return ret;
        }
        else
        {
            Message *msg = new Message(*m);
            messageQueue.push_back(msg);
        }
        if(udpConn)
        {
		ret = udpConn->recv_msg(m);
		if (m->type == messageType)
		{
			return ret;
		}
		else
		{
			Message* msg = new Message(*m);
			messageQueue.push_back(msg);
		}
        }


    }
    return ret;
}

int VRBClient::setUserInfo(const char *userInfo)
{
    if (serverHost == NULL)
        return 0;
    if (!sConn || (!sConn->is_connected())) // not connected to a server
    {
        return 0;
    }
    TokenBuffer tb;
    tb << userInfo;
    Message msg(tb);
    msg.type = COVISE_MESSAGE_VRB_SET_USERINFO;
    sConn->send_msg(&msg);
    return 1;
}

int VRBClient::sendMessage(const Message *m)
{
	if (!sConn || (!sConn->is_connected())) // not connected to a server
	{
		return 0;
	}
	return sendMessage(m, sConn);
}

void covise::VRBClient::sendMessage(TokenBuffer & tb, int type)
{
    Message m(tb);
    m.type = type;
    sendMessage(&m);
}

int VRBClient::sendUdpMessage(const vrb::UdpMessage* m)
{
	if (!udpConn) // not connected to a server
	{
		return 0;
	}
    m->sender = ID;
	return udpConn->send_udp_msg(m);

}

void covise::VRBClient::sendUdpMessage(TokenBuffer& tb, vrb::udp_msg_type type, int sender)
{
	vrb::UdpMessage m(tb);
	m.type = type;
	m.sender = sender;
	sendUdpMessage(&m);
}


int covise::VRBClient::sendMessage(const Message* m, Connection* conn)
{
#ifdef MB_DEBUG
	std::cerr << "VRBCLIENT::SENDMESSAGE: Sending Message: " << m->type << std::endl;
	std::cerr << "VRBCLIENT::SENDMESSAGE: Data: " << m->data << std::endl;
#endif

	if (serverHost == NULL)
		return 0;
	//cerr << "sendMessage " << conn << endl;

#ifdef _MSC_VER
	struct __timeb64 start;
	_ftime64(&start);
#else
	struct timeval start, end;
	gettimeofday(&start, NULL);
#endif

	conn->send_msg(m);
#ifdef _MSC_VER
	struct __timeb64 end;
	_ftime64(&end);
#else
	gettimeofday(&end, NULL);
#endif

#ifdef _MSC_VER
	float delay = (end.millitm - start.millitm) * 1e-6f + end.time - start.time;
#else
	float delay = (end.tv_usec - start.tv_usec) * 1e-6f + end.tv_sec - start.tv_sec;
#endif

	if (m->data.length() > 1000)
		delay /= m->data.length() / 1000;
	sendDelay = 0.99f * sendDelay + delay * 0.01f;
	return 1;
}

int VRBClient::isConnected()
{
    if (isSlave)
        return 1;
    if (sConn == NULL)
    {
        return completeConnection();
    }
    return sConn->is_connected();
}

int VRBClient::connectToServer(std::string sessionName)
{
    startupSession = sessionName;
    if (!udpConn && !isSlave)
    {
		setupUdpConn();
	}
	if (isSlave || serverHost == NULL|| sConn != nullptr)
        return 0;

    connMutex.lock();
    if (!connFuture.valid())
    {
        connFuture = std::async(std::launch::async, [this]() -> ClientConnection *
        {
            ClientConnection *myConn = new ClientConnection(serverHost, m_credentials.tcpPort, 0, (sender_type)0,0, 1.0);
            if (!myConn->is_connected()) // could not open server port
            {
                if (firstVrbConnection)
                {
                    fprintf(stderr, "Could not connect to server on %s; port %d\n", serverHost->getAddress(), m_credentials.tcpPort);
                    firstVrbConnection = false;
                }

                delete myConn;
                myConn = NULL;
                sleep(1);
                return nullptr;
            }
            struct linger linger;
            linger.l_onoff = 0;
            linger.l_linger = 1;
            setsockopt(myConn->get_id(NULL), SOL_SOCKET, SO_LINGER, (char *)&linger, sizeof(linger));

            return myConn;
        });
    }
    connMutex.unlock();
    completeConnection();


    if (!sConn)
    {
        return false;
    }
    return true;
}

bool VRBClient::completeConnection(){
    if (connFuture.valid())
    {
        std::lock_guard<std::mutex> g(connMutex);
        auto status = connFuture.wait_for(std::chrono::seconds(0));
        if (status == std::future_status::ready)
        {
            m_isConnected = true;
            sConn = connFuture.get();
            if(sConn != nullptr)
            {
                TokenBuffer tb;
				if (startupSession == "")
				{
					tb << name;
                    tb << Host::getHostaddress();
                }
				else
				{
					tb << "-g";
					tb << Host::getHostaddress();
					tb << startupSession.c_str();
				}


                Message msg(tb);
                msg.type = COVISE_MESSAGE_VRB_CONTACT;
                sConn->send_msg(&msg);
                return true;
            }
        }
    }
    return false;
}

void VRBClient::setupUdpConn() 
{
    if(serverHost)
    {
	udpConn = new UDPConnection(0, 0, m_credentials.udpPort, serverHost->getAddress());
    }
}

void VRBClient::setID(int i)
{
    ID = i;
}

int VRBClient::isCOVERRunning()
{
    if (serverHost == NULL)
        return 0;
    if (!sConn || (!sConn->is_connected())) // could not open server port
    {
        return 0;
    }
    int i;
    Host host;
    TokenBuffer tb;
    tb << host.getAddress();
    Message msg(tb);
    msg.type = COVISE_MESSAGE_VRB_CHECK_COVER;
    sConn->send_msg(&msg);
    if (sConn->check_for_input(5))
    {
        sConn->recv_msg(&msg);
        if (msg.type == COVISE_MESSAGE_VRB_CHECK_COVER)
        {
            TokenBuffer rtb(&msg);
            rtb >> i;
            return i;
        }
    }
    return 0;
}

void VRBClient::connectToCOVISE(int argc, const char **argv)
{
    if (serverHost == NULL)
    {
        std::cerr << "can't send Message, because not connected to Server" << std::endl;
        return;
    }
    if (!sConn || (!sConn->is_connected())) // could not open server port
    {
        std::cerr << "can't send Message, because not connected to Server" << std::endl;
        return;
    }
    int i;

    Host host;
    TokenBuffer tb;
    tb << host.getAddress();
    tb << argc;
    for (i = 0; i < argc; i++)
        tb << argv[i];
    Message msg(tb);
    msg.type = COVISE_MESSAGE_VRB_CONNECT_TO_COVISE;
    sConn->send_msg(&msg);
}

float VRBClient::getSendDelay()
{
    return sendDelay;
}

void VRBClient::shutdown(){
    if(m_isConnected)
    {
        int id = sConn->getSocket()->get_id();
        ::shutdown(id, 2); //2 stands for SHUT_RDWR/SD_BOTH 
    }
}


vrb::VrbCredentials covise::readcollaborativeConfigurationFile(const char *collaborativeConfigurationFile){
    if (collaborativeConfigurationFile != NULL)
    {
        std::string ipAddress;
        int tcpPort = 0;
        FILE *fp = fopen(collaborativeConfigurationFile, "r");
        if (fp)
        {
            char buf[5000];
            int tcpPort = 0;
            while (!feof(fp))
            {
                char *retval_fgets;
                retval_fgets = fgets(buf, 5000, fp);
                if (retval_fgets == NULL)
                {
                    std::cerr << "VRBClient::VRBClient: fgets failed" << std::endl;
                    return vrb::VrbCredentials{};
                }
                if (strncmp(buf, "server:", 7) == 0)
                {
                    char *hostName = new char[strlen(buf) + 1];
                    size_t retval;
                    retval = sscanf(buf, "server:%s", hostName);
                    if (retval != 1)
                    {
                        std::cerr << "VRBClient::VRBClient: sscanf failed" << std::endl;
                        delete[] hostName;
                        return vrb::VrbCredentials{};
                    }
                    if (strcasecmp(hostName, "NONE"))
                    {
                        ipAddress = hostName;
                    }
                    
                    delete[] hostName;
                }
                else if (strncmp(buf, "port:", 5) == 0)
                {
                    size_t retval;
                    retval = sscanf(buf, "port:%d", &tcpPort);
                    if (retval != 1)
                    {
                        std::cerr << ": sscanf failed" << std::endl;
                        return vrb::VrbCredentials{};
                    }
                }
            }
            fclose(fp);
            return vrb::VrbCredentials{ipAddress, tcpPort, tcpPort + 1};
        }
        return vrb::VrbCredentials{};
    }
}