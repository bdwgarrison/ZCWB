#ifndef WORKORDERDATUM_H
#define WORKORDERDATUM_H

#include <QWidget>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <string>


namespace ZC_WO_Reader{
class WorkOrderDatum;
class DBStructure;
}

namespace zwr = ZC_WO_Reader;

class zwr::WorkOrderDatum : public QObject
{
    Q_OBJECT
public:
    std::string index1_FullWorkOrderNum_;
    std::string index2_MatrixType_;
    std::string index3_DependentData_;
    std::string index4_IndependentData_;
    std::string index5_Datum_;

    WorkOrderDatum(std::string index1_FullWorkOrderNum,
                   std::string index2_MatrixType,
                   std::string index3_DependentData,
                   std::string index4_IndependentData,
                   std::string index5_Datum):
                        index1_FullWorkOrderNum_(index1_FullWorkOrderNum),
                        index2_MatrixType_(index2_MatrixType),
                        index3_DependentData_(index3_DependentData),
                        index4_IndependentData_(index4_IndependentData),
                        index5_Datum_(index5_Datum){}
    WorkOrderDatum(const WorkOrderDatum& w);
    WorkOrderDatum& operator= (const WorkOrderDatum&);

    void printAll() const;

};

/// key declarations
struct index1_FullWorkOrderNum{};
struct index2_MatrixType{};
struct index3_DependentData{};
struct index4_IndependentData{};
struct index5_Datum{};
/// hash key declarations
struct index1_FullWorkOrderNum_Hash{};
struct index2_MatrixType_Hash{};
struct index3_DependentData_Hash{};
struct index4_IndependentData_Hash{};
struct index5_Datum_Hash{};

///container declaration
typedef boost::multi_index_container<
    zwr::WorkOrderDatum,
    boost::multi_index::indexed_by<

        boost::multi_index::ordered_non_unique<boost::multi_index::tag<index1_FullWorkOrderNum>,  BOOST_MULTI_INDEX_MEMBER(zwr::WorkOrderDatum,std::string,index1_FullWorkOrderNum_)>,
        boost::multi_index::ordered_non_unique<boost::multi_index::tag<index2_MatrixType>,BOOST_MULTI_INDEX_MEMBER(zwr::WorkOrderDatum,std::string,index2_MatrixType_)>,
        boost::multi_index::ordered_non_unique<boost::multi_index::tag<index3_DependentData>, BOOST_MULTI_INDEX_MEMBER(zwr::WorkOrderDatum,std::string,index3_DependentData_)>,
        boost::multi_index::ordered_non_unique<boost::multi_index::tag<index4_IndependentData>, BOOST_MULTI_INDEX_MEMBER(zwr::WorkOrderDatum,std::string,index4_IndependentData_)>,
        boost::multi_index::ordered_non_unique<boost::multi_index::tag<index5_Datum>, BOOST_MULTI_INDEX_MEMBER(zwr::WorkOrderDatum,std::string,index5_Datum_)>,

        boost::multi_index::hashed_non_unique<boost::multi_index::tag<index1_FullWorkOrderNum_Hash>,  BOOST_MULTI_INDEX_MEMBER(zwr::WorkOrderDatum,std::string,index1_FullWorkOrderNum_)>,
        boost::multi_index::hashed_non_unique<boost::multi_index::tag<index2_MatrixType_Hash>,  BOOST_MULTI_INDEX_MEMBER(zwr::WorkOrderDatum,std::string,index2_MatrixType_)>,
        boost::multi_index::hashed_non_unique<boost::multi_index::tag<index3_DependentData_Hash>,  BOOST_MULTI_INDEX_MEMBER(zwr::WorkOrderDatum,std::string,index3_DependentData_)>,
        boost::multi_index::hashed_non_unique<boost::multi_index::tag<index4_IndependentData_Hash>,  BOOST_MULTI_INDEX_MEMBER(zwr::WorkOrderDatum,std::string,index4_IndependentData_)>,
        boost::multi_index::hashed_non_unique<boost::multi_index::tag<index5_Datum_Hash>,  BOOST_MULTI_INDEX_MEMBER(zwr::WorkOrderDatum,std::string,index5_Datum_)>

    >
> WorkOrderDataBase;

/// indices for wodb
typedef boost::multi_index::index<WorkOrderDataBase,index1_FullWorkOrderNum>::type wodb_indexed1_t;
typedef boost::multi_index::index<WorkOrderDataBase,index2_MatrixType>::type wodb_indexed2_t;
typedef boost::multi_index::index<WorkOrderDataBase,index3_DependentData>::type wodb_indexed3_t;
typedef boost::multi_index::index<WorkOrderDataBase,index4_IndependentData>::type wodb_indexed4_t;
typedef boost::multi_index::index<WorkOrderDataBase,index5_Datum>::type wodb_indexed5_t;

typedef boost::multi_index::index<WorkOrderDataBase,index1_FullWorkOrderNum_Hash>::type wodb_indexed1_hash_t;
typedef boost::multi_index::index<WorkOrderDataBase,index2_MatrixType_Hash>::type wodb_indexed2_hash_t;
typedef boost::multi_index::index<WorkOrderDataBase,index3_DependentData_Hash>::type wodb_indexed3_hash_t;
typedef boost::multi_index::index<WorkOrderDataBase,index4_IndependentData_Hash>::type wodb_indexed4_hash_t;
typedef boost::multi_index::index<WorkOrderDataBase,index5_Datum_Hash>::type wodb_indexed5_hash_t;

/// iterators for wodb
typedef boost::multi_index::index<WorkOrderDataBase,index1_FullWorkOrderNum>::type::const_iterator  wodb_indexed1_iterator_t;
typedef boost::multi_index::index<WorkOrderDataBase,index2_MatrixType>::type::const_iterator  wodb_indexed2_iterator_t;
typedef boost::multi_index::index<WorkOrderDataBase,index3_DependentData>::type::const_iterator  wodb_indexed3_iterator_t;
typedef boost::multi_index::index<WorkOrderDataBase,index4_IndependentData>::type::const_iterator  wodb_indexed4_iterator_t;
typedef boost::multi_index::index<WorkOrderDataBase,index5_Datum>::type::const_iterator  wodb_indexed5_iterator_t;

typedef boost::multi_index::index<WorkOrderDataBase,index1_FullWorkOrderNum_Hash>::type::const_iterator  wodb_indexed1_hash_iterator_t;
typedef boost::multi_index::index<WorkOrderDataBase,index2_MatrixType_Hash>::type::const_iterator  wodb_indexed2_hash_iterator_t;
typedef boost::multi_index::index<WorkOrderDataBase,index3_DependentData_Hash>::type::const_iterator  wodb_indexed3_hash_iterator_t;
typedef boost::multi_index::index<WorkOrderDataBase,index4_IndependentData_Hash>::type::const_iterator  wodb_indexed4_hash_iterator_t;
typedef boost::multi_index::index<WorkOrderDataBase,index5_Datum_Hash>::type::const_iterator  wodb_indexed5_hash_iterator_t;


void findABC(std::string);

class zwr::DBStructure
{
public:
    DBStructure();

//public slots:
    void addData(QString,QString,QString,QString,QString);
    void printWorkOrderParameters(QString woNum) const;

    bool stringIsParameterHeader(QString inString);
    bool validDependentParameter(QString);
    bool validIndependentParameter(QString);

private:
    WorkOrderDataBase data;
    std::map<std::string,int> workOrderParameterMap;
    QList<QString> validWorkOrderParameterNames;
};






#endif // WORKORDERDATUM_H
