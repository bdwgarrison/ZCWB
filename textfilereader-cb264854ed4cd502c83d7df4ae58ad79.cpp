#include "textfilereader.h"

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QLabel>
#include <iostream>

zwr::TextFileReader::TextFileReader()
{

    textForm = new QTextEdit;
    textForm->setWordWrapMode(QTextOption::NoWrap);
    textForm->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
    textForm->setContentsMargins(0,0,0,0);
    QSizePolicy policy = textForm->sizePolicy();
    policy.setVerticalStretch(1);
    textForm->setSizePolicy(policy);

    chartForm = new QTableWidget;
    chartForm->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    graphForm = new QCustomPlot;
    graphForm->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    graphForm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(chartForm, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(toggleGraphVisibility(int,int)));
    connect(graphForm, SIGNAL(selectionChangedByUser()),this,SLOT(selectionChanged()));
    connect(this,SIGNAL(graphIntSelected(int)),chartForm,SLOT(selectColumn(int)));
    //connect(chartForm,SIGNAL(cellClicked(int,int)),this,SLOT(highlightGraphInt(int,int)));

    QHBoxLayout* dataLay = new QHBoxLayout;
    dataLay->addWidget(textForm);
    dataLay->addWidget(chartForm);
    dataLay->addWidget(graphForm);
    dataLay->setContentsMargins(0,0,0,0);

    QHBoxLayout* buttonLay = new QHBoxLayout;
    QPushButton* textButton = new QPushButton("Input/Output");
    QPushButton* chartButton = new QPushButton("Table");
    QPushButton* graphButton = new QPushButton("Graph");
    buttonLay->addWidget(textButton);
    buttonLay->addWidget(chartButton);
    buttonLay->addWidget(graphButton);
    connect(textButton,SIGNAL(clicked()),this,SLOT( toggleTextVisibile()));
    connect(chartButton,SIGNAL(clicked()),this,SLOT( toggleChartVisibile()));
    connect(graphButton,SIGNAL(clicked()),this,SLOT( toggleGraphVisibile()));
    buttonLay->setContentsMargins(0,0,0,0);
    buttonLay->setAlignment(Qt::AlignTop);

    QGridLayout* gridLay = new QGridLayout;
    gridLay->addLayout(buttonLay,0,0);
    gridLay->addLayout(dataLay,1,0);
    gridLay->setContentsMargins(0,0,0,0);

    this->setLayout(gridLay);
    this->setContentsMargins(0,0,0,0);
}

void zwr::TextFileReader::clearData()
{
    textForm->clear();
    chartForm->clear();
    chartForm->setColumnCount(0);
    chartForm->setRowCount(0);
    graphForm->clearGraphs();
}


QStringList zwr::TextFileReader::dependentParametersUsed(QString& temp)
{
    int startKeyWord=0;
    int tempLength = temp.length();
    QStringList output;

    for( int k=0; k< tempLength; k++)
    {
        ///case: normal param
        if ( temp[k] == '\t' &&  k != startKeyWord )
        {
            output.append(QString(temp.mid(startKeyWord,k-startKeyWord)));
            startKeyWord=k+1;
        }
        ///case: last param
        else if ( k==tempLength-1 &&  k != startKeyWord )
        {
            output.append(QString(temp.mid(startKeyWord,k-startKeyWord+1)));
            startKeyWord=k+1;
        }
        ///case: empty param
        else if (temp[k] =='\t' && k == startKeyWord)
        {
            output.append(QString( '\t'));
            startKeyWord=k+1;
        }
    }
    return output;
}

