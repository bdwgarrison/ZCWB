#include "specmtx.h"
#include "stdmtx.h"
#include <string>
#include <iostream>
#include <QString>
#include <vector>

using namespace std;

void specMtx::getdata_init(ifstream& infile)
{
    string inword;
    bool DATA_unformatted = 0;
    while( infile >> inword )
    {
        if(inword == "#DATA")
        {
            DATA_unformatted=1;
            while( infile >> inword)
            {
                mtx[0].push_back( QString(inword.c_str()).toDouble() );
                infile>>inword;
                mtx[1].push_back( QString(inword.c_str()).toDouble() );
            }
            break;
        }
    }
    if (DATA_unformatted == 0)
    {
        infile.clear();
        infile.seekg(0, ios::beg);
        while( infile >> inword)
        {
            mtx[0].push_back( QString(inword.c_str()).toDouble() );
            infile>>inword;
            mtx[1].push_back( QString(inword.c_str()).toDouble() );
        }
    }
}

void specMtx::getdata_reg(ifstream& infile)
{
    vector<double> newv;
    string inword;
    bool DATA_unformatted = 0;
    while( infile >> inword)
    {
        if(inword == "#DATA")
        {
            DATA_unformatted=1;
            while( infile >> inword)
            {
                infile >> inword;
                newv.push_back(QString(inword.c_str()).toDouble());
            }
            break;
        }
    }

    if (DATA_unformatted == 0)
    {
        infile.clear();
        infile.seekg(0, ios::beg);
        while( infile >> inword)
        {
            mtx[0].push_back( QString(inword.c_str()).toDouble() );
            infile>>inword;
            mtx[1].push_back( QString(inword.c_str()).toDouble() );
        }
    }
    mtx.push_back(newv);
}

void specMtx::print()
{
    long unsigned int cols = mtx.size();
    long unsigned int rows = mtx[0].size();
    for(long unsigned k=0; k<rows; k++)
    {
        for(long unsigned j=0; j<cols; j++)
        {
            cout<< mtx[j][k]<< "     ";
        }
        cout<<endl;
    }
}

void specMtx::normalize()
{
    stdMtx stdmtx;
    vector<vector<double> > stdata = stdmtx.getMtx();

    unsigned long mtx_back = mtx[0].size()-1;
    unsigned long stdata_back = stdata[0].size()-1;

    /** starts at back entry of data matrix, which is smallest size wave length,
     and clears all entries less than the minimum of the standard**/
    while ( mtx[0][mtx_back] < stdata[0][stdata_back] )
    {
        for(long unsigned k=0; k< mtx.size(); k++)
            mtx[k].pop_back();
        mtx_back--;
    }

    while ( mtx[0][mtx_back] > stdata[0][stdata_back] )
    {
        for(int k=0; k<2; k++)
            stdata[k].pop_back();
        stdata_back--;
    }

    /** flip matrices over **/
    for(long unsigned k=0; k<mtx.size(); k++)
    {
        reverse(mtx[k].begin(), mtx[k].end());
    }
    reverse(stdata[0].begin(), stdata[0].end());
    reverse(stdata[1].begin(), stdata[1].end());

    /** now sheer off the largest numbers, making the
     standard and data have the same number of entries **/
    while (mtx[0][mtx_back] > stdata[0][stdata_back])
    {
        for(long unsigned k=0; k< mtx.size(); k++)
            mtx[k].pop_back();
        mtx_back--;
    }
    while (mtx[0][mtx_back] < stdata[0][stdata_back])
    {
        for(int k=0; k<2; k++)
            stdata[k].pop_back();
        stdata_back--;
    }

    for(long unsigned k=0; k < mtx_back+1; k++)
    {
        for(long unsigned j=1; j<mtx.size(); j++)
        {
            mtx[j][k] = mtx[j][k] * stdata[1][k] / 100;
        }
    }

}

specMtx::specMtx(QList<QString> argv, char RorT)
{
    int argc = argv.size();
    vector<ifstream*> infiles(argc);

    for (int k=0; k<argc; k++)
        infiles[k] = new ifstream( argv[k].toUtf8().constData() );


    mtx = vector<vector<double> > (2);

    getdata_init(*infiles[0]);

    for(int k=1; k<argc; k++)
    {
        getdata_reg(*infiles[k]);
    }

    if (RorT == 'R')
        //vector<vector<double> > stdata = create_std();
        normalize();

    else ///if (RorT == 'T') is unnecessary
        for(int k=0; k<mtx.size(); k++)
        {
            reverse(mtx[k].begin(), mtx[k].end());
        }

}
