#include "JsonValue.h"
#include "JsonParser.h"
#include <sstream>

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
        parser1.getStoredValues().deleteElementOnPath(path,0);
    }
    catch(const std::exception& ex){
        cerr << "Invalid input. Not a number entered in an array in path." << endl;
    }
    return;
}

int main(){
    //for console interface - map of function pointers if too many commands, for now we can do swtich/if,elseif...
    cout << "Enter the JSON string (press enter twice to submit):\n"; //we dont have a submit button
    string jsonInput1;
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
            if(ex.what() == "stod"){
                cerr << "Not a number, wrong number value in input. " << endl;
            }else{
                cerr << ex.what() << endl;
            }
            cout << "Enter a valid JSON string (press enter twice to submit) (type 'exit' to quit) : \n";
            while (std::getline(cin, line) && !line.empty()) {//std::getline(cin, jsonInput1);
                jsonInput1 += line + "\n";
            }
            if (jsonInput1 == "exit" || jsonInput1 == "Exit") {
                cout << "Exiting the program." << endl;
                return 0;
            }
            JsonParser1 = JsonParser(jsonInput1);
            JsonParser1.parseAndStoreJsonValue();
        }
    }
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
        else if (command == "exit" || command == "Exit") {
            cout << "Exiting the program." << endl;
            return 0;
            //break;
        }else{
            cout << "Invalid command please try again. " << endl;
        }
    }
    return 0;
}