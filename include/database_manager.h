#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <chrono>
#include <optional>

constexpr std::string_view STRING_TABLE = "strings";
constexpr std::string_view LIST_TABLE = "lists";
constexpr std::string_view SET_TABLE = "sets";
constexpr std::string_view EVICTION_TABLE = "eviction";

class DatabaseManager {
public:
    DatabaseManager(const std::string& connectionString);
    ~DatabaseManager();

    void connect();
    void disconnect();
    void deleteKey(const size_t storeId, const std::string& key);
    void clearStore(const size_t storeId);

    size_t changePolicy(const size_t storeId, const std::string& policy);
    // bool exceedsCapacity(const size_t storeId);
    // std::string evictLRU(const size_t storeId);
    

    size_t setExpiration(const size_t storeId, const std::string& key, const std::chrono::seconds& sec);
    std::optional<std::chrono::steady_clock::time_point> getExpiration(const size_t storeId, const std::string& key, const std::string& type);
    
    void insertString(const size_t storeId, const std::string& key, const std::string& value);
    void deleteString(const size_t storeId, const std::string& key);
    std::optional<std::string> fetchString(const size_t storeId, const std::string& key);

private:
    std::string connectionString;
    pqxx::connection* conn;
};

#endif
