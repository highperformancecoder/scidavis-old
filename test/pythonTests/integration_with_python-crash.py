print "hello"
app.loadProject("integration_with_python-crash.sciprj")
print "after load project"
lower_limit = 0
upper_limit = 30
g=graph("Graph1")
curveName="Table1_2"
integral = Integration(g.activeLayer(),curveName,lower_limit,upper_limit)
integral.setMethod(0) #method = 1 -> Linear interpolation
integral.run()
print "#",integral.result()
app.exit()
# 10.2358922829 : area calculated running this script in SciDAVis 1.21. Exactly the
# same result obtained using Analysis -> Integrate... using both 1.21 and 1.22 versions
# run this script in SciDAVis 1.22 and the program will crash -> Segmentation fault
