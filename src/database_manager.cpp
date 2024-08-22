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
        // conn->disconnect();
        delete conn;
        conn = nullptr;
    }
}

void DatabaseManager::persistKeyValue(const std::string& key, const std::string& value) {
    pqxx::work txn(*conn);
    txn.exec0("INSERT INTO key_value_store (key, value) VALUES (" + txn.quote(key) + ", " + txn.quote(value) + ") ON CONFLICT (key) DO UPDATE SET value = excluded.value;");
    txn.commit();
}

std::string DatabaseManager::fetchValue(const std::string& key) {
    pqxx::work txn(*conn);
    pqxx::result res = txn.exec("SELECT value FROM key_value_store WHERE key = " + txn.quote(key));
    txn.commit();
    if (!res.empty()) {
        return res[0][0].c_str();
    } else {
        throw std::runtime_error("Key not found");
    }
}
