#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
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

vector<string> csvLines(const string& line){
    vector<string> information;
    string info;
    bool quotes = false;
    for(char c : line){
        if (c == '"'){
            quotes = !quotes;
        }
        else if(c == ',' && !quotes ){
            information.push_back(info);
            info.clear();
        }
        else{
            info += c;
        }
    }
    information.push_back(info);
    return information;
}

vector<Car> read(const string& filename){
    vector<Car> cars;
    ifstream file(filename);
    if (!file.is_open()){
        cout << "Failed to open file" << endl;
        return cars;
    }

    string line;
    if (getline(file, line)) {

    }
    while (getline(file, line)){
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
bool compareCars(const Car& a, const Car& b, const vector<string>& categories) {
    for (const auto& category : categories) {
        if (category == "mpg") {
            if (a.mpg != b.mpg) return a.mpg > b.mpg;
        } else if (category == "highwayMPG") {
            if (a.highwayMPG != b.highwayMPG) return a.highwayMPG > b.highwayMPG;
        } else if (category == "year") {
            if (a.year != b.year) return a.year > b.year;
        } else if (category == "horsePower") {
            if (a.horsePower != b.horsePower) return a.horsePower > b.horsePower;
        } else if (category == "torque") {
            if (a.torque != b.torque) return a.torque > b.torque;
        }
    }
    return false;
}

void rankCars(vector<Car>& cars, const vector<string>& categories) {
    sort(cars.begin(), cars.end(), [&categories](const Car& a, const Car& b) {
        return compareCars(a, b, categories);
    });
}

vector<Car> searchCars(const vector<Car>& cars, const unordered_map<string, string>& searchCriteria) {
    vector<Car> results;
    for (const auto& car : cars) {
        bool match = true;
        for (const auto& [key, value] : searchCriteria) {
            if (key == "driveLine" && car.driveLine != value) match = false;
            if (key == "engineType" && car.engineType != value) match = false;
            if (key == "hybrid" && car.hybrid != value) match = false;
            if (key == "transmission" && car.transmission != value) match = false;
            if (key == "fuelType" && car.fuelType != value) match = false;
            if (key == "classification" && car.classification != value) match = false;
            if (key == "ID" && car.ID != value) match = false;
            if (key == "make" && car.make != value) match = false;
            if (key == "modelYear" && car.modelYear != value) match = false;
        }
        if (match) {
            results.push_back(car);
        }
    }
    return results;
}

void printCars(const vector<Car>& cars) {
    for (const auto& car : cars) {
        cout << "ID: " << car.ID << ", Make: " << car.make << ", Model Year: " << car.modelYear
             << ", MPG: " << car.mpg << ", Highway MPG: " << car.highwayMPG << ", Year: " << car.year
             << ", HorsePower: " << car.horsePower << ", Torque: " << car.torque << endl;
    }
}

int main() {
    string filename = "cars.csv";
    vector<Car> cars = read(filename);

    vector<string> categories = {"mpg", "highwayMPG", "year", "horsePower"};
    rankCars(cars, categories);
    cout << "Top 20 Cars Ranked:" << endl;
    printCars(vector<Car>(cars.begin(), cars.begin() + 20));

    unordered_map<string, string> searchCriteria = {
        {"make", "Toyota"},
        {"modelYear", "2020"}
    };

    vector<Car> searchResults = searchCars(cars, searchCriteria);
    cout << "\nSearch Results:" << endl;
    printCars(searchResults);

    return 0;
}
