#include <stdexcept>
#include <strings.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <iostream>
#include "TCPConn.h"
#include "strfuncts.h"
//LARKIN ADDITIONS
#include <thread>
#include <chrono>
#include "PasswdMgr.h"

// The filename/path of the password file so that if we ever want to 
//   use a different password file (the one written to disk), we don't
//   have to update everywhere in this file... just up here at the top
const char pwdfilename[] = "passwd";
const char logfilename[] = "logfile";
//PasswdMgr myPasswdMgrObject(pwdfilename);

TCPConn::TCPConn(){ // LogMgr &server_log):_server_log(server_log) {

}

//ENTER THE NEW CONSTRUCTOR WITH THE TCP SERVER OBJECT SO I 
//CAN WRITE TO MY LOG FILE... I ONLY WANT ONE TCP SERVER
//TCPConn::TCPConn(TCPServer &svr):_mysvr(svr){

//}

TCPConn::~TCPConn() {

}

/**********************************************************************************************
 * accept - simply calls the acceptFD FileDesc method to accept a connection on a server socket.
 *
 *    Params: server - an open/bound server file descriptor with an available connection
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

bool TCPConn::accept(SocketFD &server) {
   return _connfd.acceptFD(server);
}

/**********************************************************************************************
 * sendText - simply calls the sendText FileDesc method to send a string to this FD
 *
 *    Params:  msg - the string to be sent
 *             size - if we know how much data we should expect to send, this should be populated
 *
 *    Throws: runtime_error for unrecoverable errors
 **********************************************************************************************/

int TCPConn::sendText(const char *msg) {
   return sendText(msg, strlen(msg));
}

int TCPConn::sendText(const char *msg, int size) {
   if (_connfd.writeFD(msg, size) < 0) {
      return -1;  
   }
   return 0;
}

/**********************************************************************************************
 * startAuthentication - Sets the status to request username
 *
 *    Throws: runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPConn::startAuthentication() {

   // Skipping this for now
   _status = s_username;
   //std::cout << "TO SERVER: Inside TCPConn::startAuthentication and looping inside \n";
   
   _connfd.writeFD("Username: ");

   //THIS FUNCTION ONLY *STARTS* Authentication... it does not end it.. don't use code belo
   //if(_status == s_passwd){
   //   _connfd.writeFD("Enter password:");
   //} 
   //TESTING THIS... AM I ACCESSING MY PASSWORD FILE


   
}

/**********************************************************************************************
 * handleConnection - performs a check of the connection, looking for data on the socket and
 *                    handling it based on the _status, or stage, of the connection
 *
 *    Throws: runtime_error for unrecoverable issues
 **********************************************************************************************/

void TCPConn::handleConnection() {

   timespec sleeptime;
   sleeptime.tv_sec = 0;
   sleeptime.tv_nsec = 100000000;

   try {
      switch (_status) {
         case s_username:
            getUsername();
            break;

         case s_passwd:
            getPasswd();
            break;

         case s_menu:
            getMenuChoice();
            break;
   
         //IS THIS CALLING THE SAME PROCESS/FUNCTIONS TWICE SO USER ENTERS *NEW* PASSWORD TWICE
         //BEFORE WRITING TO THE PASSWORD FILE
         case s_changepwd: 
         case s_confirmpwd:
            changePassword();
            break;

         
         default:
            throw std::runtime_error("Invalid connection status!");
            break;
      }
   } catch (socket_error &e) {
      std::cout << "Socket error, disconnecting.";
      disconnect();
      return;
   }

   nanosleep(&sleeptime, NULL);
}

/**********************************************************************************************
 * getUsername - called from handleConnection when status is s_username--if it finds user data,
 *               it expects a username and compares it against the password database
 *
 *    Throws: runtime_error for unrecoverable issues
 **********************************************************************************************/

void TCPConn::getUsername() {
   // Insert your mind-blowing code here
   /* LARKIN - How do we find userdata?*/
   
   //STEP0: Hardcode username variable
   //_username = "bobby";
   //std::cout << "I'm in TCPConn::getUsername() and _username is:" << _username << "\n";
   //std::cout << "I'm in TCPConn::getUsername() and _inputbuf has:" << _inputbuf << "\n";
   
   //STEP1: Get the user input after they pass back a username
   if (_connfd.hasData()) {
   _connfd.readFD(_username);
   //std::cout << "_username entered was:" << _username << "\n";

   
   //STEP2: Check the username against the database file 'passwd' NOT PROGRAMMED YET
   PasswdMgr myPasswdMgrObject(pwdfilename);
   if(myPasswdMgrObject.checkUser(_username.c_str()) == true){
   //STEP3: IF Good username update status, prompt for password, ELSE disconnect client
   _status = s_passwd;
   _connfd.writeFD("Enter password: ");
   } else {
      //disconnect due to bad username
      _connfd.writeFD("Bad Username Disconnecting...goodbye!\n");
      disconnect();
   }
   


   }
}

