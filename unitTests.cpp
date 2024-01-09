#include "JsonValue.h"
#include "JsonParser.h"

string recordInputFromFile(const string &fileName){
    std::ifstream f;
    f.open(fileName);
    string plh="-", mainStr="";
    while(plh!="")
    {
        plh="";
        std::getline(f, plh);
        mainStr=mainStr+plh;
    }
    f.close();
    return mainStr;
}

void testParseAndStoreJsonValue(JsonParser &testParser){
    testParser.parseAndStoreJsonValue();  //test parsing and storing
}

void testPrint1(JsonParser &testParser){
    testParser.printStoredJsonValues(); //test printing
}

void testFindByKey(JsonParser &testParser){
    testParser.getStoredValues().findByKey("name").print(""); //test search by key
}


void testPrint2(JsonParser &testParser){
    testParser.shortPrintStoredJsonValues(); //test short print
}

void testPrint3(JsonParser &testParser){
    string indent = "";
    testParser.exactPrintStoredJsonValues(indent); //test exact print
}

void testSaveToFile(JsonParser &testParser,string &testFileName){
    testParser.saveToJsonFile(testFileName);
}

void testSaveByKeyToFile(JsonParser &testParser,string &testFileName){
    testParser.getStoredValues().findByKey("name").saveToFile(testFileName);
}

void testRemove(JsonParser &testParser,vector<string> &testPath){
    string plh ="";
    testParser.getStoredValues().findElementByPath(testPath, 0,"delete",testParser.getStoredValues(),plh);
}

int main(){
    string testFileName = "TestFileToRecord1";
    vector<string> path;
    path.push_back("offices");
    path.push_back("0");
    path.push_back("name");
    // Test case 1
    string input1 = recordInputFromFile("testInput1.txt");
    JsonParser jsonParser1(input1);

    cout << "Test Case 1:" << endl;
    testParseAndStoreJsonValue(jsonParser1);
    testPrint1(jsonParser1);
    /*testFindByKey(jsonParser1);
    testPrint2(jsonParser1);
    testPrint3(jsonParser1);
    testSaveToFile(jsonParser1, testFileName);
    testSaveByKeyToFile(jsonParser1, testFileName);
    testRemove(jsonParser1, path);*/

    path.pop_back();
    path.pop_back();
    path.pop_back();
    path.push_back("name");
    // Test case 2
    string input2 = recordInputFromFile("testInput2.txt");
    JsonParser jsonParser2(input2);

    cout << "\nTest Case 2:" << endl;
    testParseAndStoreJsonValue(jsonParser2);
    testPrint1(jsonParser2);
    /*testFindByKey(jsonParser2);
    testPrint2(jsonParser2);
    testPrint3(jsonParser2);
    testSaveToFile(jsonParser2, testFileName);
    testSaveByKeyToFile(jsonParser2, testFileName);
    testRemove(jsonParser2, path);*/

    // Test case 3
    string input3 = recordInputFromFile("testInput3.txt");
    JsonParser jsonParser3(input3);

    cout << "\nTest Case 3:" << endl;
    testParseAndStoreJsonValue(jsonParser3);
    testPrint1(jsonParser3);
    /*testFindByKey(jsonParser3);
    testPrint2(jsonParser3);
    testPrint3(jsonParser3);
    testSaveToFile(jsonParser3, testFileName);
    testSaveByKeyToFile(jsonParser3, testFileName);
    testRemove(jsonParser3,path);*/

    // Test case 4
    string input4 = recordInputFromFile("testInput4.txt");
    JsonParser jsonParser4(input4);

    cout << "\nTest Case 4:" << endl;
    testParseAndStoreJsonValue(jsonParser4);
    testPrint1(jsonParser4);
    /*testFindByKey(jsonParser4);
    testPrint2(jsonParser4);
    testPrint3(jsonParser4);
    testSaveToFile(jsonParser4, testFileName);
    testSaveByKeyToFile(jsonParser4, testFileName);
    testRemove(jsonParser4, path);*/

    string input5 = R"( Enter a test input to test )";
    JsonParser jsonParser5(input5);
    return 0;
}