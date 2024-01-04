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
private:
    bool isWhitespace(char c);
    void skipSpaces();
    //bool checkIfCommaIsLegit();
    JsonValue parseObject();
    string parseKey();
    JsonValue parseString();
    void escapeCharCare(string & curString);
    JsonValue parseNull();
    JsonValue parseNumber();
    JsonValue parseBool();
    JsonValue parseArray();
    
};

#endif 