/**********************************************************************************************
 * getPasswd - called from handleConnection when status is s_passwd--if it finds user data,
 *             it assumes it's a password and hashes it, comparing to the database hash. Users
 *             get two tries before they are disconnected
 *
 *    Throws: runtime_error for unrecoverable issues
 **********************************************************************************************/

void TCPConn::getPasswd() {
   // Insert your astounding code here
   //std::cout << "Enter getPasswd() and Do Something\n"; 

    
   //STEP0: Hardcode passwd variable
   //SKIPPING THIS
   
   //STEP1: Get the user password
   if (_connfd.hasData()) {
   //THERE IS NO _passwd private variable, but there is an inputbuf that I can use. Is this INSECURE?
   //created this... but prob dont need it
   //    std::string tempPasswd;
   //    _connfd.readFD(tempPasswd);
   //    std::cout << "passwd entered was:" << tempPasswd << "\n";
   
   _connfd.readFD(_inputbuf);
   //std::cout << "passwd entered was:" << _inputbuf << " and is stored in _inputbuf\n";
   
   //std::string enteredusername (_username);
   //std::string enteredpasswd (_inputbuf);
   PasswdMgr myPasswdMgrObject(pwdfilename);
   bool result = false;
   //std::cout << "result of pwdObject is: " << result;
   result = myPasswdMgrObject.checkPasswd(_username.c_str(), _inputbuf.c_str());
   //std::cout << "result of pwdObject is: " << result;
   if (result == false){
      _pwd_attempts++;
      _connfd.writeFD("Enter password: ");
   } else {
      _status = s_menu;
      _inputbuf = "";
      sendMenu();
   }
   
   if (_pwd_attempts > 1){
      _connfd.writeFD("Two Bad Password attempts... Disconnecting...goodbye!\n");
      disconnect();
   }

//   if(myPasswdMgrObject.checkPasswd(_username.c_str(), _inputbuf.c_str()) == true){
//      std::cout << "GOOD PASSWORD";
//   } else {
//      _pwd_attempts++;
//      if ( _pwd_attempts ==2 ) {
//      _connfd.writeFD("Two Bad Password attempts... Disconnecting...goodbye!\n");
//      disconnect();
//      }
//   }
   /*
   std::cout << "\nHalting progrmain in <TCPConn::getPasswd()>\n"
      "Prior to this you should have checked\n"
      "1. Was the username valid\n"
      "2. Is their password correct\n"
      "Press a key on tcpserver to display the menu to tcpclient\n";
   getchar();
   */
   //STEP3: IF user authenticates, send the menu, set status to menu, ELSE disconnect
   
   
   //_status = s_menu;
   //sendMenu();
   //std::cout << "Press Enter Key to Exit getPasswd()\n\n";
   //getchar();
   }
   //std::cout << "looping inside getPassword()\n";

}

/**********************************************************************************************
 * changePassword - called from handleConnection when status is s_changepwd or s_confirmpwd--
 *                  if it finds user data, with status s_changepwd, it saves the user-entered
 *                  password. If s_confirmpwd, it checks to ensure the saved password from
 *                  the s_changepwd phase is equal, then saves the new pwd to the database
 *
 *    Throws: runtime_error for unrecoverable issues
 **********************************************************************************************/

void TCPConn::changePassword() {
   // Insert your amazing code here
   std::cout << "Enter changePassword() and DO SOMETHING\n"; 
   std::cout << "Press Enter Key to Exit getPasswd()\n\n";
   getchar();
   
}


/**********************************************************************************************
 * getUserInput - Gets user data and includes a buffer to look for a carriage return before it is
 *                considered a complete user input. Performs some post-processing on it, removing
 *                the newlines
 *
 *    Params: cmd - the buffer to store commands - contents left alone if no command found
 *
 *    Returns: true if a carriage return was found and cmd was populated, false otherwise.
 *
 *    Throws: runtime_error for unrecoverable issues
 **********************************************************************************************/

bool TCPConn::getUserInput(std::string &cmd) {
   std::string readbuf;

   // read the data on the socket
   _connfd.readFD(readbuf);

   // concat the data onto anything we've read before
   _inputbuf += readbuf;

   // If it doesn't have a carriage return, then it's not a command
   int crpos;
   if ((crpos = _inputbuf.find("\n")) == std::string::npos)
      return false;

   cmd = _inputbuf.substr(0, crpos);
   _inputbuf.erase(0, crpos+1);

   // Remove \r if it is there
   clrNewlines(cmd);

   return true;
}

/**********************************************************************************************
 * getMenuChoice - Gets the user's command and interprets it, calling the appropriate function
 *                 if required.
 *
 *    Throws: runtime_error for unrecoverable issues
 **********************************************************************************************/

