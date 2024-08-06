#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
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

enum State {

        WELCOME,

        RANKING,

        SEARCH,

        INPUT,

        RESULT,

        EXIT

};

void setText(sf::Text& text, float x, float y) {
    text.setPosition(x, y);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
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

    string filename = "C:/Users/asrit/Downloads/cars.csv";

    unordered_map<string, vector<Car>> makeMap;
    unordered_map<string, vector<Car>> modelYearMap;
    unordered_map<int, vector<Car>> yearMap;
    unordered_map<int, vector<Car>> horsePowerMap;
    unordered_map<int, vector<Car>> forwardGearMap;

    vector<Car> cars = read(filename, makeMap, modelYearMap, yearMap, horsePowerMap, forwardGearMap);
    sf::RenderWindow window(sf::VideoMode(1200, 900), "AutoSearch Vehicle Selection Assistant");



    // Load font
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {  // Ensure this path is correct
        cout << "Failed to load font" << endl;
        return -1;
    }

    // Text for displaying welcome message and options
    sf::Text welcomeText("Welcome to the AutoSearch Vehicle Selection Assistant!\n\n"
                         "Press 'I' to input search criteria.\n"
                         "Press 'Esc' to exit.", font, 24);
    welcomeText.setFillColor(sf::Color::White);
    setText(welcomeText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 100);

    // Text for displaying user input
    string userInput;
    sf::Text inputText("", font, 18);
    inputText.setFillColor(sf::Color::Yellow);
    inputText.setStyle(sf::Text::Bold);
    setText(inputText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 45);

    bool showCursor = true;
    sf::Clock cursorClock;
    sf::RectangleShape cursor(sf::Vector2f(2.f, inputText.getCharacterSize()));
    cursor.setFillColor(sf::Color::White);

    State currentState = WELCOME;
    bool needsUpdate = true;

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
                    } else if (event.key.code == sf::Keyboard::I){
                        currentState = INPUT;
                        needsUpdate = true;
                    }
                } else if (currentState == INPUT) {
                    if (event.key.code == sf::Keyboard::Return) {
                        cout << "User input: " << userInput << endl;
                        currentState = RESULT;
                        needsUpdate = true;
                    } else if (event.key.code == sf::Keyboard::BackSpace) {
                        if (!userInput.empty()) {
                            userInput.pop_back();
                            needsUpdate = true;
                        }
                    }
                } else if (currentState == RESULT) {
                    if (event.key.code == sf::Keyboard::B) {
                        currentState = WELCOME;
                        needsUpdate = true;
                    }
                }
            } else if (event.type == sf::Event::TextEntered) {
                if (currentState == INPUT) {
                    if (event.text.unicode < 128 && event.text.unicode > 31) { // ASCII range for printable characters
                        userInput += static_cast<char>(event.text.unicode);
                        needsUpdate = true;
                    }
                }
            }
        }

        if (cursorClock.getElapsedTime().asSeconds() >= 0.5f) {
            showCursor = !showCursor;
            cursorClock.restart();
            needsUpdate = true;
        }

        if (needsUpdate) {
            window.clear(sf::Color::Blue);

            if (currentState == WELCOME) {
                window.draw(welcomeText);
            } else if (currentState == INPUT) {
                inputText.setString("Enter search criteria (e.g., make): "+ userInput);
                window.draw(inputText);

                if (showCursor) {
                    float cursorX = inputText.getPosition().x + inputText.getLocalBounds().width / 2.0f + 5;
                    float cursorY = inputText.getPosition().y - inputText.getCharacterSize() / 2.0f;
                    cursor.setPosition(cursorX, cursorY);
                    window.draw(cursor);
                }
            } else if (currentState == RANKING) {

                rankCars(cars, {
                        {"mpg", 1.0},
                        {"highwayMPG", 0.8},
                        {"year", 0.5},
                        {"horsePower", 0.7},
                        {"torque", 0.6}
                });

                sf::Text carText("", font, 18);
                carText.setFillColor(sf::Color::White);
                float yPosition = 50.0f;
                float maxWidth = window.getSize().x - 20;

                for (size_t i = 0; i < 20 && i < cars.size(); ++i) {
                    string carDetails = "ID: " + cars[i].ID + ", Make: " + cars[i].make + ", Model Year: " + cars[i].modelYear +
                                        ", MPG: " + to_string(cars[i].mpg) + ", Highway MPG: " + to_string(cars[i].highwayMPG) +
                                        ", Year: " + to_string(cars[i].year) + ", HorsePower: " + to_string(cars[i].horsePower) +
                                        ", Torque: " + to_string(cars[i].torque);
                    vector<string> wrappedText = wrapText(carDetails, font, 18, maxWidth);
                    for (const auto &line: wrappedText) {
                        carText.setString(line);
                        carText.setPosition(10, yPosition);
                        window.draw(carText);
                        yPosition += 30;
                    }
                    yPosition += 10;
                }

                sf::Text instructionText("Press 'B' to go back to the main menu.\nPress 'Esc' to exit.", font, 24);
                instructionText.setFillColor(sf::Color::White);
                setText(instructionText, window.getSize().x / 2.0f, yPosition + 50);
                window.draw(instructionText);
            } else if (currentState == RESULT) {
                sf::Text resultText("Search Results for \"" + userInput + "\":\n\n... (Display results here)\n\n"
                                                                          "Press 'B' to go back to the main menu.\n"
                                                                          "Press 'Esc' to exit.", font, 24);
                resultText.setFillColor(sf::Color::White);
                setText(resultText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 100);
                window.draw(resultText);
            }

            window.display();
            needsUpdate = false;
        }
    }

    return 0;
}
















