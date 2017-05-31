#include "Converter.h"
#include <iomanip>
#include <iostream>

using namespace std;
using namespace PCA;

int Converter::everything(std::ifstream& fin, std::ofstream& fout){
    string word;
    string line;
    double value;
    string atom = "ATOM";
    string end = "END";
    string endmdl = "ENDMDL";
    int i;
    
    cout<<"begin"<<"\n";
    do{
	fin>>word;
//	cout<<word;
	if(word.compare(atom)==0){
	    fout<<word;
	    cout<<word;
	}
    }while(word.compare(end)!=NULL);
    
    cout<<"end"<<"\n";
return 0;
}