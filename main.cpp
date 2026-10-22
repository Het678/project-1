/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cctype>
using namespace std;

// ---------------------------------------------------------
// Student class: data is private, accessed through functions
// ---------------------------------------------------------
class Student {
private:
    string id;            // example: 25ec401
    string name;
    int totalClasses;
    int presentClasses;

public:
    // Default constructor
    Student() {
        id = "";
        name = "";
        totalClasses = 0;
        presentClasses = 0;
    }

    // Parameterized constructor
    Student(string studentId, string studentName) {
        id = studentId;
        name = studentName;
        totalClasses = 0;
        presentClasses = 0;
    }

    // Getter functions
    string getId() const { return id; }
    string getName() const { return name; }
    int getTotalClasses() const { return totalClasses; }
    int getPresentClasses() const { return presentClasses; }

    // Mark attendance: 'P' = Present, 'A' = Absent
    void markAttendance(char status) {
        totalClasses++;
        if (status == 'P') {
            presentClasses++;
        }
    }

    // Calculate attendance percentage
    float calculatePercentage() const {
        if (totalClasses == 0) {
            return 0.0;
        }
        return (float)presentClasses / totalClasses * 100;
    }

    // Display one student as a table row
    void displayRow() const {
        cout << left << setw(12) << id
             << setw(20) << name
             << setw(8) << totalClasses
             << setw(10) << presentClasses
             << fixed << setprecision(2) << calculatePercentage() << "%"
             << endl;
    }

    // Display full details of one student
    void displayDetails() const {
        cout << "Student ID      : " << id << endl;
        cout << "Student Name    : " << name << endl;
        cout << "Total Classes   : " << totalClasses << endl;
        cout << "Present Classes : " << presentClasses << endl;
        cout << "Percentage      : " << fixed << setprecision(2)
             << calculatePercentage() << "%" << endl;
    }
};

// ---------------------------------------------------------
// Helper functions
// ---------------------------------------------------------

// Safely read an integer from the user
int readInt() {
    int number;
    while (!(cin >> number)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Please enter a number: ";
    }
    cin.ignore(1000, '\n');
    return number;
}

// Convert a text to lowercase (so 25EC401 and 25ec401 are the same)
string toLowerText(string text) {
    for (int i = 0; i < (int)text.size(); i++) {
        text[i] = tolower(text[i]);
    }
    return text;
}

// Find a student by ID. Returns the position, or -1 if not found
int findStudent(const vector<Student>& students, string id) {
    id = toLowerText(id);
    for (int i = 0; i < (int)students.size(); i++) {
        if (students[i].getId() == id) {
            return i;
        }
    }
    return -1;
}

// ---------------------------------------------------------
// Menu option functions
// ---------------------------------------------------------

// IDs are created automatically: 25ec401, 25ec402, 25ec403 ...
void addStudent(vector<Student>& students, int& nextIdNumber) {
    string name;
    cout << "Enter Student Name: ";
    getline(cin, name);

    if (name.empty()) {
        cout << "Name cannot be empty!" << endl;
        return;
    }

    string id = "25ec" + to_string(nextIdNumber);   // build the new ID
    nextIdNumber++;                                 // next student gets next number

    Student s(id, name);
    students.push_back(s);

    cout << "Student added successfully! Assigned ID: " << id << endl;
}

void displayAllStudents(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No students found." << endl;
        return;
    }

    cout << left << setw(12) << "ID"
         << setw(20) << "Name"
         << setw(8) << "Total"
         << setw(10) << "Present"
         << "Percentage" << endl;
    cout << "--------------------------------------------------------" << endl;

    for (int i = 0; i < (int)students.size(); i++) {
        students[i].displayRow();
    }
}

// Mark attendance of ALL students in one go (one class / one lecture)
void markAllAttendance(vector<Student>& students) {
    if (students.empty()) {
        cout << "No students found. Add students first." << endl;
        return;
    }

    cout << "Marking attendance for all students." << endl;
    cout << "Enter P = Present, A = Absent for each student." << endl;
    cout << "------------------------------------------------" << endl;

    int presentCount = 0;
    int absentCount = 0;

    for (int i = 0; i < (int)students.size(); i++) {
        char status;
        while (true) {
            cout << students[i].getId() << "  "
                 << students[i].getName() << " (P/A): ";
            cin >> status;
            cin.ignore(1000, '\n');
            status = toupper(status);

            if (status == 'P' || status == 'A') {
                break;
            }
            cout << "Invalid choice. Please enter P or A." << endl;
        }

        students[i].markAttendance(status);

        if (status == 'P') {
            presentCount++;
        } else {
            absentCount++;
        }
    }

    cout << "------------------------------------------------" << endl;
    cout << "Attendance saved. Present: " << presentCount
         << ", Absent: " << absentCount << endl;
}

void searchStudent(const vector<Student>& students) {
    string id;
    cout << "Enter Student ID to search (e.g. 25ec401): ";
    getline(cin, id);

    int index = findStudent(students, id);
    if (index == -1) {
        cout << "Student not found!" << endl;
    } else {
        cout << "\n--- Student Found ---" << endl;
        students[index].displayDetails();
    }
}

void showPercentage(const vector<Student>& students) {
    string id;
    cout << "Enter Student ID: ";
    getline(cin, id);

    int index = findStudent(students, id);
    if (index == -1) {
        cout << "Student not found!" << endl;
        return;
    }

    float percent = students[index].calculatePercentage();
    cout << "Attendance of " << students[index].getName() << " is "
         << fixed << setprecision(2) << percent << "%" << endl;

    if (percent < 75) {
        cout << "Warning: Attendance is below 75%!" << endl;
    }
}

// Show students whose attendance is below 75%
void showDefaulters(const vector<Student>& students) {
    bool found = false;

    for (int i = 0; i < (int)students.size(); i++) {
        if (students[i].getTotalClasses() > 0 &&
            students[i].calculatePercentage() < 75) {
            if (!found) {
                cout << "Students with attendance below 75%:" << endl;
                found = true;
            }
            students[i].displayRow();
        }
    }

    if (!found) {
        cout << "No students are below 75% attendance." << endl;
    }
}

void showMenu() {
    cout << "\n===== STUDENT ATTENDANCE MANAGEMENT SYSTEM =====" << endl;
    cout << "1. Add Student" << endl;
    cout << "2. Display All Students" << endl;
    cout << "3. Mark Attendance (All Students)" << endl;
    cout << "4. Search Student by ID" << endl;
    cout << "5. Calculate Attendance Percentage" << endl;
    cout << "6. Show Students Below 75%" << endl;
    cout << "7. Exit" << endl;
    cout << "Enter your choice: ";
}

// ---------------------------------------------------------
// Main function
// ---------------------------------------------------------
int main() {
    vector<Student> students;
    int nextIdNumber = 401;     // first ID will be 25ec401
    int choice;

    do {
        showMenu();
        choice = readInt();
        cout << endl;

        switch (choice) {
            case 1: addStudent(students, nextIdNumber); break;
            case 2: displayAllStudents(students); break;
            case 3: markAllAttendance(students); break;
            case 4: searchStudent(students); break;
            case 5: showPercentage(students); break;
            case 6: showDefaulters(students); break;
            case 7: cout << "Thank you! Exiting program..." << endl; break;
            default: cout << "Invalid choice! Please enter 1-7." << endl;
        }
    } while (choice != 7);

    return 0;
}