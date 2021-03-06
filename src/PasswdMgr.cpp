//Is the <argon2.h> in brackets correct
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
//LARKIN ADDITIONS
#include "argon2.h"

const int hashlen = 32;
const int saltlen = 16;

//The PasswdMgr constructor accepts a password file name and save it
// to the PasswdMgr classes private variable _pwd_file declared
// in PasswdMgr.h file
// IN SHORT: pass in a Const Char and make it a string
PasswdMgr::PasswdMgr(const char *pwd_file):_pwd_file(pwd_file) {
   //std::cout << "Enter PasswdMgr() Constructor and DO SOMETHING\n";
   //std::cout << " _pwd_file variable is: " << _pwd_file << "\n";
   //std::cout << "pwd_file argument is: " << pwd_file << "\n";
 
   FILE *fptr;
   
   if ((fptr = fopen(_pwd_file.c_str(),"r")) != NULL){
            //printf("Opened passwd file\n");
   } else {
      fptr = fopen(_pwd_file.c_str(),"w");
      //printf("TO PROGRAM: Created empty passwd file in current working directory\n"
      //   "and added a user bob with password bob");
      addUser("bob","bob");
   }

   
   if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }
   fclose(fptr);// Close the fptr object for future use
   //}  

   //std::cout << "Press Enter Key to Exit PasswdMgr() Constructor\n\n";
   //getchar();
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
   //std::cout << "Enter checkUser() and DO SOMETHING\n";
   //std::cout << "name passed inside of checkUser is: " << name << std::endl;

   
   //bool result = false; 
   std::vector<uint8_t> passwd, salt; //just contain garbage right now
   
   /************************
   //PROGRAM THIS AFTER WE ADD A USER INTO OUR FILE SUCCESSFULLY
   *************************/

   
   if(findUser(name, passwd, salt) == 1) {
      //std::cout << "Found user, return true from findUser()\n";
      //result = true;

      //WHAT HAPPENS IF I WRITE TO FILE WHEN I FOUND THE USER.. THAT WILL OVERWRITE
      //HOW DO I GET THE 'POSITION' READ IN THE FILE

      return true;
   }

   //std::cout << "Press Enter Key to Exit checkUser()\n\n";
   //getchar();    
   
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
   //std::cout << "Enter checkPasswd() and DO SOMETHING\n"; 
   
   //NOT DECLARING THE SIZES OF THE USERHASH AND PASSHASH
   //SCREWED ME
   
   std::vector<uint8_t> userhash(32); // hash from the password file
   std::vector<uint8_t> passhash(32); // hash derived from the parameter passwd
   std::vector<uint8_t> salt(16);

   
   FileFD pwfile(_pwd_file.c_str());
   pwfile.openFile(FileFD::readfd);

   std::string stringstripnewline (passwd);
   clrNewlines(stringstripnewline);
   passwd = stringstripnewline.c_str();

/***
   std::cout << "PRINT VARIABLES BEFORE FINDUSER IN CHECKPASSWD: ";
   std::cout << "const char *password is: " << passwd;
   std::cout << "strlen(passwd) is: " << strlen(passwd);
   for (int i = 0; i < 16; i++){ std::cout << salt[i] << "."; }
   std::cout << std::endl;
   for (int i = 0; i < 32; i++){ std::cout << userhash[i] << "."; }
   std::cout << std::endl;
   for (int i = 0; i < 32; i++){ std::cout << passhash[i] << "."; }
   std::cout << std::endl;
***/
   // Check if the user exists and return the users hash and salt
   if (!findUser(name, userhash, salt))
      return false;
/***
   std::cout << "PRINT VARIABLES AFTER FINDUSER IN CHECKPASSWD: ";
   for (int i = 0; i < 16; i++){ std::cout << salt[i] << "."; }
   std::cout << std::endl;
   for (int i = 0; i < 32; i++){ std::cout << userhash[i] << "."; }
   std::cout << std::endl;
   for (int i = 0; i < 32; i++){ std::cout << passhash[i] << "."; }
   std::cout << std::endl;
***/
   std::string nametostring (name);
   //readuser(pwfile, &name, &hash, &salt)
   readUser(pwfile, nametostring, passhash, salt);
