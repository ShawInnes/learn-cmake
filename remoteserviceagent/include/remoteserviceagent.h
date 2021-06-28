#ifndef REMOTESERVICEAGENT_LIBRARY_H
#define REMOTESERVICEAGENT_LIBRARY_H
#include <stdbool.h>

// Returns n! (the factorial of n).  For negative n, n! is defined to be 1.
int Factorial(int n);

// Returns true if n is a prime number.
bool IsPrime(int n);

// Returns n * 2
int Double(int n);

// Returns true if n is an even number.
bool IsEven(int n);

// Returns true if n is an even number.
bool IsOdd(int n);

namespace RemoteServicesAgent {
    const char* getLibInterfaceName();
    const char *getLibInterfaceVersion();
}

#endif //REMOTESERVICEAGENT_LIBRARY_H

