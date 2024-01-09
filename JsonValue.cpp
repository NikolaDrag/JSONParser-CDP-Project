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

void JsonValue::print(const string & indentation) const{ //pass the command
    if(typeToParse == STRING){
        cout << stringVal;
    }
    if(typeToParse == NUMBER){
        cout << std::to_string(numberVal);
    }
    if(typeToParse == BOOLEAN){
        cout << std::boolalpha << boolVal;
    }
    if(typeToParse == NULLVAL){
        cout << "null";
    }
    if(typeToParse == OBJECT){
        cout << "{ " << indentation;
        for(const auto& key : objectVal.keysOrder){
            cout << "Key: " <<  key << ", Value: ";
            objectVal.objectMap.at(key).print(indentation);
        }
        cout << "} " << indentation;
    }
    if(typeToParse == ARRAY){
        cout << "[" << indentation;
        for (auto it = arrayVal.begin(); it != arrayVal.end(); ++it) {
            it->print(indentation);
            if(it != arrayVal.end()-1){
                cout << "," << indentation;
            }
        }
        cout << "]" << indentation;
    }
    return;
}

void JsonValue::exactPrint(string & indentation) const{ //this will be the good print when we pass "\n"
    if(typeToParse == STRING){
        cout << "\"" << stringVal << "\"" ;
    }
    if(typeToParse == NUMBER){
        cout << std::to_string(numberVal) ;
    }
    if(typeToParse == BOOLEAN){
        cout << std::boolalpha << boolVal ;
    }
    if(typeToParse == NULLVAL){
        cout << "null" ;
    }
    if(typeToParse == OBJECT){
        cout << "{ ";
        indentation = indentation + " ";
        cout<< indentation;
        int counter =0;
        for(const auto& key : objectVal.keysOrder){
            cout << "\"" <<  key << "\"" << ":";
            objectVal.objectMap.at(key).exactPrint(indentation);
            if (++counter < objectVal.keysOrder.size()) {
                cout << "," << indentation;
            }
        }
        indentation.pop_back();
        cout << indentation << "} ";
    }
    if(typeToParse == ARRAY){
        cout << "[";
        indentation.push_back('\t');
        cout<< indentation;
        for (auto it = arrayVal.begin(); it != arrayVal.end(); ++it) {
            it->exactPrint(indentation);
            if(it != arrayVal.end()-1){
                cout << "," << indentation;
            }
        }
        indentation.pop_back();
        cout << indentation <<"]" ;
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
        cin.ignore();
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
    string indent = "\n";
    exactPrint(indent);
    cout.rdbuf(coutBuffer);  // Restore cout to the original buffer
    outputFile.close();
}

void JsonValue::findElementByPath(const vector<string> & path, int trackPath,const string &command,JsonValue &newValue,string &fileName){ //we parse it and then we asign if parse is correct
    if (path.empty()) {
        throw std::invalid_argument("Invalid empty path.");
        return;
    }
    if(typeToParse == OBJECT){
        for(auto it = objectVal.keysOrder.begin(); it != objectVal.keysOrder.end();it++){
            if(*it == path[trackPath] && trackPath+1 == path.size()){ //if we are at the desired key
                if(command == "delete"){
                    objectVal.objectMap.erase(*it);
                    objectVal.keysOrder.erase(it); 
                    return;
                }
                if(command == "change"){
                    objectVal.objectMap[*it] = newValue;
                    return;
                }
                if(command == "save"){
                    objectVal.objectMap[*it].saveToFile(fileName);
                    return;
                }
            }
            if(*it == path[trackPath]){
                return objectVal.objectMap.at(*it).findElementByPath(path,trackPath+1,command,newValue,fileName);
            }
        }
    }
    if(typeToParse == ARRAY){
        int currentIndex =0;
        for (auto it = arrayVal.begin(); it != arrayVal.end(); ++it,currentIndex++) {
            if(currentIndex == std::stod(path[trackPath]) && trackPath+1 == path.size()){ 
                if(command == "delete"){
                    arrayVal.erase(it);
                    return;
                }
                if(command == "change"){
                    *it = newValue;
                    return;
                }
                if(command == "save"){
                    (*it).saveToFile(fileName);
                    return;
                }
            }
            if(currentIndex == std::stod(path[trackPath])){ 
                return it->findElementByPath(path,trackPath+1,command,newValue,fileName);
            }
        }
    }
    throw std::invalid_argument("Invalid path.");
    return;
}

void JsonValue::createAnElement(const vector<string> & path, int trackPath,const string &newKey,JsonValue &newValue){
    if (path.empty()) {
        throw std::invalid_argument("Invalid empty path.");
        return;
    }
    if(typeToParse == OBJECT){
        for(auto it = objectVal.keysOrder.begin(); it != objectVal.keysOrder.end();it++){
            if(*it == path[trackPath] && trackPath+1 == path.size()){
                objectVal.objectMap[newKey] = newValue;
                objectVal.keysOrder.push_back(newKey); 
                return;
            }
            if(*it == path[trackPath]){
                return objectVal.objectMap.at(*it).createAnElement(path,trackPath+1,newKey,newValue);
            }
        }
    }
    if(typeToParse == ARRAY){
        int currentIndex =0;
        for (auto it = arrayVal.begin(); it != arrayVal.end(); ++it,currentIndex++) {
            if(currentIndex == std::stod(path[trackPath]) && trackPath+1 == path.size()){
                arrayVal.insert(it,newValue); //insert new value in array at specified position
                return;
            }
            if(currentIndex == std::stod(path[trackPath])){ 
                return it->createAnElement(path,trackPath+1,newKey,newValue);
            }
        }
    }
    throw std::invalid_argument("Invalid path.");
    return;
}