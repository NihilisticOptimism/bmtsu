#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <memory>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iomanip>
#include <iterator>

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::getline;
using std::ofstream;
using std::ifstream;
using std::endl;
using std::stringstream;
using std::list;
using std::shared_ptr;
using std::make_shared;
using std::ostream_iterator;
using std::dynamic_pointer_cast;

// Базовый класс ElectronicDevice
class ElectronicDevice {
protected:
    string brand;
    string mod;
    double price;
    vector<string> installedApps;

public:
    ElectronicDevice(const string& b, const string& m, double p, const vector<string>& apps)
        : brand(b), mod(m), price(p), installedApps(apps) {}

    string getBrand() const { return brand; }
    string getModel() const { return mod; }
    double getPrice() const { return price; }
    vector<string> getInstalledApps() const { return installedApps; }

    void setPrice(double p) { price = p; }
    void setModel(const string& m) { mod = m; }
    void addApp(const string& app) { installedApps.push_back(app); }

    virtual void display() const {
        cout << "Brand: " << brand << ", model: " << mod
             << ", Price: " << price
             << ", Apps: ";
        for (const auto& app : installedApps) cout << app << " ";
        cout << endl;
    }

    virtual void saveToFile(ofstream& out) const {
        out << brand << ";" << mod << ";" << price << ";";
        for (int i = 0; i < installedApps.size(); ++i) {
            out << installedApps[i];
            if (i != installedApps.size() - 1) out << "|";
        }
    }

    virtual void loadFromFile(ifstream& in) {
        string appsStr;
        getline(in, brand, ';');
        getline(in, mod, ';');
        in >> price;
        in.ignore();
        getline(in, appsStr);

        installedApps.clear();
        stringstream str_app(appsStr);
        string app;
        while (getline(str_app, app, '|')) {
            installedApps.push_back(app);
        }
    }
};


class Smartphone : public ElectronicDevice {
private:
    string os;
    int memory; 

public:
    Smartphone(const string& b, const string& m, double p, const vector<string>& apps,
               const string& o, int mem)
        : ElectronicDevice(b, m, p, apps), os(o), memory(mem) {}
    
    string getOS() const { return os; }
    int getMemory() const { return memory; }
    
    void display() const override {
        cout << "[Smartphone] ";
        ElectronicDevice::display();
        cout << "  OS: " << os << ", Memory: " << memory << "GB" << endl;
    }

    void saveToFile(ofstream& out) const override {
        out << "Smartphone;";
        ElectronicDevice::saveToFile(out);
        out << ";" << os << "-" << memory << endl;
    }

    
};


class Laptop : public ElectronicDevice {
private:
    double screenSize; 
    int batteryCapacity;

public:
    Laptop(const string& b, const string& m, double p, const vector<string>& apps,
           double screen, int battery)
        : ElectronicDevice(b, m, p, apps), screenSize(screen), batteryCapacity(battery) {}

    double getScreenSize() const { return screenSize; }
    int getBatteryCapacity() const { return batteryCapacity; }

    void display() const override {
        cout << "[Laptop] ";
        ElectronicDevice::display();
        cout << "  Screen: " << screenSize << "\", Battery: " << batteryCapacity << "Wh" << endl;
    }

    void saveToFile(ofstream& out) const override {
        out << "Laptop;";
        ElectronicDevice::saveToFile(out);
        out << ";" << screenSize << "-" << batteryCapacity << endl;
    }
};


void loadFromFile(const string& filename, list<shared_ptr<ElectronicDevice>>& devices) {
    ifstream file(filename);
    if (!file) {
        cout << "Cannot open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream str_line(line);
        string type, brand, mod, param, appsStr;
        double price;
        getline(str_line, type, ';');
        getline(str_line, brand, ';');
        getline(str_line, mod, ';');
        str_line >> price;
        str_line.ignore();
        getline(str_line, param, ';');
        getline(str_line, appsStr);

        vector<string> apps;
        stringstream str_app(appsStr);
        string app;
        while (getline(str_app, app, '|')) {
            apps.push_back(app);
        }

        if (type == "Smartphone") {
            string os;
            int memory;
            stringstream paramStream(param);
            getline(paramStream, os, '-');
            paramStream >> memory;
            devices.push_back(make_shared<Smartphone>(brand, mod, price, apps, os, memory));
        } else if (type == "Laptop") {
            double screen;
            int battery;
            stringstream paramStream(param);
            paramStream >> screen;
            paramStream.ignore();
            paramStream >> battery;
            devices.push_back(make_shared<Laptop>(brand, mod, price, apps, screen, battery));
        }
    }
    cout << "Data loaded from " << filename << endl;
}

void saveToFile(const string& filename, const list<shared_ptr<ElectronicDevice>>& devices) {
    ofstream file(filename);
    if (!file) {
        cout << "Cannot open file for writing: " << filename << endl;
        return;
    }

    for (const auto& device : devices) {
        device->saveToFile(file); // ig this is similar to python's .
    }
    cout << "Data saved to " << filename << endl;
}

