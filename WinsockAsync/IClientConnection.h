#if !defined(__IClientConnection_H)
#define  __IClientConnection_H

#include "IDataReceiver.h"
#include "IConnection.h"

// Interface restricting which methods the client should have aceess to
// into a connection object when the connection is made towards a client
class IClientConnection : public IConnection
{
public:
	virtual void beginConnect();
	virtual ~IClientConnection() { }
};


#endif