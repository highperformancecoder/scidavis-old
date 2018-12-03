graph=newGraph()
layer1=graph.activeLayer()
layer2=graph.addLayer()
assert graph.numLayers()==2
graph.setActiveLayer(layer2)
assert graph.activeLayer()==layer2

graph.addLayer()
graph.addLayer()
assert graph.numLayers()==4
for (i,l) in enumerate(graph.layers()):
    l.setTitle("graph"+str(i))
    l.insertFunctionCurve("sin(x)",0,2*math.pi)
    l.replot()

graph.setCols(2)
graph.setRows(2)
graph.setMargins(40,40,40,40)
graph.setSpacing(20,20)
graph.setAlignment(0,1)

graph.arrangeLayers(True)

# doesn't seem to export image?
graph.export("graph.png")
app.exit()
