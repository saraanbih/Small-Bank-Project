#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>


using namespace std;
const string ClientsFileName = "MyFile.txt";
const string UserFileName = "Users.txt";

enum enMainMenuePermissions
{
    eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
    pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
};

void ShowMainMenue();
bool CheckAccessPermission(enMainMenuePermissions Permission);
void ShowTansactionsMenueScreen();
void ShowTansactionsMenueScreen();
void Login();
void ShowAcessDenied();
void ShowManageUserMenueScreen();
void AddNewClients();

struct sUser
{
    string UserName;
    int Password;
    int Permissions;
    bool MarkForDelete = false;
};

sUser CurrentUser;

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

sUser ReadNewUser()
{
    sUser User;
    cout << "Enter UserName? ";
    getline(cin >> ws, User.UserName);
    cout << "Enter PassWord? ";
    cin >> User.Password;
    return User;
}

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double
    return Client;
}

sUser ConvertLineToRecord(string Line, string Seperator = "#//#")
{
    sUser User;
    vector<string> vUsersData;
    vUsersData = SplitString(Line, Seperator);

    User.UserName = vUsersData[0];
    User.Password = stoi(vUsersData[1]);
    User.Permissions = stoi(vUsersData[2]);
    return User;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

string ConvertRecordToLine(sUser User, string Seperator = "#//#")
{
    string stUserRecord = "";
    stUserRecord += User.UserName + Seperator;
    stUserRecord += to_string(User.Password) + Seperator;
    stUserRecord += to_string(User.Permissions) + Seperator;
    return stUserRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }

        MyFile.close();

    }
    return false;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";

    // Usage of std::ws will extract all the whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

vector <sClient> LoadClientsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

vector<sUser> LoadUserDataFromFile(string FileName)
{
    vector<sUser> vUsers;
    fstream Users;

    Users.open("Users.txt", ios::in);

    if (Users.is_open())
    {
        string Line;
        sUser User;

        while (getline(Users, Line))
        {
            User = ConvertLineToRecord(Line);
            vUsers.push_back(User);
        }
        Users.close();
    }
    return vUsers;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintUserRecordLine(sUser User)
{
    cout << "| " << setw(15) << left << User.UserName;
    cout << "| " << setw(10) << left << User.Password;
    cout << "| " << setw(40) << left << User.Permissions;
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
    {
        ShowAcessDenied();
        return;
    }

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void ShowAllUsersScreen()
{
    vector<sUser> vUsers = LoadUserDataFromFile(UserFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUsers.size() == 0)
        cout << "\t\t\t\tNo Users Avaliable In the System!";
    else
        for (sUser User : vUsers)
        {
            PrintUserRecordLine(User);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

void PrintUserCard(sUser User)
{
    cout << "The following are the client Details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername    : " << User.UserName;
    cout << "\nPassword    : " << User.Password;
    cout << "\nPermissions : " << User.Permissions;
    cout << "\n------------------------------------\n";
}

string ReadUserName()
{
    string UserName;
    cout << "Enter Username?";
    cin >> UserName;
    return UserName;
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;
}

bool FindUserByUserName(string UserName, vector<sUser> &vUsers, sUser& User)
{

    for (sUser &_User : vUsers)
    {
        if (_User.UserName == UserName)
        {
            User = _User;
            return true;
        }
    }
    return false;
}

bool FindUserByUsernameAndPassword(string Username, int Password, sUser& User)
{

    vector <sUser> vUsers = LoadUserDataFromFile(UserFileName);

    for (sUser U : vUsers)
    {

        if (U.UserName == Username && U.Password == Password)
        {
            User = U;
            return true;
        }

    }
    return false;

}

bool  LoadUserInfo(string Username, int Password)
{

    if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
        return true;
    else
        return false;

}

void ShowAcessDenied()
{
    cout << "\n--------------------------------------\n";
    cout << "Access Denied,";
    cout << "\nYou don't Have Permission To Do this,";
    cout << "\nPlease Contact Your Admin.";
}

int ReadPermissions()
{
    int Permissions = 0;
    char Answer = 'n';


    cout << "\nDo you want to give full access? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        return -1;
    }

    cout << "\nDo you want to give access to : \n ";

    cout << "\nShow Client List? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pListClients;
    }

    cout << "\nAdd New Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pAddNewClient;
    }

    cout << "\nDelete Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pDeleteClient;
    }

    cout << "\nUpdate Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pUpdateClients;
    }

    cout << "\nFind Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pFindClient;
    }

    cout << "\nTransactions? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pTranactions;
    }

    cout << "\nManage Users? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pManageUsers;
    }

    return Permissions;
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
    if (CurrentUser.Permissions == enMainMenuePermissions::eAll)
        return true;

    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;

}

