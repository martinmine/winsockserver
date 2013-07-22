#include <iostream>
#include "IClientConnection.h"
#include "IConnectionEvenentListener.h"
#include "IDataReceiver.h"
#include "ConnectionListenerFactory.h"
#include "IncommingConnection.h"
#include "OutgoingConnection.h"
#include "IStatusListener.h"
// Note, ONLY the Interface headers + factory is included to make the 
// implementation work
using namespace std;

// Class which holds data associated with the connection
class SmallConnection : public IDataReceiver
{
private:
	IncommingConnection * conn;
public:
	SmallConnection(IncommingConnection* c) : conn(c)
	{ }

	// Called when a connection when data is received
	void receivedData(const char* data, const int len)
	{
		conn->sendData(data, len);
	}
};

// Class which manages connections created
class CustomConnectionMgr : public IClientConnectionEventListener
{
public:
	void connectionAccepted(IncommingConnection* conn)
	{
		IDataReceiver* newConn = new SmallConnection(conn);
		conn->setReceiver(newConn);
		// NOTE: Class should contain a map set which holds refferances
		//       to the IDataReceiver pointer untill it is disconnected
		//cout << "Connection " << conn->getConnectionID() << " received\n";
	}

	void connectionDisconnected(IncommingConnection* connection)
	{
		cout << "Connection " << connection->getConnectionID() << " closed\n";
		// Note: Do not call delete on connection, as the connection object
		//       is re-used later on
	}
};


class Session : public IConnectionConnector, 
			    public IStatusListener<NetworkConnection>
{
private:
	OutgoingConnection conn;

public:
	// Construct the OutgoingConnection object
	Session() : conn(this, inet_addr("127.0.0.1"), 9494) 
	{ 
		conn.beginConnect(this);	
	}

	// Function called when disconnected
	void onDisconnect(NetworkConnection* connection)
	{
		cout << "Disconnected\n";
	}

	// Function called when connected
	void connected(OutgoingConnection* conn)
	{
		cout << "It works! (Hopefully)\n";
	}
};

int main()
{
	IClientConnectionListener* listener = 
		ConnectionListenerFactory::CreateConnectionListener(20, 9494);

	listener->registerListener(new CustomConnectionMgr());
	listener->startListening();

	//cout << "Ready\n";

	int w;
	//cin >> w;

	Session* sess = new Session();

	char k;
	while (true)
		cin >> k;
	return 0;
}

