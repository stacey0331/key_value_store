#include <algorithm>
#include "key_value_store.h"

TypeMismatchError::TypeMismatchError(const std::string& msg)
    : std::runtime_error(msg) {}

KeyValueStore::KeyValueStore() 
    : store(), capacity(0) {}

/*
    Set key to hold the string value. 
    If key already holds a value, it is overwritten, regardless of its type. 
    Any previous time to live associated with the key is discarded on successful SET operation.
*/
void KeyValueStore::set(const std::string& key, const std::string& val) {
    store.insert_or_assign(key, Value(val));
}

/*
    Get the value of key. 
    If the key does not exist the special value nil is returned. 
    An error is returned if the value stored at key is not a string, because GET only handles string values.
*/
std::optional<std::string> KeyValueStore::get(const std::string& key) {
    auto it = store.find(key);
    if (it == store.end()) {
        return std::nullopt;
    }

    if (!it->second.isString()) {
         throw TypeMismatchError("The value at key " + key + " is not a string.");
    }
    return it->second.getString();
}

/* 
    Removes the specified keys. A key is ignored if it does not exist. 
    Integer reply: the number of keys that were removed.
*/
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
        throw TypeMismatchError("The value at key " + key + " is not a list.");
    }

    auto& lst = it->second.getList();
    lst.push_front(val);
    return lst.size();
}

/*
    Insert all the specified values at the tail of the list stored at key. 
    If key does not exist, it is created as empty list before performing the push operation. 
    When key holds a value that is not a list, an error is returned.

    It is possible to push multiple elements using a single command call just specifying multiple arguments at the end of the command. 
    Elements are inserted one after the other to the tail of the list, from the leftmost element to the rightmost element. 
    So for instance the command RPUSH mylist a b c will result into a list containing a as first element, b as second element and c as third element.
*/
int KeyValueStore::rPush(const std::string& key, const std::string& val) {
    auto [it, inserted] = store.emplace(key, std::deque<std::string>());
    
    if (!inserted && !it->second.isList()) {
        throw TypeMismatchError("The value at key " + key + " is not a list.");
    }

    auto& lst = it->second.getList();
    lst.push_back(val);
    return lst.size();
}

/*
    Removes and returns the first elements of the list stored at key.
    By default, the command pops a single element from the beginning of the list. 
    When provided with the optional count argument, the reply will consist of up to count elements, depending on the list's length.
*/
std::optional<std::string> KeyValueStore::lPop(const std::string& key) {
    auto it = store.find(key);
    if (it == store.end() || !it->second.isList() || it->second.getList().empty()) {
        return std::nullopt;
    }

    auto& lst = it->second.getList();
    auto ret = std::move(lst.front());
    lst.pop_front();
    return ret;
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
    if (start > end || start < 0 || end < 0) return std::nullopt;
    
    auto it = store.find(key);
    if (it == store.end() || !it->second.isList()) return std::nullopt;

    auto& lst = it->second.getList();
    if (start >= lst.size()) return std::nullopt;

    auto endIdx = std::min(static_cast<size_t>(end), lst.size()-1);
    return std::deque<std::string>(lst.begin()+start, lst.begin()+endIdx+1);
}

/*
    Returns the length of the list stored at key. 
    If key does not exist, it is interpreted as an empty list and 0 is returned. 
    An error is returned when the value stored at key is not a list.
*/
int KeyValueStore::lLen(const std::string& key) {
    auto it = store.find(key);
    if (it == store.end()) {
        return 0;
    }

    if (!it->second.isList()) {
        throw TypeMismatchError("The value at key " + key + " is not a list.");
    }
    return it->second.getList().size();
}