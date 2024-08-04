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
int getCategoryValue(const Car& car, const string& category) {
    if (category == "dimensions") return car.dimensions[0] * car.dimensions[1] * car.dimensions[2];
    if (category == "driveLine") return hash<string>{}(car.driveLine);
    if (category == "engineType") return hash<string>{}(car.engineType);
    if (category == "hybrid") return car.hybrid == "Yes" ? 1 : 0;
    if (category == "forwardGear") return car.forwardGear;
    if (category == "transmission") return hash<string>{}(car.transmission);
    if (category == "mpg") return car.mpg;
    if (category == "fuelType") return hash<string>{}(car.fuelType);
    if (category == "highwayMPG") return car.highwayMPG;
    if (category == "classification") return hash<string>{}(car.classification);
    if (category == "ID") return hash<string>{}(car.ID);
    if (category == "make") return hash<string>{}(car.make);
    if (category == "modelYear") return hash<string>{}(car.modelYear);
    if (category == "year") return car.year;
    if (category == "horsePower") return car.horsePower;
    if (category == "torque") return car.torque;
    return 0;
}

int main() {
    string filename = "C:/Users/HP/CLionProjects/Project3/cars.csv";
    vector<Car> cars = read(filename);

    vector<string> categories(4);
    cout << "Enter 4 categories from the following list: dimensions, driveLine, engineType, hybrid, forwardGear, transmission, mpg, fuelType, highwayMPG, classification, ID, make, modelYear, year, horsePower, torque" << endl;
    for (int i = 0; i < 4; ++i) {
        cout << "Enter category " << i + 1 << ": ";
        cin >> categories[i];
    }

    sort(cars.begin(), cars.end(), [&categories](const Car& a, const Car& b) {
        for (const string& category : categories) {
            int valA = getCategoryValue(a, category);
            int valB = getCategoryValue(b, category);
            if (valA != valB) {
                return valA > valB;
            }
        }
        return false;
    });

    cout << "Top 20 cars based on selected categories:" << endl;
    for (int i = 0; i < min(20, (int)cars.size()); ++i) {
        const auto& car = cars[i];
        cout << "Car ID: " << car.ID << ", Make: " << car.make << ", Model Year: " << car.modelYear << ", Horsepower: " << car.horsePower << ", Torque: " << car.torque << endl;
    }

    return 0;
}
