#include <iostream>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>

using namespace std;
using namespace oracle::occi;

// read database password from user input
// without showing the password on the screen
string readPassword() {
    struct termios settings;
    tcgetattr(STDIN_FILENO, &settings);
    settings.c_lflag = (settings.c_lflag & ~(ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);

    string password = "";
    getline(cin, password);

    settings.c_lflag = (settings.c_lflag | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);
    return password;
}

void displayClients(Connection *);

void insertClients(Connection *);

void removeClient(Connection *);

void updateClient(Connection *);

void displayProject(Connection *);

void insertProject(Connection *);

int main() {
    string userName;
    string password;
    // address of the Oracle server
    const string connectString = "sunfire.csci.viu.ca";
    int answer;
    cout << "Your user name: ";
    getline(cin, userName);
    cout << "Your password: ";
    password = readPassword();
    cout << endl;
    try {
        // establish database connection
        Environment *env = Environment::createEnvironment();
        Connection *conn = env->createConnection(userName, password, connectString);
        //code goes here

        cout << " Welcome to the client manager application. Please choose the options from given list" << endl;
        cout << "1. Display the client list" << endl;
        cout << "2. Enter a new client into the system" << endl;
        cout << "3. Remove a client from the system" << endl;
        cout << "4. Update client information(address and city)" << endl;
        cout << "5. Display the projects the client is working on" << endl;
        cout << "6. Enter a new project into the system and assign client to them" << endl;
        cout << "7. Please enter 7 to quit the program" << endl;
        cin >> answer;

        while (answer != 7) {
            if (answer == 1) {
                displayClients(conn);
            } else if (answer == 2) {
                insertClients(conn);
            } else if (answer == 3) {
                removeClient(conn);
            } else if (answer == 4) {
                updateClient(conn);
            } else if (answer == 5) {
                displayProject(conn);
            } else if (answer == 6) {
                insertProject(conn);
            }
            else {
                cout << "You entered a wrong option. Please enter a right option again" << endl;
                cin >> answer;
            }
            cout << "What's the next thing you would like to do?" << endl;
            cin >> answer;
            cout << endl;
        }//while
        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    } //try
    catch (SQLException &e) {
        cout << e.what();
        return 0;
    }
    cout << "Thanks for using the application. Bye" << endl;

    return 0;
} //main

void displayClients(Connection *conn) {
    cout << "Below is your client list stored in your system" << endl;
    string clients = "";
    clients = clients + "select * from clients";
    Statement *stmt = conn->createStatement(clients);
    ResultSet *rs = stmt->executeQuery();
    while (rs->next()) {
        int cid;
        string lastname, firstname, address, city, phone, email;
        cid = rs->getInt(1);
        lastname = rs->getString(2);
        firstname = rs->getString(3);
        address = rs->getString(4);
        city = rs->getString(5);
        phone = rs->getString(6);
        email = rs->getString(7);
        cout << "Client Id: " << cid << endl;
        cout << "Last Name: " << lastname << endl;
        cout << "First Name: " << firstname << endl;
        cout << "Address: " << address << endl;
        cout << "City: " << city << endl;
        cout << "Phone: " << phone << endl;
        cout << "Email: " << email << endl;
        cout << "--------------------";
        cout << endl;
    }
    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);
} //displayClients()

void insertClients(Connection *conn) {
    string cfname, clname, cemail, caddress, cphone, ccity;
    string ans;
    int clid;
    cout << "Please enter the client information into the system" << endl;
    cout << "Please enter client id: ";
    cin >> clid;
    cout << endl;
    cout << "Please enter client last name: ";
    cin >> clname;
    cout << endl;
    cout << "Please enter client first name: ";
    cin >> cfname;
    cout << endl;
    cout << "Please enter client address: ";
    cin.ignore();
    getline(cin, caddress);
    //cin>>caddress;
    cout << endl;
    cout << "Please enter client city: ";
    cin >> ccity;
    cout << endl;
    cout << "Please enter client phone number: ";
    cin >> cphone;
    cout << endl;
    cout << "Please enter client email: ";
    cin >> cemail;
    cout << endl;
    string insertClient = "";
    insertClient = insertClient + "INSERT INTO clients " + "VALUES (:1, :2, :3, :4, :5, :6, :7)";
    Statement *insertStatement = conn->createStatement(insertClient);
    insertStatement->setInt(1, clid);
    insertStatement->setString(2, clname);
    insertStatement->setString(3, cfname);
    insertStatement->setString(4, caddress);
    insertStatement->setString(5, ccity);
    insertStatement->setString(6, cphone);
    insertStatement->setString(7, cemail);
    insertStatement->executeQuery();
    cout << "Would you like to save the client information in the system. Please enter yes to proceed: ";
    cin >> ans;
    cout << endl;
    if (ans == "yes") {
        conn->commit();
        cout << "Your new client has been added into the system. Have a nice day" << endl;
    }
    conn->terminateStatement(insertStatement);
}//insertClients

