# Script to test the subsections of:
# "Accessing SciDAVis's functions from Python"
# available at: https://highperformancecoder.github.io/scidavis-handbook/sec-python.html
# Basically a Ctrl+C -> Ctrl+V most of times

## Working with Tables

# 1st, create some empty tables - use defaults
t1=newTable()
t2=newTable()
t1.confirmClose(False)
t2.confirmClose(False)

assert t1.cell(1,1) == 0.0
t1.setCell(1,1,pi)
assert t1.cell(1,1) == pi

assert t2.text(1,1) == ""
# need to set the desired column as text first
c1t2=t2.column("1")
c1t2.setColumnMode("Text")
t2.setText(1,1,"pi")
assert t2.text(1,1) == "pi"

assert t2.colName(2) == "2"
t2.setColName(2,"two")
assert t2.colName(2) == "two"

assert t1.numRows() == 30
assert t1.numCols() == 2
t1.setNumRows(10)
t1.setNumCols(5)
assert t1.numRows() == 10
assert t1.numCols() == 5

# fill columns with x^2, x = row number, to perform the normalization test
for i in range(1,t1.numRows()+1):
 t1.setCell(1,i,i**2)
 t1.setCell(2,i,i**2)

t1.normalize(1) #normalize only column 1
assert t1.cell(1,t1.numRows()) == 1.0 # verify only column 1
t1.normalize() #normalize all columns
assert t1.cell(2,t1.numRows()) == 1.0 # verify only column 2

# t.importASCII will not be included in this test file - add it to TODO list
# t.notifyChanges() will not be included because I guess it is deprecated - add it to a TOCHECK list
app.exit()
