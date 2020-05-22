/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                                                  (C)2004 ZAIK/RRZK  ++
// ++ Description: ReadVTK module                                         ++
// ++                                                                     ++
// ++ Author:                                                             ++
// ++                                                                     ++
// ++                       Thomas van Reimersdahl                        ++
// ++               Institute for Computer Science (Prof. Lang)           ++
// ++                        University of Cologne                        ++
// ++                         Robert-Koch-Str. 10                         ++
// ++                             50931 Koeln                             ++
// ++                                                                     ++
// ++ Date:  26.09.2004                                                   ++
// ++**********************************************************************/

#include <iostream>

#include "ReadVTK.h"
#include <vtk/coVtk.h>
#include <vtkDataSetReader.h>
#include <vtkXMLGenericDataObjectReader.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#if VTK_MAJOR_VERSION < 5
#include <vtkIdType.h>
#endif
#include <vtkDataSetAttributes.h>

#include <do/coDoData.h>
#include <do/coDoAbstractStructuredGrid.h>
#include <do/coDoSet.h>

const size_t FilenameLength = 2048;

template<class Reader>
vtkDataSet *readFile(const char *filename)
{
    vtkSmartPointer<Reader> reader = vtkSmartPointer<Reader>::New();
    reader->SetFileName(filename);
    reader->Update();
    if (reader->GetOutput())
        reader->GetOutput()->Register(reader);
    return vtkDataSet::SafeDownCast(reader->GetOutput());
}

vtkDataSet *getDataSet(const char *filename)
{
    char fn[FilenameLength] = "";
    Covise::getname(fn, filename);
    if (fn[0] == '\0')
        return NULL;
    vtkDataSet *ds = NULL;
    const char *ext = "";
    size_t len = strlen(fn);
    if (len >= 4)
        ext = fn+len-4;
    if (strcmp(ext, ".vtk")==0)
    {
        ds = readFile<vtkDataSetReader>(fn);
    }
    if (!ds)
        ds = readFile<vtkXMLGenericDataObjectReader>(fn);
    if (!ds && strcmp(ext, ".vtk")!=0)
    {
        ds = readFile<vtkDataSetReader>(fn);
    }
    return ds;
}

std::vector<std::string> getFields(vtkFieldData *dsa)
{
    std::vector<std::string> fields;
    if (!dsa)
        return fields;
    int na = dsa->GetNumberOfArrays();
    for (int i=0; i<na; ++i)
    {
        fields.push_back(dsa->GetArrayName(i));
        //cerr << "field " << i << ": " << fields[i] << endl;
    }
    return fields;
}

void setParamChoices(coChoiceParam *param, const std::vector<std::string> &choices)
{
    int ival = param->getValue();
    std::string sval;
    if (param->getActLabel())
        sval = param->getActLabel();
    std::vector<const char *> vals;
    vals.push_back("(NONE)");
    for (int i=0; i<(int)choices.size(); ++i)
    {
        if (sval == choices[i])
            ival = i+1;
        vals.push_back(choices[i].c_str());
    }
    if (ival >= vals.size())
        ival = 0;
    param->setValue((int)vals.size(), &vals[0], ival);
}


//////
////// we must provide main to init covise
//////

ReadVTK::ReadVTK(int argc, char *argv[])
    : coModule(argc, argv, "Read Visualization Toolkit (VTK) data files")
{
    m_filename = new char[256];

    m_portGrid = addOutputPort("GridOut0", "UniformGrid|StructuredGrid|UnstructuredGrid|RectilinearGrid|Polygons|Lines|Points", "Grid output");

    m_portNormals = addOutputPort("DataOut2", "Vec3", "normals");

    m_pParamFile = addFileBrowserParam("filename", "Specify the filename of the VTK data file(s).");
    m_pParamFile->setValue("./", "*.vtk/*.vti;*.vtp;*.vtr;*.vts;*.vtu/*");

    m_pTime = addBooleanParam("timesteps", "Read several timesteps at once.");
    m_pTime->setValue(0);

    m_pParamFilePattern = addStringParam("filenamepattern", "Specify the filename pattern allowing to read in several vtk datafiles/timesteps.");
    m_pParamFilePattern->setValue("");

    m_pTimeMin = addIntSliderParam("timesteps_min", "Adjust minimal timestep.");
    m_pTimeMin->setValue(0);

    m_pTimeMax = addIntSliderParam("timesteps_max", "Adjust maximal timestep.");
    m_pTimeMax->setValue(0);

    m_iTimestep = 0;
    m_iTimestepMin = 0;
    m_iTimestepMax = 0;

    for (int i=0; i<NumPorts; ++i)
    {
        std::stringstream str;
        str << "point_data_" << i;
        m_pointDataChoice[i] = addChoiceParam(str.str().c_str(), "point data field");

        str.str("");
        str << "PointDataOut" << i;
        std::string name = str.str();
        str.str("");
        str << "vertex data " << i;
        m_portPointData[i] = addOutputPort(name.c_str(), "Float|Vec3|RGBA|Int|Byte", str.str().c_str());
    }
    for (int i=0; i<NumPorts; ++i)
    {
        std::stringstream str;
        str << "cell_data_" << i;
        m_cellDataChoice[i] = addChoiceParam(str.str().c_str(), "cell data field");

        str.str("");
        str << "CellDataOut" << i;
        std::string name = str.str();
        str.str("");
        str << "cell data " << i;
        m_portCellData[i] = addOutputPort(name.c_str(), "Float|Vec3|RGBA|Int|Byte", str.str().c_str());
    }
}

