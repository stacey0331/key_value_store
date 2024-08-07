#include <algorithm>
#include "key_value_store.h"

TypeMismatchError::TypeMismatchError(const std::string& key, const std::string& type)
    : std::runtime_error("The value at key " + key + " is not a " + type + ".") {}

KeyValueStore::KeyValueStore() 
    : store(), capacity(0) {}

/*
    SET key value [NX | XX] [GET] [EX seconds | PX milliseconds |
    EXAT unix-time-seconds | PXAT unix-time-milliseconds | KEEPTTL]

    Set key to hold the string value. 
    If key already holds a value, it is overwritten, regardless of its type. 
    Any previous time to live associated with the key is discarded on successful SET operation.

    Nil reply: GET not given: Operation was aborted (conflict with one of the XX/NX options).
    Simple string reply: OK. GET not given: The key was set.
    Nil reply: GET given: The key didn't exist before the SET.
    Bulk string reply: GET given: The previous value of the key.
*/
std::string KeyValueStore::set(const std::string& key, const std::string& val) {
    store.insert_or_assign(key, Value(val));
    return "OK";
}

/*
    GET key

    Get the value of key. 
    If the key does not exist the special value nil is returned. 
    An error is returned if the value stored at key is not a string, because GET only handles string values.

    Bulk string reply: the value of the key.
    Nil reply: if the key does not exist.
*/
std::optional<std::string> KeyValueStore::get(const std::string& key) {
    auto it = store.find(key);
    if (it == store.end()) {
        return std::nullopt;
    }

    if (!it->second.isString()) {
         throw TypeMismatchError(key, "stirng");
    }
    return it->second.getString();
}

/* 
    DEL key [key ...]

    Removes the specified keys. A key is ignored if it does not exist. 
    
    Integer reply: the number of keys that were removed.
*/
size_t KeyValueStore::del(const std::string& key) {
    return store.erase(key);
}

/*
    LPUSH key element [element ...]

    Insert all the specified values at the head of the list stored at key. 
    If key does not exist, it is created as empty list before performing the push operations. 
    When key holds a value that is not a list, an error is returned.

    It is possible to push multiple elements using a single command call just specifying multiple arguments at the end of the command. 
    Elements are inserted one after the other to the head of the list, from the leftmost element to the rightmost element. 
    So for instance the command LPUSH mylist a b c will result into a list containing c as first element, b as second element and a as third element.

    Integer reply: the length of the list after the push operation.
*/
size_t KeyValueStore::lPush(const std::string& key, const std::string& val) {
    auto [it, inserted] = store.emplace(key, std::deque<std::string>());
    
    if (!inserted && !it->second.isList()) {
        throw TypeMismatchError(key, "list");
    }

    auto& lst = it->second.getList();
    lst.push_front(val);
    return lst.size();
}

/*
    RPUSH key element [element ...]

    Insert all the specified values at the tail of the list stored at key. 
    If key does not exist, it is created as empty list before performing the push operation. 
    When key holds a value that is not a list, an error is returned.

    It is possible to push multiple elements using a single command call just specifying multiple arguments at the end of the command. 
    Elements are inserted one after the other to the tail of the list, from the leftmost element to the rightmost element. 
    So for instance the command RPUSH mylist a b c will result into a list containing a as first element, b as second element and c as third element.

    Integer reply: the length of the list after the push operation.
*/
size_t KeyValueStore::rPush(const std::string& key, const std::string& val) {
    auto [it, inserted] = store.emplace(key, std::deque<std::string>());
    
    if (!inserted && !it->second.isList()) {
        throw TypeMismatchError(key, "list");
    }

    auto& lst = it->second.getList();
    lst.push_back(val);
    return lst.size();
}

