#include <fstream>
#include <vector>
#include <string>
#include <tuple>

namespace PCA{

class Converter{
private:
//    std::ifstream& fin;

    /** x, y, z, <element symbol> <atom name> <residue nuber> <chainID>*/
    std::vector<std::tuple <double, double, double, std::string, std::string, int, char>>data;

public:
    Converter(std::ifstream& fin, std::ofstream& fout);
    ~Converter();
    static int everything();
    static int CA(std::ofstream& fout);
};
}// end of namespase PCA