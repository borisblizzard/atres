/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a cache object.

#ifndef ATRES_CACHE_H
#define ATRES_CACHE_H

#include <hltypes/harray.h>
#include <hltypes/hlist.h>
#include <hltypes/hmap.h>

#include "atres.h"
#include "atresExport.h"

namespace atres
{
	/// @brief Special object that caches calculated text entries.
	/// @note The classes uses a hash value to store objects because it would require the implementation of the
	/// comparison operators if the Entry objects would be used as keys. This is simply an alternate way to handle
	/// things. Even though it appears unnecessary and hacky, there is no better way.
	template <typename T>
	class Cache
	{
	public:
		/// @brief Constructor.
		inline Cache()
		{
			this->maxSize = 1000;
		}
		/// @brief Sets max size for cache.
		/// @param[in] value New max size.
		inline void setMaxSize(int value)
		{
			this->maxSize = value;
			this->update();
		}
		/// @brief Adds a cache entry.
		/// @param[in] entry The cache entry.
		inline T* add(const T& entry)
		{
			unsigned int hash = entry.hash();
			if (!this->data.hasKey(hash))
			{
				this->data[hash] = harray<T*>();
			}
			harray<T*>& dataArray = this->data[hash];
			int index = this->_indexOf(dataArray, entry);
			if (index < 0) // this prevents duplicates
			{
				index = dataArray.size();
				T* newEntry = new T(entry);
				dataArray += newEntry;
				this->entries += newEntry;
			}
			return dataArray[index];
		}
		/// @brief Gets a cache entry.
		/// @param[in] entry The cache entry.
		/// @return The entry.
		inline T* get(const T& entry)
		{
			unsigned int hash = entry.hash();
			if (this->data.hasKey(hash))
			{
				harray<T*>& dataArray = this->data[hash];
				int index = this->_indexOf(dataArray, entry);
				if (index >= 0)
				{
					return dataArray[index];
				}
			}
			return NULL;
		}
		/// @brief Clears cache.
		inline void clear()
		{
			foreach_l (T*, it, this->entries)
			{
				delete (*it);
			}
			this->entries.clear();
			this->data.clear();
		}
		/// @brief Gets the current size of the cache.
		/// @return The current size of the cache.
		inline int getSize() const
		{
			return this->entries.size();
		}
		/// @brief Updates all cache entries.
		inline void update()
		{
			if (this->maxSize < 0)
			{
				return;
			}
			int overSize = this->entries.size() - this->maxSize;
			if (overSize <= 0)
			{
				return;
			}
			// manual implementation due to crashes happening with removeEntry
			hlist<T*> removed = this->entries.removeFirst(overSize);
			unsigned int hash = 0;
			int index = 0;
			harray<T*> dataArray;
			foreach_l (T*, it, removed)
			{
				hash = (*it)->hash();
				if (this->data.hasKey(hash))
				{
					dataArray = this->data[hash];
					index = dataArray.indexOf(*it);
					if (index >= 0) // safety guard, should never be < 0, but crashes have been happening
					{
						if (dataArray.size() <= 1)
						{
							this->data.removeKey(hash);
						}
						else
						{
							this->data[hash].removeAt(index); // must not use dataArray directly, harray reference doesn't seem to work
						}
					}
				}
				delete (*it);
			}
		}
		
	protected:
		/// @brief Max size of the cache.
		int maxSize;
		/// @brief The cache entries.
		hmap<unsigned int, harray<T*> > data;
		/// @brief A list of all hashes.
		/// @note Using hlist because add/remove has a constant complexity while harray would have to reorder/resize all elements.
		hlist<T*> entries;
		
		/// @brief Finds index of element in data array.
		/// @param[in] entry The cache entry.
		/// @return The entry.
		inline int _indexOf(const harray<T*>& array, const T& element) const
		{
			int size = array.size();
			for_iter (i, 0, size)
			{
				if (array[i]->isEqual(element))
				{
					return i;
				}
			}
			return -1;
		}

	};
	
}
#endif

