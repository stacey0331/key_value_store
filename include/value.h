#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <deque>
#include <unordered_set>
#include <variant>
#include <iostream>

class Value {
public:
    enum class Type {
        STRING,
        LIST,
        SET
    };

    Value(const std::string& str) : type(Type::STRING), value{str} {}
    Value(const std::deque<std::string>& lst) : type(Type::LIST), value{lst} {}
    Value(const std::unordered_set<std::string>& set) : type(Type::SET), value{set} {}

    Type getType() const { return type; }

    bool isString() const { return type == Type::STRING; }
    bool isList() const { return type == Type::LIST; }
    bool isSet() const { return type == Type::SET; }

    std::string getString() const {
        if (type == Type::STRING) {
            return std::get<std::string>(value);
        }
        throw std::bad_variant_access();
    }

    std::deque<std::string> getList() const {
        if (type == Type::LIST) {
            return std::get<std::deque<std::string>>(value);
        }
        throw std::bad_variant_access();
    }

    std::unordered_set<std::string> getSet() const {
        if (type == Type::SET) {
            return std::get<std::unordered_set<std::string>>(value);
        }
        throw std::bad_variant_access();
    }

private:
    Type type;
    std::variant<std::string, std::deque<std::string>, std::unordered_set<std::string>> value;
};

#endif