graph=newGraph()
layer = graph.activeLayer()
image = layer.addImage("scidavis-logo.png")
assert image.fileName()=="scidavis-logo.png"
image.setSize(260,100)
from PyQt4.QtCore import QSize 
assert image.size()==QSize(260,100)
image.setCoordinates(200,800,330,850)
layer.replot()
graph.exportImage("imageMarker.png");
app.exit()
