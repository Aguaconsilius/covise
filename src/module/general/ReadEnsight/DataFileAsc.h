/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CLASS    DataFileAsc
//
// Description:
//
// Initial version: 2002-
//
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// (C) 2002 by VirCinity IT Consulting
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Changes:
//

#ifndef DATAFILEASC_H
#define DATAFILEASC_H

#include "GeoFileAsc.h"

class DataFileAsc : public EnFile
{
public:
    /// default CONSTRUCTOR
    DataFileAsc(ReadEnsight *mod);
    DataFileAsc(ReadEnsight *mod, const string &name, const int &dim, const int &numVals);

    void read(dimType dim, coDistributedObject **outObjects, const string &baseName, int &timeStep, int numTimeSteps);

    void readCells(dimType dim, coDistributedObject **outObjects, const string &baseName, int &timeStep, int numTimeSteps);

    void setIndexMap(const int *im);

private:
    int lineCnt_; // actual linecount
    int numVals_; // number of values
    int *indexMap_; // may contain indexMap
    int actPartIndex_;
};
#endif
