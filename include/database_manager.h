#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <pqxx/pqxx>
#include <string>
#include <iostream>

class DatabaseManager {
public:
    DatabaseManager(const std::string& connectionString);
    ~DatabaseManager();

    void connect();
    void disconnect();
    void persistKeyValue(const std::string& key, const std::string& value);
    std::string fetchValue(const std::string& key);

private:
    std::string connectionString;
    pqxx::connection* conn;
};

#endif
