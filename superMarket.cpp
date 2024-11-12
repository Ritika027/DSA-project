#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

class Bill {
private:
    string item;
    int rate, quantity;
public:
    Bill() : item(""), rate(0), quantity(0) {}

    void setItem(const string& item) { this->item = item; }
    void setRate(int rate) { this->rate = rate; }
    void setQuantity(int quantity) { this->quantity = quantity; }

    string getItem() const { return item; }
    int getRate() const { return rate; }
    int getQuantity() const { return quantity; }
};

void clearInventory() {
    ofstream out("Bill.txt", ios::trunc);
    if (out) {
        cout << "Inventory cleared successfully!\n";
    } else {
        cerr << "Error: Unable to clear inventory.\n";
    }
}

void addItem(Bill& b) {
    string item;
    int rate, quantity;

    cout << "Enter Item Name: ";
    cin >> item;
    cout << "Enter Rate of Item: ";
    cin >> rate;
    cout << "Enter Quantity of Item: ";
    cin >> quantity;

    b.setItem(item);
    b.setRate(rate);
    b.setQuantity(quantity);

    ofstream out("Bill.txt", ios::app);
    if (out) {
        out << b.getItem() << " : " << b.getRate() << " : " << b.getQuantity() << endl;
        cout << "Item added successfully!\n";
    } else {
        cerr << "Error: Unable to open file for writing.\n";
    }
}

void displayInventory() {
    ifstream in("Bill.txt");
    if (in) {
        cout << "\nCurrent Inventory:\n";
        cout << left << setw(15) << "Item" << setw(10) << "Rate" << setw(10) << "Quantity" << endl;
        cout << "--------------------------------------" << endl;
        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string itemName;
            int itemRate, itemQuantity;
            char delimiter;
            ss >> itemName >> delimiter >> itemRate >> delimiter >> itemQuantity;
            cout << left << setw(15) << itemName << setw(10) << itemRate << setw(10) << itemQuantity << endl;
        }
    } else {
        cerr << "Error: Unable to open file.\n";
    }
}

void printBill() {
    int totalBill = 0;
    string item;
    int quantity;

    cout << "\nStart Adding Items to the Bill (Type 'done' to finish):\n";
    while (true) {
        cout << "Enter Item Name: ";
        cin >> item;
        if (item == "done") break;

        cout << "Enter Quantity: ";
        cin >> quantity;

        ifstream in("Bill.txt");
        ofstream out("BillTemp.txt");
        bool itemFound = false;
        if (in && out) {
            string line;
            while (getline(in, line)) {
                stringstream ss(line);
                string itemName;
                int itemRate, itemQuantity;
                char delimiter;
                ss >> itemName >> delimiter >> itemRate >> delimiter >> itemQuantity;

                if (item == itemName) {
                    itemFound = true;
                    if (quantity <= itemQuantity) {
                        int amount = itemRate * quantity;
                        totalBill += amount;
                        cout << left << setw(15) << "Item" << setw(10) << "Rate" << setw(10) << "Quantity" << setw(10) << "Amount" << endl;
                        cout << left << setw(15) << itemName << setw(10) << itemRate << setw(10) << quantity << setw(10) << amount << endl;
                        itemQuantity -= quantity;
                    } else {
                        cout << "Insufficient quantity for " << item << ".\n";
                    }
                }
                out << itemName << " : " << itemRate << " : " << itemQuantity << endl;
            }
            if (!itemFound) cout << "Item not available!\n";
            in.close();
            out.close();
            remove("Bill.txt");
            rename("BillTemp.txt", "Bill.txt");
        } else {
            cerr << "Error: Unable to open file.\n";
        }
    }
    cout << "\nTotal Bill: $" << totalBill << endl;
    cout << "Thank you for shopping!\n";
}

void mainMenu() {
    Bill b;
    bool exit = false;

    while (!exit) {
        int choice;
        cout << "\n==============================\n";
        cout << "   Super Market Billing System\n";
        cout << "==============================\n";
        cout << "1. Add New Item to Inventory\n";
        cout << "2. View Inventory\n";
        cout << "3. Clear Inventory\n";
        cout << "4. Start New Bill\n";
        cout << "5. Exit\n";
        cout << "==============================\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addItem(b);
                break;
            case 2:
                displayInventory();
                break;
            case 3:
                clearInventory();
                break;
            case 4:
                printBill();
                break;
            case 5:
                exit = true;
                cout << "Exiting... Thank you for using the system!\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 5.\n";
                break;
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
