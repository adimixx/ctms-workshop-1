#include "Core.h"

CtmsDB *db;

User Login(), LoggedInUser;

long MainMenu();

int InputInt;

int main() {

    Database DatabaseConn;

    if (!DatabaseConn.ConnectionFunction()) {
        return 0;
    }

    db = new CtmsDB(DatabaseConn);
    do {
        LoggedInUser = Login();
    } while (MainMenu() != 0);
}

User Login() {
    string username, password;
    bool LoggedIn = false;
    Dependency::ClearScreen("CTMS :: LOGIN");
    do {
        username = input::getInput("Username");
        password = input::getInput("Password");
        Dependency::ClearScreen("CTMS :: LOGIN");

        auto checkUser = from(db->user)
                >> where([&](User const &a) { return a.Username == username && a.Password == md5(password); })
                >> first_or_default();
        if (checkUser.Id == 0) {
            cout << "Invalid Username or Password. Please Try Again\n\n";
        } else if (checkUser.IsActive == false) {
            cout << "Your account has been disabled. Please contact Administrator\n\n";
        } else {
            LoggedIn = true;
            return checkUser;
        }
    } while (!LoggedIn);
}

long MainMenu() {
    do{
        Dependency::ClearScreen("CRUISE TICKETING MANAGEMENT SYSTEM\n");
        cout << "WELCOME " + LoggedInUser.Username + "\n\n\n";

        cout << "TICKETING MENU\n";
        cout << "1. Issue new Ticket\n";
        cout << "2. Check - In Ticket\n";
        cout << "\n\n";

        if ((from(db->user_role) >> where([&](User_Role const x) { return x.RoleID == 1 && x.UserID == LoggedInUser.Id; })
                                 >> count()) == 1) {
            cout << "ADMINISTRATOR MENU\n";
            cout << "3. Route Management\n";
            cout << "4. Vessel Management\n";
            cout << "5. Package Management\n";
            cout << "6. User Management\n";
            cout << "7. System Reporting\n";
            cout << "\n\n";
        }

        cout << "0. Logout\n";

        InputInt = input::InputInt("Enter your input");

        if (InputInt == 1) {

        } else if (InputInt == 3) {
            RouteManagement route(db);
            route.Menu();
        }

        else if (InputInt == 4) {
            VesselManagement vessel(db);
            vessel.Menu();
        }
        if (InputInt == 5) {
            PackageManagement packageManagement(db);
            packageManagement.Menu();
        }
    }while (InputInt != 0);

    Dependency::ClearScreen("LOGGING OUT :: CTMS\n");
    string Input = input::getInput("Press any key to Re-Login, 0 to Exit");
    if (Input == "0") return 0;
    return 1;
}
