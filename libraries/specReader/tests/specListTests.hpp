
BOOST_AUTO_TEST_SUITE( spec_reader_test_specList )

BOOST_AUTO_TEST_CASE( spec_reader_specList )
{
	LOG_INFO( "\n>>> spec_reader_specList <<<" );
	{
		SpecList specList;
		specList.initDirectoryPaths();
		specList.addSpecFromDirectories();
	}
	{
		std::string jsonString = " { \"standard\": { \"id\": \"test\", \"label\": \"Test\", \"type\": \"tester\" } } ";
		Specification spec;
		SpecList specList;

		spec.setFromString( jsonString );
		specList.addSpecification( spec );
		BOOST_CHECK_EQUAL( specList.getSpecNumber(), 1 );

		specList.clearSpecifications();
		BOOST_CHECK_EQUAL( specList.getSpecNumber(), 0 );
	}

	{
		std::string jsonString = " { \"standard\": { \"id\": \"test\", \"label\": \"Test\", \"type\": \"tester\" } } ";
		Specification spec1, spec2;
		SpecList specList;

		spec1.setFromString( jsonString );
		specList.addSpecification( spec1 );
		BOOST_CHECK_EQUAL( specList.getSpecNumber(), 1 );

		specList.getSpec( spec2, 0 );
		BOOST_CHECK_EQUAL( specList.getSpecNumber(), 1 );
		BOOST_CHECK_EQUAL( spec1.getId(),    spec2.getId()    );
		BOOST_CHECK_EQUAL( spec1.getLabel(), spec2.getLabel() );
		BOOST_CHECK_EQUAL( spec1.getType(),  spec2.getType()  );

		specList.clearSpecifications();
		BOOST_CHECK_EQUAL( specList.getSpecNumber(), 0 );
	}

	{
		SpecList specList;
		specList.addDirectoryPath( "./fileSpecification" );
		specList.addSpecFromDirectories();
		specList.clearSpecifications();
		BOOST_CHECK_EQUAL( specList.getSpecNumber(), 0 );
	}
	{
		SpecList specList;
		std::vector< Specification > specs;
		specList.addDirectoryPath( "./fileSpecification" );
		specList.addSpecFromDirectories();
		specList.getSpecList( specs );
		BOOST_CHECK_EQUAL( specList.getSpecNumber(), specs.size() );
	}

	{
		SpecList specList;
		specList.addDirectoryPath( "./NoDirectory" );
		specList.addSpecFromDirectories();
		BOOST_CHECK_EQUAL( specList.getSpecNumber(), 0 );
		
		specList.clearDirectories();
		specList.addDirectoryPath( "./test.json" );
		specList.addSpecFromDirectories();
		BOOST_CHECK_EQUAL( specList.getSpecNumber(), 0 );
	}
}

BOOST_AUTO_TEST_SUITE_END()
