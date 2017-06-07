#include "Converter.h"
#include "Utile.h"
#include "FindByURL.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <exception>

using namespace std;
using namespace PCA;

Converter::Converter(){};
Converter::~Converter(){};

int Converter::init(string name_in){
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
    int countSave = 0;
    int countInChain=0;
    int countInChainSave=0;
    
    cout<<"begin"<<"\n";
    vector<tuple<double, double, double, string, string, int, char>> oneModel;
    
//    string filename(proteinName);
//    filename += ".pdb";
//    ifstream fin(filename);
    string webData;
    string url = "https://files.rcsb.org/view/"+proteinName+".pdb";
    
    FindByURL(url, &webData);
    
    stringstream fin(webData);
    
    if(!fin)
	throw runtime_error("Can't open the file\n");

//    data.push_back(vector<tuple<double, double, double, string, string, int, char>>());
    int model = 0;
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
	    
	    oneModel.push_back(make_tuple(x,y,z,elementSymbol,atomName,resNumber,chainID));
//	    data[model].push_back(make_tuple(x,y,z,elementSymbol,atomName,resNumber,chainID));
	    
	    if(chainID!=previousChainID){
		if(model==0){
		    chainDelimiter.push_back(count);
		    numAtomsInChain.push_back(count-countInChain);
		    countInChain=count;
		    countInChainSave=countInChain;
		}
	    }
	    previousChainID = chainID;
	    count++;
	    countSave=count;
	}
	
	if(word.compare(endOfModel)==0){
	    cout<<model<<"model\n"<<flush;
	    data.push_back(oneModel);
	    oneModel = vector<tuple<double, double, double, string, string, int, char>>();// clean
	    model++;
//	    data.push_back(vector<tuple<double, double, double, string, string, int, char>>());
	    previousChainID = 'A';
	    count = 0;
	    countInChain = 0;
	}
    }while(word.compare(end)!= 0);
    chainDelimiter.push_back(countSave);
    numAtomsInChain.push_back(countSave-countInChainSave);

    
    if(oneModel.size()!=0)
	data.push_back(oneModel);
//cout<<data[countSave].size();
//    if(data[countSave].size()==0){
//	data.erase(data.begin() + countSave);
//	cout<<"yes\n";
//    }

    numModels = data.size();
cout<<numModels<<flush;
    numAtoms = data[0].size();
    numChains = Utile::abc(get<6>(data[0][data[0].size()-1]));
    
    cout<<"Number of models:"<<numModels<<"\n";
    cout<<"Number of chains:"<<numChains<<"\n";
    cout<<"Number of atoms:"<<numAtoms<<"\n";
    for(int i=0;i<numAtomsInChain.size();i++){
	cout<<Utile::abc(i+1)<<"\t"<<numAtomsInChain[i]<<"\n"<<flush;
    }
    
//    fin.close();
//    cout<<"end"<<"\n";
return 0;
}



