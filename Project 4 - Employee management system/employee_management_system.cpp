#include <iostream>         // Provides input and output stream functionalities
#include <windows.h>        // Contains declarations for all functions in the Windows API
#include <unistd.h>         // Contains various CONSTANTs and FUNCTION for system calls
#include <conio.h>          // Provides console input/output function
#include <cstdlib>          // Contains functions for performing general utilities like memory management, random numbers
#include <cstring>          // C-style strings
#include <cstdio>           // functions for input and output to files
#include <fstream>          // Provides file stream classes for file input/output
using namespace std;

int password_user();    

// Define a class for employee
class Employee_management{
private:
    // Variables for employee details
    char name[30];          // Stores the name of the employee
    char id[5];             // Stores the employee ID
    char designation[10];   // Stores the designation of the employee
    int age, ctc;           // Stores the stage and salary of the employee
    char experience;        // experience details of the employee

public:
    void menu();            // Display main menu
    void waitForEnter();    // Waits for the user to press enter
    void insert();          // Inserts a new employee record
    void display();         // Displays all employee records
    void modify();          // Modifies an existing employee record
    void search();          // Searches for an employee record by ID
    void deleted();         // Deletes an employee record

};

// Define the Menu Function
void Employee_management::menu(){
    password_user();
    while (true)
    {
        /*Prompt the statement to show the options in the menu */
        int choice;
        char x;             // Options to choose for action Y or N
        system("cls");      // Command "cls" in order to clear the screen
        cout << "\n\t\t\t\t\t-------------------------------" << endl;
        cout << "\t\t\t\t\t>> EMPLOYEE MANAGEMENT SYSTEM" << endl;
        cout << "\t\t\t\t\t-------------------------------" << endl;
        cout << "\t\t\t\t\t 1. Enter New Record" << endl;
        cout << "\t\t\t\t\t 2. Display Employee record" << endl;
        cout << "\t\t\t\t\t 3. Modify Employee Record" << endl;
        cout << "\t\t\t\t\t 4. Search Employee Record" << endl;
        cout << "\t\t\t\t\t 5. Delete Record" << endl;
        cout << "\t\t\t\t\t 6. Exit" << endl;

        cout << "\t\t\t\t\t............................" << endl;
        cout << "\t\t\t\t\t>> Your Choice Options : " << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:             // Add a new employee
            do{
                insert();
                cout << "\n\n\t\t\t Add new employee to the record process? Press (Y, N): ";
                cin >> x;                
            } while (x == 'y' || x == 'Y');
            waitForEnter();
            break;
        case 2:
            display();
            break;
        case 3:
            modify();
            break;
        case 4:
            search();
            break;
        case 5:
            deleted();
            break;
        case 6:
            system("cls");
            cout << "\n\t\t\t Close the statement" << endl;
        default:
            cout << "\n\n\t\t\t Invalid Choice ... Please Try Again...\n";
        }
        cout << "\t\t\t\t\t............................" << endl;

        getch();            // Wait for key press
    }
    
}

// Define Insert Function
void Employee_management::insert(){         // Insert data of an employee
    system("cls");
    fstream file;           // Declares a file stream object
    cout << "\n-------------------------------------------------------------------------------------------------------" << endl;
    cout << "------------------------------------- Employee Insert Data -------------------------------------------" << endl;
    cout << "\n Enter Name of Employee: ";
    cin >> name;
    cout << "\n Enter Employee ID [max 4 digits]: ";
    cin >> id;
    if (strlen(id) == 4 && isdigit(id[0]) && isdigit(id[1]) && isdigit(id[2]) && isdigit(id[3])){
        cout << "\nEmployee ID is valid.\n";
    } else {
        cout << "\nInvalid Employee ID. Please enter exactly 4 digits.\n";
    }
    cout << "\n Enter Designation: ";
    cin >> designation;
    cout << "\n Enter Employee Age: ";
    cin >> age;
    cout << "\n Enter Employee CTC: ";
    cin >> ctc;
    cout << "\n Enter Employee Experience: ";
    cin >> experience;
    // Opens the file for appending and output
    file.open("Employee_Record.txt", ios::app | ios::out);
    // Add the details form for an employee
    file << " " << name << " " << id << " " << designation << " " << age << " " << ctc << " " << experience << "\n";
    file.close();
}

