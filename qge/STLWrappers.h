#pragma once

/// This file defines some STL wrapper functions that provide a simpler interface to the STL containers.
/// Provides the following functions for all STL containers.
/// -find(inContainer, item) -> iterator to item if found, else end iterator
/// -contains(container, item) -> true if container contains item, else false
/// -containsAll(container, items) -> true if container contains all the specified items
/// -containsAny(container,items) -> true if container contains any of the specified items
/// -add(inContainer, item) -> adds item to the container
/// -remove(fromContainer, item) -> removes item from the container
/// -count(inContainer,item) -> number of times item is in the container
/// All functions use the most efficient search, add, and removal operations available for the container.
/// @file

#include "Vendor.h"

/// This namespace contains some STL wrapper functions that provide a simpler interface to the STL.
/// Read the STLWrappers.h documentation for more info.
namespace STLWrappers
{

	/// @name find(inContainer, item)
	/// Finds an item in a container.
	/// Returns an iterator to the item if found, otherwise returns the end iterator.
	/// If the container is a map (or unordered map) 'item' should be a key.
	/// In that case, if the key is found, you will get an iterator to the key-value pair, otherwise, you will get the end iterator.
	/// @note The most efficient search algorithm available for the container is used.
	///@{
	///
	/// Generic find() overload that works on any container. Complexity is linear.
	template<typename ContainerType, typename ItemType>
	auto find(const ContainerType& inContainer, const ItemType& item)
	{
		return std::find(std::begin(inContainer), std::end(inContainer), item);
	}
	///
	/// find() overload for set, uses binary search tree, thus complexity is logarithmic.
	template<typename ItemType>
	auto find(const std::set<ItemType>& inContainer, const ItemType& item)
	{
		return inContainer.find(item);
	}
	///
	/// find() overload for unordered set, uses hash table, thus complexity is constant.
	template<typename ItemType>
	auto find(const std::unordered_set<ItemType>& inContainer, const ItemType& item)
	{
		return inContainer.find(item);
	}
	///
	/// find() overload for map, uses binary search tree, thus complexity is logarithmic.
	template<typename KeyType, typename ValueType>
	auto find(const std::map<KeyType,ValueType>& inContainer, const KeyType& item)
	{
		return inContainer.find(item);
	}
	///
	/// find() overload for unordered map, uses hash table, thus complexity is constant.
	template<typename KeyType, typename ValueType>
	auto find(const std::unordered_map<KeyType, ValueType>& inContainer, const KeyType& item)
	{
		return inContainer.find(item);
	}
	///@}

	/// Returns true if the specified container contains the specified item.
	/// If the container is a map (or unordered map), 'item' should be a key.
	/// @note The most efficient search algorithm available for the container is used.
	template<typename ContainerType, typename ItemType>
	bool contains(const ContainerType& container, const ItemType& item)
	{
		return find(container, item) != std::end(container);
	}

	/// @name containsAll(container,items)
	/// Returns true if the specified container contains *all* the specified items.
	/// If the container is a map (or unordered map), the items should be keys.
	///@{
	/// containsAll() overload for all types except initializer lists
	template<typename ContainerToCheckType, typename ContainerOfItemsType>
	bool containsAll(const ContainerToCheckType& container, const ContainerOfItemsType& items)
	{
		// approach:
		// - for each item in items
		//		- if its not contained in container, return false
		// - at the end (return true)

		for (const auto& item : items)
			if (!contains(container, item))
				return false;
		return true;
	}
	///
	/// containsAll() overload for initializer lists
	template<typename ContainerToCheckType, typename ItemType>
	bool containsAll(const ContainerToCheckType& container, const std::initializer_list<ItemType>& items)
	{
		// approach:
		// - for each item in items
		//		- if its not contained in container, return false
		// - at the end (return true)

		for (const auto& item : items)
			if (!contains(container, item))
				return false;
		return true;
	}
	///@}

