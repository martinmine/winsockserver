#include <iostream>
#include "IConnection.h"
#include "IConnectionEvenentListener.h"
#include "IDataReceiver.h"
#include "ConnectionListenerFactory.h"
// Note, ONLY the Interface headers + factory is included to make the 
// implementation work
using namespace std;
using namespace NativeWinsock;

// Class which holds data associated with the connection
class SmallConnection : public IDataReceiver
{
private:
	IConnection * conn;
public:
	SmallConnection(IConnection* c) : conn(c)
	{ }

	// Called when a connection when data is received
	void receivedData(const char* data, const int len)
	{
		conn->sendData(data, len);
	}
};

// Class which manages connections created
class CustomConnectionMgr : public IConnectionEventListener
{
public:
	void connectionAccepted(IConnection* conn)
	{
		IDataReceiver* newConn = new SmallConnection(conn);
		conn->setReceiver(newConn);
		// NOTE: Class should contain a map set which holds refferances
		//       to the IDataReceiver pointer untill it is disconnected
		cout << "Connection " << conn->getConnectionID() << " received\n";
	}

	void connectionDisconnected(IConnection* connection)
	{
		cout << "Connection " << connection->getConnectionID() << " closed\n";
		// Note: Do not call delete on connection, as the connection object
		//       is re-used later on
	}
};

int main()
{
	IConnectionListener* listener = 
		ConnectionListenerFactory::CreateConnectionListener(20, 9494);

	listener->registerListener(new CustomConnectionMgr());
	listener->startListening();

	cout << "Ready\n";
	char k;
	while (true)
		cin >> k;
	return 0;
}