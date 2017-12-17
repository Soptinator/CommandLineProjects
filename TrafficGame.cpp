/* ------------------------------------------------
* Traffic Game
*
* Class: CS 141, Fall 2017.  Tues 9am lab
* System: Windows 10, Visual Studio Community 2017
* Author: Phillip Sopt
* TA:  Moumita Samanta
*
* Description: This program is designed as a game to move cars around in a table until the player wins
* -------------------------------------------------
*/
#include<iostream>//for cout and cin
#include<iomanip>//for setw

using namespace std;

//global chars to store the values of the table
char p1 = 'G', p2 = 'G', p3 = '.', p4 = '.', p5 = '.', p6 = 'Y',
p7 = 'P', p8 = '.', p9 = '.', p10 = 'B', p11 = '.', p12 = 'Y',
p13 = 'P', p14 = 'R', p15 = 'R', p16 = 'B', p17 = '.', p18 = 'Y',
p19 = 'P', p20 = '.', p21 = '.', p22 = 'B', p23 = '.', p24 = '.',
p25 = 'O', p26 = '.', p27 = '.', p28 = '.', p29 = 'T', p30 = 'T',
p31 = 'O', p32 = '.', p33 = 'F', p34 = 'F', p35 = 'F', p36 = '.';

//ResetBoard will ask the user to change the board
void ResetBoard() {
	cout << "You have choosen to reset the board. Enter in 36 characters for the new board. Capitals Please: " << endl;
	cin >> p1 >> p2 >> p3 >> p4 >> p5 >> p6 >> p7 >> p8 >> p9 >> p10 >> p11 >> p12 >> p13 >> p14 >> p15 >> p16 >> p17 >> p18 >> p19 >> p20 >> p21 >> p22 >> p23 >> p24 >> p25 >> p26 >> p27 >> p28 >> p29 >> p30 >> p31 >> p32 >> p33 >> p34 >> p35 >> p36;
}

//Exit will exit the program with a little message
void Exit() {
	cout << "Thanks for playing!" << endl;
	exit(0);
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
	cout << "Enter '-' to reset board, or 'x' to exit." << endl;
}

// DisplayBoard will print out the board to the console
void DisplayBoard() {

	const int W = 2;// used for the width for setw
	cout << setw(W) << "- - - - - - - - -" << endl;
	cout << setw(W) << "|" << setw(W) << p1 << setw(W) << p2 << setw(W) << p3 << setw(W) << p4 << setw(W) << p5 << setw(W) << p6 << setw(W) << "|" << endl;
	cout << setw(W) << "|" << setw(W) << p7 << setw(W) << p8 << setw(W) << p9 << setw(W) << p10 << setw(W) << p11 << setw(W) << p12 << setw(W) << "|" << endl;
	cout << setw(W) << "|" << setw(W) << p13 << setw(W) << p14 << setw(W) << p15 << setw(W) << p16 << setw(W) << p17 << setw(W) << p18 << endl;
	cout << setw(W) << "|" << setw(W) << p19 << setw(W) << p20 << setw(W) << p21 << setw(W) << p22 << setw(W) << p23 << setw(W) << p24 << setw(W) << "|" << endl;
	cout << setw(W) << "|" << setw(W) << p25 << setw(W) << p26 << setw(W) << p27 << setw(W) << p28 << setw(W) << p29 << setw(W) << p30 << setw(W) << "|" << endl;
	cout << setw(W) << "|" << setw(W) << p31 << setw(W) << p32 << setw(W) << p33 << setw(W) << p34 << setw(W) << p35 << setw(W) << p36 << setw(W) << "|" << endl;
	cout << setw(W) << "- - - - - - - - -" << endl;
}

//MoveOneSquare takes two chars and switches them from left to right
void MoveOneSquare(char& charLeft, char& charRight) {
	char temporary = charRight;
	charRight = charLeft;
	charLeft = temporary;
}
//MovePair checks to see if the 2 chars can be switched and then switches them
void MovePair(char& char1, char& char2, char realLetter) {

	static bool checkError = false; //used to avoid displaying error multiple times
	if (char1 == realLetter) { //if the char1 is what we are looking for

		if (char2 == '.') { //and char2 is an empty spot

			MoveOneSquare(char1, char2); //switch char1 and char2
			checkError = true;//be ready to show one error
		}
		else {//if char2 isn't empty
			if (checkError) {//if we can show an error
				cout << "*** Move was attempted, but couldn't be completed. ***" << endl; //display an error message
				checkError = false;//allow no more errors to be show til checkError is true again
			}
		}
	}
}

