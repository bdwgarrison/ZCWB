#ifndef THICKNESSFILEREADER_H
#define THICKNESSFILEREADER_H

#include "textfilereader.h"
//#include <QPushButton>
namespace ZC_WO_Reader{
class ThicknessFileReader;
}

namespace zwr = ZC_WO_Reader;
class zwr::ThicknessFileReader : public TextFileReader
{
public:
    ThicknessFileReader();
    void setFilePath();
public slots:
    void readThicknessFile(QString woName, DBStructure *wodb);
    void writeTextToFile();
    void trimTop(QString&, QTextStream&);
    virtual void formatFile(){return;}



private:
    QLabel* statLabel;
};

#endif // THICKNESSFILEREADER_H
