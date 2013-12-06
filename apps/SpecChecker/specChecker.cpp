#include <Common/log.hpp>

#include <SpecReader/Specification.hpp>
#include <SpecReader/SpecChecker.hpp>

#include <iomanip>
#include <string>
#include <fstream>

// #include <boost/exception/diagnostic_information.hpp>
#include <boost/program_options.hpp>

namespace bpo = boost::program_options;

void checkNodes( const std::shared_ptr< spec_reader::SpecNode > specNode, size_t level = 0 )
{
	std::shared_ptr< spec_reader::SpecNode > sn = specNode;
	
	bool end = false;
	while( ! end )
	{
		LOG_INFO( common::details::kColorGreen << std::setw( 3 * level + 2 ) << "* " << sn->getId() << common::details::kColorStd );
		if( sn->isGroup() )
		{
			checkNodes( sn->firstChild(), level + 1 );
		}
		sn = sn->next();
		if( sn == nullptr )
			end = true;
	}
}

int main( int argc, char** argv )
{
	bpo::options_description            cmdlineOptions;
	bpo::positional_options_description pod;
	bpo::variables_map                  vm;

	std::string specPath;

	common::level = common::eLogInfo;

	
	// Supported options
	cmdlineOptions.add_options()
		( "help,h",        "produce help message"                    )
		( "spec,s",        bpo::value< std::string >(), "specification file" )
		( "verbose,v",     bpo::value<int>()->default_value( 1 ),      "verbose level (trace, debug, warning, error, fatal)" )
	;

	// Default option
	pod.add( "input", -1 );
	// Parsing options
	try
	{
		bpo::store( bpo::command_line_parser( argc, argv ).options( cmdlineOptions ).positional( pod ).run(), vm);

		// get environment options and parse them
		if( const char* env_options = std::getenv("QC_OPTIONS") )
		{
			const std::vector< std::string > vecOptions = bpo::split_unix( env_options, " " );
			bpo::store(bpo::command_line_parser( vecOptions ).options( cmdlineOptions ).positional( pod ).run(), vm);
		}
		bpo::notify( vm );
	}
	catch( const bpo::error& e)
	{
		LOG_ERROR( "command line error: " << e.what() );
		exit( -2 );
	}
	catch(...)
	{
		LOG_ERROR( "unknown error in command line." );
		exit( -2 );
	}

	switch( vm[ "verbose" ].as< int >() )
	{
		case 0 :  common::level = common::eLogTrace;   break;
		case 1 :  common::level = common::eLogInfo;    break;
		case 2 :  common::level = common::eLogWarning; break;
		case 3 :  common::level = common::eLogError;   break;
		case 4 :  common::level = common::eLogFatal;   break;
		default : common::level = common::eLogWarning;    break;
	}


	if( vm.count( "spec" ) )
	{
		specPath = vm[ "spec" ].as< std::string >();
	}

	if( vm.count( "help" ) || specPath.empty() )
	{
		common::level = common::eLogInfo;
		LOG_INFO( common::details::kColorBlue  << "Spec Checker project"                  << common::details::kColorStd << std::endl );
		LOG_INFO( common::details::kColorBlue  << "NAME"                                   << common::details::kColorStd );
		LOG_INFO( common::details::kColorGreen << "\tSpecChecker"                      << common::details::kColorStd << std::endl );
		LOG_INFO( common::details::kColorBlue  << "SYNOPSIS"                               << common::details::kColorStd );
		LOG_INFO( common::details::kColorGreen << "\tspecChecker [options] -s [file]" << common::details::kColorStd << std::endl );
		LOG_INFO( common::details::kColorBlue  << "DESCRIPTION"                            << common::details::kColorStd << std::endl );

		LOG_INFO( "Spec Validation schema." << std::endl );
		LOG_INFO( common::details::kColorBlue  << "OPTIONS" << common::details::kColorStd << std::endl );
		LOG_INFO( cmdlineOptions );
		return 0;
	}

	try
	{
		spec_reader::SpecChecker specChecker( specPath );

		if( ! specChecker.check() )
			throw std::runtime_error( "Cannot open specification file: " + specPath );

		spec_reader::Specification spec;
		spec.setFromString( specChecker.getSpecString() );

		LOG_INFO( common::details::kColorCyan << "Specification: " << spec.getId() << " (" << spec.getType() << ")" << common::details::kColorStd << std::endl );
		checkNodes( spec.getFirstNode() );
	}
	catch( std::runtime_error e )
	{
		LOG_FATAL( "Runtime error: " << e.what() );
		exit( -2 );
	}
	catch( std::exception e )
	{
		LOG_FATAL( "Error: " << e.what() );
		exit( -2 );
	}

	return 0;
}