/***
   std::cout << "PRINT VARIABLES AFTER READUSER IN CHECKPASSWD: ";
   for (int i = 0; i < 16; i++){ std::cout << salt[i] << "."; }
   std::cout << std::endl;
   for (int i = 0; i < 32; i++){ std::cout << userhash[i] << "."; }
   std::cout << std::endl;
   for (int i = 0; i < 32; i++){ std::cout << passhash[i] << "."; }
   std::cout << std::endl;
***/
   //remember (&rethash, &retsalt, *inpasswd, *insalt)
   //hashArgon2(passhash, salt, passwd, &salt);
   hashArgon2(passhash, salt, passwd, NULL);
   
/***
   std::cout << "PRINT VARIABLES AFTER HASHARGON2 IN CHECKPASSWD: ";
   for (int i = 0; i < 16; i++){ std::cout << salt[i] << "."; }
   std::cout << std::endl;
   for (int i = 0; i < 32; i++){ std::cout << userhash[i] << "."; }
   std::cout << std::endl;
   for (int i = 0; i < 32; i++){ std::cout << passhash[i] << "."; }
   std::cout << std::endl;
   ***/
   
   
   if (userhash == passhash){
      //std::cout << "userhash is equal to passhash";
      return true;
   } else {
      return false;
   }
      
   
   
   //std::cout << std::endl;

   //std::cout << "Press Enter Key to Exit checkPasswd()\n\n";
   //getchar();

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
   
   
   //std::cout << "Press Enter Key to Exit changePasswd()\n\n";
   //getchar();
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
   //std::cout << "Enter readUser() and DO SOMETHING\n"; 
   //std::cout << "pwfile in readUser() is: " << pwfile << "\n"; 
   clrNewlines(name);
   //std::cout << "name in readUser() is: " << name << " may or may not be blank\n"; 
    
   
   
   //STEP 0: Knowing this function gets called in a loop, we need to return one SET of uname, salt, hash
   //STEP 1: Read in one username, salt, and password
   //pwfile.openFile(FileFD::readfd);
   // CHECK FOR END OF FILE, readStr() returns zero if no bytes read 

   int bytesread = 0;
   bytesread = pwfile.readStr(name);
   if (bytesread == 0 ) {
      //std::cout << "print this end of file" << std::endl;
      return false; //we've reached the end of the passwd file
   }
   //std::cout << "username read from passwd file is: " << name << std::endl; //Does readStr() get the newline?
   
   
   //READ THE SALT, STORE IN Salt Memory location passed to this function
   bytesread = pwfile.readBytes<uint8_t>(salt, 16);

   //PRINT  THE SALT TO SCREEN   
   for (unsigned int i=0; i<salt.size(); i++) {
      //std::cout << "," << (int) salt[i];
   }
   //std::cout << "\n";
   //bytesread = pwfile.readBytes<uint8_t>(salt);
   //std::cout << "bytesread var for salt is: " << bytesread << std::endl;
   
   
   //READ THE PASSWORD HASH AND PRINT TO SCREEN
   bytesread = pwfile.readBytes<uint8_t>(hash, 32);
   for (unsigned int i=0; i<hash.size(); i++) {
      //std::cout << "," << (int) hash[i];
   }
   //std::cout << "bytes read var for hash is: " << bytesread << std::endl;
  // std::cout << "\n";

   uint8_t togetnewline;
   pwfile.readByte(togetnewline);
   // I NEED if(!pwfile.readbytes<uint8_t>(hash, 32) < 0)
   //print out what was passed
   
   //std::cout << "Press Enter Key to Exit readUser()";
   //getchar();
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

   std::cout << "did my std::string newname (name) thing work?\n &name is:" << name << "\n";
   // Insert your wild code here!
   //ssize_t FileDesc::writeByte(unsigned char data) {return write(_fd, &data, 1);}

   //ssize_t FileDesc::writeFD(const char *data) {
   //pwfile.writeFD(name);
   //pwfile.writeBytes();

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
   //std::cout << "Enter findUser() and DO SOMETHING\n"; 
   
   //convert const char *name to string
   std::string makeastring (name);
   clrNewlines(makeastring); 
   //std::cout << "name passed inside of findUser is: " << name;
   //std::cout << "name is now makeastring and is: " << makeastring;
   

   //bool results = false; //initialize to false... we have an unknown user


   //print out args passed to function
   //std::cout << "name: " << name << std::endl;
   //std::cout << "hash: " << hash << std::endl;
   //std::cout << "salt: " << salt << std::endl;

   //REMEMBER: findUser will get called first and this FileFD will get created... but as a local variable
   FileFD pwfile(_pwd_file.c_str());
   //FILE *bobspwfileptr;
   //bobspwfileptr = fopen(_pwd_file.c_str(),"r");
   //fpos_t mypositioninfile;

   // You may need to change this code for your specific implementation
   if (!pwfile.openFile(FileFD::readfd))
      throw pwfile_error("Could not open passwd file for reading");

   //open the password file and loop over all users
   //pwfile.openFile(FileFD::readfd);
   bool eof = false;
   while (!eof) {
      std::string uname; //create an emptry string for readuser to return
      // a result to
      
      //if (readUser(pwfile, uname, hash, salt)==true){
         // A user, salt, password has been returned and weve NOT reached end of file
      //   std::cout << "name is :" << name << " and uname is: " << uname << "\n";
      //std::cout << "when does this execute alpha";
      if (!readUser(pwfile, uname, hash, salt)) {
         //std::cout << "\nwhen does this execute bravo\n";
         eof = true;
         continue;
      }
      //std::cout << "TROUBLESHOOTING: name is: " << name << "and  makeastring is: " << makeastring << " and uname is: " << uname;
      
      if (!uname.compare(makeastring)) {
         //std::cout << "Executed the Compare Function, found match" << std::endl;
         //results = true;
         
// I THINK I CARE ABOUT RIGHT HERE
//int getfileposition = fgetpos(bobspwfileptr, &mypositioninfile); //returns zero on success
//std::cout << "getfileposition is: " << getfileposition << " and mypositioninfile is: " << &mypositioninfile;          

         pwfile.closeFD();
         return true;
         //break; // if we found a user, break out of the while loop so that
         // the username, salt, and hash are in the current variables/registers
         //return true;
      }
       //std::cout << "when does this execute charlie";
   }

   //why am I clearing all of these... don't I want to save them for passwd checking???
   //also, if craeting a new user... these are going to get overwritten anyhow...
   hash.clear();
   salt.clear();
   pwfile.closeFD();
   
   //std::cout << "Press Enter Key to Exit findUser()\n\n";
   //getchar();
   return false;
   //return results;
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
   //std::cout << "Enter hashArgon2() and DO SOMETHING\n"; 

   //what do i have in the funciton and how do i access it
      //std::cout << ret_hash.at(0) << "..." << ret_hash.at(31) << "\n";
      //std::cout << ret_salt.at(0) << "..." << ret_salt.at(15) << "\n";
      //std::cout << "inpassword was: " << in_passwd << "with strlen of: " << strlen(in_passwd);
   // NOPE CANT DO THIS... std::cout << "Size of in_passwd is: " << sizeof(in_passwd) << "\n";
   //std::cout << "strlen() of in_password is: " << strlen(in_passwd) << "\n";
   //std::cout << "sizeof(in_passwd)/sizeof(*in_passwd): " << (sizeof(in_passwd)/sizeof(*in_passwd)) << "\n";
   //std::cout << in_salt << "\n";   // this was null for initial add user
   
   // the argon2 wants a uint8_t array and NOT a vector
   //uint8_t uintpasswd[strlen(in_passwd)]; 
   //std::cout << "Priting out uintpassword: ";
   //for (int i = 0; i < strlen(in_passwd); i++){
   //   uintpasswd[i] = in_passwd[i];
   //   std::cout << uintpasswd[i] << ".";
   //}
   //std::cout << std::endl;
   //std::cout << "sizeof(uintpasswd) is: " << sizeof(uintpasswd) << std::endl;
   //std::cout << "strlen(uintpasswd) is: " << strlen(uintpasswd);
   
   uint8_t uinthash[32]; 
   //std::cout << "Priting out uinthash: ";
   //for (int i = 0; i < 32; i++){
   //   uinthash[i] = ret_hash[i];
   //   std::cout << uinthash[i] << ".";
   //}
   //std::cout << std::endl;

   uint8_t uintsalt[16]; 
   //std::cout << "Priting out uintsalt: ";
   for (int i = 0; i < 16; i++){
      uintsalt[i] = ret_salt[i];
   //   std::cout << uintsalt[i] << ".";
   }
   //std::cout << std::endl;
   
   //ARGON PARAMTERS PASSED TO argon2i_hash_raw() 
   //argon2i_hash_raw(1, (1<<10), 1, uintpasswd, sizeof(uintpasswd), &ret_salt, 16, &ret_hash, 32);
   //IN THIS FUNCTION... returnhash is blank with a size of 1000
   

   //THIS PROCESSES PROCESS MODIFIES uinthash which can be copied back over into ret_hash
   argon2i_hash_raw(1, (1<<10), 1, (uint8_t *)in_passwd, strlen(in_passwd), uintsalt, 16, uinthash, 32);
   
   //FIRST TRY AND DID NOT WORK RIGHT
   //argon2i_hash_raw(1, (1<<10), 1, uintpasswd, strlen(in_passwd), &ret_salt, ret_salt.size(), &ret_hash, ret_hash.size());
   // when the argon2 returns the array, loop over and put back into a vector

   //WAHT DO I HAVE AFTER ARGON2 FUNCTION
