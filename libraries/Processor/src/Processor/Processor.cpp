#include "Processor.hpp"

#include <fstream>

Processor::Processor()
{

}

Processor::~Processor()
{

}


void Processor::process( const std::string& filePath, const std::string& specPath, report_generator::Report& report )
{
	std::ifstream is;

	is.rdbuf()->open( filePath, std::ios::in );
	file_reader::FileReader file( is.rdbuf() );

	spec_reader::Specification spec;
	spec_reader::SpecChecker specChecker( specPath );

	if( ! specChecker.check() )
		throw std::runtime_error( "Cannot open specification file: " + specPath );

	spec.setFromString( specChecker.getSpecString() );

	comparator::Comparator comp;
	comp.check( spec, file, report );
}