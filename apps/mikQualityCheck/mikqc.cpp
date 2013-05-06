#include <common/global.hpp>

#include <extractor/Extractor.hpp>
#include <systemInfo/FileSystemInfo.hpp>
#include <report/Report.hpp>

#include <iomanip>
#include <string>
#include <fstream>

#include <boost/exception/diagnostic_information.hpp>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>

namespace bpo = boost::program_options;

int main( int argc, char** argv )
{
	boost::shared_ptr<common::formatters::Formatter> formatter( common::formatters::Formatter::get() );
	boost::shared_ptr<common::Color>                 color( common::Color::get() );

	bpo::options_description cmdlineOptions;
	bpo::positional_options_description pod;
	bpo::variables_map vm;

	std::vector<std::string> paths;

	formatter->init_logging();
	
	// Supported options
	cmdlineOptions.add_options()
		( "help,h", "produce help message" )
		( "input,i", bpo::value< std::vector<std::string> >(), "input file" )
		( "color", "display the output with colors" )
		( "verbose,v", bpo::value<int>()->default_value( 2 ), "verbose level (trace, debug, warning, error, fatal)" )
		( "script", "format the output such as it could be dump in a file" )
		( "display-level", bpo::value<bool>()->default_value( false ), "display verbose level befora each message." )
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
			const std::vector<std::string> vecOptions = bpo::split_unix( env_options, " " );
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
	
	if( vm.count( "color" ) )
	{
		color->enable();
	}
	if( vm.count( "script" ) )
	{
		color->disable();
	}
	
	formatter->displayLogLevel( vm[ "display-level" ].as< bool >() );
	
	switch( vm[ "verbose" ].as< int >() )
	{
		case 0 :  formatter->setLogLevel( boost::log::trivial::trace   ); break;
		case 1 :  formatter->setLogLevel( boost::log::trivial::debug   ); break;
		case 2 :  formatter->setLogLevel( boost::log::trivial::info    ); break;
		case 3 :  formatter->setLogLevel( boost::log::trivial::warning ); break;
		case 4 :  formatter->setLogLevel( boost::log::trivial::error   ); break;
		case 5 :  formatter->setLogLevel( boost::log::trivial::fatal   ); break;
		default : formatter->setLogLevel( boost::log::trivial::warning ); break;
	}
	
	if( vm.count( "input" ) )
	{
		paths = vm[ "input" ].as< std::vector<std::string> >();
	}

	if( vm.count( "help" ) || paths.size() == 0 )
	{
		LOG_INFO( color->_blue  << "Quality Check project" << color->_std );
		LOG_INFO( "" );
		LOG_INFO( color->_blue  << "NAME" << color->_std );
		LOG_INFO( color->_green << "\tmikqc - quality check report on file(s)" << color->_std );
		LOG_INFO( "" );
		LOG_INFO( color->_blue  << "SYNOPSIS" << color->_std );
		LOG_INFO( color->_green << "\tmikqc [options] -i [files]" << color->_std );
		LOG_INFO( "" );
		LOG_INFO( color->_blue  << "DESCRIPTION" << color->_std );
		LOG_INFO( "" );

		LOG_INFO( "Analyse file(s) based on:" );
		LOG_INFO( "    - system informations" );
		LOG_INFO( "    - file header" );
		LOG_INFO( "    - file metadatas" );
		LOG_INFO( "    - file datas" );
		LOG_INFO( "" );
		LOG_INFO( color->_blue  << "OPTIONS" << color->_std );
		LOG_INFO( "" );
		LOG_INFO( cmdlineOptions );
		return 0;
	}

	BOOST_FOREACH( std::string& path, paths )
	{
		try
		{
			Report report;

			FileSystemInfo fileSystemInfo( path );
			LOG_INFO( fileSystemInfo );
			fileSystemInfo.getReport( &report );

			Extractor extractor;
			extractor.init();

			extractor.openFilename( path );
			
			extractor.analyse();

			extractor.closeFilename();

			extractor.getReport( &report );

			report.exportReport( path + ".xml" );
		}
		catch( ... )
		{
			LOG_FATAL( color->_error  << "error" << color->_std );
#ifndef QC_PRODUCTION
			LOG_FATAL( color->_red << boost::current_exception_diagnostic_information() << color->_std );
#endif
			exit( -2 );
		}
	}

	return 0;
}


