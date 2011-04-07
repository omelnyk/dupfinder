/*-------------------------------------------------------------------------
 *
 * vectormap.cpp
 *     Multimap implementation based on std::map<K, vector<V> >
 *
 * Copyright (c) 2011, Olexandr Melnyk <me@omelnyk.net>
 *
 *------------------------------------------------------------------------- 
 */

#ifndef DUPFINDER_VECTORMAP_HPP
#define DUPFINDER_VECTORMAP_HPP

#include <vector>
#include <map>

template <class K, class V, class Compare = std::less<K>,
	class Allocator = std::allocator<std::pair<const K, std::vector<V> > >,
	class VectorAllocator = std::allocator<V> >
class vectormap : public std::map< K, std::vector<V, VectorAllocator>, Compare, Allocator >
{
	typedef std::map<K, std::vector<V, VectorAllocator>, Compare, Allocator > base;
	typedef std::vector<V, VectorAllocator> value_vector;
	using base::insert;

public:
	typename base::iterator insert(const K &key, const value_vector &value)
	{
		std::pair<typename base::iterator, bool> p = insert(make_pair(key, value));
		return p.first;
	}

	void insert(const K &key, const V &value)
	{
		typename base::iterator it = this->find(key);
		if (it == this->end())
			it = insert(key, value_vector());
		it->second.push_back(value);
	}
};

#endif // DUPFINDER_VECTORMAP_HPP