void zwr::TextFileReader::uploadData(QString woName, QString input, DBStructure* wodb)
{
    int startKeyWord=0;
    int paramCount=1;
    int inputLength = input.length();
    int totalParamCount = depParams.size()+1;
    int rowCount = chartForm->rowCount();

    chartForm->setRowCount( rowCount+1 );
    for( int k=0; k< inputLength; k++)
    {
        ///first param is indep param => put in vertical axis and update string for data entry
        if ( input[k] == '\t' &&  startKeyWord == 0 /*&& k != 0 */)
        {
            if( !(input.left(k).isEmpty()))
                indepParam = input.left(k);

            chartForm->setVerticalHeaderItem( rowCount,new QTableWidgetItem(input.left(k)));
            //data[0].push_back( input.left(k).toDouble());
            data[0].push_back( formatIndepParam(input.left(k)));

            //std::cout<<input.left(k).toStdString()<<"\t"<<formatIndepParam(input.left(k))<<"\n";

            startKeyWord=k+1;
        }
        ///case: normal param
        else if ( input[k] == '\t' &&  k != startKeyWord )
        {
            wodb->addData(woName,
                         fileType,
                         depParams[paramCount-1],
                         indepParam,
                         QString(input.mid(startKeyWord,k-startKeyWord)));
            chartForm->setCellWidget( rowCount,paramCount-1,new QLabel(input.mid(startKeyWord,k-startKeyWord)));

            data[paramCount].push_back(input.mid(startKeyWord,k-startKeyWord).toDouble());
            startKeyWord=k+1;
            ++paramCount;
        }

        ///case: last param
        else if ( k==inputLength-1 &&  k != startKeyWord )
        {
            wodb->addData(woName,
                         fileType,
                         depParams[paramCount-1],
                         indepParam,
                         QString(input.mid(startKeyWord,k-startKeyWord+1)));
            chartForm->setCellWidget(rowCount,paramCount-1,new QLabel(input.mid(startKeyWord,k-startKeyWord+1)));

            data[paramCount].push_back( input.mid(startKeyWord,k-startKeyWord+1).toDouble());
            startKeyWord=k+1;
            ++paramCount;
        }
        ///case: empty param

        else if (input[k] =='\t' && k == startKeyWord)
        {
            chartForm->setCellWidget(rowCount,paramCount,new QLabel( "\t"));
            data[paramCount-1].push_back( 0);

            startKeyWord=k+1;
            ++paramCount;
        }
        ///case: line ends before all parameters are filled

        if (k == inputLength-1 && paramCount < totalParamCount)
        {
            while(paramCount < totalParamCount)
            {
                chartForm->setCellWidget(rowCount,paramCount,new QLabel( "\t"));
                data[paramCount-1].push_back( 0);

                ++paramCount;
            }
        }
    }
}

void zwr::TextFileReader::readFile(QString woName, DBStructure *wodb)
{
    clearData();
    woFileName = woName;
    //std::cout<<woName.toStdString()<<"\n"<<woFileName.toStdString()<<"\n"<<woFile.fileName().toStdString()<<"\n";

    //setFilePath();

    //std::cout<<woName.toStdString()<<"\n"<<woFileName.toStdString()<<"\n"<<woFile.fileName().toStdString()<<"\n";
    woFile.setFileName(woFileName + ".txt");
    std::cout<<woName.toStdString()<<"\n"<<woFileName.toStdString()<<"\n"<<woFile.fileName().toStdString()<<"\n";

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
        //int layerCount=0;
        indepParamCount = 0;
        indepParam = QString("");

        do
        {
            input = streamer.readLine();
            textForm->insertPlainText(input + "\n");

            if (haveHeader && (wodb->validIndependentParameter(input) || input[0] == '\t'))
            {
                uploadData(woName,input,wodb);
            }

            else if (!haveHeader && wodb->stringIsParameterHeader(input))
            {
                depParams = dependentParametersUsed(input);
                if (depParams.size() != 0)
                    depParams.pop_front();
                chartForm->setColumnCount( depParams.size());
                chartForm->setHorizontalHeaderLabels( depParams);
                //data.resize( depParams.size()+1 );
                data = QVector<QVector<double> >( depParams.size()+1);


                haveHeader=1;
            }

        }while (input.left(4) != "Time");

        textForm->insertPlainText( streamer.readAll() );
        //wodb->printWorkOrderParameters(woName);
    }
    woFile.close();
}


