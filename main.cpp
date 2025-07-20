/*Made by Tuna AYDIN on 19/07/2025*/

#include <iostream>
#include <fstream>
using namespace std;
// Declare name of the file as a global variable to make things easier
string textFile  = "database.txt";

// Declare a class named user to hold neccessary functions and data
class user
{
private:
    string name;
    string surname;
    long long int money;    
public:
    // declare id and password as public so other functions can access them
    int id;
    string password;
    // Functions:
    //Puts data given from a function into the object
    void initialize(int _id, string _name, string _surname, string _password, long long int _money);
    // INcreases the money according to given amount
    void deposit();
    // Decreases the money according to given amount
    void withdraw();
    // Transfers money between accounts
    void transfer();
    // Shows current balance
    void check();
    
    // Puts data to an file
    friend ostream& operator <<(ostream& out , const user& p)
    {
        out << p.id << " " << p.name << " " << p.surname<< " " << p.password << " " << p.money << endl;
        return out;
    }
    
    // Takes data from file
    friend istream& operator>>(istream& in, user& p)
    {
        in >> p.id >> p.name >> p.surname >> p.password >> p.money;
        return in;
    }
    
    
};

void user::initialize(int _id, string _name, string _surname, string _password, long long int _money)
{
    id = _id;
    name = _name;
    surname = _surname;
    password = _password;
    money = _money;
}

void user::check()
{
    cout << "\nYour current balance is: " << money << "₺" << endl;
}

void user::deposit()
{
    long long int add_money;
    cout << "\nEnter the amount you want to deposit: ";
    cin >> add_money;
    
    if(add_money <= 0)
    {
        cout << "Amount should be positive" << endl;
        return;
    }
    
    money += add_money;
    
    // Read existing data from file
    vector<user> allData;
    ifstream inFile(textFile);
    user temp;
    // Increment through data in file
    while (inFile >> temp)
    {
        // Find id that matches with users id in file
        if (id == temp.id)
        {
            temp.money = money;  // Update money for the current user
        }
        allData.push_back(temp);
    }
    inFile.close();
    
    // Write updated data to the file
    ofstream outFile(textFile, ios::trunc);  // Open file in truncation mode to overwrite it
    for (const auto& user : allData)
    {
        outFile << user;
    }
    outFile.close();
    
    cout << "Deposit successful. Your new balance is: " << money << endl;
}

void user::withdraw()
{
    long long int drawn;
    // Prevents user from inputing negative amount or money bigger than what they have
    do
    {
        cout << "\nEnter the amount you want to withdraw: ";
        cin >> drawn;
        if (drawn <= 0 || drawn > money)
        {
            cout << "Invalid amount. Please try again." << endl;
        }
    } while (drawn <= 0 || drawn > money);
    
    money -= drawn;
    
    // Read and update data
    vector<user> allData;
    ifstream inFile(textFile);
    user temp;
    while (inFile >> temp)
    {
        if (id == temp.id)
        {
            temp.money = money;  // Update money for the current user
        }
        allData.push_back(temp);
    }
    inFile.close();
    
    // Write updated data to the file
    ofstream outFile(textFile, ios::trunc);  // Open file in truncation mode to overwrite it
    for (const auto& user : allData)
    {
        outFile << user;
    }
    outFile.close();
    
    cout << "Withdrawal successful. Your new balance is: " << money << endl;
}

void user::transfer()
{
    // For the id of the account we are looking for
    int tr_id;
    long long int money_sent;
    
    cout << "\nPlease enter the ID of the person you want to transfer money to: ";
    cin >> tr_id;
    
    do
    {
        cout << "Please enter the amount you want to send (can't be larger than your balance or negative): ";
        cin >> money_sent;
        if (money_sent <= 0 || money_sent > money)
        {
            cout << "Invalid amount. Please try again." << endl;
        }
    } while (money_sent <= 0 || money_sent > money);
    
    // Read and update data
    vector<user> allData;
    ifstream inFile(textFile);
    user temp;
    bool transferSuccess = false;
    
    while (inFile >> temp)
    {
        if (temp.id == tr_id)
        {
            temp.money += money_sent;  // Add money to the recipient's account
            transferSuccess = true;
        }
        if (temp.id == id)
        {
            temp.money -= money_sent;  // Subtract money from the sender's account
        }
        allData.push_back(temp);
    }
    inFile.close();
    
    if (transferSuccess)
    {
        // Write updated data to the file
        ofstream outFile(textFile, ios::trunc);
        for (const auto& user : allData)
        {
            outFile << user;
        }
        outFile.close();
        
        cout << "Transfer successful. Your new balance is: " << money - money_sent << endl;
    }
    else
    {
        cout << "Transfer failed. User with ID " << tr_id << " not found" << endl;
    }
}


