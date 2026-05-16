#include "math.h"

double ldexp(double x, int exp) {
    if (exp > 0) { while (exp--) x *= 2.0; }
    else { while (exp++) x /= 2.0; }
    return x;
}