/*int main() {
    string filename = "C:/Users/asrit/Downloads/cars.csv";

    unordered_map<string, vector<Car>> makeMap;
    unordered_map<string, vector<Car>> modelYearMap;
    unordered_map<int, vector<Car>> yearMap;
    unordered_map<int, vector<Car>> horsePowerMap;
    unordered_map<int, vector<Car>> forwardGearMap;

    vector<Car> cars = read(filename, makeMap, modelYearMap, yearMap, horsePowerMap, forwardGearMap);


    sf::RenderWindow window(sf::VideoMode(1700, 800), "AutoSearch Vehicle Selection Assistant", sf::Style::Close);
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        cerr << "Failed to load font" << endl;
        return -1;
    }

    sf::Text instructions("Press 'R' to rank cars and display top 20.\nPress 'S' to search cars based on criteria.\nPress 'Esc' to exit.", font, 20);
    instructions.setFillColor(sf::Color::White);
    instructions.setPosition(10, 10);

    sf::Text rankingText("Top 20 Cars (Ranked):\n\n... (Car details here)\n\n"
                         "Press 'B' to go back to the main menu.\n"
                         "Press 'Esc' to exit.", font, 24);
    rankingText.setFillColor(sf::Color::White);
    rankingText.setPosition(10, 10);

    sf::Text searchText("Search Mode Activated.\n\n... (Search criteria input here)\n\n"
                        "Press 'B' to go back to the main menu.\n"
                        "Press 'Esc' to exit.", font, 24);
    searchText.setFillColor(sf::Color::White);
    searchText.setPosition(10, 10);

    bool displayTop20 = false;
    bool searchMode = false;
    vector<Car> searchResults;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    displayTop20 = true;
                    searchMode = false;
                }
                if (event.key.code == sf::Keyboard::S) {
                    displayTop20 = false;
                    searchMode = true;
                }
            }
        }

        window.clear(sf::Color::Blue);

        if (displayTop20) {
            rankCars(cars, {
                    {"mpg", 1.0},
                    {"highwayMPG", 0.8},
                    {"year", 0.5},
                    {"horsePower", 0.7},
                    {"torque", 0.6}
            });

            sf::Text carText("", font, 18);
            carText.setFillColor(sf::Color::White);
            float yPosition = 50.0f;
            float maxWidth = window.getSize().x - 20;


            for (size_t i = 0; i < 20 && i < cars.size(); ++i) {
                string carDetails = "ID: " + cars[i].ID + ", Make: " + cars[i].make + ", Model Year: " + cars[i].modelYear +
                        ", MPG: " + to_string(cars[i].mpg) + ", Highway MPG: " + to_string(cars[i].highwayMPG) +
                        ", Year: " + to_string(cars[i].year) + ", HorsePower: " + to_string(cars[i].horsePower) +
                        ", Torque: " + to_string(cars[i].torque);
                //carText.setPosition(10, yPosition);
                //window.draw(carText);
                //yPosition += 20;
                vector<string> wrappedText = wrapText(carDetails, font, 18, maxWidth);
                for (const auto &line: wrappedText) {
                    carText.setString(line);
                    carText.setPosition(10, yPosition);
                    window.draw(carText);
                    yPosition += 30;
                }
                yPosition += 10;

            }
        } else if (searchMode) {

            sf::Text searchText("Search Mode Activated. Implement search criteria input here.", font, 20);
            searchText.setFillColor(sf::Color::White);
            searchText.setPosition(10, 50);
            window.draw(searchText);
        } else {
            window.draw(instructions);
        }

        window.display();
    }

    return 0;
}*/
