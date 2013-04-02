#include "Extractor.hpp"

#include <common/global.hpp>
#include <report/Report.hpp>

#include "specs/Specifications.hpp"
#include "specs/NodeSpecification.hpp"
#include "inputFile/File.hpp"

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/none.hpp>

namespace bfs = boost::filesystem;

Extractor::Extractor()
{
	_file = new File();
	_specs = new Specifications();
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
	bfs::path path( filename );
	_ext = path.extension().string();
	return _file->open( filename );
}

void Extractor::closeFilename( )
{
	_file->close();
}

void Extractor::analyse( )
{
	Specs specs;
	_specs->getSpecList( specs );

	BOOST_FOREACH( Specification spec, specs )
	{
		std::vector< std::string > exts = spec.getsupportedExtensions();
		bool extIsSupported = false;

		COMMON_COUT( common::details::kColorBlue << "Analyse file as a : " << spec.getLabel() << common::details::kColorStd );

		BOOST_FOREACH( std::string ext, exts )
		{
			if( ext == _ext )
			{
				extIsSupported = true;
			}
		}

		if( extIsSupported )
		{
			COMMON_COUT( "-> Parse Header" );
			SpecIt header = spec.getHeader( );

			BOOST_FOREACH( SubSpec v, header->second )
			{
				NodeSpecification ns( _file, v );

				ns.isValid( );
			}
		}
	}
}

void Extractor::getReport( Report* report )
{
	report->add( "header", "TODO" );
}
