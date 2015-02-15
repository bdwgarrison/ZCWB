#ifndef REPORTFILEREADER_H
#define REPORTFILEREADER_H


#include "textfilereader.h"

namespace ZC_WO_Reader{
class ReportFileReader;
}
namespace zwr = ZC_WO_Reader;


class zwr::ReportFileReader : public TextFileReader
{
public:
    ReportFileReader();
    void setFilePath();

public slots:

    double formatIndepParam(QString inParam) {return 0;}
    void writeTextToFile();
    void trimTop(QString&, QTextStream&) {return;}
    void readReportFile(QString woName, DBStructure* wodb);
    virtual void formatFile(){return;}

};

#endif // REPORTFILEREADER_H
