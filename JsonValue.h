#ifndef JSONVALUE_H
#define JSONVALUE_H

#include <iostream>
#include <map>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;

class JsonValue;

typedef map<string, JsonValue> JsonObject; //if vector has 1 Element it is string,number,boolean,null, otherwise its an array
typedef vector<JsonValue> JsonArray;

class JsonValue { //izpolzva se za jsonObject i JsonArray
public:
    enum Type {
        NULLVAL,
        STRING,
        NUMBER,
        BOOLEAN,
        OBJECT,
        ARRAY
    };

    JsonValue();
    JsonValue(const string& inputStr);
    JsonValue(const double& inputNumber);
    JsonValue(const bool& inputBool);
    JsonValue(const JsonObject& inputObj);
    JsonValue(const JsonArray& inputArr);

    JsonValue& operator=(const JsonValue& other);

    void setJsonObject(const JsonObject& newObj);
    void setTypeToParse(Type typeVal);
    Type getTypeval() const;
    string getStringval() const;
    double getNumberval() const;
    bool getBoolval() const;
    JsonObject getObjectval() const;
    JsonArray getArrayval() const;

    void print() const;
private:
    Type typeToParse;
    string stringVal;
    double numberVal;
    bool boolVal;
    JsonObject objectVal;
    JsonArray arrayVal;
};

#endif 