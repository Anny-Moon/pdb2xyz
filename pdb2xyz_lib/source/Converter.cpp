#include "Converter.h"
#include "Keyword.h"
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
    string atomName="";
    char chainID;
    char previousChainID = 'A';
    int count=0;
    int countSave = 0;
    int countInChain=0;
    int countInChainSave=0;
    char a,b,c,d;
    
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

    //read the first word to be sure that we got PDB file, not something else
    getline(fin, line);
    stringstream sin(line);
    sin>>word;
    if(word.compare(Keyword::header)!=0){
	string error = "Error: Cannot find protein with name '";
	error += proteinName + "'.\n";
	throw runtime_error(error);
    }
//    data.push_back(vector<tuple<double, double, double, string, string, int, char>>());
    int model = 0;
    chainDelimiter.push_back(0);
    
    do{
	getline(fin, line);
	stringstream sin(line);
	sin>>word;
	
	if(word.compare(Keyword::atom)==0){ //find "ATOM"
//cout<<word<<"\t";
	    sin.ignore(8);
//	    sin>>setw(4)>>noskipws>>atomName>>skipws;
sin>>noskipws>>a>>b>>c>>d>>skipws;
//cout<<a<<b<<c<<d<<"||\t";
if(!isblank(a))
atomName+=a;
if(!isblank(b))
atomName+=b;
if(!isblank(c))
atomName+=c;
if(!isblank(d))
atomName+=d;
//cout<<"|"<<atomName<<"|\n";

//	    sin.ignore(4+(4-atomName.size()));
sin.ignore(4);
	    sin>>chainID;
//cout<<chainID<<"\n";
	    sin>>setw(4)>>resNumber;
//	    sin.ignore(24); //skip everything from ATOM to coordinates;
	    sin>>setw(8)>>x;
	    sin>>setw(8)>>y;
	    sin>>setw(8)>>z;
	    sin.ignore(22); //skip everything before atom name;
	    sin>>setw(2)>>elementSymbol;
	    
	    oneModel.push_back(make_tuple(x,y,z,elementSymbol,atomName,resNumber,chainID));
//	    data[model].push_back(make_tuple(x,y,z,elementSymbol,atomName,resNumber,chainID));
atomName="";
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
	
	if(word.compare(Keyword::endOfModel)==0){
//	    cout<<model<<"model\n"<<flush;
	    data.push_back(oneModel);
	    oneModel = vector<tuple<double, double, double, string, string, int, char>>();// clean
	    model++;
//	    data.push_back(vector<tuple<double, double, double, string, string, int, char>>());
	    previousChainID = 'A';
	    count = 0;
	    countInChain = 0;
	}
    }while(word.compare(Keyword::end)!= 0);
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
    numAtoms = data[0].size();
    numChains = Utile::abc(get<6>(data[0][data[0].size()-1]));
    
    cout<<"Number of models: "<<numModels<<"\n";
    cout<<"Number of atoms: "<<numAtoms<<"\n";
    cout<<"Number of chains: "<<numChains<<" :\n";
    for(int i=0;i<numAtomsInChain.size();i++){
	cout<<Utile::abc(i+1)<<"\t"<<numAtomsInChain[i]<<"\tatoms\n"<<flush;
    }
    
//    fin.close();
//    cout<<"end"<<"\n";
return 0;
}



