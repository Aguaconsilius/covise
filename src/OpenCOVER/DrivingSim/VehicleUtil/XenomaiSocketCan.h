/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#ifndef __XenomaiSocketCan_h
#define __XenomaiSocketCan_h

#include <iostream>
#include <vector>
#ifdef MERCURY
typedef int nanosecs_rel_t;
#include <sys/socket.h>
#include <linux/can.h>
typedef canid_t can_id_t;
#define RTDM_TIMEOUT_INFINITE 0
#else
#include <rtdm/rtcan.h>
#endif
#include <cerrno>
#include <cstring>
#include <util/coExport.h>
#include <stdio.h>

VEHICLEUTILEXPORT std::ostream &operator<<(std::ostream &, const can_frame &);
namespace vehicleUtil
{


class VEHICLEUTILEXPORT XenomaiSocketCan
{
public:
    XenomaiSocketCan(const std::string &);
    ~XenomaiSocketCan();

    int recvFrame(can_frame &, int = 0) const;

    int sendFrame(const can_frame &, int = 0) const;

    void addRecvFilter(u_int32_t, u_int32_t = CAN_SFF_MASK);
    int applyRecvFilters();

    int setRecvTimeout(nanosecs_rel_t);
    int setSendTimeout(nanosecs_rel_t);

    static void printFrame(const can_frame &);
    static void printFrame(const char *text, const can_frame &frame); // using fprintf,stderr for xenomai

    static std::string frameToString(const can_frame &);

private:
    int Socket;
    std::string device;

    std::vector<can_filter> recvFilterVector;
};

inline int XenomaiSocketCan::recvFrame(can_frame &frame, int flags) const
{
#ifdef MERCURY
    int ret_read = recv(Socket, (void *)&frame, sizeof(can_frame), flags);
#else
    int ret_read = rt_dev_recv(Socket, (void *)&frame, sizeof(can_frame_t), flags);
#endif
    if (ret_read < 0)
    {
        switch (ret_read)
        {
        case -ETIMEDOUT:
            //std::cerr << device << ": XenomaiSocketCan::recvFrame(): rt_dev_recv: timed out!" << std::endl;
            break;
        case -EBADF:
            fprintf(stderr, "%s: XenomaiSocketCan::recvFrame(): rt_dev_recv: aborted because socket was closed!\n", device.c_str());
            break;
        default:
            fprintf(stderr, "%s: XenomaiSocketCan::recvFrame(): recv: %s %d!\n", device.c_str(), strerror(-ret_read),ret_read);
        }
    }
    /*else if(device=="rtcan1") {
      std::cerr << "frame received: " << frame << std::endl;
   }*/
    if (frame.can_id & CAN_ERR_FLAG)
    {
        std::cerr << "XenomaiSocketCan::recvFrame(): Error frame: " << frame << std::endl;
    }

    return ret_read;
}

inline int XenomaiSocketCan::sendFrame(const can_frame &frame, int flags) const
{
#ifdef MERCURY
    int ret_send = send(Socket, (void *)&frame, sizeof(can_frame), flags);
#else
    int ret_send = rt_dev_send(Socket, (void *)&frame, sizeof(can_frame_t), flags);
#endif
    if (ret_send < 0)
    {
        switch (ret_send)
        {
        case -ETIMEDOUT:
            //std::cerr << device << ": XenomaiSocketCan::sendFrame(): rt_dev_send: timed out!" << std::endl;
            break;
        case -EBADF:
            fprintf(stderr, "%s: XenomaiSocketCan::sendFrame(): rt_dev_send: aborted because socket was closed!\n", device.c_str());
            break;
        default:
            fprintf(stderr, "%s: XenomaiSocketCan::sendFrame(): rt_dev_send: %s!\n", device.c_str(), strerror(-ret_send));
        }
    }
    /*else if(device=="rtcan1") {
      std::cerr << "frame send: " << frame << std::endl;
   }*/

    return ret_send;
}
}
#endif
