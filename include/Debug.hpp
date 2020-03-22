#ifndef DEBUG_HEADER
#define DEBUG_HEADER

#include <iostream>
using namespace std;

#ifdef DEBUGGING_ON
#define DEBUG(X) do{ X } while (0)
#define DEBUG_LOG(message) cout << "[" << __FILE__ << " at line " << __LINE__ << "] " << message << endl
#else
#define DEBUG(X)
#define DEBUG_LOG(message)
#endif

class Nullable
{

public:

	Nullable() : _isNull(true) {}
	Nullable(bool isNull) : _isNull(isNull) {}

	inline bool IsNull(void) const { return _isNull; }

protected:

	inline void SetNull(bool isNull) { _isNull = isNull; return; }

private:

	bool _isNull;
};

#endif
