#ifndef NOTEPAGE_H
#define NOTEPAGE_H
#include "textfilereader.h"
#include <QTableWidget>
#include <QVector>

namespace ZC_WO_Reader{
class NotePage;
}

namespace zwr = ZC_WO_Reader;

class zwr::NotePage : public TextFileReader
{
public:
    explicit NotePage();
    QVector<double> getTimeStamps();
    QVector<QString> getEvents();

public slots:
    void addNote();
    void writeTextToFile();
    void setFilePath();
    void readNoteData(QString woName, DBStructure* wodb);

private:
    QTableWidget* noteTable;
};

#endif // NOTEPAGE_H
