#include <iostream>
#include <vector>
#include <map>
#include <ctime>

using namespace std;

struct Item {
    string name;
    double quantity;
    string unit;
    time_t expirationDate;
};

class Refrigerator {
private:
    map<string, Item> inventory;  // Stores items by name
    vector<string> history;       // Keeps track of actions
    vector<string> shoppingList;  // List of items to buy

    // Checks if an item is expired
    bool isExpired(const Item &item) {
        return difftime(item.expirationDate, time(0)) < 0;
    }

public:
    // Add an item to the refrigerator
    void addItem(string name, double quantity, string unit, time_t expDate) {
        if (inventory.count(name)) {
            inventory[name].quantity += quantity;
        } else {
            inventory[name] = {name, quantity, unit, expDate};
        }
        history.push_back("Added " + to_string(quantity) + " " + unit + " of " + name);
    }

    // Consume a certain quantity of an item
    void useItem(string name, double quantity) {
        if (inventory.count(name)) {
            if (inventory[name].quantity >= quantity) {
                inventory[name].quantity -= quantity;
                history.push_back("Used " + to_string(quantity) + " " + inventory[name].unit + " of " + name);
            } else {
                cout << "Not enough " << name << " available!\n";
            }
        } else {
            cout << name << " is not in the refrigerator!\n";
        }
    }

    // Display the current inventory
    void showInventory() {
        cout << "\nCurrent Inventory:\n";
        for (const auto &item : inventory) {
            cout << "- " << item.second.name << ": " << item.second.quantity << " " << item.second.unit;
            if (isExpired(item.second)) {
                cout << " (Expired)";
            }
            cout << endl;
        }
    }

    void discardExpiredItems() {
        for (auto it = inventory.begin(); it != inventory.end();) {
            if (isExpired(it->second)) {
                cout << "Discarding expired item: " << it->second.name << endl;
                it = inventory.erase(it);
            } else {
                ++it;
            }
        }
    }

    void createShoppingList() {
        shoppingList.clear();
        for (const auto &item : inventory) {
            if (item.second.quantity < 1.0) {
                shoppingList.push_back(item.second.name);
            }
        }

        cout << "\nSuggested Shopping List:\n";
        for (const string &item : shoppingList) {
            cout << "- " << item << endl;
        }
    }
};

int main() {
    Refrigerator fridge;

    time_t futureDate = time(0) + 86400;  // 1 day from now
    time_t pastDate = time(0) - 3600;     // 1 hour ago (expired)

    // Add items to the fridge
    fridge.addItem("Milk", 1.5, "Liters", futureDate);
    fridge.addItem("Vegetables", 500, "grams", pastDate);
    fridge.addItem("Eggs", 12, "pieces", futureDate);

    // Perform actions
    fridge.showInventory();
    fridge.useItem("Milk", 0.5);
    fridge.discardExpiredItems();
    fridge.createShoppingList();

    return 0;
}
