#include "epsilon.h"

double machineEpsilon(float start)
{
    float prev_epsilon;
    float EPS = 1;

    while ((start+EPS) != start)
    {
        prev_epsilon = EPS;
        EPS /= 2;
    }
    return prev_epsilon;
}

bool areSame(double a, double b)
{
    return fabs(a - b) < machineEpsilon(std::max(a,b));
}
