#include "Extractor.hpp"

#include <common/global.hpp>

#include <report/Report.hpp>

#include "specs/Specifications.hpp"
#include "specs/NodeSpecification.hpp"
#include "inputFile/File.hpp"

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/none.hpp>
#include <boost/algorithm/string.hpp>

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
	boost::algorithm::to_lower( _ext );
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
		_file->goToBegin();
		std::vector< std::string > exts = spec.getsupportedExtensions();
		bool extIsSupported = false;

		LOG_INFO( common::Color::get()->_blue << "Analyse file as a : " << spec.getLabel() << common::Color::get()->_std );

		BOOST_FOREACH( std::string ext, exts )
		{
			if( ext == _ext )
			{
				extIsSupported = true;
			}
		}

		if( extIsSupported )
		{
			LOG_INFO( "-> Parse Header" );
			SpecIt header = spec.getHeader( );
			bpt::ptree specReport;

			specReport.put( "<xmlattr>.id",    spec.getId()    );
			specReport.put( "<xmlattr>.label", spec.getLabel() );
			specReport.put( "<xmlattr>.type",  spec.getType()  );
			
			NodeSpecification ns( _file );
			bool isValidFile = true;
			
			GroupProperties groupProperties;

			BOOST_FOREACH( SubSpec v, header->second )
			{
				bpt::ptree nodeReport;
				if( ! ns.isValid( v, groupProperties, nodeReport ) )
				{
					LOG_INFO( v.second.get< std::string >( "id" ) );
					isValidFile = false;
				}
				specReport.push_back( bpt::ptree::value_type( v.second.get< std::string >( "id" ), nodeReport ) );
			}
			
			if( isValidFile )
			{
				specReport.put( "<xmlattr>.status", "valid" );
				LOG_INFO( common::Color::get()->_green << "**********" << common::Color::get()->_std );
				LOG_INFO( common::Color::get()->_green << "VALID " << spec.getLabel() << common::Color::get()->_std );
				LOG_INFO( common::Color::get()->_green << "**********" << common::Color::get()->_std );
			}
			else
			{
				specReport.put( "<xmlattr>.status", "not valid" );
				LOG_ERROR( "**********" );
				LOG_ERROR( "NOT VALID " << spec.getLabel() );
				LOG_ERROR( "**********" );
			}
			_report.push_back( specReport );
		}
	}
}

void Extractor::getReport( Report* report )
{
	report->add( _report, kSpec );
}

