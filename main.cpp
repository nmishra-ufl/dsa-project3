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


int main() {
    string filename = "C:/Users/HP/CLionProjects/Project3/cars.csv";
    vector<Car> cars = read(filename);
    for (const auto& car : cars) {
        std::cout << "Description: " << car.ID << ", Fuel Usage (City): " << car.mpg
                  << ", Fuel Usage (Highway): " << car.highwayMPG
                  << ", Torque: " << car.torque
                  << ", Horsepower: " << car.horsePower << std::endl;
    }
    return 0;
}
