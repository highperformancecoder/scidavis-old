# Script to test the subsections of:
# "API documentation" of the handbook
# available at: https://highperformancecoder.github.io/scidavis-handbook/sec-python.html

## subsection: class MDIWindow (inherits QWidget)

tName = "Table01"
t=newTable(tName,2,10)
t.confirmClose(False)
assert t.name() == tName
tNewName = tName+"New"
t.setName(tNewName)
assert t.name() == tNewName
assert t.windowLabel() == ""
tNewLabel = "a table"
t.setWindowLabel(tNewLabel)
assert t.windowLabel() == tNewLabel
t.setCaptionPolicy(MyWidget.Name)
assert t.captionPolicy()==MyWidget.Name
t.setCaptionPolicy(MyWidget.Label)
assert t.captionPolicy()==MyWidget.Label
t.setCaptionPolicy(MyWidget.Both)
assert t.captionPolicy()==MyWidget.Both

## default captionPolicy is 2: show both name and label

# t.folder() will return something like 
# <scidavis.Folder object at 0xaec9c8e4>
# But the expected output would be a folder name...

# I don't know how to test "confirmClose(boolean)" because if I set it as True and try to
# close the object there will be no error message,  but a confirmation window will open. 
# Otherwise it just closes...

t.setCell(1,1,10.0)
t2 = app.clone(t)
t2.confirmClose(False)
assert t2.captionPolicy() == MyWidget.Both
assert t2.numRows() == t.numRows()
assert t2.numCols() == t.numCols()
assert t2.cell(1,1) == t.cell(1,1)

# Do some tests of "class MDIWindow" for graph, matrix and note
## graph...
gName = "Graph01"
g=newGraph(gName)
g.confirmClose(False)
assert g.name() == gName
gNewName = gName+"New"
g.setName(gNewName)
assert g.name() == gNewName
assert g.windowLabel() == ""
gNewLabel = "a graph"
g.setWindowLabel(gNewLabel)
assert g.windowLabel() == gNewLabel
for i in (MyWidget.Name,MyWidget.Label,MyWidget.Both):
  g.setCaptionPolicy(i)
  assert g.captionPolicy() == i
g2 = app.clone(g)
assert g2.captionPolicy() == MyWidget.Both
g2.confirmClose(False)

# matrix...
mName = "Matrix01"
m=newMatrix(mName,10,10)
m.confirmClose(False)
assert m.name() == mName
mNewName = mName+"New"
m.setName(mNewName)
assert m.name() == mNewName
assert m.windowLabel() == ""
mNewLabel = "a matrix"
m.setWindowLabel(mNewLabel)
assert m.windowLabel() == mNewLabel
for i in (MyWidget.Name,MyWidget.Label,MyWidget.Both):
  m.setCaptionPolicy(i)
  assert m.captionPolicy() == i

m2 = app.clone(m)
assert m2.captionPolicy() == MyWidget.Both
m2.confirmClose(False)

# note...
nName = "Note01"
n=newNote(nName)
n.confirmClose(False)
assert n.name() == nName
nNewName = nName+"New"
n.setName(nNewName)
assert n.name() == nNewName
assert n.windowLabel() == ""
nNewLabel = "a note"
n.setWindowLabel(nNewLabel)
assert n.windowLabel() == nNewLabel
for i in (MyWidget.Name,MyWidget.Label,MyWidget.Both):
  n.setCaptionPolicy(i)
  assert n.captionPolicy() == i
n2 = app.clone(n)
assert n2.captionPolicy() == MyWidget.Both
n2.confirmClose(False)
app.exit()
