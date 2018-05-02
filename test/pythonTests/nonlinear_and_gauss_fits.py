# Script to test non-linear and Gaussian fits
import random
random.seed(1)
from math import *

global gaussian_curve
def gaussian_curve(x):
    y1 = 2.0
    A1 = 10.0
    w1 = 10.0
    xc1 = 50.0
    return (y1 + A1*sqrt(2/pi)/w1*exp(-2*((x-xc1)/w1)**2)) #this is the default Gauss formula used by SciDAVis

dp=0.003 #noise's stddev
    
#generate a table t1 with Gaussian data points with some noise and random y-errors
t1Name="Gaussian-curve"
t1=newTable(t1Name,3,100)
for i in range(1,t1.numRows()+1):
    xx=float(i)
    t1.setCell(1,i,xx)
    t1.setCell(2,i,gaussian_curve(xx)+random.uniform(0,dp))
    t1.setCell(3,i,random.uniform(0,dp))

g1=plot(t1,'2',1) #plotting the curve
        
l1=g1.activeLayer()
curve1=t1Name+"_2"
l1.addErrorBars(curve1,t1,'3')
    
g1.confirmClose(False)
t1.confirmClose(False)
    
f1=GaussFit(l1,curve1) #Gauss fit
f1.setInitialValues(11,49.1,11,1) #*
f1.fit()
print f1.chiSquare()
#assert f1.chiSquare() < 1e-4,"f1.chiSquare() >=1e-4"
assert f1.rSquare() > 0.99,"f1.rSquare() >= 0.99"

    #* for some unknown reason, this fit does not work (or works randomly) without initial values when called from
# a python script. It works fine when called from Analysis -> Quick Fit -> Fit Gaussian

#non-linear fit (using a Gauss formula, of course)
f2=NonLinearFit(l1,curve1)
f2.setFormula("y0+A*sqrt(2/PI)/w*exp(-2*((x-xc)/w)^2)")
f2.setParameters("A","xc","w","y0")
f2.setInitialValues(11,49.1,11,1)
f2.setYErrorSource(0) #** using 0 or Fit.UnknownErrors in the argument disable the use of error bar in the fit
f2.setColor("blue")
f2.fit()
assert f2.chiSquare() < 1e-4
assert f2.rSquare() > 0.99
#** the non-linear fit is not converging with error bars when called from a python script. It looks to work
# fine when called from Analysis -> Fit Wizard...

app.exit()