/*
void zwr::TextFileReader::uploadAndPrepGraphData(QString woName, QString temp, DBStructure* wodb)
{
    int startKeyWord=20;
    int paramCount=1;
    int tempLength = temp.length();
    int totalParamCount=depParams.size();

    QString sTimeStamp = temp.left(8);
    double timeStamp = temp.left(2).toDouble() * 10000 + temp.mid(3,2).toDouble() * 100 + temp.mid(6,2).toDouble();
    data[0].push_back( timeStamp);
    chartForm->setCellWidget( chartForm->rowCount()-1,0,new QLabel(sTimeStamp));

    /// 20 is first non time-stamp entry
    for( int k=20; k< tempLength; k++)
    {

        ///case: normal param
        if ( temp[k] == '\t' &&  k != startKeyWord )
        {
            wodb->addData(woName,
                         QString("HMI"),
                         depParams[paramCount],
                         sTimeStamp,
                         QString(temp.mid(startKeyWord,k-startKeyWord)));
            chartForm->setCellWidget( chartForm->rowCount()-1,paramCount,new QLabel(temp.mid(startKeyWord,k-startKeyWord)));
            data[paramCount].push_back(QString(temp.mid(startKeyWord,k-startKeyWord)).toDouble());
            startKeyWord=k+1;
            ++paramCount;
        }


        ///case: last param
        else if ( k==tempLength-1 &&  k != startKeyWord )
        {
            wodb->addData(woName,
                         QString("HMI"),
                         depParams[paramCount],
                         sTimeStamp,
                         QString(temp.mid(startKeyWord,k-startKeyWord+1)));
            chartForm->setCellWidget(chartForm->rowCount()-1,paramCount,new QLabel(temp.mid(startKeyWord,k-startKeyWord+1)));
            data[paramCount].push_back(QString(temp.mid(startKeyWord,k-startKeyWord+1)).toDouble());
            startKeyWord=k+1;
            ++paramCount;
        }
        ///case: empty param

        else if (temp[k] =='\t' && k == startKeyWord)
        {
            chartForm->setCellWidget(chartForm->rowCount()-1,paramCount,new QLabel( "\t"));
            data[paramCount].push_back( 0 );
            startKeyWord=k+1;
            ++paramCount;
        }
        ///case: line ends before all parameters are filled
        if (k == tempLength-1 && paramCount < totalParamCount)
        {
            while(paramCount < totalParamCount)
            {
                chartForm->setCellWidget(chartForm->rowCount()-1,paramCount,new QLabel( "\t"));
                data[paramCount].push_back( 0 );
                ++paramCount;
            }
        }
    }
}
*/
void zwr::TextFileReader::graphData()
{
    int primeSize = data.size();

    graphForm->clearGraphs();
    graphForm->legend->setVisible(true);
    QPen pen;
    for (int k=1; k<primeSize; k++)
    {
        graphForm->addGraph();
        graphForm->graph(k-1)->setData( data[0], data[k]);
        graphForm->graph(k-1)->rescaleAxes(true);
        graphForm->graph(k-1)->setName( depParams[k-1]);
        formatXAxis();
        //graphForm->xAxis->setTickLabelType(QCPAxis::ltDateTime);
        //graphForm->xAxis->setDateTimeFormat("hh\nmm\nss");

        pen.setColor(QColor(sin(k*0.3)*100+100, sin(k*0.6+0.7)*100+100, sin(k*0.4+0.6)*100+100));
        graphForm->graph(k-1)->setPen(pen);
    }
    graphForm->rescaleAxes(true);
    graphForm->update();
    this->update();
}



/*
void zwr::TextFileReader::writeTextToFile()
{
    woFile.resize(0);
    QTextStream fileEraser(&woFile);
    fileEraser << textForm->toPlainText();
}*/

void zwr::TextFileReader::toggleGraphVisibile()
{
    if (graphForm->isVisible()) graphForm->hide();
    else graphForm->setVisible(true);
}
void zwr::TextFileReader::toggleChartVisibile()
{
    if (chartForm->isVisible()) chartForm->hide();
    else chartForm->setVisible(true);
}
void zwr::TextFileReader::toggleTextVisibile()
{
    if (textForm->isVisible()) textForm->hide();
    else textForm->setVisible(true);
}
void zwr::TextFileReader::toggleGraphVisibility(int unused,int k)
{
    if (k < graphForm->graphCount())
    {
        bool b = graphForm->graph(k)->visible();
        if (b)
            graphForm->graph(k)->setVisible(0);
        else
            graphForm->graph(k)->setVisible(1);
        graphForm->rescaleAxes(1);
        graphForm->update();
    }
    //graphForm->graph(k)->set
}

