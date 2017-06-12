#include "FindByURL.h"
#include <iostream>

using namespace std;
using namespace PCA;

size_t FindByURL::curlWriteCallback(char *data, size_t size, size_t nmemb, void *buffer){
    
    for(size_t i = 0; i < size*nmemb; i++)
	*((string*)buffer)+=data[i];

    return size*nmemb;
}


FindByURL::FindByURL(const string& url, string* output){

    int err;
    CURL *easy_handle = curl_easy_init();
    curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, output);
    curl_easy_setopt(easy_handle, CURLOPT_FAILONERROR, true);
    err=curl_easy_perform(easy_handle);
/*    
	if(err!=0){
	    string error = "Error: Page ";
	    error+=url;
	    error+=" is not found.\nProbably the protein with the required name does not exist.\n";
	    throw runtime_error(error);
	}
*/
    curl_easy_cleanup(easy_handle);

}

FindByURL::~FindByURL(){};