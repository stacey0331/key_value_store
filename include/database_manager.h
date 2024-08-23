#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <pqxx/pqxx>
#include <string>
#include <iostream>

constexpr std::string_view STRING_TABLE = "strings";
constexpr std::string_view LIST_TABLE = "lists";
constexpr std::string_view SET_TABLE = "sets";

class DatabaseManager {
public:
    DatabaseManager(const std::string& connectionString);
    ~DatabaseManager();

    void connect();
    void disconnect();
    void insertString(const size_t userId, const std::string& key, const std::string& value);
    std::optional<std::string> fetchString(const size_t userId, const std::string& key);

private:
    std::string connectionString;
    pqxx::connection* conn;
};

#endif
