//By Filiz İpek Oktay - 30780

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct resource { //Available resources loaded
    string resType;
    int resQuan;
};

struct consump {
    char builType;
    vector<int> reqQuan; //Required quantity
};


bool nameCheck(string & filename, string & filetype) { //Function for file name checking
    if(filename.length() > filetype.length() && filename.substr(filename.length() - 4) == ".txt"){ // Checking the extension part of the filename
        if(filename.substr(0, 5) == filetype && ((filename.at(5) - '0') <= 9)){ //Matching the input with the filetype
            return true;
        } else if(filename.substr(0, 11) == filetype && ((filename.at(11) - '0') <= 9)){ //Checking if the number is in the correct range
            return true;
        } else if(filename.substr(0, 6) == filetype && ((filename.at(6) - '0') <= 9)){ //Checking if the number is in the correct range
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool suffStock(vector<resource> &stock, vector<consump> &consumption, vector<vector<char>> &colony, string & notsuff) { //Function that returns the bool if the quantity of resources in the stock is enough to construct the buildings
    for (int i = 0; i < colony.size(); i++) {
        for (int j = 0; j < colony[0].size(); j++) { //For each of the elements in the the colony matrix
            if (colony[i][j] == '-') {// Cell is empty, resources won't be reduced
            } else { //Cell is full
                bool foundBuilding = false;
                for (int k = 0; k < consumption.size(); k++) {
                    if (colony[i][j] == consumption[k].builType) { //Checking if the building type matches
                        foundBuilding = true;
                        bool resourcesEnough = true;
                        for (int m = 0; m < stock.size(); m++) {
                            if (consumption[k].reqQuan[m] > stock[m].resQuan) { //Checking if the stock of each resource is enough
                                resourcesEnough = false;
                                notsuff = stock[m].resType; //Insufficient resource
                                break; // No need to check other resources
                            } else {
                                stock[m].resQuan -= consumption[k].reqQuan[m]; // Consuming the required resource
                            }
                        }
                        if (!resourcesEnough) {
                            return false;
                        }
                    }
                }
                if (!foundBuilding) {
                    return false;
                }
            }
        }
    }
    return true;
}

void isValid(string build, vector<consump> &consumption, bool & val){ //Checking if the building type is valid according to consumption file
    val = false;
    for (int k = 0; k < consumption.size(); k++) {
        if (build.length() == 1 && build[0] == consumption[k].builType) { // Building type is valid
            val = true;
        }

    }
}


void unableFile(string & filename, string & filetype, bool & flag){ //Promting the appropriate message in case file does not open
    flag = nameCheck(filename, filetype);
    while (!flag){
        cout << "Unable to open the file " << filename << " for reading.\n"; //Displaying the appropriate message
        cout << "Please enter the correct file name: \n";
        cin >> filename; //Asking for the correct file name
        flag = nameCheck(filename, filetype);
    }
}


void printColony(vector<vector<char>> & colony){ //Printing the colony matrix
    cout << "Colony:\n";
    for(int j = 0; j < colony.size(); j++){
        for(int n = 0; n < colony[0].size(); n++){
            cout << setw(5) << left << colony[j][n];
        }
        cout << endl;
    }
}

void displayAll(vector<resource> &stock, vector<consump> &consumption, vector<vector<char>> &colony){ //Displaying the consumption quantities of all resources

    for (int m = 0; m < stock.size(); m++) {
        vector<vector<int>> colonyReduced(colony.size(), vector<int>(colony[0].size(), 0)); // Initializing a matrix of integers with 0
        cout << "Consumption of resource " << stock[m].resType << " by each building in the colony:\n";
        for (int i = 0; i < colony.size(); i++) {
            for (int j = 0; j < colony[0].size(); j++) {
                if (colony[i][j] == '-') { // Cell is empty, resources won't be reduced
                    colonyReduced[i][j] += 0; // No change to colonyReduced
                } else { //The consumption quantities will be added to new matrix
                    for (int k = 0; k < consumption.size(); k++) {
                        if (colony[i][j] == consumption[k].builType) {
                            colonyReduced[i][j] = consumption[k].reqQuan[m];
                        }
                    }
                }
                cout << setw(5) << left << colonyReduced[i][j]; //Printing the new matrix with a manipulation
            }
            cout << endl; // Move to the next row in the output
        }
    }
}

int main(){
    string filename, filetype, sentence, sentence2, sentence3, notsuff;
    ifstream file1, file2, file3;
    bool flag, nameCheck(string & filename, string & filetype);


    cout << "Welcome to the colony management system \n"; // Prompting welcome message
    filetype = "stock";
    cout << "Please enter file name for resources stock:\n"; //asking for the file name
    cin >> filename;
    unableFile(filename, filetype, flag); //Prompting error message for unable filename

    vector<resource> stock; //Source vector constructed by resource struct

    file1.open(filename.c_str()); //Opening the stock file
    if(!file1.fail()){
        while(getline(file1,sentence)){
            resource temp;
            temp.resType = sentence.substr(0,sentence.find(' '));//Temporary struct to push back into the stock vector
            string str = sentence.substr(sentence.find(' ')+ 1);
            int num;
            stringstream ss;
            //Converting string to an integer by using sstream
            ss << str;
            ss >> num;

            temp.resQuan = num;
            stock.push_back(temp);
        }
        cout << "Available resources loaded from " << filename << endl; //Printing the available resources
        cout << "Resource stock:\n";
        for(int i = 0; i < stock.size(); i++){ //Printing the resources
            cout << stock[i].resType << " " << stock[i].resQuan << endl;
        }

        filetype = "consumption";
        cout << "Please enter file name for resource consumption per building type:\n"; //Asking for building type that will be constructed
        cin >> filename;
        unableFile(filename, filetype, flag);

        vector<consump> consumption; //Creating new vector of consump struct to store the consumption file data

        file2.open(filename.c_str()); //Opening the consumption file
        if(!file2.fail()){
            while(getline(file2,sentence2)){ //Reading the file and storing it into a vector of a vector of a struct
                consump temp;
                temp.builType = sentence2[0];
                int idx = sentence2.find(' ') + 1;
                while(sentence2.find(' ', idx) != string::npos){ //Adding the required quantity values into the struct of a vector
                    string str = sentence2.substr(idx, sentence2.find(' ', idx) - idx);
                    int num;
                    stringstream ss1;
                    //Converting string to an integer by using sstream
                    ss1 << str;
                    ss1 >> num;
                    temp.reqQuan.push_back(num);
                    idx = sentence2.find(' ', idx) + 1;
                }

                string str2 = sentence2.substr(sentence2.rfind(' ') + 1);
                int num2;
                stringstream ss2;
                //Converting string to an integer by using sstream
                ss2 << str2;
                ss2 >> num2;

                temp.reqQuan.push_back(num2);
                consumption.push_back(temp); //Creating new vector of consump struct to store the consumption file data
            }
            cout << "Resources consumption per building type loaded from " << filename << endl;
            cout << "Resources consumption per building type:\n";
            for(int i = 0; i < consumption.size(); i++){ //Printing the resources
                cout << consumption[i].builType << " ";
                for(int j = 0; j < consumption[0].reqQuan.size() - 1; j++){//Printing the vector of struct
                    cout << consumption[i].reqQuan[j] << " ";
                }
                cout << consumption[i].reqQuan[consumption[0].reqQuan.size() - 1] << endl; //The last quantity of each line
            }

            filetype = "colony";
            cout << "Please enter file name for colony:\n"; //Asking for the colony filename
            cin >> filename;
            unableFile(filename, filetype, flag); //In case the filename is not correct

            vector<vector<char>> colony; //Creating a matrix to store the colony file data

            file3.open(filename.c_str()); // Opening the colony file
            if(!file3.fail()){
                while(getline(file3,sentence3)){ //To store the file into a vector of vector of characters
                    vector<char> temp; //Temporary vector to push back it into colony matrix
                    for(int i = 0; i < sentence3.size(); i++){
                        temp.push_back(sentence3[i]);
                    }
                    colony.push_back(temp);

                }

                unsigned long int colonySize = colony.size() * colony[0].size(); //Adding a variable to not calculate colony.size() every time


                if(suffStock(stock, consumption, colony, notsuff)){ //To check whether resources in the stock are sufficient
                    cout << "Colony loaded from " << filename << endl;

                    printColony(colony); //Printing the colony matrix

                    cout << "Resources stock after loading the colony:\n";
                    cout << "Resource stock:\n"; //Promting the remaining resource quantities
                    for(int i = 0; i < stock.size(); i++){ //Printing the resources
                        cout << stock[i].resType << " " << stock[i].resQuan << endl;
                    }

                    displayAll(stock, consumption, colony); //Displaying all the resources

                    //OPTIONS
                    cout << "Please enter an option number:\n"; //Promting the option menu
                    cout << "1. Construct a new building on the colony.\n";
                    cout << "2. Destruct/Disassemble a building from the colony.\n";
                    cout << "3. Print the colony.\n";
                    cout << "4. Print the consumption of all resources by each building in the colony.\n";
                    cout << "5. Print the consumption of a specific resource by each building in the colony.\n";
                    cout << "6. Print the resources stock.\n";
                    cout << "7. Exit the program.\n";

                    string optionStr ;
                    cin >> optionStr;
                    int optionNum = 0;

                    stringstream ss3;
                    //converting string to an integer by using sstream
                    ss3 << optionStr;
                    ss3 >> optionNum;


                    while (optionNum != 7) {
                        while (optionNum > 0 && optionNum < 8) { //If the option input is in correct change

                            if (optionNum == 1) { // Constructing new buildings
                                bool empty = false;
                                int count = 0;

                                // Counting the number of empty cells in the colony
                                for (int k = 0; k < colony.size(); k++) {
                                    for (int p = 0; p < colony[0].size(); p++) {
                                        if (colony[k][p] == '-') {
                                            empty = true;//If empty is true that means there is at least one cell that is empty
                                            count++;
                                        }
                                    }
                                }

                                if (!empty) { //No empty cells, all are full
                                    cout << "There are no empty cells in the colony. Can not add a new building.\n";
                                } else { //All cells are empty
                                    cout << "Please enter the type of the building that you want to construct:\n";
                                    string build;
                                    cin >> build;
                                    bool val = false; // Valid building type
                                    vector<consump> consumeVector; // A vector to store the quantities of the resources that will be consumed from the stock

                                    // Check if the building type is valid
                                    isValid(build, consumption, val);

                                    while (!val) { // Invalid building type
                                        cout << "Invalid building type, please enter a valid building type:\n";
                                        cin >> build;
                                        isValid(build, consumption, val); //Updating the val boolean
                                    }

                                    while(val){ //Valid building type
                                        int cellNum = 0; // Number of cells that will occupy
                                        cout << "Please enter the number of cells that the building will occupy:\n"; // Asking for the number of cells
                                        cin >> cellNum;


                                        while(cellNum == 0 || count < cellNum) { //Invalid number of cells
                                            cout << "Invalid number of cells, please enter a valid number of cells:\n";
                                            cin >> cellNum;
                                        }


                                        bool flag3 = false, flag4 = true;

                                        // Matching the building type with the consumption data
                                        for (int j = 0; j < consumption.size(); j++) {
                                            if (build[0] == consumption[j].builType) {
                                                consumeVector.push_back(consumption[j]); // Storing the consumption values
                                            }

                                        }

                                        for(int j = 0; j < consumeVector.size(); j++){
                                            for (int p = 0; p < consumeVector[0].reqQuan.size(); p++) {
                                                if ((cellNum * consumeVector[j].reqQuan[p]) <= stock[p].resQuan) {//Checking the resource quantities if they are sufficient
                                                    flag3 = true;
                                                } else { // If resources in the stock are not sufficient
                                                    cout << "Not enough " << stock[p].resType << " to build this building.\n";
                                                    val = false;
                                                    flag3 = false;
                                                    flag4 = false;
                                                    break;
                                                }
                                                if(!flag4)
                                                    break;

                                            }
                                        }

                                        if (flag3) {
                                            vector<vector<int>> rowCol(cellNum); //New container to store the row, column data
                                            for (int i = 0; i < cellNum; i++) {
                                                bool corRowCol = false;
                                                cout << "Please enter the row and the column index of the cell number " << i + 1 << ":\n";
                                                int row, column;
                                                cin >> row >> column; // Taking input of row and column
                                                while (!corRowCol) { //If the row, column pairs are valid
                                                    if ((colony.size() - 1 >= row && colony[0].size() - 1 >= column) && colony[row][column] == '-') { // Checking if it is empty
                                                        rowCol[i].push_back(row);
                                                        rowCol[i].push_back(column);
                                                        corRowCol = true;
                                                    } else if ((colony.size() - 1 >= row && colony[0].size() - 1 >= column) && colony[row][column] != '-'){ //If the cell is not empty
                                                        cout << "The cell is not empty, please enter the row and the column index of another cell:\n";
                                                        cin >> row >> column;
                                                    }else { //Invalid row and column
                                                        cout << "Invalid row or column index, please enter a valid row and column index:\n";
                                                        cin >> row >> column;
                                                    }
                                                }
                                            }

                                            // Constructing the buildings
                                            for (int i = 0; i < rowCol.size(); i++) {
                                                int row = rowCol[i][0]; //Row input
                                                int col = rowCol[i][1]; //Column input
                                                colony[row][col] = build[0]; // Adding building type char into the colony matrix
                                                for (int p = 0; p < stock.size(); p++) { // To consume the resources from the stock
                                                    stock[p].resQuan -= (consumeVector[0].reqQuan[p] * cellNum); //If the resources in the stock are enough for whole consumption of the resources
                                                }
                                            }
                                            cout << "The building is added to the colony.\n";
                                            val = false; //The building is constructed, getting out of loop
                                        }
                                    }
                                }
                            } else if(optionNum == 2){ //Removing a building

                                //Counting the empty places
                                int count = 0;
                                for (int k = 0; k < colony.size(); k++) {
                                    for (int p = 0; p < colony[0].size(); p++) {
                                        if (colony[k][p] == '-') {
                                            count++;
                                        }
                                    }
                                }

                                if(count == colonySize){//No buildings in the colony
                                    cout << "There are no buildings in the colony. Can not remove a building.\n";
                                } else { //There is at least one colony that is empty
                                    cout << "Please enter the row and the column index of the cell that you want to remove:\n";
                                    int row = 0, col = 0;
                                    cin >> row >> col; // Taking the input of row and column

                                    //Checking whether the number of row and column exist in the colony matrix
                                    bool flag2 = false;
                                    while(!flag2){
                                        if((row >= 0 && col >= 0) && (row <= colony.size() - 1) && (col <= colony[0].size() - 1)){
                                            if(colony[row][col] == '-'){
                                                cout << "The cell is already empty, please enter the row and the column index of another cell:\n";
                                                cin >> row >> col;
                                            } else {
                                                flag2 = true;
                                            }
                                        } else {
                                            cout << "Invalid row or column index, please enter a valid row and column index:\n";
                                            cin >> row >> col;
                                        }
                                    }


                                    //Adding the remaning resources to the stock after removing a building
                                    for(int t = 0; t < consumption.size(); t++){
                                        if(consumption[t].builType == colony[row][col]){
                                            for(int p = 0; p < stock.size(); p++){
                                                stock[p].resQuan += consumption[t].reqQuan[p];
                                            }
                                        }
                                    }
                                    colony[row][col] = '-'; //Removing the building from the colony
                                    cout << "The building is removed, and the corresponding resources are added back to the stock.\n";
                                }


                            } else if (optionNum == 3){ //Printing the Colony
                                printColony(colony);

                            } else if (optionNum == 4){ //Displaying the quantites of all resources
                                displayAll(stock, consumption, colony);

                            } else if (optionNum == 5){ //Displaying the quantites of a specific resource
                                cout << "Please enter the type of the resource:\n"; //Getting the input in a case-sensitive manner
                                string resInput;
                                cin >> resInput;//Firstly, checking the input if it's valid or not

                                int pos = 0;
                                bool valid = false;
                                for(int n = 0; n < stock.size(); n++){
                                    if(resInput == stock[n].resType){
                                        valid = true;
                                        pos = n; //Position of the input that is matching with the stock matrix


                                        //Displaying resources according to the input of resource type
                                        cout << "Consumption of resource " << resInput << " by each building in the colony:\n";
                                        vector<vector<int>> colonyReduced(colony.size(), vector<int>(colony[0].size(), 0)); // Initialize the colonyReduced vector outside the loop
                                        for(int i = 0; i < colony.size(); i++){
                                            for (int j = 0; j < colony[0].size(); j++) {
                                                if (colony[i][j] == '-') { // Cell is empty, resources won't be reduced
                                                    colonyReduced[i][j] = 0; // No change to colonyReduced
                                                } else {
                                                    for (int k = 0; k < consumption.size(); k++) {
                                                        if (colony[i][j] == consumption[k].builType) { //Checking if the building type matches
                                                            colonyReduced[i][j] = consumption[k].reqQuan[pos];//Getting the quantities for each resource
                                                        }
                                                    }
                                                }
                                                cout << setw(5) << left << colonyReduced[i][j]; //Printing the resources and quantities
                                            }
                                            cout << endl; // Move to the next row in the output
                                            break; // To exit from the loop
                                        }
                                    }
                                }

                                if(!valid){ //Promting appropriate message for invalid input
                                    cout << "The resource " << resInput << " was not found in the resources stock.\n";
                                }

                            } else if (optionNum == 6){
                                cout << "Resource stock:\n"; //Promting the remaining resource quantities
                                for(int i = 0; i < stock.size(); i++){ //Printing the resources
                                    cout << stock[i].resType << " " << stock[i].resQuan << endl;
                                }
                            } else if(optionNum == 7){ //Exiting from the program by returning 0
                                cout << "Thank you for using the colony management system. The program will terminate.\nGoodbye!\n";
                                return 0;
                            }


                            cout << "Please enter an option number:\n"; // Prompting the option menu
                            cin >> optionStr;
                            stringstream ss4;
                            // Converting string to an integer using stringstream
                            optionNum = 0;
                            ss4 << optionStr;
                            ss4 >> optionNum;
                        }

                        while(!(optionNum > 0 && optionNum < 8)){
                            // If the code reaches here, it means the optionNum was not in the valid range (1 to 7).
                            cout << "Invalid option number.Please enter an option number:\n";
                            cin >> optionStr;
                            stringstream ss5;
                            // Converting string to an integer using stringstream
                            optionNum = 0;
                            ss5 << optionStr;
                            ss5 >> optionNum;
                        }
                    }

                    // When optionNum becomes 7, the loop will break, and the program will reach this point.
                    cout << "Thank you for using the colony management system. The program will terminate.\nGoodbye!\n";
                    return 0;

                } else { //Prompting message and terminating the program
                    cout << "Not enough " << notsuff << " to build this building.\n";
                    cout << "Not enough resources to build this colony.\n";
                    cout << "Thank you for using the colony management system. The program will terminate.\n";
                    cout << "Goodbye!\n";
                    return 0;
                }

            }
        }

    }

    return 0;
}
