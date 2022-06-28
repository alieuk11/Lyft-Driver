// Include all the necessary libraries
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <functional>
#include <string>
#include <cmath>

using namespace std;

// Struct to store the information from the location file
struct Location {
    int X; 
    int Y;
};

// Struct to store the information from the name file
struct Name {
    double ID;
    string name;
};

// Struct to store the combined information from both files
struct Planet {
    int X;
    int Y;
    char symbol;
    double ID;
    string name;
    bool visited;

};

// Function that outputs the map grid into a text file
    void makeMap (vector<vector<char> > &mapGrid, ofstream &fout) {
        for (size_t i = 0; i < mapGrid.size(); ++i) {
            for (size_t j = 0; j < mapGrid[i].size(); ++j) {
                fout << mapGrid[i][j];
            }
            fout << endl;
        }
    }

// Function which erases all the XX found in the files. Will run until npos is returned
void clearCorruption (vector <Name> &planetsVector) {
    for (size_t i = 0; i < planetsVector.size(); ++i) {
        while (planetsVector[i].name.find("XX")!= planetsVector[i].name.npos) {
            planetsVector[i].name.erase(planetsVector[i].name.find("XX"),2);
        }

    }
// Removes all the underscores and replaces them with spaces
    for(size_t i = 0; i < planetsVector.size(); ++i){
        for (size_t j = 0; j < planetsVector[i].name.size(); ++j) {
             if (planetsVector[i].name[j] == '_') {
            planetsVector[i].name[j] = ' ';
        }
    }
    }
}


int main() {
    
string locationFile;
string namesFile;
Planet planet;
Name planetName;
vector<Planet> planetVector;
vector<Name> nameVector;
Location endLoc;
Location startLoc;
Location dimensions;
int tracker;

// Prompts user for their location file 
    cout << "Enter Locations Filename: ";
    cin >> locationFile; 
// Prompts user for their names file
    cout << "Enter Names Filename: ";
    cin >> namesFile;

// Reads in both location and names file and displays an error message if the file cannot be opened
    ifstream fin (namesFile);
    if (!fin.is_open()) {
        cout << "Input file could not be opened";
        return 1;
    }
    ifstream fin2 (locationFile);
    if (!fin2.is_open()) {
        cout << "Input file could not be opened";
        return 1;
    }
// Reads in the name and location file to their corresponding declared structs
    fin2 >> dimensions.X >> dimensions.Y;
    fin2 >> startLoc.X >> startLoc.Y;
    fin2 >> endLoc.X >> endLoc.Y;

    while (fin2 >> planet.X >> planet.Y >> planet.symbol >> planet.ID) {
        planet.visited = 0;
        planetVector.push_back(planet);
    }

    while (fin >> planetName.ID >> planetName.name) {
        nameVector.push_back(planetName);
    }

// Removes all the planets that aren't in the given parameters
    for (size_t i = 0; i < planetVector.size(); ++i) {

        if (planetVector[i].X < 0 || planetVector[i].Y < 0){
            cout << planetVector[i].ID << " out of range - ignoring" << endl;
            planetVector.erase(planetVector.begin() + i);
        }

        if (planetVector[i].X == 0 || planetVector[i].Y == 0) {
            cout << planetVector[i].ID << " out of range - ignoring" << endl;
            planetVector.erase(planetVector.begin() + i);
        }

        if (planetVector[i].X > dimensions.X || planetVector[i].Y > dimensions.Y ) {
            cout << planetVector[i].ID << " out of range - ignoring" << endl;
            planetVector.erase(planetVector.begin() + i);
        }
    }
// Clears the errors and removes all the underscores and XX
    clearCorruption(nameVector);

// Compares ID numbers and names in both structs and sets the correct planet name 
    for (size_t i = 0; i < planetVector.size(); ++i){
        for (size_t j = 0 ; j < nameVector.size() ;++j){
            if ( planetVector[j].ID == nameVector[j].ID){
                planetVector[j].name = nameVector[j].name;
            }
        }
    }

// Declares an output file stream 
    ofstream fout ("journey.txt");

// Creates a vector of periods with dimensions equal to the first row of the planet file
    vector<char> columns (dimensions.Y, '.');
    vector<vector<char> > mapGrid (dimensions.X, columns);

// Labels the start and end ponits for the map
    mapGrid[startLoc.X-1][startLoc.Y-1] = 'S';
    mapGrid[endLoc.X-1][endLoc.Y-1] = 'E';

// Adds ID symbol for the planets to the map based on their coordinates
    for ( size_t i = 0; i < planetVector.size(); ++i){
        int xCoord = planetVector[i].X-1;
        int yCoord = planetVector[i].Y-1;
        mapGrid [xCoord][yCoord] = planetVector[i].symbol;
}

// Calls the makeMap function to output the map
    makeMap (mapGrid, fout);

// Outputs starting location to journey.txt
fout << "Start at " << startLoc.X << " " << startLoc.Y << endl;


// Determines the nearest non-visited planet, visits it, updates location, then goes to the next planet and repeats
for (int i = 0; i < planetVector.size(); ++i){
    
    double closestDistance = 1000000;

    for ( int j = 0; j < planetVector.size(); ++j){

        if(planetVector[j].visited == 0){

            double distance = 0;
            double valueOne = (startLoc.X - planetVector[j].X) * (startLoc.X - planetVector[j].X);
            double valueTwo = (startLoc.Y - planetVector[j].Y) * (startLoc.Y - planetVector[j].Y);
            distance = sqrt (valueOne + valueTwo);

// Selects the closest planet and finds the one with the lower ID number
            if(distance == closestDistance){

                if(planetVector[j].ID < planetVector[tracker].ID){

                    tracker = j;
                }
            }

            if(distance < closestDistance){

                closestDistance = distance;
                tracker = j;
            }

        }
    }
// Updates the location values and marks the previous planet as visited
    startLoc.X = planetVector[tracker].X;
    startLoc.Y = planetVector[tracker].Y;
    planetVector[tracker].visited = 1;

// Outputs the location of the next planet traveled to
    fout << "Go to " << planetVector[tracker].name << " at " << planetVector[tracker].X << " " << planetVector[tracker].Y << endl;


}
// Outputs the ending location
    fout << "End at " << endLoc.X << " " << endLoc.Y;

// Close the output file
fout.close();
return 0;
}