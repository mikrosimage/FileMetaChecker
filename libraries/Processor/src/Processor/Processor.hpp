#ifndef _PROCESSOR_PROCESSOR_HPP_
#define _PROCESSOR_PROCESSOR_HPP_

#include <SpecReader/SpecChecker.hpp>
#include <SpecReader/Specification.hpp>
#include <FileReader/FileReader.hpp>
#include <Report/Report.hpp>
#include <Comparator/Comparator.hpp>

class Processor
{
public:
	Processor();
	~Processor();

	void process( const std::string& filePath, const std::string& specPath, report_generator::Report& report );
private:

};

#endif
