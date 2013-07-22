#if !defined(__ICONNECTIONCONNECTOR_H)
#define __ICONNECTIONCONNECTOR_H

//#include "OutgoingConnection.h"

// Interface encapsulating the function called from the outgoing 
// connection once a connection has been established

class OutgoingConnection;
class IConnectionConnector
{
public:
	// Function called once a connection has connected/changed state
	virtual void connected(OutgoingConnection* conn) = 0;
};

#endif 