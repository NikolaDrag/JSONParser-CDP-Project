#include "JsonValue.h"
#include "JsonParser.h"

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
            cout << "Please enter your desired filename: ";
            string fileName;
            std::getline(cin, fileName);
            JsonParser1.saveToJsonFile(JsonParser1.getStoredValues(), fileName);
        }
        else if(command == "find" || command == "Find"){ //shte vidim kak da sledi za key-name (key-string)
            cout << "Enter key that you want to find bindings for: ";
            string key;
            std::getline(cin,key);
            char choice1;
            cout << "Select the desired action: (P)rint, (W)rite in file.";
            cin >> choice1;
            char choice2;
            cout << "Select if you wish the use the whole array or a element at index: (W)hole array, (I)ndex.";
            cin >> choice2;
            cin.ignore();
            int index;
            if(choice2 == 'I' || choice2 == 'i'){
                cout << "Enter your desired index (starting at 0): ";
                cin >> index;
                cin.ignore();
            }
            if(choice1 == 'p' || choice1 == 'P'){
                if(choice2 == 'W' || choice2 == 'w'){
                    JsonParser1.getStoredValues().findByKey(key).print("");
                    cout << endl;
                }
                if(choice2 == 'I' || choice2 == 'i'){
                    JsonParser1.getStoredValues().findByKey(key).getArrayval()[index].print("");
                    cout << endl;
                }
            }
        }
        else if (command == "exit" || command == "Exit") {
            cout << "Exiting the program." << endl;
            //return 0;
            break;
        }else{
            cout << "Invalid command please try again.";
        }
    }

    /*string jsonInput2 = R"({
   "name":"ACME Software Co.",
   "type":"Software Development Company",
   "offices": [
               {
                   "name":"Headquarters",
                   "address":"Sofia"
               },
               {
                   "name":"Front Office",
                   "address":"New York City"
               }
              ],
   "members":[
               {   
                   "id" : "0",
                   "name" : "John Smith",
                   "birthdate" : "1980-01-01"
               },
               {
                   "id" : "1",
                   "name" : "Jane Smith",
                   "birthdate" : "1981-02-02"
               },
               {
                   "id" : "2",
                   "name" : "John Doe",
                   "birthdate" : "1982-03-03"
               }
           ],
   "management":{
                   "directorId":"0",
                   "presidentId":"1"
                }
 }

              )";
              //cout <<  jsonInput2[201] << jsonInput2[202] <<jsonInput2[203]<< jsonInput2[204];
    JsonParser JsonParser2(jsonInput2);
    JsonParser2.parseAndStoreJsonValue();
    //JsonParser2.printStoredJsonValues();
    //JsonParser2.shortPrintStoredJsonValues();
    JsonParser2.getStoredValues().findByKey("name").print("");
    //string testFileName = "TestJson1";
    //JsonParser2.saveToJsonFile(JsonParser2.getStoredValues(), testFileName);*/
    /*
    Notes: 1. parsing for legitamate input works, but care how "" are entered from user input with escape signs or not
    */
    return 0;
}