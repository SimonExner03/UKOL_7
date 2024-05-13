#include <iostream>
#include <string>

//------------------CLIENT------------------
class Client {
private:
    static int objectsCount;
    int code;
    std::string name;
public:
    static int GetObjectsCount();

    Client(int c, std::string n);
    ~Client();

    int GetCode();
    std::string GetName();
};

int Client::objectsCount = 0;

Client::Client(int c, std::string n) {
    this->code = c;
    this->name = n;
    Client::objectsCount += 1;
}
Client::~Client() {
    Client::objectsCount -= 1;
}
int Client::GetCode() {
    return this->code;
}
std::string Client::GetName() {
    return this->name;
}
int Client::GetObjectsCount() {
    return Client::objectsCount;
}

//------------------ACCOUNT--------------

class Account {
private:
    static int defaultinterestRate;
    static int objectsCount;
    int number;

protected:
    double balance = 0;
    double interestRate = 0;
    Client* owner;

public:
    static int GetObjectsCount();
    Account(int n, Client* c);
    Account(int n, Client* c, double ir);
    virtual ~Account();

    int GetNumber();
    double GetBalance();
    double GetInterestRate();
    Client* GetOwner();

    virtual bool CanWithdraw(double a);
    virtual void AccountPrint();
};

int Account::objectsCount = 0;
int Account::defaultinterestRate = 0.05;

Account::Account(int n, Client* c) {
    this->number = n;
    this->owner = c;
    Account::objectsCount += 1;
}
Account::Account(int n, Client* c, double ir) {
    this->number = n;
    this->owner = c;
    this->interestRate = ir;
    Account::objectsCount += 1;
}

Account::~Account() {
    Account::objectsCount -= 1;
}

int Account::GetNumber() {
    return this->number;
}
double Account::GetBalance() {
    return this->balance;
}
double Account::GetInterestRate() {
    return this->interestRate;
}
Client* Account::GetOwner() {
    return this->owner;
}

bool Account::CanWithdraw(double a) {
    return (this->balance >= a);
}

void Account::AccountPrint() {
    std::cout << "Account number: " << this->number << ", Balance: " << this->balance << std::endl;
}

//------------------Credit Account--------------------

class CreditAccount : public Account {
private:
    double credit;

public:
    CreditAccount(int n, Client* o, double c);
    CreditAccount(int n, Client* o, double ir, double c);

    bool CanWithdraw(double a) override;
    void AccountPrint() override;
};

CreditAccount::CreditAccount(int n, Client* o, double c) : Account(n, o) {
    this->credit = c > 0 ? c : 0;
}

CreditAccount::CreditAccount(int n, Client* o, double ir, double c) : Account(n, o, ir) {
    this->credit = c > 0 ? c : 0;
}

bool CreditAccount::CanWithdraw(double a) {
    return ((this->GetBalance() + this->credit) >= a);
}

void CreditAccount::AccountPrint() {
    std::cout << "Credit Account number: " << this->GetNumber() << ", Balance: " << this->GetBalance() << ", Credit: " << this->credit << std::endl;
}

//------------------Bank---------------------
class Bank {
private:
    Client** clients;
    int clientsCount;
    int actualClients = 0;
    Account** accounts;
    int accountsCount;
    int actualAccounts = 0;
public:
    Bank(int c, int a);
    ~Bank();

    Client* GetClient(int c);
    Account* GetAccount(int n);

    Client* CreateClient(int c, std::string n);
    Account* CreateAccount(int n, Client* c);
    Account* CreateAccount(int n, Client* c, double ir);

    void AddInterest();
};

Bank::Bank(int c, int a) {
    this->clientsCount = c;
    this->accountsCount = a;
    this->accounts = new Account * [a];
    this->clients = new Client * [c];
}
Bank::~Bank() {
    for (int i = 0; i < this->accountsCount; i++) {
        delete this->accounts[i];
    }
    delete[] this->accounts;

    for (int i = 0; i < this->clientsCount; i++) {
        delete this->clients[i];
    }
    delete[] this->clients;
}

Client* Bank::GetClient(int c) {
    for (int i = 0; i < this->clientsCount; i++) {
        if (this->clients[i]->GetCode() == c) {
            return this->clients[i];
        }
    }
    return nullptr;
}
Account* Bank::GetAccount(int n) {
    for (int i = 0; i < this->accountsCount; i++) {
        if (this->accounts[i]->GetNumber() == n) {
            return this->accounts[i];
        }
    }
    return nullptr;
}

Client* Bank::CreateClient(int c, std::string n) {
    this->clients[this->actualClients] = new Client(c, n);
    this->actualClients++;
    return this->clients[this->actualClients - 1];
}
Account* Bank::CreateAccount(int n, Client* c) {
    this->accounts[this->actualAccounts] = new Account(n, c);
    this->actualAccounts++;
    return this->accounts[this->actualAccounts - 1];
}
Account* Bank::CreateAccount(int n, Client* c, double ir) {
    this->accounts[this->actualAccounts] = new Account(n, c, ir);
    this->actualAccounts++;
    return this->accounts[this->actualAccounts - 1];
}


int main() {

    Client* o = new Client(0, "Smith");

    CreditAccount* ca = new CreditAccount(1, o, 1000);
    std::cout << ca->CanWithdraw(1000) << std::endl;
    Account* a = ca;
    std::cout << a->CanWithdraw(1000) << std::endl;
    std::cout << ca->CanWithdraw(1000) << std::endl;


    a->AccountPrint();

    a = nullptr;
    delete ca;

    return 0;
}