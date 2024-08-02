#include <algorithm>
#include "key_value_store.h"

KeyValueStore::KeyValueStore() 
    : store(), capacity(0) {}

void KeyValueStore::set(const std::string& key, const std::string& val) {
    store.insert_or_assign(key, Value(val));
}

std::optional<std::string> KeyValueStore::get(const std::string& key) {
    auto it = store.find(key);
    if (it == store.end()) {
        return std::nullopt;
    }
    return it->second.getString();
}

void KeyValueStore::del(const std::string& key) {
    store.erase(key);
}

/*
    Insert all the specified values at the head of the list stored at key. 
    If key does not exist, it is created as empty list before performing the push operations. 
    When key holds a value that is not a list, an error is returned.

    It is possible to push multiple elements using a single command call just specifying multiple arguments at the end of the command. 
    Elements are inserted one after the other to the head of the list, from the leftmost element to the rightmost element. 
    So for instance the command LPUSH mylist a b c will result into a list containing c as first element, b as second element and a as third element.
*/
int KeyValueStore::lPush(const std::string& key, const std::string& val) {
    auto [it, inserted] = store.emplace(key, std::deque<std::string>());
    
    if (!inserted && !it->second.isList()) {
        return 0;
    }

    auto& lst = it->second.getList();
    lst.push_front(val);
    return lst.size();
}

int KeyValueStore::rPush(const std::string& key, const std::string& val) {
    return 0;
}

std::optional<std::string> KeyValueStore::lPop(const std::string& key) {
    return "";
}

/*
    Removes and returns the last elements of the list stored at key.
    By default, the command pops a single element from the end of the list.
    When provided with the optional count argument, the reply will consist of up to count elements, depending on the list's length.
*/
std::optional<std::string> KeyValueStore::rPop(const std::string& key) {
    auto it = store.find(key);
    if (it == store.end() || !it->second.isList() || it->second.getList().empty()) {
        return std::nullopt;
    }

    auto& lst = it->second.getList();
    auto ret = std::move(lst.back());
    lst.pop_back();
    return ret;
}

/*
    Returns the specified elements of the list stored at key. 
    The offsets start and stop are zero-based indexes, with 0 being the first element of the list (the head of the list), 1 being the next element and so on.
    These offsets can also be negative numbers indicating offsets starting at the end of the list. 
    For example, -1 is the last element of the list, -2 the penultimate, and so on.
    Note that right most element is included. 
*/
std::optional<std::deque<std::string>> KeyValueStore::lRange(const std::string& key, const int& start, const int& end) {
    auto it = store.find(key);
    if (it == store.end() || !it->second.isList()) return std::nullopt;

    auto& lst = it->second.getList();
    if (start >= lst.size()) return std::nullopt;

    return std::deque<std::string>(lst.begin()+start, lst.begin()+std::min(static_cast<size_t>(end), lst.size()-1)+1);
}

/*
    Returns the length of the list stored at key. 
    If key does not exist, it is interpreted as an empty list and 0 is returned. 
    An error is returned when the value stored at key is not a list.
*/
int KeyValueStore::lLen(const std::string& key) {
    auto it = store.find(key);
    if (it == store.end()) return 0;
    if (!it->second.isList()) return -1;
    return it->second.getList().size();
}