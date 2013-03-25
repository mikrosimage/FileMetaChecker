#include "common/global.hpp"
#include "HeaderAnalyser.hpp"

#include <sstream>
#include <cstdlib>

#include <boost/foreach.hpp>

std::ostream& operator<<( std::ostream& out, HeaderSegmentStatus status )
{
	switch( status )
	{
		case isNotValid: out << "not valid"; break;
		case isValid:    out << "valid"; break;
		case pass:       out << "passed"; break;
	}
	return out;
}

HeaderAnalyser::HeaderAnalyser( const std::string& filePath )
{
	_fileReader.setFilePath( filePath );
	_fileReader.extractFileInfo();
	COMMON_COUT( _fileReader );

	JsonParser jsonParser;
	jsonParser.readJsonFolder( "json" );

	bpt::ptree pt;

	for( size_t index = 0; index < jsonParser._ptreeList.size(); ++index )
	{
		if( testExtension( jsonParser._ptreeList.at( index ), _fileReader._fileInfo.extension ) )
		{
			pt = jsonParser._ptreeList.at( index );
			COMMON_COUT( "ptree n°: " << index );
			COMMON_COUT( pt.get<std::string>( "fileType.name" ) );
		}
	}

	_fileReader.openFile();

	COMMON_COUT( "Header : " << ( analyseFileHeader( pt ) ? "OK" : "Error" ) );
	COMMON_COUT( ">>> " << _hexaHeader );
	writeReport( pt );
}

bool HeaderAnalyser::testExtension( bpt::ptree &pt, const std::string& extension )
{
	bool validExtension = false;

	// format extension :
	std::string ext = extension;
	std::transform( ext.begin(), ext.end(), ext.begin(), ::tolower );
	ext.erase( 0, 1 );

	// read extension list :
	BOOST_FOREACH( Node &node, pt.get_child( "fileType.extension" ) )
	{
		if ( ext == node.second.data())
		{
			validExtension = true;
			// COMMON_COUT( "Valid extension : " << ext );
		}
	}
	return validExtension;
}




