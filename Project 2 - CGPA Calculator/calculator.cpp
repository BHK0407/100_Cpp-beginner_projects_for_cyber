#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// Define s struct name Course

struct Course {
    string name;            // Name of the course
    int credit_hours;       // Credit hours for the course
    double grade;           // Grade received in the course (0.0 - 4.0)
};

// Function double to calculate CGPA "CalculateCGPA"

double CalculateCGPA (const vector<Course>& courses){
    double totalGradePoints = 0.0;
    int totalCreditHours = 0;

    // Interate through each course to calculate total grade points and credits hours
    for (const Course& course: courses){
        totalGradePoints += course.grade * course.credit_hours;
        totalCreditHours += course.credit_hours;
    }

    // Return CGPA if there are credit hours, otherwise return 0.0
    if (totalCreditHours > 0){
        return totalGradePoints / totalCreditHours;
    } else {
        return 0.0;
    }
}

int main(){
    char choice;
    do {
        // Vector to hold all courses
        vector<Course> courses;

        // Introduction to the program
        cout << "CGPA Calculator" << endl;
        // Prompt the number of courses
        int numCourses;
        cout << "Enter the number of courses: ";
        cin >> numCourses;

        // Input course detail for each course
        for(int i = 0; i < numCourses; i++){
            Course course;

            // Get course details
            cout << "\nEnter details for Course " << i + 1 << ": " << endl;
            cout << "Course Name: ";
            cin.ignore(); // To clear the newline character from the input buffer
            getline(cin,course.name);
        
    
            cout << "Credit Hours: ";
            cin >> course.credit_hours;
            cout << "Grade (0-4): ";
            cin >> course.grade;

            // Validate the grade input
            if (course.grade < 0 || course.grade > 4){
                cout << "Invalid grade. Please enter a grade between 0 and 4." << endl;
                i--; // Decrement i to retry this interation
                continue;
            }
            // Add the course to the vector
            courses.push_back(course);
        }
        // Calculate and display the CGPA
        double cgpa = CalculateCGPA(courses);
        cout << fixed << setprecision(2); // Set output format to 2 decimal places
        cout << "\nYour CGPA is: " << cgpa << endl;

        system("pause");

        system("cls");
        
    }while (choice == 'y' || choice == 'Y');
    
    return 0;
}