void editDevice(ElectronicDevice* device) {
    if (!device) return;

    cout << "Editing device: ";
    device->display();

    double newPrice;
    string newModel, newApp;

    cout << "Enter new price: ";
    cin >> newPrice;
    device->setPrice(newPrice);
    cin.ignore();

    cout << "Enter new model: ";
    getline(cin, newModel);
    device->setModel(newModel);

    cout << "Enter new app to add: ";
    getline(cin, newApp);
    device->addApp(newApp);

    cout << "Device updated.\n";
}

void menu(list<shared_ptr<ElectronicDevice>>& devices) {
    int choice;
    do {
        cout << "\n========== Electronic Device Manager ==========\n";
        cout << "1. Load data from file\n";
        cout << "2. Display all devices\n";
        cout << "3. Search/Filter devices\n";
        cout << "4. Sort devices\n";
        cout << "5. Edit existing device\n";
        cout << "6. Delete device\n";
        cout << "7. Save data to file\n";
        cout << "8. Special functions (lambdas)\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string filename;
                cout << "Enter filename: ";
                cin >> filename;
                loadFromFile(filename, devices);
                break;
            }
            case 2: {
                cout << "\n--- All Devices ---\n";
                for (const auto& device : devices) {
                    device->display();
                }
                break;
            }
            case 3: {
                double minPrice;
                cout << "Enter minimum price: ";
                cin >> minPrice;
                auto it = copy_if(devices.begin(), devices.end(),
                                    ostream_iterator<shared_ptr<ElectronicDevice>>(cout, "\n"),
                                    [minPrice](const shared_ptr<ElectronicDevice>& d) {
                                        return d->getPrice() > minPrice;
                                    });
                break;
            }
            case 4: {
                cout << "Sort by:\n1. Price\n2. Brand then price\n";
                int sortChoice;
                cin >> sortChoice;
                if (sortChoice == 1) {
                    devices.sort([](const shared_ptr<ElectronicDevice>& a,
                                    const shared_ptr<ElectronicDevice>& b) {
                        return a->getPrice() < b->getPrice();
                    });
                } else if (sortChoice == 2) {
                    devices.sort([](const shared_ptr<ElectronicDevice>& a,
                                    const shared_ptr<ElectronicDevice>& b) {
                        if (a->getBrand() != b->getBrand())
                            return a->getBrand() < b->getBrand();
                        return a->getPrice() > b->getPrice();
                    });
                }
                cout << "Sorted.\n";
                break;
            }
            case 5: {
                int index = 0;
                cout << "Select device to edit (0-" << devices.size()-1 << "):\n";
                for (const auto& d : devices) {
                    cout << index++ << ": ";
                    d->display();
                }
                int idx;
                cin >> idx;
                if (idx >= 0 && idx < devices.size()) {
                    auto it = devices.begin();
                    advance(it, idx);
                    editDevice(it->get());
                }
                break;
            }
            case 6: {
                int index = 0;
                cout << "Select device to delete (0-" << devices.size()-1 << "):\n";
                for (const auto& d : devices) {
                    cout << index++ << ": ";
                    d->display();
                }
                int idx;
                cin >> idx;
                if (idx >= 0 && idx < devices.size()) {
                    auto it = devices.begin();
                    advance(it, idx);
                    devices.erase(it);
                    cout << "Device deleted.\n";
                }
                break;
            }
            case 7: {
                string filename;
                cout << "Enter filename to save: ";
                cin >> filename;
                saveToFile(filename, devices);
                break;
            }
            case 8: {
                cout << "\n--- Special Lambda Functions ---\n";

                auto maxMemPhone = max_element(devices.begin(), devices.end(),
                    [](const shared_ptr<ElectronicDevice>& a, const shared_ptr<ElectronicDevice>& b) {
                        auto* sa = dynamic_cast<Smartphone*>(a.get());
                        auto* sb = dynamic_cast<Smartphone*>(b.get());
                        if (!sa) return true;
                        if (!sb) return false;
                        return sa->getMemory() < sb->getMemory();
                    });
                if (maxMemPhone != devices.end()) {
                    cout << "Smartphone with max memory:\n";
                    (*maxMemPhone)->display();
                }

                int countLaptops = count_if(devices.begin(), devices.end(),
                    [](const shared_ptr<ElectronicDevice>& d) {
                        auto* laptop = dynamic_cast<Laptop*>(d.get());
                        return laptop && laptop->getScreenSize() > 15.0;
                    });
                cout << "Laptops with screen > 15\": " << countLaptops << endl;

                
                devices.sort([](const auto& a, const auto& b) {
                    return a->getPrice() < b->getPrice();
                });
                cout << "Devices sorted by price.\n";
                break;
            }
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

int main() {
    list<shared_ptr<ElectronicDevice>> devices;

    loadFromFile("devices.txt", devices);

    menu(devices);

    saveToFile("devices_saved.txt", devices);

    return 0;
}