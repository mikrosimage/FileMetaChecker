#include "Extractor.hpp"

#include <report/Report.hpp>

#include "specs/Specification.hpp"
#include "inputFile/File.hpp"

Extractor::Extractor()
	: _specs( )
{
	_file = new File();
	_specs = new Specification();
}

Extractor::~Extractor()
{
	delete _file;
	delete _specs;
}

void Extractor::init()
{
	_specs->init();
	_specs->updateList();
}

bool Extractor::openFilename( const std::string& filename )
{
	return _file->open( filename );
}

void Extractor::closeFilename( )
{
	_file->close();
}

void Extractor::getReport( Report* report )
{
	report->add( "header", "TODO" );
}
