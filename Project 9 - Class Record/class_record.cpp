#include <iostream>
#include <vector>
#include <string>
#include <algorithm>             // For std::remove_if

// Class representing each student's record
class Record {
public:
    int rollNo;                         // Unique roll number (Primary Key)
    std::string first_name;             // Student's first name
    std::string last_name;              // Student's last name
    std::string date_of_birth;          // Date of birth in DD/MM/YY format
    std::string branch;                 // Branch of study
    int yearOfAdmission;                // Year that the student was admitted
    int tentativeYearOfPassing;         // Expected year of graduation
    std::string state;                  // Student's home state
    std::string pin;                    // PIN code (used for district identification)
    std::string country;                // Student's country

    // Constructor to initialize all attribute of a record
    Record(int rollNo, const std::string& first_name, const std::string& last_name, const std::string& date_of_birth,
            const std::string& branch, int yearOfAdmission, int tentativeYearOfPassing, const std::string& state,
            const std::string& pin, const std::string& country)
        : rollNo(rollNo), first_name(first_name), last_name(last_name), date_of_birth(date_of_birth), branch(branch),
        yearOfAdmission(yearOfAdmission), tentativeYearOfPassing(tentativeYearOfPassing), state(state), pin(pin),
        country(country) {}    
};

// Class to manage the class records
class ClassRecords {
private:
    std::vector<Record> records;        // A vector (list) to store all student records
public:
    // Method to add a new record to the list
    void addRecord() {
        // Local variables to temporarily store input data
        int rollNo, yearOfAdmission, tentativeYearOfPassing;
        std::string first_name, last_name, date_of_birth, branch, state, pin, country;

        // Prompt the user for input and store it in respective variables
        std::cout << "Enter Roll No: ";
        std::cin >> rollNo;
        std::cin.ignore();              // Clear the input buffer to avoid issues with getline()
        std::cout << "Enter First Name: ";
        std::getline(std::cin, first_name);
        std::cout << "Enter Last Name: ";
        std::getline(std::cin, last_name);
        std::cout << "Enter Date of Birth (DD/MM/YYYY): ";
        std::getline(std::cin, date_of_birth);
        std::cout << "Enter Branch: ";
        std::getline(std::cin, branch);
        std::cout << "Enter Year of Admission: ";
        std::cin >> yearOfAdmission;
        std::cout << "Enter Tentative Year of Passing: ";
        std::cin >> tentativeYearOfPassing;
        std::cout << "Enter state: ";
        std::getline(std::cin, state);
        std::cout << "Enter PIN: ";
        std::getline(std::cin, pin);
        std::cout << "Enter Country: ";
        std::getline(std::cin, country);

        // Create a new record and add it to the record vector
        records.emplace_back(rollNo, first_name, last_name, date_of_birth, branch, yearOfAdmission, tentativeYearOfPassing, state, pin, country);
        std::cout << "Record added successfully!\n";
    }

    // Method to update an existing record based on Roll No
    void updateRecord(int rollNo) {
        for(auto& record : records) {
            // Find the record with matching Roll No
            if (record.rollNo == rollNo) {
                std::cout << "Updating record for Roll No: " << rollNo << "\n";

                // Allow user to update specific fields of the record
                std::cout << "Enter new Name: ";
                std::getline(std::cin, record.first_name);
                std::cout << "Record updated successfully!\n";
                return; // Exit the method after updating
            }
        }
        // If the Roll No is not found, display a message
        std::cout << "Record not found!\n";
    }

    // Method to delete a record based on Roll No
    void deleteRecord(int rollNo) {
        // Use std::remove_if to find the record and erase it from the vector
        auto it = std::remove_if(records.begin(), records.end(), [&](Record& record) { return record.rollNo == rollNo; });
        if (it != records.end()) {
            records.erase(it, records.end());       // Remove the found record
            std::cout << "Record deleted sucessfully!\n";
        } else {
            // If no record is found with the Roll No, display a message
            std::cout << "Record not found!\n";
        }
    }

    // Method to generate a report of students with a common name
    void generateReportByName(const std::string& first_name) const {
        std::cout << "Students with the name: " << first_name << "\n";
        for(const auto& record : records) {
            if (record.first_name == first_name) { // Check if the student's name matches
                printRecord(record);                // Print the matching record
            }
        }
    }

    // Method to generate a report of students from common state
    void generateReportByState(const std::string& state) const {
        std::cout << "Students from the state: " << state << "\n";
        for(const auto& record : records) {
            if (record.state == state) {            // check if the student's state matches
                printRecord(record);                // Print the matching record
            }
        }
    }
    
    // Method to generate a report of students from common district (based on PIN)
    void generateReportByPIN(const std::string& pin) const {
        std::cout << "Students from the district (PIN): " << pin << "\n";
        for(const auto& record : records) {
            if (record.pin == pin) {            // check if the student's state matches
                printRecord(record);                // Print the matching record
            }
        }
    }
private:
    // Helper method to print a record's details
    void printRecord(const Record& record) const {
        std::cout << "Roll No: " << record.rollNo << "\n";
        std::cout << "First Name: " << record.first_name << "\n";
        std::cout << "Last Name: " << record.last_name << "\n";
        std::cout << "Date of Birth: " << record.date_of_birth << "\n";
        std::cout << "Branch: " << record.branch << "\n";
        std::cout << "Year of Admission: " << record.yearOfAdmission << "\n";
        std::cout << "Tentative Year of Passing: " << record.tentativeYearOfPassing << "\n";
        std::cout << "State: " << record.state << "\n";
        std::cout << "PIN: " << record.pin << "\n";
        std::cout << "Country: " << record.country << "\n";
    }
};

int main() {
    ClassRecords ClassRecords;          // Create an instance of ClassRecords to manage all student records
    int choice, rollNo;                 // Variables to store user choices and Roll No for operations
    std::string first_name, state, pin; // Variables for searching by first_name, state, or PIN

    // Display a menu for the user to perform various operations on the records
    do {
        std::cout << "1. Add Record\n2. Update Record\n3. Delete Record\n4. Generate Report by Name\n";
        std::cout << "5. Generate Report by State\n6. Generate Report by District (PIN)\n7. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();              // Clear input buffer after numeric input

        // Perform the operation based on user's choice
        switch(choice) {
            case 1:
                ClassRecords.addRecord();       // Call the method to add a new record
                break;
            case 2:
                std::cout << "Enter Roll No of the record to update: ";
                std::cin >> rollNo;
                std::cin.ignore();
                ClassRecords.updateRecord(rollNo);      // Call the method to update a record
                break;
            case 3:
                std::cout << "Enter Roll No of the record to delete: ";
                std::cin >> rollNo;
                std::cin.ignore();
                ClassRecords.deleteRecord(rollNo);      // Call the method to delete a record
                break;
            case 4:
                std::cout << "Enter Name to search: ";
                std::getline(std::cin, first_name);
                ClassRecords.generateReportByName(first_name);      // Call the method to generate a report by name
                break;
            case 5:
                std::cout << "Enter State to search: ";
                std::getline(std::cin, state);
                ClassRecords.generateReportByState(state);      // Call the method to generate a report by state
                break;
            case 6:
                std::cout << "Enter PIN to search: ";
                std::getline(std::cin, pin);
                ClassRecords.generateReportByPIN(pin);      // Call the method to generate a report by PIN
                break;
            case 7:
                std::cout << "Existing...\n";
                break;
            default:
                std::cout << "Invalid choice!\n";            
        }
    } while(choice != 7); // Keep showing the menu until the user chooses to exit

    return 0;
}