short ReadMainMenueOption(int Permissions)
{
    short Choice;

    cout << "Choose what do you want to do? [1 to 8]? ";    
    cin >> Choice;

    if (Permissions > 127 || Permissions == -1)
        return Choice;
    else
        return 1;
   
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

sUser ChangeUserRecord(string UserName)
{
    sUser User;
    char Answer;

    User.UserName = UserName;
    
    cout << "\n\nEnter Password? ";
    cin >> User.Password;

    cout << "Are you sure you want to give full acess ? y/n ?";
    cin >> Answer;

    if (tolower(Answer) == 'y')
        User.Permissions = -1;
    else
        User.Permissions = ReadPermissions();

    return User;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;
}

bool MarkUserForDeleteByUserName(string UserName,vector<sUser> &vUsers)
{
    for (sUser &User: vUsers)
    {
        if (User.UserName == UserName)
        {
            User.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();
    }

    return vClients;
}

vector<sUser> SaveUsersDataToFile(string FileName,vector<sUser> vUsers)
{
    fstream Users;
    Users.open(FileName, ios::out);
    string DateLine;

    if (Users.is_open())
    {
        for (sUser &U : vUsers)
        {
            if (U.MarkForDelete == false)
            {
                DateLine = ConvertRecordToLine(U);
                Users << DateLine << endl;
            }
        }
        Users.close();
    }

    return vUsers;
}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }
}

