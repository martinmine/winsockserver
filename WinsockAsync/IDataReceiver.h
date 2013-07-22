#if !defined(__IDATARECEIVER_H)
#define  __IDATARECEIVER_H

// Interface describing which functions that can be called on an object
// when data is received. A callback for when data has been sent can also
// be implemented here if needed.
class IDataReceiver
{
public:
	// Function called when data is received
	virtual void receivedData(const char*, const int) = 0;
	// Declare the destructor as virtual, so derived classes will get
	// deleted when a delete is called on a IDataReceiver
	virtual ~IDataReceiver() { }
};

#endif