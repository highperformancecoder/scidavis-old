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

# Commands rowCount, insertRows and removeRows are not working (tested in version 1.22):
# rowCount always returns 0, insertRows and removeRows don't add or remove any row.
# Of course it makes sense because if one try to change the number of rows of a column, the other
# rows on the table must also be changed. Moreover, these commands are already given by t.numRows
# and t.setNumRows...
# This suggests that the mentioned column commands can be removed from scidavis.sip (and from 
# the manual too) in the future.

# fill c1 with row random values
for j in range(1,nRows+1):
  t.setCell(1,j,random.uniform(0,1.0))
c2.copy(c1)
for j in range(1,nRows+1):
  assert t.cell(2,j) == t.cell(1,j)

c3.copy(c1,0,10,10)
for k in range(1,11):
  assert t.cell(3,k+10) == t.cell(1,k)
# I guess that there is an inconsistency in the command "copy(Column,int,int,int)"... It works when it
# is called using 'pure python' indexation for vectors, that is, starting from zero, not in the same
# way as column indexing works on SciDAVis,starting from 1.

assert c1.plotDesignation() == "X"
assert c2.plotDesignation() == "Y"
c3.setPlotDesignation("yErr")
assert c3.plotDesignation() == "yErr"

for i in range(0,nRows+1):
  assert c1.isInvalid(i) == False
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
  t.setText(3,k,"a")

assert c3.textAt(1) == "a"
c3.setTextAt(1,"b")
assert c3.textAt(1) == "b"
# 0-based index for textAt and setTextAt too

v2 = ["q","w","e","r","t"]
c3.replaceTexts(2,v2)
for l in range(2,2+len(v2)-1):
  assert c3.textAt(l) == v2[l-2] 

# I don't know how to test the following column commands:
# setDateAt(int, QDate)
# timeAt(int)
# setTimeAt(int, QTime)
# dateTimeAt(int)
# setDateTimeAt(int, QDateTime)
# replaceDateTimes(int, list of QDateTime values)

# the last two commands of column class:
# x() and y()
# will return something like <scidavis.Column object at 0x7fb09993b938>
# Then I'll not thest them..
app.exit()
