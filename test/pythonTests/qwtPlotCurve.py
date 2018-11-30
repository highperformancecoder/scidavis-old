import random
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
t1=newTable(t1Name,3,NP)
for i in range(t1.numRows()):
    xx=float(i+1)
    t1.column(0).setValueAt(i,xx)
    t1.column(1).setValueAt(i,f1(xx)+random.uniform(0,dp))
    t1.column(2).setValueAt(i,10*f1(xx)+random.uniform(0,dp))

g1=plot(t1,'2',1)
layer = g1.activeLayer()
layer.insertCurve(t1,'3',0,1)

symbol = QwtSymbol()
symbol.setStyle(QwtSymbol.Triangle)
symbol.setOutlineColor(QtGui.QColor(Qt.red))
symbol.setFillColor(QtGui.QColor(Qt.green))
symbol.setSize(20)
curve=layer.curve(0)
curve.setSymbol(symbol)

# enable RHS curve and axes
layer.curve(1).setXAxis(Layer.Top)
layer.curve(1).setYAxis(Layer.Right)
layer.enableAxis(QwtPlot.xTop,True)
layer.enableAxis(QwtPlot.yRight,True)

layer.replot()

g1.exportImage("triangle.png")

# OK now extract the pen and brush
pen=symbol.pen()
brush=pen.brush()
assert brush.color()==Qt.red
brush=symbol.brush()
assert brush.color()==Qt.green
assert symbol.style()==QwtSymbol.Triangle
assert symbol.size()==QtCore.QSize(20,20)

assert curve.dataSize()==NP
for i in range(t1.numRows()):
    assert curve.x(i)==t1.column(0).valueAt(i)
    assert curve.y(i)==t1.column(1).valueAt(i)
    assert curve.minXValue() < curve.x(i) < curve.maxXValue()
    assert curve.minYValue() < curve.y(i) < curve.maxYValue()

# equality not defined for QwtSymbol
#assert curve.symbol()==symbol
symbol=curve.symbol()
pen=symbol.pen()
brush=pen.brush()
assert brush.color()==Qt.red
brush=symbol.brush()
assert brush.color()==Qt.green
assert symbol.style()==QwtSymbol.Triangle
assert symbol.size()==QtCore.QSize(20,20)


pen=curve.pen()
pen.setColor(Qt.blue)
pen.setWidth(5)
pen.setStyle(Qt.DashLine)
pen.setJoinStyle(Qt.RoundJoin)
pen.setCapStyle(Qt.RoundCap)
pen.setMiterLimit(3)
curve.setPen(pen)
assert curve.pen()==pen
pen=curve.pen()
assert pen.color()==Qt.blue
assert pen.width()==5
assert pen.style()==Qt.DashLine
assert pen.joinStyle()==Qt.RoundJoin
assert pen.capStyle()==Qt.RoundCap
assert pen.miterLimit()==3

pen.setDashPattern([3,1,2,1])
pen.setDashOffset(2)
assert pen.dashPattern()==[3,1,2,1]
assert pen.dashOffset()==2
assert pen.style()==Qt.CustomDashLine

brush=curve.brush()
brush.setColor(Qt.red)
brush.setTransform(QtGui.QTransform(0,1,0,1,10,10))
brush.setStyle(Qt.BDiagPattern)
curve.setBrush(brush)
assert curve.brush()==brush
brush=curve.brush()
assert brush.color()==Qt.red
assert brush.transform()==QtGui.QTransform(0,1,0,1,10,10)
assert brush.style()==Qt.BDiagPattern

curve.setOutlineColor(Qt.blue)
curve.setFillColor(Qt.green)
curve.setFillStyle(Qt.CrossPattern)
assert curve.pen().color()==Qt.blue
assert curve.brush().color()==Qt.green
assert curve.brush().style()==Qt.CrossPattern
app.exit()
