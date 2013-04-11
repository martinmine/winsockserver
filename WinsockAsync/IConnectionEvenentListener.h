#if !defined(__ICONNECTIONEVENTLISTENER_H)
#define  __ICONNECTIONEVENTLISTENER_H
#include "IConnection.h"

namespace NativeWinsock
{
	// Interface restricting which functions the manager should be able to call
	// on a listening object when a connection is established
	class IConnectionEventListener
	{
	public:
		// Function called when a connection is accepted from the listener
		virtual void connectionAccepted(IConnection* conn) = 0;
		// Function called when a connection is disconnected
		virtual void connectionDisconnected(IConnection* connection) = 0;
		// Declare deconstructor as virtual so derived destructors will be
		// called when delete is called on an IConnectionReceiver
		virtual ~IConnectionEventListener() { }
	};
}
#endif