//   std::cout << "after argon2\n";
//   std::cout << ret_hash.at(0) << "..." << ret_hash.at(31) << "\n";
//   std::cout << ret_salt.at(0) << "..." << ret_salt.at(15) << "\n";
   
   //std::cout << "Priting out uintpasswd: ";
   //for (int i = 0; i < strlen(in_passwd); i++){
      //uintpasswd[i] = in_passwd[i];
   //   std::cout << uintpasswd[i] << ".";
   //}
   //std::cout << std::endl;
   
 //  std::cout << "Copying uinthash into ret_hash: ";
   for (int i = 0; i < 32; i++){
      ret_hash[i] = uinthash[i];
 //     std::cout << ret_hash[i] << ".";
   }
 //  std::cout << std::endl;

   //std::cout << "Priting out uintsalt: ";
   //for (int i = 0; i < 16; i++){
      //uintsalt[i] = ret_salt[i];
   //   std::cout << uintsalt[i] << ".";
   //}
   //std::cout << std::endl;   
   //std::cout << in_passwd << "\n";
   //std::cout << in_salt << "\n";   // this was null for initial add user
   

   //std::cout << "Press Enter Key to Exit hashArgon2()\n\n";
   //getchar();
}

/****************************************************************************************************
 * addUser - First, confirms the user doesn't exist. If not found, then adds the new user with a new
 *           password and salt
 *
 *    Throws: pwfile_error if issues editing the password file
 ****************************************************************************************************/

