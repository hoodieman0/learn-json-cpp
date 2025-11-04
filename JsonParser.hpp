#ifndef JSONPARSER_HPP
#define JSONPARSER_HPP

#include <queue>
#include <fstream>
#include "Constants.hpp"

#include <iostream>

using namespace json;

enum Token
{
    OBJECT_START,
    OBJECT_END,
    ARRAY_START,
    ARRAY_END,
    STRING,
    NUMBER,
    TRUE,
    FALSE,
    NNNN,
    NAME
};

struct TokenValue
{
    Token Identifier;
    std::string Value;
};


class JsonParser
{
private:
    std::ifstream& file;
    std::queue<TokenValue> lex;

    void ParseJsonFromFile();
    void ReadObject();
    void ReadArray();
    void ReadValue();
    void ReadString();
    void ReadNumber();
    void ReadTrue();
    void ReadFalse();
    void ReadNull();
    void RemoveWhitespace();

    void ReadNameValuePair();
    char ReadDelimitedValue();

public:
    JsonParser(std::ifstream& file);
    ~JsonParser();
};



#endif