#include "Converter.h"
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;
using namespace PCA;

Converter::Converter(std::ifstream& fin){
    string word;
    string line;
    double x, y, z;
    double resNumber;
    string elementSymbol;
    string atomName;
    char chainID;
    string atom = "ATOM";
    string end = "END";
    string endmdl = "ENDMDL";
    string remark = "REMARK";
    int count=0;
    cout<<"begin"<<"\n";

    do{
//	fin>>word;
	getline(fin, line);
	stringstream sin(line);
	sin>>word;
	if(word.compare(atom)==0){ //find "ATOM"
	    sin.ignore(8);
	    sin>>setw(4)>>atomName;

	    sin.ignore(4+(4-atomName.size()));
	    sin>>chainID;
	    sin>>setw(4)>>resNumber;
//	    sin.ignore(24); //skip everything from ATOM to coordinates;
	    sin>>setw(8)>>x;
	    sin>>setw(8)>>y;
	    sin>>setw(8)>>z;
	    sin.ignore(22); //skip everything before atom name;
	    sin>>setw(2)>>elementSymbol;
	    data.push_back(make_tuple(x,y,z,elementSymbol,atomName,resNumber,chainID));
	}
	count++;
    }while(word.compare(end)!= 0);
    
    
//    fout.seekp(ios_base::beg); //return the carrege to the beginning of the file
//    fout<<count<<"\n";
    cout<<"end"<<"\n";
}

Converter::~Converter(){};

int Converter::allAtoms(std::ofstream& fout){
    
    fout<<data.size()<<"\n";
    fout<<"comment\n";
    for(int i=0; i<data.size(); i++){
	fout<<get<3>(data[i])<<"\t"; // print atom name
	fout<<get<0>(data[i])<<"\t"; // print x
	fout<<get<1>(data[i])<<"\t"; // print y
	fout<<get<2>(data[i])<<"\n"; // print z
	
//	cout<<get<4>(data[i])<<"\t";
//	cout<<get<6>(data[i])<<"\t";
//	cout<<get<5>(data[i])<<"\n";
    }
return 0;
}

int Converter::check(){
    
    cout<<data.size()<<"\n";
    for(int i=0; i<data.size(); i++){
	cout<<get<4>(data[i])<<"\t"; // print atom name
	cout<<get<5>(data[i])<<"\t"; // print x
	cout<<get<6>(data[i])<<"\t"; // print y

	cout<<get<0>(data[i])<<"\t"; // print z
	cout<<get<1>(data[i])<<"\t";
	cout<<get<2>(data[i])<<"\t";

	cout<<get<3>(data[i])<<"\n";
    }
return 0;
}