#if !defined (__OUGOINGCONNECTION_H)
#define __OUGOINGCONNECTION_H

#include "NetworkConnection.h"
#include "IStatusListener.h"
#include "IConnectionConnector.h"

class OutgoingConnection : public NetworkConnection
{
private:
	HANDLE waitHandle;
	IConnectionConnector* listener;

public:
	OutgoingConnection(IStatusListener<NetworkConnection>* listener,
		const unsigned int addr,
		const unsigned short prt) 
		: NetworkConnection(listener) 
	{ 
		sockaddr_in endpoint;
		endpoint.sin_family = AF_INET;
		endpoint.sin_addr.s_addr = addr;
		endpoint.sin_port = htons(prt);

		setAddress(endpoint);
		setSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	};

	int beginConnect(IConnectionConnector* listener);
	void connectionResultReceived();
};

void CALLBACK ConnectionConnectedCallback(PVOID param, BOOLEAN timeout);
#endif