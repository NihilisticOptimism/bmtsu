#include <iostream>
#include <string>
#include <iomanip>
using std::cout;
using std::string;
using std::endl;

class BankAccount {
protected:
    string acc_n;
    string owner;
    double balance;
public:
    BankAccount(string num, string name, double initial = 0) 
        : acc_n(num), owner(name), balance(initial) {}
    
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposit: " << amount << endl;
        }
    }
    
    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdraw: " << amount << endl;
            return true;
        }
        cout << "Not enough money (poor)" << endl;
        return false;
    }
    
    void display() const {
        cout << "Account: " << acc_n << ", Owner: " << owner 
             << ", Balance: " << balance << endl;
    }
    
    double getBalance() const { return balance; }
};

class SavingsAccount : public BankAccount {
private:
    double int_r;
public:
    SavingsAccount(string num, string name, double rate, double initial = 0) 
        : BankAccount(num, name, initial), int_r(rate) {}
    
    void addInterest() {
        double interest = balance * int_r;
        balance += interest;
        cout << "Interest deposited: " << interest << endl;
    }
    
    void display() const {
        BankAccount::display();
        cout << "Interest rate: " << int_r * 100 << "%" << endl;
    }
};

int main() {
    BankAccount account1("1", "Mister Watson", 10000);
    cout << "=== Bank account ===" << endl;
    account1.display();
    account1.deposit(5000);
    account1.display();
    account1.withdraw(15000);
    account1.display();
    cout << "\n";
    
    SavingsAccount account2("2", "Sherlock Holmes", 0.05, 20000);
    cout << "=== Savings account ===" << endl;
    account2.display();
    account2.addInterest();
    account2.deposit(15000);
    account2.display();
    account2.addInterest();
    account2.display();
    return 0;
}