ReadVTK::~ReadVTK()
{
    if (m_filename != NULL)
    {
        delete[] m_filename;
        m_filename = NULL;
    }
}

//////
////// this is our compute-routine
//////

int ReadVTK::compute(const char *)
{
    FILE *pFile = Covise::fopen(m_pParamFile->getValue(), "r");
    if (!pFile)
    {
        Covise::sendError("ERROR: can't open file %s", m_filename);
        return FAIL;
    }
    fclose(pFile);

    update();

    return SUCCESS;
}

//////
////// the read-function itself
//////

bool ReadVTK::FileExists(const char *filename)
{
    FILE *fp = NULL;
    if (!(fp = fopen(filename, "r")))
    {
        std::cout << "File: " << filename << " does not exist." << std::endl;
        return false;
    }
    else
    {
        std::cout << "File: " << filename << " ok." << std::endl;
        fclose(fp);
    }
    return true;
}

void ReadVTK::update()
{

    coDoFloat *str_s3d_out = NULL;
    coDoFloat *unstr_s3d_out = NULL;
    coDoVec3 *str_v3d_out = NULL;
    coDoVec3 *unstr_v3d_out = NULL;

    std::string sInfo;
    int iScalar = 0, iVector = 0;
    //char *cValue=NULL;
    
    bool timesteps = m_pTime->getValue();

    std::vector<coDistributedObject *> dogrid, dopoint[NumPorts], docell[NumPorts], donormal;
    for (int iTimestep = m_iTimestepMin; iTimestep <= m_iTimestepMax; iTimestep++)
    {
        std::string grid_name = m_portGrid->getObjName();
        std::string normal_name = m_portNormals->getObjName();
        std::string pointDataName[NumPorts], cellDataName[NumPorts];
        for (int i=0; i<NumPorts; ++i)
        {
            pointDataName[i] = m_portPointData[i]->getObjName();
            cellDataName[i] = m_portCellData[i]->getObjName();
        }

        vtkDataSet *vdata = NULL;
        if (timesteps)
        {
            const char *filenamepattern = m_pParamFilePattern->getValue();
            if (!std::string(filenamepattern).find("%"))
            {
                Covise::sendInfo("no valid filename pattern - does not contain %% for printf format string");
                return;
            }

            sprintf(m_filename, filenamepattern, iTimestep);
            std::cout << "New Filename is " << m_filename << std::endl;

            vdata = getDataSet(m_filename);

            char buf[256];
            snprintf(buf, sizeof(buf), "%d", iTimestep - m_iTimestepMin);

            grid_name += "_";
            grid_name += buf;

            for (int i=0; i<NumPorts; ++i)
            {
                pointDataName[i] += "_";
                pointDataName[i] += buf;
                cellDataName[i] += "_";
                cellDataName[i] += buf;
            }
        }
        else
        {
            strcpy(m_filename, m_pParamFile->getValue());
        }
        vdata = getDataSet(m_filename);

        if (!vdata)
        {
            continue;
            Covise::sendInfo("could not read: %s", m_filename);
            return;
        }
        coDoGrid *grid = coVtk::vtkGrid2Covise(grid_name, vdata);
        /* if (!grid)
      {
         Covise::sendInfo("not supported: %s", vdata->GetClassName());
         return;
      }*/

        dogrid.push_back(grid);

        vtkFieldData *fieldData = vdata->GetFieldData();
        vtkDataSetAttributes *pointData = vdata->GetPointData();
        vtkDataSetAttributes *cellData = vdata->GetCellData();
        for (int i=0; i<NumPorts; ++i)
        {
            if (pointData && m_pointDataChoice[i]->getValue() > 0)
            {
                std::vector<std::string> fields = getFields(pointData);
                std::string label = fields[m_pointDataChoice[i]->getValue()-1];
                if (coDistributedObject *pdata = coVtk::vtkData2Covise(pointDataName[i], pointData, coVtk::Any, label.c_str(), dynamic_cast<coDoAbstractStructuredGrid *>(grid)))
                    dopoint[i].push_back(pdata);
            }
            else if (fieldData && m_pointDataChoice[i]->getValue() > 0)
            {
                std::vector<std::string> fields = getFields(fieldData);
                std::string label = fields[m_pointDataChoice[i]->getValue()-1];
                int index = -1;
                vtkDataArray *varr = fieldData->GetArray(label.c_str(), index);
                if (coDistributedObject *pdata = coVtk::vtkData2Covise(pointDataName[i], varr, dynamic_cast<coDoAbstractStructuredGrid *>(grid)))
                    dopoint[i].push_back(pdata);
            }
            if (cellData && m_cellDataChoice[i]->getValue() > 0)
            {
                std::vector<std::string> fields = getFields(cellData);
                std::string label = fields[m_cellDataChoice[i]->getValue()-1];
                if (coDistributedObject *cdata = coVtk::vtkData2Covise(cellDataName[i], cellData, coVtk::Any, label.c_str(), dynamic_cast<coDoAbstractStructuredGrid *>(grid)))
                    docell[i].push_back(cdata);
            }
        }

        if (coDistributedObject *normdata = coVtk::vtkData2Covise(normal_name, vdata, coVtk::Normals, NULL, dynamic_cast<coDoAbstractStructuredGrid *>(grid)))
            donormal.push_back(normdata);
    }

    coDistributedObject *outGrid = NULL, *outNormals = NULL;
    coDistributedObject *outPointData[NumPorts], *outCellData[NumPorts];
    for (int i=0; i<NumPorts; ++i)
    {
        outPointData[i] = NULL;
        outCellData[i] = NULL;
    }

    if (timesteps)
    {
        char buf[1024];
        snprintf(buf, sizeof(buf), "%d %d", m_iTimestepMin, m_iTimestepMax);
        std::cout << "TIMESTEP is " << std::string(buf) << std::endl;
        outGrid = NULL;
        if (dogrid.size() > 0)
        {
            outGrid = new coDoSet(m_portGrid->getObjName(), (int)dogrid.size(), &dogrid[0]);
            outGrid->addAttribute("TIMESTEP", buf);
        }
        for (int i=0; i<NumPorts; ++i)
        {
            if (dopoint[i].size() > 0)
            {
                outPointData[i] = new coDoSet(m_portPointData[i]->getObjName(), (int)dopoint[i].size(), &dopoint[i][0]);
                outPointData[i]->addAttribute("TIMESTEP", buf);
            }
            if (docell[i].size() > 0)
            {
                outCellData[i] = new coDoSet(m_portCellData[i]->getObjName(), (int)docell[i].size(), &docell[i][0]);
                outCellData[i]->addAttribute("TIMESTEP", buf);
            }
        }

        outNormals = NULL;
        if (donormal.size() > 0)
        {
            outNormals = new coDoSet(m_portNormals->getObjName(), (int)donormal.size(), &donormal[0]);
            outNormals->addAttribute("TIMESTEP", buf);
        }
    }
    else
    {
        if (!dogrid.empty())
            outGrid = dogrid[0];
        if (!donormal.empty())
            outNormals = donormal[0];
        for (int i=0; i<NumPorts; ++i)
        {
            if (!dopoint[i].empty())
            {
                outPointData[i] = dopoint[i][0];
            }
            if (!docell[i].empty())
            {
                outCellData[i] = docell[i][0];
            }
        }
    }

    // Assign sets to output ports:
    if (outGrid)
        m_portGrid->setCurrentObject(outGrid);
    if (outNormals)
        m_portNormals->setCurrentObject(outNormals);
    for (int i=0; i<NumPorts; ++i)
    {
        m_portPointData[i]->setCurrentObject(outPointData[i]);
        m_portCellData[i]->setCurrentObject(outCellData[i]);
    }
}

