#include "JsonValue.h"
#include "JsonParser.h"

void saveToFileCommand(JsonParser &parser1){
    cout << "Please enter your desired filename: ";
    string fileName;
    std::getline(cin, fileName);
    parser1.saveToJsonFile(fileName);
    return;
}

void findCommand(JsonParser &parser1){ //add for invalid command maybe
    cout << "Enter key that you want to find bindings for: ";
    string key;
    std::getline(cin,key);
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

int main(){
    //for console interface - map of function pointers if too many commands, for now we can do swtich/if,elseif...
    cout << "Enter the JSON string:\n";
    string jsonInput1;
    std::getline(cin, jsonInput1);
    JsonParser JsonParser1(jsonInput1);
    JsonParser1.parseAndStoreJsonValue();

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
        else if (command == "exit" || command == "Exit") {
            cout << "Exiting the program." << endl;
            //return 0;
            break;
        }else{
            cout << "Invalid command please try again. ";
        }
    }
    return 0;
}