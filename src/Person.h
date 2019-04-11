#pragma once

#include <string>

class Person {
public:
    explicit Person(const std::string& name, double amount);

    std::string name_;
    double amount_;
};