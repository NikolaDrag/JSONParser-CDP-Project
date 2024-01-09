#include "JsonParser.h"


bool JsonParser::isDigit(char chr){
    if(chr >= '0' && chr <= '9'){
        return true;
    }else{
        return false;
    }
}

JsonParser::JsonParser(string &jsonToParse) : jsonInput(jsonToParse), position(0) {}

JsonValue& JsonParser::getStoredValues(){ //need a & to change and delete we have const on other methods on JsonValue so it's fine 
    return storedValues;
}

void JsonParser::parseAndStoreJsonValue() { //this parse should be used, so the normal parse() is like a helper
    position =0;
    JsonValue parsedValue; //for some readability
    if(jsonInput[0] == 0){
        throw std::invalid_argument("Invalid input: input is an empty string. ");
        return;
    }
    while(jsonInput[position] != 0){ // avoid extra text
        parsedValue = parse();
    }
    storedValues = parsedValue; 
    return;
}

void JsonParser::printStoredJsonValues() const {
    string indent = "\n";
    storedValues.exactPrint(indent);
    return;
}

void JsonParser::shortPrintStoredJsonValues() const {
    storedValues.print("");
    return;
}

void JsonParser::exactPrintStoredJsonValues(string & indentation)const{
    storedValues.exactPrint(indentation);
    return;
}

