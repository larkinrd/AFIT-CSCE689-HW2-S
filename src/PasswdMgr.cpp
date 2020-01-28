#include <argon2.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <list>
#include "PasswdMgr.h"
#include "FileDesc.h"
#include "strfuncts.h"

const int hashlen = 32;
const int saltlen = 16;

//The PasswdMgr constructor accepts a password file name and save it
// to the PasswdMgr classes private variable _pwd_file
PasswdMgr::PasswdMgr(const char *pwd_file):_pwd_file(pwd_file) {
   std::cout << "Enter PasswdMgr() Constructor and DO SOMETHING\n";
   std::cout << " _pwd_file variable is: " << _pwd_file << "\n";
   std::cout << "pwd_file argument is: " << pwd_file << "\n";
 
   std::cout << "Press Enter Key to Exit PasswdMgr() Constructor\n\n";
   getchar();
}


PasswdMgr::~PasswdMgr() {

}

/*******************************************************************************************
 * checkUser - Checks the password file to see if the given user is listed
 *
 *    Throws: pwfile_error if there were unanticipated problems opening the password file for
 *            reading
 *******************************************************************************************/

bool PasswdMgr::checkUser(const char *name) {
   //uses standard template library, a vector of unsigned 8 bit integers to store single chars
   // we have two variables to use at our disposal, first is passwd and second is salt
   std::cout << "Enter checkUser() and DO SOMETHING\n"; 
   
   std::vector<uint8_t> passwd, salt; //just contain garbage right now
   
   /************************
   //PROGRAM THIS AFTER WE ADD A USER INTO OUR FILE SUCCESSFULLY
   *************************/

   /*
   if( findUser(name, passwd, salt) == 0) {
      std::cout << "Found user return true\n";
      return true;
   }*/

   std::cout << "Press Enter Key to Exit checkUser()\n\n";
   getchar();    
   
   //else return false, did not find user
   return false; //UMMM was 'return result' the original code?
   //return result;  
}

/*******************************************************************************************
 * checkPasswd - Checks the password for a given user to see if it matches the password
 *               in the passwd file
 *
 *    Params:  name - username string to check (case insensitive)
 *             passwd - password string to hash and compare (case sensitive)
 *    
 *    Returns: true if correct password was given, false otherwise
 *
 *    Throws: pwfile_error if there were unanticipated problems opening the password file for
 *            reading
 *******************************************************************************************/

bool PasswdMgr::checkPasswd(const char *name, const char *passwd) {
   std::cout << "Enter checkPasswd() and DO SOMETHING\n"; 
   std::vector<uint8_t> userhash; // hash from the password file
   std::vector<uint8_t> passhash; // hash derived from the parameter passwd
   std::vector<uint8_t> salt;

   // Check if the user exists and get the passwd string
   if (!findUser(name, userhash, salt))
      return false;

   hashArgon2(passhash, salt, passwd, &salt);

   if (userhash == passhash)
      return true;

   std::cout << "Press Enter Key to Exit checkPasswd()\n\n";
   getchar();
   return false;
}

/*******************************************************************************************
 * changePasswd - Changes the password for the given user to the password string given
 *
 *    Params:  name - username string to change (case insensitive)
 *             passwd - the new password (case sensitive)
 *
 *    Returns: true if successful, false if the user was not found
 *
 *    Throws: pwfile_error if there were unanticipated problems opening the password file for
 *            writing
 *
 *******************************************************************************************/

bool PasswdMgr::changePasswd(const char *name, const char *passwd) {

   // Insert your insane code here
   std::cout << "Enter changePasswd() and DO SOMETHING\n"; 
   
   
   std::cout << "Press Enter Key to Exit changePasswd()\n\n";
   getchar();
   return true;
}

/*****************************************************************************************************
 * readUser - Taking in an opened File Descriptor of the password file, reads in a user entry and
 *            loads the passed in variables
 *
 *    Params:  pwfile - FileDesc of password file already opened for reading
 *             name - std string to store the name read in
 *             hash, salt - vectors to store the read-in hash and salt respectively
 *
 *    Returns: true if a new entry was read, false if eof reached 
 * 
 *    Throws: pwfile_error exception if the file appeared corrupted
 *
 *****************************************************************************************************/

bool PasswdMgr::readUser(FileFD &pwfile, std::string &name, std::vector<uint8_t> &hash, std::vector<uint8_t> &salt)
{
   // Insert your perfect code here!
   std::cout << "Enter readUser() and DO SOMETHING\n"; 

   //STEP 1: LOOP over the password file and print it to the screen
   
   // I NEED if(!pwfile.readbytes<uint8_t>(hash, 32) < 0)
   //print out what was passed

   
   std::cout << "Press Enter Key to Exit readUser()\n\n";
   getchar();
   return true;
}

/*****************************************************************************************************
 * writeUser - Taking in an opened File Descriptor of the password file, writes a user entry to disk
 *
 *    Params:  pwfile - FileDesc of password file already opened for writing
 *             name - std string of the name 
 *             hash, salt - vectors of the hash and salt to write to disk
 *
 *    Returns: bytes written
 *
 *    Throws: pwfile_error exception if the writes fail
 *
 *****************************************************************************************************/

int PasswdMgr::writeUser(FileFD &pwfile, std::string &name, std::vector<uint8_t> &hash, std::vector<uint8_t> &salt)
{
   std::cout << "Enter writeUser() and DO SOMETHING\n"; 
   int results = 0;

   // Insert your wild code here!

   std::cout << "Press Enter Key to Exit writeUser()\n\n";
   getchar();
   return results; 
}

