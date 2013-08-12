#ifndef _SPECREADER_SPECNODE_HPP_
#define _SPECREADER_SPECNODE_HPP_

#include <common/global.hpp>

#include <boost/property_tree/ptree.hpp>

namespace bpt = boost::property_tree;

namespace spec_reader
{

class SpecNode
{
public:
	
	SpecNode( const bpt::ptree::const_iterator node, const size_t& index, const size_t& indexTotal, const SpecNode* parent = NULL );
	~SpecNode();

	std::string getId()    const;
	std::string getLabel() const;
	std::string getType()  const;
	std::string getDisplayType() const;

	std::string getCount() const;
	std::string getRequired();

	std::vector< std::string > getValues() const;

	std::vector< std::pair< std::string, std::string > > getRange() const;
	std::vector< std::pair< std::string, std::string > > getRepetition() const;

	std::map< std::string, std::string > getMap();

	bool isBigEndian() const;
	bool isOptional() const;
	bool isOrdered() const;

	bool hasGroup() const;
	std::string getGroupSize();

	SpecNode next() const;
	SpecNode firstChild() const;
	SpecNode* parent() const;

	bool isLastNode() const;

	size_t getIndex();			//@todo Delete !!!!
	size_t getIndexTotal();		//@todo Delete !!!!

protected:
	std::string getProperty( const std::string& prop ) const;
	std::string getProperty( const std::string& prop, const std::string& defaultValue ) const;

private:
	bpt::ptree::const_iterator _node;
	const SpecNode* _parent;
	size_t _index;
	size_t _indexTotal;
};

}

#endif
