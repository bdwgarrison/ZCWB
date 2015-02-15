#ifndef RGAFILEREADER_H
#define RGAFILEREADER_H

#include "textfilereader.h"

namespace ZC_WO_Reader{
class RGAFileReader;
}

namespace zwr = ZC_WO_Reader;
class zwr::RGAFileReader : public TextFileReader
{
public:
    RGAFileReader();
    void setFilePath();

public slots:
    double formatIndepParam(QString inParam);
    void formatFile();
    void writeTextToFile();
    void trimTop(QString&, QTextStream&);
    void formatXAxis();
    void setBaseTime(QString input);
    void locateColumnsForDesiredHeader(QString &input, QTextStream &streamer);
    //void formatFile();


};

#endif // RGAFILEREADER_H
