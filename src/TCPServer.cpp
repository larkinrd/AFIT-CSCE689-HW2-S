#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <strings.h>
#include <vector>
#include <iostream>
#include <memory>
#include <sstream>
#include "TCPServer.h"
//LARKINS ADDITIONS
#include <fstream> //for using input file stream
#include <stdio.h> // for reading and writing text file
#include <stdlib.h> // for reading and writing text file
#include <cstring> //is/was using insecure c string for whitelist
#include "strfuncts.h"



TCPServer::TCPServer(){ // :_server_log("server.log", 0) {
}

//As Lt Col Noel just explained this Constructor, like in PasswdMgr
// only takes in a const char* and saves it to a private string
// variable named _log_file
TCPServer::TCPServer(const char *log_file):_log_file(log_file) {
   //std::cout << "Enter TCPServer(const char *log_file):_server_log(log_file) Constructor\n";
  // std::cout << "_log_file variable is: " << _log_file << "\n";
   //std::cout << "_log_file argument is: " << log_file << "\n";
 
// HERE IS MY CODE

//STEP XX: read in a file using #include <stdio.h> and <stdlib.h> at top
         
      FILE *fptr;
      //if ((fptr = fopen("./whitelist.txt","r")) == NULL){
      //   printf("Cannot open input file - whitelist.txt.\n");
      //STEP 3B: If no file, create one using #include <stdio.h> and <stdlib.h> at top
      fptr = fopen(_log_file.c_str(),"w");
      printf("TO SERVER: Created logfile in current working directory\n");
      if(fptr == NULL)
      {
         printf("Error!");   
         exit(1);             
      }
      //printf("Enter num: ");
      //scanf("%d",&num);
      //fprintf(fptr,"10.10.10.10\n"); //Remeber the '\n'
      //fprintf(fptr,"127.0.0.2\n");  // Add a second entry 
      //fprintf(fptr, "127.0.0.1\n"); // Comment out and delete ./whitelist.txt to test
      //printf("Wrote whitlisted IPs to ./logfile\n");
      fprintf(fptr,"Successfully Created and wrote to my Log File\n"
         "= = = = = = = = = = = = = = = = \n\n"); //Remeber the '\n'
      fclose(fptr);// Close the fptr object for future use
      //}  

   /*********************************
    * HERE IS COL NOELS CODE I morphed and not needed anymore

   //just created the logfile ?object? within the constructor
   FileFD logfile(_log_file.c_str());

   //test to open the logfile
   if (!logfile.openFile(FileFD::readfd))
      throw pwfile_error("Could not open log file for reading");

   //use COL NOELS functions to append to file
   logfile.openFile(FileFD::appendfd);
   logfile.writeFD("use noels to apend to file\n");
   std::string justastring ("this is just a string\n");
   logfile.writeFD(justastring);

   int bytesWritten = writeToLogFile(logfile, justastring);
   //std::cout << "wrote " << i << " bytes to log file in constructor" << std::endl;
   ****************************************************/

   //std::cout << "Press Enter Key to Exit PasswdMgr() Constructor\n\n";
   //getchar();

   //FOR FUTURE USE JUST COPY THIS CODE TO WRITE LOG MESSAGE
   // FileFD logfile(_log_file.c_str());
   // std::string logmsg ("this is just a log msg\n");
   // int bytesWritten = writeToLogFile(logfile, logmsg);
   // std::cout << "wrote " << bytesWritten << " bytes to log file" << std::endl;
   
   //TO MAKE BETTER, 
   // Create a logmsg buffer variable
   // Do I always need to declare the FileFD???

}

TCPServer::~TCPServer() {

}

/**********************************************************************************************
 * bindSvr - Creates a network socket and sets it nonblocking so we can loop through looking for
 *           data. Then binds it to the ip address and port
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::bindSvr(const char *ip_addr, short unsigned int port) {

   struct sockaddr_in servaddr;

   // _server_log.writeLog("Server started.");
   /*LARKIN - Time permitting... write to a log file*/
   FileFD logfile(_log_file.c_str());
   _logmsg = "Server Started";
   int bytesWritten = writeToLogFile(logfile, _logmsg); 
   // Set the socket to nonblocking
   _sockfd.setNonBlocking();

   // Load the socket information to prep for binding
   _sockfd.bindFD(ip_addr, port);
   
   //FileFD logfile(_log_file.c_str()); //Not sure if I need this everytime
   //logfile.openFile(FileFD::appendfd);
   //std::string justastring ("this is just a string in bindSvr()\n");
   //logfile.writeFD(justastring);
   //logfile.writeFD("Ended bindSvr()\n");
   //logfile.closeFD();
   //std::cout << "this executed\n";

   //FOR FUTURE USE JUST COPY THIS CODE TO WRITE LOG MESSAGE
   
   //Commented out below becuase I made a class wide private variable
   // called _logmsg
    //std::string logmsg ("Completed bindSvr() function");
    _logmsg = "Completed bindSvr() function";
    bytesWritten = writeToLogFile(logfile, _logmsg);
    //std::cout << "wrote " << bytesWritten << " bytes to log file" << std::endl;
   
}

