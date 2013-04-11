#include "SessionManager.h"
#include <iostream>
using namespace std;

namespace NativeWinsock
{
	ConnectionManager::ConnectionManager(const int maxConn, 
										unsigned short port)
										: maxConnections(maxConn),
										  isListening(false),
										  connectionsAccepted(0),
										  waitHandle(0)
	{
		int errorCode;				
		sockaddr_in acceptAddress;

		initializeWinsock();		// Create winsock

		listener = WSASocket(AF_INET, SOCK_STREAM, 0,	// Create listener 
							NULL, 0, WSA_FLAG_OVERLAPPED);
										// Create the accept address
		acceptAddress.sin_family		= AF_INET;		
		acceptAddress.sin_addr.s_addr	= htonl(INADDR_ANY);
		acceptAddress.sin_port			= htons(port);	// Bind bort

		errorCode = bind(this->listener,				// bind
						(SOCKADDR*)&acceptAddress, sizeof(acceptAddress));
		
		if (errorCode)
			throw new exception("Something is already listening on this port");
	}

	ConnectionManager::~ConnectionManager()
	{
		CloseHandle(waitHandle);		// Close the handles
		CloseHandle(newWaitHandle);
	}

	void ConnectionManager::connectionAccepted()
	{
		bool wouldNotBlock;	// If a blocking operation wants to run

		do
		{									// Try and get a free connection
			Session* connection = getFreeConnection();
											// Try and accept the connection
			wouldNotBlock = connection->acceptConnection(listener); 

			if (wouldNotBlock)				// Ignore if pending IO
			{
				if (connection->valid())	// If a connection was received
				{
					eventListener->connectionAccepted(	// Cast it
						dynamic_cast<IConnection*>(connection));
					connection->beginReceive();	// Start receive
				}
				else
				{
					onDisconnect(connection);	// Destroy it
				}
			}
		}
		while (wouldNotBlock && isListening);	// While can accept anything
		continueListen();						// Go back to sleep
	}

	void ConnectionManager::continueListen()
	{
		if (!isListening)						// Ignore call if not listening
		{
			CloseHandle(waitHandle);			// Close current handle
			waitHandle = WSACreateEvent();		// Create new handle

			WSAEventSelect(listener, waitHandle, FD_ACCEPT);	// Wait for con

			RegisterWaitForSingleObject(&newWaitHandle, // Make thread pool
				waitHandle, 
				ConnectionAcceptCallback, 
				this, INFINITE, WT_EXECUTEONLYONCE);
		}
	}

	void ConnectionManager::startListening()
	{
		if (isListening)	// Throw exception if already listening
		{
			throw new exception("Connection is already listening");
		}
		else
		{
			listen(listener, 500);	// Listen and set backlog
			continueListen();		// Wait for connections
		}
	}

	Session* ConnectionManager::getFreeConnection()
	{
		Session* sess = nullptr;
		
		if (freeConnections.size())
		{
			sess = freeConnections.front();	// Take out front most element
			freeConnections.pop_front();
			//Todo: Concurrency issues
		}

		if (sess == nullptr)	// No free connections, make one
			sess = new Session(connectionsAccepted++, this);

		return sess;
	}

	void ConnectionManager::onDisconnect(Session* connection)
	{
		eventListener->connectionDisconnected(connection);	// Notify
		connection->setFree();					// Set it to be free
		freeConnections.push_front(connection);	// Add to 'free' connections
	}

	void ConnectionManager::registerListener(IConnectionEventListener* lstnr)
	{
		eventListener = lstnr;
	}

	void CALLBACK ConnectionAcceptCallback(PVOID param, BOOLEAN timeout)
	{
		(static_cast<ConnectionManager*>(param))->connectionAccepted();
	}

	void initializeWinsock()
	{
		static bool initialized = false;

		if (!initialized)	// If not initialized
		{
			WSADATA socketVersionData;
			int errorCode = WSAStartup(WSOCK_VER, &socketVersionData);
			
			if (errorCode)
				throw new exception("Invalid winsock supported");

			initialized = true;
		}
	}
}