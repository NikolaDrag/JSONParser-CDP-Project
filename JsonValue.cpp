#include "JsonValue.h"

JsonValue::JsonValue() : typeToParse(NULLVAL) {}

JsonValue::JsonValue(const string& inputStr) : typeToParse(STRING), stringVal(inputStr) {}

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

string JsonValue::getStringval() const {
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

void JsonValue::print(const string & identation) const{ //pass the command
    if(typeToParse == STRING){
        cout << stringVal << identation;
    }
    if(typeToParse == NUMBER){
        cout << std::to_string(numberVal) << identation;
    }
    if(typeToParse == BOOLEAN){
        cout << std::boolalpha << boolVal << identation;
    }
    if(typeToParse == NULLVAL){
        cout << "null" << identation;
    }
    if(typeToParse == OBJECT){
        cout << "{ " << identation;
        for(const auto& key : objectVal.keysOrder){
            cout << "Key: " <<  key << ", Value: ";
            objectVal.objectMap.at(key).print(identation);
        }
        cout << "} " << identation;
    }
    if(typeToParse == ARRAY){
        cout << "[" << identation;
        for (auto it = arrayVal.begin(); it != arrayVal.end(); ++it) {
            it->print(identation);
            if(it != arrayVal.end()-1){
                cout << "," << identation;
            }
        }
        cout << "]" << identation;
    }
    return;
}

void JsonValue::exactPrint(const string & identation) const{ //pass the command
    if(typeToParse == STRING){
        cout << "\"" << stringVal << "\"" << identation;
    }
    if(typeToParse == NUMBER){
        cout << std::to_string(numberVal) << identation;
    }
    if(typeToParse == BOOLEAN){
        cout << std::boolalpha << boolVal << identation;
    }
    if(typeToParse == NULLVAL){
        cout << "null" << identation;
    }
    if(typeToParse == OBJECT){
        cout << "{ " << identation;
        int counter =0;
        for(const auto& key : objectVal.keysOrder){
            cout << "\"" <<  key << "\"" << ":";
            objectVal.objectMap.at(key).exactPrint(identation);
            if (++counter < objectVal.keysOrder.size()) {
                cout << "," << identation;
            }
        }
        cout << "} " << identation;
    }
    if(typeToParse == ARRAY){
        cout << "[" << identation;
        for (auto it = arrayVal.begin(); it != arrayVal.end(); ++it) {
            it->exactPrint(identation);
            if(it != arrayVal.end()-1){
                cout << "," << identation;
            }
        }
        cout << "]" << identation;
    }
    return;
}

JsonValue JsonValue::findByKey(const string & keyValue)const{ //Shte izpolzvam rekursiven proces sus valueVector kato state variable
    JsonArray valueVector;
    findByKeyHelper(keyValue,valueVector);
    return JsonValue(valueVector);
}

void JsonValue::findByKeyHelper(const string & keyValue,JsonArray &currArrOfMatchingVal)const{ //we will call this on the storedKeyVal in JsonParser
    if(typeToParse == OBJECT){
        for(const auto& key : objectVal.keysOrder){
            if(key == keyValue){
                currArrOfMatchingVal.push_back(objectVal.objectMap.at(key));
            }
            if(objectVal.objectMap.at(key).getTypeval() == OBJECT || objectVal.objectMap.at(key).getTypeval() == ARRAY){ //noting to push if its not in an object
                objectVal.objectMap.at(key).findByKeyHelper(keyValue,currArrOfMatchingVal);
            }
        }
    }
    if(typeToParse == ARRAY){
        for (auto it = arrayVal.begin(); it != arrayVal.end(); ++it) {
            if(it->getTypeval() == OBJECT || it-> getTypeval() == ARRAY){ //nothing to look for if its not in an object or an array
            it->findByKeyHelper(keyValue, currArrOfMatchingVal);
            }
        }
    }
    return;
}

void JsonValue::saveToFile(string &fileName)const{
    std::ofstream outputFile;
    if (std::ifstream(fileName)) { // Check if the file already exists
        char choice1;
        cout << "File already exists. Do you want to (O)verwrite or (C)hange the name? "; //console interface
        cin >> choice1;
        if (choice1 == 'C' || choice1 == 'c') {
            cout << "Enter a new file name: ";
            std::getline(cin, fileName);
        }
    }
    outputFile.open(fileName);
    if (!outputFile.is_open()) {
        cerr << "Error opening file for writing: " << fileName << endl;
        return;
    }
    std::streambuf* coutBuffer = cout.rdbuf();   // Redirect cout to the file
    cout.rdbuf(outputFile.rdbuf());
    exactPrint("\n");
    cout.rdbuf(coutBuffer);  // Restore cout to the original buffer
    outputFile.close();
}

void JsonValue::deleteElementOnPath(const vector<string>& path, int trackPath){ //we pass the stored jsonvalue, trackPath starts at 0
    if (path.empty()) {
        cerr << "Invalid empty path." << endl;
        return;
    }
    if(typeToParse == OBJECT){
        for(auto it = objectVal.keysOrder.begin(); it != objectVal.keysOrder.end();it++){
            if(*it == path[trackPath] && trackPath+1 == path.size()){ //if we are at the desired key
                objectVal.objectMap.erase(*it);
                objectVal.keysOrder.erase(it); 
                return;
            }
            if(*it == path[trackPath]){
                return objectVal.objectMap.at(*it).deleteElementOnPath(path,trackPath+1); //we continue on the right path
            }
        }
    }
    if(typeToParse == ARRAY){
        int currentIndex =0;
        for (auto it = arrayVal.begin(); it != arrayVal.end(); ++it,currentIndex++) {
            if(currentIndex == std::stod(path[trackPath]) && trackPath+1 == path.size()){ 
                arrayVal.erase(it);
                return;
            }
            if(currentIndex == std::stod(path[trackPath])){ 
                return it->deleteElementOnPath(path,trackPath+1);
            }
        }
    }
    cerr << "Invalid path." << endl;
    return;
} //!!! split this function into serach and delete, where search returns the element to be deleted