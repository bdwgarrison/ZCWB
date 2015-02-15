#ifndef STDMTX_H
#define STDMTX_H

#include <vector>

class stdMtx
{
public:
    stdMtx();
    std::vector<std::vector<double> > getMtx() {return mtx;}

private:
    std::vector<std::vector<double> > mtx;
};

#endif // STDMTX_H
