#include "workorderdatum.h"
#include <iostream>

zwr::WorkOrderDatum::WorkOrderDatum(const WorkOrderDatum& w)
{
    index1_FullWorkOrderNum_ = w.index1_FullWorkOrderNum_;
    index2_MatrixType_ = w.index2_MatrixType_;
    index3_DependentData_ = w.index3_DependentData_;
    index4_IndependentData_ = w.index4_IndependentData_;
    index5_Datum_ = w.index5_Datum_;
}

zwr::WorkOrderDatum& zwr::WorkOrderDatum::operator= (const WorkOrderDatum& w)
{
    index1_FullWorkOrderNum_ = w.index1_FullWorkOrderNum_;
    index2_MatrixType_ = w.index2_MatrixType_;
    index3_DependentData_ = w.index3_DependentData_;
    index4_IndependentData_ = w.index4_IndependentData_;
    index5_Datum_ = w.index5_Datum_;
    return *this;
}

void zwr::WorkOrderDatum::printAll() const
{
    std::cout<< index1_FullWorkOrderNum_<<"\t"<< index2_MatrixType_<<"\t"<< index3_DependentData_<<"\t"<<
                index4_IndependentData_<<"\t"<< index5_Datum_<<"\n";
}


zwr::DBStructure::DBStructure()
{

    validWorkOrderParameterNames.append("Layer");
    validWorkOrderParameterNames.append("Pocket");
    validWorkOrderParameterNames.append("Shutter");
    validWorkOrderParameterNames.append("Base Material");
    validWorkOrderParameterNames.append("Final Material");
    validWorkOrderParameterNames.append("Layer Thickness (A)");
    validWorkOrderParameterNames.append("Density");
    validWorkOrderParameterNames.append("Z-Ratio");
    validWorkOrderParameterNames.append("IAD Parameters");
    validWorkOrderParameterNames.append("Radius Start");
    validWorkOrderParameterNames.append("Track Zero Input Angstroms");
    validWorkOrderParameterNames.append("Total Thickness On Crystal");
    validWorkOrderParameterNames.append("Rates");
    validWorkOrderParameterNames.append("Gain");

    int validParameterNamesCount = validWorkOrderParameterNames.size();
    for (int k=0; k< validParameterNamesCount; k++)
        workOrderParameterMap[validWorkOrderParameterNames[k].toStdString()] = k+1;
     /*
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Layer","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Pocket","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Shutter","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Base Material","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Final Material","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Layer Thickness (A)","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Density","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Z-Ratio","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","IAD Parameters","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Radius Start","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Track Zero Input Angstroms","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Total Thickness On Crystal","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Rates","",""));
    data.insert(WorkOrderDatum("DependentParameters","WorkOrder","Gain","",""));
    */
}


void zwr::DBStructure::addData(QString s1, QString s2, QString s3, QString s4, QString s5)
{
    //if(
        //(
    data.insert(WorkOrderDatum(s1.toStdString(), s2.toStdString(),
                                    s3.toStdString(), s4.toStdString(), s5.toStdString()
                                    )
                     );
         //).second
    //) return;
        //WorkOrderDatum(s1.toStdString(), s2.toStdString(), s3.toStdString(), s4.toStdString(), s5.toStdString()).printAll();
}


bool zwr::DBStructure::validDependentParameter(QString inParam)
{
    return (workOrderParameterMap[inParam.toStdString()] > 0 &&
            workOrderParameterMap[inParam.toStdString()] < validWorkOrderParameterNames.size()+1);
    //return (data.get<index3_DependentData>().find( inParam.toStdString() ) != data.get<index3_DependentData>().end() );
}

bool zwr::DBStructure::validIndependentParameter(QString input)
{
    return input.left(2).toInt();
}

bool zwr::DBStructure::stringIsParameterHeader(QString inString)
{

    int parameterCount = validWorkOrderParameterNames.size();
    //int validParameterNamesSize =
    int stringSize = inString.size();
    if ( parameterCount != 0)
    {
        for (int k=0; k<parameterCount; k++)
        {
            ///first check if string is large enough to form the substring in question, lest the program crash
            int comparedSize = validWorkOrderParameterNames[k].size();
            if (stringSize >= comparedSize)
                //if (inString.left (validWorkOrderParameterNames[k].size()) == validWorkOrderParameterNames[k])
                if (inString.left (comparedSize) == validWorkOrderParameterNames[k])
                    return true;
        }
    }
    else return true;
    return false;


}
/*
{
    int startKeyWord=0;
    int tempLength = temp.length();

    for( int k=0; k< tempLength; k++)
    {
        ///case: normal param
        if ( temp[k] == '\t' &&  k != startKeyWord )
        {
            if (validDependentParameter(temp.mid(startKeyWord,k-startKeyWord)))
                return false;
            //output.append(QString(temp.mid(startKeyWord,k-startKeyWord)));
            else
                startKeyWord=k+1;
        }
        ///case: last param
        else if ( k==tempLength-1 &&  k != startKeyWord )
        {
            //output.append(QString(temp.mid(startKeyWord,k-startKeyWord+1)));
            if (!validDependentParameter(temp.mid(startKeyWord,k-startKeyWord+1)))
                return false;
            else
                startKeyWord=k+1;
        }
        ///case: empty param
        else if (temp[k] =='\t' && k == startKeyWord)
        {
            startKeyWord=k+1;
            //return false;
        }
        ///case: empty line
        else if (temp[k]=='\t' && k == tempLength-1)
            return false;
    }
    return true;
}*/


void zwr::DBStructure::printWorkOrderParameters(QString woNum) const
{
    std::pair<wodb_indexed1_iterator_t,wodb_indexed1_iterator_t> wo_start_end =
            data.get<index1_FullWorkOrderNum>().equal_range( woNum.toStdString());

    wodb_indexed1_iterator_t wo_itr = wo_start_end.first;
    wodb_indexed1_iterator_t wo_end = wo_start_end.second;

    while (wo_itr != wo_end)
    {
        wo_itr->printAll();
        ++wo_itr;
    }
}
