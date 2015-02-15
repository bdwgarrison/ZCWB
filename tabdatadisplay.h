#ifndef TABDATADISPLAY_H
#define TABDATADISPLAY_H

#include <QTabWidget>

namespace ZC_WO_Reader{
class TabDataDisplay;
}

namespace zwr = ZC_WO_Reader;
class zwr::TabDataDisplay : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabDataDisplay();

signals:

public slots:

};

#endif // TABDATADISPLAY_H
