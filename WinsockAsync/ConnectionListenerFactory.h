#if !defined(__CONNECTIONLISTENERFACTORY_H)
#define  __CONNECTIONLISTENERFACTORY_H

#include "IConnectionListener.h"
#include "SessionManager.h"

namespace NativeWinsock
{
	class ConnectionListenerFactory
	{
	public:
		// Creates a new connection manager with the given parameters
		static IConnectionListener* CreateConnectionListener(
			const int maxConnections, const unsigned short port)
		{
			return new ConnectionManager(maxConnections, port);
		}
	};
}
#endif