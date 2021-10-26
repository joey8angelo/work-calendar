/* THIS PROGRAM TAKES A COPY AND PASTED SCHEDULE FOR UCPD
IT FINDS THE DATE AND TIME D'ANGELO WORKS AND TURNS THAT DATA INTO AN ICS FILE TO INPUT INTO A CALENDAR APPLICATION*/

#include <iostream>
#include <fstream>
#include <vector> //HELLO
#include <string> // new main-edits
#include <stdlib.h> // after u
#include <ctime>

using namespace std;

struct ShiftData {
    string shift;
    int startYear;
    int endYear;
    string startTime;
    string endTime;
    int startDay;
    int endDay;
    int startMonth;
    int endMonth;
};

void splitWeeks();

void storeShifts(string, vector<ShiftData*>&);

void getNextDay(int&, int&, int&);

string makeDateStamp(int, int, int, string);

bool isLeap(int);

int main() {
    ofstream outFS;
    vector<ShiftData*> W1Shifts; //vector of structs
    vector<ShiftData*> W2Shifts;
    
    splitWeeks();
 
    storeShifts("week1.txt", W1Shifts);
    storeShifts("week2.txt", W2Shifts);
    
    
    outFS.open("output.ics");
    if (!outFS.is_open()) {
        cout << "could not open output.ics" << endl;
        exit(EXIT_FAILURE);
    }
    
    
    outFS << "BEGIN:VCALENDAR\nPRODID:-//Google Inc//Google Calendar 70.9054//EN\nVERSION:2.0\nCALSCALE:GREGORIAN\nMETHOD:PUBLISH\nBEGIN:VTIMEZONE\nTZID:America/Los_Angeles\nLAST-MODIFIED:20201011T015911Z\nBEGIN:DAYLIGHT\nTZNAME:PDT\nTZOFFSETFROM:-0800\nTZOFFSETTO:-0700\nDTSTART:19700308T020000\nEND:DAYLIGHT\nBEGIN:STANDARD\nTZNAME:PST\nTZOFFSETFROM:-0700\nTZOFFSETTO:-0800\nDTSTART:19701101T020000\nEND:STANDARD\nEND:VTIMEZONE\n";
    for (int i = 0; i < W1Shifts.size(); i++) {
        outFS << "BEGIN:VEVENT\nDTSTART:";
        outFS << makeDateStamp(W1Shifts.at(i)->startYear, W1Shifts.at(i)->startMonth, W1Shifts.at(i)->startDay, W1Shifts.at(i)->startTime) << endl;
        outFS << "DTEND:";
        outFS << makeDateStamp(W1Shifts.at(i)->endYear, W1Shifts.at(i)->endMonth, W1Shifts.at(i)->endDay, W1Shifts.at(i)->endTime) << endl;
        outFS << "UID:";
        outFS << makeDateStamp(W1Shifts.at(i)->startYear, W1Shifts.at(i)->startMonth, W1Shifts.at(i)->startDay, W1Shifts.at(i)->startTime);
        outFS << "@joey8angelo@gmail.com" << endl;
        outFS << "DESCRIPTION:\nLOCATION:\n";
        outFS << "STATUS:CONFIRMED\nSUMMARY:" << W1Shifts.at(i)->shift << endl;
        outFS << "TRANSP:OPAQUE\nEND:VEVENT\n";
    }
    for (int i = 0; i < W2Shifts.size(); i++) {
        outFS << "BEGIN:VEVENT\nDTSTART:";
        outFS << makeDateStamp(W2Shifts.at(i)->startYear, W2Shifts.at(i)->startMonth, W2Shifts.at(i)->startDay, W2Shifts.at(i)->startTime) << endl;
        outFS << "DTEND:";
        outFS << makeDateStamp(W2Shifts.at(i)->endYear, W2Shifts.at(i)->endMonth, W2Shifts.at(i)->endDay, W2Shifts.at(i)->endTime) << endl;
        outFS << "UID:";
        outFS << makeDateStamp(W2Shifts.at(i)->startYear, W2Shifts.at(i)->startMonth, W2Shifts.at(i)->startDay, W2Shifts.at(i)->startTime);
        outFS << "@joey8angelo@gmail.com" << endl;
        outFS << "DESCRIPTION:\nLOCATION:\n";
        outFS << "STATUS:CONFIRMED\nSUMMARY:" << W2Shifts.at(i)->shift << endl;
        outFS << "TRANSP:OPAQUE\nEND:VEVENT\n";
    }
 
    outFS << "END:VCALENDAR";
    
    outFS.close();
    
    return 0;
    
}

void splitWeeks() {
    ifstream inFS;
    ofstream outFS;
    int W2Index;
   
    vector<string> tempVec;
    string tempStr;
   
    inFS.open("schedule.txt"); //original input file
   
    if (!inFS.is_open()) {
        cout << "could not open schedule.txt" << endl;
        exit(EXIT_FAILURE);
    }
   
    while (getline(inFS, tempStr)) {
        tempVec.push_back(tempStr); //store every line of the file
    }
   
    tempVec.erase(tempVec.begin() + 0);
    tempVec.erase(tempVec.begin() + 0);
   
    for(int i = 1; i < tempVec.size() - 1; i++) {
        if (tempVec.at(i).substr(0, 5) == "SHIFT") { //find the index of the start of week two
            W2Index = i;
        }
    }
   
    tempVec.erase(tempVec.begin() + (W2Index - 1)); //erase extra stuff
    W2Index -= 1; //The start of week two changes indicies when the line before it is deleted
    tempVec.erase(tempVec.end());
   
    inFS.close();
   
    string newStr;
   
    for (int i = 0; i < tempVec.size(); i++) {
        newStr = "";
        for (int j = 0; j < tempVec.at(i).length(); j++) { //remove spaces to properly count
            if(tempVec.at(i)[j]!=' '){
                newStr+=string(1,tempVec.at(i)[j]);
            }
        }
        tempVec.at(i) = newStr;
    }
   
    outFS.open("week1.txt"); //write to w1 and w2 txt files
   
    if (!outFS.is_open()) {
        cout << "could not open week1.txt" << endl;
        exit(EXIT_FAILURE);
    }
   
    for(int i = 0; i < W2Index; i++) {
        outFS << tempVec.at(i) << endl;
    }
   
    outFS.close();
    outFS.open("week2.txt");
   
    if (!outFS.is_open()) {
        cout << "could not open week2.txt" << endl;
        exit(EXIT_FAILURE);
    }
   
    for(int i = W2Index; i < tempVec.size(); i++) {
        outFS << tempVec.at(i) << endl;
    }
   
    outFS.close();

}

