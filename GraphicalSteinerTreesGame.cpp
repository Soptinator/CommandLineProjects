/*Program: #6, Steiner Trees
*
* Class: CS 141, Fall 2017.  Tues 9am lab
* System: Windows 10, 2017 Visual Studio Community
* Author: Phillip Sopt
* TA:  Moumita Samanta
* Description: This program was designed to be a sort of game where the player has to create a path between points by adding other points and
* drawing lines.
* NOTICE: this program will need QT Creator and Stanford STD Libraries to compile.
*/
#include <iostream>
#include "gobjects.h"   // for all graphics objects
#include "gevents.h"    // for mouse events
#include <fstream>      // For file input
#include <cmath>
#include <ctime>
#include <string>       //used with GLabel a lot
#include <vector>       //used to store the MyPoint,MyLine
using namespace std;

// Global constants
const int PointRadius = 7;

// Create a graphics window, which will be used to draw graphical shapes.
// This can be a global variable, but you may not have any other global variables.
GWindow graphicsWindow;

//displayHeader will display my info
void displayHeader() {
    //Display the Header
    cout << "Program #6 : Steiner Trees" << endl;
    cout << "Author : Phillip Sopt" << endl;
    cout << "Lab : Tues 9am" << endl;
    cout << "TA : Moumita Samanta" << endl;
    cout << "Date : Nov 25, 2017" << endl;
    cout << "System : C++ in Qt Creator, Windows 10" << endl;
}

//Corrdinate is used to store x and y coordinates instead of using int[2]
struct Coordinate{
    int x;
    int y;
};

//MyPoint is a custom class to make handling point coloring easly
class MyPoint{
public:
    MyPoint(int, int, int, string);     //constructor
    int getX(){return x;}               //simple getters
    int getY(){return y;}
private:
    GOval *point;   //oval obj containing the position and color
    int x;
    int y;
};

//MyPoint Contructor takes in all the parameters it needs to create a point
MyPoint::MyPoint(int x, int y, int radius, string color){
    this->x = x-radius;
    this->y = y-radius;
    point = new GOval( x-radius, y-radius, radius*2, radius*2);     //offset oval obj by the radius abit
    point->setFilled(true);
    point->setFillColor(color);     //set the color
    graphicsWindow.add(point);
}

//MyButton is a class designed to make handeling GRects a lot easier with member fuctions to check if a point is
//within the rect
class MyButton{
public:
    MyButton(int, int, int, int, string, string);
    int getX(){return x;}
    int getY(){return y;}
    bool isPointInside(int, int);
private:
    GRect *rect;        //rect for GRect
    GLabel *label;      //label used to store the label
    int x;
    int y;
};

//the construction of MyButton takes in parameters for both the rect and for the GLabel
MyButton::MyButton(int x, int y, int width, int height, string color, string text){
    this->x = x;
    this->y = y;
    rect = new GRect(x,y,width,height);     //create the rect
    rect->setFilled(true);
    rect->setLineWidth(0);
    rect->setFillColor(color);              //set the color
    label = new GLabel();
    label->setText(text);

    // Find size of text, to determine placement relative to enclosing rectangle
    int xOffset = (width - rect->getBounds().getWidth()) / 2;
    int yOffset = (rect->getBounds().getHeight()) / 2;
    label->setLocation( x + xOffset, y + yOffset);          //set the labels location

    //add them to the graphicsWindow
    graphicsWindow.add(rect);
    graphicsWindow.add(label);
}

//isPointInside is very simple, it returns true if x,y is inside rect
bool MyButton::isPointInside(int x, int y){
    return rect->contains(x,y);
}

//getDistance will get the distance between point x,y and fx,fy
double getDistance(int x, int y, int fx, int fy){
    return sqrt(pow(fx - x, 2) + pow(fy - y, 2));    //get the distance between fx,fy and x,y
}

//MyLine is designed to make dealing with GLine* a little easier by storing the length and start/end positions
class MyLine{
public:
    MyLine(Coordinate,Coordinate);          //default constructor
    Coordinate getStart(){ return start; }  //standard accessors
    Coordinate getEnd(){ return end; }
    double getLength(){ return length; }
private:
    GLine* line;
    Coordinate start;
    Coordinate end;
    double length;
};

//Constructor will take in two Coordinates and make a GLine and set it's width and length and add it to the graphicsWindow
MyLine::MyLine(Coordinate start, Coordinate end){
    this->start = start;
    this->end = end;
    this->length = getDistance(start.x,start.y,end.x,end.y);

    line = new GLine(start.x,start.y,end.x,end.y);
    line->setLineWidth(2);
    graphicsWindow.add(line);
}

