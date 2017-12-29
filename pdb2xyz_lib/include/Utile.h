/* Copyright 2017 Anna Sinelnikova
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
*/

#ifndef PDB2XYZ_FIND_UTILE
#define PDB2XYZ_FIND_UTILE

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

#endif