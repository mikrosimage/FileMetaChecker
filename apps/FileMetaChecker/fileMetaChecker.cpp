#include <Common/log.hpp>

#include <SpecReader/Specification.hpp>
#include <FileReader/FileReader.hpp>
#include <ReportGenerator/Report.hpp>
#include <Comparator/Comparator.hpp>

#include <iomanip>
#include <string>
#include <fstream>

// #include <boost/exception/diagnostic_information.hpp>
#include <boost/program_options.hpp>

namespace bpo = boost::program_options;

int main( int argc, char** argv )
{
	bpo::options_description            cmdlineOptions;
	bpo::positional_options_description pod;
	bpo::variables_map                  vm;

	std::string filePath;
	std::string specPath;

	common::level = common::eLogInfo;

	
	// Supported options
	cmdlineOptions.add_options()
		( "help,h",        "produce help message"                    )
		( "input,i",       bpo::value< std::string >(), "input file" )
		( "spec,s",        bpo::value< std::string >(), "specification file" )
		// ( "color",         "display the output with colors"                                                                  )
		( "verbose,v",     bpo::value<int>()->default_value( 1 ),      "verbose level (trace, debug, warning, error, fatal)" )
		// ( "script",        "format the output such as it could be dump in a file"                                            )
		// ( "display-level", bpo::value<bool>()->default_value( false ), "display verbose level before each message."          )
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

	if( vm.count( "input" ) )
	{
		filePath = vm[ "input" ].as< std::string >();
	}

	if( vm.count( "spec" ) )
	{
		specPath = vm[ "spec" ].as< std::string >();
	}

	if( vm.count( "help" ) || filePath.empty() || specPath.empty() )
	{
		common::level = common::eLogInfo;
		LOG_INFO( common::details::kColorBlue  << "Quality Check project"      << common::details::kColorStd << std::endl );
		LOG_INFO( common::details::kColorBlue  << "NAME"                       << common::details::kColorStd );
		LOG_INFO( common::details::kColorGreen << "\tFileMetaChecker"          << common::details::kColorStd << std::endl );
		LOG_INFO( common::details::kColorBlue  << "SYNOPSIS"                   << common::details::kColorStd );
		LOG_INFO( common::details::kColorGreen << "\tfmc [options] -i [files]" << common::details::kColorStd << std::endl );
		LOG_INFO( common::details::kColorBlue  << "DESCRIPTION"                << common::details::kColorStd << std::endl );

		LOG_INFO( "File quality analyze based on:" );
		LOG_INFO( "    - system informations"      );
		LOG_INFO( "    - file header"              );
		LOG_INFO( "    - file metadatas"           );
		LOG_INFO( "    - file datas"  << std::endl );
		LOG_INFO( common::details::kColorBlue  << "OPTIONS" << common::details::kColorStd << std::endl );
		LOG_INFO( cmdlineOptions );
		return 0;
	}

	try
	{
		LOG_INFO( common::details::kColorCyan  << std::setfill( '=' ) << std::setw( filePath.size() + 14 ) << common::details::kColorStd );
		LOG_INFO( common::details::kColorCyan  << "| Check: " << filePath << common::details::kColorStd );

		report_generator::Report report;

		// FileSystemInfo fileSystemInfo( path );
		// LOG_INFO( fileSystemInfo );
		// fileSystemInfo.getReport( &report );
		
		std::ifstream is;
		std::filebuf * fb = is.rdbuf();

		fb->open( filePath, std::ios::in );
		file_reader::FileReader file( fb );
		LOG_INFO( common::details::kColorCyan  << "| File length: " << file.getPosition() << "/" << file.getLength() << common::details::kColorStd );

		spec_reader::Specification spec;
		spec.setFromFile( specPath );
		
		LOG_INFO( common::details::kColorCyan  << "| Specification: " << spec.getId() << " (" << spec.getType() << ")" << common::details::kColorStd  );
		LOG_INFO( common::details::kColorCyan  << std::setfill( '=' ) << std::setw( filePath.size() + 14 ) << common::details::kColorStd );
		
		comparator::Comparator comp;
		comp.check( spec, file, report );

		report.print();
		report.writeXml();
		fb->close();
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


