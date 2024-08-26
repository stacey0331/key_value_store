#ifndef KEY_VALUE_STORE_H
#define KEY_VALUE_STORE_H

#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <stdexcept>
#include <deque>
#include "eviction_policy.h"
#include "lru.h"
#include "lfu.h"
#include "database_manager.h"

class KeyValueStore {
    public:
        KeyValueStore();

        // size_t setCapacity(const size_t newCapacity);
        // size_t useLRU();
        // size_t useLFU();

        // expiration
        size_t expire(const size_t storeId, const std::string& key, const std::chrono::seconds& sec);
        // size_t persist(const std::string& key);

        // strings
        std::string set(const size_t storeId, const std::string& key, const std::string& val);
        std::optional<std::string> get(const size_t storeId, const std::string& key);
        size_t del(const size_t storeId, const std::string& key);

        // // lists
        // size_t lPush(const std::string& key, const std::string& val);
        // size_t rPush(const std::string& key, const std::string& val);
        // std::optional<std::string> lPop(const std::string& key);
        // std::optional<std::string> rPop(const std::string& key);
        // std::optional<std::deque<std::string>> lRange(const std::string& key, const int& start, const int& end);
        // size_t lLen(const std::string& key);
        
        // // sets
        // size_t sAdd(const std::string& key, const std::string& val);
        // size_t sRem(const std::string& key, const std::string& val);
        // std::unordered_set<std::string> sMembers(const std::string& key);
        // size_t sIsMember(const std::string& key, const std::string& val);
        // size_t sCard(const std::string& key);

    private:
        // std::unordered_map<std::string, Value> store;
        // std::unordered_map<std::string, std::chrono::time_point<std::chrono::steady_clock>> expiration;
        // size_t capacity;
        // std::unique_ptr<EvictionPolicy> evictionPolicy;

        DatabaseManager dbManager;

        // helpers
        bool isExpired(const size_t storeId, const std::string& key, const std::string& type);
};

class TypeMismatchError : public std::runtime_error {
    public:
        explicit TypeMismatchError(const std::string& key, const std::string& type);
};

#endif