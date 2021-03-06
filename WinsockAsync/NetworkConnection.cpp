#include "NetworkConnection.h"
#include "IStatusListener.h"

#pragma region Constructor / Deconstructor
NetworkConnection::NetworkConnection(
	IStatusListener<NetworkConnection>* statusListenerPtr)
	:	statusListener(statusListenerPtr),
		closed(true),
		sendError(false),
		isFree(false),
		bufferInfo(),
		receiveOverlapped(),
		flags(0),	
		bytesReceived(0),
		sendOverlapped(),
		bytesSent(0),
		sendInfo(),
		newWaitHandle(0)
{
	bufferInfo.buf = receiveBuffer;	// Prepare the buffer!
	bufferInfo.len = 1024;
	receiveOverlapped.hEvent = WSACreateEvent();
}


NetworkConnection::~NetworkConnection()
{
	CloseHandle(newWaitHandle);
}
#pragma endregion

#pragma region Return values
#pragma endregion

#pragma region Functions
	
void NetworkConnection::sendData(const char* data, const int length)
{
	if (!closed && !sendError)	// Ignore call if something is wrong
	{
		// To-do: Make mutex

		// Set send information 
		sendInfo.buf = (char*)data;
		sendInfo.len = length;
			// Make winsock start sending
		if (WSASend(connection, &sendInfo, 1, 
			&bytesSent, 0, &sendOverlapped, NULL) 
				== SOCKET_ERROR)
		{			// If the error returned by the winsock was not a 
					// IO pending error
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				this->sendError = true;	// Set error flag
				// The send error bool makes sure that the close function 
				// doesn't get called before data processing/the current 
				// thread is done, as closing the socket and terminating 
				// the socket could possibly lead to a handfull of 
				// nullreffies during packet processing which could lead 
				// to a lot of null reffernce exceptions.
			}
		}
	}
}

void NetworkConnection::disconnect()
{
	statusListener->onDisconnect(this);	// Notify that our state has changed
}

void NetworkConnection::setReceiver(IDataReceiver* receiver)
{
	dataReceiver = receiver;
}

int NetworkConnection::getIP()
{
	if (closed)
		return 0;
	return address.sin_addr.S_un.S_addr;
}

#pragma endregion

#pragma region Functions

void NetworkConnection::setFree()
{
	close();
	isFree = true;
}

void NetworkConnection::setTaken(SOCKET conn)
{
	connection = conn;
	setTaken();
}

void NetworkConnection::setTaken()
{
	isFree = sendError = closed = false;
}

void NetworkConnection::close()
{
	if (!closed)
	{
		closed = true;
		shutdown(connection, SD_BOTH);	// Stop any send/receive operation
		closesocket(this->connection);	// Close the connection

		statusListener->onDisconnect(this);	// Call the manager to clean up
	}
}

void NetworkConnection::continueReceive()
{
	if (closed || sendError)	// Close if anything is wrong
	{
		close();
	}
	else
	{
		bufferInfo.buf = (char*)receiveBuffer;	// Set buffer data
		bufferInfo.len = BUFFER_SIZE;
	
										// 0 means send completed instantly
		if (WSARecv(connection, &bufferInfo, 1, &bytesReceived, 
			&flags, &receiveOverlapped, NULL) == 0) 
		{
			dataReceived();	// Wait for more data
		}
		else
		{
			if (WSAGetLastError() == WSA_IO_PENDING) // Data is pending
			{		// Send a wait handle to the WinAPI thread pool
				RegisterWaitForSingleObject(&newWaitHandle, 
					receiveOverlapped.hEvent, 
					DataReceivedCallback, 
					this, INFINITE, WT_EXECUTEONLYONCE);
			}
			else
			{
				close();
			}
		}
	}
}

void NetworkConnection::dataReceived()
{
	if (!closed)	// Ignore call if connection is closed
	{	
		BOOL operationSucceeded = 	// Call receive 
			WSAGetOverlappedResult(connection, &receiveOverlapped, 
										&bytesReceived, FALSE, &flags);
		// If anything went wrong, didnt receive anything or 
		// the send error flag was set
		if (!operationSucceeded || bytesReceived < 1 || sendError)
		{		// If not blocking error
			if (WSAGetLastError() != WSA_IO_INCOMPLETE)	
				close();	// Close connection
			else			// If not, continue waiting
				continueReceive();	
		}
		else
		{		// If receiver object associated, let them know data is 
			if (dataReceiver != nullptr)	// received
				dataReceiver->receivedData(receiveBuffer, 
											(int)bytesReceived);

			continueReceive();	// wait for more data
		}
	}
}

void NetworkConnection::beginReceive()
{
	continueReceive();
}
#pragma endregion

void CALLBACK DataReceivedCallback(PVOID param, BOOLEAN timeout)
{
	(static_cast<NetworkConnection*>(param))->dataReceived();
}
