#include "database_manager.h"

DatabaseManager::DatabaseManager(const std::string& connStr) : connectionString(connStr), conn(nullptr) {}

DatabaseManager::~DatabaseManager() {
    if (conn) {
        disconnect();
    }
}

void DatabaseManager::connect() {
    if (!conn) {
        conn = new pqxx::connection(connectionString);
    }
}

void DatabaseManager::disconnect() {
    if (conn) {
        delete conn;
        conn = nullptr;
    }
}

void DatabaseManager::insertString(const size_t userId, const std::string& key, const std::string& value) {
    pqxx::work txn(*conn);
    txn.exec0("INSERT INTO strings (user_id, key, value) VALUES (" 
                + std::to_string(userId) + ", "
                + txn.quote(key) + ", "
                + txn.quote(value) + ") "
                "ON CONFLICT (user_id, key) DO UPDATE "
                "SET value = excluded.value;");
    txn.commit();
}

std::optional<std::string> DatabaseManager::fetchString(const size_t userId, const std::string& key) {
    pqxx::work txn(*conn);
    pqxx::result res = txn.exec(
        "SELECT value FROM " + std::string(STRING_TABLE) + 
        " WHERE user_id = " + std::to_string(userId) + 
        " AND key = " + txn.quote(key)
    );
    txn.commit();
    if (!res.empty()) {
        return res[0][0].c_str();
    } else {
        return std::nullopt;
    }
}
