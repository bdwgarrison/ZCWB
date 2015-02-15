#ifndef WOSCROLLREAD_H
#define WOSCROLLREAD_H

#include <QWidget>
#include "textfilereader.h"
#include "workorderscroller.h"
#include "tabdatadisplay.h"
#include "workorderfilereader.h"
#include "hmifilereader.h"
#include "thicknessfilereader.h"
#include "spectralfilereader.h"
#include "rgafilereader.h"
#include "reportfilereader.h"
#include "pyrometerfilereader.h"
#include "notepage.h"

namespace ZC_WO_Reader{
class WOScrollRead;
}

namespace zwr = ZC_WO_Reader;

class zwr::WOScrollRead : public QWidget
{
    Q_OBJECT
public:
    explicit WOScrollRead(QWidget *parent = 0);
signals:
    void sendMenuItem(QMenu*&);

public slots:
    void openNewFile();
    void addConjoinGraphsButton();
    void conjoinGraphs();
    void overlapNotesWithGraphs();
    void addOverlapNotesButton();

    void addParticularMenuFor(int selectedTab);

private:
    WorkOrderFileReader* woBox;
    ReportFileReader* reportBox;
    RGAFileReader* rgaBox;
    PyrometerFileReader* pyroBox;
    PyrometerFileReader* tcEnviroBox;
    PyrometerFileReader* tcSampleBox;

    HMIFileReader* crystalBox;
    HMIFileReader* trackBox;
    ThicknessFileReader* stepBox;
    SpectralFileReader* spectralBox;

    NotePage* noteBox;

    TabDataDisplay* tabDisplay;

    WorkOrderScroller* scrollWid;
    DBStructure* dataBase;

    QHBoxLayout* menuButtons;

    QVector< TextFileReader*> readerVector;
};

#endif // WOSCROLLREAD_H
