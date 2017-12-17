/*
 Program: #5, Boggle Generator

 Class: CS 141, Fall 2017.  Tues 9am lab
 System: Windows 10, 2017 Visual Studio Community
 Author: Phillip Sopt
 TA:  Moumita Samanta

 Description: This program is designed to generate the most optimized boggle table.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <cctype>     // Allows using the tolower() function
#include <ctime>      // Timer functions
#include <cstring>    // For strlen
#include <vector>	  // For storing the adjecent Containers
#include <string>	  // For finding the word in the board
#include <iomanip>

using namespace std;

// Global constants
const char DictionaryFileName[] = "dictionary.txt";
const int MaxNumberOfWords = 263533; // Number of dictionary words
const int HalfMaxWords = 131766;
const int MinWordLength = 3;         // Minimum dictionary word size to be stored
const int MaxWordLength = 16;	     // Max word size.  Add 1 for null
const int MaxUserInputLength = 81;   // Max user input length
const int NumberOfLetters = 26;      // Letters in the alphabet
const int TotalSecondsToPlay = 60;   // Total number of seconds to play per board
const int SizeOfBoard = 16;
const int BoardRow = 4;
const int BoardCol = 4;

//The Container Class is used to store each individual char on the board in a more versitile Container Obj
//with functions that can be used to determine paths between letters
class Container 
{
public:
	Container(char, vector<Container*>);			//constructor
	char getLetter() { return charLetter; }
	void setLetter(char);							//mutator for charLetter
	int getPath(string, int, vector<Container*>);
	void addAdjacent(Container*);

private:
	char charLetter;				//the letter that is store at this Container
	vector<Container*> adjacent;	//all the Containers that are adjacent to this one
};

//Contructor for the Container obj
Container::Container(char charLetter = 'n', vector<Container*> adjacent = {})
{
	this->charLetter = charLetter;
	this->adjacent = adjacent;
}

//addAdjacent is used for adding another adjacenet Container to this Containers adjacent array
void Container::addAdjacent(Container* newContainer)
{
	//if it's not a nullptr then go ahead and add it to the adjacent list
	if (newContainer != nullptr)
	{
		adjacent.push_back(newContainer);
	}
}

//findSameLetter will look through the adjacent list and charToFind exactly the letter we are looking for
vector<Container*> findSameLetters(char charToFind, vector<Container*> list, vector<Container*> visited)
{
	vector<Container*> result;		//store the result
	for (int i = 0; i < list.size(); i++)
	{
		//look through the list and store the same letters in the result
		if (charToFind == list[i]->getLetter())
		{
			//look to see if this letter has been visited already
			bool visitedFlag = false;
			for (int j = 0; j < visited.size(); j++)
			{
				if (visited[j] == list[i])
				{
					visitedFlag = true;
					break;
				}
			}
			//if we didn't visit it the add it to the result
			if (!visitedFlag)
			{
				result.push_back(list[i]);
			}
		}
	}
	return result;	//return the result
}

//setLetter is a simple mutator for charLetter
void Container::setLetter(char charLetter)
{
	this->charLetter = charLetter;
}

//getPath is a recursive function that will return the length of the path that is walked along the letters of the board without repeating itself
//from the start of the string to the end of it
int Container::getPath(string word, int index, vector<Container*> visited = {})
{
	//look to see if we have anymore steps to take
	if (index >= word.length())
	{
		return 0;
	}

	vector<Container*> lettersNeeded = findSameLetters(word[index], adjacent, visited);		//a least of possible letters to jump to next
																							//if we have more then one route to take
	if (lettersNeeded.size() > 1 && index < word.length() - 1)
	{
		int tempIndex = index;	//store a temporary index to avoid contaminating the parent one
		vector<Container*> tempVisited = visited;		//make a copy of the visited array
		int tempResult = 0;		//make a result to compare the multiple paths

								//loop through the multiple path option and determine which one gets you to the end
		for (int i = 0; i < lettersNeeded.size(); i++)
		{
			tempVisited.push_back(lettersNeeded[i]);
			tempResult = 1 + lettersNeeded[i]->getPath(word, 1 + tempIndex, tempVisited);		//recursive call to check the current possible path
			if (tempResult + index >= word.length())		//if this path leads to the end
			{
				return tempResult;		//return the result and start climbing back up the recursive ladder
			}
		}
		return 0;		//if you looped through all your options and none lead to the ending start climbing back up the recursive ladder
	}
	else if (lettersNeeded.size() > 0)//if we have exactly one routes
	{
		visited.push_back(lettersNeeded[0]);
		return 1 + lettersNeeded[0]->getPath(word, ++index, visited);	//recursive call to the next Container
	}
	else// if we have no more routes and we didn't reach the word length
	{
		return 0;
	}
}

//displayHeader will display my info
void displayHeader() 
{
	//Display the Header
	cout << "Program #5 : Boggle" << endl;
	cout << "Author : Phillip Sopt" << endl;
	cout << "Lab : Tues 9am" << endl;
	cout << "TA : Moumita Samanta" << endl;
	cout << "Date : Nov 7, 2017" << endl;
	cout << "System : C++ in 2017 Visual Studio Community" << endl;
	cout << endl;
}

 //--------------------------------------------------------------------------------
 // Display instructions
void displayInstructions()
{
	printf("Welcome to the game of Boggle, where you play against the clock   \n");
	printf("to see how many words you can find using adjacent letters on the  \n");
	printf("board.  Each letter can be used only once for a given word.       \n");
	printf("  \n");
	printf("When prompted to provide input you may also:                      \n");
	printf("     Enter 'r' to reset the board to user-defined values.         \n");
	printf("     Enter 's' to solve the board and display all possible words. \n");
	printf("     Enter 't' to toggle the timer on/off.                        \n");
	printf("     Enter 'x' to exit the program.                               \n");
	printf("  \n");
}//end displayInstructions()

 //---------------------------------------------------------------------------
 // Read in dictionary
 //    First dynamically allocate space for the dictionary.  Then read in words
 // from file.  Note that the '&' is needed so that the new array address is
 // passed back as a reference parameter.
void readInDictionary(
	char ** &dictionary,                      // dictionary words
	long int &numberOfWords)                  // number of words stored
{
	// Allocate space for large array of C-style strings
	dictionary = new char*[MaxNumberOfWords];

	// For each array entry, allocate space for the word (C-string) to be stored there
	for (int i = 0; i < MaxNumberOfWords; i++) {
		dictionary[i] = new char[MaxWordLength + 1];
		// just to be safe, initialize C-strings to all null characters
		for (int j = 0; j < MaxWordLength; j++) {
			dictionary[i][j] = '\0';
		}//end for (int j=0...
	}//end for (int i...

	 // Now read in the words from the file
	ifstream inStream;                 // declare an input stream for my use
	numberOfWords = 0;                   // Row for the current word
	inStream.open(DictionaryFileName);
	// make sure file open was OK
	if (!inStream.is_open()) 
	{ 
		cout << "dictionary.txt is NOT OPEN/FOUND" << endl;
		exit(-1); 
	}      

									// Keep repeating while input from the file yields a word
	char theWord[81];    // declare input space to be clearly larger than largest word
	while (inStream >> theWord) {
		int wordLength = (int)strlen(theWord);
		if (wordLength >= MinWordLength && wordLength <= MaxWordLength) {
			strcpy(dictionary[numberOfWords], theWord);
			// increment number of words
			numberOfWords++;
		}
	}//end while( inStream...

	cout << "The dictionary total number of words is: " << MaxNumberOfWords << endl;
	cout << "Number of words of the right length is:  " << numberOfWords << endl;

	// close the file
	inStream.close();
}//end readInDictionary()

 //--------------------------------------------------------------------------------------
 // Use binary search to look up the search word in the dictionary array, returning index
 // if found, -1 otherwise
int binarySearch(const char searchWord[MaxWordLength + 1], // word to be looked up
	char **dictionary)               // the dictionary of words
{
	int low, mid, high;     // array indices for binary search
	int searchResult = -1;  // Stores index of word if search succeeded, else -1

							// Binary search for word
	low = 0;
	high = MaxNumberOfWords - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		// searchResult negative value means word is to the left, positive value means
		// word is to the right, value of 0 means word was found
		searchResult = strcmp(searchWord, dictionary[mid]);
		if (searchResult == 0) {
			// Word IS in dictionary, so return the index where the word was found
			return mid;
		}
		else if (searchResult < 0) {
			high = mid - 1; // word should be located prior to mid location
		}
		else {
			low = mid + 1; // word should be located after mid location
		}
	}

	// Word was not found
	return -1;
}//end binarySearch()

 //---------------------------------------------------------------------------
 // Get random character
 //    Find random character using a table of letter frequency counts.
 // Iterate through the array and charToFind the first position where the random number is
 // less than the value stored.  The resulting index position corresponds to the
 // letter to be generated (0='a', 1='b', etc.)
char getRandomCharacter()
{
	// The following table of values came from the frequency distribution of letters in the dictionary
	float letterPercentTotals[NumberOfLetters] = {
		0.07680,  //  a
		0.09485,  //  b
		0.13527,  //  c
		0.16824,  //  d
		0.28129,  //  e
		0.29299,  //  f
		0.32033,  //  g
		0.34499,  //  h
		0.43625,  //  i
		0.43783,  //  j
		0.44627,  //  k
		0.49865,  //  l
		0.52743,  //  m
		0.59567,  //  n
		0.66222,  //  o
		0.69246,  //  p
		0.69246,  //  q
		0.76380,  //  r
		0.86042,  //  s
		0.92666,  //  t
		0.95963,  //  u
		0.96892,  //  v
		0.97616,  //  w
		0.97892,  //  x
		0.99510,  //  y
		1.00000 }; //  z

				   // generate a random number between 0..1
				   // Multiply by 1.0 otherwise integer division truncates remainders
	float randomNumber = 1.0 * rand() / RAND_MAX;

	// Find the first position where our random number is less than the
	// value stored.  The index corresponds to the letter to be returned,
	// where 'a' is 0, 'b' is 1, and so on.
	for (int i = 0; i<NumberOfLetters; i++) {
		if (randomNumber < letterPercentTotals[i]) {
			// we found the spot.  Return the corresponding letter
			return (char) 'a' + i;
		}
	}

	// Sanity check
	cout << "No alphabetic character generated.  This should not have happened. Exiting program.\n";
	exit(-1);
	return ' ';   // should never get this
}//end getRandomCharacter

//displayBoard will take the board that is storing all the container objs and print their character values
void displayBoard(Container board[BoardCol][BoardRow])
{
	//it's pretty self explanitory, loop through the board, get the Container at i,j and print it's character
	for (int i = 0; i < BoardCol; i++)
	{
		for (int j = 0; j < BoardRow; j++)
		{
			cout << board[i][j].getLetter() << " ";
		}
		cout << endl;
	}
}

//clampBoard is used to make sure that for values of index are valid
Container* clampBoard(Container* object, int col, int row)
{
	//if the row or col is outside of board's range then return a nullptr so
	//addAdjacent() knows not to add that Container
	if ((row < 0 || row > BoardRow - 1) || (col < 0 || col > BoardCol -1))
	{
		return nullptr;
	}
	else
	{
		return object;
	}	
}

void randomizeBoard(Container(&board)[BoardCol][BoardRow])
{
	//first populate the board
	for (int i = 0; i < BoardCol; i++)
	{
		for (int j = 0; j < BoardRow; j++)
		{
			board[i][j] = Container(getRandomCharacter());
		}
	}
}

//populateBoard is going to be used to generate the random chars and store them as Container Objs in the board
void populateBoard(Container (&board)[BoardCol][BoardRow])
{
	randomizeBoard(board);

	//Then loop through the board again and setup the adjacent Containers for each Container
	for (int i = 0; i < BoardCol; i++)
	{
		for (int j = 0; j < BoardRow; j++)
		{
			//add the containers in the cardinal directions (right, left, up, down)
			board[i][j].addAdjacent(clampBoard(&board[i + 1][j], i + 1, j));		
			board[i][j].addAdjacent(clampBoard(&board[i - 1][j], i - 1, j));		
			board[i][j].addAdjacent(clampBoard(&board[i][j + 1], i, j + 1));
			board[i][j].addAdjacent(clampBoard(&board[i][j - 1], i, j - 1));

			//add the Containers on the diagonals
			board[i][j].addAdjacent(clampBoard(&board[i - 1][j - 1], i - 1, j - 1));
			board[i][j].addAdjacent(clampBoard(&board[i - 1][j + 1], i - 1, j + 1));
			board[i][j].addAdjacent(clampBoard(&board[i + 1][j + 1], i + 1, j + 1));
			board[i][j].addAdjacent(clampBoard(&board[i + 1][j - 1], i + 1, j - 1));
		}
	}
}

//resetBoard takes in the board and asks the user for input to fill up the board
void resetBoard(Container (&board)[BoardCol][BoardRow])
{
	cout << "Enter 16 characters for a new board :" << endl;

	//loop through the board and ask the user for an input for that position
	for (int i = 0; i < BoardRow; i++)
	{
		for (int j = 0; j < BoardCol; j++)
		{
			char c;
			cin >> c;
			board[i][j].setLetter(c);
		}
	}
}

//exitProgram will exit the porgram
void exitProgram()
{
	cout << "Exiting Program..." << endl;
	exit(0);
}

//foundInBoard will loop through the board and determin if theres enought characters to construct the word
bool foundInBoard(string board, char word[])
{
	for (int i = 0; i < strlen(word); i++)
	{
		if (strchr(&board[0], word[i]) == nullptr)
		{
			return false;
		}
	}
	return true;
}

//startSearch looks for the first char that is == to the userInput[0]
//returns the length of the word if found and returns -1 if not found
void startSearch(Container (board)[BoardCol][BoardRow], int wordPos, string userInput, int &datum, int length)
{
	if (length < 11)
	{
		int pathLength = 0;
		//now charToFind the starting point in the board and call the recursive getPath() on that first letter
		for (int i = 0; i < BoardCol; i++)
		{
			for (int j = 0; j < BoardRow; j++)
			{
				//if we found a starting letter
				if (board[i][j].getLetter() == userInput[0])
				{
					//call the recursive getPath() and start trying to build the word
					vector<Container*> first;
					first.push_back(&board[i][j]);
					pathLength = board[i][j].getPath(userInput, 1, first) + 1;		//getpath()+1 to count the first letter being found
					if (pathLength == length)		//if you found a path that is == to the word's length
					{
						datum += 1;
						return;
					}
				}
			}
		}
	}
}

//getScore will take in the length of the found word and return it's proper score
int getScore(int length)
{
	if (length >= 6) { return length; }		//6 -> length
	else if (length >= 5) { return 4; }		//5 -> 3
	else if (length >= 4) { return 2; }		//4 -> 2
	else if (length >= 3) { return 1; }		//3 -> 1
	else if (length < 3) { return 0; }		//<3 -> 0
}

//resetData will clear the data table back to empty
void resetData(int(&data)[4][14])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			if (i == 0)
			{
				data[0][j] = j + 3;			//make the first columns display the length of the string
			}
			else
			{
				data[i][j] = 0;				//set everything else to 0
				data[2][j] = getScore(data[0][j]);		//set the 3 column to the point equivalents
			}
		}
	}
}

//splitLoop lets you start looking for solutions in the middle of the dictionary as indicated by int start and ends at int end
void splitLoop(int start, int end, Container(board)[BoardCol][BoardRow], char **dictionary, int(&data)[4][14], char boardAlphabet[16])
{
	for (int i = start; i < end; i++)
	{
		int length = strlen(dictionary[i]);
		if (foundInBoard(boardAlphabet, dictionary[i]))
		{
			startSearch(board, 0, dictionary[i], data[1][length - 3], length);		//start looking for a solution to this word dictionary[i]
		}
	}
}

//generateSolutions will loop through the dictionary and just try to get all the paths it can and display them
void generateSolutions(Container(&board)[BoardCol][BoardRow], char** dictionary, int (&data)[4][14])
{
	resetData(data);
	//make an alphabet out of the board to search for the chars in the word
	char boardAlphabet[16];
	for (int i = 0; i < BoardCol; i++)
	{
		for (int j = 0; j < BoardRow; j++)
		{
			boardAlphabet[(i * 4) + j] = board[i][j].getLetter();
		}
	}
	splitLoop( 0, MaxNumberOfWords, board, dictionary, data, boardAlphabet);
}

//getTotalScore will generate a score based on the information in data
int getTotalScore(int(&data)[4][14])
{
	int totalScore = 0;
	for (int i = 0; i < 14; i++)
	{
		data[3][i] = data[2][i] * data[1][i];
		totalScore += data[3][i];
	}
	return totalScore;
}

//optimizeRandomGenerate will take the board, pick a random place on the board and check to see if this new board is better then the old
void optimizeRandomGenerate(Container(&board)[BoardCol][BoardRow], int &totalScore, char **dictionary, int (&data)[4][14])
{
	char copy;
	int oldScore = totalScore;
	int x = rand() % 4;
	int y = rand() % 4;
	copy = board[x][y].getLetter();
	board[x][y].setLetter(getRandomCharacter());
	generateSolutions(board, dictionary, data);
	totalScore = getTotalScore(data);
	if (oldScore > totalScore)
	{
		board[x][y].setLetter(copy);
		totalScore = oldScore;
	}
}

//optimizeRandomSwitch will pick a random place on the board and switch it with another place on the board, then compare this board to the old one
void optimizeRandomSwitch(Container(&board)[BoardCol][BoardRow], int &totalScore, char **dictionary, int(&data)[4][14])
{
	char copy, copy1;
	int oldScore = totalScore;
	int x = rand() % 4;
	int y = rand() % 4;
	int x1 = rand() % 4;
	int y1 = rand() % 4;
	copy = board[x][y].getLetter();
	copy1 = board[x1][y1].getLetter();
	board[x][y].setLetter(copy1);
	board[x1][y1].setLetter(copy);
	generateSolutions(board, dictionary, data);
	totalScore = getTotalScore(data);
	if (oldScore > totalScore)
	{
		board[x][y].setLetter(copy);
		board[x1][y1].setLetter(copy1);
		totalScore = oldScore;
	}
}

 //---------------------------------------------------------------------------
int main()
{
	srand(777777);				//set the seed to a good starting board
	
	// declare variables
	char **dictionary;              // 2d array of dictionary words, dynamically allocated
	long int numberOfWords;         // how many words actually found in dictionary
	string userInput;				//string to store user Input
	Container board[BoardCol][BoardRow];	//used to store the entire board
	int wordLength = 0;						//the length of the found word
	bool timerFlag = false;					//flag for the timer to show, the program will start with the timer on
	bool wordsFound[MaxNumberOfWords];		//bool map of the found words
	int score = 0;							//the score of the players
	int numMoves = 1;						//the number of moves
	int data[4][14];						//data to store all of the score bewteen the different lengths

	//initialize data
	resetData(data);

	// Declare a variable to hold a time, and get the current time
	time_t startTime = time(NULL);

	//display info and instructions
	displayHeader();
	//displayInstructions();

	// read in dictionary.  numberOfWords returns the actual number of words found
	readInDictionary(dictionary, numberOfWords);

	//populate the board
	populateBoard(board);
	cout << endl;

	int elapsedSeconds = 0;
	int totalScore = 0;
	//resetBoard(board);
	while (true)
	{
		totalScore = 0;									//set the score back to zero
		generateSolutions(board, dictionary, data);		//populate data with the possible solutions of the current board
		
		totalScore = getTotalScore(data);				//set score according to the data
		int oldScore = totalScore;
		
		//start by randomly changing board values
		optimizeRandomGenerate(board, totalScore, dictionary, data);

		//start swaping pieces
		if (rand() % 4 == 2 && elapsedSeconds < 1000)//every now and again switch some board positions till 1000 seconds
		{
			optimizeRandomSwitch(board, totalScore, dictionary, data);
		}
		if (oldScore < totalScore)
		{
			cout << "Phillip Sopt" << endl;
			cout << "Seconds elapsed: " << elapsedSeconds << endl;
			elapsedSeconds = difftime(time(NULL), startTime);
			cout << "Board is: " << endl;
			displayBoard(board);
			int s = 6;
			cout << setw(s) << "Len" << setw(s) << "#" << setw(s) << "Pts" << setw(s) << "Score" << endl;
			for (int i = 0; i < 14; i++)
			{
				//data[3][i] = data[2][i] * data[1][i];
				cout << setw(s) << data[0][i] << setw(s) << data[1][i] << setw(s) << data[2][i] << setw(s) << data[3][i] << endl;
			}
			cout << "-----------------------------" << endl;
			cout << "Total Score: " << totalScore << endl;
		}
	}

	return 0;
}//end main()