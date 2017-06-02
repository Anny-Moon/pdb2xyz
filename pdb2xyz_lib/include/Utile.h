
namespace PCA{
class Utile{

public:
    static int abc(char letter);
    static char abc(int number);
};


inline int Utile::abc(char letter){
    return (int)(letter -'A'+1);
}

inline char Utile::abc(int number){
    return (char)((int)'A' + number - 1);
}

}//end of namespace PCA