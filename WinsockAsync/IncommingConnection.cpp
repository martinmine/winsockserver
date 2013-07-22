#include "IncommingConnection.h"
#include <WinSock2.h>

bool IncommingConnection::acceptConnection(SOCKET sock)
{
	int size = sizeof(getAddress());	// Set size, and try accept
	SOCKET connection = accept(sock, (SOCKADDR*)&getAddress(), &size);

	if (connection == INVALID_SOCKET)	// If failed accepting
		return false;

	setTaken(connection);
	return true;
}

bool IncommingConnection::free() const
{
	return isFree;
}

bool IncommingConnection::valid() const
{
	return isValid;
}

void IncommingConnection::setFree()
{
	close();
	isFree = true;
}

