# script that generates a generic curve with some noise and interpolate it
# Interpolation is not documented in the SciDAVis manual
import random

app.switchToLanguage("en")

global f1
def f1(x):
 a1 = 5.0
 a2 = -10.0
 a3 = 10.0
 b1 = 2.0
 return ((a1+a2*x+a3*x**2-b1/x**3)/1000.0)

dp=5.0 #noise's stddev
NP=51

# set seed to ensure replicability
random.seed(1)

t1Name="generic-curve"
t1=newTable(t1Name,2,NP)
for i in range(1,t1.numRows()+1):
 xx=float(i)
 t1.setCell(i,1,xx)
# t1.setCell(i,2,f1(xx)+dp*r[i])
 t1.setCell(i,2,f1(xx)+random.uniform(0,dp))

g1=plot(t1,'2',1) #plotting the curve

g1.confirmClose(False)
t1.confirmClose(False)

l1=g1.activeLayer()
curve1=t1Name+"_2"

start=t1.cell(1,1)
end=t1.cell(t1.numRows(),1)

# brief syntax of interpolation:
# Interpolation(layer,curve name, start, end, method number)
# valid method numbers are 0, 1, and 2 for Linear, Cubic, and Akima methods, respectively
interp0=Interpolation(l1,curve1,start,end,Interpolation.Linear)
interp0.setOutputPoints(3*NP)
interp0.run()

interp1=Interpolation(l1,curve1,start,end,Interpolation.Cubic)
interp1.setOutputPoints(3*NP)
interp1.setColor("blue")
interp1.run()

interp2=Interpolation(l1,curve1,start,end,Interpolation.Akima)
interp2.setOutputPoints(3*NP)
interp2.setColor("green")
interp2.run()

g1.exportImage("interpolation.png")
app.exit()
