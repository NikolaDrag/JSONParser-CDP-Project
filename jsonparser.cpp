#include "JsonParser.h"

using std::cerr;

bool isDigit(char chr){
    if(chr >= '0' && chr <= '9'){
        return true;
    }else{
        return false;
    }
}

JsonValue JsonParser::getStoredValues(){
        return storedValues;
}

JsonParser::JsonParser(string &jsonToParse) : jsonInput(jsonToParse), position(0) {}

void JsonParser::parseAndStoreJsonValue() {
    position =0;
    JsonValue parsedValue = parse(); 
    cout << parsedValue.getTypeval();
    storedValues = parsedValue;
    return;
}

void JsonParser::printStoredJsonValues() const {
        storedValues.print();
}

JsonValue JsonParser::parse() {
     skipSpaces();
        if(isDigit(jsonInput[position]) || jsonInput[position] == '-'){
            return parseNumber();
        }
        switch (jsonInput[position]) {
        case '"':
            return parseString();
            break;
        case '{':
            return parseObject();
            break;
        case 't':
        case 'f':
            return parseBool();
            break;
        case '[':
            return parseArray();
            break;
        case 'n':
            return parseNull();
            break;
        case 0:
            cerr << "Input is an empty string.";
            return JsonValue("");
            break;
        default:
            cerr << "Invalid input: unexpected character '" << jsonInput[position] << "' at position " << position << endl;
            throw std::invalid_argument("Invalid input: unexpected character at position " + std::to_string(position) + ".");
            break;
        }
}

bool JsonParser::isWhitespace(char c) {
   return (c == ' ' || c == '\t' || c == '\n' || c == '\r'); //ensure we skip newlines, tabs and other nonessential charecters
}

void JsonParser::skipSpaces() {
    if(jsonInput[position] == 0){
            return;
        }
        while(isWhitespace(jsonInput[position])){ //we go to the next position that isnt a whitespace
            position++;
        }
        return;
}

bool JsonParser::checkIfCommaIsLegit() {
    skipSpaces();
        if(jsonInput[position] == ','){
            position++; //skip the ,
            skipSpaces();
            if(jsonInput[position] == '}' || jsonInput[position] == ']' || jsonInput[position] == 0 || jsonInput[position] == ',' ){ //if we have a comma and we end the input or have another comma after it
                throw std::invalid_argument("Invalid comma at position: " + std::to_string(position) + ".");
                return false;
            }
        }else{
            skipSpaces();
            if(jsonInput[position] != '}' && jsonInput[position] != ']' && jsonInput[position] != 0){
                throw std::invalid_argument("Invalid character at position: " + std::to_string(position) + ". Missing comma or other.");
                return false;
            }
        }
        return true;
}

JsonValue JsonParser::parseObject() {
    JsonObject jsonObj;
        position++; //za {
        skipSpaces();
        if(jsonInput[position] != '"' && jsonInput[position] != '}'){
            throw std::invalid_argument("Invalid character in object at position: " + std::to_string(position) + ".");
        }else if(jsonInput[position] == '}'){ //check for empty object {}
            position++;
            return jsonObj;
        }else{
            while(jsonInput[position] != '}'){
                if(jsonInput[position] == 0){
                    throw std::invalid_argument("Missing closing bracket for object at position: " + std::to_string(position) + ".");
                }
                skipSpaces(); //not needed the first time but it will exit instantly
                string key="";
                key = parseKey().getStringval(); //increases the position as well
                skipSpaces();
                if(jsonInput[position] == ':'){
                    position++; //za :
                }else{
                   throw std::invalid_argument("Invalid character in object at position: " + std::to_string(position) + ".");
                }
                skipSpaces();
                jsonObj[key] = parse(); //parse the obj to the key it can be number,obj,arr and so on, checks for comma as well
            }
            position++; // za }
        }
        checkIfCommaIsLegit();
        JsonValue jsonObjToReturn(jsonObj);
        return jsonObjToReturn;
}