void storeShifts(string weekStr, vector<ShiftData*>& WShifts) {
    ifstream inFS;
    vector<vector<string>*> week;
    string tempVal;
    int month;
    int year;
   
    inFS.open(weekStr);
   
    if (!inFS.is_open()) {
        cout << "could not open " << weekStr << endl;
        exit(EXIT_FAILURE);
    }
    
    //store the week into a vector of vectors containing 8 values for shift/hours/date
    
    int index = 0;
    int weekIndex = -1;
    
    while (inFS >> tempVal) {
        if (index % 9 == 0) {
            week.push_back(new vector<string>);
            weekIndex++;
        }
        week.at(weekIndex)->push_back(tempVal);
        index++;
    }
    
    //find the shift/hours/date that D'Angelo works
    
    index = 0;
    
    cout << "starting month for " << weekStr << ": ";
    cin >> month;
    cout << "starting year for " << weekStr << ": ";
    cin >> year;
    
    for (int i = 0; i < week.size(); i++) {
        for (int j = 0; j < week.at(i)->size(); j++) {
            if (week.at(i)->at(j) == "D'Angelo") {

                WShifts.push_back(new struct ShiftData);
                string time = week.at(i)->at(1);
                string Sday = week.at(0)->at(j);
                
                WShifts.at(index)->shift = week.at(i)->at(0);
                WShifts.at(index)->startTime = time.substr(0 , time.length() - 5);
                WShifts.at(index)->endTime = time.substr(time.length() - 4 , time.length());
                if (Sday.substr(0, 3) == "Sun" || Sday.substr(0, 3) == "Mon" || Sday.substr(0, 3) == "Fri") {
                    WShifts.at(index)->startDay = atoi(Sday.substr(6, Sday.length()).c_str());
                }
                else if (Sday.substr(0, 3) == "Tue") {
                    WShifts.at(index)->startDay = atoi(Sday.substr(7, Sday.length()).c_str());
                }
                else if (Sday.substr(0, 3) == "Thu" || Sday.substr(0, 3) == "Sat") {
                    WShifts.at(index)->startDay = atoi(Sday.substr(8, Sday.length()).c_str());
                }
                else {
                    WShifts.at(index)->startDay = atoi(Sday.substr(9, Sday.length()).c_str());
                }
                WShifts.at(index)->endDay = WShifts.at(index)->startDay;

                WShifts.at(index)->startMonth = month; //problem: MONTH INPUT DOES NOT WORK IF THE FIRST SHIFT WORKED ON THE SCHEDULE IS NOT IN THE START MONTH
                WShifts.at(index)->endMonth = month;
                WShifts.at(index)->startYear = year;
                WShifts.at(index)->endYear = year;

                if (atoi(time.substr(0 , time.length() - 5).c_str()) > atoi(time.substr(time.length() - 4 , time.length()).c_str())) { // if start of shift is larger than end of shift then it goes to the next day
                    getNextDay(WShifts.at(index)->endMonth, WShifts.at(index)->endDay, WShifts.at(index)->endYear);
                }
                
                index++;
            }
        }
    }
}

void getNextDay(int& m, int& d, int& y) {
    if (d > 0 && d < 28) {
        d += 1;
    }
    else {
        if (m == 2) { // if its feb
            if (d == 28) {
                if (isLeap(y) == true) { // leap year check
                    d += 1;
                }
                else {
                    d = 1;
                    m = 3;
                }
            }
            else {
                d = 1;
                m = 3;
            }
        }
        else if (m == 4 || m == 6 || m == 9 || m == 11) { // if April, June, September, November
            if (d == 30) {
                d = 1;
                m += 1;
            }
            else {
                d += 1;
            }
        }
        else {
            if (d == 31) {
                d = 1;
                if (m == 12) { // end of the year
                    m = 1;
                    y += 1;
                }
                else {
                    m += 1;
                }
            }
            else {
                d += 1;
            }
        }
    }
}

string makeDateStamp (int y, int m, int d, string t) { // formatting for ics
    string output;
    
    output += to_string(y);
    if (m < 10) {
        output += "0" + to_string(m);
    }
    else {
        output += to_string(m);
    }
    if (d < 10) {
        output += "0" + to_string(d);
    }
    else {
        output += to_string(d);
    }
    output += "T" + t + "00";
    
    return output;
}

bool isLeap(int y) {
    bool is_leap_year = false;
    if (y % 4 == 0) {
        if (y % 100 == 0) {
            if (y % 400 == 0)
                is_leap_year = true;
            else
                is_leap_year = false;
        }
        else
            is_leap_year = true;
    }
    else
        is_leap_year = false;
    return is_leap_year;
}