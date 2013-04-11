#if !defined(__ICONNECTIONLISTENER_H)
#define  __ICONNECTIONLISTENER_H

#include "IConnectionEvenentListener.h"

namespace NativeWinsock
{
	// Interface which gives the client access only to those functions that
	// should be available in the session manager class
	class IConnectionListener
	{
	public:
		// Starts listening for connections
		virtual void startListening() = 0;
		// Registers an event listener on the listener
		virtual void registerListener(IConnectionEventListener* listener) = 0;
	};
}

#endif