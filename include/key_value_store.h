#ifndef KEY_VALUE_STORE_H
#define KEY_VALUE_STORE_H

#include <unordered_map>
#include <optional>
#include "value.h"

class KeyValueStore {
    public:
        KeyValueStore();

        // strings
        void set(const std::string& key, const std::string& val);
        std::optional<std::string> get(const std::string& key);
        void del(const std::string& key);

        // lists
        int lPush(const std::string& key, const std::string& val);
        int rPush(const std::string& key, const std::string& val);
        std::optional<std::string> lPop(const std::string& key);
        std::optional<std::string> rPop(const std::string& key);
        std::optional<std::deque<std::string>> lRange(const std::string& key, const int& start, const int& end);
        int lLen(const std::string& key);
        
        // // sets
        // void sAdd(const std::string& key, const std::string& val);
        // void sRem(const std::string& key, const std::string& val);
        // std::unordered_set<std::string> sMembers(const std::string& key);
        // size_t sIsMember(const std::string& key, const std::string& val);
        // size_t sCard(const std::string& key);

    private:
        std::unordered_map<std::string, Value> store;
        size_t capacity;
};

#endif