# Script to test non-linear and Gaussian fits
import random
from math import *

global gaussian_curve
def threeExponential_curve(x):
    return exp(-0.1*x)+0.5*exp(0.4*x)+0.2*exp(.3*x)

dp=0.003 #noise's stddev
    
#generate a table t1 with Gaussian data points with some noise and random y-errors
t1Name="Gaussian-curve"
t1=newTable(t1Name,3,100)
for i in range(1,t1.numRows()+1):
    xx=float(i)
    t1.setCell(1,i,xx)
    t1.setCell(2,i,threeExponential_curve(xx)+random.uniform(0,dp))
    t1.setCell(3,i,random.uniform(0,dp))

g1=plot(t1,'2',1) #plotting the curve
        
l1=g1.activeLayer()
curve1=t1Name+"_2"
l1.addErrorBars(curve1,t1,'3')
    
g1.confirmClose(False)
t1.confirmClose(False)
    
f1=ThreeExpFit(l1,curve1)
f1.setInitialValues(.8,.5,0.4,-.2,1.1,0,1)
f1.fit()
print f1.rSquare()
assert f1.rSquare() > 0.99,"f1.rSquare() >= 0.99"
assert f1.formula()=="A1*exp(-x/t1)+A2*exp(-x/t2)+A3*exp(-x/t3)+y0",f1.formula()

app.exit()