// Define an Display Function
void Employee_management::display(){
    system("cls");
    int total = 1;
    fstream file;
    cout << "\n-------------------------------------------------------------------------------------------------------" << endl;
    cout << "------------------------------------- Employee Record Data -------------------------------------------" << endl;
    file.open("Employee_Record.txt", ios::in);
    if (!file){
        cout << "\n\t\t\tNo Data is Present...";
        file.close();
    } else {
        // Read data from file
        file >> name >> id >> designation >> age >> ctc >> experience;
        cout << "\n -------------------------------------------------------------------------------------------------";
        cout << "\n||    NAME    ||    ID    ||    DESIGNATION    ||    AGE    ||      CTC      ||    EXPERIENCE    ||";
        cout << "\n -------------------------------------------------------------------------------------------------";
        while (!file.eof()){        // Loops until the end of the file is reached
            cout << "\n";
            cout << total++ << ". " << name << "\t " << id << "\t\t  " << designation << "\t\t  " << age << "\t\t" << experience;
            file >> name >> id >> designation >> age >> ctc >> experience; 
        }
    }
    file.close();
    waitForEnter();
}

// Define an Modify Function
void Employee_management::modify(){
    system("cls");          // Clean the screen
    char checkId[5];        // Stores the ID of the employee to be modified
    int found = 0;          // Display the employee details if found
    fstream file, file1;    // Declares file stream objects for reading and writing
    cout << "\n-------------------------------------------------------------------------------------------------------" << endl;
    cout << "------------------------------------- Employee Modify Data ------------------------------------------" << endl;
    // Open the file for reading
    file.open("Employee_Record.txt", ios::in);
    if (!file){
        cout << "\n\t\t\tNo Data is Present...";
        file.close();
    } else {
        cout << "\nEnter employee id: ";
        cin >> checkId;
        // Open a temporary file for writing updated records
        file1.open("record.txt", ios::app | ios::out);
        while (!file.eof()){
            // Check if the current records's ID matches the one to be modified
            if (strcmp(checkId, id) == 0){
                cout << "\n Enter Name of Employee: ";
                cin >> name;
                cout << "\n Enter Employee ID [max 4 digits]: ";
                cin >> id;
                // Again to make sure enter correct/valid ID
                if (strlen(id) == 4 && isdigit(id[0]) && isdigit(id[1]) && isdigit(id[2]) && isdigit(id[3])){
                    cout << "\nEmployee ID is valid.\n";
                } else {
                    cout << "\nInvalid Employee ID. Please enter exactly 4 digits.\n";
                }
                cout << "\n Enter Designation: ";
                cin >> designation;
                cout << "\n Enter Employee Age: ";
                cin >> age;
                cout << "\n Enter Employee CTC: ";
                cin >> ctc;
                cout << "\n Enter Employee Experience: ";
                cin >> experience;
                cout << "\n\n Successfully Modify Details of Employee" << endl;
                file1 << " " << name << " " << id << " " << designation << " " << age << " " << ctc << " " << experience << "\n" << endl;
                found++;
            } else {
                file << " " << name << " " << id << " " << designation << " " << age << " " << ctc << " " << experience << "\n" << endl;
            }
            file >> name >> id >> designation >> age >> ctc >> experience;
        }
        // Check if there is nothing showing about the employee
        if (found == 0)
        {
            cout << "\n\n\tEmployee ID is Not Found... Please Try Again!!!" << endl;
        }
        file1.close();
        file.close();
        // Delete the original file
        remove("Employee_Record.txt");
        // Renames the temporary file into the original file name
        rename("record.txt", "Employee_Record.txt");
    }
    waitForEnter();
}

// Define for Search Function
void Employee_management::search(){     // Search for an employee record by ID and display it
    system("cls");
    fstream file; 
    char checkId[5];
    cout << "\n-------------------------------------------------------------------------------------------------------" << endl;
    cout << "------------------------------------- Employee Search Data -------------------------------------------" << endl;
    // Opens the file for reading
    file.open("Employee_Record.txt", ios::in);
    cout << "\n\nEnter Employee ID: ";
    cin >> checkId;
    if (!file){
        cout << "\n\t\t\tNo Data is Present...";
        file.close();
    } else {
        file >> name >> id >> designation >> age >> ctc >> experience;
        while (!file.eof()){
            if(strcmp(checkId, id) == 0) {          // Check if the current's record ID match with the search's ID
                cout << "\n Enter Name of Employee: ";
                cin >> name;
                cout << "\n Enter Employee ID [max 4 digits]";
                cin >> id;
                // Again to make sure enter correct /valid employee id
                if (strlen(id) == 4 && isdigit(id[0]) && isdigit(id[1]) && isdigit(id[2]) && isdigit(id[3])){
                    cout << "\nEmployee ID is valid";
                } else {
                    cout << "\nInvalid employee ID. Please enter exactly 4 digits...";
                }
                cout << "\n Enter Employee Designation: ";
                cin >> designation;
                cout << "\n Enter Employee Age: ";
                cin >> age;
                cout << "\n Enter Employee CTC: ";
                cin >> ctc;
                cout << "\n Enter Employee Experience: ";
                cin >> experience;
                cout << "-----------------------------";
            }
            file >> name >> id >> designation >> age >> ctc >> experience;
        }
    }
    file.close();
    waitForEnter();
}