//FindRowWithLetter will look through all the rows and return the row with the letter in it, making sure there's more than one
int FindRowWithLetter(char letter) {

	//the following if else block determins the row in which the letter is in
	if (p1 == letter && p2 == letter || p2 == letter && p3 == letter || p3 == letter && p4 == letter || p4 == letter && p5 == letter || p5 == letter && p6 == letter) {
		return 1;
	}
	else if (p7 == letter && p8 == letter || p8 == letter && p9 == letter || p9 == letter && p10 == letter || p10 == letter && p11 == letter || p11 == letter && p12 == letter) {
		return 2;
	}
	else if (p13 == letter && p14 == letter || p14 == letter && p15 == letter || p15 == letter && p16 == letter || p16 == letter && p17 == letter || p17 == letter && p18 == letter) {
		return 3;
	}
	else if (p19 == letter && p20 == letter || p20 == letter && p21 == letter || p21 == letter && p22 == letter || p22 == letter && p23 == letter || p23 == letter && p24 == letter) {
		return 4;
	}
	else if (p25 == letter && p26 == letter || p26 == letter && p27 == letter || p27 == letter && p28 == letter || p28 == letter && p29 == letter || p29 == letter && p30 == letter) {
		return 5;
	}
	else if (p31 == letter && p32 == letter || p32 == letter && p33 == letter || p33 == letter && p34 == letter || p34 == letter && p35 == letter || p35 == letter && p36 == letter) {
		return 6;
	}
	else {
		return 0;
	}
}

//FindColumnWithLetter will look through all the columns and return the column with the letter in it, making sure there's more then one
int FindColumnWithLetter(char letter) {

	//the following if else block determins the row in which the letter is in
	if (p1 == letter && p7 == letter || p7 == letter && p13 == letter || p13 == letter && p19 == letter || p19 == letter && p25 == letter || p25 == letter && p31 == letter) {
		return 1;
	}
	else if (p2 == letter && p8 == letter || p8 == letter && p14 == letter || p14 == letter && p20 == letter || p20 == letter && p26 == letter || p26 == letter && p32 == letter) {
		return 2;
	}
	else if (p3 == letter && p9 == letter || p9 == letter && p15 == letter || p15 == letter && p21 == letter || p21 == letter && p27 == letter || p27 == letter && p33 == letter) {
		return 3;
	}
	else if (p4 == letter && p10 == letter || p10 == letter && p16 == letter || p16 == letter && p22 == letter || p22 == letter && p28 == letter || p28 == letter && p34 == letter) {
		return 4;
	}
	else if (p5 == letter && p11 == letter || p11 == letter && p17 == letter || p17 == letter && p23 == letter || p23 == letter && p29 == letter || p29 == letter && p35 == letter) {
		return 5;
	}
	else if (p6 == letter && p12 == letter || p12 == letter && p18 == letter || p18 == letter && p24 == letter || p24 == letter && p30 == letter || p30 == letter && p36 == letter) {
		return 6;
	}
	else {
		return 0;
	}

}

// MoveCarHorizontal takes the letter we are looking for and the row of letters and the ammount to move by and moves the car that amount.
// Positive amount indicates right negative indicates left
void MoveCarHorizontal(char realLetter, char& letter1, char& letter2, char& letter3, char& letter4, char& letter5, char& letter6, int amount) {

	if (amount > 0) {//if the user wants to move right

		while (amount > 0) {//check if we still have to move

			if (letter6 == realLetter) {//since we are moving right, check to see if the car is in the rightmost position
				cout << "*** Move was attempted, but couldn't be completed. ***" << endl;//if it is, display error
				return; // end the function
			}

			//the next few function call will iterate through the row from right to left until it finds the car we want to move
			//then it will move the car once to the right
			MovePair(letter5, letter6, realLetter);
			MovePair(letter4, letter5, realLetter);
			MovePair(letter3, letter4, realLetter);
			MovePair(letter2, letter3, realLetter);
			MovePair(letter1, letter2, realLetter);
			amount--;//decrement the move counter
		}
	}
	else {//if the user wants to move to the left

		while (amount < 0) {//amount will be negative and check if we still have to move

			if (letter1 == realLetter) {//see if the car is in the leftmost position
				cout << "*** Move was attempted, but couldn't be completed. ***" << endl;
				return;
			}

			//iterate through the row until you find the car and move it one space left if possible
			MovePair(letter2, letter1, realLetter);
			MovePair(letter3, letter2, realLetter);
			MovePair(letter4, letter3, realLetter);
			MovePair(letter5, letter4, realLetter);
			MovePair(letter6, letter5, realLetter);
			amount++;//amount is negative so we have to increment
		}
	}
}

