#ifndef PYROMETERFILEREADER_H
#define PYROMETERFILEREADER_H
#include "textfilereader.h"


namespace ZC_WO_Reader{
class PyrometerFileReader;
}

namespace zwr = ZC_WO_Reader;

class zwr::PyrometerFileReader : public TextFileReader
{
public:
    PyrometerFileReader();

    void setFilePath();

public slots:
    void formatXAxis();
    double formatIndepParam(QString inParam);
    void writeTextToFile();
    void trimTop(QString&, QTextStream&);
    virtual void formatFile();

};

#endif // PYROMETERFILEREADER_H