// Define for Delete Function
void Employee_management::deleted(){
    system("cls");          // Clear the current screen
    fstream file, file1;
    char checkId[5];        // Store the ID of the employee to be deleted
    int found = 0;
    cout << "\n-------------------------------------------------------------------------------------------------------" << endl;
    cout << "------------------------------------- Delete Employee Data ------------------------------------------" << endl;
    file.open("Employee_Record.txt", ios::in);
    if (!file){
        cout << "\n\t\t\tNo Data is Present..." << endl;
        file.close();
    } else {
        cout << "\nEnter Employee Id To Remove Data: ";
        cin >> checkId;
        // Open a temporary file for writing records excluding the one to be deleted
        file1.open("record.txt", ios::app | ios::out);
        file >> name >> id >> designation >> age >> ctc >> experience;
        while (!file.eof())
        {
            if (strcmp(checkId, id) != 0) {
                file1 << " " << name << " " << id << " " << designation << " " << age << " " << ctc << " " << experience << "\n" << endl;
            } else {
                found++;
                cout << "\n\t\t\tSuccessfully Delete Data";
            }
            file >> name >> id >> designation >> age >> ctc >> experience;
        }
        if (found == 0){
            cout << "\n\n\tEmployee ID Not Found... Please Try Again...";
        }
        file1.close();
        file.close();
        remove("Employee_Record.txt");
        rename("record.txt", "Employee_Record.txt");
        waitForEnter();
    }
}

// Define an wait for Enter function
void Employee_management::waitForEnter(){       // Wait for the user press enter before returning to the menu
    cout << "\n\nPress enter to go back: ";
    cin.get();
}

