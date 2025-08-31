/// Author: Mohammed Saeed
/// CGPA Calculator in C++

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

// ====================== Grade Conversions ======================

// Convert letter grade -> 4.0 scale
double letterTo4(const string& g)
{
    string s = g;
    for (char& c : s) c = toupper(c); // make uppercase

    if (s == "A+" || s == "A") return 4.0;
    if (s == "A-") return 3.7;
    if (s == "B+") return 3.3;
    if (s == "B")  return 3.0;
    if (s == "B-") return 2.7;
    if (s == "C+") return 2.3;
    if (s == "C")  return 2.0;
    if (s == "C-") return 1.7;
    if (s == "D+") return 1.3;
    if (s == "D")  return 1.0;
    if (s == "F")  return 0.0;

    cout << "Invalid letter grade. Using 0.0\n";
    return 0.0;
}

// Convert percentage -> 4.0 scale
double percentageTo4(double p)
{
    if (p >= 90) return 4.0;
    if (p >= 85) return 3.7;
    if (p >= 80) return 3.3;
    if (p >= 75) return 3.0;
    if (p >= 70) return 2.7;
    if (p >= 65) return 2.3;
    if (p >= 60) return 2.0;
    if (p >= 55) return 1.7;
    if (p >= 50) return 1.3;
    return 0.0;
}

// ====================== Structs ======================

struct Course {
    string code;
    double credits;
    string gradeRaw;
    double grade4;
};

struct Semester {
    int index;
    vector<Course> courses;
    double totalCredits;
    double totalGradePoints;
    double gpa;
};

// ====================== Print ======================
void printLine(char ch = '-', int n = 60)
{
    for (int i = 0; i < n; i++) cout << ch;
    cout << "\n";
}

void printSemester(const Semester& sem)
{
    cout << "\n";
    printLine('=');
    cout << "Semester " << sem.index << " Summary\n";
    printLine('=');
    cout << left << setw(12) << "Course"
         << setw(10) << "Credits"
         << setw(14) << "Grade"
         << setw(10) << "GPA"
         << "Points\n";
    printLine();

    cout << fixed << setprecision(2);
    for (auto& c : sem.courses)
    {
        cout << left << setw(12) << c.code
             << setw(10) << c.credits
             << setw(14) << c.gradeRaw
             << setw(10) << c.grade4
             << (c.grade4 * c.credits) << "\n";
    }
    printLine();
    cout << "Total Credits: " << sem.totalCredits << "\n";
    cout << "Total Points:  " << sem.totalGradePoints << "\n";
    cout << "Semester GPA:  " << sem.gpa << "\n";
}

// ====================== Save Report ======================
void saveReport(const vector<Semester>& all, double cumCredits, double cumPoints)
{
    ofstream out("cgpa_report.txt");
    if (!out)
    {
        cout << "Error: Could not save file.\n";
        return;
    }

    out << fixed << setprecision(2);
    for (auto& sem : all)
    {
        out << "==============================\n";
        out << "Semester " << sem.index << " Summary\n";
        out << "==============================\n";
        for (auto& c : sem.courses)
        {
            out << c.code << " | "
                << "Credits: " << c.credits << " | "
                << "Grade: " << c.gradeRaw << " | "
                << "GPA: " << c.grade4 << " | "
                << "Points: " << (c.credits * c.grade4) << "\n";
        }
        out << "Semester GPA: " << sem.gpa << "\n\n";
    }
    double cgpa = (cumCredits > 0 ? cumPoints / cumCredits : 0.0);
    out << "========== OVERALL ==========\n";
    out << "Cumulative Credits: " << cumCredits << "\n";
    out << "Cumulative Points:  " << cumPoints << "\n";
    out << "CGPA: " << cgpa << "\n";
    out.close();

    cout << "Report saved to cgpa_report.txt\n";
}

// ====================== Main ======================
int main()
{
    cout << "===== CGPA Calculator =====\n\n";
    cout << "Choose grading mode:\n";
    cout << " 1) Letter grades (A+, A, B-, ...)\n";
    cout << " 2) GPA on 4.0 scale\n";
    cout << " 3) GPA on 10.0 scale\n";
    cout << " 4) Percentage (0-100)\n";
    cout << "Choice: ";

    int mode;
    cin >> mode;

    double cumCredits = 0, cumPoints = 0;
    vector<Semester> allSems;

    int semIndex = 1;
    char more = 'y';
    while (more == 'y' || more == 'Y') {
        int n;
        cout << "\nEnter number of courses for Semester " << semIndex << ": ";
        cin >> n;

        Semester sem;
        sem.index = semIndex;
        sem.totalCredits = 0;
        sem.totalGradePoints = 0;

        for (int i = 0; i < n; i++) {
            Course c;
            cout << "\nCourse " << (i + 1) << " code/name: ";
            cin >> c.code;

            cout << "Credit Hours (1-2-3-4): ";
            cin >> c.credits;

            cout << "Grade: ";
            cin >> c.gradeRaw;

            if (mode == 1) {
                c.grade4 = letterTo4(c.gradeRaw);
            } else if (mode == 2) {
                c.grade4 = stod(c.gradeRaw);
            } else if (mode == 3) {
                c.grade4 = (stod(c.gradeRaw) / 10.0) * 4.0;
            } else {
                c.grade4 = percentageTo4(stod(c.gradeRaw));
            }

            sem.courses.push_back(c);
            sem.totalCredits += c.credits;
            sem.totalGradePoints += c.credits * c.grade4;
        }

        sem.gpa = sem.totalGradePoints / sem.totalCredits;
        printSemester(sem);

        cumCredits += sem.totalCredits;
        cumPoints += sem.totalGradePoints;
        allSems.push_back(sem);

        cout << "\nAdd another semester? (y/n): ";
        cin >> more;
        semIndex++;
    }

    printLine('=');
    cout << "OVERALL SUMMARY\n";
    printLine('=');
    cout << "Cumulative Credits: " << cumCredits << "\n";
    cout << "Cumulative Points:  " << cumPoints << "\n";
    cout << "CGPA: " << (cumCredits > 0 ? cumPoints / cumCredits : 0.0) << "\n";

    saveReport(allSems, cumCredits, cumPoints);

    cout << "\nThank you. Goodbye!\n";
    return 0;
}