JsonValue JsonParser::parseKey() {
     string k ="";
        position++; // minavame  purvoto "
        while(jsonInput[position] != '"'){
            if(jsonInput[position] == '\\'){
                position++;
                escapeCharCare();
            }
            k.push_back(jsonInput[position]);
            position++;
        }
        position++; //second "
        //za key nqmame , sled tova imame :
        return JsonValue(k);
}

JsonValue JsonParser::parseString() {
    string s ="";
        position++; // minavame  purvoto "
        while(jsonInput[position] != '"'){
            if(jsonInput[position] == '\\'){
                position++;
                escapeCharCare();
            }
            s.push_back(jsonInput[position]);
            position++;
        }
        position++; //second "
        checkIfCommaIsLegit(); //za , !!! ima zapetaq sled posleden element ili ima skoba sled zapetaq
        return JsonValue(s);
}

void JsonParser::escapeCharCare() { //c++ will notice only the manualy entered \ not the automatic ones that we put with R"
   if(jsonInput[position] == '"' || jsonInput[position] == '/' || jsonInput[position] == 'r' || jsonInput[position] == 'b' || jsonInput[position] == 'f' || jsonInput[position] == 'n' ||
        jsonInput[position] == 't' || jsonInput[position] == '\\'){
            position++;
            return;
        }
        if(jsonInput[position] == 'u'){
            position++;
            for(int i=0;i< 4;i++){
                if((jsonInput[position] >= '0' && jsonInput[position]<= '9') || (jsonInput[position] >= 'A' && jsonInput[position]<= 'F')){
                    position++;
                }else{
                    throw std::invalid_argument("Invalid hexadecimal digit at position: " + std::to_string(position) + ".");
                    return;
                }
            }
        }
        throw std::invalid_argument("Invalid character after escape symbol \\ at position: " + std::to_string(position) + ".");
        return;
}

JsonValue JsonParser::parseNull() {
     if (jsonInput.substr(position, 4) == "null") {
            position += 4; // move the position after "null"
        }else{
            throw std::invalid_argument("Invalid nullvalue at position: " + std::to_string(position) + ".");
        }
        checkIfCommaIsLegit();
        return JsonValue();
}

JsonValue JsonParser::parseNumber() {
     bool hadDot = false;
        string number = "";
        while (isDigit(jsonInput[position]) || jsonInput[position] == '.' || jsonInput[position] == '-' || jsonInput[position] == 'e' || 
        jsonInput[position] == '+' || jsonInput[position] == 'E'){
            number.push_back(jsonInput[position]);
            position++;
        }
        skipSpaces();
        if(jsonInput[position] != '}' && jsonInput[position] != ']' && jsonInput[position] != 0 && jsonInput[position] != ','){
            throw std::invalid_argument("Invalid digit/character at position: " + std::to_string(position) + ".");
        }
        checkIfCommaIsLegit();
        double numberVal = std::stod(number); //stod will throw exeption by itself
        return JsonValue(numberVal);
}

JsonValue JsonParser::parseBool() {
    if (jsonInput.substr(position, 4) == "true") {
            position += 4; // move the position after "true"
            checkIfCommaIsLegit();
            return JsonValue(true);
    } else if (jsonInput.substr(position, 5) == "false") {
            position += 5; // move the position after "false"
            checkIfCommaIsLegit();
            return JsonValue(false);
        } else {
            throw std::invalid_argument("Invalid boolean value at position " + std::to_string(position) + ".");
            return JsonValue();
        }
}

JsonValue JsonParser::parseArray() {
   position++; //for opening [
    JsonValue arr;
    arr.setTypeToParse(JsonValue::Type::ARRAY);
    while(jsonInput[position] != ']'){
        if(jsonInput[position] == 0){
            throw std::invalid_argument("Missing closing bracket for array at position: " + std::to_string(position) + ".");
        }
        arr.getArrayval().push_back(parse());
    }
    position++; //for closing ]
    checkIfCommaIsLegit(); 
    return arr;
}