# coding: utf-8
# Script to test linear and polynomial fits

import random
# set seed to ensure replicability
random.seed(1)


dp=1.5 #noise's stddev

#generate a table t1 with linear data points with some noise and random y-errors
t1Name="Linear-data"
t1=newTable(t1Name,3,100)
for i in range(1,t1.numRows()+1):
 t1.setCell(i,1,i+random.uniform(0,dp))
 t1.setCell(i,2,2+3*i+random.uniform(0,dp))
 t1.setCell(i,3,random.uniform(0,dp))

g1=plot(t1,'2',1) #plotting the linear data

l1=g1.activeLayer()
curve1=t1Name+"_2"
l1.addErrorBars(curve1,t1,'3')

g1.confirmClose(False)
t1.confirmClose(False)

f1=LinearFit(l1,curve1) #linear fit
f1.fit()

f2=PolynomialFit(l1,curve1,5) #polynomial fit of degree 5
f2.setColor("blue")
f2.fit()

print("#","linear coef. =",f1.results()[0]," ± ",f1.errors()[0],"\n# angular coef. =",f1.results()[1]," ± ",f1.errors()[1])
g1.exportImage("linearAndPolyFits.png")
app.exit()
