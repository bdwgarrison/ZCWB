#include "textfilereader.h"

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QLabel>
#include <iostream>

zwr::TextFileReader::TextFileReader()
{
    particularOptions = new QMenu("Particular Options");
    separator="\t";
    baseTime=0;
    //textForm = new QTextEdit;
    textForm = new dropBox;
    textForm->setWordWrapMode(QTextOption::NoWrap);
    textForm->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
    textForm->setContentsMargins(0,0,0,0);
    QSizePolicy policy = textForm->sizePolicy();
    policy.setVerticalStretch(1);
    textForm->setSizePolicy(policy);

    chartForm = new QTableWidget;
    chartForm->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    //toggleChartVisibile();
    setChartVisible(false);

    tableForm = new QTableView;
    tableForm->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);


    graphForm = new QCustomPlot;
    graphForm->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    graphForm->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    graphForm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    graphForm->setPlottingHint( QCP::phForceRepaint);

    //connect(chartForm, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(toggleGraphVisibility(int,int)));
    //connect(tableForm, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(toggleGraphVisibility(int,int)));
    connect(graphForm, SIGNAL(selectionChangedByUser()),this,SLOT(selectionChanged()));
    //connect(this,SIGNAL(graphIntSelected(int)),chartForm,SLOT(selectColumn(int)));
    connect(this,SIGNAL(graphIntSelected(int)),tableForm,SLOT(selectColumn(int)));


    menuButtons = new QHBoxLayout;
    QHBoxLayout* dataLay = new QHBoxLayout;
    dataLay->addWidget(textForm);
    dataLay->addWidget(chartForm);
    dataLay->addWidget(tableForm);
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
    connect(chartButton,SIGNAL(clicked()),this,SLOT( toggleTableVisibile()));
    connect(graphButton,SIGNAL(clicked()),this,SLOT( toggleGraphVisibile()));
    buttonLay->setContentsMargins(0,0,0,0);
    buttonLay->setAlignment(Qt::AlignTop);

    QGridLayout* gridLay = new QGridLayout;
    gridLay->addLayout(buttonLay,0,0);
    gridLay->addLayout(dataLay,1,0);
    gridLay->addLayout(menuButtons,2,0);
    gridLay->setContentsMargins(0,0,0,0);

    this->setLayout(gridLay);
    this->setContentsMargins(0,0,0,0);
}

void zwr::TextFileReader::clearData()
{
    textForm->clear();
    graphForm->clearGraphs();
}

/**
 private variable map for valid header names
    -change the variable name and rest follow
    =>have to do some scheme for data in desired column: need to overlook others

 hardcode which position
    -change all each time

  find number of lines first to avoid vector.push_back reallocation
 */

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
            if( !(input.left(k).isEmpty()) )
                indepParam = input.left(k);

            chartForm->setVerticalHeaderItem( rowCount,new QTableWidgetItem(input.left(k)));
            data[0].push_back( formatIndepParam(input.left(k)));

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

