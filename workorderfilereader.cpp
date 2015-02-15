#include "workorderfilereader.h"

zwr::WorkOrderFileReader::WorkOrderFileReader()
{
    fileType = "WorkOrder";
    setChartVisible(true);
}

void zwr::WorkOrderFileReader::setFilePath()
{
    woFile.setFileName(woFileName + ".txt");
}

void zwr::WorkOrderFileReader::readWOFile(QString woName, DBStructure *wodb)
{
    textForm->clear();

    setFilePath();
    if ( woFile.open(QFile::ReadOnly | QFile::Text) )
    {
        QTextStream streamer(&woFile);
        QString input;
        do
        {
            input = streamer.readLine();
            textForm->insertPlainText(input + "\n");
        }while (input.left(17) != "Run Instructions:");

        bool haveHeader = 0;
        int layerCount=0;

        do
        {
            input = streamer.readLine();
            textForm->insertPlainText(input + "\n");

            if (haveHeader && (wodb->validIndependentParameter(input) || input[0] == '\t'))
            {
                if (input.left(2).toInt())
                    ++layerCount;

                //chartForm->setRowCount( chartForm->rowCount() +1 );
                //uploadData(woName,"WorkOrder",input,wodb,QString::number(layerCount));
            }

            else if (!haveHeader && wodb->stringIsParameterHeader(input))
            {
                depParams = dependentParametersUsed(input);
                ///just added

                data = QVector<QVector<double> >( depParams.size()+1);


                //chartForm->verticalHeader()->hide();
                haveHeader=1;
            }

        }while (input.left(4) != "Time");
        createTable();
        textForm->insertPlainText( streamer.readAll() );

    }
    //wodb->printWorkOrderParameters(woName);
    woFile.close();
}

void zwr::WorkOrderFileReader::trimTop(QString&, QTextStream&)
{
    return;
}
