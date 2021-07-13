/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#ifndef COCONFIGCONSTANTS_H
#define COCONFIGCONSTANTS_H

#define CO_CONFIG_TRANSFORM_FILE "transform.xml"

#include <util/coTypes.h>
#include <QString>
#include <QStringList>

namespace covise
{

class CONFIGEXPORT coConfigConstants
{

public:
    enum ConfigScope
    {
        Default = -1,
        Global = 1,
        Cluster,
        Host,
        Additional
    };

    static const QStringList &getArchList();
    static const QString &getHostname();
    static const QString &getBackend();

    static void  setMaster(const QString &hostname);
    static const QString &getMaster();

    static void setBackend(const QString &backend);
    static void setRank(int rank, int shmGroupRootRank=-1);
    static int getRank();
    static int getShmGroupRootRank();

protected:
    coConfigConstants();
    virtual ~coConfigConstants();

private:
    QString backend;
    QString hostname;
    QString master;
    QStringList archlist;
    int rank = -1;
    int shmGroupRoot = -1;

    static coConfigConstants *instance;

    void initXerces();
    void terminateXerces();
};

class CONFIGEXPORT coConfigDefaultPaths
{

private:
    coConfigDefaultPaths();
    ~coConfigDefaultPaths();

public:
    static const QString &getDefaultTransformFileName();

    static const QString &getDefaultLocalConfigFileName();
    static const QString &getDefaultGlobalConfigFileName();

    static const QString &getDefaultLocalConfigFilePath();
    static const QString &getDefaultGlobalConfigFilePath();

    static const QStringList &getSearchPath();

private:
    static coConfigDefaultPaths *getInstance()
    {

        if (instance)
        {
            return instance;
        }
        else
        {
            return new coConfigDefaultPaths();
        }
    }

    static coConfigDefaultPaths *instance;

    void setNames();
    QString configGlobal, configLocal, defaultTransform, configGlobalPath, configLocalPath;
    QStringList searchPath;
};
}
#endif