void AddDataUserLineToFile(string FileName, string  stDataLine)
{
    fstream Users;
    Users.open(FileName, ios::out | ios::app);

    if (Users.is_open())
    {

        Users << stDataLine << endl;

        Users.close();
    }
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadClientsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

bool DeleteUserByUserName(string UserName,vector<sUser>& vUsers)
{
    sUser User;
    char Answer;

    if (FindUserByUserName(UserName,vUsers,User))
    {
        PrintUserCard(User);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (tolower(Answer) == 'y')
        {
            MarkUserForDeleteByUserName(UserName, vUsers);
            SaveUsersDataToFile(UserFileName, vUsers);

            vUsers = LoadUserDataFromFile(UserFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with UserName (" << UserName << ") is Not Found!";
        return false;
    }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

bool UpdateUserByUserName(string UserName,vector<sUser> &vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(UserName,vUsers,User))
    {
        PrintUserCard(User);
        cout << "\n\nAre you want to update this User? y/n ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            for (sUser &U : vUsers)
            {
                if (U.UserName == UserName)
                {
                    U = ChangeUserRecord(UserName);
                    break;
                }
            }

            SaveUsersDataToFile(UserFileName, vUsers);
            
            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with UserName (" << UserName << ") is Not Found!";
        return false;
    }
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
    {
        ShowAcessDenied();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Users Screen";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUsers = LoadUserDataFromFile(UserFileName);
    string UserName = ReadUserName();
    DeleteUserByUserName(UserName, vUsers);
}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
    {
        ShowAcessDenied();
        return;
    }
    
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Users Screen";
    cout <<  "\n-----------------------------------\n";

    vector<sUser> vUsers = LoadUserDataFromFile(UserFileName);
    string UserName = ReadUserName();
    UpdateUserByUserName(UserName, vUsers);
}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
    {
        ShowAcessDenied();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}

void ShowAddNewUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New User Screen";
    cout << "\n-----------------------------------\n";

}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
    {
        ShowAcessDenied();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUsers = LoadUserDataFromFile(UserFileName);
    sUser User;
    string UserName = ReadUserName();
    if (FindUserByUserName(UserName, vUsers, User))
        PrintUserCard(User);
    else
        cout << "User with UserName[" << UserName << "] is not found";
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

bool FindUser(sUser UserInfo,vector<sUser> vUsers,sUser &User)
{
   
    for(sUser _User : vUsers)
    {
        if(_User.UserName==UserInfo.UserName&&_User.Password==UserInfo.Password)
        {
            User = _User;
            return true;
        }
    }
    return false;
}

bool AddNewUser()
{
    sUser User;
    char FullAccess,AddMore;
    User = ReadNewUser();
    
    vector<sUser> vUsers = LoadUserDataFromFile(UserFileName);
    if (!FindUser(User, vUsers, User))
    {
        cout << "Do You Want to give full access? y/n?";
        cin >> FullAccess;
        if (toupper(FullAccess) == 'Y')
        {
            User.Permissions = -1;
        }
        else
        {
            User.Permissions = ReadPermissions();
        }
        AddDataUserLineToFile(UserFileName, ConvertRecordToLine(User));
    }
    else
        return false;
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

void AddNewUsers()
{
    char AddMore = 'Y';

    cout << "Adding New User:\n";

    do
    {
        if (!AddNewUser())
        {
            cout << "User Exists, please Enter another one\n";
            
            AddNewUser();
        }
        else
        {
            cout << "\nUsers Added Successfully, do you want to add more users? Y/N? ";
            cin >> AddMore;
        }

    } while (tolower(AddMore) == 'y');
}

void ShowDepositScreen()
{
    cout << "\n------------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n------------------------------------\n";
}

void ShowWithDrawScreen()
{
    cout << "\n------------------------------------\n";
    cout << "\tWithDraw Screen";
    cout << "\n------------------------------------\n";
}

short ReadManageUsersOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

enum enTransactionMenueOptions
{
    eDeposit = 1, eWithDraw = 2,
    eTotalBalances = 3, eMainMenue = 4
};

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6, eManageUsers = 7,
    eLogout = 8, eAccessDenied = 9
};

enum enManageUsersOptions
{
    eListUsers = 1, eAddNewUser = 2,
    eDeleteUser = 3, eUpdateUser = 4,
    eFindUser = 5, eMain = 6
};

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}

void GoBackToTransactionMenue()
{
    cout << "\n\nPress any key to go back to Transaction Menue...";
    system("pause>0");
    ShowTansactionsMenueScreen();
}

void ReturnToTransactionMenue()
{
    cout << "\n\nPress any key to go back to Transaction Menue...";
    system("pause>0");
    ShowManageUserMenueScreen();
}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

double ReadDespositAmount()
{
    double depositamount = 0;
    cout << "Please,Enter Deposit Amount?";
    cin >> depositamount;

    return depositamount;
}

double ReadWithDrawAmount()
{
    double withdrawamount = 0;
    cout << "Please,Enter Deposit Amount?";
    cin >> withdrawamount;

    return withdrawamount;
}

void Deposit()
{
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;

    string AccountNumber = ReadClientAccountNumber();
    double DepositAmount = 0;
    char DepositAgain;
    while (true)
    {
        if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        {   
            PrintClientCard(Client);
            DepositAmount = ReadDespositAmount();
            cout << "Are you sure you want perform this transaction? y/n ?";
            cin >> DepositAgain;
            if (DepositAgain == 'y' || DepositAgain == 'Y')
            {
                sClient C;
                for (sClient& C : vClients)
                {
                    if (C.AccountNumber == AccountNumber)
                    {
                        C.AccountBalance += DepositAmount;
                        SaveCleintsDataToFile(ClientsFileName, vClients);
                         cout << "Done Successfuly,New Balance is " << C.AccountBalance << endl;
                        break;
                    }
                }
              
            }
            else
            {
                GoBackToTransactionMenue();
            }
            break;
        }
        else
        {
            cout << "Client With [" << AccountNumber << "] does not exist.\n";
            AccountNumber = ReadClientAccountNumber();
        }
    }
}

void WithDraw()
{
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;

    string AccountNumber = ReadClientAccountNumber();
    double WithDrawAmount = 0;
    char WithDrawAgain;
    while (true)
    {
        if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        {
            PrintClientCard(Client);
            WithDrawAmount = ReadWithDrawAmount();
          
               if (Client.AccountBalance < WithDrawAmount)
                {
                    cout << "Amount Exceeds the balance, you can withdraw up to :" << Client.AccountBalance << endl;
                     WithDrawAmount = ReadWithDrawAmount();
                }
               else
                {
                    cout << "Are tyou sure you want perform this transaction? y/n ?";
                      cin >> WithDrawAgain;
                      if (WithDrawAgain == 'y' || WithDrawAgain == 'Y')
                        {
                          for (sClient& C : vClients)
                          {
                              if (C.AccountNumber == AccountNumber)
                              {
                                  C.AccountBalance += (-1 * WithDrawAmount);
                                  SaveCleintsDataToFile(ClientsFileName, vClients);
                                  cout << "Done Successfuly,New Balance is " << C.AccountBalance << endl;
                                  break;
                              }
                          }
                      }
                      else
                      {
                             GoBackToTransactionMenue();
                      }
                      break;
               }           
        }
        else
        {
            cout << "Client With [" << AccountNumber << "] does not exist.\n";
            AccountNumber = ReadClientAccountNumber();
        }
    }
}

void TotalBalances()
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    double totalbalances = 0;

    cout << "\n\t\t\t\t\Balances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
    {
        cout << "\t\t\t\tNo Clients Available In the System!";
    }
    else
    {
        for (sClient Client : vClients)
        {
            PrintClientRecordLine(Client);
            cout << endl;
            totalbalances += Client.AccountBalance;
        }
        
    }
        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;
        cout << "\t\t\t\tTotal Balances = " << totalbalances << endl;
    
}

void PerformTransactionMenueOption(enTransactionMenueOptions TransactionMenueOptions)
{
    switch (TransactionMenueOptions)
    {
    case enTransactionMenueOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        Deposit();
        GoBackToTransactionMenue();
        break;
    case enTransactionMenueOptions::eWithDraw:
        system("cls");
        ShowWithDrawScreen();
        WithDraw();
        GoBackToTransactionMenue();
        break;
    case enTransactionMenueOptions::eTotalBalances:
        system("cls");
        TotalBalances();
        GoBackToTransactionMenue();
        break;
    case enTransactionMenueOptions::eMainMenue:
        system("cls");
        ShowMainMenue();
        break;
    
    }
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eTransactions:
        system("cls");
        ShowTansactionsMenueScreen();
        break;
    case enMainMenueOptions::eManageUsers:
        system("cls");
        ShowManageUserMenueScreen();
        break;
    case enMainMenueOptions::eLogout:
        system("cls");
        Login();
        break;
    }
}

void ShowTansactionsMenueScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
    {
        ShowAcessDenied();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] WithDraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";

    PerformTransactionMenueOption((enTransactionMenueOptions)ReadTransactionsMenueOption());
}

void ShowMainMenue()
{
    short Choice;

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";
    
    cout << "Choose what do you want to do? [1 to 8]? ";
    cin >> Choice;
    
    PerfromMainMenueOption((enMainMenueOptions)Choice);
}

void PerfromManageUsersOption(enManageUsersOptions ManageUsersOption)

{
    switch (ManageUsersOption)
    {
    case enManageUsersOptions::eListUsers:
    {
        system("cls");
        ShowAllUsersScreen();
        ReturnToTransactionMenue();
        break;
    }
    case enManageUsersOptions::eAddNewUser:
        system("cls");
        AddNewUsers();
        ReturnToTransactionMenue();
        break;

    case enManageUsersOptions::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        ReturnToTransactionMenue();
        break;

    case enManageUsersOptions::eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        ReturnToTransactionMenue();
        break;

    case enManageUsersOptions::eFindUser:
        system("cls");
        ShowFindUserScreen();
        ReturnToTransactionMenue();
        break;
    case enManageUsersOptions::eMain:
        system("cls");
        ShowMainMenue();
        break;
    }
}

void ShowManageUserMenueScreen()
{

    if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
    {
        ShowAcessDenied();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    
    cout << "===========================================\n";
    PerfromManageUsersOption((enManageUsersOptions)ReadManageUsersOption());
}

void ShowLoginScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tLogin Screen";
    cout << "\n-----------------------------------\n";
}

bool FindUser(sUser &User,vector<sUser> &vUsers)
{
    for (sUser user : vUsers)
    {
        if (user.UserName == User.UserName && user.Password == User.Password)
        {
            User = user;
            return true;
        }
    }
    return false;
}

void Login()
{
    bool LoginFaild = false;

    string Username;
    int Password;
    do
    {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invlaid Username/Password!\n";
        }

        cout << "Enter Username? ";
        cin >> Username;

        cout << "Enter Password? ";
        cin >> Password;

        LoginFaild = !LoadUserInfo(Username, Password);

    } while (LoginFaild);

    ShowMainMenue();

}

int main()
{
    Login();
    
    system("pause>0");
    return 0;
} 

