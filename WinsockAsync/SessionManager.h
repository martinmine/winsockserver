#if !defined(__SESSIONMANAGER_H)
#define  __SESSIONMANAGER_H

#include <WinSock2.h>
#include <list>
using namespace std;

#include "IConnectionListener.h"
#include "IConnectionEvenentListener.h"

// Callback function called by the WinAPI thread pool when a connection
// is accepted, calls ConnectionManager::connectionAccepted();
// PVOID param: User defined object, is the ConnectionManager which the 
//				wait callback was made from
// BOOLEAN timeout: Indicates whether or not the wait has timed out
void CALLBACK ConnectionAcceptCallback(PVOID param, BOOLEAN timeout);
	
// Initializes the winsock API to be used
void initializeWinsock();

// Version of our supported winsock version
const int WSOCK_VER = 0x0202;

class ConnectionManager : public IStatusListener<NetworkConnection>, 
							public IClientConnectionListener
{
private:
	// Integer saying how many connections that has been received
	unsigned int	connectionsAccepted;
	// The limit of how many active connections there can be
	unsigned int	maxConnections;
	// Indicates the state of the listener object, if it is active or not
	bool			isListening;

	// Wait handle used by the threadpool API
	HANDLE			waitHandle;
	//The winsock socket object
	SOCKET			listener;
	// Listening wait handle 
	HANDLE			newWaitHandle;
	// Container which holds all free connectionx
	list<IncommingConnection*> freeConnections;

	// Returns a free connection which a connection is trying to accept
	IncommingConnection* getFreeConnection();

	// Event litsener which is called when a connection has been made
	IClientConnectionEventListener* eventListener;

	// Creates a wait handle and waits for the next connection
	void continueListen();

	// Prevents the default constructor from being executed
	ConnectionManager();
public:
	ConnectionManager(const int maxConnections, unsigned short port);
	~ConnectionManager();
	// Callback function when a connection is accepted
	void connectionAccepted();
	// Starts listening and accepting connection
	void startListening();
	// Closes a connection and makes it available for later use
	void closeConnection(IncommingConnection* sess);
	// Function called when a connection is disconnected
	void onDisconnect(NetworkConnection* connection);
	// Registers an event listener which calls client code when
	// a connection has been mades
	void registerListener(IClientConnectionEventListener* listener);
};

#endif