void ReadVTK::setChoices(vtkDataSet *ds)
{
    std::vector<std::string> cellFields;
    if (ds)
        cellFields = getFields(ds->GetCellData());
    for (int i=0; i<NumPorts; ++i)
    {
        setParamChoices(m_cellDataChoice[i], cellFields);
    }
    std::vector<std::string> pointFields;
    if (ds && ds->GetPointData())
        pointFields = getFields(ds->GetPointData());
    else if (ds && ds->GetFieldData())
        pointFields = getFields(ds->GetFieldData());
    for (int i=0; i<NumPorts; ++i)
    {
        setParamChoices(m_pointDataChoice[i], pointFields);
    }
}

void ReadVTK::param(const char *name, bool inMapLoading)
{
    if (strcmp(name, m_pParamFile->getName()) == 0)
    {
        m_dataSet = getDataSet(m_pParamFile->getValue());
	if(!inMapLoading)
	{
        setChoices(m_dataSet);
	
        if (strcmp(m_pParamFilePattern->getValue(), "") == 0)
        {
            m_pParamFilePattern->setValue(m_pParamFile->getValue());
        }
	}
    }
    else if (strcmp(name, m_pParamFilePattern->getName()) == 0)
    {
        int result = 0, i = 0;
        char *cTmp = new char[1024];
        // per definitionem, the fully specified filename is the minimum
        result = sscanf(m_filename, m_pParamFilePattern->getValue(), &i);
        if (result == 1)
        {
            m_iTimestepMin = i;
            sprintf(cTmp, m_pParamFilePattern->getValue(), i);
            while (FileExists(cTmp))
            {
                i++;
                sprintf(cTmp, m_pParamFilePattern->getValue(), i);
            }
            m_iTimestepMax = i - 1;
            std::cout << "Timestep Min: " << m_iTimestepMin << std::endl;
            std::cout << "Timestep Max: " << m_iTimestepMax << std::endl;

	if(!inMapLoading)
	{
            m_pTimeMin->setValue(m_iTimestepMin);
            m_pTimeMin->setMin(m_iTimestepMin);
            m_pTimeMin->setMax(m_iTimestepMax);
            m_pTimeMax->setValue(m_iTimestepMax);
            m_pTimeMax->setMin(m_iTimestepMin);
            m_pTimeMax->setMax(m_iTimestepMax);
	 }
        }
    }
    else if (strcmp(name, m_pTimeMax->getName()) == 0)
    {
        m_iTimestepMax = m_pTimeMax->getValue();
        if (m_iTimestepMax < m_iTimestepMin)
        {
	if(!inMapLoading)
	{
            m_pTimeMax->setValue(m_iTimestepMin);
	    }
            m_iTimestepMax = m_iTimestepMin;
            Covise::sendInfo("TimestepMax is not allowed to be smaller than TimestepMin. Thus, the value of TimestepMax is changed to TimestepMin.");
        }
        if (m_iTimestepMax > m_pTimeMax->getMax())
        {
	if(!inMapLoading)
	{
            m_pTimeMax->setValue(m_pTimeMax->getMax());
	    }
            m_iTimestepMax = m_pTimeMax->getMax();
            Covise::sendInfo("TimestepMax changed to maximum timestep.");
        }
        if (m_iTimestepMax < m_pTimeMax->getMin())
        {
	if(!inMapLoading)
	{
            m_pTimeMax->setValue(m_pTimeMax->getMin());
	    }
            m_iTimestepMax = m_pTimeMax->getMin();
            Covise::sendInfo("TimestepMax changed.");
        }

        m_pTimeMin->setMax(m_iTimestepMax);
        std::cout << "iTimestepMax = " << m_iTimestepMax << std::endl;
    }
    else if (strcmp(name, m_pTimeMin->getName()) == 0)
    {
        m_iTimestepMin = m_pTimeMin->getValue();
        if (m_iTimestepMin > m_iTimestepMax)
        {
	if(!inMapLoading)
	{
            m_pTimeMin->setValue(m_iTimestepMax);
	    }
            m_iTimestepMin = m_iTimestepMax;
            Covise::sendInfo("TimestepMin is not allowed to be greater than TimestepMax. Thus, the value of TimestepMin is changed to TimestepMax.");
        }
        if (m_iTimestepMin < m_pTimeMin->getMin())
        {
	if(!inMapLoading)
	{
            m_pTimeMax->setValue(m_pTimeMin->getMin());
	    }
            m_iTimestepMin = m_pTimeMin->getMin();
            Covise::sendInfo("TimestepMin changed to minimum timestep.");
        }
        if (m_iTimestepMin > m_pTimeMin->getMax())
        {
	if(!inMapLoading)
	{
            m_pTimeMax->setValue(m_pTimeMin->getMax());
	    }
            m_iTimestepMin = m_pTimeMin->getMax();
            Covise::sendInfo("TimestepMin changed.");
        }
	if(!inMapLoading)
	{
        m_pTimeMax->setMin(m_iTimestepMin);
	}
        std::cout << "iTimestepMin = " << m_iTimestepMin << std::endl;
    }
}

MODULE_MAIN(Reader, ReadVTK)
