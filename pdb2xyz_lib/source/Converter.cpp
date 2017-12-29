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

Converter::Format Converter::outputFormat;

Converter::Converter(){};
Converter::~Converter(){};

int Converter::init(string name_in, string pathToFile){
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
    char letter1,letter2,letter3,letter4;
    
    vector<tuple<double, double, double, string, string, int, char>> oneModel;
    
    stringstream fin;
    if(pathToFile.compare("")!=0){
	string filename(pathToFile);
	filename += proteinName;
	filename += ".pdb";
	ifstream file(filename);
	fin<<file.rdbuf();
	file.close();
    }
    
    else{
	cout<<"Downloading data from the Internet...\n";
	string webData;
	string url = "https://files.rcsb.org/view/"+proteinName+".pdb";
	FindByURL(url, &webData);
//	stringstream fin(webData);
	stringstream web(webData);
	fin<<web.rdbuf();
    }
    
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
	    sin.ignore(8);
	    
	    sin>>noskipws>>letter1>>letter2>>letter3>>letter4>>skipws;
	    if(!isblank(letter1))
		atomName+=letter1;
	    if(!isblank(letter2))
		atomName+=letter2;
	    if(!isblank(letter3))
		atomName+=letter3;
	    if(!isblank(letter4))
		atomName+=letter4;

	    sin.ignore(4);
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
    cout<<"PROTEIN: "<<proteinName<<"\n";
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

void Converter::fileBlock(int modelNum, int chainNum, std::ofstream& fout){
    fileHeader(modelNum, chainNum, fout);
    for(int i=chainDelimiter[chainNum]; i<chainDelimiter[chainNum+1]; i++)
	fileLine(modelNum, i, fout);
}

void Converter::xyzBlock(int modelNum, int chainNum, std::ofstream& fout){
    xyzHeader(modelNum, chainNum, fout);
    for(int i=chainDelimiter[chainNum]; i<chainDelimiter[chainNum+1]; i++)
	xyzLine(modelNum, i, fout);
}
/*
void Converter::pcaBlock(int modelNum, int chainNum, std::ofstream& fout){
    for(int i=chainDelimiter[chainNum]; i<chainDelimiter[chainNum+1]; i++)
	fileLine(modelNum, i, fout);
}
*/
inline void Converter::tbmHeader(int modelNum, int chainNum, std::ofstream& fout, int numberOfAtoms, std::string message){
	fout<<"\\* ";
	fout<<proteinName;
	fout<<"\t"<<Utile::abc(chainNum+1);
	if(numModels>1)
	    fout<<"\tMODEL "<<modelNum+1;
	fout<<"\n * Number of atoms: ";
	
	if(numberOfAtoms==0){
	    numberOfAtoms = numAtomsInChain[chainNum];
	}
	
	fout<<numberOfAtoms<<"\n";
	
	fout<<" * "<<message;
	fout<<" *\\ \n";
	
	fout<<"Amplitudes:\n";
	fout<<"Mode = 0\n";
    
	for(int n = 0; n < numberOfAtoms; n++){
	    fout<<"1\t0\t["<<n<<"]\t["<<n+1<<"]\n";
	    fout<<"1\t0\t["<<n+1<<"]\t["<<n<<"]\n";
	}
	
	fout<<"\n";
	fout<<"Geometry:\n";
	fout<<"Dimensions = 3\n";
	fout<<"Num specifiers = 0\n";
}

int Converter::print(std::ofstream& fout, char chain, int model){
    
    int chainNum=0;
    int modelNum=0;
    //check that chain and model are valid
    checkModelNumber(model);
    checkChainNumber(chain);
    
    // fixed chain in fixed model
    if(chain != '0' && model != 0){
	chainNum = Utile::abc(chain)-1;
	modelNum = model-1;
	fileBlock(modelNum, chainNum, fout);
    }
    
    //fixed chain in all models
    else if(chain != '0' && model==0){
	chainNum = Utile::abc(chain)-1;
	switch(outputFormat){
	    case Format::xyz:
		for(int modelNum=0; modelNum<numModels; modelNum++)
		    fileBlock(modelNum, chainNum, fout);
	    break;
	    
	    case Format::pca:
		for(int modelNum=0; modelNum<numModels; modelNum++){
		    fileBlock(modelNum, chainNum, fout);
		    fout<<"\n\n";
		}
	    break;
	    
	    default:
		string error("\nError: This format does not");
		error+=" support multy model output.\n";
		throw runtime_error(error);
	}
    }
    
    //all chains in fixed model
    else if (chain == '0' && model!=0){
	modelNum = model-1;
	switch (outputFormat){
	    case Format::xyz:
		for(int chainNum=0; chainNum<numChains; chainNum++)
		    xyzBlock(modelNum, chainNum, fout);
		
	    break;
	    default:
		string error("\nError: This format does not");
		error+=" support multy chain output.\n";
		throw runtime_error(error);
	}
    }
    
    //all chains in all models
    else{
	switch(outputFormat){
	    case Format::xyz:
		for(int modelNum=0;modelNum<numModels;modelNum++){
		    for(int chainNum=0;chainNum<numChains;chainNum++)
		    xyzBlock(modelNum, chainNum, fout);
		}
	    break;
	    default:
		string error("\nError: This format does not");
		error+=" support multy chain output.\n";
		throw runtime_error(error);
	}
    }
return 0;
}

Converter* Converter::filter(string atomName){
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
		if((get<4>(data[model][i])).compare(atomName)==0){
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
    
    if(ca->numAtoms==0){
	std::string error("\nError: There is no atoms ");
	error+=atomName;
	error+=" in ";
	error+=proteinName;
	error+=".\n";
	throw std::runtime_error(error);
    }
    
    cout<<"\nFilter: "<<atomName<<"\n";
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
    
    //check that chain and model are valid
    checkModelNumber(model);
    checkChainNumber(chain);
    doesNotSupportMultyModel(model, "--part");
    doesNotSupportMultyChain(chain, "--part");
    
    vector<int> atomsInPart;
    vector<int> partDelimiter;
    vector<int> repeatedAtom;
    
    int chainNum = Utile::abc(chain)-1;
    int modelNum = model-1;
    
    int count;
    int countAtoms=0;
    int resNum;
    string atomName;
    string previousAtomName = "";
    
    int haveMissings = true; 
    int havingRepeats = false;
    
    
    resNum = get<5>(data[modelNum][chainDelimiter[chainNum]]);
    partDelimiter.push_back(0);
    count = resNum;
    countAtoms++;

    for(int i=chainDelimiter[chainNum]+1;i<chainDelimiter[chainNum+1];i++){
	resNum = get<5>(data[modelNum][i]);
	atomName = get<4>(data[modelNum][i]);
//cout<<resNum<<" "<<atomName<<"\n";
	//missing
	if(resNum>count+1){
	    atomsInPart.push_back(countAtoms);
	    partDelimiter.push_back(i);
	    countAtoms=0;
	    count=resNum-1;
//cout<<resNum<<" Missing before\n"<<flush;
	}
	
	//find repeated atoms and don't count them!
	if(resNum==count && atomName.compare(previousAtomName)==0){ 
//	    cout<<resNum<<" !!\n"<<flush;
	    repeatedAtom.push_back(i);
	    count--;
	    countAtoms--;
	}
	
	if(resNum==count && atomName.compare(previousAtomName)!=0){ 
//	    countAtoms++;
	    count--;
	}
	
	countAtoms++;
	count++;
	previousAtomName = atomName;
    }
    partDelimiter.push_back(chainDelimiter[chainNum+1]);
    atomsInPart.push_back(countAtoms);

    if (partDelimiter.size()==2){
	haveMissings = false;
//	atomsInPart.push_back(numAtoms);
    }
    
    if(repeatedAtom.size()!=0){
	havingRepeats = true;
	cout<<"\n\tREPEATED ATOMS WAS EXCLUDED.\n";
	cout<<"Number of atoms: "<<countAtoms<<"\n";
    }
    
    else
	cout<<"\nNo repeated atoms.\n";
    int numParts = atomsInPart.size();
//cout<<numParts<<" numparts\n\n";
    int maxPart = 0;

    // find the longest part
    if(haveMissings){
	cout<<"Parts without missings:\n";
	for(int i=0; i<numParts;i++){
	    cout<<"\t"<<i<<". from: "<<get<5>(data[modelNum][partDelimiter[i]])<<"\t";
	    cout<<" to: "<<get<5>(data[modelNum][partDelimiter[i+1]-1])<<"\t";
	    cout<<"with "<<atomsInPart[i]<<" atoms.\n";
	    if(atomsInPart[i]>atomsInPart[maxPart])
		maxPart = i;
	}
	cout<<"I took part number: "<<maxPart<<"\n";
    
    }
    
    else
	cout<<"No missing residues.\n";
    // print the longest part without repeated atoms
/*    fout<<atomsInPart[maxPart]<<"\n";
    fout<<proteinName;
    if(numModels>1)
	fout<<"\tMODEL "<<model;
    fout<<"\t"<<chain;
    if(haveMissings){
	fout<<"\tpart from atom "<<(get<5>(data[model-1][partDelimiter[maxPart]]));
	fout<<" to "<<(get<5>(data[model-1][partDelimiter[maxPart+1]]));
    }
    fout<<"\n";
*/
//cout<<get<5>(data[modelNum][partDelimiter[maxPart]])<<" "<<get<5>(data[modelNum][partDelimiter[maxPart+1]-1])<<"\n";
    string message = "\tpart from residue ";
    message+= to_string(get<5>(data[modelNum][partDelimiter[maxPart]]));
    message+=" to ";
    message+= to_string(get<5>(data[modelNum][partDelimiter[maxPart+1]-1]));
    
    fileHeader(modelNum, chainNum, fout, atomsInPart[maxPart], message);
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
	fileLine(modelNum, i, fout);
    }

return haveMissings;
}