void zwr::TextFileReader::uploadDataQuickly(QString woName, QString input, DBStructure* wodb)
{
    int startKeyWord=0;
    int paramCount=1;
    int inputLength = input.length();
    int totalParamCount = depParams.size()+1;

    for( int k=0; k< inputLength; k++)
    {
        ///first param is indep param => put in vertical axis and update string for data entry
        if ( input[k] == '\t'
             // || input.mid(k,3) == "   " )
             &&  startKeyWord == 0 )
        {
            if( !(input.left(k).isEmpty()) )
                indepParam = input.left(k);

            data[0].push_back(formatIndepParam(input.left(k)));

            startKeyWord=k+1;
        }
        ///case: normal param
        else if ( input[k] == '\t'
                  // || input.mid(k,3) == "   " )
                 &&  k != startKeyWord )
        {
            wodb->addData(woName,
                         fileType,
                         depParams[paramCount-1],
                         indepParam,
                         QString(input.mid(startKeyWord,k-startKeyWord)));

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

            data[paramCount].push_back(input.mid(startKeyWord,k-startKeyWord+1).toDouble());
            startKeyWord=k+1;
            ++paramCount;
        }
        ///case: empty param

        else if ( ( input[k] =='\t'
                   ///single char for empty param
                   || input[k] == '\ ' )
                && k == startKeyWord)
        {
            //data[paramCount-1].push_back( 0);

            startKeyWord=k+1;

            ///might need to put this back in to allow for empty WOparams,
            /// but for data it is not necessary
            //++paramCount;
        }
        ///case: line ends before all parameters are filled

        if (k == inputLength-1 && paramCount < totalParamCount)
        {
            while(paramCount < totalParamCount)
            {
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

    setFilePath();

    if ( woFile.open(QFile::ReadOnly | QFile::Text) )
    {
        chartForm->clear();
        chartForm->setRowCount(0);
        chartForm->setColumnCount(0);
        QTextStream streamer(&woFile);
        QString input;

        do
        {
            input = streamer.readLine();
            textForm->insertPlainText(input + "\n");
        }while (input.left(17) != "Run Instructions:");

        bool haveHeader = 0;
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
void zwr::TextFileReader::writeTextToFile()
{
    woFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
    woFile.resize(0);
    QTextStream fileEraser(&woFile);
    fileEraser << textForm->toPlainText();
    woFile.close();
    readData(woFileName,WODB);
}*/
void zwr::TextFileReader::writeDataToFile()
{
    woFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
    woFile.resize(0);
    QTextStream fileEraser(&woFile);
    int rowCount = data[0].size();
    int colCount = data.size();

    for(int k=0;k<colCount-1;++k)
        fileEraser<< "\t" + depParams[k];

    fileEraser<< "\n";
    for(int k=0;k<rowCount;++k){
        fileEraser << QString::number(data[0][k]);

        for(int j=1;j<colCount;++j){
            fileEraser << "\t" + QString::number(data[j][k]);
        }
        fileEraser << "\n";
    }
    woFile.close();
    readData(woFileName,WODB);
}

void zwr::TextFileReader::toggleGraphVisibile()
{
    if (graphForm->isVisible()) graphForm->hide();
    else graphForm->setVisible(true);
}
void zwr::TextFileReader::toggleTableVisibile()
{
    if (tableForm->isVisible()) tableForm->hide();
    else tableForm->setVisible(true);
}

void zwr::TextFileReader::toggleChartVisible()
{
    if (chartForm->isVisible()) chartForm->hide();
    else chartForm->setVisible(true);
}
void zwr::TextFileReader::toggleTextVisibile()
{
    if (textForm->isVisible()) textForm->hide();
    else textForm->setVisible(true);
}

double zwr::TextFileReader::formatIndepParam(QString inParam)
{
    return inParam.toDouble();
}

void zwr::TextFileReader::toggleCurveVisibility(int k)
{
    if (k < graphForm->graphCount())
    {
        bool b = graphForm->graph(k)->visible();
        if (b){
            graphForm->graph(k)->setVisible(0);
            graphForm->graph(k)->removeFromLegend();
        }
        else{
            graphForm->graph(k)->setVisible(1);
            graphForm->graph(k)->addToLegend();
        }
        graphForm->rescaleAxes(1);
        graphForm->replot();
    }
}

///when user clicks on legend item, highlight corresponding curve and vice versa
void zwr::TextFileReader::selectionChanged()
{
    int graphCount = graphForm->graphCount();
    int legendNum = 0;
    for (int i=0; i< graphCount; ++i)
    {
      if (graphForm->graph(i)->visible()){
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
}

void zwr::TextFileReader::highlightGraphInt(int unused,int k)
{

    if( k < graphForm->graphCount() )
    //if( k < graphForm->legend->itemCount())
        if (graphForm->graph(k)->visible())
            graphForm->graph(k)->setSelected(true);
    else return;
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
    QString tempPath = woFileName + "-Spectral-Corrected.tsv";
    output->insertPlainText( "\nExport[\"" + tempPath +"\", Grid[TconvXSet]];");
    output->show();
}

void zwr::TextFileReader::tfCalcOutput()
{
    int colCount = data.size();
    QString tfPath(woFileName.left( woFileName.size()-12) + "tfcalc");
    QDir newDir;
    newDir.mkdir( tfPath );
    QFile tempFile;
    for (int k=1; k<colCount; ++k){
        int rowCount = data[k].size();
        tempFile.setFileName( tfPath + "/" + depParams[k-1] + ".txt");
        tempFile.open(QFile::ReadWrite | QFile::Truncate | QFile::Text);
        tempFile.resize(0);
        QTextStream tempStream(&tempFile);
        for( int j=0; j<rowCount; ++j)
            //tempStream << ( QString::number(data[0][j]) +"\t"+ QString::number(data[k][j])+"\n");
            tempStream << ( QString::number(data[0][j]) +"\t"+ QString::number(data[k][j])+"\r\n");
        tempFile.close();
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

void zwr::TextFileReader::createChart()
{
    int colCount = data.size()-1;
    int rowCount = data[0].size();

    chartForm->clear();
    chartForm->setRowCount(rowCount);
    chartForm->setColumnCount(colCount);

    chartForm->setHorizontalHeaderLabels( depParams );
    QStringList indepParams;
    for( int k=0; k<rowCount; ++k){
        indepParams << QString::number(data[0][k]);
    }

    chartForm->setVerticalHeaderLabels(indepParams);

    for( int k=1; k<colCount; ++k)
        for( int j=0; j<rowCount; ++j){
            chartForm->setCellWidget(j,k,new QLabel( QString::number(data[k][j]) ));
        }
    chartForm->update();
}

void zwr::TextFileReader::createTable()
{
    tableFormModel = new chartDataView( data[0].size(), data.size()-1, &data, &depParams);
    tableForm->setModel( tableFormModel );
}
void zwr::TextFileReader::createStringTable()
{
    stringTableFormModel = new stringDataView( noteData[0].size(), noteData.size()-1, &noteData, &noteDepParams);
    tableForm->setModel( stringTableFormModel );
}

void zwr::TextFileReader::smoothOutput()
{
    double kernel[11]={0.000259309, 0.00328418, 0.0236599, 0.0969558, 0.226001, 0.299656, 0.226001, 0.0969558, 0.0236599, 0.00328418, 0.000259309};
    int kerInc=5;
    int maxCount = data[0].size()-kerInc;
    int colCount = data.size();
    QVector<QVector<double> > oldData = data;

    for(int j=0;j<colCount;++j){
        for(int k= kerInc; k<maxCount; ++k ){
            data[j][k] = oldData[j][k-5]*kernel[0] +
                     oldData[j][k-4]*kernel[1] +
                     oldData[j][k-3]*kernel[2] +
                     oldData[j][k-2]*kernel[3] +
                     oldData[j][k-1]*kernel[4] +
                     oldData[j][k]*kernel[5] +
                     oldData[j][k+1]*kernel[6] +
                     oldData[j][k+2]*kernel[7] +
                    oldData[j][k+3]*kernel[8] +
                    oldData[j][k+4]*kernel[9] +
                    oldData[j][k+5]*kernel[10];
        }
    }
    createTable();
    graphData();
}

void zwr::TextFileReader::mathematicaPlotOutput()
{
    QTextEdit* output = new QTextEdit;
    int rowCountButOne = data[0].size()-1;
    int colCount = data.size();
    for(int j=1; j<colCount; ++j){

        output->insertPlainText(depParams[j-1] + "={");
        for( int k=0; k<rowCountButOne; ++k)
          output->insertPlainText( "{" + QString::number(data[0][k]) + ","
                                  + QString::number(data[j][k]) + "},\n");

        output->insertPlainText("{" + QString::number(data[0][rowCountButOne]) + ","
             + QString::number(data[j][rowCountButOne]) + "}};\n\n");
    }
    output->show();
}



void zwr::TextFileReader::readData(QString woName, DBStructure* wodb)
{
    WODB = wodb;
    textForm->clear();
    woFileName = woName;

    setFilePath();

    if ( woFile.open(QFile::ReadOnly | QFile::Text) )
    {
        QTextStream streamer(&woFile);
        QString input;

        trimTop(input, streamer);

        data = QVector<QVector<double> >( depParams.size()+1);
        input = streamer.readLine();
        while (!input.isNull())
        {
            //uploadDataQuickly(woName,input,wodb);
            parseFormattedData(input);
            input = streamer.readLine();
        }
        createTable();
        graphData();
    }
    woFile.close();
}

void zwr::TextFileReader::trimTop(QString&, QTextStream&)
{
    return;
}
void zwr::TextFileReader::cropTop(QString& input, QTextStream& stream)
{
    if (!decapitator.isNull() && !decapitator.isEmpty()){
        while(input != decapitator){
            input = stream.readLine();
        }
    }
    return;
}

void zwr::TextFileReader::addInterpOutputButton()
{
    //QPushButton* button = new QPushButton("Interp Format");
    //menuButtons->addWidget(button);
    //connect(button,SIGNAL(clicked()),this,SLOT(writeTextToFile()));

    QAction* action = new QAction(tr("Format for Interpolation"), this);
    particularOptions->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(interpOutput()));
}
void zwr::TextFileReader::addUploadButton()
{/*
    QPushButton* button = new QPushButton("Upload");
    menuButtons->addWidget(button);
    connect(button,SIGNAL(clicked()),this,SLOT(writeTextToFile()));
    */
    QAction* action = new QAction(tr("Upload"), this);
    particularOptions->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(writeTextToFile()));
}
void zwr::TextFileReader::addFormatButton()
{
    /*
    QPushButton* button = new QPushButton("Format");
    menuButtons->addWidget(button);
    connect(button,SIGNAL(clicked()),this,SLOT(formatFile()));
    */
    QAction* action = new QAction(tr("Format"), this);
    particularOptions->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(formatFile()));
}
void zwr::TextFileReader::addMathematicaOutputButton()
{
    /*
    QPushButton* button = new QPushButton("Mathematica Src");
    menuButtons->addWidget(button);
    connect(button,SIGNAL(clicked()),this,SLOT(mathematicaOutput()));
    */
    QAction* action = new QAction(tr("Output Mathematica Code"), this);
    particularOptions->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(mathematicaOutput()));
}
void zwr::TextFileReader::addSmoothOutputButton()
{
    /*
    QPushButton* button = new QPushButton("Smooth Plots");
    menuButtons->addWidget(button);
    connect(button,SIGNAL(clicked()),this,SLOT(smoothOutput()));
    */
    QAction* action = new QAction(tr("Smooth Plots"), this);
    particularOptions->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(smoothOutput()));
}
void zwr::TextFileReader::addTFCalcOutputButton()
{
    /*
    QPushButton* button = new QPushButton("TFCalc Format");
    menuButtons->addWidget(button);
    connect(button,SIGNAL(clicked()),this,SLOT(tfCalcOutput()));
    */
    QAction* action = new QAction(tr("Create Folder with TFCalc Files"), this);
    particularOptions->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(tfCalcOutput()));
}
void zwr::TextFileReader::addMathematicaPlotButton()
{
    /*
    QPushButton* button = new QPushButton("Mathematica Plot");
    menuButtons->addWidget(button);
    connect(button,SIGNAL(clicked()),this,SLOT(mathematicaPlotOutput()));
    */
    QAction* action = new QAction(tr("Format for Plotting in Mathematica"), this);
    particularOptions->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(mathematicaPlotOutput()));
}

void zwr::TextFileReader::addHideButton()
{
    /*
    QPushButton* button = new QPushButton("Hide Selected Columns");
    menuButtons->addWidget(button);
    connect(button,SIGNAL(clicked()),this,SLOT(hideSelectedColumns()));
    */
    QAction* action = new QAction(tr("Hide Slected Columns"), this);
    particularOptions->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(hideSelectedColumns()));
}

void zwr::TextFileReader::hideSelectedColumns()
{
    QItemSelectionModel *select = tableForm->selectionModel();
    int cap = select->selectedColumns().size();
    for(int k=0; k<cap; ++k)
        toggleCurveVisibility( select->selectedColumns()[k].column() );
}

void zwr::TextFileReader::addConvertTimeStampButton()
{
    /*
    QPushButton* button = new QPushButton("Convert Time Stamp");
    menuButtons->addWidget(button);
    connect(button,SIGNAL(clicked()),this,SLOT(convertTimeStamp()));
    */
    QAction* action = new QAction(tr("Convert Time Stamp to Hours"), this);
    particularOptions->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(convertTimeStamp()));
}
void zwr::TextFileReader::convertTimeStamp()
{
    QString convert = textForm->toPlainText();
    textForm->clear();
    QTextStream stream(&convert);
    QString input = stream.readLine();
    double hrs = 0;
    while (!input.isNull()){
        hrs = input.left(2).toDouble() + input.mid(3,2).toDouble()/60 + input.mid(6,2).toDouble()/3600;
        textForm->insertPlainText( QString::number(hrs) + "\t" + input.mid(9, input.length()-9) + "\n");
        input = stream.readLine();
    }
}


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

        //pen.setColor(QColor(sin(k*0.3)*100+100, sin(k*1+.3)*100+100, sin(k*2+.6)*100+100));
        pen.setColor(QColor(sin(k*500)*100+100, sin(k*1000+.785)*100+100, sin(k*1000+1.57)*100+100));
        graphForm->graph(k-1)->setPen(pen);
    }

    //graphForm->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    graphForm->xAxis->setLabelFont( QFont(QFont().family(),18) );
    graphForm->yAxis->setLabelFont( QFont(QFont().family(),18) );
    graphForm->rescaleAxes(true);
    graphForm->replot();
    this->update();
}

