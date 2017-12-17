/* ------------------------------------------------
* Graphical Traffic
*
* Class: CS 141, Fall 2017.  Tues 9am lab
* System: Windows 10, QT Creator
* Author: Phillip Sopt
* TA:  Moumita Samanta
*
* Description: This program is designed as a game similar to the Traffic Game in Program 2 except with graphics
* NOTICE: this program will need QT Creator and Stanford STD Libraries to compile.
* -------------------------------------------------
*/

#include <iostream>     //used for cout,cin
#include "gobjects.h"   //for all graphics objects
#include <string>       //used for input
#include <cstring>      //used for string operations
#include <fstream>      //for file io
#include <vector>       //used for holding the board.txt input

using namespace std;

int const SIZE = 6;//constant for the rows and columns in the table
int const CHAR_ZERO = 48;//constant char so i can switch between char and int easier

//DisplayHeader will display my info
void DisplayHeader() {
    //Display the Header
    cout << "Program #4 : Graphical Traffic" << endl;
    cout << "Author : Phillip Sopt" << endl;
    cout << "Lab : Tues 9am" << endl;
    cout << "TA : Moumita Samanta" << endl;
    cout << "Date : Oct 18, 2017" << endl;
    cout << "System : C++ in Qt Creator" << endl;
}

//DisplayInstructions will show how to play the game
void DisplayInstuctions() {

    cout << "Welcome to the traffic game!" << endl;
    cout << endl;

    cout << "Move the vehicles so that the Red car(RR) can exit" << endl;
    cout << "the board to the right.Each move should be of the" << endl;
    cout << "of the form : CDN   where :" << endl;
    cout << "C  is the vehicle to be moved" << endl;
    cout << "D  is the direction(u = up, d = down, l = left or r = right)" << endl;
    cout << "N  is the number of squares to move it" << endl;
    cout << "For example GR2  means move the G vehicle to the right" << endl;
    cout << "2 squares.Lower - case input such as   gr2  is also accepted." << endl;
    cout << "Enter \"reset\" to reset board, or \"exit\" to exit." << endl;
}

//DrawTableBackground draws the background of the game
void DrawTableBackground(GWindow graphicsWindow){
    //set up the Board outline
    GRect *boardOutline = new GRect(10,10,270,270);
    boardOutline->setColor("darkGray");
    boardOutline->setFilled(true);
    graphicsWindow.add(boardOutline);

    //setup inner playing surface
    GRect *innerPlaySurface = new GRect(20,20,250,250);
    innerPlaySurface->setColor("gray");
    innerPlaySurface->setFilled(true);
    graphicsWindow.add(innerPlaySurface);

    //setup the board exit
    GRect *boardExit = new GRect(270,100,10,50);
    boardExit->setColor("lightGray");
    boardExit->setFilled(true);
    graphicsWindow.add(boardExit);

    //ints used for the for loop to space the bar lines evenly
    int space = 40,width = 10,length = 250,start = 20;
    //width is for the width of the bars
    //length is for the length of the bars
    //start is the beginng point where the bars should start

    GRect *bar;//declare a blank bar to use

    //this for loop will create the bar for us
    for(int i = 0;i<=SIZE;i++){//loop through all the rows/columns
        bar = new GRect(start+(i*space),start,width,length);//define bar and place it in it's spot
        bar->setColor("lightGray");//set it's color
        bar->setFilled(true);//set it's fil
        graphicsWindow.add(bar);//output it to the window

        bar = new GRect(start,start+(i*space),length,width);//redifine bar and place it in a new place
        bar->setColor("lightGray");//set it's color
        bar->setFilled(true);//set it's fil
        graphicsWindow.add(bar);//output it to the window
    }
}

//ReadFileInput will attempt to read the file and store it in inputBoard
void ReadFileInput(vector<string> &inputBoard){
    ifstream inputFileStream;//input stream for the board.txt
    inputFileStream.open("board.txt");//open board.txt

    if(!inputFileStream.is_open()){//check if it's not open
        //if it's not open display an error and generate a board.txt
        cout << "ERROR: NO BOARD.TXT FOUND! GENERATING..." << endl;//display error

        ofstream boardOutput;//ouput stream for board.txt
        boardOutput.open("board.txt");//make board.txt

        //populate it
        boardOutput << "8" << endl
                    << "G H 1 1 2" << endl
                    << "Y V 1 6 3" << endl
                    << "M V 2 1 3" << endl
                    << "R H 3 2 2" << endl
                    << "B V 2 4 3" << endl
                    << "O V 5 1 2" << endl
                    << "C H 5 5 2" << endl
                    << "P H 6 3 3" << endl;

        boardOutput.close();//close and save board.txt
        inputFileStream.open("board.txt");//open board.txt again
    }
    string line;//temp string to hold the current line
    while(getline(inputFileStream, line)){//loop through the text file line by line
        inputBoard.push_back(line);//store the current line in the inputBoard
    }
    inputFileStream.close();//close the inputFile stream
}