//------------------------------------------------------------------------------------------
// Read in the sets of points information from the data file
// You muse make the pointsArray Vector a reference parameter so that read-in values are
// reflected back to the caller.
void readInputFile( vector<MyPoint> &pointsArray, int &idealScore)
{
    ifstream inputFileStream;  // declare the input file stream

    // Open input file and verify.
    // You must first:
    //    1. Outside of QtCreator go into your project's resources folder, copy readme.txt into data.txt
    //    2. Edit data.txt to have the datafile information as shown on the assignment page
    //    3. Now WITHIN QtCreator right-click on the "res" folder (under "Other files") and
    //       choose "add new" and add the new data.txt file so it shows up in your environment.
    inputFileStream.open("data.txt");
    if( !inputFileStream.is_open()) {
        cout << "Could not find input file data.txt.  Exiting..." << endl;
        exit( -1);
    }

    // Read sets of data from input file.  Each read operation reads the next set of data.
    // The first line in the input file indicates which set to use.
    int whichSetToUse;
    inputFileStream >> whichSetToUse;

    int howManyPoints;    // How many points are in the set of points
    int x, y;             // Store individual point x,y values from input file

    // Read in and discard some number of sets of points before we get the real set we will be using
    for( int i = 0; i < whichSetToUse-1; i++) {
        inputFileStream >> howManyPoints >> idealScore;
        for( int j = 0; j < howManyPoints; j++) {
            inputFileStream >> x >> y;    // Read in the points
        }
    }
    inputFileStream >> howManyPoints >> idealScore;

    // Read in and store the points information
    for( int i = 0; i < howManyPoints; i++) {
        inputFileStream >> x >> y;    // Read in the points

        // This is where you should take the points information and create points on the screen.
        // For now we will just print out the points information.
        pointsArray.push_back(MyPoint(x,y,PointRadius,"black"));
        //cout << "Point " << i << " is at " << x << "," << y << endl;
    }
}//end readInputFile()

//getCurrentScore will read the entire lineArray and determine the length of each line in the array and add the score in the result
int getCurrentScore(vector<MyLine> lineArray){
    int result = 0;
    for(int i = 0; i < lineArray.size(); i++){      //loop through the array
        result += lineArray[i].getLength();
    }
    return result;
}

//clampToPoint will return a Coordinate that contains the poistion x,y snapped to the nearest point
Coordinate clampToPoint(int x, int y, vector<MyPoint> points){
    double minDistance = 500;       //used to compare the distances between points
    Coordinate result;

    //loop through the points array
    for(int i = 0; i < points.size(); i++){
        double newDistance = getDistance(x,y,points[i].getX(),points[i].getY());    //calculate the new distance

        //if the current point is closer then the last, make it our new result point
        if(minDistance > newDistance){
            minDistance = newDistance;
            result.x = points[i].getX() + PointRadius;
            result.y = points[i].getY() + PointRadius;
        }
    }
    return result;  //return the int[2] array of the point
}