double zwr::TextFileReader::dayTimeToHours(QString time)
{
    return time.left(2).toDouble() + time.mid(3,2).toDouble()/60 + time.mid(6,2).toDouble()/3600;
}

void zwr::TextFileReader::addSetxIntervalButton()
{
    QPushButton* button = new QPushButton("Set X Interval");
    graphxLowEnd = new QLineEdit;
    graphxLowEnd->setPlaceholderText("X Low End");
    graphxHighEnd = new QLineEdit;
    graphxHighEnd->setPlaceholderText("X High End");
    QHBoxLayout* lay = new QHBoxLayout;
    lay->addWidget(graphxLowEnd);
    lay->addWidget(graphxHighEnd);
    lay->addWidget(button);
    QWidget* wid = new QWidget;
    wid->setLayout(lay);
    menuButtons->addWidget(wid);
    connect(button,SIGNAL(clicked()),this,SLOT(setxInterval()));
}

void zwr::TextFileReader::setxInterval()
{
    graphForm->xAxis->setRangeLower( graphxLowEnd->text().toDouble());
    graphForm->xAxis->setRangeUpper( graphxHighEnd->text().toDouble());
    //graphForm->yAxis->rescale();
    graphForm->replot();
}

void zwr::TextFileReader::addSetyIntervalButton()
{
    QPushButton* button = new QPushButton("Set Y Interval");
    graphyLowEnd = new QLineEdit;
    graphyLowEnd->setPlaceholderText("Y Low End");
    graphyHighEnd = new QLineEdit;
    graphyHighEnd->setPlaceholderText("Y High End");
    QHBoxLayout* lay = new QHBoxLayout;
    lay->addWidget(graphyLowEnd);
    lay->addWidget(graphyHighEnd);
    lay->addWidget(button);
    QWidget* wid = new QWidget;
    wid->setLayout(lay);
    menuButtons->addWidget(wid);
    connect(button,SIGNAL(clicked()),this,SLOT(setyInterval()));
}