/**********************************************************************************************
 * listenSvr - Performs a loop to look for connections and create TCPConn objects to handle
 *             them. Also loops through the list of connections and handles data received and
 *             sending of data. 
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::listenSvr() {

   bool online = true;
   timespec sleeptime;
   sleeptime.tv_sec = 0;
   sleeptime.tv_nsec = 100000000;
   int num_read = 0;
   FileFD logfile(_log_file.c_str()); //create logfile FildFD
   //std::string logmsg; //decalre local logmsg variable for listenSvr()
   int bytesWritten = 0; //for use with writing to logfile

   // Start the server socket listening
   _sockfd.listenFD(5);

    
   while (online) {
      struct sockaddr_in cliaddr;
      socklen_t len = sizeof(cliaddr);

      if (_sockfd.hasData()) {
         TCPConn *new_conn = new TCPConn();//*this);
         /*LARKIN COMMENTS*****
         * The -> is called the arrow operator which is used to 
         * access members of a structure. Use the dot operator (.) to
         * access members of a structure through a pointer
         */ 
                  
         if (!new_conn->accept(_sockfd)) {
            // _server_log.strerrLog("Data received on socket but failed to accept.");
            continue;
         }
         std::cout << "***Got a connection***\n";
         /****BEGIN LARKIN COMMENTS****
         * Before we put the connection on the stack we check to see if on whitelist
         *****END LARKIN COMMENTS*****/

         //STEP 1: get the IP Address of the client 
         /****BEGIN LARKIN COMMENTS****
         * I used Lt Col Noel's function which is found later (scroll down) in TCPConn.cpp
         *    his code has been commented out. Here is my understanding of his fucntion(s)
         * This creates and passes ipaddr_str to the getIPAddrStr() function and from the
         * best I can tell we are just USING the function in the TCPconn class to get
         * the IP address. It can do this becuase each TCPconn Object has a set of private
         * variables including a SocketFD object which inherits from FileDesc and in FileDesc
         * there is a getIPAddrStr() which the REAL FUNCTION THAT gets the IP Address of the client
         * and updates the "buffer"... or in this case the std::string client_ipaddr_str we declared
         * *****END LARKIN COMMENTS*****/       
         std::string client_ipaddr_str;
         new_conn->getIPAddrStr(client_ipaddr_str);
         _logmsg = "Received connection from IP ";
         _logmsg += client_ipaddr_str;
         bytesWritten = writeToLogFile(logfile, _logmsg);
              
         //STEP 2: compare the IP address to a whiltelist via hard code
         /* Code commented out becuase I got it working
         std::string whitelist_ipaddr_str ("127.0.0.1");
         if (whitelist_ipaddr_str.compare(client_ipaddr_str) == 0) {
            new_conn->sendText("TO CLIENT: You ARE on my whitelist\n");
            std::cout << "TO SERVER: Authorized connection from: " << client_ipaddr_str << "\n";
         } else {
            new_conn->sendText("TO CLIENT: You are NOT on my whitelist\n");
            std::cout << "TO SERVER: Unauthorized connection attempt from: " << client_ipaddr_str << "\n";
            new_conn->disconnect();
         }*/
 
         //STEP 3: compare the IP address to a whitelist in a file

         /****BEGIN LARKIN COMMENTS****
         * Was trying ot use input file strem here... i forgot where I got the code
         *std::ifstream filein("whitelist.txt");
         *   if(!filein) {
         *   std::cout << "Cannot open input file - whitelist.txt.\nCreating Whitelist File...\n";
         *****END LARKIN COMMENTS*****/

         //STEP 3A: read in a file using #include <stdio.h> and <stdlib.h> at top
         int num;
         FILE *fptr;
         
         //commmented out next to lines so that whitelist is created
         //every time this runs
         //if ((fptr = fopen("./whitelist.txt","r")) == NULL){
         //   printf("Cannot open input file - whitelist.txt.\n");
         //STEP 3B: If no file, create one using #include <stdio.h> and <stdlib.h> at top
               fptr = fopen("./whitelist","w");
               printf("TO SERVER: Created whitelist file in current working directory\n");
               if(fptr == NULL)
               {
                  printf("Error!");   
                  exit(1);             
               }
               //printf("Enter num: ");
               //scanf("%d",&num);
               fprintf(fptr,"10.10.10.10\n");
               fprintf(fptr,"127.0.0.2\n");  // Add a second entry 
               fprintf(fptr, "127.0.0.1\n"); // Comment out and delete ./whitelist.txt to test
               printf("TO SERVER: Wrote whitlisted IPs to ./whitelist\n");
               fclose(fptr);// Close the fptr object for future use
         //}

         /****BEGIN LARKIN COMMENTS****
         * This whole section of STEP3C was a failure... here were my gotcha's
         * adding .c_str() 'coerces' a string into a regular c-string char array
         * fscanf() wanted a c-style char array which is how i found adding .c_str()
         * printf() and std::cout are Different and DIFFERENT for character array vs std::string
         *   I got all sorts of messed up and am still learning strings, char arrays, pointers
         * IO_FILE is different from ifstream ?input file stream? and they handle char[] vs string different
         * Some places I needed to append a '\n' like in fprintf() or else fscanf() would
         *   not find the end a whitepace character (space, null terminator, newline)
         *

         //STEP 3C - ATTEMPT ONE: reopen the file, read in the value(s), compare them, deterimine if 
         // client_ipaddr_str is in whitelist.txt
         
         if ((fptr = fopen("./whitelist.txt","r")) != NULL){
            printf("Opened whitelist.txt\n");
            //char* readfilecontents[30];
            //std::string readfilecontents ("255.255.255.255"); //I GET A SEGMENTATION FAULT WHEN I TRY THIS
            //CREATING A STRING OF 16 NULL CHARACTERS SINCE IPV4 WILL AT MOST BE 16 CHARS 255.255.255.255
            std::string readfilecontents ("127.0.0.1"); //I GET A SEGMENTATION FAULT WHEN I TRY THIS
            //this effing sucks... so adding .c_str() 'coerces' a string into a regular c-string char array
            while (fscanf(fptr, "%s", readfilecontents.c_str()) == 1) {
               //std::cout << "inside while loop\n";
               //fscanf(fptr, "%s", tempstring);
               //printf("Print the fptr: %s\n", readfilecontents); //NULL TERMINATED VS NEWLINE???
               //printf("Print the client IP is: %s\n", client_ipaddr_str);
               //std::string tempstring;
                  //printf("readfilecontents is: %s\n", readfilecontents);
                  //printf("*readfilecontents is: %s\n", *readfilecontents); //SEG FAULT
                  std::cout << "client+ipaddr_str is: " << client_ipaddr_str << "\n";
                  std::cout << "readfilecontents with cout is: " << readfilecontents << "\n";
               if (readfilecontents.compare(client_ipaddr_str) == 0) {
                  std::cout << "Strings are equal: " << client_ipaddr_str << "\n";
               } else {
                  std::cout << "Strings are NOT EQUAL\n";
               }
            }
            fclose(fptr);   
         }
         *****END LARKIN COMMENTS*****/       
         
         //STEP 3C - ATTEMPT TWO: Using ifstream code from https://www.youtube.com/watch?v=zVI1hENR9g4
         std::ifstream inputFile;
         std::string fromfile;
         int isOnWhitelist = 0;
         inputFile.open("./whitelist");
         while (inputFile >> fromfile) // The '>>' operator has been overloaded
         // VS CODE HOVER on '>>' says it extracts the rvalue stream... so stops when it sees a whitespace?
         {
            if(!inputFile.eof()){
            //std::cout << "TO SERVER: ./whitelist.txt file contains: " << fromfile << "\n";
               if (fromfile.compare(client_ipaddr_str) == 0) {
                     //std::cout << "TO SERVER: Strings are equal: " << client_ipaddr_str << "\nStart Authentication";
                     new_conn->sendText("TO CLIENT: You ARE on my whitelist\n");
                     _logmsg = "Connection from Whitelisted IP ";
                     _logmsg += client_ipaddr_str;
                     _logmsg += " accepted";
                     bytesWritten = writeToLogFile(logfile, _logmsg);
                     isOnWhitelist = 1;
                     //break; //break from ????while loop, add TCPConn object to the 'stack' of std::unique_ptr<TCPconn> and
                           // start authentication
                  _connlist.push_back(std::unique_ptr<TCPConn>(new_conn));
                  new_conn->sendText("Welcome to the CSCE 689 Server!\n");
                  new_conn->startAuthentication();

               }
            }
         }
         inputFile.close();
         //After looping over the whitelist file, test if user was on whitelist
         if(isOnWhitelist == 0){
               //} else {
                  //std::cout << "Strings are NOT EQUAL\n";
                  //std::cout << "TO SERVER: Unauthorized connection attempt from: " << client_ipaddr_str << "\n";
                  new_conn->sendText("TO CLIENT: You are NOT authorized to connect\n");
                  new_conn->disconnect();
                  _logmsg = "Unauthorized connection attempt from IP ";
                  _logmsg += client_ipaddr_str;
                   bytesWritten = writeToLogFile(logfile, _logmsg);
                 
               //}
         }         

         /****BEGIN LARKIN COMMENTS****
         * SUMMARY: we PUSH a TCPConn Object onto the stack and std::unique_ptr manages
         *    the items on the stack
         * _connlist is a private variable in class TCPServer.h
         * push_back is STL function to push an object onto the stack
         * std::unique_ptr is a smart pointer that owns and manages another object
         *    through a pointer and disposes of that object when the unique_ptr goes
         *    out of scope. 
         *****END LARKIN COMMENTS*****/       
         //_connlist.push_back(std::unique_ptr<TCPConn>(new_conn));

         //Lt Col Noels code commented out becuase I used it eariler (scroll up)
         //std::string ipaddr_str;
         //new_conn->getIPAddrStr(ipaddr_str); 
         
         //new_conn->sendText("Welcome to the CSCE 689 Server!\n");
         
         // Change this later
         //We have a valid connection on our whitelist, so begin authentication
         //new_conn->startAuthentication();
      }

      // Loop through our connections, handling them
      std::list<std::unique_ptr<TCPConn>>::iterator tptr = _connlist.begin();
      while (tptr != _connlist.end())
      {
         // If the user lost connection
         if (!(*tptr)->isConnected()) {
            // Log it

            // Remove them from the connect list and log to logfile
            _logmsg = "IP ";
            std::string tempstring; 

            //I GUESS this iterates over unique PTRs that contain
            //TCPconn objects, the syntax below gives us access to 
            //TCPconn methods/functions
            (*tptr)->getIPAddrStr(tempstring);
            _logmsg += tempstring;
            _logmsg += " disconnected from the server";
            bytesWritten = writeToLogFile(logfile ,_logmsg);
            
            tptr = _connlist.erase(tptr);
            std::cout << "Connection disconnected.\n";
            continue;
         }

         // Process any user inputs
         (*tptr)->handleConnection();
         
         //std::cout << "Looping over connections\n";

         // Increment our iterator
         tptr++;
      }

      // So we're not chewing up CPU cycles unnecessarily
      nanosleep(&sleeptime, NULL);
   } 


   
}


