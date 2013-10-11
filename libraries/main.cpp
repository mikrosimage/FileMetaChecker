#include <iostream>

#include "spec.hpp"
#include "report.hpp"
#include "fileReader/src/FileReader/FileReader.hpp"
#include "comparator.hpp"

int main( int argc, char** argv )
{
	Comparator c;

	// std::stringbuf buffer;
	// filereader::FileReader f( NULL );

	Report r;
	Spec s;
	
	c.check( s, /*f,*/ r);
}

