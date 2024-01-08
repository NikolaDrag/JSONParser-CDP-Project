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

void testAParser(JsonParser &testParser, vector<string> &path){
    testParser.parseAndStoreJsonValue(); //test parsing and storing
    testParser.printStoredJsonValues(); //test printing
    cout << endl;
    testParser.shortPrintStoredJsonValues(); //test short printing
    cout << endl;
    testParser.getStoredValues().findByKey("name").print(""); //test search by key
    cout << endl;
    string testFileName = "TestFileToRecord1";
    testParser.getStoredValues().findByKey("name").saveToFile(testFileName);
    testParser.saveToJsonFile(testFileName);//test save in new name
    testParser.saveToJsonFile(testFileName); //test overwrite and write to a new file //until here everything works
    //testParser.getStoredValues().deleteElementOnPath(path,0); //test delete element (not tested)
}

int main(){
    vector<string> path;
    string input1 = recordInputFromFile("testInput1.txt");
    JsonParser JsonParser1(input1);
    testAParser(JsonParser1,path);
    /*string input2 = recordInputFromFile("testInput2.txt");
    JsonParser JsonParser2(input2);
    testAParser(JsonParser2,path);
    string input3 = recordInputFromFile("testInput3.txt");
    JsonParser JsonParser3(input3);
    testAParser(JsonParser3,path);*/
    return 0;
}