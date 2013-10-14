#include <iostream>

#include <SpecReader/Specification.hpp>
#include <FileReader/FileReader.hpp>
#include <ReportGenerator/Report.hpp>
#include <Comparator/Comparator.hpp>

int main( int argc, char** argv )
{
	Comparator c;

	// std::stringbuf buffer;
	file_reader::FileReader f( NULL );

	report_generator::Report r;
	spec_reader::Specification s;
	
	c.check( s, f, r);

	return 0;
}

