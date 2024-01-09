#include "JsonValue.h"
#include "JsonParser.h"
#include <sstream>

void exitProgram(){
    cout << "Exiting the program." << endl;
    std::exit(EXIT_SUCCESS);
}

void saveToFileCommand(JsonParser &parser1){
    cout << "Please enter your desired filename: ";
    string fileName;
    std::getline(cin, fileName);
    parser1.saveToJsonFile(fileName);
    return;
}

void findCommand(JsonParser &parser1){ //add for invalid command maybe, save by key in file is practically integrated here
    cout << "Enter key that you want to find bindings for: ";
    string key;
    std::getline(cin,key);
    if(parser1.getStoredValues().findByKey(key).getArrayval().empty() == true){
        cerr << "No matching value for the key." << endl;
        return;
    }
    char choice1;
    cout << "Select the desired action: (P)rint, (W)rite in file. (E)xit: ";
    cin >> choice1;
    cin.ignore();
    if(choice1 == 'e' || choice1 == 'E'){
        return;
    }
    char choice2;
    cout << "Select if you wish the use the whole array or a element at index: (A)rray, (I)ndex.";
    cin >> choice2;
    cin.ignore();
    int index;
    if(choice2 == 'I' || choice2 == 'i'){
        cout << "Enter your desired index (starting at 0): ";
        cin >> index;
        cin.ignore();
    }
    if(choice1 == 'P' || choice1 == 'p'){
        if(choice2 == 'A' || choice2 == 'a'){
            parser1.getStoredValues().findByKey(key).print("\n");
            cout << endl;
        }
        if(choice2 == 'I' || choice2 == 'i'){
            parser1.getStoredValues().findByKey(key).getArrayval()[index].print("\n");
        }
        return;
    }
    if(choice1 == 'W' || choice1 == 'w'){
        cout << "Please enter your desired filename: ";
        string fileName;
        std::getline(cin, fileName);
        if(choice2 == 'A' || choice2 == 'a'){
            parser1.getStoredValues().findByKey(key).saveToFile(fileName);
            cout << endl;
        }
        if(choice2 == 'I' || choice2 == 'i'){
            parser1.getStoredValues().findByKey(key).getArrayval()[index].saveToFile(fileName);
        }
    }
    return;
}

void fillPath(vector<string>& path){
    string curString = "-";
    std::getline(cin, curString);
    std::istringstream iss(curString); // Split the line into individual strings based on spaces
    while (iss >> curString) {
        path.push_back(curString);
    }
    return;
}

void deleteCommand(JsonParser &parser1){
    vector<string> path;
    cout << "Enter path (with spaces in between keys, indecies): ";
    fillPath(path);
    try{
        string plh ="";
        parser1.getStoredValues().findElementByPath(path,0,"delete",parser1.getStoredValues(),plh); //4th  adn 5thargument is a placeholder here
    }
    catch(const std::exception& ex){
        if(ex.what() == "stod"){
                cerr << "Not a number in array in path. " << endl;
            }else{
                cerr << ex.what() << endl;
        }
    }
    return;
}

JsonParser enterParser(){
    cout << "Enter the JSON string (press enter twice to submit):\n"; //we dont have a submit button
    string jsonInput1 ="";
    string line;
    while (std::getline(cin, line) && !line.empty()) {//std::getline(cin, jsonInput1);
        jsonInput1 += line + "\n";
    }
    JsonParser JsonParser1(jsonInput1);
    bool validInput = false;
    while (!validInput) {
        try{
            JsonParser1.parseAndStoreJsonValue();
            validInput = true;
        }
        catch(const std::exception& ex){
            jsonInput1 ="";
            if(ex.what() == "stod"){
                cerr << "Not a number, wrong number value in input. " << endl;
            }else{
                cerr << ex.what() << endl;
            }
            cout << "Enter a valid JSON string (type 'exit' to quit) (press enter twice to submit) : \n";
            while (std::getline(cin, line) && !line.empty()) {//std::getline(cin, jsonInput1);
                jsonInput1 += line + "\n";
            }
            if (jsonInput1 == "exit\n" || jsonInput1 == "Exit\n") {
                exitProgram();
            }
            JsonParser1 = JsonParser(jsonInput1);
        }
    }
    return JsonParser1;
}

void changeCommand(JsonParser &parser1){
    vector<string> path;
    cout << "Enter path (with spaces in between keys, indecies): ";
    fillPath(path);
    JsonParser parserToInsert = enterParser();
    try{
        string plh ="";
        parser1.getStoredValues().findElementByPath(path,0,"change",parserToInsert.getStoredValues(),plh); //5th argument is a placeholder
    }
    catch(const std::exception& ex){
        if(ex.what() == "stod"){
                cerr << "Not a number in array in path. " << endl;
            }else{
                cerr << ex.what() << endl;
        }
    }
    return;
}

void savePathCommand(JsonParser &parser1){
    cout << "Enter desired filename: ";
    string fileName;
    std::getline(cin, fileName);
    vector<string> path;
    cout << "Enter path (with spaces in between keys, indecies): ";
    fillPath(path);
    try{
        string plh ="";
        parser1.getStoredValues().findElementByPath(path,0,"save",parser1.getStoredValues(),fileName);
    }
    catch(const std::exception& ex){
        if(ex.what() == "stod"){
                cerr << "Not a number in array in path. " << endl;
            }else{
                cerr << ex.what() << endl;
        }
    }
    return;
}

void insertCommand(JsonParser &parser1){
    vector<string> path;
    cout << "Enter path (with spaces in between keys, indecies): ";
    fillPath(path);
    JsonParser parserToInsert = enterParser();
    try{
        cout << "Enter a key if you want to insert in a JSON Object.";
        string key ="";
        std::getline(cin,key);
        parser1.getStoredValues().createAnElement(path,0,key,parserToInsert.getStoredValues());
    }
    catch(const std::exception& ex){
        if(ex.what() == "stod"){
                cerr << "Not a number in array in path. " << endl;
            }else{
                cerr << ex.what() << endl;
        }
    }
    return;
}

int main(){
    //for console interface - map of function pointers if too many commands, for now we can do swtich/if,elseif...
    JsonParser JsonParser1 = enterParser();
    while (true) {
        cout << "Enter a command (type 'exit' to quit): ";
        string command;
        std::getline(cin, command);
        if(command == "Print" || command == "print"){
            JsonParser1.printStoredJsonValues();
        }
        else if(command == "Save to file" || command == "save to file"){
            saveToFileCommand(JsonParser1);
        }
        else if(command == "find" || command == "Find"){
            findCommand(JsonParser1);
        }
        else if(command == "delete on path" || command == "Delete on path" || command == "delete" || command == "Delete"){
            deleteCommand(JsonParser1);
        }
        else if(command == "change element" || command == "Change element" || command == "Change" || command == "change"){
            changeCommand(JsonParser1);
        }
        else if(command == "save on path" ||command == "Save on path" ||command == "save path" ||command == "Save path"){
            savePathCommand(JsonParser1);
        }
        else if(command == "insert" || command == "Insert"){
            insertCommand(JsonParser1);
        }
        else if (command == "exit" || command == "Exit") {
            exitProgram();
        }
        else{
            cout << "Invalid command please try again. " << endl;
        }
    }
    return 0;
}