void menu(user a);
void register_();
void login();

// Main function that serves as a login page
int main()
{
    int cho;
    cout << "Welcome to the bank\nPress 1 to register\n2 to login\n3 to exit: ";
    do
    {
        cin >> cho;
        getchar();
        if(cho != 1 && cho != 2 && cho != 3)
            cout << "Wrong input Try again: ";
    } while (cho != 1 && cho != 2 && cho != 3);
    
    switch(cho)
    {
        case 1: register_();
            cout << "\nRegisterition is complate. Now please log in to your account" << endl;
            login();
            break;
        case 2: login();
            break;
        case 3: exit(1);
    }
}
// to register an account into a file
void register_()
{
    // Declere a registiration user to work with
    user reg;
    // credidentals
    string name, pass, pass1, surname;
    int id;
    long long int initial;
    
    cout << "\nPlease Enter your name: ";
    getline(cin, name);
    cout << "Please enter your surname: ";
    getline(cin, surname);
    
    do
    {
        cout << "Please enter your password: ";
        cin >> pass;
        cout << "Please enter your password again: ";
        cin >> pass1;
        if (pass != pass1)
            cout << "Passwords don't match. Try again." << endl;
    } while (pass != pass1);
    // To make unique IDs for every account
    // Find the last ID and increment it
    ifstream inFile(textFile);
    id = 1000000;  // Default ID
    
    if (inFile)
    {
        user temp;
        while (inFile >> temp)
        {
            id = temp.id + 1;  // Increment based on the last id found
        }
    }
    inFile.close();
    
    cout << "Your ID is " << id << endl;
    cout << "Please enter your initial deposit: ";
    cin >> initial;
    getchar();
    // Sends inputted data to the class
    reg.initialize(id, name, surname, pass, initial);
    
    ofstream outFile(textFile, ios::app);  // Open file in append mode
    outFile << reg;
    outFile.close();
}

// Log in function of the program
void login()
{
    // Declere a login user to send it into rest of the program
    user log;
    int id;
    string pass;
    bool flag = false;
    
    cout << "\nPlease enter your id(7 digits): ";
    cin >> id;
    getchar();
    
    ifstream inFile(textFile);
    if(!inFile)
    {
        cout << "Database is empty. Please register an account first to continue."<< endl;
        exit(3);
    }
    
    while(inFile >> log)
    {
        if(id == log.id)
        {
            do
            {
                cout << "Please enter your password: ";
                cin >> pass;
                if(pass != log.password)
                    cout << "Wrong password. Try again" << endl;
                else
                {
                    flag = true;
                }
            } while (flag == false);
        }
    }
    if(id != log.id)
    {
        cout << "This ID does not exists. Exiting program" << endl;
        exit(9);
    }
    inFile.close();
    // first search for id in file then check password then copy it to a user object called log
    // Then send the log into the menu
    if(flag == true)
    {
        menu(log);
    }
    else
    {
        cout << "Empty file exiting program." << endl;
        exit(6);
    }
    
}

// Main menu of the accounts transactions
void menu(user a)
{
    int cho;
    cout << "\nPlease select your process:\n1 for Withdrawing money\n2 for depositing money\n3 for transferring money between accounts\n4 to check balance\n5 to exit system\nPlease pick your choice: ";
    do
    {
        cin >> cho;
        getchar();
        if(cho < 1 || cho > 5)
            cout << "Invalid input. Try again: ";
    }while(cho < 1 || cho > 5);
    
    switch(cho)
    {
        case 1: a.withdraw();
            menu(a);
            break;
        case 2: a.deposit();
            menu(a);
            break;
        case 3: a.transfer();
            menu(a);
            break;
        case 4: a.check();
            menu(a);
            break;
        case 5: exit(0);
            break;
    }
}
