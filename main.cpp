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
    string file_out = "";
    string file_in = "";
    string filterAtom = "";
    int isPart = false;
    char chain = 'A';
    int model = 1;
    string outputFormat = "xyz";    
    
    while(true){
	static struct option long_options[] = {
	    // flags
	    {"part",		no_argument,		&isPart,	1},
	    // variables
	    {"output",	required_argument,	0,		'o'},
	    {"filter",	required_argument,	0,		'f'},
	    {"chain",	required_argument,	0,		'C'},
	    {"model",	required_argument,	0,		'M'},
	    {"file-out",	required_argument,	0,		'n'},
	    {"file-in",	required_argument,	0,		'i'},
	    {0,			0,			0,		0}
	};
	
	int option_index = 0;
	int c = getopt_long(argc, argv, "o:f:C:M:n:i:", long_options, &option_index);
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
	    outputFormat = optarg;
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
	case 'n':
	    file_out = optarg;
	break;
	case 'i':
	    file_in = optarg;
	break;
	default:;
	
	}
    
    }
    string proteinName = argv[optind];
    
    // Define output format------
    if(outputFormat.compare("xyz")==0)
	Converter::outputFormat = Converter::Format::xyz;
    
    else if(outputFormat.compare("tbm")==0)
	Converter::outputFormat = Converter::Format::tbm;
    
    else if(outputFormat.compare("pca")==0)
	Converter::outputFormat = Converter::Format::pca;
    
    else{
	cout<<"\nError: unknow output format.\n";
	cout<<"List of knowm formats:\n";
	cout<<"\txyz - coordinates and name of element.\n";
	cout<<"\ttbm - tight binding model format.\n";
	cout<<"\tpca - only coordinates, no header.\n";
	cout<<"\n.";
	exit(1);
    }
    //--------------------------
    
    if(file_out.size()==0)
	file_out = proteinName + "." + outputFormat;
    
    ofstream fout(file_out);
    if(!fout){
	cout<<"Error: Cannot create output file with required name.\n";
	exit(1);
    }
    Converter converter;
    converter.init(proteinName, file_in);

    if(filterAtom.size()>0){
	Converter *filtered;
	filtered = converter.filter(filterAtom);
	if(isPart)
	    filtered->printTheLongestPart(fout,chain,model);
	else
	    filtered->print(fout,chain,model);
    }
    
    else{
	if(isPart)
	    converter.printTheLongestPart(fout,chain,model);
	else
	    converter.print(fout,chain,model);
    }

    fout.close();
}//end of try

catch(std::runtime_error e){
    cout << e.what();
}

return 0;
}