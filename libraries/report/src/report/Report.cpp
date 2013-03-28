#include "Report.hpp"
#include "Export.hpp"

Report::Report()
{
	
}

void Report::add( const std::string& entry, const std::string& value )
{
	reportTree.put( entry, value );
}

void Report::exportReport( const std::string& filename )
{
	Export e;
	e.writeXml( reportTree, filename );
}
