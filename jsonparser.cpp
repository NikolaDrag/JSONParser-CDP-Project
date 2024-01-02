#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <stack>

using std::string;
using std::vector;
using std::map;
using std::stack;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

class JsonValue;

typedef map<string, JsonValue> JsonObject; //is vector has 1 Element it is string,number,boolean,null, otherwise its an array
typedef vector<JsonValue> JsonArray;

class JsonValue { //izpolzva se za jsonObject i JsonArray
public:
    JsonValue(){
        typeToParse = NULLVAL;
    }
    JsonValue(const string &inputStr) : stringVal(inputStr){
        typeToParse = STRING;
    }
    JsonValue(const double &inputNumber) : numberVal(inputNumber){
        typeToParse = NUMBER;
    }
    JsonValue(const bool &inputBool) : boolVal(inputBool){
        typeToParse = BOOLEAN;
    }
    JsonValue(const JsonObject& inputObj) : objectVal(inputObj){
        typeToParse = OBJECT;
    }
    JsonValue(const JsonArray & inputArr) : arrayVal(inputArr){
        typeToParse = ARRAY;
    }
    JsonValue& operator=(const JsonValue& other) {
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
//private: later getters
    enum Type {
        NULLVAL,
        STRING,
        NUMBER,
        BOOLEAN,
        OBJECT,
        ARRAY
    };
    Type typeToParse;
    string stringVal;
    double numberVal;
    bool boolVal;
    JsonObject objectVal;
    JsonArray arrayVal;
};

bool isDigit(char chr){
    if(chr >= '0' && chr <= '9'){
        return true;
    }else{
        return false;
    }
}
class JsonParser{

private:
    string jsonInput;
    unsigned int position;
public:
    JsonParser(string &jsonToParse):jsonInput(jsonToParse),position(0){}

    JsonValue parse(){
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
        /*case '[':
            parseArray();
            break;*/
        case 'n':
            return parseNull();
            break;
        case 0:
            cerr << "Input is an empty string.";
            return JsonValue("");
            break;
        default:
            //cerr << "Invalid input: unexpected character '" << jsonInput[position] << "' at position " << position << endl;
            throw std::invalid_argument("Invalid input: unexpected character at position " + std::to_string(position) + ".");
            break;
        }
    }

private:
    bool isWhitespace(char c) {
        return (c == ' ' || c == '\t' || c == '\n' || c == '\r'); //ensure we skip newlines, tabs and other nonessential charecters
    }

    void skipSpaces(){
        if(jsonInput[position] == 0){
            return;
        }
        while(isWhitespace(jsonInput[position])){ //we go to the next position that isnt a whitespace
            position++;
        }
        return;
    }

    bool checkIfCommaIsLegit(){
        skipSpaces();
        if(jsonInput[position] == ','){
            position++; //skip the ,
            skipSpaces();
            if(jsonInput[position] == '}' || jsonInput[position] == ']' || jsonInput[position] == 0){ //if we have a comma and we end the input
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

    JsonValue parseObject(){
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
                skipSpaces(); //not needed the first time but it will exit instantly
                string key="";
                key = parseKey().stringVal; //increases the position as well
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
        return jsonObj;
    }

    JsonValue parseKey(){
        string k ="";
        position++; // minavame  purvoto "
        while(jsonInput[position] != '"'){
            k.push_back(jsonInput[position]);
            position++;
        }
        position++; //second "
        //za key nqmame , sled tova imame :
        return JsonValue(k);
    }

    JsonValue parseString(){
        string s ="";
        position++; // minavame  purvoto "
        while(jsonInput[position] != '"'){
            s.push_back(jsonInput[position]);
            position++;
        }
        position++; //second "
        checkIfCommaIsLegit(); //za , !!! ima zapetaq sled posleden element ili ima skoba sled zapetaq
        return JsonValue(s);
    }

    JsonValue parseNull(){
        if (jsonInput.substr(position, 4) == "null") {
            position += 4; // move the position after "null"
        }
        checkIfCommaIsLegit();
        return JsonValue();
    }

    JsonValue parseNumber(){
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

    JsonValue parseBool() {
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
};

int main(){
    //1vi char sled ( e 'space', toest na position[0]
    string jsonInput = R"( {"name": "John Doe",
    "age": -30.90,
    "languages": "English"} )";
    JsonParser JsonParser1(jsonInput);
    JsonParser1.parse();
    //cout << jsonInput[43];
    /*
    Notes: Works - checkcomma,skipspaces, parseString,Bool,Key,Null
    ParseObject needs to work for other types apart from strings, after we parse he key
    */
    return 0;
}