void TCPConn::getMenuChoice() {
   if (!_connfd.hasData())
      return;
   std::string cmd;
   if (!getUserInput(cmd))
      return;
   lower(cmd);      

   // Don't be lazy and use my outputs--make your own! Sir, its 0106... I've been at this since 1730 yesterday 29JAN
   std::string msg;
   if (cmd.compare("hello") == 0) {
      _connfd.writeFD("Sir, its 0106... I've been at this since 1730 yesterday 29JAN...I'm leaving this alone!\n");
      displayCountdown();
      sendMenu();
   } else if (cmd.compare("menu") == 0) {
      sendMenu();
   } else if (cmd.compare("exit") == 0) {
      _connfd.writeFD("Disconnecting...goodbye!\n");
      disconnect();
   } else if (cmd.compare("passwd") == 0) {
msg += " ITS 0244 AND I MADE AN HONEST EFFORT AT THIS. MY THOUGHT WAS TO:\n";
msg += " have user enter existing password and check it\n";
msg += " have user enter new password once\n";
msg += " have user enter new password second time\n";
msg += " check that the passwords are equal\n";
msg += " create a new password file\n";
msg += " copy in all the users over EXCEPT the current user\n";
msg += " add a 'new' user with the users new passwords\n";
msg += " delete the original password file\n";
msg += " rename the newpassword file to passwd\n";
      _connfd.writeFD(msg);
      displayCountdown();

      //_connfd.writeFD("New Password: ");
      //_status = s_changepwd;
   } else if (cmd.compare("1") == 0) {
      msg += "You want a prediction about the weather? You're asking the wrong Phil.\n";
      msg += "I'm going to give you a prediction about this winter. It's going to be\n";
      msg += "cold, it's going to be dark and it's going to last you for the rest of\n";
      msg += "your lives!\n";
      _connfd.writeFD(msg);
      displayCountdown();
   } else if (cmd.compare("2") == 0) {
      _connfd.writeFD("42\n");
      displayCountdown();
   } else if (cmd.compare("3") == 0) {
      _connfd.writeFD("That seems like a terrible idea.\n");
      displayCountdown();
   } else if (cmd.compare("4") == 0) {
      _connfd.writeFD("Why was this left blank Robert asks?\n");
      displayCountdown();
      sendMenu();
   } else if (cmd.compare("5") == 0) {
      _connfd.writeFD("I'm singing, I'm in a computer and I'm siiiingiiiing! I'm in a\n");
      _connfd.writeFD("computer and I'm siiiiiiinnnggiiinnggg!\n");
      displayCountdown();
      sendMenu();
   } else {
      msg = "Unrecognized command: ";
      msg += cmd;
      msg += "\n";
      _connfd.writeFD(msg);
   }

}

void TCPConn::displayCountdown () {
      std::cout << "countdown: ";
      _connfd.writeFD("countdown ");
      for (int i=3; i>0; --i) {
         std::cout << i << ".. ";
         _connfd.writeFD(".. ");
         std::this_thread::sleep_for (std::chrono::seconds(1));
      }
      //std::cout << "\nEnd displayCountdown() send menu to client\n";
      //_connfd.writeFD("End displayCountdown() receive menu from server\n");
      sendMenu();
}

/**********************************************************************************************
 * sendMenu - sends the menu to the user via their socket
 *
 *    Throws: runtime_error for unrecoverable issues
 **********************************************************************************************/
void TCPConn::sendMenu() {
   //std::cout << "Enter sendMenu() and DO SOMETHING\n"; 
   std::string menustr;

   // Make this your own!
   menustr += "Available choices: \n";
   menustr += "  1). Provide weather report.\n";
   menustr += "  2). Learn the secret of the universe.\n";
   menustr += "  3). Play global thermonuclear war\n";
   menustr += "  4). Do nothing.\n";
   menustr += "  5). Sing. Sing a song. Make it simple, to last the whole day long.\n\n";
   menustr += "Other commands: \n";
   menustr += "  Hello - this class is very difficult. I'm not\n";
   menustr += "          sure about the value in troubleshooting\n";
   menustr += "          off by one characters like newlines, whitespaces,\n";
   menustr += "          and other various 'C/C++ 'isms.' However,\n";
   menustr += "          I do appreciate your patience and understainding'\n";
   menustr += "          This is sure to help me with futuer ICS research.\n";
   menustr += "  Passwd - change your password\n";
   menustr += "  Menu - display this menu\n";
   menustr += "  Exit - disconnect.\n\n";

   _connfd.writeFD(menustr);
   
   //std::cout << "Press Enter Key to Exit sendMenu()\n\n";
   //getchar();   
   
}


/**********************************************************************************************
 * disconnect - cleans up the socket as required and closes the FD
 *
 *    Throws: runtime_error for unrecoverable issues
 **********************************************************************************************/
void TCPConn::disconnect() {
   _connfd.closeFD();
}


/**********************************************************************************************
 * isConnected - performs a simple check on the socket to see if it is still open 
 *
 *    Throws: runtime_error for unrecoverable issues
 **********************************************************************************************/
bool TCPConn::isConnected() {
   return _connfd.isOpen();
}

/**********************************************************************************************
 * getIPAddrStr - gets a string format of the IP address and loads it in buf
 *
 **********************************************************************************************/
void TCPConn::getIPAddrStr(std::string &buf) {
   return _connfd.getIPAddrStr(buf);
}
