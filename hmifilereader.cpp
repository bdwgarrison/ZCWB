#include "hmifilereader.h"
#include <iostream>
zwr::HMIFileReader::HMIFileReader()
{
}
zwr::HMIFileReader::HMIFileReader(QString hmiType)
{
    fileType = hmiType;
    addHideButton();
}

void zwr::HMIFileReader::setFilePath()
{
    woFile.setFileName( woFileName + "-" + fileType + ".txt");
}


double zwr::HMIFileReader::formatIndepParam(QString inParam)
{
    return inParam.left(2).toDouble() + inParam.mid(3,2).toDouble() / 60 + inParam.mid(6,2).toDouble() / 3600;
}

void zwr::HMIFileReader::formatXAxis()
{
    //graphForm->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    //graphForm->xAxis->setDateTimeFormat("hh:\nmm:\nss");
    //graphForm->axisRect()->setupFullAxesBox();
    /*
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(2);
    customPlot->axisRect()->setupFullAxesBox();
    */
    return;
}

void zwr::HMIFileReader::trimTop(QString& input, QTextStream& streamer)
{
    input = streamer.readLine();
    depParams = dependentParametersUsed(input);
    if (depParams.size() != 0)
        depParams.pop_front();
    return;
}
/*
void zwr::HMIFileReader::formatFile()
{

}
*/


