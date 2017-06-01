#include "Converter.h"
#include <iomanip>
#include <iostream>

using namespace std;
using namespace PCA;

Converter::Converter(std::ifstream& fin, std::ofstream& fout){
    string word;
    string line;
    double x, y, z;
    double resNumber;
    string elementSymbol;
    string atomName;
    char chainIdent;
    string atom = "ATOM";
    string end = "END";
    string endmdl = "ENDMDL";
    
    cout<<"begin"<<"\n";

//cout<<fin.width()<<"\n";    
fin>>setw(15)>>word;
cout<<word<<"\n"<<flush;

//fin>>setw(0);
    do{
	fin>>word;
	if(word.compare(atom)==0){ //find "ATOM"
	
	    fin.ignore(8);
	    fin>>setw(4)>>atomName;
	    cout<<atomName<<"\t"<<flush;
	    fin.ignore(4+(4-atomName.size()));
	    fin>>chainIdent;
	    fin>>setw(4)>>resNumber;
	    cout<<chainIdent<<"\t"<<resNumber<<"\n"<<flush;
	    
//	    fin.ignore(24); //skip everything from ATOM to coordinates;
	    fin>>setw(8)>>x;
	    fin>>setw(8)>>y;
	    fin>>setw(8)>>z;
	    fin.ignore(22); //skip everything before atom name;
	    fin>>setw(2)>>elementSymbol;
	    
	    data.push_back(make_tuple(x,y,z,elementSymbol,"",0));
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