#include "pdb2xyz_lib/include/Converter.h"
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <getopt.h>

using namespace std;
using namespace PCA;

int main(int argc, char* argv[]){

try{
    string file_out;
    string filterAtom = "";
    int isPart = false;
    char chain = 'A';
    int model = 1;
    
    
    while(true){
	static struct option long_options[] = {
	    // flags
	    {"part",		no_argument,		&isPart,	1},
	    // variables
	    {"output",	required_argument,	0,		'o'},
	    {"filter",	required_argument,	0,		'f'},
	    {"chain",	required_argument,	0,		'C'},
	    {"model",	required_argument,	0,		'M'},
	    {0,			0,			0,		0}
	};
	
	int option_index = 0;
	int c = getopt_long(argc, argv, "o:f:C:M:", long_options, &option_index);
	if(c == -1)
	    break;

	switch(c){
	case 0:
	    //If the option sets a flag, do nothing.
	    if(long_options[option_index].flag != 0)
		break;
	    cout << "option " << long_options[option_index].name;
	    if(optarg)
		cout << " with argument " << optarg;
	    cout << "\n";
	break;
	case 'o':
	    file_out = optarg;
	break;
	case 'f':
	    filterAtom = optarg;
	break;
	case 'C':
	    chain = *optarg;
	break;
	case 'M':
	    model = atoi(optarg);
	break;
	default:;
	
	}
    
    }

//    if(argv[1]==NULL){
//	cout<<"I need name of the polymer\n";
//	exit(1);
//    }
    string proteinName = argv[optind];
    
    if(file_out.size()==0)
	file_out = proteinName + ".xyz";
    
    ofstream fout(file_out);
    Converter converter;
    converter.init(proteinName);

    if(filterAtom.size()>0){
	Converter *filtered;
	filtered = converter.filter(filterAtom);
	if(isPart)
	    filtered->printTheLongestPart(fout,chain,model);
	else
	    filtered->print(fout,chain,model);
    }
    
    else{
	converter.print(fout,chain,model);
	if(isPart)
	    converter.printTheLongestPart(fout,chain,model);
	else
	    converter.print(fout,chain,model);
    }
//    converter.print(fout,'A',0);
//    converter.printTheLongestPart(fout,'A',0);
//    converter.check();

    fout.close();
/*    
    Converter *ca;
    ofstream fout2("testCA.xyz");
    ofstream fout3("testCApart.xyz");
    ca = converter.filterCA();
    ca->print(fout2,0,0);
//    ca->printTheLongestPart(fout3,'A',0);
ca->printTheLongestPart(fout3);
    fout2.close();
    fout3.close();
*/

}//end of try

catch(std::runtime_error e){
    cout << e.what();
}

return 0;
}