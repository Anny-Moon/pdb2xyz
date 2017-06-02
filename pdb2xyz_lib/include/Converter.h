#include <fstream>
#include <vector>
#include <string>
#include <tuple>

namespace PCA{

class Converter{
private:
//    std::ifstream& fin;
    std::string proteinName;
    /** x, y, z, <element symbol> <atom name> <residue nuber> <chainID>*/
    std::vector<std::vector<std::tuple <double, double, double, std::string, std::string, int, char>>>data;
    int numModels;
    int numAtoms;
    int numChains;
    std::vector<int> numAtomsInChain;
    std::vector<int> chainDelimiter;
    
public:
    Converter(std::string name_in);
    ~Converter();
    
    int allAtoms(std::ofstream& fout, int chain=0, int model=1);
    int CA(std::ofstream& fout);
    int check();
    
    inline int numberOfModels();
    inline int numberOfAroms();
    inline int numberOfChains();
};

inline int Converter::numberOfModels(){
    return numModels;
}

inline int Converter::numberOfAroms(){
    return numAtoms;
}

inline int Converter::numberOfChains(){
    return numChains;
}


}// end of namespase PCA