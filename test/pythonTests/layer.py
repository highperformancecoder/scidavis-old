# test pie plots first
table=newTable("t1",2,4)
table.column(0).replaceValues(0,[15,15,40,30])
table.column(1).replaceValues(0,[60,0,30,10])
graph=plot(table,'1',5)
graph.export("layer1.png")
layer=graph.activeLayer()
# this doesn't seem to do what it says on the tin!
layer.insertCurve(table,"2",5)
layer.replot()
graph.export("layer2.png")
assert layer.isPiePlot()
assert layer.pieLegend()=="\\p{1} 1\n\\p{2} 2\n\\p{3} 3\n\\p{4} 4\n"

# insertCurve is exercised for more regular plot types elswhere, so test the parametric ones here
graph=newGraph()
layer=graph.activeLayer()
layer.insertFunctionCurve("sin(x)",0,2*math.pi)
layer.replot()
graph.export("layer3.png")

graph=newGraph()
layer=graph.activeLayer()
layer.insertPolarCurve("sin(3*t)","t")
layer.replot()
graph.export("layer4.png")

graph=newGraph()
layer=graph.activeLayer()
layer.insertParametricCurve("sin(t)","cos(3*t)",0,2*math.pi)
layer.replot()
graph.export("layer5.png")

layer.setTitle("A bunch of legends")
layer.legend().setText("What a legend")
layer.newLegend("Also a legend").setOriginCoord(0,0)
layer.replot()
graph.export("layer6.png")

layer.removeLegend()
layer.removeCurve(0)
layer.setXTitle("such and such")
layer.setYTitle("too much")
layer.setRightTitle("right!!!!")
layer.setTopTitle("top of the world")
layer.enableAxis(QwtPlot.yRight,True)
layer.enableAxis(QwtPlot.xTop,True)
layer.replot()
graph.export("layer7.png")

# different scale options

graph=newGraph()
layer=graph.activeLayer()
layer.enableAutoscaling()
layer.insertFunctionCurve("tanh(x)",0,1e6)
layer.setAxisNumericFormat(QwtPlot.yLeft,3,3)
layer.setAxisNumericFormat(QwtPlot.xBottom,2,1)
layer.setScale(QwtPlot.xBottom,1,1e6,0,5,5,1) #logarithmic
layer.setFrame(2)
layer.setMargin(5)
layer.setBackgroundColor(Qt.yellow)
layer.setCanvasColor(Qt.cyan)
layer.replot()
graph.export("layer8.png")


app.exit()
