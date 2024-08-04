#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

struct Car{
    vector<int> dimensions;
    string driveLine;
    string engineType;
    string hybrid;
    int forwardGear;
    string transmission;
    int mpg;
    string fuelType;
    int highwayMPG;
    string classification;
    string ID;
    string make;
    string modelYear;
    int year;
    int horsePower;
    int torque;


};

vector<string> csvLines(const string& line) {
    vector<string> information;
    string info;
    bool quotes = false;
    for (char c : line) {
        if (c == '"') {
            quotes = !quotes;
        } else if (c == ',' && !quotes) {
            information.push_back(info);
            info.clear();
        } else {
            info += c;
        }
    }
    information.push_back(info);
    return information;
}

vector<Car> read(const string& filename) {
    vector<Car> cars;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file" << endl;
        return cars;
    }


    string line;
    if (getline(file, line)) {
}
while (getline(file, line)) {
vector<string> information = csvLines(line);
Car car;
car.dimensions.push_back(stoi(information[0]));
car.dimensions.push_back(stoi(information[1]));
car.dimensions.push_back(stoi(information[2]));
car.driveLine = information[3];
car.engineType = information[4];
car.hybrid = information[5];
car.forwardGear = stoi(information[6]);
car.transmission = information[7];
car.mpg = stoi(information[8]);
car.fuelType = information[9];
car.highwayMPG = stoi(information[10]);
car.classification = information[11];
car.ID = information[12];
car.make = information[13];
car.modelYear = information[14];
car.year = stoi(information[15]);
car.horsePower = stoi(information[16]);
car.torque = stoi(information[17]);
cars.push_back(car);
}
file.close();
return cars;
}
void displayMenu() {
    cout << "\nVehicle Selection Assistant Menu:\n";
    cout << "1. Search for a car\n";
    cout << "2. Sort cars\n";
    cout << "3. Filter cars\n";
    cout << "4. Exit\n";
}


void searchCar(const vector<Car>& cars) {
    string query;
    cout << "Enter the car ID, make, or model year to search: ";
    cin.ignore();
    getline(cin, query);

    for (const auto& car : cars) {
        if (car.ID == query || car.make == query || car.modelYear == query) {
            cout << "Car found: " << car.ID << ", " << car.make << ", " << car.modelYear << endl;
            return;
        }
    }
    cout << "Car not found." << endl;
}

void sortCars(vector<Car>& cars) {
    int sortOption;
    cout << "Sort by: 1. Year 2. HorsePower 3. MPG\nEnter option: ";
    cin >> sortOption;


    switch (sortOption) {
        case 1:
            sort(cars.begin(), cars.end(), [](const Car& a, const Car& b) {
                return a.year < b.year;
            });
            break;
        case 2:
            sort(cars.begin(), cars.end(), [](const Car& a, const Car& b) {
                return a.horsePower < b.horsePower;
            });
            break;
        case 3:
            sort(cars.begin(), cars.end(), [](const Car& a, const Car& b) {
                return a.mpg < b.mpg;
            });
            break;
        default:
            cout << "Invalid option." << endl;
            return;
    }

    cout << "Cars sorted successfully." << endl;
}

void filterCars(const vector<Car>& cars) {
    int filterOption;
    cout << "Filter by: 1. Fuel Type 2. Drive Line\nEnter option: ";
    cin >> filterOption;
    string query;
    cout << "Enter the value to filter: ";
    cin.ignore();
    getline(cin, query);

    for (const auto& car : cars) {
        if ((filterOption == 1 && car.fuelType == query) ||
            (filterOption == 2 && car.driveLine == query)) {
            cout << "Car: " << car.ID << ", " << car.make << ", " << car.modelYear << endl;
        }
    }
}
int main() {
    string filename = "C:/Users/HP/CLionProjects/Project3/cars.csv";
    vector<Car> cars = read(filename);
    int user_choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> user_choice;

        switch (user_choice) {
            case 1:
                searchCar(cars);
                break;
            case 2:
                sortCars(cars);
                break;
            case 3:
                filterCars(cars);
                break;
            case 4:
                cout << "Program Exit" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (user_choice != 4);

    return 0;
}
