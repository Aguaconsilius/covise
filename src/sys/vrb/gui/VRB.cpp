/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */


#define setfill qtsetfill
#define setprecision qtsetprecision
#define setw qtsetw
#include <QApplication>
#undef setfill
#undef setprecision
#undef setw
#include "VRBServer.h"
#ifndef YAC
#include <covise/covise.h>
#endif
#include <config/CoviseConfig.h>
#include <util/environment.h>

#include "VRBapplication.h"


ApplicationWindow *mw;

int main(int argc, char **argv)
{
    covise::setupEnvironment(argc, argv);
    bool gui = true;
    for (size_t i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--console")==0)
        {
            gui = false;
        }
    }
    if (gui)
    {
		QApplication a(argc, argv);
#ifdef __APPLE__
        a.setAttribute(Qt::AA_DontShowIconsInMenus);
#endif
        mw = new ApplicationWindow();
        mw->setWindowTitle("VRB");
        mw->show();
        a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
		VRBServer server(gui);
		if (server.openServer() < 0)
		{
			return -1;
		}
		if (!server.startUdpServer())
		{
			cerr << "failed to open udp socket" << endl;
		}
		int exitcode = a.exec();

		server.closeServer();
		return exitcode;
    }
	else
	{
		VRBServer server(gui);
		if (server.openServer() < 0)
		{
			return -1;
		}
		if (!server.startUdpServer())
		{
			cerr << "failed to open udp socket" << endl;
		}
		if (!gui)
		{
			server.loop();
		}
		int exitcode = 0;

		server.closeServer();
		return exitcode;
	}



}
