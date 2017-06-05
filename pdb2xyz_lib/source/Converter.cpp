#include "Converter.h"
#include "Utile.h"
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;
using namespace PCA;

Converter::Converter(string name_in){
    proteinName = name_in;
    
    string word;
    string line;
    double x, y, z;
    double resNumber;
    string elementSymbol;
    string atomName;
    char chainID;
    char previousChainID = 'A';
    string atom = "ATOM";
    string end = "END";
    string endmdl = "ENDMDL";
    string remark = "REMARK";
    string endOfModel = "ENDMDL";
    int count=0;
    int countInChain=0;
    
    cout<<"begin"<<"\n";
    vector<tuple<double, double, double, string, string, int, char>> oneModel;
    
    string filename(proteinName);
    filename += ".pdb";
    
    ifstream fin(filename);
    if(!fin){
	cout<<"Can't find the file\n";
	exit(1);
    }
    
    chainDelimiter.push_back(0);
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
//cout<<elementSymbol<<"\n"<<flush;
	    oneModel.push_back(make_tuple(x,y,z,elementSymbol,atomName,resNumber,chainID));
	    
	    if(chainID!=previousChainID){
		chainDelimiter.push_back(count);
		numAtomsInChain.push_back(count-countInChain);
		countInChain=count;
	    }
	    previousChainID = chainID;
	    count++;
	}
	
	if(word.compare(endOfModel)==0){
	    data.push_back(oneModel);
	    oneModel = vector<tuple<double, double, double, string, string, int, char>>();// clean
	}
    }while(word.compare(end)!= 0);
    chainDelimiter.push_back(count);
    numAtomsInChain.push_back(count-countInChain);
for(int i=0;i<numAtomsInChain.size();i++){
    cout<<Utile::abc(i+1)<<"\t"<<numAtomsInChain[i]<<"\n"<<flush;
}
    
    if(oneModel.size()!=0)
	data.push_back(oneModel);
	
    numModels = data.size();
    numAtoms = data[0].size();
    numChains = Utile::abc(get<6>(data[0][data[0].size()-1]));
cout<<numChains<<"  atoms\n";
    
    fin.close();
    cout<<"end"<<"\n";
}

Converter::~Converter(){};

int Converter::allAtoms(std::ofstream& fout, int chain, int model){
    
    int chainNum=0;
    
    //exeptions that chain and model valid
    
    //all chains in all models
    if(chain==0 && model==0){
	for(int k=0;k<data.size();k++){
	fout<<data[k].size()<<"\n";
	fout<<proteinName<<"\t"<<"MODEL "<<k+1<<"\n";
	    for(int i=0; i<data[k].size(); i++){
		fout<<get<3>(data[k][i])<<"\t"; // print element symbol
		fout<<get<0>(data[k][i])<<"\t"; // print x
		fout<<get<1>(data[k][i])<<"\t"; // print y
		fout<<get<2>(data[k][i])<<"\n"; // print z
	    }
	    fout<<"\n";
	}
    }
    
    //all chain in fixed model
    else if (chain == 0 && model!=0){// print all chains A, B, ...
	for(int i=0; i<data[model-1].size(); i++){
	    if(i==chainDelimiter[chainNum]){
		fout<<numAtomsInChain[chainNum]<<"\n";
		fout<<proteinName;
		if(numModels>1)
		    fout<<"\tMODEL "<<model;
		fout<<"\t"<<Utile::abc(chainNum+1);
		fout<<"\n";
		chainNum++;
	    }
	    fout<<get<3>(data[model-1][i])<<"\t"; // print element symbol
	    fout<<get<0>(data[model-1][i])<<"\t"; // print x
	    fout<<get<1>(data[model-1][i])<<"\t"; // print y
	    fout<<get<2>(data[model-1][i])<<"\n"; // print z
	}
    }
    
//    //one chain in all models
//    if else(

    else{
    
    }

return 0;
}

int Converter::check(){
    
    for(int k=0; k<data.size(); k++){
	cout<<"MODEL\t"<<k+1<<"\n\n";
	for(int i=0; i<data[k].size(); i++){
	    cout<<get<4>(data[k][i])<<"\t"; // print atom name
	    cout<<get<5>(data[k][i])<<"\t"; // print x
	    cout<<get<6>(data[k][i])<<"\t"; // print y

	    cout<<get<0>(data[k][i])<<"\t"; // print z
	    cout<<get<1>(data[k][i])<<"\t";
	    cout<<get<2>(data[k][i])<<"\t";

	    cout<<get<3>(data[k][i])<<"\n";
	}
    }
return 0;
}