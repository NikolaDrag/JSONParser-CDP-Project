#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

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

class JsonObject { //can be a struct but we will keep it a class if we need to add any extra functionality later on
public:
    map<string, JsonValue> objectMap;

    vector<string> keysOrder;// Vector to track keys separately

    //void addElement(const string& key, const JsonValue& value);
    //void print() const;

private:
};

#endif