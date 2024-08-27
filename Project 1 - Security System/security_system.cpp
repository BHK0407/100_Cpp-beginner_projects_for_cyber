#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(){
    int a, i = 0;
    string text, current_password, password0, password1, password2, passwd, name, age, user, word;
    string creds[3], cp[3];  // Increase the size to 3 to store username, password, and age

    cout << "   _____                      _ _            _____           _                 " << endl;
    cout << "  / ____|                    (_) |          / ____|         | |                " << endl;
    cout << " | (___   ___  ___ _   _ _ __ _| |_ _   _  | (___  _   _ ___| |_ ___ _ __ ___  " << endl;
    cout << "  \\___ \\ / _ \\/ __| | | | '__| | __| | | |  \\___ \\| | | / __| __/ _ \\ '_ ` _ \\ " << endl;
    cout << "  ____) |  __/ (__| |_| | |  | | |_| |_| |  ____) | |_| \\__ \\ ||  __/ | | | | |" << endl;
    cout << " |_____/ \\___|\\___|\\__,_|_|  |_|\\__|\\__, | |_____/ \\__, |___/\\__\\___|_| |_| |_|" << endl;
    cout << "                                     __/ |          __/ |                      " << endl;
    cout << "                                    |___/          |___/                       " << endl;
    cout << " ______________________________________________________ " << endl;
    cout << "|                                                      |" << endl;
    cout << "|                  1. Register                         |" << endl;
    cout << "|                  2. Login                            |" << endl;
    cout << "|                  3. Change Password                  |" << endl;
    cout << "|                  4. Exit                             |" << endl;
    cout << "|______________________________________________________|" << endl;

    do {
        cout << "Enter your choice: ";
        cin >> a;
        switch(a){
            // Section 1: Register
            case 1:{
                cout << " _________________________ " << endl;
                cout << "|.........Register........|" << endl;
                cout << "|_________________________|" << endl;
                cout << "Please, enter your username: ";
                cin >> name;
                cout << "Please, enter your password: ";
                cin >> password0;
                cout << "Please, enter your age: ";
                cin >> age;

                ofstream of1;
                of1.open("file.txt");
                if(of1.is_open()){
                    of1 << name << "\n";
                    of1 << password0 << "\n";
                    of1 << age << "\n";
                    cout << "Registration successful" << endl;
                }
                of1.close();
                break;
            }
            // Section 2: Login
            case 2:{
                i = 0;
                cout << " _________________________ " << endl;
                cout << "|..........Login..........|" << endl;
                cout << "|_________________________|" << endl;

                ifstream of2;
                of2.open("file.txt");
                cout << "Please enter the username: ";
                cin >> user;
                cout << "Please enter the password: ";
                cin >> passwd;

                if(of2.is_open()){
                    while(getline(of2, text)){
                        istringstream iss(text);
                        iss >> word;
                        creds[i] = word;
                        i++;
                    }
                    of2.close();

                    if(user == creds[0] && passwd == creds[1]){
                        cout << "*** Login Successful ***" << endl;
                        cout << "Details: " << endl;
                        cout << "Username: " << creds[0] << endl;
                        cout << "Password: " << creds[1] << endl;
                        cout << "Age: " << creds[2] << endl;
                    } else {
                        cout << endl;
                        cout << "Incorrect credentials" << endl;
                        cout << "|          1. Press 2 to Login                 |" << endl;
                        cout << "|          2. Press 3 to Change Password       |" << endl;
                    }
                }
                break;
            }
            // Section 3: Change Password
            case 3:{
                i = 0;
                cout << " ___________________________________ " << endl;
                cout << "|..........Change Password..........|" << endl;
                cout << "|___________________________________|" << endl;

                ifstream of0;
                of0.open("file.txt");
                cout << "Enter the current password: ";
                cin >> current_password;

                if(of0.is_open()){
                    while(getline(of0, text)){
                        istringstream iss(text);
                        iss >> word;
                        cp[i] = word;
                        i++;
                    }
                    of0.close();

                    if(current_password == cp[1]){
                        ofstream of1;
                        of1.open("file.txt");
                        if(of1.is_open()){
                            cout << "Enter your new password: ";
                            cin >> password1;
                            cout << "Repeat your password: ";
                            cin >> password2;

                            if(password1 == password2){
                                of1 << cp[0] << "\n";  // Username
                                of1 << password1 << "\n";  // New password
                                of1 << cp[2] << "\n";  // Age
                                cout << "Password changed successfully!!!" << endl;
                            } else {
                                of1 << cp[0] << "\n";
                                of1 << current_password << "\n";  // Keep old password
                                of1 << cp[2] << "\n";
                                cout << "Passwords do not match!!!" << endl;
                            }
                        }
                        of1.close();
                    } else {
                        cout << "Current password is incorrect!" << endl;
                    }
                }
                break;
            }
            case 4:{
                cout << "____________Thank you!___________" << endl;
                break;
            }
            default:{
                cout << "Enter a valid choice." << endl;
            }
        }
    } while(a != 4);

    return 0;
}
