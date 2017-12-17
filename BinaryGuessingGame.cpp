/* ------------------------------------------------
* 0/1 Guessing Game
*
* Class: CS 141, Fall 2017.  Tues 9am lab
* System: Windows 10, Visual Studio Community 2017
* Author: Phillip Sopt
* TA:  Moumita Samanta
*
* Description: This program is designed as a game where the computer tries to guess your input based on your past inputs.
* -------------------------------------------------
*/

#include<iostream>//used for cout and cin
#include<iomanip>//used for drawing the table
#include<string>//used for the DisplayScore

using namespace std;

int const ROWS = 8;//constant for the rows in the experience table
int const COLUMNS = 2;//constant for the columns in the experience table
int const CHAR_ZERO = 48;//constant char so i can switch between char and int easier

//DisplayHeader will display my info
void DisplayHeader() {
	//Display the Header
	cout << "Program #3 : 0/1 Guessing Game" << endl;
	cout << "Author : Phillip Sopt" << endl;
	cout << "Lab : Tues 9am" << endl;
	cout << "TA : Moumita Samanta" << endl;
	cout << "Date : Oct 7, 2017" << endl;
	cout << "System : C++ in Visual Studio Community 2017" << endl;
}

//DisplayTable will display the table with the pastMoves and the forcast
void DisplayTable(int table[ROWS][COLUMNS], string pastMoves, int forcast, string binaryName[ROWS]) {
	cout << setw(10) << "0" << setw(5) << "1" << endl;
	cout << setw(10) << "---" << setw(5) << "---" << endl;
	for (int i = 0; i < 8; i++)//loop through the binaryName array
	{
		cout << setw(2) << " " << binaryName[i] << setw(5) << table[i][0] << setw(5) << table[i][1] << endl;//display the current experience row
	}
	cout << endl;
	cout << setw(5) << " " << "Past Input: " << pastMoves << "   " << "Forcast: " << forcast << endl;//display the past input and forcast
}


//DisplayScore will take in the score and display the leader board with that score.
void DisplayScore(int score) {
	string arrowLine = "";//this is for the output line with the pointing arrow for the score
	cout << "Computer        Human" << endl;
	cout << "x--------x++++++++x" << endl;

	//normalize the score and loop that many times to add spaces to the arrowLine
	score += 9;
	while (score > 0)
	{
		arrowLine.append(" ");//add spaces to arrowLine to position the arrow
		score--;
	}
	arrowLine.append("^");
	cout << arrowLine << endl;//output the string with the arrow in it.
}

//DisplayInstructions displays the intructions to the game
void DisplayInstructions() {
	cout << "Welcome to the 0 / 1 guessing game!" << endl;

	cout << "Enter 0 or 1, trying to outsmart the computer, which is going to\n"
		<< "attempt to forecast your guesses.On each move the score indicator\n"
		<< "moves to the right if the computer guesses correctly, and moves\n"
		<< "to the left if the computer does not.\n"
		<< "Enter 't' to toggle displaying the data table.\n"
		<< "Enter 'r' to reset the board\n"
		<< "Enter 'x' to exit.\n"
		<< "Good luck, you'll need it!" << endl;
}

//Opposite takes the input (0 or 1) and gives the reverse (1 or 0)
int Opposite(int binNumber) {
	return (binNumber == 1) ? 0 : 1;//if input is 1 then return 0 and vice versa
}

//FindInExperieceTable will look through the binaryId table and return the position of the matched strHistory
int FindInExperienceTable(string binaryId[ROWS], string pastInput) {
	//look through the binaryId
	for (int i = 0; i < ROWS; i++)
	{
		if (binaryId[i] == pastInput) {//if the binary input is == to the past input then return it's position
			return i;//return the row number
		}
	}
	return -1;//error
}

//UpdateTable will take the row and column and and 1 to that value in the experience table
void UpdateTable(int row, int column, int (&table)[ROWS][COLUMNS]) {
	table[row][column]++;//add one to the experience value
}

