#include "Utile.h"
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <exception>

#ifndef PDB2XYZ_CONVERTER
#define PDB2XYZ_CONVERTER

namespace PCA{

class Converter{
private:
public:

    std::string proteinName;
    /** x, y, z, <element symbol> <atom name> <residue nuber> <chainID>*/
    std::vector<std::vector<std::tuple <double, double, double, std::string, std::string, int, char>>>data;
    int numModels;
    int numAtoms;
    int numChains;
    std::vector<int> numAtomsInChain;
    
    /** first atom of 0th chain (always 0), first atom of 1st chain ...  last atom+1 of last chain */
    std::vector<int> chainDelimiter;
    
    
//public:
    Converter();
    ~Converter();
    
    int init(std::string name_in);
    int print(std::ofstream& fout, char chain=0, int model=1);
    bool printTheLongestPart(std::ofstream& fout, char chain='A', int model=1);
    
    Converter* filter(std::string atomName="CA");
    int check();
    
    inline void xyzHeader(int modelNum, int chainNum, std::ofstream& fout, int numberOfAtoms=0, std::string message = "");
    /**@param modelNum - actual number (0,1..) @param chain - 0,1,.. */
    inline void xyzLine(int modelNum, int chainNum, std::ofstream& fout);
    void xyzBlock(int modelNum, int chainNum, std::ofstream& fout);
    
    /** Throw exeptions*/
    inline void checkModelNumber(int model);
    inline void checkChainNumber(char chain);
};

inline void Converter::xyzHeader(int modelNum, int chainNum, std::ofstream& fout, int numberOfAtoms, std::string message){
	if(numberOfAtoms==0)
	    fout<<numAtomsInChain[chainNum]<<"\n";
	
	else
	    fout<<numberOfAtoms<<"\n";
	
	fout<<proteinName;
	fout<<"\t"<<Utile::abc(chainNum+1);
	if(numModels>1)
	    fout<<"\tMODEL "<<modelNum+1;
	fout<<"\t"<<message;
	fout<<"\n";
}

inline void Converter::xyzLine(int modelNum, int chainNum, std::ofstream& fout){
    fout<<std::get<3>(data[modelNum][chainNum])<<"\t"; // print element symbol
    fout<<std::get<0>(data[modelNum][chainNum])<<"\t"; // print x
    fout<<std::get<1>(data[modelNum][chainNum])<<"\t"; // print y
    fout<<std::get<2>(data[modelNum][chainNum])<<"\n"; // print z
}

inline void Converter::checkModelNumber(int model){
    if(model>numModels){
	std::string error("\nError: There is no MODEL ");
	error+=std::to_string(model);
	error+=" in ";
	error+=proteinName;
	error+=".\nNote: the first model has number 1 (not 0).\n";
	throw std::runtime_error(error);
    }
}

inline void Converter::checkChainNumber(char chain){
    if(Utile::abc(chain)>numChains){
	std::string error("\nError: There is no chain ");
	error+=chain;
	error+=" in ";
	error+=proteinName;
	error+=".\nNote: the first chain is A, the second is B and so on.\n";
	throw std::runtime_error(error);
    }
}

}// end of namespase PCA


#endif