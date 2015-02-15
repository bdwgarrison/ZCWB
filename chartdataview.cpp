#include "chartdataview.h"

chartDataView::chartDataView(QObject *parent) :
    QAbstractTableModel(parent)
{
}

///Table to quickly display data stored in matrix of doubles
chartDataView::chartDataView(int inRowNum, int inColNum, QVector<QVector<double> >* inDataMtx, QStringList* inHeaderList)
{
    rowNum = inRowNum;
    colNum = inColNum;
    dataMtx = inDataMtx;
    headerList = inHeaderList;
}


int chartDataView::rowCount(const QModelIndex & /*parent*/) const
{
   return rowNum;
}

int chartDataView::columnCount(const QModelIndex & /*parent*/) const
{
    return colNum;
}


QVariant chartDataView::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
       return QString::number( (*dataMtx)[index.column()+1][index.row()]);
    }
    return QVariant();
}
QVariant chartDataView::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            return (*headerList)[section];
        }
        else if (orientation == Qt::Vertical){
            return (*dataMtx)[0][section];
        }
    }
    return QVariant();
}

/*************** next **************/

stringDataView::stringDataView(QObject *parent) :
    QAbstractTableModel(parent)
{
}
stringDataView::stringDataView(int inRowNum, int inColNum, QVector<QVector<QString> >* inDataMtx, QStringList* inHeaderList)
{
    rowNum = inRowNum;
    colNum = inColNum;
    dataMtx = inDataMtx;
    headerList = inHeaderList;
}

int stringDataView::rowCount(const QModelIndex & /*parent*/) const
{
   return rowNum;
}

int stringDataView::columnCount(const QModelIndex & /*parent*/) const
{
    return colNum;
}

QVariant stringDataView::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
       return (*dataMtx)[index.column()+1][index.row()];
    }
    return QVariant();
}
QVariant stringDataView::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            return (*headerList)[section];

        }
        else if (orientation == Qt::Vertical){
            return (*dataMtx)[0][section];
        }
    }
    return QVariant();
}
