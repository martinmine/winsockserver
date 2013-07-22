#if !defined(__CONNECTIONLISTENERFACTORY_H)
#define  __CONNECTIONLISTENERFACTORY_H

#include "IConnectionListener.h"
#include "SessionManager.h"

class ConnectionListenerFactory
{
public:
	// Creates a new connection manager with the given parameters
	static IClientConnectionListener* CreateConnectionListener(
		const int maxConnections, const unsigned short port)
	{
		return new ConnectionManager(maxConnections, port);
	}
};
#endif