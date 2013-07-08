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

#include <ctime>

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
	// FOR EACH STREAM @todo
	int index = 1;

	Specs specs;
	_specs->getSpecList( specs );
	bpt::ptree metadataTree;
	std::vector< std::string > streamType;

	bpt::ptree statStreamReport, dateNode;
	dateNode.put( "<xmlattr>.label", "Date" );
	dateNode.put_value( getDate() );
	_statusReport.push_back( bpt::ptree::value_type( "date", dateNode ) );

	BOOST_FOREACH( Specification spec, specs )
	{
		_file->resetFile();
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
			bpt::ptree specReport, statSpecReport;

			specReport.put( "<xmlattr>.id",    spec.getId()    );
			specReport.put( "<xmlattr>.label", spec.getLabel() );
			specReport.put( "<xmlattr>.type",  spec.getType()  );
			statSpecReport = specReport;
			streamType.push_back( spec.getType() );

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
			
			if( isValidFile && _file->endOfFile() )
			{
				specReport.put( "<xmlattr>.status", "valid" );
				statSpecReport.put( "<xmlattr>.status", "valid" );
				LOG_INFO( common::Color::get()->_green << "**********" << common::Color::get()->_std );
				LOG_INFO( common::Color::get()->_green << "VALID " << spec.getLabel() << common::Color::get()->_std );
				LOG_INFO( common::Color::get()->_green << "**********" << common::Color::get()->_std );
			}
			else
			{
				if( !_file->endOfFile() )
					LOG_ERROR( "File structure : the analyser did not reach the end of the file." );
				specReport.put( "<xmlattr>.status", "not valid" );
				statSpecReport.put( "<xmlattr>.status", "not valid" );
				LOG_ERROR( "**********" );
				LOG_ERROR( "NOT VALID " << spec.getLabel() << " : " << _file->getFilename() );
				LOG_ERROR( "**********" );
			}
			statStreamReport.push_back( bpt::ptree::value_type( kSpec, statSpecReport ) );
			
			specReport.put( "<xmlattr>.date", getDate()  );
			metadataTree.push_back( bpt::ptree::value_type( kSpec, specReport ) );
		}
	}
	_streamReport.push_back( bpt::ptree::value_type( kMetadata, metadataTree ) );

	if( !streamType.empty() )
	{
		bool uniqueType = true;
		std::string refType = streamType.at(0);
		BOOST_FOREACH( std::string type, streamType )
		{
			if( type != refType )
				uniqueType = false;
		}
		if ( uniqueType )
		{
			_streamReport.put( "<xmlattr>.type", refType  );
			statStreamReport.put( "<xmlattr>.type", refType );
		}
	}

	std::stringstream streamLabel;
	streamLabel << "Stream " << index;
	_streamReport.put( "<xmlattr>.index", index );
	_streamReport.put( "<xmlattr>.label", streamLabel.str() );
	
	statStreamReport.put( "<xmlattr>.index", index );
	statStreamReport.put( "<xmlattr>.label", streamLabel.str() );

	_statusReport.put( "<xmlattr>.label", "Main Status" );
	_statusReport.push_back( bpt::ptree::value_type( kStream, statStreamReport ) );
}

void Extractor::getReport( Report* report )
{
	report->add( _statusReport, kStatus );
	report->add( _streamReport, kStream );
}

std::string Extractor::getDate()
{
	time_t now = time(0);
	char buffer [80];
	tm *ltm = localtime(&now);
	strftime( buffer, 80, "%A, %d-%m-%Y %H:%M:%S", ltm );
	std::string date = buffer;
	return date;
}