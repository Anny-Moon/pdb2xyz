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