# script that generates a generic curve with some noise and smooth it
# smoothing is not documented in the SciDAVis manual

import random

global f1
def f1(x):
 a1 = 5.0
 a2 = -10.0
 a3 = 10.0
 b1 = 2.0
 return ((a1+a2*x+a3*x**2-b1/x**3)/1000.0)

dp=0.5 #noise's stddev

# set seed to ensure replicability
random.seed(1)

t1Name="generic-curve"
t1=newTable(t1Name,2,51)
for i in range(1,t1.numRows()+1):
 xx=float(i)
 t1.setCell(1,i,xx)
 t1.setCell(2,i,f1(xx)+random.uniform(0,dp))
g1=plot(t1,'2',1) #plotting the curve

g1.confirmClose(False)
t1.confirmClose(False)

l1=g1.activeLayer()
curve1=t1Name+"_2"

smth1=SmoothFilter(l1,curve1)
smth1.setMethod(1) # method=1 - Savitzky-Golay 
smth1.setSmoothPoints(2,2) # left and right points
smth1.setPolynomOrder(3)
smth1.run()

smth2=SmoothFilter(l1,curve1)
smth2.setMethod(2) # method=2 - FFT
smth2.setSmoothPoints(3)
smth2.setColor("blue")
smth2.run()

smth3=SmoothFilter(l1,curve1)
smth3.setMethod(3) # method=3 - Moving Window Average
smth3.setSmoothPoints(3)
smth3.setColor("green")
smth3.run()

g1.exportImage("smoothing.png")
app.exit()