void removeClient(Connection *conn) {
    //string Clname;
    int Clid;
    string answer;
    cout << "Please enter the client id to remove client from the system" << endl;
    cout << "Please enter client id: ";
    cin >> Clid;
    cout << endl;
//    cout<<"Please enter client name";
//    cin>>Clname;
    cout << endl;
    string deleteClient = "";
    deleteClient = deleteClient + "DELETE from clients where clientid = :1";
    Statement *stmt = conn->createStatement(deleteClient);
    stmt->setInt(1, Clid);
    stmt->executeQuery();
    cout << "To remove the client please enter yes to proceed: ";
    cin >> answer;
    cout << endl;
    if (answer == "yes") {
        conn->commit();
        cout << "Your new client has been removed from the system. Have a nice day" << endl;
    }
    conn->terminateStatement(stmt);
}//removeClient
void updateClient(Connection *conn) {
    string address, city;
    int clientid;
    cout << "Please update clients address and city" << endl;
    cout << "Please enter client id you want to change the address and city for: ";
    cin >> clientid;
    cout << "Please enter client updated address: ";
    cin.ignore();
    getline(cin, address);
    cout << endl;
    cout << "Please enter client updated city name: ";
    cin >> city;
    cout << endl;
    string updateClient = "";
    updateClient = updateClient + "UPDATE clients SET address = :1, city = :2 where clientid = :3";
    Statement *stmt = conn->createStatement(updateClient);
    stmt->setInt(3, clientid);
    stmt->setString(1, address);
    stmt->setString(2, city);
    stmt->executeQuery();
    conn->commit();
    conn->terminateStatement(stmt);
}//updateClient


void displayProject(Connection *conn) {
    int clientID;
    string firstName, lastName, projectName, address, city, phone, email;
    cout<<"Please enter the client information to see which project they are working on"<<endl;
    cout<<"Please enter the client id: ";
    cin>>clientID;
    cout<<endl;
    cout<<"Please enter the client last name: ";
    cin>>lastName;
    cout<<endl;
    cout<<"Please enter the client first name: ";
    cin>>firstName;
    cout<<endl;
    cout<<"Now you will be displayed with client information including their address, their contact information and the project assigned to them";
    cout<<endl;
    string displayProject = "";
    displayProject = displayProject + "select PROJECTNAME, ADDRESS, CITY, PHONE, EMAIL "
            + "from projects, clients where projects.client=clients.clientid and clients.clientid = :1";
    Statement *stmt = conn->createStatement(displayProject);
    stmt->setInt(1, clientID);
    ResultSet *rs = stmt->executeQuery();
    while (rs->next()) {
        projectName = rs->getString(1);
        address = rs->getString(2);
        city = rs->getString(3);
        phone = rs->getString(4);
        email = rs->getString(5);
        cout << "Project Name: " << projectName << endl;
        cout << "Address: " << address << endl;
        cout << "City: " << city << endl;
        cout << "Phone: " << phone << endl;
        cout << "Email: " << email << endl;
        cout << "--------------------";
        cout << endl;
    }
    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);
}//displayProject

void insertProject(Connection *conn){
cout<<"Now you can enter a new project into the system and assign a client to the project"<<endl;
    string projectName, ans;
    int client, projectid;
    cout << "Please enter the project information into the system" << endl;
    cout << "Please enter project id: ";
    cin >> projectid;
    cout << endl;
    cout << "Please enter project name: ";
    cin.ignore();
    getline(cin, projectName);
    cout << endl;
    cout<<"Please enter client id you want to assign project to: ";
    cin>>client;
    cout<<endl;
    string insertProject = "";
    insertProject = insertProject + "INSERT INTO projects " + "VALUES (:1, :2, :3)";
    Statement *insertStatement = conn->createStatement(insertProject);
    insertStatement->setInt(1, projectid);
    insertStatement->setString(2, projectName);
    insertStatement->setInt(3, client);
    insertStatement->executeQuery();
    cout << "Would you like to save the project information in the system. Please enter yes to proceed: ";
    cin >> ans;
    cout << endl;
    if (ans == "yes") {
        conn->commit();
        cout << "Your new project has been added into the system. Have a nice day" << endl;
    }
    conn->terminateStatement(insertStatement);

}//insertProject