void zwr::TextFileReader::setyInterval()
{
    graphForm->yAxis->setRangeLower( graphyLowEnd->text().toDouble());
    graphForm->yAxis->setRangeUpper( graphyHighEnd->text().toDouble());
    //graphForm->yAxis->rescale();
    graphForm->replot();
}

void zwr::TextFileReader::parseFormattedData(QString& input)
{
    QStringList row = input.split( "\t", QString::SkipEmptyParts );
    int numEntries = row.size();
    for(int k=0;k<numEntries;++k){
        data[k].push_back(row[k].toDouble());
    }
}

void zwr::TextFileReader::parseUnformattedData(QString& input)
{
    QStringList row = input.split( separator, QString::SkipEmptyParts );
    int numEntries = row.size();
    for(int k=0;k<numEntries;++k){
        data[k].push_back(row[k].toDouble());
    }
}


void zwr::TextFileReader::readDataFile(QString woName, DBStructure* wodb)
{
    WODB = wodb;
    textForm->clear();
    woFileName = woName;

    ///neet to input as arg to constructor?
    setFilePath();

    if ( woFile.open(QFile::ReadOnly | QFile::Text) )
    {
        QTextStream streamer(&woFile);
        QString input = streamer.readLine();

        cropTop(input, streamer);

        data = QVector<QVector<double> >( depParams.size()+1);
        input = streamer.readLine();
        while (!input.isNull())
        {
            //uploadDataQuickly(woName,input,wodb);
            parseFormattedData(input);
            input = streamer.readLine();
        }
        createTable();
        graphData();
    }
    woFile.close();
}

void zwr::TextFileReader::addSumColumnToData()
{
    depParams.push_back("Sum");
    int rowCount = data[0].size();
    int oldColCount = data.size();
    QVector<double> sumColumn(rowCount);
    for(int k=0;k<rowCount;++k){
        double sum=0;
        for(int j=1;j<oldColCount;++j){
            sum += data[j][k];
        }
        sumColumn[k] = sum;
    }
    data.push_back(sumColumn);
    writeDataToFile();
}








