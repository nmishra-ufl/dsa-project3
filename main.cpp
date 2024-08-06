#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>
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
                 unordered_map<int, vector<Car>>& forwardGearMap,
                 unordered_map<int, vector<Car>>& mpgMap,
                 unordered_map<int, vector<Car>>& highwayMPGMap,
                 unordered_map<int, vector<Car>>& torqueMap,
                 unordered_map<string, vector<Car>>& driveLineMap,
                 unordered_map<string, vector<Car>>& fuelTypeMap,
                 unordered_map<string, vector<Car>>& classificationMap) {
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
        mpgMap[car.mpg].push_back(car);
        highwayMPGMap[car.highwayMPG].push_back(car);
        torqueMap[car.torque].push_back(car);
        driveLineMap[car.driveLine].push_back(car);
        fuelTypeMap[car.fuelType].push_back(car);
        classificationMap[car.classification].push_back(car);


        cars.push_back(car);
    }
    file.close();
    return cars;
}

double calculateWeightedSum(const Car& car, const unordered_map<string, double>& weights) {
    double sum = 0.0;
    // ((value - (min - 1))/ (max - (min - 1))) * weight
    sum += ((car.mpg - 7.0)/ 31) * weights.at("mpg");
    sum += ((car.highwayMPG - 10.0)/ 33) * weights.at("highway mpg");
    sum += ((car.year - 2008.0)/4) * weights.at("year");
    sum += ((car.horsePower - 99.0)/539) * weights.at("horsePower");
    sum += ((car.torque - 97.0)/677) * weights.at("torque");
    cout << car.mpg << " " << sum << endl;
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
                       const unordered_map<int, vector<Car>>& mpgMap,
                       const unordered_map<int, vector<Car>>& highwayMPGMap,
                       const unordered_map<int, vector<Car>>& torqueMap,
                       const unordered_map<string, vector<Car>>& driveLineMap,
                       const unordered_map<string, vector<Car>>& fuelTypeMap,
                       const unordered_map<string, vector<Car>>& classificationMap,
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
            }
            else {
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
            }
            else {
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
            }
            else {
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
            }
            else {
                results = correctResults(results, it->second);
            }
        }
    }
    if (criteriaInt.find("mpg") != criteriaInt.end()) {
        auto it = mpgMap.find(criteriaInt.at("mpg"));
        if (it != mpgMap.end()) {
            if (first) {
                results = it->second;
                first = false;
            }
            else {
                results = correctResults(results, it->second);
            }
        }
    }
    if (criteriaInt.find("highwayMPG") != criteriaInt.end()) {
        auto it = highwayMPGMap.find(criteriaInt.at("highwayMPG"));
        if (it != highwayMPGMap.end()) {
            if (first) {
                results = it->second;
                first = false;
            }
            else {
                results = correctResults(results, it->second);
            }
        }
    }
    if (criteriaInt.find("torque") != criteriaInt.end()) {
        auto it = torqueMap.find(criteriaInt.at("torque"));
        if (it != torqueMap.end()) {
            if (first) {
                results = it->second;
                first = false;
            }
            else {
                results = correctResults(results, it->second);
            }
        }
    }

    if (criteria.find("driveLine") != criteria.end()) {
        auto it = driveLineMap.find(criteria.at("driveLine"));
        if (it != driveLineMap.end()) {
            if (first) {
                results = it->second;
                first = false;
            }
            else {
                results = correctResults(results, it->second);
            }
        }
    }
    if (criteria.find("fuelType") != criteria.end()) {
        auto it = fuelTypeMap.find(criteria.at("fuelType"));
        if (it != fuelTypeMap.end()) {
            if (first) {
                results = it->second;
                first = false;
            }
            else {
                results = correctResults(results, it->second);
            }
        }
    }
    if (criteria.find("classification") != criteria.end()) {
        auto it = classificationMap.find(criteria.at("classification"));
        if (it != classificationMap.end()) {
            if (first) {
                results = it->second;
                first = false;
            }
            else {
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

enum State {

    WELCOME,

    RANKING,

    SEARCH,

    INPUT,

    RESULT,

    RANKINGRESULT,

    EXIT

};

void setText(sf::Text& text, float x, float y) {
    text.setPosition(x, y);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left, textRect.top);
}

vector<string> wrapText(const string& text, const sf::Font& font, unsigned int characterSize, float maxWidth) {
    vector<string> wrappedText;
    string currentLine;
    stringstream ss(text);
    string word;

    while (ss >> word) {
        sf::Text tempText(currentLine + " " + word, font, characterSize);
        if (tempText.getLocalBounds().width > maxWidth) {
            wrappedText.push_back(currentLine);
            currentLine = word;
        } else {
            if (!currentLine.empty()) {
                currentLine += " ";
            }
            currentLine += word;
        }
    }
    wrappedText.push_back(currentLine);
    return wrappedText;
}

int main() {
    string filename = "C:/Users/nzxtpc/OneDrive - University of Florida/cop3530/finalproject/cars.csv";
    unordered_map<string, vector<Car>> makeMap;
    unordered_map<string, vector<Car>> modelYearMap;
    unordered_map<int, vector<Car>> yearMap;
    unordered_map<int, vector<Car>> horsePowerMap;
    unordered_map<int, vector<Car>> forwardGearMap;
    unordered_map<int, vector<Car>> mpgMap;
    unordered_map<int, vector<Car>> highwayMPGMap;
    unordered_map<int, vector<Car>> torqueMap;
    unordered_map<string, vector<Car>> driveLineMap;
    unordered_map<string, vector<Car>> fuelTypeMap;
    unordered_map<string, vector<Car>> classificationMap;

    vector<Car> cars = read(filename, makeMap, modelYearMap, yearMap, horsePowerMap, forwardGearMap, mpgMap, highwayMPGMap, torqueMap, driveLineMap, fuelTypeMap, classificationMap);
//    sf::Texture backgroundTexture;
//    sf::Sprite backgroundSprite;
//
//    if (!backgroundTexture.loadFromFile("C:/Users/HP/CLionProjects/Project3/bluecar.png")) {
//        cout << "Failed to load background image" << endl;
//        return -1;
//    }
//    backgroundSprite.setTexture(backgroundTexture);

    sf::RenderWindow window(sf::VideoMode(1200, 900), "AutoSearch Vehicle Selection Assistant");

    // Load font
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        cout << "Failed to load font" << endl;
        return -1;
    }

    sf::Text welcomeText("Welcome to the AutoSearch Vehicle Selection Assistant!\n\n"
                         "Press 'R' to input ranking criteria.\n"
                         "Press 'I' to input search criteria.\n"
                         "Press 'Esc' to exit.", font, 24);
    welcomeText.setFillColor(sf::Color::Black);
    setText(welcomeText, 20, 20);

    string userInput;
    sf::Text inputText("", font, 18);
    inputText.setFillColor(sf::Color::Black);
    inputText.setStyle(sf::Text::Bold);
    setText(inputText, 20, window.getSize().y - 80);

    bool showCursor = true;
    sf::Clock cursorClock;
    sf::RectangleShape cursor(sf::Vector2f(2.f, inputText.getCharacterSize()));
    cursor.setFillColor(sf::Color::Black);

    State currentState = WELCOME;
    bool needsUpdate = true;

    unordered_map<string, string> searchCriteria;
    unordered_map<string, int> intSearchCriteria;
    vector<Car> searchResults;

    sf::Text searchCriteriaText("Please select your search criteria.\nYou can choose up to four categories.\n"
                                "Here are the categories: \n"
                                "DriveLine (All-wheel drive, Front-wheel drive, Rear-wheel drive, Four-wheel drive),\n"
                                "Number of Forward Gears (4-8), MPG (8-38), Fuel Type (Gasoline or E85),\n"
                                "Highway MPG (11-43), Classification (Automatic transmission or Manual transmission),\n"
                                "Make (Type in brand with front capital), Model Year,\n"
                                "Year (2009-2012), Horsepower (100 - 638), Torque (98 - 774). \n\n"
                                "To choose DriveLine type 'driveLine', To choose make type 'make', To choose model year type 'modelYear',\n"
                                "To choose forward gears type 'forwardGear', To choose year type 'year', \nTo choose horsepower type 'horsePower',"
                                "To choose MPG type 'mpg', To choose highway MPG type 'highwayMPG', \nTo choose torque type 'torque'." , font, 18);
    searchCriteriaText.setFillColor(sf::Color::Black);
    setText(searchCriteriaText, 20, 100);


    string currentCategory;
    bool enteringCategory = true;
    int validCategoryCount = 0;
    const int maxCategories = 4;

    sf::Text sortCriteriaText("sort criteria text", font, 18);
    sortCriteriaText.setFillColor(sf::Color::Black);
    setText(sortCriteriaText, 20, 100);

    string currentSort;
    bool enteringSort = true;
    int validSortCount = 0;
    const int maxSorts = 5;
    vector<string> sortOrder;

    sf::View view = window.getDefaultView();
    float scroll = 23.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (currentState == WELCOME) {
                    if (event.key.code == sf::Keyboard::R) {
                        currentState = RANKING;
                        needsUpdate = true;
                    }
                    else if (event.key.code == sf::Keyboard::I) {
                        currentState = INPUT;
                        needsUpdate = true;
                    }
                }
                else if (currentState == INPUT) {
                    if (event.key.code == sf::Keyboard::Return) {
                        if (enteringCategory) {
                            if (userInput == "done" || validCategoryCount >= maxCategories) {
                                searchResults = searchCars(makeMap, modelYearMap, yearMap, horsePowerMap, forwardGearMap, mpgMap, highwayMPGMap, torqueMap, driveLineMap, fuelTypeMap, classificationMap, searchCriteria, intSearchCriteria);
                                currentState = RESULT;
                                needsUpdate = true;
                            } else {
                                currentCategory = userInput;
                                enteringCategory = false;
                                userInput.clear();
                            }
                        }
                        else {
                            bool validCategory = false;
                            if (currentCategory == "make" || currentCategory == "modelYear" || currentCategory == "driveLine" || currentCategory == "fuelType" || currentCategory == "classification") {
                                searchCriteria[currentCategory] = userInput;
                                validCategory = true;
                            }
                            else if (currentCategory == "year" || currentCategory == "horsePower" || currentCategory == "forwardGear" || currentCategory == "mpg" || currentCategory == "highwayMPG" || currentCategory == "torque") {
                                intSearchCriteria[currentCategory] = stoi(userInput);
                                validCategory = true;
                            }
                            else {
                                cout << "Invalid category." << endl;
                            }
                            if (validCategory) {
                                validCategoryCount++;
                                enteringCategory = true;
                                userInput.clear();
                            }
                        }
                        needsUpdate = true;
                    }
                    else if (event.key.code == sf::Keyboard::BackSpace) {
                        if (!userInput.empty()) {
                            userInput.pop_back();
                            needsUpdate = true;
                        }
                    }
                }










                else if (currentState == RANKING) {
                    if (event.key.code == sf::Keyboard::Return) {
                        if (userInput == "done" || validSortCount >= maxSorts) {
                            unordered_map<string, double> weights = {
                                {"mpg", 0.0},
                                {"highway mpg", 0.0},
                                {"year", 0.0},
                                {"horsePower", 0.0},
                                {"torque", 0.0}
                            };
                            if(sortOrder.empty()) {
                                weights = {
                                    {"mpg", 1.0},
                                    {"highway mpg", 1.0},
                                    {"year", 1.0},
                                    {"horsePower", 1.0},
                                    {"torque", 1.0}
                                };
                            }else {
                                for(int i = 0;i<sortOrder.size();i++) {
                                    weights[sortOrder[i]] = (1/(pow(1.3,i)));
                                }
                            }
                            rankCars(cars, weights);
                            currentState = RANKINGRESULT;
                            needsUpdate = true;
                        } else if (userInput == "mpg" || userInput == "highway mpg" || userInput == "horsepower" || userInput == "torque" || userInput == "year"){
                            sortOrder.push_back(userInput);
                            validSortCount++;
                            userInput.clear();
                        }else {
                            cout << "Invalid category." << endl;
                            userInput.clear();
                        }
                        needsUpdate = true;
                    }
                    else if (event.key.code == sf::Keyboard::BackSpace) {
                        if (!userInput.empty()) {
                            userInput.pop_back();
                            needsUpdate = true;
                        }
                    }
                }
                else if ((currentState == RESULT) || (currentState == RANKINGRESULT)) {
                    if (event.key.code == sf::Keyboard::B) {
                        currentState = WELCOME;
                        needsUpdate = true;
                        validCategoryCount = 0;
                        searchCriteria.clear();
                        intSearchCriteria.clear();
                        validSortCount = 0;
                        sortOrder.clear();
                        userInput.clear();
                    }
                }
            }
            else if (event.type == sf::Event::TextEntered) {
                if (currentState == INPUT || RANKING) {
                    if (event.text.unicode < 128 && event.text.unicode > 31) {
                        userInput += static_cast<char>(event.text.unicode);
                        needsUpdate = true;
                    }
                }
            }
            else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    view.move(0, -scroll);
                }
                else if (event.mouseWheelScroll.delta < 0) {
                    view.move(0, scroll);
                }
                needsUpdate = true;
            }
        }

        if (cursorClock.getElapsedTime().asSeconds() >= 0.5f) {
            showCursor = !showCursor;
            cursorClock.restart();
            needsUpdate = true;
        }

        if (needsUpdate) {
            window.clear(sf::Color::White);
            window.setView(view);

            if (currentState == WELCOME) {
//                window.draw(backgroundSprite);
                window.draw(welcomeText);
            } else if (currentState == INPUT) {
                window.draw(searchCriteriaText);
                if (enteringCategory) {
                    inputText.setString("Enter category (or 'done' to finish, " + to_string(maxCategories - validCategoryCount) + " remaining): " + userInput);
                } else {
                    inputText.setString("Enter value for " + currentCategory + ": " + userInput);
                }
                window.draw(inputText);

                if (showCursor) {
                    float cursorX = inputText.getPosition().x + inputText.getLocalBounds().width + 5;
                    float cursorY = inputText.getPosition().y;
                    cursor.setPosition(cursorX, cursorY);
                    window.draw(cursor);
                }
            }
            else if (currentState == RANKING) {

                window.draw(sortCriteriaText);
                if (enteringSort) {
                    inputText.setString("Enter ranking category (or 'done' to finish, " + to_string(maxSorts - validSortCount) + " remaining): " + userInput);
                }
                window.draw(inputText);

                if (showCursor) {
                    float cursorX = inputText.getPosition().x + inputText.getLocalBounds().width + 5;
                    float cursorY = inputText.getPosition().y;
                    cursor.setPosition(cursorX, cursorY);
                    window.draw(cursor);
                }
            }
            else if (currentState == RESULT) {
                sf::Text resultText("Search Results:\n", font, 24);
                resultText.setFillColor(sf::Color::Black);
                setText(resultText, 20, 20);
                window.draw(resultText);

                sf::Text carText("", font, 18);
                carText.setFillColor(sf::Color::Black);
                float yPosition = 80.0f;
                float maxWidth = window.getSize().x - 20;

                for (const auto& car : searchResults) {
                    string carDetails = "ID: " + car.ID + ", Make: " + car.make + ", Model Year: " + car.modelYear +
                                        ", MPG: " + to_string(car.mpg) + ", Highway MPG: " + to_string(car.highwayMPG) +
                                        ", Year: " + to_string(car.year) + ", HorsePower: " + to_string(car.horsePower) +
                                        ", Torque: " + to_string(car.torque);
                    vector<string> wrappedText = wrapText(carDetails, font, 18, maxWidth);
                    for (const auto &line : wrappedText) {
                        carText.setString(line);
                        carText.setPosition(20, yPosition);
                        window.draw(carText);
                        yPosition += 30;
                    }
                    yPosition += 10;
                }

                sf::Text instructionText("Press 'B' to go back to the main menu.\nPress 'Esc' to exit.", font, 24);
                instructionText.setFillColor(sf::Color::Black);
                setText(instructionText, window.getSize().x / 2.0f, yPosition + 50);
                window.draw(instructionText);
            }
            else if (currentState == RANKINGRESULT) {

                sf::Text carText("", font, 18);
                carText.setFillColor(sf::Color::Black);
                float yPosition = 50.0f;
                float maxWidth = window.getSize().x - 20;

                for (size_t i = 0; i < 20 && i < cars.size(); ++i) {
                    string carDetails = "ID: " + cars[i].ID + ", Make: " + cars[i].make + ", Model Year: " + cars[i].modelYear +
                                        ", MPG: " + to_string(cars[i].mpg) + ", Highway MPG: " + to_string(cars[i].highwayMPG) +
                                        ", Year: " + to_string(cars[i].year) + ", HorsePower: " + to_string(cars[i].horsePower) +
                                        ", Torque: " + to_string(cars[i].torque);
                    vector<string> wrappedText = wrapText(carDetails, font, 18, maxWidth);
                    for (const auto &line : wrappedText) {
                        carText.setString(line);
                        carText.setPosition(20, yPosition);
                        window.draw(carText);
                        yPosition += 30;
                    }
                    yPosition += 10;
                }

                sf::Text instructionText("Press 'B' to go back to the main menu.\nPress 'Esc' to exit.", font, 24);
                instructionText.setFillColor(sf::Color::Black);
                setText(instructionText, window.getSize().x / 2.0f, yPosition + 50);
                window.draw(instructionText);

            }

            window.display();
            needsUpdate = false;
        }
    }

    return 0;
}