//PopulateBoard will take the inputBoard and use it to construct the board array
void PopulateBoard(char (&board)[6][6], vector<string> inputBoard){
    int limit = inputBoard[0][0]-CHAR_ZERO;//the limit of the number of cars
    char color;//the color of the car
    char direction;//the direction of the car
    int row;//the row the car is in
    int column;//the column the car is in
    int length;//the car's length

    //fill board with '.'
    for(int i = 0; i<SIZE; i++){
        for(int j = 0; j<SIZE; j++){
            board[i][j] = '.';
        }
    }

    for(int i = 1; i <= limit; i++){
        string currentLine = inputBoard[i];
        //we can use these indexes in the currentLine because we are assuming perfect input
        //we set color/dir/row... to the matching char in the currentLine and convert to int as necessary
        color = currentLine.at(0);
        direction = currentLine.at(2);
        row = currentLine.at(4)-CHAR_ZERO-1;
        column = currentLine.at(6)-CHAR_ZERO-1;
        length = currentLine.at(8)-CHAR_ZERO;

        if(direction == 'V'){//if the car is verticle
            for(int j = 0; j < length; j++){//loop through the length
                board[row+j][column] = color;//and set the column space to the color
            }
        }
        else if(direction == 'H'){//if the car is horizontal
            for(int j = 0; j < length; j++){//do a similar loop
                board[row][column+j] = color;//and set the row space to the color
            }
        }
    }
}

//ResetBoard will take the users input and reset the board
void ResetBoard(vector<string> &inputBoard){
    string input;//used to push into the inputBoard
    string limit = "3";// used to hold the first value

    inputBoard.clear(); //clear the current input
    cout << "Enter the new board: " << endl;
    cin >> limit;   // the limit will be first

    inputBoard.push_back(limit);    //push limit in the inputBoard
    for(int i = 1; i <= limit[0] - CHAR_ZERO + 1; i++){ //loop through the limit
        getline(cin,input); //get each line the user enters
        if(input != ""){    //if it's not blank
            inputBoard.push_back(input);    //push the line into inputBoard
        }
    }
}

//ConvertCharToColor is a helper fuction that turns 'R' -> "red"
string ConvertCharToColor(char charColor){
    string result;
    switch (charColor) {
    case 'R'://if charColor is 'R'
        result = "red";//set the result to "red"
        break;//leave the switch statment
    case 'Y'://and so on...
        result = "yellow";
        break;
    case 'B':
        result = "blue";
        break;
    case 'G':
        result = "green";
        break;
    case 'C':
        result = "cyan";
        break;
    case 'M':
        result = "magenta";
        break;
    case 'O':
        result = "orange";
        break;
    case 'P':
        result = "pink";
        break;
    default://if charColor is non of those
        cout << "Invaild Color: Defaulting to Red..." << endl;//display an error message
        result = "red";
        break;
    }
    return result;
}

//FindRowOfCar is a helper function that finds the row of the car color coming from the right
int FindRowOfCar(char color, char board[SIZE][SIZE]){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(board[i][j] == color){
                return i;
            }
        }
    }
    return -1;
}

//FindColumnOfCar is a helper function that finds the column of the car color
int FindColumnOfCar(char color, char board[SIZE][SIZE]){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(board[i][j] == color){
                return j;
            }
        }
    }
    return -1;
}

//GetLengthOfCar is a helper function that finds the length of the car color
int GetLengthOfCar(char color, char board[SIZE][SIZE]){
    int counter = 0;
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(board[i][j] == color){
                counter++;
            }
        }
    }
    return counter;
}
//SwapPairs is a helper function to swap two cars positions from left to right
void SwapPairs(char & left, char & right){
    char temp;//temp to store left
    temp = left;
    left = right;
    right = temp;
}

