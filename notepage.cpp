#include "notepage.h"
#include <QTime>
#include <iostream>

zwr::NotePage::NotePage()
{
    noteTable = new QTableWidget(0,2);
    //noteData = QVector<QVector< QString> >(2);
    noteDepParams = QStringList() << "Time" << "Event";
    noteTable->setHorizontalHeaderLabels( noteDepParams );
    noteTable->update();

    QPushButton* button = new QPushButton("Add Note");
    menuButtons->addWidget(button);
    connect(button,SIGNAL(clicked()),this,SLOT(addNote()));
    textForm->setVisible(0);
    graphForm->setVisible(0);
    tableForm->setVisible(0);

    this->layout()->addWidget(noteTable);
    noteTable->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    noteTable->horizontalHeader()->setStretchLastSection(true);
    addUploadButton();
}


void zwr::NotePage::addNote()
{
    noteTable->setRowCount( noteTable->rowCount() + 1);
    noteTable->setItem( noteTable->rowCount()-1, 0,
                              new QTableWidgetItem(
                                  QString::number(
                                      dayTimeToHours(
                                        QTime::currentTime().toString()
                                          )
                                      )
                                  )
                              );
    noteTable->item( noteTable->rowCount()-1, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    noteTable->selectRow( noteTable->rowCount()-1);
    noteTable->setCurrentCell( noteTable-> rowCount()-1, 1);
}

void zwr::NotePage::writeTextToFile()
{
    noteTable->selectRow( noteTable->rowCount()-1);
    textForm->clear();
    int rowsButOne = noteTable->rowCount()-1;
    int colsButOne = noteTable->columnCount()-1;

    for(int k=0; k<rowsButOne; ++k){
        QString row;
        for(int j=0;j<colsButOne;++j){
            row += noteTable->item(k,j)->text() + QString("\t");
        }
        row += noteTable->item(k,colsButOne)->text() + "\n";
        textForm->insertPlainText(row);
    }

    QString row;
    for(int j=0;j<colsButOne;++j)
        row += noteTable->item(rowsButOne,j)->text() + "\t";
    row += noteTable->item(rowsButOne,colsButOne)->text() + "\n";
    textForm->insertPlainText(row);

    woFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
    woFile.resize(0);
    QTextStream fileEraser(&woFile);
    fileEraser << textForm->toPlainText();
    woFile.close();
    //readNoteData(woFileName,WODB);
}

void zwr::NotePage::setFilePath()
{
    woFile.setFileName( woFileName + "-Log.txt");
}

void zwr::NotePage::readNoteData(QString woName, DBStructure* wodb)
{
    WODB = wodb;
    textForm->clear();
    woFileName = woName;

    ///neet to input as arg to constructor?
    setFilePath();

    if ( woFile.open(QFile::ReadOnly | QFile::Text) )
    {
        QTextStream streamer(&woFile);
        QString input;

        //noteTable->clear();
        noteTable->setRowCount(0);

        input = streamer.readLine();
        while (!input.isNull())
        {
            noteTable->setRowCount( noteTable->rowCount()+1 );
            QStringList entries = input.split("\t");
            int entryCount = entries.size();
            for(int k=0; k<entryCount; ++k)
                noteTable->setItem( noteTable->rowCount()-1,k,new QTableWidgetItem(entries[k]));


            input = streamer.readLine();
        }
    }
    else{
        noteTable->setRowCount(0);
    }

    woFile.close();

}

QVector<double> zwr::NotePage::getTimeStamps()
{
    int cap = noteTable->rowCount();
    QVector<double> output( cap );
    for(int k=0; k<cap; ++k)
        output[k]=noteTable->item(k,0)->text().toDouble();
    return output;
}

QVector<QString> zwr::NotePage::getEvents()
{
    int cap = noteTable->rowCount();
    QVector<QString> output( cap );
    for(int k=0; k<cap; ++k){
        if (noteTable->item(k,1)->text().size() > 30)
            output[k]=noteTable->item(k,1)->text().left(30);
        else
            output[k]=noteTable->item(k,1)->text();
    }
    return output;
}
















