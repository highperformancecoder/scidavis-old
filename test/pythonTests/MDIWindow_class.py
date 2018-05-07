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
for i in range(2,-1,-1):
  assert t.captionPolicy() == i
  if (i > 0):
    t.setCaptionPolicy(i-1)
## default captionPolicy is 2: show both name and label

# t.folder() will return something like 
# <scidavis.Folder object at 0xaec9c8e4>
# But the expected output would be a folder name...

# I don't know how to test "confirmClose(boolean)" because if I set it as True and try to
# close the object there will be no error message,  but a confirmation window will open. 
# Otherwise it just closes...

t.setCell(1,1,10.0)
t2 = t.clone()
t2.confirmClose(False)
assert t2.captionPolicy() == 0
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
for i in range(2,-1,-1):
  assert g.captionPolicy() == i
  if (i > 0):
    g.setCaptionPolicy(i-1)
g2 = g.clone()
assert g2.captionPolicy() == 0
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
for i in range(2,-1,-1):
  assert m.captionPolicy() == i
  if (i > 0):
    m.setCaptionPolicy(i-1)
m2 = m.clone()
assert m2.captionPolicy() == 0
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
for i in range(2,-1,-1):
  assert n.captionPolicy() == i
  if (i > 0):
    n.setCaptionPolicy(i-1)
n2 = n.clone()
assert n2.captionPolicy() == 0
n2.confirmClose(False)
app.exit()