//GenerateGuess will use the table, moves, and pastInput to decided on when the currentGuess will be
void GenerateGuess(int& moves, int table[ROWS][COLUMNS], string pastInput, int& currentGuess, int& score, int position) {
	if (moves > 3) {//if we have strHistory
		//look in at the position row of the experience table 
		if (table[position][0] > table[position][1]) {//if 0 is more probable then 1 to appear, pick 0
			currentGuess = 0;
		}
		else if (table[position][0] < table[position][1]) {////if 1 is more probable then 0 to appear, pick 1
			currentGuess = 1;
		}
		else {//if there's a tie choose the humans previous input
			currentGuess = Opposite(pastInput[3]);
		}
	}
	else if(moves > 1) {//if we don't have enough history to look at then choose the humans previous input
		currentGuess = Opposite(pastInput[3]);
	}
}

//DisplayTeaseMessage will look at the score and tease you with an appropiate message
void DisplayTeaseMessage(int score){
	if (score == 5) {//if the human is winning
		cout << ">>>You've got to kidding, how are you winning?!<<<" << endl;
	}
	else if (score == -5) {//if the computer is winning
		cout << ">>>Suck an egg squishy, scilion rules!<<<" << endl;
	}
}

//SetExperienceTable will alter the computers experienceTable to the users preference
void SetExperienceTable(int (&table)[ROWS][COLUMNS], string binaryID[ROWS]) {
	cout << "Enter in 16 numbers to set in the experience table: " << endl;
	for (auto &a : table){//for range loop to set the table
		for (int &x : a) {
			cin >> x;//change the current experience value
		}
	}
}

//Main
int main() {
	DisplayHeader();//show my name and lab and TA and all that

	int numScore = 0;//the score
	int experienceTable[ROWS][COLUMNS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//an array storing only the values for each past input
	int currentGuess = 0;//the current computers guess
	int numMoves = 1;//the number of numMoves that have gone by

	string strHistory = "000";//a string to hold the strHistory values
	char charUserInput;//users input
	
	bool displayTable = false;//toggle flag for the table to display
	
	//binaryId is an array that stores the '010' history, it's used for searching the experienceTable
	string binaryId[] = { "000", "001", "010", "011", "100", "101", "110", "111" };

	//main loop
	while (numScore<9 && numScore >-9) {//if the score is within -9 and 9
		if (displayTable){//if the flag is on to show the table
			GenerateGuess(numMoves,experienceTable,strHistory,currentGuess,numScore,FindInExperienceTable(binaryId,strHistory));//have the computer guess based on the parameters
			DisplayTable(experienceTable, strHistory, currentGuess, binaryId); //show the table
		}

		//ask the user for input
		cout << numMoves << ". Your input:" << endl;
		cin >> charUserInput;

		if (charUserInput == 't' || charUserInput == 'T') {//if the user entered t
			displayTable = !displayTable;//toggle the table
			continue;//start the loop at the beginning
		}
		else if (charUserInput == 'x' || charUserInput == 'X') {//if the user entered x
			exit(0);//exit the program
		}
		else if (charUserInput == 'r' || charUserInput == 'R') {//if the user entered r
			SetExperienceTable(experienceTable, binaryId);//then ask for the user to enter the experience table values
			strHistory = "000";//resets the strHistory
			numMoves = 1;//resets the numMoves
			continue;//start the loop over
		}
		else if(charUserInput == '0' || charUserInput == '1') {
			int position = FindInExperienceTable(binaryId, strHistory);//find the place we want to look in the experience table using the binaryId
			GenerateGuess(numMoves, experienceTable, strHistory, currentGuess, numScore, position);//have the computer guess based on the parameters

			if (currentGuess == charUserInput-CHAR_ZERO) {//if the computer guessed the answer
				numScore--;//give the computer a point
				cout << "Computer Gets A Point!" << endl;
			}
			else {
				numScore++;//give the human a point
				cout << "Human Gets A Point!" << endl;
			}
			DisplayTeaseMessage(numScore);//show the tease message

			if (numMoves >= 3) {//wait for 3 numMoves
				UpdateTable(position, charUserInput-CHAR_ZERO, experienceTable);//update the experience table useing the position we know and the users answer
			}
			strHistory.push_back(charUserInput);//adds the usersInput in the back of the string
			strHistory.erase(0, 1);//erase the first char
			DisplayScore(numScore);//Show the score now that the computer has guessed
		}
		else {//if the user entered anything else
			cout << "ERROR: check your input" << endl;
			continue;
		}
		numMoves++;//increment the numMoves
	}

	if (numScore == 9) {//if the player won
		cout << "Wow you actually won.....HOW?" << endl;
	}
	else {//if the computer won
		cout << "I told you I would win, silicon is supreme!!" << endl;
	}

	return 0;
}