void PasswdMgr::addUser(const char *name, const char *passwd) {
   // Add those users!
   //std::cout << "Enter addUser() and DO SOMETHING\n"; 
   
   //PRINT WHAT WE HAVE TO THE SCREEN
   //std::cout << "user name is: " << name << "\n";
   //std::cout << "passwd is: " << passwd << "\n";

   //create the uint8_t vectors to pass to the hashArgon2() function
   // YES a vecotr needs to declare an initial size
   std::vector<uint8_t> bobhash(32);
   std::vector<uint8_t> bobsalt(16);

   //std::cout << bobhash.size();
   
   for (int i = 0; i<32; i++){
      bobhash[i] = (rand() % 93) + 33; // This will get overwritten anyway
   //   std::cout << bobhash[i] << ".";
      //bobhash[10] = '\n'; // append newline works
      //std::cout << i << ":" << bobhash.at(i) << "\n";
   } 
   //std::cout << std::endl;

   //std::cout << "SEG FAULT YET...\n";

   std::string mysalt ("thisismysaltof16");
   for (int i = 0; i<16; i++){
      bobsalt.at(i) = mysalt[i];
   //   std::cout << mysalt[i] << ".";
      //std::cout << i << ".";
      //std::cout << bobsalt[i];
   }
   //std::cout << std::endl;

   //std::cout << "call(ing) the hashArgon2() function\n";
   //i can always pass in a null when a fuction expects a pointer
   //REMEMBER: You cannot get the sizeof a pointer (its a memmory address)
   hashArgon2( bobhash, bobsalt, passwd, NULL);
   
   //bobhash has been updated from the argon2 function
   //call writeUser and write username, bobhash, bobsalt to passwd file
   //PasswdMgr::writeUser(FileFD &pwfile, std::string &name, std::vector<uint8_t> &hash, std::vector<uint8_t> &salt)

      //REMEMBER: findUser will get called first and this FileFD will get created... but as a local variable
   FileFD pwfile(_pwd_file.c_str());

   // You may need to change this code for your specific implementation
   if (!pwfile.openFile(FileFD::writefd))
      throw pwfile_error("Could not open passwd file for writing");

   //std::cout << "NO error means pwfile file descriptor was opened successfully???";
   //PASSWD FILE FORMAT IS 
   // username\n
   // saltpasswd\n
   
   
   std::string newname (name); //WHY NOT JUST ACCEPT A CONST CHAR IN writeUser()...
   pwfile.openFile(FileFD::appendfd); //open file for appending
   
   //WRITE THE USERNAME
   pwfile.writeFD(newname);
   pwfile.writeFD("\n"); // append newline to end of username
   
   //WRITE THE SALT
   int byteswritten = 0;
   for (unsigned int i=0; i<bobsalt.size(); i++) {
   //   std::cout << "," << (int) bobsalt[i];
   }
   //std::cout << "\n";
   byteswritten = pwfile.writeBytes<uint8_t>(bobsalt);
   //std::cout << "byteswriten var for salt is: " << byteswritten << std::endl;
   
   //WRITE THE PASSWORD HASH
   byteswritten = pwfile.writeBytes<uint8_t>(bobhash);
   pwfile.writeFD("\n"); // append newline after salt and passwd hash
   for (unsigned int i=0; i<bobhash.size(); i++) {
   //   std::cout << "," << (int) bobhash[i];
   }
   //std::cout << "byteswriten var for hash is: " << byteswritten << std::endl;
   //std::cout << "\n";
   
   //close the file      
   pwfile.closeFD();
   
   //TODO:PROGRAM THIS LATER
   //writeUser(pwfile, newname, bobhash, bobsalt);

   //std::cout << "Press Enter Key to Exit addUser()\n\n";
   //getchar();
}

