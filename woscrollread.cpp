#include "woscrollread.h"
#include "textfilereader.h"
#include "workorderscroller.h"
#include <QString>
#include <QLayout>
#include <iostream>

zwr::WOScrollRead::WOScrollRead(QWidget *parent) :
    QWidget(parent)
{

    dataBase = new zwr::DBStructure;

    woBox = new zwr::WorkOrderFileReader();
    woBox->setTableVisible(0);
    woBox->setGraphVisible(0);

    reportBox = new zwr::ReportFileReader();
    reportBox->setTableVisible(0);
    reportBox->setGraphVisible(0);


    rgaBox = new zwr::RGAFileReader();
    //rgaBox->setTextVisible(0);
    rgaBox->setTableVisible(1);

    pyroBox = new zwr::PyrometerFileReader();
    pyroBox->setTextVisible(0);
    /*
    tcEnviroBox = new zwr::PyrometerFileReader();
    tcEnviroBox->setTextVisible(0);
    tcSampleBox = new zwr::PyrometerFileReader();
    tcSampleBox->setTextVisible(0);
*/
    crystalBox = new zwr::HMIFileReader("Crystal");
    crystalBox->setTextVisible(0);
    trackBox = new zwr::HMIFileReader("Track");
    trackBox->setTextVisible(0);

    noteBox = new zwr::NotePage();

    stepBox = new zwr::ThicknessFileReader();
    stepBox->setTableVisible(0);
    spectralBox = new zwr::SpectralFileReader();
    scrollWid = new zwr::WorkOrderScroller;

    readerVector.push_back(woBox);
    readerVector.push_back(reportBox);
    readerVector.push_back(noteBox);
    readerVector.push_back(rgaBox);
    readerVector.push_back(pyroBox);
    readerVector.push_back(crystalBox);
    readerVector.push_back(trackBox);
    readerVector.push_back(noteBox);
    readerVector.push_back(stepBox);
    readerVector.push_back(spectralBox);

    tabDisplay = new TabDataDisplay;
    tabDisplay->addTab(woBox, "Work Order");
    tabDisplay->addTab(reportBox, "Report");
    tabDisplay->addTab(noteBox, "Note Log");
    tabDisplay->addTab(rgaBox, "RGA Data");
    tabDisplay->addTab(pyroBox, "Pyrometer Data");

    tabDisplay->addTab(crystalBox, "Crystal Data");
    tabDisplay->addTab(trackBox, "Track Data");
    tabDisplay->addTab(stepBox, "Step Measurements");
    tabDisplay->addTab(spectralBox, "Spectral Measurements");

    connect(tabDisplay, SIGNAL(currentChanged(int)), this, SLOT(addParticularMenuFor( int )));

    QVBoxLayout* ctrLay = new QVBoxLayout;
    ctrLay->addWidget(scrollWid);
    ctrLay->addWidget(tabDisplay);
    setLayout(ctrLay);
    this->update();

    connect (scrollWid, SIGNAL(valueChanged()),this,SLOT(openNewFile()));
    openNewFile();


}

void zwr::WOScrollRead::addParticularMenuFor(int selectedTab)
{
    emit sendMenuItem( readerVector[selectedTab]->getParticularOptions() );
}


void zwr::WOScrollRead::openNewFile()
{

    woBox->readFile( QString(scrollWid->getFullWO()), dataBase);
    reportBox->readReportFile(QString(scrollWid->getFullWO()), dataBase);

    noteBox->readNoteData(QString(scrollWid->getFullWO()), dataBase);

    rgaBox->readData( QString(scrollWid->getFullWO()), dataBase);
    pyroBox->readData( QString(scrollWid->getFullWO()), dataBase);
    stepBox->readThicknessFile( QString(scrollWid->getFullWO()), dataBase);
    spectralBox->readData( QString(scrollWid->getFullWO()), dataBase);



    crystalBox->readData( QString(scrollWid->getFullWO()), dataBase);
    trackBox->readData( QString(scrollWid->getFullWO()), dataBase);

}


void zwr::WOScrollRead::addConjoinGraphsButton()
{
    QPushButton* button = new QPushButton("Conjoin Selected Graphs");
    this->menuButtons->addWidget(button);

    connect(button,SIGNAL(clicked()),this,SLOT(conjoinGraphs()));
}
void zwr::WOScrollRead::conjoinGraphs()
{
    //(rgaBox->getGraph()->xAxis2) = (pyroBox->getGraph()->xAxis);
    //int rows = (pyroBox->getData())[0].size();
    int newCols = (pyroBox->getData()).size();
    int totCols = rgaBox->getData().size();
    QPen pen;
    rgaBox->getGraph()->yAxis2->setVisible(true);
    rgaBox->getGraph()->yAxis2->setLabel("Temperature (C)");
    rgaBox->getGraph()->yAxis2->setLabelFont( QFont(QFont().family(),18));
    //rgaBox->getGraph()->yAxis2->setR
    for(int k=1; k<newCols; ++k ){
        rgaBox->getGraph()->addGraph( rgaBox->getGraph()->xAxis, rgaBox->getGraph()->yAxis2);
        rgaBox->getGraph()->graph(totCols + k-2)->setData( (pyroBox->getData())[0], (pyroBox->getData())[k]);
        rgaBox->getGraph()->graph(totCols + k-2)->rescaleAxes(true);
        rgaBox->getGraph()->graph(totCols + k-2)->setName( (pyroBox->getDepParams())[k-1]);
        //rgaBox->getGraph()->graph(totCols + k-2)->


        pen.setColor(QColor(sin(k*10+0.3)*100+100, sin(k*20+.3)*100+100, sin(k*30+.6)*100+100));
        rgaBox->getGraph()->graph(totCols + k -2)->setPen(pen);
    }

    rgaBox->getGraph()->replot();
    return;
}

void zwr::WOScrollRead::addOverlapNotesButton()
{
    QPushButton* button = new QPushButton("Overlap Notes With Graphs");
    this->menuButtons->addWidget(button);

    connect(button,SIGNAL(clicked()),this,SLOT(overlapNotesWithGraphs()));
}


void zwr::WOScrollRead::overlapNotesWithGraphs()
{
    QCustomPlot* plot = rgaBox->getGraph();
    QCPAxis* xaxis = plot->xAxis;

    xaxis->setAutoTicks(true);
    xaxis->setAutoTickLabels(true);
    xaxis->setTicks(true);
    plot->replot();

    QVector<double> tempTicks = xaxis->tickVector();
    QVector<QString> tempTickLabels = xaxis->tickVectorLabels();
    QVector<double> timeTicks = noteBox->getTimeStamps();

    int numTicks = tempTicks.size();

    double tickStep = xaxis->tickStep()/4;
    //std::cout<<tickStep<<"\n";
    int numTimeTicks = timeTicks.size();

    for(int k=0; k<numTicks; ++k){
        for(int j=0; j<numTimeTicks; ++j){
            double difTick = tempTicks[k]-timeTicks[j];
            if( difTick <  tickStep && difTick > -tickStep ){
                tempTicks.remove(k);
                tempTickLabels.remove(k);
                --numTicks;
                break;
            }
        }
    }

    rgaBox->getGraph()->xAxis->setAutoTicks(false);
    rgaBox->getGraph()->xAxis->setAutoTickLabels(false);
    rgaBox->getGraph()->xAxis->setTickVector( tempTicks + timeTicks );
    rgaBox->getGraph()->xAxis->setTickVectorLabels(tempTickLabels + noteBox->getEvents());
    rgaBox->getGraph()->xAxis->setTickLabelRotation(15);

    rgaBox->getGraph()->replot();
}
