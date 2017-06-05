#include "pdb2xyz_lib/include/Converter.h"
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;
using namespace PCA;

int main(int argc, char* argv[]){

try{
    if(argv[1]==NULL){
	cout<<"I need name of the polymer\n";
	exit(1);
    }
    
    ofstream fout("test.xyz");
    Converter converter(argv[1]);
    converter.allAtoms(fout,'A',3);
    
//    converter.check();

    fout.close();
}

catch(std::runtime_error e){
    cout << e.what();
}

return 0;
}