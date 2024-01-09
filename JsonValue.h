#ifndef JSONVALUE_H
#define JSONVALUE_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::cin;
using std::cerr;

class JsonValue;

struct JsonObject{
    map<string, JsonValue> objectMap;
    vector<string> keysOrder;
};

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

    void setTypeToParse(Type typeVal);

    Type getTypeval() const;
    string getStringval() const;
    double getNumberval() const;
    bool getBoolval() const;
    JsonObject getObjectval() const;
    JsonArray getArrayval() const;

    void print(const string & identation) const; //use for key matches or for short print if we pass "" identation
    void exactPrint( string & identation) const; //print according to json and print readable

    JsonValue findByKey(const string &keyValue) const;
    void findByKeyHelper(const string & keyValue, JsonArray &currentArr)const;

    void saveToFile(string &fileName)const;

    void findElementByPath(const vector<string> & path, int trackPath,const string &command,JsonValue &newValue,string &fileName); //depending on command we use it for delete, change, etc.
    void createAnElement(const vector<string> & path, int trackPath,const string &newKey,JsonValue &newValue);
private:
    Type typeToParse;
    string stringVal;
    double numberVal;
    bool boolVal;
    JsonObject objectVal;
    JsonArray arrayVal;
};

#endif 