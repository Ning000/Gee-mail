#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <stdexcept>
#include <iostream>

#define PASS_SIZE 

//#include "dbhandler.cpp"
#include "secure_functions.cpp"
#include "authentication.cpp"
//#include "hashing.cpp"
#include "dbHandler.cpp"
#include "encryption.cpp"
//#include "message.cpp"

using namespace std;

void drawMenu(string Username) {
	cout << endl << endl;
	system("clear");
	cout << endl;
	cout << "*****************************************" << endl;
	cout << "*                                       *" << endl;
	cout << "*          Welcome to GEE-MAIL!         *" << endl;
	cout << "*                                       *" << endl;
	cout << "*                          by group 14  *" << endl;
	cout << "*****************************************" << endl;

	cout << "You have " << getMessageCount(Username) << " message(s)." << endl;
	cout << endl;
	cout << "What would you like to do? Please select" << endl;
	cout << "(R)ead Messages, (S)end Message, (L)og out" << endl;

}


void sendAMessage(string Username){
    message m;
    m.From = Username;
    cout << "To: ";
    cin >> m.To;
    cin.get();
    string pw;
    bool flag = false;
    m.Salt = newSalt();
    while (!flag){
        pw = getpass("Password: ",true);
        if (pw == getpass("Verify Password: ",true)){
            pw = hashFor(pw, m.Salt, 100000);
            flag = true;
        }
    }
    cout << "Message: ";
    getline(cin, m.Message);
    m.Message = encrypt(pw, m.Salt, m.Message);
    sendMessage(m);
    cout << "Message Sent!" << endl;
    unsigned char action = 0;
    cout << "Press enter to return to main menu" << endl;
    while(action != 10){
        action = getch();
    }
    drawMenu(Username);
}


void readAMessage(string Username){
    vector<string> froms = messagesFrom(Username);
    
    
    cout << "You have messages from: ";
    for (int i = 0; i<froms.size(); i++){
        if (froms[i] != ""){
            cout << froms[i];
            if(i!=froms.size()-1){
                cout << ", ";
            }
        }
    }
    cout << endl << endl;
    cout << "Who do you want to read a message from? " << endl;
    cout << "User: ";
    string user = "";
    //cin.get();
    cin >> user;
    while(!(find(froms.begin(), froms.end(),user)!=froms.end())){
        cout << "Selected user has not sent you a message, please give a valid user: ";
        cin >> user;
        //cin.get();
    }
    char ch; //hey i think i had something that might've been working here
            //I also fixed send a message so that it can be more than one word
            //cin on its own didn't like the spaces...and it successfully
            //decrypted too although even though it was just working now there are
            //weird double enters ness for cin?? but before that the only thing
            //that was bothering me was that the menu doesn't come back up after
            //you send/read
    //flush buffer
    while ((ch = getchar()) != '\n' && ch != EOF);
    message Mess = getRecentMessage(Username, user);
    //cin.get();
    //cin.get();
    //cin.get();
    string password = getpass("Password: ",true);
    string hash = hashFor(password, Mess.Salt,100000);
    Mess.Message = decrypt(hash, Mess.Salt, Mess.Message);
    cout << "Message: " << Mess.Message << endl;
    unsigned char action = 0;
    cout << "Press enter to return to main menu" << endl;
    while(action != 10){
        action = getch();
    }
    drawMenu(Username);
    
    //vector<string> messagesFrom(string myusername){
    //string getRecentMessage(string myusername, string from){
}

bool isValidAction(char a){
    switch(a){
        case 'r': return true;
        case 'R': return true;
        case 's': return true;
        case 'S': return true;
        case 'l': return true;
        case 'L': return true;
        default: return false;
    }
}


int main(int argc, char **argv){
    if (argc == 1){
        try{
            bool loggedin = false;
            string Option = "";
            string Username = "";
            string password = "";
            string PasswordHash = "";
            string salt = "";
            //SecuString Password;
            while(!loggedin){
                cout << "Username: ";
                cin >> Username;
                cin.get();
                //Username = "hi";
                //fseek(stdin,0,SEEK_END);
                if(userExists(Username)){
                    password=getpass("Password: ",true);
                    salt = getSalt(Username);
                    PasswordHash = hashFor(password, salt, 100000);
                    if(getPassword(Username) == PasswordHash){
                        loggedin = true;
                    } else {
                        cout << "Invalid Password" << endl;
                    }
                } else {
                    cout << "User doesn't exist, Creating account..." << endl;
                    password=getpass("New Password: ",true);
                    if(password == getpass("Verify Password: ",true)){
                        cout << "Creating user..." << endl;
                        salt=newSalt();
                        createUser(Username,hashFor(password, salt, 100000),salt);
                        cout << "User Created Successfully, Please login" << endl;
                    } else {
                        cout << "Passwords didn't match, please try again" << endl;
                    }
                }
            }
            
            bool done = false;
            unsigned char action = 0;
            drawMenu(Username);
            while(!done){
                //drawMenu(Username);
                //cin.get();
                unsigned char action = 0;
                while(!isValidAction(action)){
                    //cin >> action;
                    action = getch();
                    //action=cin.get();
                }
                drawMenu(Username);
                if(action=='R' || action=='r'){
                    if(getMessageCount(Username)>0){
                        readAMessage(Username);
                    } else {
                        cout << "Why are you trying to read messages you don't have?" << endl;
                    }
                }
                else if(action=='S'||action=='s'){
                    sendAMessage(Username);
                }
                else if(action=='L'||action=='l'){
                    done=true;
                }
                else {
                    throw runtime_error("Somehow, You won... you beat logic itself");
                }
            }
            cout << "Good bye!" << endl;
            system("clear");            
            //out << "Log in or Create Account?" << endl;
            
            // cout << "Username: ";
            // cin >> Username;
            // if userExists(Username)
            // password=getpass("Password: ",true);
            // PasswordHash = hashIt(password)
            
            //Password.set("test");
            //out << Password.get() << endl;
            //Password->string_ = "Passw0rd";
            //cout << "Password: " << Password->get() << endl;
            
        } catch (invalid_argument const &e){
            cout << "Argument Error: " << e.what() << endl;
            return 1;
        } catch (bad_alloc const &e){
            cout << "Memory Error: " << e.what() << endl;
            return 2;
        } catch (runtime_error const &e){
            cout << "Runtime Error: " << e.what() << endl;
            return 3;
        } catch (exception const &e){
            cout << "Other Error: " << e.what() << endl;
            return 4;
        } catch (...){
            cout << "Unknown Error has Occured" << endl;
            return 5;
        }
    }
    return 0;
}

