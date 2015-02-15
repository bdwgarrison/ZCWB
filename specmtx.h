#ifndef SPECMTX_H
#define SPECMTX_H

#include <QList>
#include <QString>
#include <vector>
#include <fstream>
class specMtx
{
public:
    specMtx(QList<QString>, char);

    std::vector<std::vector<double> > getMtx() {return mtx;}
private:
    std::vector<std::vector<double> > mtx;
    void getdata_init(std::ifstream&);
    void getdata_reg(std::ifstream&);
    void print();
    void normalize();
};

#endif // SPECMTX_H
