#include <iostream>
#include <vector>
#include <string>
using namespace std;

const int MAX_FLEET_SIZE = 100;
// Define the Vehicle class
class Vehicle {
protected:
    string make;
    string model;
    int year;
    double price;
public:
    // Control the Setter
    void setMake(const string& m) { make = m;}
    void setModel(const string& m) { model = m;}
    void setYear(int y) { year = y;}
    void setPrice(int p) {price = p;}

    // Control the Getter
    string getMake() const { return make;}
    string getModel() const { return model;}
    int getYear() const { return year;}
    double getPrice() const { return price;}

    // The disPlayDetails function should print the make, model, year, and price of the vehicle
    virtual void displayDetails() const {
        cout << "Make: " << make << "\nModel: " << model << "\nYear: " << year << "\nPrice: $" << price << "\n";
    }

    // Virtual function to calculate tax (to be overridden)
    virtual float calculateTax() const = 0;

    virtual ~Vehicle() {}
};

// The Vehicle class have two derived classes Car and Truck

// Derived class Car
class Car : public Vehicle {
private:
    int numDoors;
public:
    // Setters
    void setNumDoors(int doors) { numDoors = doors; }
    int getNumDoors() const { return numDoors; }

    // OVerride displayDetails
    void displayDetails() const override{
        Vehicle::displayDetails();
        cout << "Number of doors: " << numDoors << "\n";
        cout << "Tax Payabale: $" << calculateTax() << "\n";
    }
    // Override calculateTax for Car
    float calculateTax() const override{
        return price * 0.10f; // Tax is 10% of the car's price
    }
};

// Derived class Truck
class Truck : public Vehicle {
private:
    int payload;
    int towingCapacity;
public:
    // Setters
    void setPayload(int p) { payload = p; }
    void setTowingCapacity(int t) { towingCapacity = t; }

    // Getters
    int getPayload() const { return payload; }
    int getTowingCapacity() const { return towingCapacity;}

    // OVerride displayDetails()
    void displayDetails() const override{
        Vehicle::displayDetails();
        cout << "Payload: " << payload << " lbs\n";
        cout << "Towing Capacity: " << towingCapacity << " lbs\n";
        cout << "Tax Payable: $" << calculateTax() << "\n";
    }

    // Override calculateTax for Truck
    float calculateTax() const override{
        return price * 0.12f; // Tax is 12% of the truck's price
    }
};

// Fleet Management class to manage adding, removing and searching for vehicle in the fleet
class FleetManagement {
private:
    // Array to hold pointers to Vehicle objects, with a max capacity of MAX_FLEET_SIZE
    Vehicle* fleet[MAX_FLEET_SIZE];
    // Count to track how many vehicles are in the fleet
    int vehicleCount;

public:
    // Constructor initializes vehicleCount to 0, meaning no vehicles in the fleet initially
    FleetManagement() : vehicleCount(0) {}

    // Add a vehicle to the fleet
    void addVehicle(Vehicle* v) {
        // Check if fleet is within its maximum capacity
        if (vehicleCount < MAX_FLEET_SIZE) {
            // Add the vehicle to the array and increment the count
            fleet[vehicleCount++] = v;
            cout << "Vehicle added to fleet.\n";
        } else {
            // If fleet is full, print an error message
            cout << "Fleet is full!\n";
        }
    }

    // Remove a vehicle by make and model
    void removeVehicle(const string& make, const string& model) {
        // Loop through the fleet to find a vehicle matching the make and model
        for (int i = 0; i < vehicleCount; ++i)
        {   // Compare the current vehicle's make and model
            if (fleet[i]->getMake() == make && fleet[i]->getModel() == model)
            {   // if a match is found, delete the vehicle object to free memory
                delete fleet[i];

                // Replace the deleted vehicle with the last vehicle in the array to maintain continuity
                fleet[i] = fleet[vehicleCount - 1];

                // Decrease the vehicle count to reflect the removed vehicle
                vehicleCount--;
                cout << "Vehicle removed from fleet.\n";
                return; // Exit after removing the vehicle
            }
            
        }
        // If no matching vehicle is found, print a message
        cout << "Vehicle not found.\n";
    }

    // Search for a vehicle by make and model
    void searchVehicle(const string& make, const string& model) const {
        // Loop through the fleet to find a vehicle matching the make and model
        for (int i = 0; i < vehicleCount; ++i){
            // Compare the current vehicle's make and model
            if (fleet[i]->getMake() == make && fleet[i]->getModel() == model)
            {   // If a match is found, display the details of the vehicle
                cout << "Vehicle Information:\n";
                fleet[i]->displayDetails();
                return; // Exit after displaying the vehicle details
            }
            
        }
        // If no matching vehicle is found, print a message
        cout << "Vehicle not found.\n";
    }

    // Destructor to clean up dynamically allocated memory
    ~FleetManagement() {
        // Loop through each vehicle in the fllet and delete it to prevent memory leaks
        for (int i = 0; i < vehicleCount; ++i) {
            delete fleet[i];
        }
    }
};

int main() {
    FleetManagement fleetManager;
    int choice;

    while (true)
    {
        cout << "\nMenu:\n1. Add Vehicle\n2. Remove Vehicle\n3. Search for Vehicle\n4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string type, make, model;
            int year, numDoors, payload, towingCapacity;
            double price;

            cout << "Enter vehicle type ('car'/'truck): ";
            cin >> type;

            cout << "Enter vehicle make: ";
            cin >> make;

            cout << "Enter vehicle model: ";
            cin >> model;

            cout << "Enter vehicle year: ";
            cin >> year;

            cout << "Enter vehicle price: ";
            cin >> price;

            if(type == "car") {
                Car* car = new Car();
                car->setMake(make);
                car->setModel(model);
                car->setYear(year);
                car->setPrice(price);

                cout << "Enter number of doors: ";
                cin >> numDoors;
                car->setNumDoors(numDoors);

                fleetManager.addVehicle(car);
            } else if (type == "truck") {
                Truck* truck = new Truck();
                truck->setMake(make);
                truck->setModel(model);
                truck->setYear(year);
                truck->setPrice(price);

                cout << "Enter payload in pounds: ";
                cin >> payload;
                truck->setPayload(payload);

                cout << "Enter towing capacity in pounds: ";
                cin >> towingCapacity;
                truck->setTowingCapacity(towingCapacity);

                fleetManager.addVehicle(truck);  
            } else {
                cout << "Invalid vehicle type!\n";
            }
        } else if (choice == 2) {
            string make, model;
            cout << "Enter vehicle make: ";
            cin >> make;
            cout << "Enter vehicle model: ";
            cin >> model;

            fleetManager.removeVehicle(make, model);
        } else if (choice == 3) {
            string make, model;
            cout << "Enter vehicle make: ";
            cin >> make;
            cout << "Enter vehicle model: ";
            cin >> model;

            fleetManager.searchVehicle(make, model);
        } else if (choice == 4) {
            cout << "Existing program.\n";
            break;
        } else {
            cout << "Invalid choice! Please return...\n";
        }
    }
    return 0;
    
}

