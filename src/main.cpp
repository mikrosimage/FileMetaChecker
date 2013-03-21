#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include <boost/program_options.hpp>

#include "HeaderAnalyser.hpp"

namespace bpo = boost::program_options;

int main( int argc, char** argv )
{
	
/*** OPTIONS ***/	
// Declare the supported options :
	bpo::options_description mainOptions("Allowed options");
	mainOptions.add_options()
	    ("help,h", "produce help message")
	    ("input,i", bpo::value< std::vector<std::string> >(), "input file")
	;

// Store options values :
	bpo::variables_map vm;
	bpo::store( bpo::parse_command_line(argc, argv, mainOptions), vm );
	bpo::notify(vm);    

// get the file paths :
	std::vector<std::string> paths;

	// -i [--input] option :
	if (vm.count("input")) 						
	{
		paths = vm["input"].as< std::vector<std::string> >();
	}
	// -h [--help] option :
	if( vm.count("help") || paths.size() == 0 )
	{
	    std::cout << mainOptions << "\n";
	    return 1;
	}
	// std::cout << "paths : " << paths.size() << std::endl;

// system and header info :
	
	for( size_t index=0; index < paths.size(); index++ )
	{		
		HeaderAnalyser ha( paths.at(index) );
		// fha.printResults();
		// std::cout << "path : " << paths.at(index) << std::endl;
	}

	return 0;
}


