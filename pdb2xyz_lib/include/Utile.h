
namespace PCA{
class Utile{

public:
    static int abc(char letter);
    static char abc(int number);
    static bool isLetter(int number);
};


inline int Utile::abc(char letter){
    return (int)(letter -'A'+1);
}

inline char Utile::abc(int number){
    return (char)((int)'A' + number - 1);
}

inline bool Utile::isLetter(int number){
    if(number-(int)'A'<0)
	return false;
    
    return true;
}

}//end of namespace PCA