/*****************************************************************************************************
 * findUser - Reads in the password file, finding the user (if they exist) and populating the two
 *            passed in vectors with their hash and salt
 *
 *    Params:  name - the username to search for
 *             hash - vector to store the user's password hash
 *             salt - vector to store the user's salt string
 *
 *    Returns: true if found, false if not
 *
 *    Throws: pwfile_error exception if the pwfile could not be opened for reading
 *
 *****************************************************************************************************/

bool PasswdMgr::findUser(const char *name, std::vector<uint8_t> &hash, std::vector<uint8_t> &salt) {
   std::cout << "Enter findUser() and DO SOMETHING\n"; 

   //print out args passed to function
   std::cout << "name: " << name << std::endl;
   std::cout << "hash: " << &hash << std::endl;
   std::cout << "salt: " << &salt << std::endl;

   FileFD pwfile(_pwd_file.c_str());

   // You may need to change this code for your specific implementation
   if (!pwfile.openFile(FileFD::readfd))
      throw pwfile_error("Could not open passwd file for reading");

   //open the password file and loop over all users
   bool eof = false;
   while (!eof) {
      std::string uname;

      if (!readUser(pwfile, uname, hash, salt)) {
         eof = true;
         continue;
      }

      if (!uname.compare(name)) {
         pwfile.closeFD();
         return true;
      }
   }

   hash.clear();
   salt.clear();
   pwfile.closeFD();
   
   std::cout << "Press Enter Key to Exit findUser()\n\n";
   getchar();
   return false;
}


/*****************************************************************************************************
 * hashArgon2 - Performs a hash on the password using the Argon2 library. Implementation algorithm
 *              taken from the http://github.com/P-H-C/phc-winner-argon2 example. 
 *
 *    Params:  dest - the std string object to store the hash
 *             passwd - the password to be hashed
 *
 *    Throws: runtime_error if the salt passed in is not the right size
 *****************************************************************************************************/
void PasswdMgr::hashArgon2(std::vector<uint8_t> &ret_hash, std::vector<uint8_t> &ret_salt, 
                           const char *in_passwd, std::vector<uint8_t> *in_salt) {
   // Hash those passwords!!!!
   std::cout << "Enter hashArgon2() and DO SOMETHING\n"; 
   
   // the argon2 wants a unit8_t array and NOT a vector
   // when the argon2 returns the array, loop over and put back into a vector

   std::cout << "Press Enter Key to Exit hashArgon2()\n\n";
   getchar();
}

/****************************************************************************************************
 * addUser - First, confirms the user doesn't exist. If not found, then adds the new user with a new
 *           password and salt
 *
 *    Throws: pwfile_error if issues editing the password file
 ****************************************************************************************************/

void PasswdMgr::addUser(const char *name, const char *passwd) {
   // Add those users!
   std::cout << "Enter addUser() and DO SOMETHING\n"; 
   
   //PRINT WHAT WE HAVE TO THE SCREEN
   std::cout << "user name is: " << name << "\n";
   std::cout << "passwd is: " << passwd << "\n";

   //create the uint8_t vectors to pass to the hashArgon2() function
   std::vector<uint8_t> bobhash;
   std::vector<uint8_t> bobsalt;
   
   for (int i = 0; i<32; i++){
      bobhash[i] = (rand() % 93) + 33;
      std::cout << bobhash[i];
   }
   std::cout << std::endl;

   std::string mysalt ("mysaltofsixteen");

   for (int i = 0; i<16; i++){
      bobsalt.at(i) = mysalt[i];
      std::cout << bobsalt[i];
   }
   std::cout << std::endl;

   std::cout << "call(ing) the hashArgon2() function\n";
   //i can always pass in a null when a fuction expects a pointer
   hashArgon2( bobhash, bobsalt, passwd, NULL);

   hashArgon2(std::vector<uint8_t> &ret_hash, std::vector<uint8_t> &ret_salt, 
                           const char *in_passwd, std::vector<uint8_t> *in_salt)
   
   //then call write bytes and write to passwd file

   std::cout << "Press Enter Key to Exit addUser()\n\n";
   getchar();
}


   // Password file should be in the format username\n{32 byte hash}{16 byte salt}\n
   // Using https://antelle.net/argon2-browser/ because I have not writing my code yet
   // Generated username/pwd with the following
   //  username = bob, password = bob, salt = thisismysaltof16
   //  memory = 1024 Kib, Iterations = 1, Hash length = 32, parallelism = 1, type - Argon2d
   /********  RESULTS ARE ******
    * [00.001] Testing Argon2 using Binaryen native-wasm
    * [00.001] Memory: 512 pages (32768 KB)
    * [00.004] Loading wasm...
    * [00.285] Wasm loaded, loading script...
    * [00.476] Script loaded in 191ms
    * [00.476] Calculating hash...
    * [00.486] Running...
    * [00.492] Params: pass=bob, salt=thisismysaltof16, time=1, mem=1024, hashLen=32, parallelism=1, type=0
    * [00.503] Encoded: $argon2d$v=19$m=1024,t=1,p=1$dGhpc2lzbXlzYWx0b2YxNg$mstW02XGT3/YAYvAhNN/TLxw4ZIhyNNk+yBndXVmdSI
    * [00.503] Hash: 9acb56d365c64f7fd8018bc084d37f4cbc70e19221c8d364fb20677575667522
    * [00.503] Elapsed: 10ms
   ************ END RESULTS FROM WEBSITE ********/

