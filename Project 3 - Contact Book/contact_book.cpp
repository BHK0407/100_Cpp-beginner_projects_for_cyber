#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Define a 'contact' Class
class Contact {
public:
    string name;
    string phone;
    string email;

    Contact(string n, string p, string e)
        : name(n), phone(p), email(e) {}
};

// Declare functions
void addContact(vector<Contact>& contacts);
void searchContact(const vector<Contact>& contacts);
void deleteContact(vector<Contact>& contacts);
void displayAllContacts(const vector<Contact>& contacts);
void saveContacts(const vector<Contact>& contacts, const string& filename);
void loadContacts(vector<Contact>& contacts, const string& filename);


// Declare function addContact
void addContact(vector<Contact>& contacts) {
    string name, phone, email;
    cout << "Enter name: ";
    getline(cin, name);
    cout << "Enter phone number: ";
    getline(cin, phone);
    cout << "Enter email: ";
    getline(cin, email);
    contacts.emplace_back(name, phone, email);
}

// Declare function searchContact
void searchContact(const vector<Contact>& contacts){
    string name;
    cout << "Enter name to search: ";
    getline(cin, name);
    for (const auto& contact : contacts) {
        if (contact.name == name) {
            cout << "Name: " << contact.name << ", Phone: " << contact.phone << ", Email: " << contact.email << "\n" ;
            return;
        }
    }
    cout << "Contact not found.\n";
}

// Declare function deleteContact
void deleteContact(vector<Contact>& contacts){
    string name;
    cout << "Enter name to delete: ";
    getline(cin, name);
    auto it = remove_if(contacts.begin(), contacts.end(), [&](const Contact& contact) {
        return contact.name == name;
});
if (it != contacts.end()) {
    contacts.erase(it, contacts.end());
    cout << "Contact deleted.\n";
} else {
    cout << "Contact not found.\n";
    }
}

// Declare function displayAllContacts
void displayAllContacts(const vector<Contact>& contacts) {
    for (const auto& contact : contacts){
        cout << "Name: " << contact.name << ", Phone: " << contact.phone << ", Email: " << contact.email << "\n";
    }
}

// Declare function saveContacts
void saveContacts(const vector<Contact>& contacts, const string & filename){
    ofstream file(filename);
    for (const auto& contact : contacts){
        file << contact.name << ',' << contact.phone << ',' << contact.email << "\n";
    }
}

// Declare function loadContacts
void loadContacts(vector<Contact>& contacts, const string& filename){
    ifstream file(filename);
    string line;
    while(getline(file, line)){
        istringstream ss(line);
        string name, phone, email;
        getline(ss, name, ',');
        getline(ss, phone, ',');
        getline(ss, email);
        contacts.emplace_back(name, phone, email);
    }
}

int main(){
    vector<Contact> contacts;           // Vector to store contacts
    string filename = "contacts.txt";   // File to save/load contacts
    loadContacts(contacts, filename);   // Load contacts from file at startup

    int choice;
    do {
        // Display menu
        cout << "1. Add Contact\n";
        cout << "2. Search Contact\n";
        cout << "3. Delete Contact\n";
        cout << "4. Display All Contacts\n";
        cout << "5. Save & Exit\n";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                addContact(contacts);
                break;
            case 2:
                searchContact(contacts);
                break;
            case 3:
                deleteContact(contacts);
                break;
            case 4: 
                displayAllContacts(contacts);
                break;
            case 5:
                saveContacts(contacts, filename);
                cout << "Contacts saved. Existring program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 5);

    return 0;
}