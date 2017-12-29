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

#include <string>

#ifndef PDB2XYZ_KEYWORD
#define PDB2XYZ_KEYWORD

namespace PCA{
class Keyword{
public:
    static std::string header;
    static std::string atom;
    static std::string end;
    static std::string endOfModel;
    

};
}// end of namespace;
#endif