#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>

#define MAX 100

using namespace std;

struct City {
    int index;
    string name;
};

vector<City> cities;
int roads[MAX][MAX] = {0};
double budgets[MAX][MAX] = {0.0};

int findCityIndexByName(string name) {
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    for (int i = 0; i < cities.size(); i++) {
        string cityName = cities[i].name;
        transform(cityName.begin(), cityName.end(), cityName.begin(), ::tolower);
        if (cityName == name) return i;
    }
    return -1;
}

void addCities() {
    int count;
    cout << "Enter the number of cities to add: ";
    cin >> count;
    cin.ignore();
    for (int i = 0; i < count; i++) {
        City c;
        c.index = cities.size() + 1;
        cout << "Enter name for city " << c.index << ": ";
        getline(cin, c.name);

        if (findCityIndexByName(c.name) == -1) {
            cities.push_back(c);
        } else {
            cout << "City already exists. Skipping.\n";
        }
    }
}

void addRoad() {
    string c1, c2;
    cin.ignore();
    cout << "Enter the name of the first city: "; getline(cin, c1);
    cout << "Enter the name of the second city: "; getline(cin, c2);
    int i = findCityIndexByName(c1);
    int j = findCityIndexByName(c2);
    if (i != -1 && j != -1 && i != j) {
        roads[i][j] = roads[j][i] = 1;
        cout << "Road added between " << cities[i].name << " and " << cities[j].name << ".\n";
    } else {
        cout << "Invalid cities entered.\n";
    }
}

void deleteRoad() {
    string c1, c2;
    cin.ignore();
    cout << "Enter the name of the first city: "; getline(cin, c1);
    cout << "Enter the name of the second city: "; getline(cin, c2);
    int i = findCityIndexByName(c1);
    int j = findCityIndexByName(c2);
    if (i != -1 && j != -1 && roads[i][j] == 1) {
        roads[i][j] = roads[j][i] = 0;
        budgets[i][j] = budgets[j][i] = 0.0;
        cout << "Road deleted between " << cities[i].name << " and " << cities[j].name << ".\n";
    } else {
        cout << "Road not found between the cities.\n";
    }
}

void addBudget() {
    string c1, c2;
    double budget;
    cin.ignore();
    cout << "Enter the name of the first city: "; getline(cin, c1);
    cout << "Enter the name of the second city: "; getline(cin, c2);
    int i = findCityIndexByName(c1);
    int j = findCityIndexByName(c2);
    if (i != -1 && j != -1 && roads[i][j] == 1) {
        cout << "Enter the budget for the road: ";
        cin >> budget;
        budgets[i][j] = budgets[j][i] = budget;
        cout << "Budget added for the road between " << cities[i].name << " and " << cities[j].name << ".\n";
    } else {
        cout << "Road not found between the cities.\n";
    }
}

void editCity() {
    int index;
    string newName;
    cout << "Enter the index of the city to edit: ";
    cin >> index;
    cin.ignore();
    if (index >= 1 && index <= cities.size()) {
        cout << "Enter new name for city: ";
        getline(cin, newName);
        cities[index - 1].name = newName;
        cout << "City updated successfully.\n";
    } else {
        cout << "Invalid city index.\n";
    }
}

void searchCity() {
    int index;
    cout << "Enter city index to search: ";
    cin >> index;
    if (index >= 1 && index <= cities.size()) {
        cout << index << ": " << cities[index - 1].name << "\n";
    } else {
        cout << "City not found.\n";
    }
}

void displayCities() {
    cout << "Cities:\n";
    for (auto c : cities) {
        cout << c.index << ": " << c.name << "\n";
    }
}

void displayRoads() {
    displayCities();
    cout << "Roads Adjacency Matrix:\n";
    for (int i = 0; i < cities.size(); i++) {
        for (int j = 0; j < cities.size(); j++) {
            cout << roads[i][j] << " ";
        }
        cout << endl;
    }
}

void displayAll() {
    displayRoads();
    cout << "\nBudgets Adjacency Matrix:\n";
    for (int i = 0; i < cities.size(); i++) {
        for (int j = 0; j < cities.size(); j++) {
            cout << fixed << setprecision(2) << budgets[i][j] << "\t";
        }
        cout << endl;
    }
}

void saveToFile() {
    ofstream cityFile("cities.txt");
    cityFile << "Index\tCity_Name\n";
    for (auto c : cities) {
        cityFile << c.index << "\t" << c.name << "\n";
    }
    cityFile.close();

    ofstream roadFile("roads.txt");
    roadFile << "#\tRoad\tBudget\n";
    int count = 1;
    for (int i = 0; i < cities.size(); i++) {
        for (int j = i + 1; j < cities.size(); j++) {
            if (roads[i][j]) {
                roadFile << count++ << ".\t" << cities[i].name << "-" << cities[j].name << "\t" << budgets[i][j] << "\n";
            }
        }
    }
    roadFile.close();
}

void loadFromFile() {
    ifstream cityFile("cities.txt");
    if (cityFile.is_open()) {
        string header;
        getline(cityFile, header);
        City c;
        while (cityFile >> c.index) {
            cityFile.ignore();
            getline(cityFile, c.name);
            cities.push_back(c);
        }
        cityFile.close();
    }

    ifstream roadFile("roads.txt");
    if (roadFile.is_open()) {
        string header;
        getline(roadFile, header);
        string line;
        while (getline(roadFile, line)) {
            size_t dash = line.find('-');
            size_t tab = line.find('\t', dash);
            if (dash != string::npos && tab != string::npos) {
                string name1 = line.substr(line.find('\t') + 1, dash - line.find('\t') - 1);
                string name2 = line.substr(dash + 1, tab - dash - 1);
                double budget = stod(line.substr(tab + 1));

                int i = findCityIndexByName(name1);
                int j = findCityIndexByName(name2);
                if (i != -1 && j != -1) {
                    roads[i][j] = roads[j][i] = 1;
                    budgets[i][j] = budgets[j][i] = budget;
                }
            }
        }
        roadFile.close();
    }
}

int main() {
    loadFromFile();

    int choice;
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add new city(ies)\n";
        cout << "2. Add roads between cities\n";
        cout << "3. Add the budget for roads\n";
        cout << "4. Edit city\n";
        cout << "5. Search for a city using its index\n";
        cout << "6. Display cities\n";
        cout << "7. Display roads\n";
        cout << "8. Display recorded data on console\n";
        cout << "9. Delete a road\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addCities(); break;
            case 2: addRoad(); break;
            case 3: addBudget(); break;
            case 4: editCity(); break;
            case 5: searchCity(); break;
            case 6: displayCities(); break;
            case 7: displayRoads(); break;
            case 8: displayAll(); break;
            case 9: deleteRoad(); break;
            case 10: saveToFile(); return 0;
            default: cout << "Invalid choice.\n"; break;
        }
    }

    return 0;
}//
// Created by Admin on 23/05/2025.
//
