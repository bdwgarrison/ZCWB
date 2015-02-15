#ifndef TEXTFILEREADER_H
#define TEXTFILEREADER_H

#include <QTextEdit>
#include <QTextStream>
#include <QFile>
#include <QTableWidget>
#include <QMap>
#include "workorderdatum.h"
#include "qcustomplot.h"
#include "chartdataview.h"
#include "dropbox.h"

namespace ZC_WO_Reader{
class TextFileReader;
}

namespace zwr = ZC_WO_Reader;
class zwr::TextFileReader : public QWidget
{
    Q_OBJECT
public:
    explicit TextFileReader();


    QCustomPlot* getGraph(){return graphForm;}
    QStringList getDepParams(){return depParams;}


    const QString& getDBPath() {return dbPath;}
    const QString& getWOCompound() {return woCompound;}

    //QTableWidget* getChartForm() {return chartForm;}
    QStringList dependentParametersUsed(QString &temp);
    void uploadData(QString woName, QString temp, DBStructure* wodb);
    void uploadDataQuickly(QString woName, QString input, DBStructure* wodb);

    void uploadAndPrepGraphData(QString woName, QString temp, DBStructure* wodb);

    virtual double formatIndepParam(QString);

    virtual void formatXAxis() {return;}
    void clearData();
    virtual void setFilePath();
    QVector<QVector< double> > getData() {return data;}

    void addSumColumnToData();
signals:
    void WOLoaded(TextFileReader*);
    void emitParameterChart(QTableWidget*);

    void graphIntSelected(int);
public slots:
    void readDataFile(QString woName, DBStructure* wodb);


    void parseFormattedData(QString &input);
    void parseUnformattedData(QString &input);

    virtual void addNote() {return;}
    double dayTimeToHours(QString time);

    void toggleGraphVisibile();
    void toggleTableVisibile();
    void toggleChartVisible();
    void toggleTextVisibile();

    void readData(QString woName, DBStructure* wodb);
    void readFile(QString, DBStructure*);

    void addMathematicaOutputButton();
    void mathematicaOutput();

    void addTFCalcOutputButton();
    void tfCalcOutput();

    void addInterpOutputButton();
    void interpOutput();

    void addSmoothOutputButton();
    void smoothOutput();

    void addMathematicaPlotButton();
    void mathematicaPlotOutput();

    void addHideButton();
    void hideSelectedColumns();

    void addConvertTimeStampButton();
    void convertTimeStamp();

    void addFormatButton();
    virtual void formatFile(){return;}

    void setxInterval();
    void addSetxIntervalButton();

    void setyInterval();
    void addSetyIntervalButton();

    virtual void trimTop(QString&, QTextStream&);
    virtual void cropTop(QString&, QTextStream&);
    double avgOfCol(int n) {double avg=0; int cap= data[n].size();
                            for(int k=0;k<cap;++k) avg+=data[n][k]; return avg/cap;}
    double sdvOfCol(int n) {double sdv=0; int cap= data[n].size();
                            for(int k=0;k<cap;++k) sdv+=data[n][k]*data[n][k];
                            return qSqrt(sdv/cap - avgOfCol(n)*avgOfCol(n));}

    void graphData();
    void selectionChanged();
    void highlightGraphInt(int unused,int k);

    void setGraphVisible(bool b) {graphForm->setVisible(b);}
    void setTableVisible(bool b){tableForm->setVisible(b);}
    void setChartVisible(bool b){chartForm->setVisible(b);}
    void setTextVisible(bool b){textForm->setVisible(b);}

    void toggleCurveVisibility(int k);//{ graphForm->graph(k)->setVisible( ~(graphForm->graph(k)->visible()) );}

    void addUploadButton();
    virtual void writeTextToFile()=0;
    void writeDataToFile();

    void createChart();
    void createTable();
    void createStringTable();

    QMenu*& getParticularOptions() {return particularOptions;}
protected:
    QString decapitator;
    QString separator;

    QVector<QVector< QString> > noteData;
    stringDataView* stringTableFormModel;
    QStringList noteDepParams;


    double baseTime;
    QHBoxLayout* menuButtons;

    QString dbPath;
    QString fileType;
    QString woCompound;
    QString woFolderPath;
    QString woFileName;
    QFile woFile;


    QTableWidget* chartForm;
    dropBox* textForm;

    QCustomPlot* graphForm;
    QTableView* tableForm;
    chartDataView* tableFormModel;

    QVector<QVector<double> > data;
    QStringList depParams;
    DBStructure* WODB;
    QStringList desiredHeaderList;
    QList<int> desiredHeaderListNumbers;

    int indepParamCount;
    QString indepParam;

    QMap<QString,int> HeaderMap;
    QLineEdit* graphxLowEnd;
    QLineEdit* graphxHighEnd;
    QLineEdit* graphyLowEnd;
    QLineEdit* graphyHighEnd;

    QMenu* particularOptions;
};

#endif // TEXTFILEREADER_H