void JsonParser::saveToJsonFile(string& fileName) {
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
    char choice2;
    cout << "Enter desired indentation: (L)ong and readable or (S)hort: "; //console interface
    cin >> choice2;
    cin.ignore(); // Clear the newline character from the buffer
    std::streambuf* coutBuffer = cout.rdbuf();   // Redirect cout to the file
    cout.rdbuf(outputFile.rdbuf());
    string indent;
    if(choice2 == 'L' || choice2 == 'l'){
        indent = "\n";
        exactPrintStoredJsonValues(indent);
    }else{ //if its 'S' or 's'
        indent = "";
        exactPrintStoredJsonValues(indent);
    }
    cout.rdbuf(coutBuffer);  // Restore cout to the original buffer
    outputFile.close();
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
            throw std::invalid_argument("Invalid input: input is an empty string.");
            return JsonValue("");
            break;
        default:
            throw std::invalid_argument("Invalid input: unexpected character at position " + std::to_string(position) + ".");
            return JsonValue("");
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

JsonValue JsonParser::parseObject() { //if method is too long or perceived as unreadable we can quickly extract methods for the errors;
    position++; //za {
    skipSpaces(); //eventualni spaces mejdu { i key
    JsonObject obj;
    if(jsonInput[position] == '}'){ //ako imame {}
        position++;
        return JsonValue(obj);
    }
    while(jsonInput[position] != '}'){
        if(jsonInput[position] == 0){ //if we dont get a closing bracket
            throw std::invalid_argument("Invalid character in object at position: " + std::to_string(position) + ". Missing closing } bracket.");
            return JsonValue(obj);
        }
        if(jsonInput[position] != '\"'){ //ako nqmame kavichki za kluch, ako nqmame string
            throw std::invalid_argument("Invalid character in object at position: " + std::to_string(position) + ". Missing key or invalid input.");
            return JsonValue(obj);
        }
        string key = parseKey();
        if (obj.objectMap.find(key) != obj.objectMap.end()) {
            throw std::invalid_argument("Key already exists in object at position: " + std::to_string(position) + "."); //check if the key already exists
            return JsonValue(obj);
        }
        obj.keysOrder.push_back(key); //pushing key in order for the object
        skipSpaces(); //skip spaces mejdu kluch i :
        if(jsonInput[position] != ':'){
            throw std::invalid_argument("Invalid character in object at position: " + std::to_string(position) + ". Missing : after key.");
            return JsonValue(obj);
        }
        position++; //ako imame : go minavame
        obj.objectMap[key] = parse(); //connect key to next object, parse() will skip begining spaces
        skipSpaces(); // value e -> space value space, pruviq space go skipvame s parse() vtoriq tuk, toest ne se skipvat vuv valuetata kato string etc.
        if(jsonInput[position] == '}'){ // proverqvame dali prikluchva obekta za da napravim proverka za zapetaq
            position++;
            break;
        }
        if(jsonInput[position] != ','){ //ako nqma zapetaq no i ne priluchva obekta, ako ima zapetaq cikula 
        //shte produlji ako sled zapetaqta nqma kavichki za kluch shte vlezem v gorniq error
            throw std::invalid_argument("Invalid character in object at position: " + std::to_string(position) + ". Missing comma or invalid symbol.");
            return JsonValue(obj);
        }
        position++; //za zapetaq
        skipSpaces(); //sled zapetaq skipvame spaces za da zapochnem noviq kluch
    }
    skipSpaces(); //for extra text
    return JsonValue(obj);
}

string JsonParser::parseKey() {
     string k ="";
        position++; // minavame  purvoto "
        while(jsonInput[position] != '"'){
            if(jsonInput[position] == 0){
                throw std::invalid_argument("Invalid character in key. Missing \" closing quotes.");
                return k;
            }
            if(jsonInput[position] == '\\'){
                k.push_back(jsonInput[position]);
                position++;
                escapeCharCare(k);
            }
            k.push_back(jsonInput[position]);
            position++;
        }
        position++; //second "
        //za key nqmame , sled tova imame :
        return k;
}

JsonValue JsonParser::parseString() {
    string s ="";
        position++; // minavame  purvoto "
        while(jsonInput[position] != '"'){
            if(jsonInput[position] == 0){
                throw std::invalid_argument("Invalid character in string. Missing \" closing quotes.");
            }
            if(jsonInput[position] == '\\'){
                s.push_back(jsonInput[position]); //push the escape char
                position++; //skip the escape char 
                escapeCharCare(s); //push and skip the hexadec or char after the escape char
            }
            s.push_back(jsonInput[position]); //push letter
            position++;
        }
        position++; //second ", nqma da obgirjvame ako samo edin string e validen json za sega!!!
        return JsonValue(s);
}

void JsonParser::escapeCharCare(string & curString) { //c++ will notice only the manualy entered \ not the automatic ones that we put with R"
    if(jsonInput[position] == '"' || jsonInput[position] == '/' || jsonInput[position] == 'r' || jsonInput[position] == 'b' || jsonInput[position] == 'f' || jsonInput[position] == 'n' ||
        jsonInput[position] == 't' || jsonInput[position] == '\\'){ //ako e escape char
            curString.push_back(jsonInput[position]);
            position++;
            return;
        }
    if(jsonInput[position] == 'u'){ //ako e hexadecimal
        curString.push_back(jsonInput[position]);
        position++;
        for(int i=0;i< 4;i++){
            if((jsonInput[position] >= '0' && jsonInput[position]<= '9') || (jsonInput[position] >= 'A' && jsonInput[position]<= 'F')){
                curString.push_back(jsonInput[position]);
                position++;
            }else{ //ako nqkoe chislo ne e chast ot hexadecimal
                throw std::invalid_argument("Invalid hexadecimal digit at position: " + std::to_string(position) + ".");
                return;
            }
        }
    }
    throw std::invalid_argument("Invalid character after escape symbol \\ at position: " + std::to_string(position) + ".");  //ako ne e validen char sled escape char
    return;
}

JsonValue JsonParser::parseNull() {
    if (jsonInput.substr(position, 4) == "null") {
        position += 4; // move the position after "null", toest na purviq char sled null
    }else{
        throw std::invalid_argument("Invalid nullvalue at position: " + std::to_string(position) + ".");
    } //nqma da obgirjvame ako samo edin string e validen json za sega!!!
    return JsonValue();
}

JsonValue JsonParser::parseNumber() {
    string number = "";
    while (isDigit(jsonInput[position]) || jsonInput[position] == '.' || jsonInput[position] == '-' || jsonInput[position] == 'e' || 
    jsonInput[position] == '+' || jsonInput[position] == 'E'){
        number.push_back(jsonInput[position]);
        position++;
    }
    //nqma da obgirjvame ako samo edin string e validen json za sega!!!
    double numberVal = std::stod(number); //stod will throw exeption by itself
    return JsonValue(numberVal);
}

JsonValue JsonParser::parseBool() {
    if (jsonInput.substr(position, 4) == "true") {
            position += 4; // move the position after "true"
            return JsonValue(true);
    } else if (jsonInput.substr(position, 5) == "false") {
            position += 5; // move the position after "false"
            return JsonValue(false);
    } else {
        throw std::invalid_argument("Invalid boolean value at position " + std::to_string(position) + ".");
        return JsonValue();
    }
}

JsonValue JsonParser::parseArray() {
    JsonArray arr;
    position++; // Move past the opening bracket '['
    skipSpaces(); //skip if we have [ space ], pri purvoto minavane dublira s parse() no tova nqma znachenie za skipSpaces()
    if(jsonInput[position] == ']'){
        position++; //skip the ]
        return JsonValue(arr);
    }
    while(jsonInput[position] != ']'){
        if(jsonInput[position] == 0){
            throw std::invalid_argument("Invalid character in array at position: " + std::to_string(position) + ". Missing closing ] bracket.");
            return JsonValue(arr);
        }
        arr.push_back(parse());
        skipSpaces();// value e -> space value space, pruviqt space go skipvame s parse() vtoriq tuk, toest ne se skipvat vuv valuetata kato string etc.
        if(jsonInput[position] == ']'){
            position++;
            break;
        }
        if(jsonInput[position] != ','){ //ako nqma zapetaq no i ne prikluchva obekta, ako ima zapetaq cikula 
        //shte produlji ako sled zapetaqta nqma kavichki za kluch shte vlezem v gorniq error
            throw std::invalid_argument("Invalid character in array at position: " + std::to_string(position) + ". Missing comma or invalid symbol.");
            return JsonValue(arr);
        }
        position++; //skip ,
    }
    skipSpaces(); //for extra text
    return JsonValue(arr);
}