/*
    LPOP key [count]

    Removes and returns the first elements of the list stored at key.
    By default, the command pops a single element from the beginning of the list. 
    When provided with the optional count argument, the reply will consist of up to count elements, depending on the list's length.

    Nil reply: if the key does not exist.
    Bulk string reply: when called without the count argument, the value of the first element.
    Array reply: when called with the count argument, a list of popped elements.
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
    RPOP key [count]

    Removes and returns the last elements of the list stored at key.
    By default, the command pops a single element from the end of the list.
    When provided with the optional count argument, the reply will consist of up to count elements, depending on the list's length.

    Nil reply: if the key does not exist.
    Bulk string reply: when called without the count argument, the value of the last element.
    Array reply: when called with the count argument, a list of popped elements.
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
    LRANGE key start stop

    Returns the specified elements of the list stored at key. 
    The offsets start and stop are zero-based indexes, with 0 being the first element of the list (the head of the list), 1 being the next element and so on.
    These offsets can also be negative numbers indicating offsets starting at the end of the list. 
    For example, -1 is the last element of the list, -2 the penultimate, and so on.
    Note that right most element is included. 

    Array reply: a list of elements in the specified range, or an empty array if the key doesn't exist.
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
    LLEN key

    Returns the length of the list stored at key. 
    If key does not exist, it is interpreted as an empty list and 0 is returned. 
    An error is returned when the value stored at key is not a list.

    Integer reply: the length of the list.
*/
size_t KeyValueStore::lLen(const std::string& key) {
    auto it = store.find(key);
    if (it == store.end()) {
        return 0;
    }

    if (!it->second.isList()) {
        throw TypeMismatchError(key, "list");
    }
    return it->second.getList().size();
}

/*
    SADD key member [member ...]

    Add the specified members to the set stored at key. 
    Specified members that are already a member of this set are ignored. 
    If key does not exist, a new set is created before adding the specified members.
    An error is returned when the value stored at key is not a set.

    Integer reply: the number of elements that were added to the set, not including all the elements already present in the set.
*/
size_t KeyValueStore::sAdd(const std::string& key, const std::string& val) {
    auto [it, inserted] = store.emplace(key, std::unordered_set<std::string>());
    if (!inserted && !it->second.isSet()) {
        throw TypeMismatchError(key, "set");
    }

    auto& set = it->second.getSet();
    set.insert(val);
    return 1;
}

/*
    SREM key member [member ...]

    Remove the specified members from the set stored at key. 
    Specified members that are not a member of this set are ignored. 
    If key does not exist, it is treated as an empty set and this command returns 0.
    An error is returned when the value stored at key is not a set.

    Integer reply: the number of members that were removed from the set, not including non existing members.
*/
size_t KeyValueStore::sRem(const std::string& key, const std::string& val) {
    auto it = store.find(key);
    if (it == store.end()) {
        return 0;
    }

    if (!it->second.isSet()) {
        throw TypeMismatchError(key, "set");
    }

    return it->second.getSet().erase(val);
}

/*
    SMEMBERS key

    Returns all the members of the set value stored at key.
    This has the same effect as running SINTER with one argument key.

    Set reply: a set with all the members of the set.
*/
std::unordered_set<std::string> KeyValueStore::sMembers(const std::string& key) {
    auto it = store.find(key);
    if (it == store.end()) {
        return {};
    }

    if (!it->second.isSet()) {
        throw TypeMismatchError(key, "set");
    }
    return it->second.getSet();
}

/*
    SISMEMBER key member

    Returns if member is a member of the set stored at key.

    Integer reply: 0 if the element is not a member of the set, or when the key does not exist.
    Integer reply: 1 if the element is a member of the set.
*/
size_t KeyValueStore::sIsMember(const std::string& key, const std::string& val) {
    auto it = store.find(key);
    if (it == store.end()) {
        return 0;
    }

    if (!it->second.isSet()) {
        throw TypeMismatchError(key, "set");
    }

    const auto& set = it->second.getSet();
    return set.contains(val) ? 1 : 0;
}

/*
    SCARD key

    Returns the set cardinality (number of elements) of the set stored at key.

    Integer reply: the cardinality (number of elements) of the set, or 0 if the key does not exist.
*/
size_t KeyValueStore::sCard(const std::string& key) {
    auto it = store.find(key);
    if (it == store.end()) {
        return 0;
    }

    if (!it->second.isSet()) {
        throw TypeMismatchError(key, "set");
    }
    return it->second.getSet().size();
}