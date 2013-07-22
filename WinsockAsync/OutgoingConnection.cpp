#include "OutgoingConnection.h"
#include <WinSock2.h>

int OutgoingConnection::beginConnect(IConnectionConnector* lst)
{
	listener = lst;

	SOCKET connection = getSocket();	// Get inherited member
	if (connection == INVALID_SOCKET)	// Check if is valid socket
		return 1;
	
	int code = WSAConnect(connection, (SOCKADDR*)&getAddress(), sizeof(getAddress()), getSenderBuffer(),
			getSenderBuffer(), NULL, NULL);

	if (code == SOCKET_ERROR)	// Requires waiting or didn't connect
	{
		int errorCode = WSAGetLastError();	// Get the error code 
		if (errorCode == WSA_IO_PENDING)	// If it is IO Pending, we must waut
		{
			waitHandle = WSACreateEvent();		// Make wait handle
			WSAEventSelect(connection, waitHandle, FD_CONNECT);	// Associate wait handle
			RegisterWaitForSingleObject(&waitHandle, // Give to thread pool WinAPI
				waitHandle, 
				ConnectionConnectedCallback, 
			this, INFINITE, WT_EXECUTEONLYONCE);
			
			return 0;
		}
		else
		{
			return errorCode;
		}
	}
	else	// Connected instantly
	{
		connectionResultReceived();	// Notify we are connected!
		return -1;
	}
}

void CALLBACK ConnectionConnectedCallback(PVOID param, BOOLEAN timeout)
{
	(static_cast<OutgoingConnection*>(param))->connectionResultReceived();
}

void OutgoingConnection::connectionResultReceived()
{
	CloseHandle(waitHandle);	// Close handle and notify that we are connected
	setTaken();					// Prepare resources
	listener->connected(this);	// Notify listener we are changed state
}