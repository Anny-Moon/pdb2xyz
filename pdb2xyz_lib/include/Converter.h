#include "Utile.h"
#include <fstream>
#include <iostream>
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
    static enum class Format{xyz, tbm, pca} outputFormat;
    
    std::string proteinName;
    /** x, y, z, <element symbol> <atom name> <residue number> <chainID>*/
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
    
    /** If path to file is "" then it will go to the Internet*/
    int init(std::string name_in, std::string pathToFile="");
    int print(std::ofstream& fout, char chain=0, int model=1);
    bool printTheLongestPart(std::ofstream& fout, char chain='A', int model=1);
    
    Converter* filter(std::string atomName="CA");
    
    inline void xyzHeader(int modelNum, int chainNum, std::ofstream& fout, int numberOfAtoms=0, std::string message = "");
    /**@param modelNum - actual number (0,1..) @param chain - 0,1,.. */
    inline void xyzLine(int modelNum, int atom, std::ofstream& fout);
    void xyzBlock(int modelNum, int chainNum, std::ofstream& fout);
    
    void tbmHeader(int modelNum, int chainNum, std::ofstream& fout, int numberOfAtoms=0, std::string message = "");
    inline void tbmLine(int modelNum, int atom, std::ofstream& fout);
    void tbmBlock(int modelNum, int chainNum, std::ofstream& fout);
    
    inline void pcaLine(int modelNum, int atom, std::ofstream& fout);
    void pcaBlock(int modelNum, int chainNum, std::ofstream& fout);
    
    inline void fileHeader(int modelNum, int chainNum, std::ofstream& fout, int numberOfAtoms=0, std::string message = "");
    inline void fileLine(int modelNum, int atom, std::ofstream& fout);
    inline void fileBlock(int modelNum, int chainNum, std::ofstream& fout);
    
    
    /** Throw exeptions*/
    inline void checkModelNumber(int model);
    inline void doesNotSupportMultyModel(int model, std::string who="This regime");
    inline void checkChainNumber(char chain);
    inline void doesNotSupportMultyChain(char chain, std::string who="This regime");
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

inline void Converter::xyzLine(int modelNum, int atom, std::ofstream& fout){
    fout<<std::get<3>(data[modelNum][atom])<<"\t"; // print element symbol
    fout<<std::get<0>(data[modelNum][atom])<<"\t"; // print x
    fout<<std::get<1>(data[modelNum][atom])<<"\t"; // print y
    fout<<std::get<2>(data[modelNum][atom])<<"\n"; // print z
}

inline void Converter::tbmLine(int modelNum, int atom, std::ofstream& fout){
    fout<<"(";
    fout<<std::get<0>(data[modelNum][atom])<<"\t"; // print x
    fout<<std::get<1>(data[modelNum][atom])<<"\t"; // print y
    fout<<std::get<2>(data[modelNum][atom])<<")\t"; // print z
    fout<<"<>\t["<<atom<<"]\n";
}

inline void Converter::pcaLine(int modelNum, int atom, std::ofstream& fout){
    fout<<std::get<0>(data[modelNum][atom])<<"\t"; // print x
    fout<<std::get<1>(data[modelNum][atom])<<"\t"; // print y
    fout<<std::get<2>(data[modelNum][atom])<<"\n"; // print z
}


inline void Converter::fileHeader(int modelNum, int chainNum, std::ofstream& fout, int numberOfAtoms, std::string message){
    switch(outputFormat){
	case Format::xyz:
	    xyzHeader(modelNum, chainNum, fout, numberOfAtoms, message);
	break;
    
	case Format::tbm:
	    tbmHeader(modelNum, chainNum, fout, numberOfAtoms, message);
        break;
	
	case Format::pca:
	break;
	
	default:
	    std::cout<<"Error in fileHeader: unknown output format\n";
	    exit(1);
    }

}

inline void Converter::fileLine(int modelNum, int atom, std::ofstream& fout){
    switch(outputFormat){
	case Format::xyz:
	    xyzLine(modelNum, atom, fout);
	break;
	
	case Format::tbm:
	    tbmLine(modelNum, atom, fout);
	break;
    
	case Format::pca:
	    pcaLine(modelNum, atom, fout);
	break;
    
	default:
	    std::cout<<"Error in fileLine: unknown output format\n";
	    exit(1);
    }
}
/*
inline void Converter::fileBlock(int modelNum, int chainNum, std::ofstream& fout){
    switch(outputFormat){
	case Format::xyz:
	    xyzBlock(modelNum, chainNum, fout);
	break;
    
	case Format::tbm:
	    tbmBlock(modelNum, chainNum, fout);
	break;

	case Format::pca:
	    pcaBlock(modelNum, chainNum, fout);
	break;

	default:
	    std::cout<<"Error in fileLine: unknown output format\n";
	    exit(1);
    }

}
*/
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

inline void Converter::doesNotSupportMultyModel(int model, std::string who){
    if(model==0){
	std::string error("\nError: ");
	error+= who;
	error+=" does not support multy model output.\n ";
	throw std::runtime_error(error);
    }
}

inline void Converter::doesNotSupportMultyChain(char chain, std::string who){
    if(chain=='0'){
	std::string error("\nError: ");
	error+= who;
	error+=" does not support multy model output.\n ";
	throw std::runtime_error(error);
    }
}

}// end of namespase PCA


#endif