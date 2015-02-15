#ifndef WORKORDERSCROLLER_H
#define WORKORDERSCROLLER_H

#include <QWidget>
#include <QSlider>
#include <QLineEdit>
#include <QComboBox>

namespace ZC_WO_Reader{
class WorkOrderScroller;
}

namespace zwr = ZC_WO_Reader;

class zwr::WorkOrderScroller : public QWidget
{
    Q_OBJECT
public:
    explicit WorkOrderScroller(QWidget *parent = 0);

signals:
    void valueChanged();

public slots:
    void updateWoNum(int);
    void updateWoNum();
    QString getDir();
    QString getPartialWO();
    const QString& getFullWO() {return *fullWO;}
    void getMem();
    void setMem();
    void makeFolder();
    void getWORange();

    void search();


private:
    void configureWOPath();
    void updateDisplay(int);
    int woNum;
    int maxWO;
    int minWO;
    QSlider* scroller;
    QLineEdit* display;
    QLineEdit* jungleEntry;
    QLineEdit* searchBar;
    QComboBox* prefix;
    QString* fullWO;
    QString* jungleLetter;
};

#endif // WORKORDERSCROLLER_H
