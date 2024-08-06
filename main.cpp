#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
using namespace std;

struct Car {
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
        }
        else if (c == ',' && !quotes) {
            information.push_back(info);
            info.clear();
        }
        else {
            info += c;
        }
    }
    information.push_back(info);
    return information;
}

vector<Car> read(const string& filename,
                 unordered_map<string, vector<Car>>& makeMap,
                 unordered_map<string, vector<Car>>& modelYearMap,
                 unordered_map<int, vector<Car>>& yearMap,
                 unordered_map<int, vector<Car>>& horsePowerMap,
                 unordered_map<int, vector<Car>>& forwardGearMap) {
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

        makeMap[car.make].push_back(car);
        modelYearMap[car.modelYear].push_back(car);
        yearMap[car.year].push_back(car);
        horsePowerMap[car.horsePower].push_back(car);
        forwardGearMap[car.forwardGear].push_back(car);

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

vector<Car> correctResults(const vector<Car>& base, const vector<Car>& additional) {
    unordered_set<string> baseNew;
    for (const auto& car : base) {
        baseNew.insert(car.ID);
    }
    vector<Car> result;
    for (const auto& car : additional) {
        if (baseNew.find(car.ID) != baseNew.end()) {
            result.push_back(car);
        }
    }
    return result;
}

vector<Car> searchCars(const unordered_map<string, vector<Car>>& makeMap,
                       const unordered_map<string, vector<Car>>& modelYearMap,
                       const unordered_map<int, vector<Car>>& yearMap,
                       const unordered_map<int, vector<Car>>& horsePowerMap,
                       const unordered_map<int, vector<Car>>& forwardGearMap,
                       const unordered_map<string, string>& criteria,
                       const unordered_map<string, int>& criteriaInt) {
    vector<Car> results;
    bool first = true;

    if (criteria.find("make") != criteria.end()) {
        auto it = makeMap.find(criteria.at("make"));
        if (it != makeMap.end()) {
            if (first) {
                results = it->second;
                first = false;
            }
            else {
                results = correctResults(results, it->second);
            }
        }
    }
    if (criteria.find("modelYear") != criteria.end()) {
        auto it = modelYearMap.find(criteria.at("modelYear"));
        if (it != modelYearMap.end()) {
            if (first) {
                results = it->second;
                first = false;
            } else {
                results = correctResults(results, it->second);
            }
        }
    }
    if (criteriaInt.find("year") != criteriaInt.end()) {
        auto it = yearMap.find(criteriaInt.at("year"));
        if (it != yearMap.end()) {
            if (first) {
                results = it->second;
                first = false;
            } else {
                results = correctResults(results, it->second);
            }
        }
    }
    if (criteriaInt.find("horsePower") != criteriaInt.end()) {
        auto it = horsePowerMap.find(criteriaInt.at("horsePower"));
        if (it != horsePowerMap.end()) {
            if (first) {
                results = it->second;
                first = false;
            } else {
                results = correctResults(results, it->second);
            }
        }
    }
    if (criteriaInt.find("forwardGear") != criteriaInt.end()) {
        auto it = forwardGearMap.find(criteriaInt.at("forwardGear"));
        if (it != forwardGearMap.end()) {
            if (first) {
                results = it->second;
                first = false;
            } else {
                results = correctResults(results, it->second);
            }
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
    string filename = "C:/Users/HP/CLionProjects/Project3/cars.csv";

    unordered_map<string, vector<Car>> makeMap;
    unordered_map<string, vector<Car>> modelYearMap;
    unordered_map<int, vector<Car>> yearMap;
    unordered_map<int, vector<Car>> horsePowerMap;
    unordered_map<int, vector<Car>> forwardGearMap;

    vector<Car> cars = read(filename, makeMap, modelYearMap, yearMap, horsePowerMap, forwardGearMap);

    cout << "Welcome to the AutoSearch Vehicle Selection Assistant!" << endl;

    unordered_map<string, double> weights = {
            {"mpg", 1.0},
            {"highwayMPG", 0.8},
            {"year", 0.5},
            {"horsePower", 0.7},
            {"torque", 0.6}
    };

    string optionDisplay;
    cout << "Would you like to display the current top 20 ranked cars? (yes/no)" << endl;
    cin >> optionDisplay;
    if (optionDisplay == "yes" || optionDisplay == "Yes"){
        cout << "Here are the current Top 20 Cars Ranked:" << endl;
        rankCars(cars, weights);
        printCars(vector<Car>(cars.begin(), cars.begin() + 20));
    }

    cout << "\nPlease select your search criteria" << endl;
    cout << "You can choose up to four categories" << endl;
    cout << "Here are the categories: DriveLine, Engine Type, Hybrid, "
            "Number of Forward Gears, Transmission, MPG, Fuel Type, "
            "Highway MPG, Classification, Make, Model Year, "
            "Year, Horsepower, Torque." << endl;


    unordered_map<string, string> searchCriteria;
    unordered_map<string, int> intSearchCriteria;
    string category;
    string value;
    int intValue;
    for (int i = 0; i < 4; i++) {
        cout << "Enter category (or 'done' to finish): ";
        cin >> category;
        if (category == "done") {
            break;
        }
        if (category == "make" || category == "modelYear") {
            cout << "Enter value: ";
            cin >> value;
            searchCriteria[category] = value;
        }
        else if (category == "year" || category == "horsePower" || category == "forwardGear") {
            cout << "Enter value: ";
            cin >> intValue;
            intSearchCriteria[category] = intValue;
        }
        else {
            cout << "Invalid category." << endl;
        }
    }

    vector<Car> searchResults = searchCars(makeMap, modelYearMap, yearMap, horsePowerMap, forwardGearMap, searchCriteria, intSearchCriteria);
    cout << "\nSearch Results:" << endl;
    printCars(searchResults);

    return 0;
}