bool HeaderAnalyser::analyseFileHeader( bpt::ptree &pt )
{
	bool isValid = false;
	_hexaHeader = "";

	BOOST_FOREACH( Node &n, pt.get_child( "header" ) )
	{
		unsigned int size = n.second.get<unsigned int>( "length" );
		char buffer[size];

		// read header information
		if( _fileReader._file.is_open() )
		{	
			_fileReader._file.read( buffer, size );
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

		isValid = ( nodeStatus == isNotValid ) ? false : true;
	}

	return isValid;
}

HeaderSegmentStatus HeaderAnalyser::analyseChunkNode( Node &n )
{
	HeaderSegmentStatus chunkNodeValid = isNotValid;
	COMMON_COUT( "Chunk : " << n.second.get<std::string>("shortId") );
	
	// header
	unsigned int dataSize = 0;

	BOOST_FOREACH( Node &f, n.second.get_child("values") )
	{
		unsigned int size;
		
		HeaderSegmentStatus fieldStatus = isNotValid;

		if( boost::optional<bpt::ptree &> length = f.second.get_child_optional("length") ) 
		{
			size = f.second.get<unsigned int>("length");
			// dataSize -= size;
			// COMMON_COUT( "length : " << size );

			char buffer[size];
			// Header to buffer
			if (_fileReader._file.is_open()) 
			{	
				_fileReader._file.read(buffer, size);
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
			if (_fileReader._file.is_open()) 
			{	
				_fileReader._file.read(buffer, size);
			}

			//std::stringstream ssh;
			//for (int i = 0; i < size; ++i)
			//{
			//	ssh << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buffer[i];
			//}
			//_hexaHeader = _hexaHeader + ssh.str();
			//fieldStatus = testNode( f , buffer, size);
			fieldStatus = pass;
		}


		displayResults( f.second.get<std::string>("shortId"), fieldStatus );

		chunkNodeValid = fieldStatus;

		// chunkNodeindex++;
	}
	return chunkNodeValid;
}




HeaderSegmentStatus HeaderAnalyser::testNode( Node &n , char* buffer, unsigned int size)
{
	HeaderSegmentStatus nodeValid = isNotValid;

	/** HEXA **/
	if( n.second.get<std::string>("type") == "hexa" )
	{
		std::stringstream ss;
		for( size_t i = 0; i < size; ++i)
		{
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buffer[i];
		}
		std::cout << "HEXA : " << ss.str() << " : ";
		if( boost::optional<bpt::ptree &> value = n.second.get_child_optional("value") )
		{
			std::cout << " | " << n.second.get<std::string>("value") << std::endl;
			if( ss.str() == n.second.get<std::string>("value") )
			{
				nodeValid = isValid;
			}
		}
		else
		{
			nodeValid = pass;
		}
		n.second.put("value", ss.str());
		n.second.put("status", nodeValid);
	}

	/** ASCII **/
	if( n.second.get<std::string>("type") == "ascii" )
	{
		std::stringstream ss;
		std::string str;
		for( size_t i = 0; i < size; ++i)
		{
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)buffer[i];
			str.push_back((char) std::strtol(ss.str().c_str(), NULL, 16));
			// str.push_back((char)buffer[i]);
		}
		std::cout << "ASCII : " << str << " : " << ss.str() << " : ";
		if( boost::optional<bpt::ptree &> value = n.second.get_child_optional("value") )
		{
			// std::cout << " | " << n.second.get<std::string>("value") << std::endl;
			if( str == n.second.get<std::string>("value") )
			{
				nodeValid = isValid;
			}
		}
		else
		{
			nodeValid = pass;
		}
		n.second.put("value", str);
		n.second.put("status", nodeValid);
	}

	/** INT **/
	if( n.second.get<std::string>("type") == "int" )
	{
		std::stringstream ss;
		for( size_t i = 0; i < size; ++i )
		if( n.second.get<std::string>("endian") == "little" )
		{
			for( int i = size-1; i >= 0; --i )
			{
				ss << std::hex << std::setw( 2 ) << std::setfill( '0' ) << (int)(unsigned char)buffer[i];
			}
		}
		else
		{
			for( size_t i = 0; i < size; ++i )
			{
				ss << std::hex << std::setw( 2 ) << std::setfill( '0' ) << (int)(unsigned char)buffer[i];
			}
		}
		int buff;
		ss >> buff;
		COMMON_COUT( "INT : " << buff << " : " << ss.str() << " : " );
		

		if( boost::optional<bpt::ptree &> value = n.second.get_child_optional("value") )
		{
			if( buff == n.second.get<int>("value") )
			{
				nodeValid = isValid;
			}
		}
		/** CHOICE **/
		else if( boost::optional<bpt::ptree &> choice = n.second.get_child_optional("choice") ) 
		{
			BOOST_FOREACH( Node &m, *choice)
			{
				// HeaderSegmentStatus str = testNode( m , buffer, size);
				if( buff == m.second.get_value<int>() )
				{
					nodeValid = isValid;
				}
			}
		}
		/** INTERVAL **/
		else if( boost::optional<bpt::ptree &> choice = n.second.get_child_optional("interval") ) 
		{
			std::vector<int> minMax;
			BOOST_FOREACH( Node &m, *choice)
			{
				minMax.push_back( m.second.get_value<int>() );
			}
			if( minMax.at(0) < buff && buff < minMax.at(1) )
			{
				nodeValid = isValid;
			}
		}
		else
		{
			nodeValid = pass;
		}

		n.second.put("value", buff);
		n.second.put("status", nodeValid);
	}

	/** FLOAT **/
	if( n.second.get<std::string>("type") == "float" )
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
		
		if( boost::optional<bpt::ptree &> value = n.second.get_child_optional("value") )
		{
			if( buff == n.second.get<float>("value") )
			{
				nodeValid = isValid;
			}
		}
		else
		{
			nodeValid = pass;
		}
		n.second.put("value", buff);
		n.second.put("status", nodeValid);
	}

	/** VERSION **/
	if( n.second.get<std::string>("type") == "version" )
	{
		std::stringstream ss;
		ss << std::dec << (int)(unsigned char)buffer[0];
		ss << "."<< std::dec << (int)(unsigned char)buffer[1];
		// std::cout << "VERSION : " << ss.str() << std::endl;
		nodeValid = pass;
		n.second.put("value", ss.str());
		n.second.put("status", nodeValid);
	}


	/** LABEL **/
	if( n.second.get<std::string>("type") == "label" )
	{
		if( boost::optional<bpt::ptree &> labels = n.second.get_child_optional("labels") ) 
		{
			BOOST_FOREACH( Node &m, *labels)
			{
				// HeaderSegmentStatus str = testNode( m , buffer, size);
				if ( isValid == testNode( m , buffer, size))
				{
					nodeValid = isValid;
				}
			}
		}
		else
		{
			COMMON_COUT( "Not standard !!" );
		}
	}


	/** BITS **/
	if( n.second.get<std::string>("type") == "bits" )
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
					if( boost::optional<bpt::ptree &> value = m.second.get_child_optional("value") )
					{
						if ( (buff & (1 << offset)) == m.second.get<int>("value") )
						{
							nodeValid = isValid;
						}
					}
					m.second.put("value", (buff & (1 << offset)) );
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

					if( boost::optional<bpt::ptree &> value = m.second.get_child_optional("value") )
					{
						if ( bit == m.second.get<int>("value") )
						{
							nodeValid = isValid;
						}
					}
					m.second.put("value", sb.str() );
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
	if( n.second.get<std::string>("type") == "chunk" )
	{
		if( boost::optional<bpt::ptree &> chunks = n.second.get_child_optional("rChoice") ) 
		{
			BOOST_FOREACH( Node &m, *chunks)
			{
				if ( isValid == testNode( m , buffer, m.second.get<int>("length") ) )
				{
					// Chunk test :	
					HeaderSegmentStatus chunkStatus = analyseChunkNode( m );				
					displayResults( m.second.get<std::string>("shortId"), chunkStatus );
					nodeValid = chunkStatus;
					// Header to buffer :
					//if (_fileReader._file.is_open()) 
					//{	
					//	_fileReader._file.read(buffer, size);
					//	}
				}
			}
			// Ne passe pas au suivant : avancer dans le buffer !
		}
		else
		{
			COMMON_COUT( "Not standard !!" );
		}
	}
	return nodeValid;
}

void HeaderAnalyser::displayResults( std::string id, HeaderSegmentStatus status )
{
	COMMON_COUT( id << " : " << status );
}

void HeaderAnalyser::writeReport( const bpt::ptree &pt )
{
	write_json( "output.json", pt );
}
