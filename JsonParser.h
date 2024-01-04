#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "JsonValue.h"

class JsonParser {
private:
    string jsonInput;
    unsigned int position;
    JsonValue storedValues;

public:
    JsonParser(string &jsonToParse);

    JsonValue parse();

    JsonValue getStoredValues();
    void parseAndStoreJsonValue();

    void printStoredJsonValues() const;
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