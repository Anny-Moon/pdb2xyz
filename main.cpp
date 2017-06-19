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
    Converter converter;
    converter.init(argv[1]);
    
    converter.print(fout,'A',0);
    
//    converter.check();

    fout.close();
    
    Converter *ca;
    ofstream fout2("testCA.xyz");
    ofstream fout3("testCApart.xyz");
    ca = converter.filterCA();
    ca->print(fout2,0,0);
    ca->printTheLongestPart(fout3,'A',0);
//ca->printTheLongestPart(fout3);
    fout2.close();
    fout3.close();
}

catch(std::runtime_error e){
    cout << e.what();
}

return 0;
}