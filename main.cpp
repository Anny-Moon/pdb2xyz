#include "pdb2xyz_lib/include/Converter.h"
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;
using namespace PCA;

int main(int argc, char* argv[]){

    if(argv[1]==NULL){
	cout<<"I need name of the polymer\n";
	exit(1);
    }
    
    string nameIn(argv[1]);
    nameIn += ".pdb";
    
    ifstream fin(nameIn);
    if(!fin){
	cout<<"Can't find the file\n";
	exit(1);
    }
    ofstream fout("test.xyz");
    
//    string word;
//    fin<<word;
//    cout>>word>>flush;

    Converter converter(fin);
    converter.allAtoms(fout);
    converter.check();

    fin.close();
    fout.close();
return 0;
}