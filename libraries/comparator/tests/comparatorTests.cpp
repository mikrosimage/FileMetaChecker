#define BOOST_TEST_MODULE qc_report

#include <boost/test/unit_test.hpp>

#include <Comparator.hpp>
#include <FileReader.hpp>
#include <Export/Export.hpp>
#include <Transform/Transform.hpp>

using namespace boost::unit_test;
using namespace comparator;

namespace sr = spec_reader;
namespace be = basic_element;
namespace rg = report_generator;
namespace fr = filereader;

boost::shared_ptr<common::formatters::Formatter> formatter( common::formatters::Formatter::get() );
boost::shared_ptr<common::Color>                 color( common::Color::get() );

BOOST_AUTO_TEST_SUITE( comparator_test_suite01 )

BOOST_AUTO_TEST_CASE( report_init )
{
	formatter->init_logging();
	color->enable();
}

BOOST_AUTO_TEST_CASE( comparator_comparator )
{
	LOG_INFO( ">>> comparator_comparator <<<" );
	// {
	// 	sr::SpecList specs;
	// 	sr::Specification spec;
	// 	spec.setFromFile( "comparetest.json" );
	// 	specs.addSpecification( spec );
	// 	BOOST_CHECK_EQUAL( specs.getSpecNumber(), 1 );
		
	// 	std::ifstream is;
	// 	is.open( "test.txt", std::ios::in );
	// 	fr::FileReader file( is.rdbuf() );

	// 	Comparator comp( &file, specs );
		
	// 	rg::Report report;
	// 	comp.compare( "test", report );
	// }
	{
		std::string jsonString = " { ";
		jsonString += " \"standard\": ";
		jsonString += " { ";
		jsonString += "   \"id\": \"test\",";
		jsonString += "   \"extension\": [ ";
		jsonString += "   \"ext1\",";
		jsonString += "   \"ext2\",";
		jsonString += "   \"ext3\"]";
		jsonString += " }, ";
		jsonString += " \"header\": [ ";
		jsonString += "    { ";
		jsonString += "     \"id\": \"test1\",";
		jsonString += "     \"label\": \"Test 1\",";
		jsonString += "     \"type\": \"unknown\"";
		jsonString += "    }";
		jsonString += "  ]";
		jsonString += "}";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::ifstream is;
		is.open( "test.txt", std::ios::in );
		fr::FileReader file( is.rdbuf() );

		Comparator comp( &file, specList );
		
		rg::Report report;
		BOOST_CHECK_THROW( comp.compare( "test", report ), std::runtime_error );
	}
	{
		std::string jsonString = " { ";
		jsonString += " \"standard\": ";
		jsonString += " { ";
		jsonString += "   \"id\": \"test\",";
		jsonString += "   \"extension\": [ ";
		jsonString += "   \"ext1\",";
		jsonString += "   \"ext2\",";
		jsonString += "   \"ext3\"]";
		jsonString += " }, ";
		jsonString += " \"header\": [ ";
		jsonString += "    { ";
		jsonString += "     \"id\": \"test1\",";
		jsonString += "     \"label\": \"Test 1\",";
		jsonString += "     \"type\": \"hexa\"";
		jsonString += "    },";
		jsonString += "    { ";
		jsonString += "     \"id\": \"test2\",";
		jsonString += "     \"label\": \"Test 2\",";
		jsonString += "     \"type\": \"ascii\",";
		jsonString += "     \"group\": [ ";
		jsonString += "         { ";
		jsonString += "          \"id\": \"child1\",";
		jsonString += "          \"label\": \"Child 1\",";
		jsonString += "          \"type\": \"hexa\",";
		jsonString += "          \"values\": \"ff00\"";
		jsonString += "         },";
		jsonString += "         { ";
		jsonString += "          \"id\": \"child2\",";
		jsonString += "          \"label\": \"Child 2\",";
		jsonString += "          \"type\": \"ascii\",";
		jsonString += "          \"group\": [ ";
		jsonString += "              { ";
		jsonString += "               \"id\": \"child21\",";
		jsonString += "               \"label\": \"Child 2-1\",";
		jsonString += "               \"type\": \"raw\"";
		jsonString += "              } ]";
		jsonString += "         }";
		jsonString += "      ]";
		jsonString += "    },";
		jsonString += "    { ";
		jsonString += "     \"id\": \"test3\",";
		jsonString += "     \"label\": \"Test 3\",";
		jsonString += "     \"type\": \"raw\"";
		jsonString += "    }";
		jsonString += "  ]";
		jsonString += " }";

		sr::Specification spec;
		sr::SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );

		std::ifstream is;
		is.open( "test.txt", std::ios::in );
		fr::FileReader file( is.rdbuf() );

		Comparator comp( &file, specList );
		
		rg::Report report;
		comp.compare( "test", report );

		rg::Transform tr( report );
		rg::Export exporter( tr.transformTree( rg::Transform::eReportTypeXml ) );
		exporter.writeXml( "test.xml" );
	}
}

BOOST_AUTO_TEST_SUITE_END()
