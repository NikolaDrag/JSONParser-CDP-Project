#include "JsonValue.h"
#include "JsonParser.h"
#include <stack>

using std::stack;
using std::cin;


int main(){
    //1vi char sled ( e 'space', toest na position[0]
    /*string jsonInput1 = R"( {"name": "John Doe",
    "age": -30.90,
    "languages": "English"} )";
    JsonParser JsonParser1(jsonInput1);
    JsonParser1.parse();
    JsonParser1.parseAndStoreJsonValue();
    JsonParser1.printStoredJsonValues();*/

    string jsonInput2 = R"({
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
              cout <<  jsonInput2[201] << jsonInput2[202] <<jsonInput2[203]<< jsonInput2[204];
    JsonParser JsonParser2(jsonInput2);
    JsonParser2.parse();
    /*
    Notes: 1. parsing for legitamate input works, but care how "" are entered from user input with escape signs or not
    */
    return 0;
}