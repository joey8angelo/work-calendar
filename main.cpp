#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>

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

void splitWeeks(int&, int&);

void storeShifts(string, vector<ShiftData*>&, int);

void getNextDay(int&, int&, int&);

string makeDateStamp(int, int, int, string);

bool isLeap(int);

int splitDayNum(string);

bool isName(string);

int main() {
    ofstream outFS;
    vector<ShiftData*> W1Shifts; //vector of structs
    vector<ShiftData*> W2Shifts;
    int month1, month2;
    
    splitWeeks(month1, month2);
 
    storeShifts("week1.txt", W1Shifts, month1); // stores the data in the text files into the formatted vector of structs
    storeShifts("week2.txt", W2Shifts, month2);
    
    
    outFS.open("output.ics");
    if (!outFS.is_open()) {
        cout << "could not open output.ics" << endl;
        exit(EXIT_FAILURE);
    }
    
    //ICS file formatting
    outFS << "BEGIN:VCALENDAR\nPRODID:-//Google Inc//Google Calendar 70.9054//EN\nVERSION:2.0\nCALSCALE:GREGORIAN\nMETHOD:PUBLISH\nBEGIN:VTIMEZONE\nTZID:America/Los_Angeles\nLAST-MODIFIED:20201011T015911Z\nBEGIN:DAYLIGHT\nTZNAME:PDT\nTZOFFSETFROM:-0800\nTZOFFSETTO:-0700\nDTSTART:19700308T020000\nEND:DAYLIGHT\nBEGIN:STANDARD\nTZNAME:PST\nTZOFFSETFROM:-0700\nTZOFFSETTO:-0800\nDTSTART:19701101T020000\nEND:STANDARD\nEND:VTIMEZONE\n";
    //repeat for every event in the file
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
    //repeat again for every event for the second week
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

void splitWeeks(int& month1, int& month2) {
    ifstream inFS;
    ofstream outFS;
    int W2Index;
    string newStr;
    vector<string> tempVec;
    string tempStr;
   
    inFS.open("input.txt"); //original input file
   
    if (!inFS.is_open()) {
        cout << "could not open input.txt" << endl;
        exit(EXIT_FAILURE);
    }
   
    while (getline(inFS, tempStr))
        tempVec.push_back(tempStr); //store every line of the file
   
    tempVec.erase(tempVec.begin() + 0);
    month1 = atoi(tempVec.at(0).substr(0, 2).c_str());
    tempVec.erase(tempVec.begin() + 0);
   
    for(int i = 1; i < tempVec.size() - 1; i++) {
        if (tempVec.at(i).substr(0, 5) == "SHIFT") //find the index of the start of week two
            W2Index = i;
    }
    month2 = atoi(tempVec.at(W2Index - 1).substr(0, 2).c_str());
    tempVec.erase(tempVec.begin() + (W2Index - 1)); //erase extra stuff
    W2Index -= 1; //The start of week two changes indicies when the line before it is deleted
    tempVec.erase(tempVec.end());
   
    inFS.close();
   
    for (int i = 0; i < tempVec.size(); i++) {
        newStr = "";
        for (int j = 0; j < tempVec.at(i).length(); j++) { //remove spaces to properly count
            if(tempVec.at(i)[j]!=' ')
                newStr+=string(1,tempVec.at(i)[j]);
        }
        tempVec.at(i) = newStr;
    }
   
    outFS.open("week1.txt"); //write to w1 and w2 txt files
   
    if (!outFS.is_open()) {
        cout << "could not open week1.txt" << endl;
        exit(EXIT_FAILURE);
    }
   
    for(int i = 0; i < W2Index; i++)
        outFS << tempVec.at(i) << endl;
   
    outFS.close();
    outFS.open("week2.txt");
   
    if (!outFS.is_open()) {
        cout << "could not open week2.txt" << endl;
        exit(EXIT_FAILURE);
    }
   
    for(int i = W2Index; i < tempVec.size(); i++)
        outFS << tempVec.at(i) << endl;
   
    outFS.close();

}

void storeShifts(string weekStr, vector<ShiftData*>& WShifts, int month) {
    ifstream inFS;
    vector<vector<string>*> week;
    string tempVal;
    int year;
    int currMonth;
   
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
    
    for (int i = 0; i < week.size(); i++) {
        for (int j = 0; j < week.at(i)->size(); j++) {
            if (isName(week.at(i)->at(j))) { //find D'Angelo

                WShifts.push_back(new struct ShiftData); //make a new struct for this new shift
                string shiftTime = week.at(i)->at(1);
                string Sday = week.at(0)->at(j);
                
                WShifts.at(index)->shift = week.at(i)->at(0);
                WShifts.at(index)->startTime = shiftTime.substr(0 , shiftTime.length() - 5);
                WShifts.at(index)->endTime = shiftTime.substr(shiftTime.length() - 4 , shiftTime.length());
                WShifts.at(index)->startDay = splitDayNum(Sday);
                WShifts.at(index)->endDay = WShifts.at(index)->startDay;

                WShifts.at(index)->startMonth = month;

                time_t t = time(NULL);
                tm* tPtr = localtime(&t);

                year = (tPtr->tm_year) + 1900;
                currMonth = (tPtr->tm_mon) + 1;

                if (currMonth > month) { // if the current month is less than the starting month of the first shift, then a year change occurred
                    ++year;
                }

                WShifts.at(index)->startYear = year;
                if (splitDayNum(week.at(0)->at(2)) > WShifts.at(index)->startDay) { //if the first shift is not the first day, and the month changed in between, add the new month/year
                    if (WShifts.at(index)->startMonth == 12) {
                        WShifts.at(index)->startMonth = 1; 
                        WShifts.at(index)->startYear += 1;
                    }
                    else
                        WShifts.at(index)->startMonth += 1;
                }
                WShifts.at(index)->endMonth = WShifts.at(index)->startMonth;
                WShifts.at(index)->endYear = WShifts.at(index)->startYear;

                if (atoi(shiftTime.substr(0 , shiftTime.length() - 5).c_str()) > atoi(shiftTime.substr(shiftTime.length() - 4 , shiftTime.length()).c_str()))// if the time of the start of the shift is larger than the end of the shift, a new day has passed
                    getNextDay(WShifts.at(index)->endMonth, WShifts.at(index)->endDay, WShifts.at(index)->endYear);
                
                index++;
            }
        }
    }
}

void getNextDay(int& m, int& d, int& y) { //next day must check for: last day of the month, year, and if its a leap year
    if (d > 0 && d < 28)
        d += 1;
    else {
        if (m == 2) { // if its feb
            if (d == 28) {
                if (isLeap(y) == true)// leap year check
                    d += 1;
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
            else
                d += 1;
        }
        else {
            if (d == 31) {
                d = 1;
                if (m == 12) { // end of the year
                    m = 1;
                    y += 1;
                }
                else
                    m += 1;
            }
            else
                d += 1;
        }
    }
}

string makeDateStamp (int y, int m, int d, string t) { // formatting date for ics
    string output;
    
    output += to_string(y);
    if (m < 10)
        output += "0" + to_string(m);
    else
        output += to_string(m);
    if (d < 10)
        output += "0" + to_string(d);
    else
        output += to_string(d);

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

int splitDayNum(string day) { //finding the number attached to the name of the day
    if (day.substr(0, 3) == "Sun" || day.substr(0, 3) == "Mon" || day.substr(0, 3) == "Fri")
        return atoi(day.substr(6, day.length()).c_str());

    else if (day.substr(0, 3) == "Tue") 
        return atoi(day.substr(7, day.length()).c_str());
    
    else if (day.substr(0, 3) == "Thu" || day.substr(0, 3) == "Sat") 
        return atoi(day.substr(8, day.length()).c_str());
    
    else 
        return atoi(day.substr(9, day.length()).c_str());
}
 
bool isName(string s) {
    string temp = "";
    string name = "dangelo";
    for(int i = 0; i < s.size(); i++) {
        if (name.find(tolower(s[i])) == string::npos){// any char not in "name" gets removed
            s.erase(i, 1);                            // this accounts for the scheduling convention that if someone is being trained
            i--;                                      // by you it will appear as D'Angelo(C##)
        }
        else
            temp += tolower(s[i]);
    }
    if (temp == name)
        return true;
    else 
        return false;
}