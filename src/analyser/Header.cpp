#include "common/global.hpp"
#include "TreeSpec.hpp"
#include "Header.hpp"

#include "utils/all.hpp"

#include <sstream>
#include <cstdlib>
#include <iomanip>

#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "specificationFile/FileSpecificationLoader.hpp"

std::ostream& operator<<( std::ostream& out, HeaderSegmentStatus status )
{
	switch( status )
	{
		case eStatusNotValid: out << "not valid"; break;
		case eStatusValid:    out << "valid"; break;
		case eStatusPass:     out << "passed"; break;
	}
	return out;
}

Header::Header( )
{
	FileSpecificationLoader specsLoader;
	specsLoader.load( );
	
	_specs = specsLoader.getSpecifications();
}

bool Header::analyseFileHeader( const std::string& filePath )
{
	_fileReader.setFilePath( filePath );
	
	if( _fileReader.getFileStatus() != "File" )
	{
		COMMON_COUT_DEBUG( "skip file : " << filePath );
		return false;
	}
	
	_fileReader.open();

	for( size_t index = 0; index < _specs.size(); ++index )
	{
		if( foundExtensionInSpec( _specs.at( index ), _fileReader.getExt() ) )
		{
			_specTree = _specs.at( index );
		}
	}
	
	specName = _specTree.get<std::string>( "fileType.name" );
	
	reportStatus = checkInformations( _specTree );
	return reportStatus;
}

bool Header::foundExtensionInSpec( bpt::ptree &pt, const std::string& extension )
{
	bool validExtension = false;
	std::string ext = extension;
	
	// format extension
	std::transform( ext.begin(), ext.end(), ext.begin(), ::tolower );
	ext.erase( 0, 1 );

	// read extension list :
	BOOST_FOREACH( Node &node, pt.get_child( kFiletype + "." + kExtension ) )
	{
		if ( ext == node.second.data())
		{
			validExtension = true;
			// COMMON_COUT( "Valid extension : " << ext );
		}
	}
	return validExtension;
}


bool Header::checkInformations( bpt::ptree& pt )
{
	bool isValid = false;
	_hexaHeader = "";

	BOOST_FOREACH( Node &n, pt.get_child( kHeader ) )
	{
		unsigned int size = n.second.get<unsigned int>( kLength );
		char buffer[size];

		// read header information
		if( _fileReader.isOpen() )
		{	
			_fileReader.read( buffer, size );
		}

		std::stringstream ssh;
		for( size_t i = 0; i < size; ++i )
		{
			ssh << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buffer[i];
		}
		_hexaHeader = _hexaHeader + ssh.str();

		// Header test
		HeaderSegmentStatus nodeStatus = testNode( n , buffer, size);

		COMMON_COUT( n.second.get<std::string>("id") << " : " << nodeStatus );

		isValid = ( nodeStatus == eStatusNotValid ) ? false : true;
	}

	return isValid;
}

HeaderSegmentStatus Header::analyseChunkNode( Node &n )
{
	HeaderSegmentStatus chunkNodeValid = eStatusNotValid;
	COMMON_COUT( "Chunk : " << n.second.get<std::string>("shortId") );
	
	// header
	unsigned int dataSize = 0;

	BOOST_FOREACH( Node &f, n.second.get_child("values") )
	{
		unsigned int size;
		
		HeaderSegmentStatus fieldStatus = eStatusNotValid;

		if( boost::optional<bpt::ptree &> length = f.second.get_child_optional("length") ) 
		{
			size = f.second.get<unsigned int>("length");
			// dataSize -= size;
			// COMMON_COUT( "length : " << size );

			char buffer[size];
			// Header to buffer
			if (_fileReader.open()) 
			{	
				_fileReader.read(buffer, size);
			}
			std::stringstream ssh;
			for( size_t i = 0; i < size; ++i )
			{
				ssh << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buffer[i];
			}
			_hexaHeader = _hexaHeader + ssh.str();

			fieldStatus = testNode( f , buffer, size);

			if( f.second.get<std::string>("id") == "subChunk size" ) 
			{
				std::stringstream ss;
				for (int i = size-1; i >=0; --i)
				{
					ss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buffer[i];
				}
				int buff;
				ss >> buff;
				std::cout << "buff : " << buff << " : " << ss.str() << std::endl;
				dataSize += buff;
			}	
		}
		else
		{
			size = dataSize;
			std::cout << "size :" << size << std::endl;	

			char buffer[size];
			// Header to buffer :
			if (_fileReader.open()) 
			{	
				_fileReader.read(buffer, size);
			}

			//std::stringstream ssh;
			//for (int i = 0; i < size; ++i)
			//{
			//	ssh << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buffer[i];
			//}
			//_hexaHeader = _hexaHeader + ssh.str();
			//fieldStatus = testNode( f , buffer, size);
			fieldStatus = eStatusPass;
		}


		displayResults( f.second.get<std::string>("shortId"), fieldStatus );

		chunkNodeValid = fieldStatus;

		// chunkNodeindex++;
	}
	return chunkNodeValid;
}

template< typename CheckerType >
void analyse( Node &n, char* buffer, size_t size )
{
	CheckerType checker;
	checker.isValid( n, buffer, size );
	n.second.put( kStatus, checker.getStatus() );
}