/**********************************************************************************************
 * shutdown - Cleanly closes the socket FD.
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::shutdown() {

   _sockfd.closeFD();
}

/**********************************************************************************************
 * writeToLogFile - add to the serverlog
 *    Params - Pass in the logfile and the String message to add
 *    Throws: ??
 **********************************************************************************************/


int TCPServer::writeToLogFile(FileFD &logfile, std::string &logmsg) {
   //std::cout << "serverlog() called" << std::endl;
   //THE File object must already be created and passed
   if (!logfile.openFile(FileFD::readfd))
      throw pwfile_error("Could not open log file for reading");

   //use COL NOELS functions to append to file
   logfile.openFile(FileFD::appendfd);
  
   // get the current date/time based on current system
   time_t now = time(0);
   
   // convert now to string form
   char* dt = ctime(&now);
   std::string mydatetime (dt); // convert char * to string
   clrNewlines(mydatetime);
   int bytesWritten = logfile.writeFD(mydatetime);
   bytesWritten += logfile.writeFD(" :: ");
   clrNewlines(logmsg); //strip any/all newlines
   bytesWritten += logfile.writeFD(logmsg); 
   bytesWritten += logfile.writeFD("\n"); //append a newline
   logfile.closeFD();
   std::cout << "wrote " << bytesWritten << " bytes to log file." << std::endl;
   return bytesWritten;
   //return 1;
}

