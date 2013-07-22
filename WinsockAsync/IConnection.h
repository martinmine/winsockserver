#if !defined(__IConnection_H)
#define  __IConnection_H

#include "IDataReceiver.h"

// Interface restricting which methods the client should have aceess to
// into a connection object, unknown if is outgoing or ingoing connection
class IConnection
{
public:
	// Sends data to the connection
	virtual void sendData(const char* data, const int length) = 0;
	// Sets the receiver object which is called when data is received
	virtual void setReceiver(IDataReceiver* receiver) = 0;
	// Disconnects the connections
	virtual void disconnect() = 0;
	// Returns the IP as a 32-bit integer
	virtual int  getIP() = 0;
	// Returns the ID of the connection
	virtual ~IConnection() { }
};

#endif