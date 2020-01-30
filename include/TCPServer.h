#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <list>
#include <memory>
#include "Server.h"
#include "FileDesc.h"
#include "TCPConn.h"

class TCPServer : public Server 
{
public:
   TCPServer();
   TCPServer(const char *log_file);
   ~TCPServer();

   void bindSvr(const char *ip_addr, unsigned short port);
   void listenSvr();
   void shutdown();

   int writeToLogFile(FileFD &logfile, std::string &logmsg); 

private:
   // Class to manage the server socket
   SocketFD _sockfd;
 
   // List of TCPConn objects to manage connections
   std::list<std::unique_ptr<TCPConn>> _connlist;

   std::string _log_file;
   std::string _logmsg;

};


#endif
