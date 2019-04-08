from checkThrow import checkThrow

app.loadProject("Establishing_contact.sciprj");
n=app.note("TestNote")
assert n.windowTitle()=="TestNote"

# executing this script causes a crash, so import the script directly here
# n.executeAll()

# Script to test the subsections of:
# "Accessing SciDAVis's functions from Python"
# available at: https://highperformancecoder.github.io/scidavis-handbook/sec-python.html
# Basically a Ctrl+C -> Ctrl+V

## "Establishing contact"
t = table("Table1")
m = matrix("Matrix1")
g = graph("Graph1")
n = note("Notes1")

# create an empty table named "tony" with 5 columns and 2 rows: 
#(this need to be corrected in the manual because it is written 5 rows and 2 columns)
t2 = newTable("tony", 5, 2)
assert t2.numCols() == 5
assert t2.numRows() == 2
assert t2.name() == "tony"
# use defaults
t3 = newTable()
# create an empty matrix named "gina" with 42 rows and 23 columns:
m2 = newMatrix("gina", 42, 23)
# use defaults
m3 = newMatrix()
# create an empty graph window
g2 = newGraph()
# create an empty note named "momo"
n2 = newNote("momo")
# use defaults
n3 = newNote()

f0 = activeFolder()
# and
f1 = rootFolder()

f2 = f1.folders()[0]
# I can't see how the following folder method can pass:
# a) it is not documented in the manual
# b) Folder has no "folder" method
# c) scidavis.sip doesn't define it either
# f3 = f1.folder("New Folder",True,False)
checkThrow('t1 = f1.table("Table2",False)')
checkThrow('m4 = f1.matrix("Matrix2",False)')
checkThrow('g3 = f1.graph("Graph2",False)')
checkThrow('n4 = f1.note("Notes2",False)')

app.exit()
