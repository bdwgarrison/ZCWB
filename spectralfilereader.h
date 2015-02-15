#ifndef SPECTRALFILEREADER_H
#define SPECTRALFILEREADER_H

#include "textfilereader.h"

namespace ZC_WO_Reader{
class SpectralFileReader;
}

namespace zwr = ZC_WO_Reader;
class zwr::SpectralFileReader : public TextFileReader
{
public:
    SpectralFileReader();
    void setFilePath();

public slots:
    void writeTextToFile();
    void trimTop(QString&, QTextStream&);
    void formatXAxis();
    virtual void formatFile(){return;}


};

#endif // SPECTRALFILEREADER_H
