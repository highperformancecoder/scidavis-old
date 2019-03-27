# Script to test the subsections of:
# "API documentation" of the handbook
# available at: https://highperformancecoder.github.io/scidavis-handbook/sec-python.html

## subsection: "class Column (inherits AbstractAspect)"

import random
random.seed(1)

nCols = 3
nRows = 20

t = newTable("Table01",nCols,nRows)
t.confirmClose(False)
c1 = t.column("1")
c2 = t.column("2")
c3 = t.column("3")


# Testing column name (verification and change)
assert c2.name() == "2"
c2.setName("col2")
assert c2.name() == "col2"


# Testing data types (column modes) of a column (verification and change)
colModes = ["Numeric","Text","Month","Day","DateTime"]
for i in range(0,len(colModes)-1):
  assert c3.columnMode() == colModes[i]
  c3.setColumnMode(colModes[i+1])
assert c3.columnMode() == colModes[-1]
c3.setColumnMode(colModes[0])

# rowCount() is the largest row index of filled cells in a column
# numRows() is the number of rows in table, never smaller than maximum of all rowCount's
assert c1.rowCount() == 0
assert t.numRows() == 20

# in an empty Table, insertRows works only at the column top
c1.insertRows(0,8)
assert c1.rowCount() == 8
assert t.numRows() == 20

c1.removeRows(3,3)
assert c1.rowCount() == 5
assert t.numRows() == 20

# set a value in row beyond rowCount
c1.setValueAt(12,3.5e-4)
assert c1.rowCount() == 13

c1.insertRows(11,18)
assert c1.valueAt(30) == 3.5e-4
assert t.numRows() == 31

# fill c1 with row random values
for j in range(1,nRows+1):
  t.setCell(j,1,random.uniform(0,1.0))
c2.copy(c1)
for j in range(1,nRows+1):
  assert t.cell(j,2) == t.cell(j,1)

c3.copy(c1,0,10,10)
for k in range(1,11):
  assert t.cell(k+10,3) == t.cell(k,1)
# I guess that there is an inconsistency in the command "copy(Column,int,int,int)"... It works when it
# is called using 'pure python' indexation for vectors, that is, starting from zero, not in the same
# way as column indexing works on SciDAVis,starting from 1.

assert c1.plotDesignation() == "X"
assert c2.plotDesignation() == "Y"
c3.setPlotDesignation("yErr")
assert c3.plotDesignation() == "yErr"

for i in range(0,nRows):
  assert c1.isInvalid(i) == False
assert c1.isInvalid(nRows) == True
for l in range(0,10):
# print c3.isInvalid(l)
  assert c3.isInvalid(l) == True
# Another inconsistency regarding indexes...

assert c2.formula(1) == ""
# setFormula(int, string) is not working. I don't know if it make sense to set a formula for a single row
# of a column... I would suggests to remove it from scidavis.sip and from the manual

# clearFormulas() -- I'll not test this

assert c1.valueAt(10) == c2.valueAt(10)
c2.setValueAt(10, 0.1)
assert c2.valueAt(10) == 0.1
# Here the handbook indicates correctly that valueAt() is given by a 0-based index

v1 = [.2,.3,.4,.5,.6,.7,.8,.9,.99]
c2.replaceValues(11,v1)
for j in range(11,11+len(v1)-1):
  assert c2.valueAt(j) == v1[j-11]
# replaceValues() is also 0-based index


c3.setColumnMode("Text")
for k in range(1,nRows+1):
  t.setText(k,3,"a")

print(type(c3.textAt(1)))
assert c3.textAt(1) == "a"
c3.setTextAt(1,"b")
assert c3.textAt(1) == "b"
# 0-based index for textAt and setTextAt too

v2 = ["q","w","e","r","t"]
c3.replaceTexts(2,v2)
for l in range(2,2+len(v2)-1):
  assert c3.textAt(l) == v2[l-2]

