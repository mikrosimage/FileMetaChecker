#ifndef _SPECREADER_SPECLIST_HPP_
#define _SPECREADER_SPECLIST_HPP_

#include <Specification/Specification.hpp>

class SpecList
{
public:
	SpecList();
	~SpecList();

	void initDirectoryPaths();
	void addDirectoryPath( const std::string& directoryPath );
	void addSpecFromDirectories();
	void addSpecification( const Specification& spec );

	void clearSpecifications();
	void clearDirectories();

	void getSpec( Specification& spec, const size_t index ) const;
	void getSpecList( std::vector< Specification >& specs ) const;
	
	size_t getSpecNumber() const;

private:
	std::vector< Specification >  _specifications;
	std::vector< std::string >    _directories;
};





#endif
