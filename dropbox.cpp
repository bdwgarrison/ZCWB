#include "dropbox.h"
#include "specmtx.h"
#include <vector>

#include <iostream>

using namespace std;


void dropBox::condenseTransmittanceFiles()
{
    getFileNameList();
    condenseFiles(fileNameList, 'T');
}

void dropBox::condenseReflectanceFiles()
{
    getFileNameList();
    condenseFiles(fileNameList, 'R');
}


void dropBox::condenseFiles(QStringList fileList,char RorT)
{
    specMtx spec( fileList, RorT);
    vector<vector<double> > mtx = spec.getMtx();


    this->setLineWrapMode(QTextEdit::NoWrap);
    this->clear();
    long unsigned int cols = mtx.size();
    long unsigned int rows = mtx[0].size();
    ///set header, start with tab
    this->insertPlainText("\t");
    for (long unsigned j=1; j<cols-1; ++j)
        this->insertPlainText( headerList[j-1] + "\t");
    this->insertPlainText( headerList[cols-2] );

    for(long unsigned k=0; k<rows; k++)
    {
        QString temp;
        for(long unsigned j=0; j<cols-1; j++)
        {
            temp+= QString::number(mtx[j][k]);
            temp+= "\t";
        }
        temp+= QString::number(mtx[cols-1][k]);
        this->append(temp);
    }
}

QStringList dropBox::getFileNameList()
{
    fileNameList = QStringList();
    headerList = QStringList();

    QString input = this->toPlainText();
    int textLength = input.length();
    int start =0;

    for (int k=0; k<textLength; k++)
    {
        ///go through long string
        if (input.mid(k,7) == "file://")
        {
            ///when arriving at file path, crawl forward until end of path name
            k+=7;
            start = k;
        }

        if (input[k] =='\n')
            fileNameList.push_back(QString(input.mid(start,k-start)));
        else if ( k == textLength-1 )
            fileNameList.push_back(QString(input.mid(start,k-start+1)));
    }

    int numFiles = fileNameList.size();
    for (int k=0; k<numFiles; ++k){
        QString tempName = fileNameList[k];
        int nameLength = tempName.size();
        int j=3;
        while( tempName[nameLength-j].toLatin1() != '/' )
            ++j;
        headerList.push_back( tempName.mid ( nameLength-j+1,j-4) );
    }

    return fileNameList;
}