# add three columns
t.addColumns(3)
colNames = [str(t.column(i).name()) for i in range(t.numCols())]
assert colNames == ["1", "col2", "3", "2", "4", "5"]

# check that you get same column either by name or by index
c4 = t.column("2")
c4b = t.column(3) # index is base 0
print(c4b.name(),c4.name())
assert c4 is c4b

# set column 4 as X (X2).
c4.setPlotDesignation("X")
c5 = t.column("4")
c5.setPlotDesignation("Y")
c6 = t.column("5")
c6.setPlotDesignation("Y")


# DateTime data
c4.setColumnMode("DateTime")
dt1 = QtCore.QDateTime(2018,5,4,10,12,23,300)
for i in range(16):
    c4.setDateTimeAt(i,dt1)
    dt1 = dt1.addSecs(60*60*6) # add 6 hours

# print(c4.columnFormat())
assert c4.columnFormat() == "yyyy-MM-dd hh:mm:ss.zzz"
c4.setColumnFormat('yyyy-MM-dd hh:mm')
assert t.columnFormat(3) == c4.columnFormat()

# print(c4.dateTimeAt(8))
assert c4.dateTimeAt(8).toString(QtCore.Qt.ISODate) == "2018-05-06T10:12:23"

# check rowCount insertRows
assert c4.rowCount() == 16

# 16+3 < 31
c4.insertRows(3,3)
assert t.numRows() == 31
assert c4.rowCount() == 19

# 19+15 > 31
c4.insertRows(10,15)
assert c4.rowCount() == 34
assert t.numRows() == 34
assert c1.rowCount() == 31

# Date data
c5.setColumnMode("Month")
dt1 = QtCore.QDate(2018,5,4)
assert c5.columnFormat() == "MMMM"

c5.setDateAt(2,QtCore.QDate(2018,11,20))
c5.setDateAt(3,QtCore.QDate(2018,5,4))
# possible formats for 'Month': ['M', 'MM', 'MMM', 'MMMM']
c5.setColumnFormat("MMM")
assert c5.dateAt(2).toString(QtCore.Qt.ISODate) == "2018-11-20"

c6.setColumnMode("Day")
# print(c6.columnMode())
assert c6.columnMode() == "Day"
# possible formats for 'Day': ['d', 'dd', 'ddd', 'dddd']
# print(c6.columnFormat())
assert c6.columnFormat() == "dddd"
c6.setColumnFormat('ddd')

c6.setDateAt(2,QtCore.QDate(2018,11,20))
c6.setDateAt(3,QtCore.QDate(2018,5,4))
dt1 = QtCore.QDateTime(2018,5,4,10,12,23,300)
c6.setTimeAt(3,dt1.time())
c6.setTimeAt(4,dt1.addSecs(45).time())

assert c6.timeAt(3).toString() == "10:12:23"
assert c6.timeAt(4).toString() == "10:13:08"

# replaceDateTimes(int, list of QDateTime values)
dt1 = QtCore.QDateTime(2018,6,4,10,12,23,300)
dtlist = []

for i in range(8):
    dtlist.append(dt1)
    dt1 = dt1.addSecs(60*60) # add 1 hour

# only valid values are replaced
c4.replaceDateTimes(21,dtlist)

# x() and y()
# check that X-column of column("3") is column("1")
assert c3.x() == c1
# check that X-column of column("4") is column("2")
assert c5.x() == c4

# look for Y-column of column("4")
print("column(4).y().name: "+c4.y().name())
print("Column 4, named '"+c4.name()+"' is a '"+c4.plotDesignation()+"' column, whose 'y' is named '"+
    c4.y().name()+"' and is a '"+c4.y().plotDesignation()+"' column.")

# same for column("1")
print("column(3).x().name: "+c3.x().name())
print("Column 3, named '"+c3.name()+"' is a '"+c3.plotDesignation()+"' column, whose 'x' is named '"+
    c3.x().name()+"' and is a '"+c3.x().plotDesignation()+"' column.")

app.exit()
