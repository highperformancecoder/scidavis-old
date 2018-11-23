graph=newGraph()
layer = graph.activeLayer()
image = layer.addImage("scidavis-logo.png")
assert image.fileName()=="scidavis-logo.png"
image.setSize(260,100)
assert image.size()==QtCore.QSize(260,100)
image.setCoordinates(200,800,330,850)
layer.replot()
graph.exportImage("imageMarker.png");
app.exit()
