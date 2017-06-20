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
    
    inline int numberOfModels();
    inline int numberOfAtoms();
    inline int numberOfChains();
    
    /** Trow exeptions*/
    inline void checkModelNumber(int model);
    inline void checkChainNumber(char chain);
};

inline int Converter::numberOfModels(){
    return numModels;
}

inline int Converter::numberOfAtoms(){
    return numAtoms;
}

inline int Converter::numberOfChains(){
    return numChains;
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