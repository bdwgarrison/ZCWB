#include "rgafilereader.h"
#include <sstream>
#include <iostream>

zwr::RGAFileReader::RGAFileReader()
{
    separator=" ";
    fileType = "RGA";
    addHideButton();
    addUploadButton();
    addFormatButton();
    addSetxIntervalButton();
    addSetyIntervalButton();
    desiredHeaderList << "Hydrogen" <<"Nitrogen"<<"Oxygen"<<"Water"<< "Argon";
}

void zwr::RGAFileReader::setFilePath()
{
    woFile.setFileName(woFileName + "-RGA.txt");
}

double zwr::RGAFileReader::formatIndepParam(QString inParam)
{
    return inParam.toDouble();
}

void zwr::RGAFileReader::trimTop(QString& input, QTextStream& streamer)
{
    input = streamer.readLine();
    depParams = dependentParametersUsed(input);
    if (depParams.size() != 0)
        depParams.pop_front();
    return;
}

void zwr::RGAFileReader::formatXAxis()
{
    graphForm->yAxis->setLabel("Pressure (Torr)");

    return;
}

void zwr::RGAFileReader::setBaseTime(QString input)
{
    baseTime = input.left(2).toDouble()+input.mid(3,2).toDouble()/60 +input.mid(6,2).toDouble()/3600;
    if (input.mid(9,2) == "PM" && input.left(2).toDouble() != 12)
        baseTime += 12;
    return;
}

void zwr::RGAFileReader::locateColumnsForDesiredHeader(QString& input, QTextStream& streamer)
{
    while (input.left(3) != "Gas")
    {
        input = streamer.readLine();
    }
    std::string tempInput;
    QStringList tempHeader;

    ///go through top of file, find Gas and extract the names of all column headers
    while (input.left(3) == "Gas")
    {
        std::stringstream tempStream( input.toStdString());
        tempStream >> tempInput;
        tempStream >> tempInput;
        if (tempHeader.size() < 9)
            tempStream >> tempInput;

        tempHeader.push_back( QString(tempInput.c_str()));

        input = streamer.readLine();
    }

    int fullHeaderCount = tempHeader.size();
    int desiredHeaderCount = desiredHeaderList.size();
    ///check to see if desired header strings are in the obtained header strings and mark the column number
    for(int k=0;k<desiredHeaderCount;++k){
        QString checkForThisString = desiredHeaderList[k];
        for(int j=0;j<fullHeaderCount;++j){
            if( checkForThisString == tempHeader[j]){
                desiredHeaderListNumbers.push_back(j+1);
                break;
            }
        }
    }

    input = streamer.readLine();
    input = streamer.readLine();
    input = streamer.readLine();

    data = QVector<QVector<double> >( desiredHeaderCount+1);
    depParams = desiredHeaderList;

    while (!input.isNull()){
        QStringList tempLineData = input.split(separator,QString::SkipEmptyParts);

        data[0].push_back( baseTime + tempLineData[0].toDouble()/3600 );
        for(int k=0;k<desiredHeaderCount;++k){
            data[k+1].push_back( tempLineData[ desiredHeaderListNumbers[k] ].toDouble() );
        }

        input = streamer.readLine();
    }
    addSumColumnToData();


}

void zwr::RGAFileReader::formatFile()
{

    QStringList fileNameList = textForm->getFileNameList();
    QFile file(fileNameList[0]);
    if ( file.open(QFile::ReadOnly | QFile::Text) )

    {
        QTextStream streamer(&file);
        QString input = streamer.readLine();
        setBaseTime(input);

        locateColumnsForDesiredHeader(input, streamer);
        writeDataToFile();
    }
    file.close();

}

void zwr::RGAFileReader::writeTextToFile()
{
    woFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
    woFile.resize(0);
    QTextStream fileEraser(&woFile);
    fileEraser << textForm->toPlainText();
    woFile.close();
    readData(woFileName,WODB);
}





