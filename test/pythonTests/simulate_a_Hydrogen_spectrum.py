# This script is aimed to simulate the emission spectrum of hydrogen for some selected
# transitions (emitting at 4102,4341,4861,6563 angstroms) at given ion temperatures
import numpy as np #importing numpy

global np #define np as global in order to be able to use it inside loops
np.random.seed(1)

PI=np.pi
dp=0.003 #noise's stddev
T=10 #ion temperature in eV
Te=T #electron temperature equals to the ion one
ne=1e19 #electron density
M=1 #mass number of the element (M=1 for hydrogen)
h=4.135667662*1e-15 #Plank's constant in eV
alfa=1.69e8 #a constant previously calculated
wins=0.5 #instrumental broadening in angstroms
Npts=12001 #number of points of the curve
t=newTable("Spectrum-for-T--"+str(T)+"eV",2,Npts) 
y0=0
xc=np.array([4102,4341,4861,6563])
xc1=xc[0]
xc2=xc[1]
xc3=xc[2]
xc4=xc[3]
A62=9.7320e+05
A52=2.5304e+06
A42=8.4193e+06
A32=4.4101e+07
w1=(xc1**2*T*(alfa*M)**-1 + wins**2 )**0.5
w2=(xc2**2*T*(alfa*M)**-1 + wins**2 )**0.5
w3=(xc3**2*T*(alfa*M)**-1 + wins**2 )**0.5
w4=(xc4**2*T*(alfa*M)**-1 + wins**2 )**0.5
I62=ne**2*(6)**2*(h**2/(2*PI*Te))**3/2*np.exp(-(13.6-13.22070378)/Te)*A62
I52=ne**2*(5)**2*(h**2/(2*PI*Te))**3/2*np.exp(-(13.6-13.0545016)/Te)*A52
I42=ne**2*(4)**2*(h**2/(2*PI*Te))**3/2*np.exp(-(13.6-12.7485392)/Te)*A42
I32=ne**2*(3)**2*(h**2/(2*PI*Te))**3/2*np.exp(-(13.6-12.0875051)/Te)*A32
for i in range(1,Npts):
 x=4000+(i-1)*0.25
 y0=np.random.normal(0,dp)
 y=y0+(((I62)*np.exp(-4*np.log(2)*((x-xc1)/w1)**2))+((I52)*np.exp(-4*np.log(2)*((x-xc2)/w2)**2))+((I42)*np.exp(-4*np.log(2)*((x-xc3)/w3)**2))+((I32)*np.exp(-4*np.log(2)*((x-xc4)/w4)**2)))/I32
 t.setCell(i,1,x)
 t.setCell(i,2,y)
g=plot(t,'2',0)
l=g.activeLayer()
l.setXTitle("Wavelength (Angs)")
l.setYTitle("Intensity (a. u.)")
l.setTitle("Hydrogen alpha, beta, gamma and \n delta emissions for T=10eV")
l.setScale(0,-0.1,1.1) 
g.confirmClose(False) #close g without confirmation
t.confirmClose(False)
g.exportImage("Hspectrum.png");
app.exit()
