#ifndef NEAR_H
#define NEAR_H

// performs customisable fuzzy matching of reals
inline bool near(double x, double y, double tol)
{return fabs(x-y)/(fabs(x)+fabs(y)) < tol;}


#endif