int Converter::print(std::ofstream& fout, char chain, int model){
    
    int chainNum=0;
    
    //check that chain and model are valid
    checkModelNumber(model);
    checkChainNumber(chain);
    
    //all chains in all models
    if(chain =='0' && model==0){
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
		//gfout<<"\n";
	    }
	}
    }
    
    //all chains in fixed model
    else if (chain == '0' && model!=0){// print all chains A, B, ...
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
    else if(chain != '0' && model==0){
	
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
    
    int countAtomsInChain=0;
    
    for(int model=0;model<numModels;model++){
    ca->data.push_back(vector<tuple<double, double, double, string, string, int, char>>());
	for(int chain=0;chain<numChains;chain++){
	    countAtomsInChain=0;
	    for(int i=chainDelimiter[chain];i<chainDelimiter[chain+1];i++){
		if((get<4>(data[model][i])).compare("CA")==0){
		    ca->data[model].push_back(data[model][i]);
		    if(model==0){
			ca->numAtoms++;
			countAtomsInChain++;
		    }
		}
	    }
	    
	    if(model==0){
		ca->numAtomsInChain.push_back(countAtomsInChain);
		ca->chainDelimiter.push_back(ca->numAtoms);
		
	    }
	}
	//oneModel = vector<tuple<double, double, double, string, string, int, char>>();// clean
    }
    
    cout<<"\nC's alpha only:\n";
    cout<<"Number of models: "<<ca->numModels<<"\n";
    cout<<"Number of atoms: "<<ca->numAtoms<<"\n";
    cout<<"Number of chains: "<<ca->numChains<<" :\n";
//    cout<<"Chain name and number of atoms in:\n";
    for(int i=0;i<ca->numChains;i++){
	cout<<Utile::abc(i+1)<<"\t"<<ca->numAtomsInChain[i]<<"\tatoms\n"<<flush;
    }
return ca;
}

bool Converter::printTheLongestPart(std::ofstream& fout, char chain, int model){
    vector<int> atomsInPart;
    vector<int> partDelimiter;
    vector<int> repeatedAtom;
    
    int chainNum = Utile::abc(chain)-1;
    int count;
    int countAtoms=0;
    int resNum;
    string atomName;
    string previousAtomName = "";
    int tmp_model;
    
    int haveMissings = true; 
    int havingRepeats = false;
    
    //check that chain and model are valid
    checkModelNumber(model);
    checkChainNumber(chain);

    tmp_model = model-1;

    resNum = get<5>(data[tmp_model][chainDelimiter[chainNum]]);
    partDelimiter.push_back(0);
    count = resNum;
    countAtoms++;

    for(int i=chainDelimiter[chainNum]+1;i<chainDelimiter[chainNum+1];i++){
	resNum = get<5>(data[tmp_model][i]);
	atomName = get<4>(data[tmp_model][i]);
cout<<resNum<<" "<<atomName<<"\n";
	//find repeated atoms and don't count them!
	if(resNum==count && atomName.compare(previousAtomName)==0){ 
	    cout<<resNum<<" !!\n"<<flush;
	    repeatedAtom.push_back(i);
	    count--;
	    countAtoms--;
	}
	
	if(resNum==count && atomName.compare(previousAtomName)!=0){ 
	    count--;
	}
	
	if(resNum>count+1){
	    atomsInPart.push_back(countAtoms);
	    partDelimiter.push_back(i);
	    countAtoms=0;
	    count=resNum-1;
cout<<resNum<<" Missing before\n"<<flush;
	}
	countAtoms++;
	count++;
	previousAtomName = atomName;
	
    }
    partDelimiter.push_back(resNum);
    
    if (partDelimiter.size()==2){
	haveMissings = false;
	atomsInPart.push_back(numAtoms);
    }
    
    if(repeatedAtom.size()!=0)
	havingRepeats = true;
   
    int numParts = atomsInPart.size();
    int maxPart = 0;

    // find the longest part
    if(haveMissings){
	for(int i=1; i<numParts;i++){
	    if(atomsInPart[i]>atomsInPart[maxPart])
	    maxPart = i;
	}
    }
    
    // print the longest part without repeated atoms
    fout<<atomsInPart[maxPart]<<"\n";
    fout<<proteinName;
    if(numModels>1)
	fout<<"\tMODEL "<<model;
    fout<<"\t"<<chain;
    if(haveMissings){
	fout<<"\tpart from atom "<<(get<5>(data[model-1][partDelimiter[maxPart]]));
	fout<<" to "<<(get<5>(data[model-1][partDelimiter[maxPart+1]]));
    }
    fout<<"\n";
    int countRepeat = 0;
    for(int i=partDelimiter[maxPart]; i<partDelimiter[maxPart+1]; i++){

	//don't print repeated atoms
	if(havingRepeats){
	    if(repeatedAtom[countRepeat]==i){
		countRepeat++;
		continue;
	    }
	}
	//------
	
	fout<<get<3>(data[model-1][i])<<"\t"; // print element symbol
	fout<<get<0>(data[model-1][i])<<"\t"; // print x
	fout<<get<1>(data[model-1][i])<<"\t"; // print y
	fout<<get<2>(data[model-1][i])<<"\n"; // print z
    }

return haveMissings;
}