// Password_User Function
int password_user(){
    // Store the registration details
    string pass_reg = "";
    string name_reg = "";
    // Store Login details
    string pass = "";
    string name = "";
    fstream file;
    char ch;        // Use to capture and store each character of the password
    system("cls");
    cout << "\n\t\t\t -----------------------------------------------------------------------------";
    cout << R"(
     /$$      /$$ /$$$$$$$$ /$$        /$$$$$$   /$$$$$$  /$$      /$$ /$$$$$$$$       /$$$$$$$$ /$$$$$$        /$$$$$$$$ /$$      /$$ /$$$$$$$  /$$        /$$$$$$  /$$     /$$ /$$$$$$$$ /$$$$$$$$
    | $$  /$ | $$| $$_____/| $$       /$$__  $$ /$$__  $$| $$$    /$$$| $$_____/      |__  $$__//$$__  $$      | $$_____/| $$$    /$$$| $$__  $$| $$       /$$__  $$|  $$   /$$/| $$_____/| $$_____/
    | $$ /$$$| $$| $$      | $$      | $$  \__/| $$  \ $$| $$$$  /$$$$| $$               | $$  | $$  \ $$      | $$      | $$$$  /$$$$| $$  \ $$| $$      | $$  \ $$ \  $$ /$$/ | $$      | $$      
    | $$/$$ $$ $$| $$$$$   | $$      | $$      | $$  | $$| $$ $$/$$ $$| $$$$$            | $$  | $$  | $$      | $$$$$   | $$ $$/$$ $$| $$$$$$$/| $$      | $$  | $$  \  $$$$/  | $$$$$   | $$$$$   
    | $$$$_  $$$$| $$__/   | $$      | $$      | $$  | $$| $$  $$$| $$| $$__/            | $$  | $$  | $$      | $$__/   | $$  $$$| $$| $$____/ | $$      | $$  | $$   \  $$/   | $$__/   | $$__/   
    | $$$/ \  $$$| $$      | $$      | $$    $$| $$  | $$| $$\  $ | $$| $$               | $$  | $$  | $$      | $$      | $$\  $ | $$| $$      | $$      | $$  | $$    | $$    | $$      | $$      
    | $$/   \  $$| $$$$$$$$| $$$$$$$$|  $$$$$$/|  $$$$$$/| $$ \/  | $$| $$$$$$$$         | $$  |  $$$$$$/      | $$$$$$$$| $$ \/  | $$| $$      | $$$$$$$$|  $$$$$$/    | $$    | $$$$$$$$| $$$$$$$$
    |__/     \__/|________/|________/ \______/  \______/ |__/     |__/|________/         |__/   \______/       |________/|__/     |__/|__/      |________/ \______/     |__/    |________/|________/
                                                                                                                                                                                                
                                                                                                                                                                                                
                                                                                                                                                                                                
           /$$      /$$  /$$$$$$  /$$   /$$  /$$$$$$   /$$$$$$  /$$$$$$$$ /$$      /$$ /$$$$$$$$ /$$   /$$ /$$$$$$$$        /$$$$$$  /$$     /$$ /$$$$$$  /$$$$$$$$ /$$$$$$$$ /$$      /$$          
          | $$$    /$$$ /$$__  $$| $$$ | $$ /$$__  $$ /$$__  $$| $$_____/| $$$    /$$$| $$_____/| $$$ | $$|__  $$__/       /$$__  $$|  $$   /$$//$$__  $$|__  $$__/| $$_____/| $$$    /$$$          
          | $$$$  /$$$$| $$  \ $$| $$$$| $$| $$  \ $$| $$  \__/| $$      | $$$$  /$$$$| $$      | $$$$| $$   | $$         | $$  \__/ \  $$ /$$/| $$  \__/   | $$   | $$      | $$$$  /$$$$          
          | $$ $$/$$ $$| $$$$$$$$| $$ $$ $$| $$$$$$$$| $$ /$$$$| $$$$$   | $$ $$/$$ $$| $$$$$   | $$ $$ $$   | $$         |  $$$$$$   \  $$$$/ |  $$$$$$    | $$   | $$$$$   | $$ $$/$$ $$          
          | $$  $$$| $$| $$__  $$| $$  $$$$| $$__  $$| $$|_  $$| $$__/   | $$  $$$| $$| $$__/   | $$  $$$$   | $$          \____  $$   \  $$/   \____  $$   | $$   | $$__/   | $$  $$$| $$          
          | $$\  $ | $$| $$  | $$| $$\  $$$| $$  | $$| $$  \ $$| $$      | $$\  $ | $$| $$      | $$\  $$$   | $$          /$$  \ $$    | $$    /$$  \ $$   | $$   | $$      | $$\  $ | $$          
          | $$ \/  | $$| $$  | $$| $$ \  $$| $$  | $$|  $$$$$$/| $$$$$$$$| $$ \/  | $$| $$$$$$$$| $$ \  $$   | $$         |  $$$$$$/    | $$   |  $$$$$$/   | $$   | $$$$$$$$| $$ \/  | $$          
          |__/     |__/|__/  |__/|__/  \__/|__/  |__/ \______/ |________/|__/     |__/|________/|__/  \__/   |__/          \______/     |__/    \______/    |__/   |________/|__/     |__/          
       
    )" << endl;
    cout << "\n\t\t\t -----------------------------------------------------------------------------";
    cout << "\n\n\t\t\t   -| Press: [1] Register & [2] Login  |-";
    int selection;
    cout << "\n\nPlease Enter Your Selection: ";
    cin >> selection;
    if (selection == 1){
        system("cls");
        cout << "\n\t\t\t\t\t---------------------------";
        cout << "\n\t\t\t\t\t>> REGISTRATION EMPLOYEE <<";
        cout << "\n\t\t\t\t\t---------------------------";
        cout << "\n\n\t\t\t\t\tEnter Your Name: ";
        cin >> name_reg;
        cout << "\n\n\t\t\t\t\tEnter Your Password: ";
        ch = _getch();
        while (ch != 13)    // 13 is the ASCII code for Enter Key 
        {
            pass_reg.push_back(ch);
            cout << '*';    // Display asterisks for each character entered    
            ch = _getch();  // Read the next character
        }
        file >> name_reg >> pass_reg;
        while (!file.eof())
        {
            if (pass == pass_reg && name == name_reg){
                cout << "\n\n\n\t\t\t\t\t| Verifying EMPLOYEE |\n\t\t\t\t\t";
                for (int a = 1; a < 8; a++)
                {
                    Sleep(500);
                    cout << "...";
                }
                cout << "\n\nAccess Granted...\n\n";
                system("PAUSE");
                system("cls");
                
            } else {
                cout << "\n\n\n\t\t\t\t\t| Verifying EMPLOYEE |\n\t\t\t\t\t";
                for (int a = 1; a < 8; a++){
                    Sleep(500);
                    cout << "...";
                }
                cout << "\n\nAccess Aborted...\n\n";
                system("PAUSE");
                system("cls");
                password_user();
            }
            file >> name_reg >> pass_reg;
        }
        file.close();
    } else {
        cout << "\nInvalid Input...Please Try Again...";
        password_user();
    }
    return 0;                                                                                                                                                                                                                                                                                                                                                                          
}

int main()
{
    // Call the function
    Employee_management system;
    system.menu();
    return 0;
}