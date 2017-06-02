#include <fstream>
#include <vector>
#include <string>
#include <tuple>

namespace PCA{

class Converter{
private:
//    std::ifstream& fin;

    /** x, y, z, <element symbol> <atom name> <residue nuber> <chainID>*/
    std::vector<std::vector<std::tuple <double, double, double, std::string, std::string, int, char>>>data;

public:
    Converter(std::ifstream& fin);
    ~Converter();
    int allAtoms(std::ofstream& fout);
    int CA(std::ofstream& fout);
    int check();
};
}// end of namespase PCA