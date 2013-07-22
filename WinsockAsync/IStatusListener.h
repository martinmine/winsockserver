#if !defined(__ISTATUSLISTENER_H)
#define  __ISTATUSLISTENER_H

// Template interface describing which funcions the manager should
// have access to when a connection is closed
template <typename T> class IStatusListener
{
public:
	// Function called when a connection is disconnected
	virtual void onDisconnect(T* conn) = 0 { };
}; 

#endif