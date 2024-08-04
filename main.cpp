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
double calculateWeightedSum(const Car& car, const unordered_map<string, double>& weights) {
    double sum = 0.0;
    sum += car.mpg * weights.at("mpg");
    sum += car.highwayMPG * weights.at("highwayMPG");
    sum += car.year * weights.at("year");
    sum += car.horsePower * weights.at("horsePower");
    sum += car.torque * weights.at("torque");
    return sum;
}

void rankCars(vector<Car>& cars, const unordered_map<string, double>& weights) {
    sort(cars.begin(), cars.end(), [&weights](const Car& a, const Car& b) {
        return calculateWeightedSum(a, weights) > calculateWeightedSum(b, weights);
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

    // User-defined weights for each category
    unordered_map<string, double> weights = {
        {"mpg", 1.0},
        {"highwayMPG", 0.8},
        {"year", 0.5},
        {"horsePower", 0.7},
        {"torque", 0.6}
    };

    rankCars(cars, weights);
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