int Converter::print(std::ofstream& fout, char chain, int model){
    
    int chainNum=0;
    
    //exeptions that chain and model are valid
    if(Utile::abc(chain)>numberOfChains()){
	string error("Error: There is no chain ");
	error+=chain;
	error+=" in ";
	error+=proteinName;
	error+=".\n";
	throw runtime_error(error);
    }
    
    if(model>data.size()){
	string error("Error: There is no MODEL ");
	error+=to_string(model);
	error+=" in ";
	error+=proteinName;
	error+=".\nNote: the first model has number 1 (not 0).\n";
	throw runtime_error(error);
    }
    
    
    //all chains in all models
    if(chain==0 && model==0){
	for(int model=0;model<numModels;model++){
	    for(int chain=0;chain<numChains;chain++){
		fout<<numAtomsInChain[chain]<<"\n";
		fout<<proteinName;
		if(numModels>1)
		    fout<<"\tMODEL "<<model+1;
		fout<<"\t"<<Utile::abc(chain+1);
		fout<<"\n";
		    for(int i=chainDelimiter[chain]; i<chainDelimiter[chain+1]; i++){
			fout<<get<3>(data[model][i])<<"\t"; // print element symbol
			fout<<get<0>(data[model][i])<<"\t"; // print x
			fout<<get<1>(data[model][i])<<"\t"; // print y
			fout<<get<2>(data[model][i])<<"\n"; // print z
		    }
		fout<<"\n";
	    }
	}
    }
    
    //all chains in fixed model
    else if (chain == 0 && model!=0){// print all chains A, B, ...
	for(int chain=0; chain<numChains; chain++){
	    fout<<numAtomsInChain[chain]<<"\n";
	    fout<<proteinName;
	    if(numModels>1)
		fout<<"\tMODEL "<<model;
	    fout<<"\t"<<Utile::abc(chain+1);
	    fout<<"\n";
	    for(int i=chainDelimiter[chain]; i<chainDelimiter[chain+1]; i++){
		fout<<get<3>(data[model-1][i])<<"\t"; // print element symbol
		fout<<get<0>(data[model-1][i])<<"\t"; // print x
		fout<<get<1>(data[model-1][i])<<"\t"; // print y
		fout<<get<2>(data[model-1][i])<<"\n"; // print z
	    }
	}
    }
    
    //fixed chain in all models
    else if(chain != 0 && model==0){
	
	for(int model=0; model<numModels; model++){
	    chainNum = Utile::abc(chain)-1;
	    fout<<numAtomsInChain[chainNum]<<"\n";
	    fout<<proteinName;
	    if(numModels>1)
		fout<<"\tMODEL "<<model+1;
	    fout<<"\t"<<chain;
	    fout<<"\n";
	    
	    for(int i=chainDelimiter[chainNum];i<chainDelimiter[chainNum+1];i++){
		fout<<get<3>(data[model][i])<<"\t"; // print element symbol
		fout<<get<0>(data[model][i])<<"\t"; // print x
		fout<<get<1>(data[model][i])<<"\t"; // print y
		fout<<get<2>(data[model][i])<<"\n"; // print z
	    }
	    
	}
    
    }

    // fixed chain in fixed model
    else{
	chainNum = Utile::abc(chain)-1;
	fout<<numAtomsInChain[chainNum]<<"\n";
	fout<<proteinName;
	if(numModels>1)
	    fout<<"\tMODEL "<<model;
	fout<<"\t"<<chain;
	fout<<"\n";
	
	for(int i=chainDelimiter[chainNum]; i<chainDelimiter[chainNum+1]; i++){
	    fout<<get<3>(data[model-1][i])<<"\t"; // print element symbol
	    fout<<get<0>(data[model-1][i])<<"\t"; // print x
	    fout<<get<1>(data[model-1][i])<<"\t"; // print y
	    fout<<get<2>(data[model-1][i])<<"\n"; // print z
	}
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

Converter* Converter::filterCA(){
    Converter *ca = new Converter();
    ca->proteinName = proteinName;
    ca->numModels = numModels;
    ca->numChains = numChains;
    
    ca->numAtoms = 0;
    ca->chainDelimiter.push_back(0);
    
    for(int model=0;model<numModels;model++){
    ca->data.push_back(vector<tuple<double, double, double, string, string, int, char>>());
	for(int chain=0;chain<numChains;chain++){
	    for(int i=chainDelimiter[chain];i<chainDelimiter[chain+1];i++){
		if((get<4>(data[model][i])).compare("CA")==0){
		    ca->data[model].push_back(data[model][i]);
		    if(model==0)
			ca->numAtoms++;
		}
	    }
	    
	    if(model==0){
		ca->numAtomsInChain.push_back(ca->numAtoms);
		
		ca->chainDelimiter.push_back(ca->numAtoms);
		
	    }
	}
	//oneModel = vector<tuple<double, double, double, string, string, int, char>>();// clean
    }
    
    cout<<"C alpha only:\n";
    cout<<"Number of models:"<<ca->numModels<<"\n";
    cout<<"Number of chains:"<<ca->numChains<<"\n";
    cout<<"Number of atoms:"<<ca->numAtoms<<"\n";
    for(int i=0;i<ca->numAtomsInChain.size();i++){
	cout<<Utile::abc(i+1)<<"\t"<<ca->numAtomsInChain[i]<<"\n"<<flush;
    }
return ca;
}