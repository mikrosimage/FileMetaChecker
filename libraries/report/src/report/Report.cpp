#include "Report.hpp"
#include "Export.hpp"

#include <common/global.hpp>

#include <boost/foreach.hpp>

Report::Report()
{
	
}

void Report::add( const std::string& entry, const std::string& value )
{
	reportTree.put( entry, value );
}

void Report::add( const bpt::ptree& tree, const std::string& rootpath )
{
	reportTree.push_back( bpt::ptree::value_type( rootpath, tree ) );
}

void Report::exportReport( const std::string& filename )
{
	Export e;
	e.writeXml( reportTree, filename );
}
