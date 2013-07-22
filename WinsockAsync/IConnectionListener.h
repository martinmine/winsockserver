#if !defined(__IClientConnectionLISTENER_H)
#define  __IClientConnectionLISTENER_H

#include "IConnectionEvenentListener.h"

// Interface which gives the client access only to those functions that
// should be available in the session manager class
class IClientConnectionListener
{
public:
	// Starts listening for connections
	virtual void startListening() = 0;
	// Registers an event listener on the listener
	virtual void registerListener(IClientConnectionEventListener* listener) = 0;
};

#endif