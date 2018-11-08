#ifndef PURE2D_PRIVATE_UTIL_H	
#define PURE2D_PRIVATE_UTIL_H

#include <unordered_map>
#include <string>
#include <type_traits>

// TODO: Consider moving this to some util public interface.
#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof(std::remove_pointer_t<std::decay<decltype(arr)>::type>))
#define vec_bounds_assert(arr, index) assert(index >= 0 && index < arr.size()) 

template <typename K, typename V>
V* mapGet(std::unordered_map<K, V>& map, const K& key)
{
	auto itr = map.find(key);
	if (itr == map.end()) return nullptr;

	return &itr->second;
}




#endif // PURE2D_PRIVATE_UTIL_H
