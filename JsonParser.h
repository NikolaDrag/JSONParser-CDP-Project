#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "JsonValue.h"

class JsonParser {
private:
    std::string jsonInput;
    unsigned int position;

public:
    JsonParser(std::string &jsonToParse);

    JsonValue parse();

private:
    bool isWhitespace(char c);
    void skipSpaces();
    bool checkIfCommaIsLegit();
    JsonValue parseObject();
    JsonValue parseKey();
    JsonValue parseString();
    void escapeCharCare();
    JsonValue parseNull();
    JsonValue parseNumber();
    JsonValue parseBool();
    JsonValue parseArray();
};

#endif 