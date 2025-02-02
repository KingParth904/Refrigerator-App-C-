#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <algorithm>

using namespace std;

struct Item {
    string name;
    double quantity;
    string unit;
    time_t expiration_date; 
};

class Refrigerator {
private:
    map<string, Item> inventory;
    vector<string> history;
    vector<string> shopping_list;

    bool isExpired(const Item &item) {
        time_t now = time(0);
        return difftime(item.expiration_date, now) < 0;
    }

public:
    void insertItem(const string &name, double quantity, const string &unit, time_t exp_date) {
        if (inventory.find(name) != inventory.end()) {
            inventory[name].quantity += quantity;
        } else {
            inventory[name] = {name, quantity, unit, exp_date};
        }
        history.push_back("Added " + to_string(quantity) + " " + unit + " of " + name);
    }

    void consumeItem(const string &name, double quantity) {
        if (inventory.find(name) != inventory.end()) {
            if (inventory[name].quantity >= quantity) {
                inventory[name].quantity -= quantity;
                history.push_back("Consumed " + to_string(quantity) + " of " + name);
            } else {
                cout << "Not enough " << name << " available!" << endl;
            }
        } else {
            cout << "Item not found in refrigerator!" << endl;
        }
    }

    void checkStatus() {
        cout << "Current Inventory:\n";
        for (const auto &pair : inventory) {
            cout << pair.second.name << ": " << pair.second.quantity << " " << pair.second.unit;
            if (isExpired(pair.second)) {
                cout << " (Expired)";
            }
            cout << endl;
        }
    }

    void removeExpiredItems() {
        for (auto it = inventory.begin(); it != inventory.end();) {
            if (isExpired(it->second)) {
                cout << "Removing expired item: " << it->second.name << endl;
                it = inventory.erase(it);
            } else {
                ++it;
            }
        }
    }

    void generateShoppingList() {
        shopping_list.clear();
        for (const auto &pair : inventory) {
            if (pair.second.quantity < 1.0) {
                shopping_list.push_back(pair.second.name);
            }
        }
        cout << "Suggested Shopping List:\n";
        for (const string &item : shopping_list) {
            cout << "- " << item << endl;
        }
    }
};

int main() {
    Refrigerator fridge;

    time_t exp1 = time(0) + 86400; // 1 day from now
    time_t exp2 = time(0) - 3600;  // 1 hour ago (expired)

    fridge.insertItem("Milk", 1.5, "Liters", exp1);
    fridge.insertItem("Vegetables", 500, "grams", exp2);
    fridge.insertItem("Eggs", 12, "pieces", exp1);

    fridge.checkStatus();
    fridge.consumeItem("Milk", 0.5);
    fridge.removeExpiredItems();
    fridge.generateShoppingList();

    return 0;
}
