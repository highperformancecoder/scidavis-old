# Script to test mathematical functions listed in Table 4.4 of
# https://highperformancecoder.github.io/scidavis-handbook/sec-python.html
# it will be done in the same order of that in the table
# I'm sorry in advance for the lack of creativity :-D

##*** Attention, some assert tests may fail due to the rounding that python does.***##

# The 'pi' constant is defined internally with the value of: 3.14159265359
# so, lets define the pi/2 and pi/4 values as piH and piQ:
piH = pi/2.0
piQ = pi/4.0

# acos(x) - inverse cosine
assert acos(-1.0) == pi
assert acos(0) == piH

# asin(x) - inverse sine
assert asin(0) == 0.0
assert asin(1.0) == piH

# atan(x) - inverse tangent
assert atan(1.0) == piQ

# atan2(y,x) - equivalent to atan(y/x), but more efficient
assert atan2(1.0,1.0) == piQ
assert atan2(-1.0,1.0) == -piQ

# ceil(x) - ceiling; smallest integer greater or equal to x
assert ceil(pi) == 4
assert ceil(1.000001) == 2
assert ceil(-1.1) == -1

# cos(x) - cosine of x
assert cos(0.0) == 1.0
assert cos(piH) < 1e-16

# cosh(x) - hyperbolic cosine of x
assert cosh(0.0) == 1.0

# degrees(x) - convert angle from radians to degrees
assert degrees(pi) == 180.0
assert degrees(piH) == 90.0
assert degrees(piQ) == 45.0

# exp(x) - Exponential function: e raised to the power of x.
# The 'e' constant is also defined internally with the value of: 2.71828182846
assert exp(1.0) == e

# fabs(x) - absolute value of x
assert fabs(-1) == 1
assert fabs(-pi) == pi
assert fabs(-1e-10) == 1e-10
assert fabs(1) == 1

# floor(x) - largest integer smaller or equal to x
assert floor(1.1) == 1
assert floor(-1.1) == -2
assert floor(1e-10) == 0

# fmod(x,y) - remainder of integer division x/y
assert fmod(3,2) == 1
assert fmod(-3,2) == -1

# frexp(x) - Returns the tuple (mantissa,exponent) such that x=mantissa*(2**exponent) where exponent is an integer and 0.5 <=abs(m)<1.0
assert frexp(3) == (0.75,2)
assert frexp(4) == (0.5,3)

# hypot(x,y) - equivalent to sqrt(x*x+y*y)
assert hypot(3,4) == 5
assert hypot(5,12) == 13
assert hypot(7,24) == 25

# ldexp(x,y) - equivalent to x*(2**y)
assert ldexp(2,0) == 2
assert ldexp(2,1) == 4
assert ldexp(2,2) == 8

# log(x) - natural (base e) logarythm of x
assert log(e) == 1
assert log(1) == 0

# log10(x) - decimal (base 10) logarythm of x
assert log10(10) == 1
assert log10(1) == 0

# modf(x) - return fractional and integer part of x as a tuple
assert modf(1.5) == (0.5, 1.0)
assert modf(-1.5) == (-0.5, -1.0)

# pow(x,y) - x to the power of y; equivalent to x**y
assert pow(2,0) == 2**0
assert pow(2,1) == 2**1
assert pow(2,2) == 2**2

# radians(x) - convert angle from degrees to radians
assert radians(180.0) == pi
assert radians(90.0) == piH
assert radians(45.0) == piQ

# sin(x) - sine of x
assert sin(0.0) == 0.0
assert sin(piH) == 1.0

# sinh(x) - hyperblic sine of x
assert sinh(0.0) == 0.0

# sqrt(x) - square root of x
assert sqrt(4.0) == 2.0
assert sqrt(2.0) == 2**(0.5)

# tan(x) - tangent of x
assert tan(0.0) == 0.0
assert tan(piQ) == -tan(-piQ)

# tanh(x) - hyperbolic tangent of x
assert tanh(0.0) == 0.0
app.exit()
