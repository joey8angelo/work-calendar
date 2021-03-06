# Work Calendar UCPD
Program to ease the process of adding work shifts into a calendar program.
## Table of Contents
* [Introduction](#introduction)
* [Input/Output](#input/output)
* [Code](#code)
* [ShiftData Struct](#shiftdata-struct)
* [splitWeeks](#splitweeks)
* [storeShifts](#storeshifts)
* [Honorable Mentions](#honorable-mentions)

## Introduction
A forever changing work schedule, no 9-5, M-F. Keeping track of a schedule that is never the same can be challenging. You might overlook a day and not show up... not me, but someone...

No matter! This program is for my personal use at UCPD and only demonstrates what I am learning in school. The program takes in a copy and paste from a spreadsheet made by my superiors and creates a .ics file that can easily be imported into a calendar program of your choosing. It also outputs two other .txt files with week 1 and week 2 split up to appreciate them both separately.

## Input/Output
The input file takes the copied text from the spreadsheet and looks like this:
![Input](./images/input.png)
It then gets cleaned up and stored in week1.txt and week2.txt where it will eventually become output.ics:
```
BEGIN:VCALENDAR
PRODID:-//Google Inc//Google Calendar 70.9054//EN
VERSION:2.0
CALSCALE:GREGORIAN
METHOD:PUBLISH
BEGIN:VTIMEZONE
TZID:America/Los_Angeles
LAST-MODIFIED:20201011T015911Z
BEGIN:DAYLIGHT
TZNAME:PDT
TZOFFSETFROM:-0800
TZOFFSETTO:-0700
DTSTART:19700308T020000
END:DAYLIGHT
BEGIN:STANDARD
TZNAME:PST
TZOFFSETFROM:-0700
TZOFFSETTO:-0800
DTSTART:19701101T020000
END:STANDARD
END:VTIMEZONE
BEGIN:VEVENT
DTSTART:20211106T240000
DTEND:20211107T010000
UID:20211106T240000@joey8angelo@gmail.com
DESCRIPTION:
LOCATION:
STATUS:CONFIRMED
SUMMARY:SHFTNME
TRANSP:OPAQUE
END:VEVENT
BEGIN:VEVENT
DTSTART:20211105T030000
DTEND:20211105T040000
UID:20211105T030000@joey8angelo@gmail.com
DESCRIPTION:
LOCATION:
STATUS:CONFIRMED
SUMMARY:SHFTNME
TRANSP:OPAQUE
END:VEVENT
BEGIN:VEVENT
DTSTART:20211104T080000
DTEND:20211104T090000
UID:20211104T080000@joey8angelo@gmail.com
DESCRIPTION:
LOCATION:
STATUS:CONFIRMED
SUMMARY:SHFTNME
TRANSP:OPAQUE
END:VEVENT
BEGIN:VEVENT
DTSTART:20211113T150000
DTEND:20211113T160000
UID:20211113T150000@joey8angelo@gmail.com
DESCRIPTION:
LOCATION:
STATUS:CONFIRMED
SUMMARY:SHFTNME
TRANSP:OPAQUE
END:VEVENT
BEGIN:VEVENT
DTSTART:20211112T180000
DTEND:20211112T190000
UID:20211112T180000@joey8angelo@gmail.com
DESCRIPTION:
LOCATION:
STATUS:CONFIRMED
SUMMARY:SHFTNME
TRANSP:OPAQUE
END:VEVENT
BEGIN:VEVENT
DTSTART:20211111T230000
DTEND:20211111T240000
UID:20211111T230000@joey8angelo@gmail.com
DESCRIPTION:
LOCATION:
STATUS:CONFIRMED
SUMMARY:SHFTNME
TRANSP:OPAQUE
END:VEVENT
END:VCALENDAR
```

## Code
I will highlight some of the interesting parts of the code here.

### ShiftData Struct
```c++
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
```
Structs such as this one are stored in a vector alongside all the other structs corresponding to the days that I need to work, ready to be written to the output file.

### splitWeeks
This function reads in the information from the input file and creates the two week .txt files. It also finds the months that start week 1 and 2.

It will start by cutting out all the information that we do not need, and stores every line in a vector.
```c++
while (getline(inFS, tempStr))
    tempVec.push_back(tempStr);
   
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
```

### storeShifts
storeShifts() stores the information from the week 1 and 2 files into ShiftData structs.

week is a vector of vector<string> pointers
the input is stored in the form

SHIFT HOURS Sunday1 Monday2 Tuesday3 Wednesday4 Thursday5 Friday6 Saturday7

So every shift name is stored as the first index of the current vector etc.
This algorithm splits the weeks into groups of 9
```c++
while (inFS >> tempVal) {
    if (index % 9 == 0) {
        week.push_back(new vector<string>);
        weekIndex++;
    }
    week.at(weekIndex)->push_back(tempVal);
    index++;
}
```

The current year and month are found, the time of entry is usually a week or so before the first shift, in this case the year and month may be different. As the starting month is already found earlier we can compare them, if the month of the first shift is less than the current month, then we know that the year has changed and can be added by 1.

The vector is traversed and looks for any instances of "dangelo" where in the isName function the apostrophe and any characters not contained in "dangelo" are removed, made lowercase, and compared, resulting in a boolean value.

```c++
for (int i = 0; i < week.size(); i++) {
    for (int j = 0; j < week.at(i)->size(); j++) {
        if (isName(week.at(i)->at(j))) {
            ...
            index++;
        }
    }
}
...
bool isName(string s) {
    string temp = "";
    string name = "dangelo";
    for(int i = 0; i < s.size(); i++) {
        if (name.find(tolower(s[i])) == string::npos){
            s.erase(i, 1);
            i--;
        }
        else
            temp += tolower(s[i]);
    }
    if (temp == name)
        return true;
    else 
        return false;
}
```
The vector(WShifts) of ShiftData structs can be filled
```c++
WShifts.push_back(new struct ShiftData);
```
And information can be written to its data members
```c++
WShifts.at(index)->shift = week.at(i)->at(0);
```
### Honorable Mentions
There are some helper functions that do various other tasks that deserve recognition as well, these include: 
* getNextDay()
* makeDateStamp()
* isLeap()
* splitDayNum()

But you can check them out on your own time.