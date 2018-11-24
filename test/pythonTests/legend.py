graph=newGraph()
layer = graph.activeLayer()
legend = layer.newLegend("hello world")
legend.setBackgroundColor(Qt.green)
legend.setTextColor(Qt.darkBlue)
legend.setFrameStyle(2)
legend.setFont(QtGui.QFont("Arial",14,QtGui.QFont.Bold))
legend.setOriginCoord(400,400)
layer.replot()
graph.exportImage("legend.png");
app.exit()

