# JSONParser-CDP-Project
JSONParser CDP Project FMI

Here is a link to the github page of the project: https://github.com/NikolaDrag/JSONParser-CDP-Project/tree/main.
When you start the program you will be prompted to input a JSON text. Input a line with the text that you want to be parsed.

Then you can enter the following commands to perform various tasks:

1. Print/print -> print out the json in readable format to the console.
2. Save to file/save to file -> save the json input in json format in a seperate file. Follow the prompted instructions.
3. Find/find -> find all the bindings to a key and print them out or save them in a file.


We have the following classes: JsonParser and JsonValue. The main function is the console interface where we use the features of those classes.
The .txt files testInputX, where X is in the range [1,2,3,4], we have test inputs which we read and we can test in the unitTests.cpp.
CommandToCompileTest.txt is the command needed to compile the unitTests.cpp into an .exe in VSCode.
TestJson1 and TestsJson2 are files which were created using the Save to file features of the project.