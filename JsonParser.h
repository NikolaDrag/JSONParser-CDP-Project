#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "JsonValue.h"
#include <fstream>

using std::cin;

class JsonParser {
private:
    string jsonInput;
    unsigned int position;
    JsonValue storedValues;

public:
    bool isDigit(char chr);

    JsonParser(string &jsonToParse);

    JsonValue getStoredValues();

    void parseAndStoreJsonValue();
    void printStoredJsonValues() const;
    void shortPrintStoredJsonValues() const;
    void exactPrintStoredJsonValues(const string & identation) const; //print according to jsonrules needed in file, just a different print style method

    void saveToJsonFile(const JsonValue& jsonData, string& fileName);

    JsonValue parse();
private:
    bool isWhitespace(char c);
    void skipSpaces();
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