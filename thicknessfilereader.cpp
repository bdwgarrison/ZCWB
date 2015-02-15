#include "thicknessfilereader.h"
#include <QPushButton>

zwr::ThicknessFileReader::ThicknessFileReader()
{
    fileType = "Thickness";
    QPushButton* saveButton = new QPushButton("Upload");
    saveButton->setMaximumWidth(75);
    statLabel = new QLabel;
    this->layout()->addWidget(statLabel);
    this->layout()->addWidget(saveButton);
    connect(saveButton,SIGNAL(clicked()),this,SLOT(writeTextToFile()));
    //connect(textForm, SIGNAL(textChanged()), this, SLOT(writeTextToFile()));
}

void zwr::ThicknessFileReader::writeTextToFile()
{
    woFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
    woFile.resize(0);
    QTextStream fileEraser(&woFile);
    fileEraser << textForm->toPlainText();
    woFile.close();
    readThicknessFile(woFileName,WODB);
}

void zwr::ThicknessFileReader::setFilePath()
{
    woFile.setFileName(woFileName + "-Thickness.txt");
}

void zwr::ThicknessFileReader::readThicknessFile(QString woName, DBStructure *wodb)
{
    woFileName = woName;
    WODB = wodb;
    textForm->clear();

    woFile.setFileName( woFileName + "-Thickness.txt");
    if ( woFile.open(QFile::ReadOnly | QFile::Text) )
    {
        QTextStream streamer(&woFile);
        QString input;
        data = QVector<QVector<double> >(2);
        depParams << "Position" << "Step";
        double avg=0;
        do{
            input = streamer.readLine();
            textForm->insertPlainText(input + "\n");
            int inputLength = input.size();
            int tabLocation=0;
            ///need to fix qualifier
            while ( input[tabLocation] != '\t' && tabLocation<50)
                ++tabLocation;

            QString position = input.left(tabLocation);
            QString step = input.right(inputLength - tabLocation-1);
            if (step.toDouble())
            {
                data[0].push_back(position.toDouble());
                data[1].push_back(step.toDouble());
                avg+=data[1].last();
                wodb->addData(woName,"Thickness","Position","Step",step);
            }

        }while (!input.isNull());


        int dataCount = data[1].size();
        avg/=dataCount;
        double sdev=0;
        for (int k=0; k<dataCount; ++k)
            sdev += (data[1][k] - avg)*(data[1][k] - avg);
        sdev = qSqrt(sdev/dataCount);
        statLabel->setText("Average: " + QString::number(avg) + "\nSdev: " + QString::number(sdev));

        graphForm->clearGraphs();
        graphForm->xAxis->setLabel("Radial Position (cm)");
        graphForm->yAxis->setLabel("Thickness (Angstroms)");
        graphForm->addGraph();
        graphForm->graph(0)->setData(data[0],data[1]);
        graphForm->graph(0)->setLineStyle(QCPGraph::lsNone);

        graphForm->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
        graphForm->rescaleAxes();
        graphForm->update();

        this->update();
    }
    woFile.close();
}

void zwr::ThicknessFileReader::trimTop(QString&, QTextStream&)
{
    return;
}
