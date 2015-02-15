#ifndef WORKORDERFILEREADER_H
#define WORKORDERFILEREADER_H

#include "textfilereader.h"

namespace ZC_WO_Reader{
class WorkOrderFileReader;
}

namespace zwr = ZC_WO_Reader;
class zwr::WorkOrderFileReader : public TextFileReader
{
public:
    WorkOrderFileReader();
    void setFilePath();
public slots:
    void readWOFile(QString woName, DBStructure *wodb);
    void writeTextToFile(){return;}
    void trimTop(QString&, QTextStream&);
    virtual void formatFile(){return;}


};

#endif // WORKORDERFILEREADER_H
