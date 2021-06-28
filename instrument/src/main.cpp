#include <stdio.h>
#include "remoteserviceagent.h"

int main() {
    printf("Name %s\n", RemoteServicesAgent::getLibInterfaceName());
    printf("Version %s\n", RemoteServicesAgent::getLibInterfaceVersion());

    int question = 323;
    int factorial = 0;
    bool isEven = false;
    bool isOdd = false;
    bool isPrime = false;

    factorial = Factorial(question);
    printf("Question = %d\nFactorial = %d\n", question, factorial);

    isEven = IsEven(question);
    printf("Question = %d\nIsEven = %d\n", question, isEven);

    isOdd = IsOdd(question);
    printf("Question = %d\nIsOdd = %d\n", question, isOdd);

    isPrime = IsPrime(question);
    printf("Question = %d\nIsPrime = %d\n", question, isPrime);

    return 0;
}
