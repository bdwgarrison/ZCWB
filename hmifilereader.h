#ifndef HMIFILEREADER_H
#define HMIFILEREADER_H

#include "textfilereader.h"

namespace ZC_WO_Reader{
class HMIFileReader;
}

namespace zwr = ZC_WO_Reader;
class zwr::HMIFileReader : public TextFileReader
{
public:
    HMIFileReader();
    HMIFileReader(QString);

    void setFilePath();
    double formatIndepParam(QString inParam);
    void formatXAxis();
public slots:
    void readHMIFileQuickly(QString woName, QString hmiType, DBStructure *wodb);
    void writeTextToFile(){return;}
    void trimTop(QString&, QTextStream&);// {return;}
    virtual void formatFile(){return;}



private:
    //QString fileType;
};

#endif // HMIFILEREADER_H
