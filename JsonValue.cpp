#include "JsonValue.h"

JsonValue::JsonValue() : typeToParse(NULLVAL) {}

JsonValue::JsonValue(const std::string& inputStr) : typeToParse(STRING), stringVal(inputStr) {}

JsonValue::JsonValue(const double& inputNumber) : typeToParse(NUMBER), numberVal(inputNumber) {}

JsonValue::JsonValue(const bool& inputBool) : typeToParse(BOOLEAN), boolVal(inputBool) {}

JsonValue::JsonValue(const JsonObject& inputObj) : typeToParse(OBJECT), objectVal(inputObj) {}

JsonValue::JsonValue(const JsonArray& inputArr) : typeToParse(ARRAY), arrayVal(inputArr) {}

JsonValue& JsonValue::operator=(const JsonValue& other) {
    if (this != &other) {
        typeToParse = other.typeToParse;
        stringVal = other.stringVal;
        numberVal = other.numberVal;
        boolVal = other.boolVal;
        objectVal = other.objectVal;
        arrayVal = other.arrayVal;
    }
    return *this;
}

void JsonValue::setTypeToParse(Type typeVal) {
    typeToParse = typeVal;
}

JsonValue::Type JsonValue::getTypeval() const {
    return typeToParse;
}

std::string JsonValue::getStringval() const {
    return stringVal;
}

double JsonValue::getNumberval() const {
    return numberVal;
}

bool JsonValue::getBoolval() const {
    return boolVal;
}

JsonObject JsonValue::getObjectval() const {
    return objectVal;
}

JsonArray JsonValue::getArrayval() const {
    return arrayVal;
}

void JsonValue::print() const{ //pass the command
    if(typeToParse == STRING){
        cout << stringVal << endl;
    }
    if(typeToParse == NUMBER){
        cout << std::to_string(numberVal) << endl;
    }
    if(typeToParse == BOOLEAN){
        cout << std::boolalpha << boolVal << endl;
    }
    if(typeToParse == NULLVAL){
        cout << "null" << endl;
    }
    if(typeToParse == OBJECT){
        for (auto it = objectVal.begin(); it != objectVal.end(); ++it) {
        cout << "Key: " << it->first << ", Value: ";
        it->second.print();
        cout << endl;
        }
    }
    if(typeToParse == ARRAY){
        for (auto it = arrayVal.begin(); it != arrayVal.end(); ++it) {
        it->print();
        cout << endl;
        }
    }
    return;
}
