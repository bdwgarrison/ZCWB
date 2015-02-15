#include "pyrometerfilereader.h"
#include <iostream>

zwr::PyrometerFileReader::PyrometerFileReader()
{
    addFormatButton();
    addUploadButton();
    addConvertTimeStampButton();
}

void zwr::PyrometerFileReader::setFilePath()
{
    woFile.setFileName( woFileName + "-Pyrometer.txt");
}

void zwr::PyrometerFileReader::formatXAxis()
{
    graphForm->xAxis->setLabel("Time of Day (hrs)");
    //graphForm->yAxis->setLabel("Temperature (C)");
    graphForm->yAxis->setLabel("mm/s or A/s or dm");
}

double zwr::PyrometerFileReader::formatIndepParam(QString inParam)
{
    //return baseTime + inParam.left(3).toDouble() + inParam.mid(5,2).toDouble() / 60 + inParam.mid(8,2).toDouble() / 3600;
    return inParam.toDouble();
}


void zwr::PyrometerFileReader::trimTop(QString& input, QTextStream& streamer)
{
    input = streamer.readLine();
    depParams = dependentParametersUsed(input);

    if (depParams.size() != 0)
        depParams.pop_front();
    return;
}

void zwr::PyrometerFileReader::writeTextToFile()
{
    woFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
    woFile.resize(0);
    QTextStream fileEraser(&woFile);
    fileEraser << textForm->toPlainText();
    woFile.close();
    readData(woFileName,WODB);
}

void zwr::PyrometerFileReader::formatFile()
{
    QStringList fileNameList = textForm->getFileNameList();
    QFile file(fileNameList[0]);
    if ( file.open(QFile::ReadOnly | QFile::Text) )
    {
        QTextStream streamer(&file);
        QString input = streamer.readLine();
        input = streamer.readLine();
        input = streamer.readLine();

        //std::cout<< input.toStdString()<<"\n";
        baseTime = input.mid(6,2).toDouble() + input.mid(9,2).toDouble()/60 + input.mid(12,2).toDouble()/3600;
        //std::cout<<baseTime<<"\n";
        input = streamer.readLine();
        while (input.left(4) != "Time"){
            input = streamer.readLine();
        }
        input = streamer.readLine();

        QStringList tempHeader;
        tempHeader << "Temperature";

        QVector<QVector<double> > tempData( tempHeader.size() + 1 );

        while (!input.isNull()){
            std::stringstream tempStream(input.toStdString());
            std::string tempString;
            tempData[0].push_back(
                        baseTime + input.left(3).toDouble() + input.mid(4,2).toDouble() / 60 + input.mid(7,2).toDouble() / 3600
                        );
            tempStream >> tempString;
            tempStream >> tempString;
            tempStream >> tempString;
            tempStream >> tempString;
            tempStream >> tempString;
            tempData[1].push_back( QString( tempString.c_str()).toDouble() );
            if (tempData[1].last() == 0){
                tempData[0].pop_back();
                tempData[1].pop_back();
            }
            input = streamer.readLine();
        }


        textForm->clear();

        textForm->insertPlainText( "\t" + tempHeader[0] + "\n");

        int rowCount = tempData[0].size()-1;
        for(int j=0; j<rowCount; ++j)
            textForm->insertPlainText(
                        QString::number(tempData[0][j]) + "\t" + QString::number(tempData[1][j]) + "\n"
                    );
        textForm->insertPlainText(
                    QString::number(tempData[0][rowCount]) + "\t" + QString::number(tempData[1][rowCount])
                );
    }
    file.close();
}