//DisplayBoard clears the graphicsWindow and then uses board to display the updated board
void DisplayBoard(GWindow graphicsWindow, char board[SIZE][SIZE]){
    int origin = 30;    //origin of the table
    int width = 30;     //width of each square
    int space = 10;     //space between the squares
    static vector<GRect*> cars; //used to store the cars on screen

    //clear the current graphics window
    for(int i = 0; i < cars.size(); i++){
        graphicsWindow.remove(cars.at(i));
    }

    GRect *temp;  //temporary car to use for the loops
    //loop through the board and place each car where it belongs
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            //if there's a car in that location i,j then make temp a new car at that location
            //using origin as a reference and add i,j * the width of the car + the space
            //then set the color and add it to the graphicsWindow and add it to the vector cars
            if(board[i][j] != '.'){
                temp = new GRect(origin+(j*(width+space)),origin+(i*(width+space)),width,width);
                temp->setFilled(true);
                temp->setColor(ConvertCharToColor(board[i][j]));
                graphicsWindow.add(temp);
                cars.push_back(temp);

                //the following code is to fill in the spaces between cars
                if(i+1 < 7){ //don't go out of board bounds
                    if(board[i][j]==board[i+1][j]){ //if the same color is next to you horizontally
                        //modify the bounds by changing only the height
                        temp->setBounds(temp->getBounds().getX(),
                                        temp->getBounds().getY(),
                                        temp->getBounds().getWidth(),
                                        temp->getBounds().getHeight()+space);
                    }
                }

                if(j+1 < 7){
                    if(board[i][j]==board[i][j+1]){ //if the same color is next to you vertically
                        //modify the bounds by changing only the width
                        temp->setBounds(temp->getBounds().getX(),
                                        temp->getBounds().getY(),
                                        temp->getBounds().getWidth()+space,
                                        temp->getBounds().getHeight());
                    }
                }
            }//end if(board[i][j] != '.'
        }//end of j loop
    }//end of i loop
}

//DeterminOrientation uses color to find out if the car is horizontal or vertical
char DeterminOrientation(char color, char board[SIZE][SIZE]){
    //call helper functions to find the car's position
    int i = FindRowOfCar(color,board);
    int j = FindColumnOfCar(color,board);

    //if i+1 is inside the range of board
    if(i+1 <= SIZE){
        if(board[i+1][j] == color){
            return 'V';
        }
    }
    return 'H';
}

//Move Piece will move the car by distance, movDim is the moving Dimension and staticDim is the stationary Dimension
void MovePieceHorizontal(char color, int distance, char (&board)[SIZE][SIZE], int moveDim, int staticDim, int length, char direction){
    int rightEnd = moveDim + length - 1;//store the other end of the car, the right end
    if(direction == 'L'){//if we are moving left
        if(moveDim - distance < 0){
            cout << "Attempt is made to move a piece off the edge of the board.  Please retry." << endl;
            return;
        }
        //Check for overlap
        for(int i = moveDim; i >= moveDim - distance; i--){
            if(board[staticDim][i] != '.' && board[staticDim][i] != color){//check the rows
                cout << "Attempt is made to move a piece on top of another piece.  Please retry." << endl;
                return;
            }
        }
        //Move the car
        for(int i = rightEnd; i > rightEnd - distance; i--){//start at the other side of the car, and loop to the left til distance
            SwapPairs(board[staticDim][i],board[staticDim][i - length]);
        }
    }
    else if(direction == 'R'){//if we are moving right
        if(rightEnd + distance >= SIZE){
            cout << "Attempt is made to move a piece off the edge of the board.  Please retry." << endl;
            return;
        }
        //Check for overlap
        for(int i = rightEnd; i <= rightEnd + distance; i++){
            if(board[staticDim][i] != '.' && board[staticDim][i] != color){//check the rows for overlap
                cout << "Attempt is made to move a piece on top of another piece.  Please retry." << endl;
                return;
            }
        }
        //Move the car
        for(int i = moveDim; i < moveDim + distance; i++){//start at the other side of the car, and loop to the right til distance
            SwapPairs(board[staticDim][i],board[staticDim][i + length]);
        }
    }
}

