#include <fstream>
#include <vector>
#include <string>

namespace PCA{

class Converter{
private:

public:
    static int everything(std::ifstream& fin, std::ofstream& fout);
    static int CA(std::ifstream& fin, std::ofstream& fout);
};
}// end of namespase PCA