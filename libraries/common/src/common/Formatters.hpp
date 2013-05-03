#ifndef _COMMON_FORMATTERS_HPP_
#define _COMMON_FORMATTERS_HPP_

#include "color.hpp"

#include <fstream>

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>


#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/empty_deleter.hpp>
#include <boost/log/expressions/formatters/stream.hpp>
#include <boost/log/expressions/attr.hpp>
#include <boost/log/expressions/message.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

namespace common {

namespace formatters {

class Formatter : boost::noncopyable
{
private:
	Formatter( ) { }
	
public:
	
	static boost::shared_ptr<Formatter> get();
	~Formatter(){ formatter = NULL; }
	
	void init_logging()
	{
		boost::shared_ptr< sinks::text_ostream_backend > backend = boost::make_shared< sinks::text_ostream_backend >();
		backend->add_stream( boost::shared_ptr< std::ostream >( &std::clog, logging::empty_deleter() ));
		//backend->add_stream( boost::shared_ptr< std::ostream >( new std::ofstream("sample.log") ) );
	
		backend->auto_flush(true);
		
		boost::shared_ptr< logging::core > core = logging::core::get();
		typedef sinks::synchronous_sink< sinks::text_ostream_backend > sink_t;	
		boost::shared_ptr< sink_t > sink( new sink_t( backend ) );
		
		sink->set_formatter (
			expr::stream
			//<< Color::get()->_yellow << "["<< logging::trivial::severity << "]" << Color::get()->_std << " "
			<< "["<< logging::trivial::severity << "]" << " "
			<< expr::smessage
		);
		
		core->add_sink(sink);
		/*
		logging::core::get()->set_filter
		(
			logging::trivial::severity >= logging::trivial::info
		);*/
	}
	
	void setColor( bool colorEnable )
	{
		boost::shared_ptr< logging::core > core = logging::core::get();
		
		core->remove_all_sinks();
		init_logging();
	}
	
public:
	static Formatter* formatter;
};

}
}
#endif