//Move Piece will move the car by distance, movDim is the moving Dimension and staticDim is the stationary Dimension
void MovePieceVertical(char color, int distance, char (&board)[SIZE][SIZE], int moveDim, int staticDim, int length, char direction){
    int bottomEnd = moveDim + length - 1;//store the other end of the car, the bottom end
    if(direction == 'U'){//if we are moving left
        if(moveDim - distance < 0){
            cout << "Attempt is made to move a piece off the edge of the board.  Please retry." << endl;
            return;
        }
        //Check for overlap
        for(int i = moveDim; i >= moveDim - distance; i--){
            if(board[i][staticDim] != '.' && board[i][staticDim] != color){//check the rows
                cout << "Attempt is made to move a piece on top of another piece.  Please retry." << endl;
                return;
            }
        }
        //Move the car
        for(int i = bottomEnd; i > bottomEnd - distance; i--){//start at the other side of the car, and loop up til distance
            SwapPairs(board[i][staticDim],board[i - length][staticDim]);
        }
    }
    else if(direction == 'D'){//if we are moving right
        if(bottomEnd + distance >= SIZE){
            cout << "Attempt is made to move a piece off the edge of the board.  Please retry." << endl;
            return;
        }
        //Check for overlap
        for(int i = bottomEnd; i <= bottomEnd + distance; i++){
            if(board[i][staticDim] != '.' && board[i][staticDim] != color){//check the rows for overlap
                cout << "Attempt is made to move a piece on top of another piece.  Please retry." << endl;
                return;
            }
        }
        //Move the car
        for(int i = moveDim; i < moveDim + distance; i++){//start at the other side of the car, and loop down til distance
            SwapPairs(board[i][staticDim],board[i + length][staticDim]);
        }
    }
}

//Start of int main
int main(){
    GWindow *graphicsWindow = new GWindow(300,300);//set up the Graphics window
    char board[SIZE][SIZE];//the board for the game
    vector<string> inputBoard;//make a new place to store the stuff we find in board.txt
    string userInput;//string used to analyze the user's input

    //input vars to store into after we break apart the string userInput
    char color;
    char direction;
    int distance;

    //fill board with '.'
    for(int i = 0; i<SIZE; i++){
        for(int j = 0; j<SIZE; j++){
            board[i][j] = '.';
        }
    }

    DisplayHeader();//Display all my info first
    DisplayInstuctions();//Displays the instructions like in Traffic
    DrawTableBackground(*graphicsWindow);//Draw the table in the back
    ReadFileInput(inputBoard);//read the input file and store it to inputBoard
    PopulateBoard(board,inputBoard);//take inputBoard and populate board with it
    DisplayBoard(*graphicsWindow,board);

    //game loop
    while(true){
        cout << "Please enter an input: " << endl;
        cin >> userInput;

        //the following if else statments check if the input is valid
        if(userInput == "exit"){
            exit(0);
        }
        else if(userInput == "reset"){//if the user wants to reset the board
            ResetBoard(inputBoard);
            PopulateBoard(board,inputBoard);
            DisplayBoard(*graphicsWindow,board);
            continue;
        }
        else if(userInput.size() > 3){//3 is the expected input "gr3"
            //check if user entered too many chars, and display a message and loop again if it is
            cout << "User input is not one of the valid color or action items.  Please retry." << endl;
            continue;
        }
        else if(!isalpha(userInput.at(0)) || !isalpha(userInput.at(1)) || !isdigit(userInput.at(2)) || userInput.at(2)-CHAR_ZERO < 0){
            cout << "User input is not one of the valid color or action items.  Please retry." << endl;
            continue;
        }

        //set the variables to capital
        color = toupper(userInput.at(0));
        direction = toupper(userInput.at(1));
        distance = userInput.at(2)-CHAR_ZERO;

        //if user enters negative distance display an error and continue loop
        if(distance <= 0){
            cout << "User input is not one of the valid color or action items.  Please retry." << endl;
            continue;
        }

        //properties of the car we want to move
        int row = FindRowOfCar(color, board);
        int column = FindColumnOfCar(color, board);
        int length = GetLengthOfCar(color, board);
        char orientation = DeterminOrientation(color,board);

        //if the car was not found on the board display an error
        if(length == 0){
            cout << "User input is not one of the valid color or action items.  Please retry." << endl;
            continue;
        }

        //using direction move the piece or if direction isn't valid for that piece display error
        if(direction == 'R' && orientation == 'H'){
            MovePieceHorizontal(color, distance, board, column, row, length, direction);
        }
        else if(direction == 'L' && orientation == 'H'){
            MovePieceHorizontal(color, distance, board, column, row, length, direction);
        }
        else if(direction == 'U' && orientation == 'V'){
            MovePieceVertical(color, distance, board, row, column, length, direction);
        }
        else if(direction == 'D' && orientation == 'V'){
            MovePieceVertical(color, distance, board, row, column, length, direction);
        }
        else{
            cout << "Attempt was made to move a piece in the wrong direction.  Please retry." << endl;
            continue;
        }
        while(true){
            DisplayBoard(*graphicsWindow,board);//redisplay the board
        }

        //check it the player won
        if(board[2][5] == 'R'){
            cout << ">>>>>>>YOU WIN!!!!<<<<<<" << endl;
            exit(0);
        }
    }
    return 0;
}