// MoveCarVertical takes the letter we are looking for and the column of letters and the amount to move by and moves the car that amount.
// Positive amount indicates right negative indicates left
void MoveCarVertical(char realLetter, char& letter1, char& letter2, char& letter3, char& letter4, char& letter5, char& letter6, int amount) {

	if (amount > 0) {//if the user wants to move up

		while (amount > 0) {//check if we still have to move

			if (letter1 == realLetter) {//since we are moving up, check to see if the car is in the upmost position
				cout << "*** Move was attempted, but couldn't be completed. ***" << endl;//if it is, display error
				return; // end the function
			}

			//the next few function call will iterate through the column from up to down until it finds the car we want to move
			//then it will move the car once up
			MovePair(letter2, letter1, realLetter);
			MovePair(letter3, letter2, realLetter);
			MovePair(letter4, letter3, realLetter);
			MovePair(letter5, letter4, realLetter);
			MovePair(letter6, letter5, realLetter);
			amount--;//decrement the move counter
		}
	}
	else {//if the user wants to move down

		while (amount < 0) {//amount will be negative and check if we still have to move

			if (letter6 == realLetter) {//see if the car is in the downmost position
				cout << "*** Move was attempted, but couldn't be completed. ***" << endl;
				return;
			}

			//iterate through the column until you find the car and move it one space down if possible
			MovePair(letter5, letter6, realLetter);
			MovePair(letter4, letter5, realLetter);
			MovePair(letter3, letter4, realLetter);
			MovePair(letter2, letter3, realLetter);
			MovePair(letter1, letter2, realLetter);
			amount++;//amount is negative so we have to increment
		}
	}
}

//TakeInput will ask the user for input and look for x or - to exit and reset. If the input is different the program will attempt to 
//process the input and make a move
void TakeInput() {
	//static int for persistence whenever this function is called
	static int moves = 1;

	//input variables
	char car;
	char direction;
	int amount;

	cout << moves << ".Please enter your move -> "; //print the number of moves and ask the user for his move
	cin >> car;//do car first to check for x or - leave direction and amount for later

	if (car == 'x' || car == 'X') {//first look to see if the users wants to quit
		Exit();
	}
	else if (car == '-') {
		ResetBoard();//reset the board if the user wants to
		moves = 1;//resets the moves
		cout << "Board Reset!" << endl;
		return;//exit the function and main will loop and ask the user for his move again
	}

	cin >> direction >> amount;//now take in the other parameters
	car = toupper(car);//make car capital
	direction = toupper(direction);//make direction capital
	if (amount > 5 || amount < -5) {//if the user put anything greater then the board
		amount = 5;//reduce amount to 5
	}

	if (direction == 'D' || direction == 'L') {//if direction is left or up
		amount = -amount;//make amount negative assuming the user inputs a positive
	}

	if (direction == 'R' || direction == 'L') {//if direction is left or right
		int row = FindRowWithLetter(car);//find the row the car is in

										 //use the switch statment to call MoveCarHorizontal with correct parameters
		switch (row)
		{
		case 1: MoveCarHorizontal(car, p1, p2, p3, p4, p5, p6, amount);//row 1									
			break;
		case 2: MoveCarHorizontal(car, p7, p8, p9, p10, p11, p12, amount);// row 2 and so on
			break;
		case 3: MoveCarHorizontal(car, p13, p14, p15, p16, p17, p18, amount);
			break;
		case 4: MoveCarHorizontal(car, p19, p20, p21, p22, p23, p24, amount);
			break;
		case 5: MoveCarHorizontal(car, p25, p26, p27, p28, p29, p30, amount);
			break;
		case 6: MoveCarHorizontal(car, p31, p32, p33, p34, p35, p36, amount);
			break;
		default:
			cout << "*** Move was attempted, but couldn't be completed. ***" << endl;//error if the car they want to move left or right is in a column
			break;
		}
	}
	else if (direction == 'U' || direction == 'D') {//if the direction is up or down
		int column = FindColumnWithLetter(car);//find the column of the car
											   //use the switch statment to call MoveCarHorizontal with correct parameters
		switch (column)
		{
		case 1: MoveCarVertical(car, p1, p7, p13, p19, p25, p31, amount);
			break;
		case 2: MoveCarVertical(car, p2, p8, p14, p20, p26, p32, amount);
			break;
		case 3: MoveCarVertical(car, p3, p9, p15, p21, p27, p33, amount);
			break;
		case 4: MoveCarVertical(car, p4, p10, p16, p22, p28, p34, amount);
			break;
		case 5: MoveCarVertical(car, p5, p11, p17, p23, p29, p35, amount);
			break;
		case 6: MoveCarVertical(car, p6, p12, p18, p24, p30, p36, amount);
			break;
		default:
			cout << "*** Move was attempted, but couldn't be completed. ***" << endl;
			break;
		}
	}
	else {//if you couldn't find the car in either direction
		cout << "Error: Check your input " << car << direction << amount << " not found." << endl; //print an error
	}
	moves++;//after you moved increment moves
}

int main() {
	//Display the Header
	cout << "Program #2: Traffic Game" << endl;
	cout << "Author : Phillip Sopt" << endl;
	cout << "Lab : Tues 9am" << endl;
	cout << "TA : Moumita Samanta" << endl;
	cout << "Date : Oct 2, 2017" << endl;
	cout << "System : C++ in Visual Studio Community 2017" << endl;
	cout << endl << endl;

	DisplayInstuctions();
	DisplayBoard();
	//while loop to keep the game in loop
	while (true) {
		TakeInput();//take in the players input and move the car
		DisplayBoard();
		if (p18 == 'R') {//check to see if the user is near the exit and wins
			cout << "***********YOU WIN***************" << endl;
			Exit();
		}
	}
	return 0;
}