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

#include <curl/curl.h>
#include <string>

#ifndef PDB2XYZ_FIND_BY_URL
#define PDB2XYZ_FIND_BY_URL


namespace PCA{
class FindByURL{
private:
    static size_t curlWriteCallback(char *data, size_t size, size_t nmemb, void *buffer);
    
public:
    FindByURL(const std::string& url, std::string* output);
    ~FindByURL();

};



}//end of namespace PCA

#endif