HeaderSegmentStatus Header::testNode( Node &n, char* buffer, size_t size )
{
	HeaderSegmentStatus nodeValid = eStatusNotValid;

	std::string type = n.second.get<std::string>( kType );
	
	if( type == kTypeHexa )
	{
		analyse< HexaChecker >( n, buffer, size );
	}

	if( type == kTypeAscii )
	{
		analyse< AsciiChecker >( n, buffer, size );
	}

	if( type == kTypeInt )
	{
		analyse< IntChecker >( n, buffer, size );
	}

	/** FLOAT **/
	if( type == "float" )
	{
		std::stringstream ss;
		for( size_t i = 0; i < size; ++i)
		{
			for( int i = size-1; i >= 0; --i )
			{
				ss << std::dec << std::setw( 2 ) << std::setfill( '0' ) << (int)(unsigned char)buffer[i];
			}
		}
		
		float buff = strtof( ss.str().c_str(), NULL );
		COMMON_COUT( "FLOAT : " << buff << " : "<< ss.str() << " : " );
		
		if( boost::optional<bpt::ptree &> value = n.second.get_child_optional(kValue) )
		{
			if( buff == n.second.get<float>(kValue) )
			{
				nodeValid = eStatusValid;
			}
		}
		else
		{
			nodeValid = eStatusPass;
		}
		n.second.put(kValue, buff);
		n.second.put("status", nodeValid);
	}

	/** VERSION **/
	if( type == "version" )
	{
		std::stringstream ss;
		ss << std::dec << (int)(unsigned char)buffer[0];
		ss << "."<< std::dec << (int)(unsigned char)buffer[1];
		// std::cout << "VERSION : " << ss.str() << std::endl;
		nodeValid = eStatusPass;
		n.second.put(kValue, ss.str());
		n.second.put("status", nodeValid);
	}

	/** LABEL **/
	if( type == "label" )
	{
		if( boost::optional<bpt::ptree &> labels = n.second.get_child_optional("labels") ) 
		{
			BOOST_FOREACH( Node &m, *labels)
			{
				// HeaderSegmentStatus str = testNode( m , buffer, size);
				if ( eStatusValid == testNode( m , buffer, size))
				{
					nodeValid = eStatusValid;
				}
			}
		}
		else
		{
			COMMON_COUT( "Not standard !!" );
		}
	}

	/** BITS **/
	if( type == "bits" )
	{
		if( boost::optional<bpt::ptree &> bits = n.second.get_child_optional("bits") ) 
		{
			std::stringstream ss;
			for( size_t i = 0; i < size; ++i )
			{
				ss << buffer[i];
			}
			int buff;
			ss >> buff;
			// std::cout << "BITS : " << buff << std::endl;

			int offset = 7;
			BOOST_FOREACH( Node &m, *bits )
			{
				if ( m.second.get<int>("nbit") == 1 )
				{
					// std::cout << "bit : " << (buff & (1 << offset)) << std::endl;
					if( boost::optional<bpt::ptree &> value = m.second.get_child_optional(kValue) )
					{
						if ( (buff & (1 << offset)) == m.second.get<int>(kValue) )
						{
							nodeValid = eStatusValid;
						}
					}
					m.second.put(kValue, (buff & (1 << offset)) );
				}

				else if ( m.second.get<int>("nbit") > 1 )
				{
					std::stringstream sb;
					for (int i = 0; i < m.second.get<int>("nbit"); ++i)
					{
						sb << (buff & (1 << (offset-i)) );
					}
					int bit;
					sb >> bit;
					// std::cout << "bits : " << bit << std::endl;

					if( boost::optional<bpt::ptree &> value = m.second.get_child_optional(kValue) )
					{
						if ( bit == m.second.get<int>(kValue) )
						{
							nodeValid = eStatusValid;
						}
					}
					m.second.put(kValue, sb.str() );
				}
				offset -= m.second.get<int>("nbit");
			}
		}
		else
		{
			COMMON_COUT( "Not standard !!" );
		}
	}
	
	/** RCHOICE **/
	if( type == "chunk" )
	{
		if( boost::optional<bpt::ptree &> chunks = n.second.get_child_optional("rChoice") ) 
		{
			BOOST_FOREACH( Node &m, *chunks)
			{
				if ( eStatusValid == testNode( m , buffer, m.second.get<int>("length") ) )
				{
					// Chunk test
					HeaderSegmentStatus chunkStatus = analyseChunkNode( m );				
					displayResults( m.second.get<std::string>("shortId"), chunkStatus );
					nodeValid = chunkStatus;
				}
			}
		}
		else
		{
			COMMON_COUT( "Not standard !!" );
		}
	}
	return nodeValid;
}

void Header::displayResults( std::string id, HeaderSegmentStatus status )
{
	COMMON_COUT( id << " : " << status );
}

void Header::printReport()
{
	COMMON_COUT( _fileReader );
	COMMON_COUT( "standard:" << specName );
	
	COMMON_COUT( "Header : " << ( reportStatus ? "OK" : "Error" ) );
	COMMON_COUT( ">>> " << _hexaHeader );
}
