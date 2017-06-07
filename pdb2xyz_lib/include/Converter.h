#include <fstream>
#include <vector>
#include <string>
#include <tuple>

#ifndef PDB2XYZ_CONVERTER
#define PDB2XYZ_CONVERTER

namespace PCA{

class Converter{
private:
public:
//    std::ifstream& fin;
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
    
    Converter* filterCA();
    int check();
    
    inline int numberOfModels();
    inline int numberOfAtoms();
    inline int numberOfChains();
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


}// end of namespase PCA


#endif