#if !defined(__SESSION_H)
#define  __SESSION_H

#include <WinSock2.h>
#include "IStatusListener.h"
#include "IConnection.h"
#include "IDataReceiver.h"

namespace NativeWinsock
{
	// Integer specifying the size of the receive buffer
	const int BUFFER_SIZE = 1024;

	// Callback function called by the WinAPI thread pool when a connection
	// receives data, calls Session::dataReceived()
	// PVOID param: User defined object, is the ConnectionManager which the 
	//				wait callback was made from
	// BOOLEAN timeout: Indicates whether or not the wait has timed out
	void CALLBACK DataReceivedCallback(PVOID param, BOOLEAN timeout);

	class Session : public IConnection
	{
	private:
		// ID of the connection which is unique to each connection
		unsigned int		connectionID;
		// Bool saying if the connection is closed or not, as connections
		// are re-used in the managed, this is set to false when a connection
		// is closed or not used-
		bool				closed;
		// Error indicating something went wrong during a send call and the
		// socket should be closed when data is received or connection is 
		// timed out (0 bytes received)
		bool				sendError;
		// Indicating wether or not the connection is a valid connection.
		// Connections with this field set to true will be return when the 
		// max connections limit is reached
		bool				isValid;
		// Indicates if the connection is a connection that can be re-used and
		// is ready to be used
		bool				isFree;
		// Buffer for receiving data
		char				receiveBuffer[BUFFER_SIZE];
		// WinSock socket object which reffers to the underlying 
		// layers in the OSI-stack.
		SOCKET				connection;
		// Address object holding information about who we are connected to
		sockaddr_in			address;
		// Shortcut to the manager when a connection is closed
		IStatusListener<Session>*	statusListener;

		//=======Members for data receiving=======\\

		// Holds data describing the receive buffer
		WSABUF  			bufferInfo;				
		// Holds pointer to callback event (receive)
		_OVERLAPPED			receiveOverlapped;		
		// Flags for receiving
		DWORD				flags;					
		// Amount of bytes we receive per call
		DWORD				bytesReceived;			

		//=======Members for data sending=======\\

		// Overlapped information
		_OVERLAPPED			sendOverlapped;			
		// Amount of bytes we sent
		DWORD				bytesSent;				
		// Struct describing the data we're sending
		WSABUF	     		sendInfo;				
		// Handle created by the wait for data functions
		HANDLE				newWaitHandle;
		// Routs a refferance to the buffer and how much bytes received
		// when data isreceived
		IDataReceiver*		dataReceiver;

		// Continues waiting for data
		void continueReceive();
		// Closes the connection and tells the IStatusListener 
		// this object is free
		void close();

	public:
		// Default constructor which creates an 
		// invalid connection, as no ID is given
		Session();
		// Destructor, cleans up resources
		~Session();
		// Initializes fields and sets the connection ID
		Session(const unsigned int connectionID, 
			IStatusListener<Session>* listener);

		// Accepts a connection if there is any
		// Return value: True if a connection was accepted, otherwise false
		bool acceptConnection(SOCKET sock);
		// Return whether or not this connection is free
		bool free() const;
		// Return true if the connection is a valid connection, otherwise false
		bool valid() const;
		// Closes connection and makes it ready for new connection
		void setFree();
		// Start receiving data from the connection
		void beginReceive();
		// Callback when data is received
		void dataReceived();

		//=======Override functions for the IConnection interface=======\\
		// Disconnects the connection
		void disconnect();
		// Returns the ID of the connection, if invalid connection, returns 0
		unsigned int getConnectionID();
		// Returns the IP of the connected connection. If there is no active 
		// connection, it will return 0
		int getIP();
		// Sends parameter with length = 'length' to the connection.
		// If it fails, connection will close, connection will be
		// set as 'free' and 'closed' set to true
		void sendData(const char* data, const int length);
		// Sets the object which shall be invoked when data is received
		void setReceiver(IDataReceiver* receiver);
	};
}

#endif