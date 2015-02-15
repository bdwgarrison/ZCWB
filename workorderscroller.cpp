#include "workorderscroller.h"
#include <QFormLayout>
#include <iostream>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QTextEdit>

///Scroller allows user to decide which files to open and search through the database
/// Essentially, converts QWidget inputs into a QString file path to pass off to other functions
zwr::WorkOrderScroller::WorkOrderScroller(QWidget *parent) :
    QWidget(parent)
{
    QLabel* jungleLabel = new QLabel("JungleDrive Letter:");
    jungleEntry = new QLineEdit("J");
    jungleEntry->setMaximumWidth(25);
    scroller = new QSlider(Qt::Horizontal);
    display = new QLineEdit;
    prefix = new QComboBox;
    fullWO = new QString;
    prefix->addItems(QStringList() <<"1000"<<"2000"<<"3000"<<"9017");


    getMem();
    getWORange();



    scroller->setMinimum(minWO);
    scroller->setMaximum(maxWO);
    scroller->setValue(woNum);

    //getWORange();


    updateDisplay(woNum);
    configureWOPath();

    QFormLayout* dspLay = new QFormLayout;
    setLayout(dspLay);
    QHBoxLayout* lay1 = new QHBoxLayout();
    QHBoxLayout* lay2 = new QHBoxLayout();
    lay1->addWidget(jungleLabel);
    lay1->addWidget(jungleEntry);
    lay1->addWidget(prefix);
    lay1->addWidget(display);
    lay2->addWidget(scroller);

    searchBar = new QLineEdit;
    searchBar->setPlaceholderText("search");
    lay1->addWidget(searchBar);

    dspLay->addRow(lay1);
    dspLay->addRow(lay2);

    connect(searchBar, SIGNAL(returnPressed()), this, SLOT(search()));
    connect(display, SIGNAL(returnPressed()), this, SLOT(updateWoNum()));

    connect(scroller, SIGNAL(valueChanged(int)),this,SLOT(updateWoNum(int)));
}

///keeps track of the highest and lowest work order numbers
/// stores range in a text file called Memory.txt
/// if the user goes to a folder that is out of the current range, this creates a folder for it
void zwr::WorkOrderScroller::updateWoNum()
{
    woNum = display->text().toInt();
    if (woNum > maxWO){
        maxWO = woNum;
        makeFolder();
    }
    if (woNum < minWO){
        minWO = woNum;
        makeFolder();
    }
    setMem();
    configureWOPath();

    emit valueChanged();
}


void zwr::WorkOrderScroller::updateWoNum(int newNum)
{
    woNum = newNum;
    if (woNum > maxWO)
        maxWO = woNum;
    if (woNum < minWO)
        minWO = woNum;
    setMem();
    updateDisplay(woNum);
    configureWOPath();


    emit valueChanged();
}

void zwr::WorkOrderScroller::updateDisplay(int newNum)
{
    display->setText(QString("%1").arg(newNum,4,10, QChar('0')).toUpper());
}

void zwr::WorkOrderScroller::configureWOPath()
{
    *fullWO = QString( getDir() + "/" + getPartialWO() + "/" + getPartialWO() );
}

QString zwr::WorkOrderScroller::getDir()
{
    /*
        return QString( jungleEntry->text() + ":/Work Orders Data/Data-" + prefix->currentText() + "\'s" );
*/
        ///mac
        return QString( "/"+ jungleEntry->text() + "/Work Orders Data/Data-" + prefix->currentText() + "\'s" );
}

QString zwr::WorkOrderScroller::getPartialWO()
{
        return QString( "WO-" + prefix->currentText() +"-"+ display->text() );
}

void zwr::WorkOrderScroller::getMem()
{
    ///current
    /// max
    /// min
    QFile memFile( "/" + jungleEntry->text() + "/Memory.txt");
    if ( memFile.open(QFile::ReadOnly | QFile::Text) ){
        QTextStream memStream(&memFile);
        woNum = memStream.readLine().toInt();
        maxWO = memStream.readLine().toInt();
        minWO = memStream.readLine().toInt();
        memFile.close();
    }
    else std::cout<<"Failed to open Memory.txt.\n";
}

void zwr::WorkOrderScroller::setMem()
{
    scroller->setMinimum(minWO);
    scroller->setMaximum(maxWO);
    QFile memFile( "/" + jungleEntry->text() + "/Memory.txt");
    if ( memFile.open(QFile::ReadWrite | QFile::Truncate | QFile::Text) ) {
        memFile.resize(0);
        QTextStream memStream(&memFile);
        memStream << QString::number(woNum) + "\n";
        memStream << QString::number(maxWO) + "\n";
        memStream << QString::number(minWO) + "\n";

        memFile.close();
    }
    else std::cout<<"Failed to open Memory.txt.\n";
}

void zwr::WorkOrderScroller::makeFolder()
{
    QDir newDir;

    newDir.mkdir( getDir() + "/" + getPartialWO() );
}

void zwr::WorkOrderScroller::getWORange()
{
    QDir scanner;

    for(int k=1; k<10000;++k){
        scanner.setPath( getDir() + "/WO-" + prefix->currentText() +"-"+ QString("%1").arg(k,4,10, QChar('0')).toUpper() );
        if( scanner.exists() ){
            if (k<minWO){
                minWO=k;
            }
            if (k>maxWO){
                maxWO=k;
            }
        }
    }
    setMem();
}


void zwr::WorkOrderScroller::search()
{
    QString query = searchBar->text();
    QTextEdit* searchResults = new QTextEdit;
    for(int index = minWO; index < maxWO; ++index){
        QString suffix = QString("%1").arg(index,4,10, QChar('0')).toUpper();
        QString woString = getDir() + "/WO-" + prefix->currentText() +"-"+ suffix +
                "/WO-" + prefix->currentText() +"-"+ suffix +".txt";
        QFile file(woString);

        if ( file.open(QFile::ReadOnly | QFile::Text) )
        {
            QTextStream streamer(&file);
            QString input = streamer.readLine();
            while(!input.isNull()){
                if (input.contains(query)){
                    searchResults->insertPlainText("WO-"+prefix->currentText()+"-"+suffix +"\n");
                    break;
                }
                input = streamer.readLine();
            }
            //searchResults->insertPlainText("WO-"+prefix->currentText()+suffix + "\n");


        }
        //else
          //  searchResults->insertPlainText(woString + " not opened\n");
    }
    //if (searchResults-> )
    searchResults->show();

}

