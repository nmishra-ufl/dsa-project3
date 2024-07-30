#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

struct Car{
    vector<int> dimensions;
    string driveLine;
    string engineType;
    bool hybrid;
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

void loadCars(unordered_map<string, Car>& cars);
void displayMenu();
void handleSearch(const unordered_map<string, Car>& cars);
void handleSort(vector<Car>& cars);
void handleFilter(const unordered_map<string, Car>& cars);
void displayCar(const Car& car);


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
