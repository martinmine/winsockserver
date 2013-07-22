#if !defined(__INCOMMINGCONNECTION_H)
#define __INCOMMINGCONNECTION_H

#include "NetworkConnection.h"

class IncommingConnection : public NetworkConnection
{
	private:
		// ID of the connection which is unique to each connection
		unsigned int		connectionID;
		// Indicating wether or not the connection is a valid connection.
		// Connections with this field set to true will be return when the 
		// max connections limit is reached
		bool				isValid;
		// Indicates if the connection is a connection that can be re-used and
		// is ready to be used
		bool				isFree;

	public:
		// Initializes fields and sets status listener and connection ID
		IncommingConnection(unsigned int id,
			IStatusListener<NetworkConnection>* listener)
			: connectionID(id), NetworkConnection(listener) { }

		// Accepts a connection if there is any
		// Return value: True if a connection was accepted, otherwise false
		bool acceptConnection(SOCKET sock);
		// Return whether or not this connection is free
		bool free() const;
		// Return true if the connection is a valid connection, otherwise false
		bool valid() const;
		// Closes connection and makes it ready for new connection
		void setFree();
		// Returns the ID of the connection, if invalid connection, returns 0
		unsigned int getConnectionID() const { return connectionID; };
};

#endif