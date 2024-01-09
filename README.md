# JSONParser-CDP-Project
JSONParser CDP Project FMI

Here is a link to the github page of the project: https://github.com/NikolaDrag/JSONParser-CDP-Project/tree/main.
When you start the program you will be prompted to input a JSON text. Input a text that you want to be parsed.

Then you can enter the following commands to perform various tasks:

1. Print/print -> print out the json in readable format to the console.
2. Save to file/save to file -> save the json input in json format in a seperate file. Follow the prompted instructions.
3. Find/find -> find all the bindings to a key and print them out or save them in a file.
4. Delete/delete/Delete on path/delete on path -> enter a path and delete the element. If path is empty or incorrect you will get an invalid path message.
5. Change/change/Change element/change element -> enter a path and change the value of the element corresponding to a key or to an array index.
6. Save path/save path/Save on path/ save on path -> enter a filename and a path after and you can extract the element value to the desired file.
7. Insert/insert - insert a element at a position provided a path.

We have the following classes: JsonParser and JsonValue. The main function is the console interface where we use the features of those classes.
The .txt files testInputX, where X is in the range [1,2,3,4], we have test inputs which we read and we can test in the unitTests.cpp.
CommandToCompileTest.txt is the command needed to compile the unitTests.cpp into an .exe in VSCode.
TestJson1 and TestsJson2 are files which were created using the Save to file features of the project.