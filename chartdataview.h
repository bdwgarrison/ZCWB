#ifndef CHARTDATAVIEW_H
#define CHARTDATAVIEW_H

#include <QAbstractTableModel>

class chartDataView : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit chartDataView(QObject *parent = 0);
    explicit chartDataView(int inRowNum, int inColNum, QVector<QVector<double> >* inDataMtx, QStringList*);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:

public slots:

private:
    int rowNum;
    int colNum;
    QVector<QVector<double> >* dataMtx;
    QStringList* headerList;
};

class stringDataView : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit stringDataView(QObject *parent = 0);
    explicit stringDataView(int inRowNum, int inColNum, QVector<QVector<QString> >* inDataMtx, QStringList*);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:

public slots:

private:
    int rowNum;
    int colNum;
    QVector<QVector<QString> >* dataMtx;
    QStringList* headerList;
};


#endif // CHARTDATAVIEW_H