double zwr::TextFileReader::formatIndepParam(QString inParam)
{
    return inParam.toDouble();
}

void zwr::TextFileReader::selectionChanged()
{
    int graphCount = graphForm->graphCount();
    for (int i=0; i< graphCount; ++i)
    {
      QCPGraph *graph = graphForm->graph(i);
      QCPPlottableLegendItem *item = graphForm->legend->itemWithPlottable(graph);
      if (item->selected() || graph->selected())
      {
        item->setSelected(true);
        graph->setSelected(true);
        emit graphIntSelected(i);
      }
    }
}

void zwr::TextFileReader::highlightGraphInt(int unused,int k)
{
    graphForm->graph(k)->setSelected(true);
}

void zwr::TextFileReader::mathematicaOutput()
{
    int specRange = data[0].size()-1;
    QString workOrder("Set");
    QString woSet("X" + workOrder +" = {");

    QTextEdit* output = new QTextEdit;
    output->insertPlainText( "ClearAll[\"Global`*\"] \n");
    output->insertPlainText("minSetWav=" + QString::number(data[0][0]) +
                            ";\nmaxSetWav=" + QString::number(data[0][specRange]) +
                            ";\nsetSteps=" + QString::number(specRange+1) + ";\n");
    ///define header
    QString headerString("HSet = { \"\" ");
    int depParamSize = depParams.size();
    for (int k=0; k<depParamSize; ++k){
        headerString+= ", \"" + depParams[k] + "\"";
    }
    headerString += "};";
    output->insertPlainText(headerString + "\n");

///need to handle columns with differing size
    for(int i=1;i<data.size();++i){
        woSet += QString("x" + workOrder + depParams[i-1]);
        if (i<data.size()-1)
            woSet += QString(", ");
        output->insertPlainText("x" + workOrder + depParams[i-1] +"={");
        for(int k=0;k< specRange;++k){
            output->insertPlainText( QString::number(data[i][k]) + ",\n");
        }
        output->insertPlainText(QString::number(data[i][specRange]) + "};\n\n");
    }
    woSet += "};";
    output->insertPlainText( woSet + "\n\n");
    QFile MathematicaSrc(":/MathematicaSrc2000.txt");
    MathematicaSrc.open(QFile::ReadOnly | QFile::Text);
    output->insertPlainText( QTextStream( &MathematicaSrc).readAll());

    ///might need to use
    QString tempPath = dbPath + "/" + woFileName + "/" + woFileName + "-Spectral-Corrected.tsv";
    output->insertPlainText( "\nExport[\"" + tempPath +"\", Grid[TconvXSet]];");
    output->show();
}

void zwr::TextFileReader::tfCalcOutput()
{
    //int depParamSize = depParams.size();
    int colCount = data.size();
    for (int k=1; k<colCount; ++k){
        int rowCount = data[k].size();
        QTextEdit* output = new QTextEdit;
        output->setWindowTitle( depParams[k-1]);
        output->setMinimumHeight(650);
        output->setMinimumWidth(350);
        output->setLineWrapMode(QTextEdit::NoWrap);
        for( int j=0; j<rowCount; ++j)
            output->insertPlainText( QString::number(data[0][j]) +"\t"+ QString::number(data[k][j])+"\n");
        output->show();
    }

}

void zwr::TextFileReader::interpOutput()
{
    QTextEdit* output = new QTextEdit;
    int rowCountButOne = data[0].size()-1;

    output->insertPlainText("{");
    for( int k=0; k<rowCountButOne; ++k)
        output->insertPlainText( "{{" + QString::number(data[0][k]) + ","
                                + QString::number(data[1][k]) + "},"
                                + QString::number(data[2][k]) + "},\n");

    output->insertPlainText("{{" + QString::number(data[0][rowCountButOne]) + ","
            + QString::number(data[1][rowCountButOne]) + "},"
            + QString::number(data[2][rowCountButOne]) + "}};\n\n");
    output->show();
}

void zwr::TextFileReader::setFilePath()
{
    woFile.setFileName(woFileName + ".txt");
}
