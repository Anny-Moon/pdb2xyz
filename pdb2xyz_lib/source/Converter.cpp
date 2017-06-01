#include "Converter.h"
#include <iomanip>
#include <iostream>

using namespace std;
using namespace PCA;

Converter::Converter(std::ifstream& fin, std::ofstream& fout){
    string word;
    string line;
    double x, y, z;
    string atomName;
    string atom = "ATOM";
    string end = "END";
    string endmdl = "ENDMDL";
    
    cout<<"begin"<<"\n";
    do{
	fin>>word;
	if(word.compare(atom)==0){ //find "ATOM"
	    fin.ignore(24); //skip everything before coordinates;
	    fin>>x;
	    fin>>y;
	    fin>>z;
	    fin.ignore(22); //skip everything before atom name;
	    fin>>atomName;
	    
	    data.push_back(make_tuple(x,y,x,atomName));
	}
    }while(word.compare(end)!= 0);
    
    
    fout<<data.size()<<"\n";
    fout<<"comment\n";
    for(int i=0; i<data.size(); i++){
	fout<<get<3>(data[i])<<"\t"; // print atom name
	fout<<get<0>(data[i])<<"\t"; // print x
	fout<<get<1>(data[i])<<"\t"; // print y
	fout<<get<2>(data[i])<<"\n"; // print z
    }
    
//    fout.seekp(ios_base::beg); //return the carrege to the beginning of the file
//    fout<<count<<"\n";
    cout<<"end"<<"\n";
}

Converter::~Converter(){};