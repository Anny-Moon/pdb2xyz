#include <fstream>
#include <vector>
#include <string>
#include <tuple>

namespace PCA{

class Converter{
private:
//    std::ifstream& fin;
    std::vector<std::tuple <double, double, double, std::string>>data;

public:
    Converter(std::ifstream& fin, std::ofstream& fout);
    ~Converter();
    static int everything();
    static int CA(std::ofstream& fout);
};
}// end of namespase PCA