//------------------------------------------------------------------------------------------
int main()
{
    displayHeader();
    graphicsWindow.setCanvasSize( 400, 400);   // Set the size
    graphicsWindow.setWindowTitle("  psopt2  QtCreator Program 6");
    int currentScore = 0;           //the length of all the lines currently
    vector<MyPoint> pointsArray;    // Store all points that are created.
    vector<MyLine> lineArray;       // ... Create something similar to store all GLine * lines
    int idealScore = 0;             // Precomputed ideal score for each puzzle
    int scoreTolerance = 0;
    // Get the set of data to use from the input file
    readInputFile( pointsArray, idealScore);

    scoreTolerance = round((double)idealScore * 1.05);    //make scoreTolerance be 1.05% of the idealScore
    // Create the Control and message components.
    // First create the messages label at the top of the window
    GLabel *messagesLabel = new GLabel();
    messagesLabel->setText("Find the minimal spanning tree, using additional points.");
    messagesLabel->setLocation( 30,20);
    graphicsWindow.add( messagesLabel);

    //Create the length label that says "Length :"
    GLabel *lengthTopLabel = new GLabel();
    lengthTopLabel->setText("Length :");
    lengthTopLabel->setLocation( 330,360);
    graphicsWindow.add(lengthTopLabel);
    //create the length label that actually stores the number
    GLabel *lengthLabel = new GLabel();
    lengthLabel->setText(to_string(currentScore));      //convert the current score to a string
    lengthLabel->setLocation( 330,375);
    graphicsWindow.add(lengthLabel);

    //Create all the buttons using the MyButton constructor
    MyButton exitButton = MyButton(250,350,70,30,"red","Exit");
    MyButton addPointsButton = MyButton(10,350,70,30,"green","Add Points");
    MyButton drawLineButton = MyButton(90,350,70,30,"lightgray","Draw Lines");

    // Make the graphics window have focus, since all interaction will be through the window
    graphicsWindow.requestFocus();

    // Set values to use for keeping track of program modes
    const int Default = 0;
    const int DrawPoints = 1;
    const int DrawLines = 2;
    int programMode = Default;          // Modes are Default, DrawPoints, DrawLines. Start in Default mode
                                        // Change to the other modes when those buttons are pressed.
    bool buttonWasPressed = false;      // Set to false when a button is pressed.  Used to avoid drawing on buttons
                                        // immediately when they are pressed.

    int mouseX = -1;                    // Will store mouse position
    int mouseY = -1;
    char message[ 81];                  // Used to construct message to be displayed at top of window
    GLine *pLine = new GLine(0,0,0,0);  // Line to be drawn on the screen.  It starts out as a single unseen point.
    pLine->setLineWidth(2);
    graphicsWindow.add(pLine);

    GMouseEvent e;      // Stores the mouse event each time through the event loop

    //used for creating the MyLine object to push back into the vector
    Coordinate start;
    Coordinate end;

    // Main event loop.  Most of your code will go in here, to handle the different mouse events.
    while (true) {
        // Wait for a mouse event, then get the x,y coordinates
        e = waitForEvent(MOUSE_EVENT);
        // Get the current mouse x,y location
        mouseX = e.getX();
        mouseY = e.getY();

//------------- Perform actions for different kinds of mouse events --------
        if (e.getEventType() == MOUSE_PRESSED) {

            // See if Exit button was clicked by seeing if mouse press location is inside the rectangle
            if( exitButton.isPointInside(mouseX, mouseY)) {
                messagesLabel->setLabel("Exiting...");
                break;
            }
            //if user pressed on the draw line button
            else if(drawLineButton.isPointInside(mouseX, mouseY)){
                sprintf( message,"Drawing Lines mode activated, click and hold to add lines");  // print into a string
                messagesLabel->setLabel(message);
                programMode = DrawLines;        //change the program mode
                buttonWasPressed = true;        //toggle the buttonWasPressed flag to true
            }
            //if they pressed on the add points button
            else if(addPointsButton.isPointInside(mouseX, mouseY)){
                sprintf( message,"Adding Points mode activated, click to add points");  // print into a string
                messagesLabel->setLabel(message);
                programMode = DrawPoints;
                buttonWasPressed = true;
            }
            //wait for the flag to go false again
            else if(!buttonWasPressed){
                //check through all the programming modes
                if(programMode == DrawLines){
                    start = clampToPoint(mouseX,mouseY,pointsArray);    //set a start point from the mouse coords
                    pLine->setStartPoint(start.x, start.y);             //set the line to the start point
                    pLine->setEndPoint(mouseX,mouseY);                  //set the end of the line to the mouse
                }
                else if(programMode == DrawPoints){
                    pointsArray.push_back(MyPoint(mouseX,mouseY,PointRadius,"blue"));   //creates a new blue point, the MyPoint constructor does the heavy lifting
                }
            }
        }
//-----------------When mouse is Released-----------------------------
        else if( e.getEventType() == MOUSE_RELEASED) {

            //if DrawLines is active and the button wasn't just pressed
            if(programMode == DrawLines && !buttonWasPressed){
                Coordinate end = clampToPoint(mouseX,mouseY,pointsArray);   //get a endpoint
                //reset pLine
                pLine->setEndPoint(0, 0);
                pLine->setStartPoint(0, 0);
                lineArray.push_back(MyLine(start,end));         //store the constructed MyLine with the start and end we created

                currentScore = getCurrentScore(lineArray);      //update the current score
                lengthLabel->setText(to_string(currentScore));  //update the length label

                //if the current score is better then the tolerance
                if(currentScore <= scoreTolerance){
                    sprintf( message,"Great! %d is very close to %d!",currentScore, idealScore);        // print into a string
                    messagesLabel->setLabel(message);
                }
                else{
                    sprintf( message,"Sorry! %d is not very close to %d!",currentScore, idealScore);    // print into a string
                    messagesLabel->setLabel(message);
                }
            }
            buttonWasPressed = false;
        }
//-----------------When mouse is Dragged-----------------------------
        else if(e.getEventType() == MOUSE_DRAGGED) {

            //if pLine has a reasonable start point then make pLine follow the mouse and update the length message
            if(pLine->getStartPoint() != GPoint(0,0)){
                pLine->setEndPoint(mouseX, mouseY);
                currentScore = getCurrentScore(lineArray);                  //set the length label to the current score;
                currentScore += getDistance(pLine->getStartPoint().getX(),   //offset the current score by the length of pLine
                                            pLine->getStartPoint().getY(),
                                            pLine->getEndPoint().getX(),
                                            pLine->getEndPoint().getY());
                lengthLabel->setText(to_string(currentScore));              //change the length label every drag frame to account for pLine
            }
        }
    }

    // Close the windows, first the graphics window
    graphicsWindow.requestFocus();
    graphicsWindow.close();
    exitGraphics();

    return 0;
}

