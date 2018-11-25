arrow = ArrowMarker()
arrow.setStart(50,200)
arrow.setEnd(400,400)
arrow.setWidth(2)
arrow.drawStartArrow(True)
arrow.drawEndArrow(True)
arrow.setColor(Qt.green)
arrow.setHeadLength(10)
arrow.setHeadAngle(20)
arrow.fillArrowHead(True)

graph=newGraph()
layer = graph.activeLayer()
layer.addArrow(arrow)
layer.replot()

graph.exportImage("arrowMarker.png");
app.exit()
