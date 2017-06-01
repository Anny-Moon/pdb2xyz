#include "Converter.h"
#include <iomanip>
#include <iostream>

using namespace std;
using namespace PCA;

int Converter::everything(std::ifstream& fin, std::ofstream& fout){
    string word;
    string line;
    double value;
    double x, y, z;
    string atomName;
    string atom = "ATOM";
    string end = "END";
    string endmdl = "ENDMDL";
    int i;
    
    cout<<"begin"<<"\n";
    do{
	fin>>word;
//	cout<<word;
	if(word.compare(atom)==0){ //find "ATOM"
	    fin.ignore(24); //skip everything before coordinates;
	    fin>>x;
	    fin>>y;
	    fin>>z;
	    fin.ignore(22); //skip everything before atom name;
	    fin>>atomName;
	    
	    cout<<atomName<<"\t";
	    cout<<x<<"\t";
	    cout<<y<<"\t";
	    cout<<z<<"\n";
	}
    }while(word.compare(end)!=NULL);
    
    cout<<"end"<<"\n";
return 0;
}