#include "stdmtx.h"
#include <QString>
#include <vector>
#include <QFile>
#include <QTextStream>

using namespace std;

stdMtx::stdMtx()
{
    QFile qfile(":/AlStd/alstd.txt");
    //QFile qfile(":/0270FS5std.txt");
    //QFile qfile(":/AlStd/alstd_0129_1.txt");
    //QFile qfile(":/0270FS1std.txt");
    qfile.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&qfile);

    QString input;

    mtx = vector<vector<double> >(2);

    stream>>input;
    while (!input.isNull())
    {
        mtx[0].push_back( input.toDouble() );

        stream >> input;

        mtx[1].push_back(input.toDouble());
        stream>>input;
    }
    qfile.close();
}