/* ITS 0244 AND I MADE AN HONEST EFFORT AT THIS. MY THOUGHT WAS TO:
* have user enter existing password and check it
* have user enter new password once
* have user enter new password second time
* check that the password are equal
* create a new password file
* copy in all the users over EXCEPT the current user
* add a 'new' user with the users new passwords
* delete the original password file
* rename the newpassword file to passwd

void PasswdMgr::deleteUser(const char *name){
//bool PasswdMgr::findUser(const char *name, std::vector<uint8_t> &hash, std::vector<uint8_t> &salt)
   std::vector<uint8_t> hash(32);
   std::vector<uint8_t> salt(16);
   std::string makeastring (name);
   clrNewlines(makeastring); 
   FileFD pwfile("passwd");
   FileFD newpwfile("newpasswdfile");
   newpwfile.openFile(FileFD::appendfd);
   
   //FILE *fptr;
   //fptr = fopen(_pwd_file.c_str(),"w");
      //printf("TO PROGRAM: Created empty passwd file in current working directory\n"
      //   "and added a user bob with password bob");
   //   addUser("bob","bob");

   if (!newpwfile.openFile(FileFD::readfd))
      throw pwfile_error("Could not open newpasswd file for reading");

   if (!pwfile.openFile(FileFD::readfd))
      throw pwfile_error("Could not open passwd file for reading");

   bool eof = false;
   while (!eof) {
      std::string uname; //create an emptry string for readuser to return
      if (!readUser(pwfile, uname, hash, salt)) {
         
         eof = true;
         continue;
      }
      std::cout << "TROUBLESHOOTING: name is: " << name << "and  makeastring is: " << makeastring << " and uname is: " << uname << "\n";
      writeUser(newpwfile, uname, hash, salt);
      if (!uname.compare(makeastring)) {
         std::cout << "Executed the Compare Function, found match" << std::endl;
         //pwfile.closeFD();
         //return true;
      }
    
   }
   hash.clear();
   salt.clear();
   pwfile.closeFD();
   //return false;
      


}*/


