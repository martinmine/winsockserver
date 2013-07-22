#if !defined(__IClientConnectionEVENTLISTENER_H)
#define  __IClientConnectionEVENTLISTENER_H
#include "IncommingConnection.h"

// Interface restricting which functions the manager should be able to call
// on a listening object when a connection is established
class IClientConnectionEventListener
{
public:
	// Function called when a connection is accepted from the listener
	virtual void connectionAccepted(IncommingConnection* conn) = 0;
	// Function called when a connection is disconnected
	virtual void connectionDisconnected(IncommingConnection* connection) = 0;
	// Declare deconstructor as virtual so derived destructors will be
	// called when delete is called on an IClientConnectionReceiver
	virtual ~IClientConnectionEventListener() { }
};
#endif