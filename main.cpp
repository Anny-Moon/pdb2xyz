#include "pdb2xyz_lib/include/Converter.h"
#include <fstream>
#include <iostream>
using namespace std;
using namespace PCA;

int main(){

    ifstream fin("1abs.pdb");
    ofstream fout("test.dat");
    
//    string word;
//    fin<<word;
//    cout>>word>>flush;

    Converter converter(fin, fout);

    fin.close();
    fout.close();
return 0;
}