    /// @name containsAny(container,items)
    /// Returns true if the specified container contains *any* of the specified items.
    /// If the container is a map (or unordered map), the items should be keys.
    ///@{
    /// containsAny() overload for all types except initializer lists
    template<typename ContainerToCheckType, typename ContainerOfItemsType>
    bool containsAny(const ContainerToCheckType& container, const ContainerOfItemsType& items)
    {
        // approach:
        // - for each item in items
        //		- if its is contained in container, return true
        // - at the end return false

        for (const auto& item : items)
            if (contains(container, item))
                return true;
        return false;
    }
    ///
    /// containsAny() overload for initializer lists
    template<typename ContainerToCheckType, typename ItemType>
    bool containsAny(const ContainerToCheckType& container, const std::initializer_list<ItemType>& items)
    {
        // approach:
        // - for each item in items
        //		- if its is contained in container, return true
        // - at the end return false

        for (const auto& item : items)
            if (contains(container, item))
                return true;
        return false;
    }
    ///@}


	/// @name remove(fromContainer, item)
	/// Removes the specified item from the container (if it exists).
	/// @note The most efficient removal algorithm available for the container is used.
	///@{
	///
	/// remove() Overload that works for any container that the "erase and remove" idiom works for.
	/// Complexity is linear.
	template<typename ContainerType, typename ItemType>
	void remove(ContainerType& fromContainer, const ItemType& item)
	{
		auto new_end_itr = std::remove(std::begin(fromContainer), std::end(fromContainer), item);
		fromContainer.erase(new_end_itr,std::end(fromContainer));
	}
	///
	/// remove() overload for set, complexity is logarithmic.
	template<typename ItemType>
	void remove(std::set<ItemType>& fromContainer, const ItemType& item)
	{
		fromContainer.erase(item);
	}
	///
	/// remove() overload for unordred set, complexity is constant.
	template<typename ItemType>
	void remove(std::unordered_set<ItemType>& fromContainer, const ItemType& item)
	{
		fromContainer.erase(item);
	}
	/// remove() overload for map, complexity is logarithmic.
	template<typename KeyType, typename ValueType>
	void remove(std::map<KeyType,ValueType>& fromContainer, const KeyType& item)
	{
		fromContainer.erase(item);
	}
	///
	/// remove() overload for unordered map, complexity is constant.
	template<typename KeyType, typename ValueType>
	void remove(std::unordered_map<KeyType, ValueType>& fromContainer, const KeyType& item)
	{
		fromContainer.erase(item);
	}
	///@}

	/// Adds an item to the end of a container.
	/// @note Uses the most efficient insertion operation available for the container.
	template<typename ContainerType, typename ItemType>
	void add(ContainerType& inContainer, const ItemType& item)
	{
		inContainer.insert(std::end(inContainer), item);
	}

	/// Adds a key and value to a map (or unordered map).
	template<typename  MapType, typename KeyType, typename ValueType>
	void add(MapType& inMap, const KeyType& key, const ValueType& value)
	{
		inMap[key] = value;
	}

	/// @name count(inContianer, item)
	/// Returns the number of copies of an item that are in a container.
	/// @note Uses the most efficient search operation available for the container.
	///@{
	///
	/// Generic count() overload that works for any container.
	template<typename ContainerType, typename ItemType>
	size_t count(const ContainerType& inContainer, const ItemType& item)
	{
		return std::count(std::begin(inContainer), std::end(inContainer), item);
	}
	///
	/// count() overload for set, complexity is logarithmic.
	template<typename ItemType>
	size_t count(const std::set<ItemType>& inContainer, const ItemType& item)
	{
		return inContainer.count(item);
	}
	///
	/// count() overload for unordered set, complexity is constant.
	template<typename ItemType>
	size_t count(const std::unordered_set<ItemType>& inContainer, const ItemType& item)
	{
		return inContainer.count(item);
	}
	///
	/// count() overload for map, complexity is logarithmic.
	template<typename KeyType, typename ValueType>
	size_t count(const std::map<KeyType,ValueType>& inContainer, const KeyType& item)
	{
		return inContainer.count(item);
	}
	///
	/// count() overload for unordered map, complexity is constant.
	template<typename KeyType, typename ValueType>
	size_t count(const std::unordered_map<KeyType, ValueType>& inContainer, const KeyType& item)
	{
		return inContainer.count(item);
	}
	///@}

}
