# Script to test the subsections of:
# "API documentation" of the handbook
# available at: https://highperformancecoder.github.io/scidavis-handbook/sec-python.html

## subsection: class Table (inherits MDIWindow)

import random
random.seed(1)

tName = "Table01"
t=newTable(tName,2,30)
t.confirmClose(False)

assert t.numRows() == 30
assert t.numCols() == 2
t.setNumRows(15)
t.setNumCols(4)
assert t.numRows() == 15
assert t.numCols() == 4
c1=t.column(0) # test if it works with the syntax column(int) - 0-based index
assert c1.name() == "1"
c2=t.column("2") # test if it works with the syntax column(string)

## **COMMENT: many commands marked as DEPRECATED in the manual still working (and are very
## useful for *me* :-) . Then I will test all them. Please, see the handbook why the commands
## are marked as DEPRECATED.

assert t.text(3,1) == "" # DEPRECATED - Notice that indexes in text() function are 1-based
c3=t.column(2)
c3.setColumnMode(ColumnMode.Text)
#t.setText("3",1,"pi") # DEPRECATED
t.setText(1,3, "pi")
assert t.text(1,3) == "pi"

assert t.cell(1,1) == 0.0 # DEPRECATED - 1-based indexes
a=random.uniform(0,1.0)
t.setCell(1,1,a) # DEPRECATED
assert t.cell(1,1) == a

assert t.colName(2) == "Table01_3" # DEPRECATED - 1-based indexes
t.setColName(2,"two")  # DEPRECATED
print(t.colName(2))
assert t.colName(2) == "Table01_two"

t.setColComment(2,"column named as two") # DEPRECATED - there are no a 'comment()' function 
# that could be used to verify if setComment() works. But it works - visual check
assert t.colComment(2)=="column named as two"

t.setCommand(2, "col(1)*col(1)") # Here also doesn't have a 'command()' function for automated
# verification. But it works - visual check + apply button

# notifyChanges() # this is really DEPRECATED and unnecessary to be tested

# Since I don't know how Travis CI works, I will not test:
# importASCII(string, string, int, bool, bool, bool)
# exportASCII(string, string, bool, bool)
# but these commands are very important. Then I suggest that someone improve this script.

# the functions normalize(string or int) and normalize() was already tested in the
# "Working_with_tables.py" script

# first, create an empty table and fill columns with random values
t2=newTable(tName+"new1",2,30)
t2.confirmClose(False)
for i in range(1,t2.numRows()+1):
  t2.setCell(i,1,random.uniform(0,1.0))
  t2.setCell(i,2,random.uniform(3.0,1.0))
t2.sortColumn("1",Table.ascending) #sort 1st col ascending
t2.sortColumn("2",Table.descending) #sort 2nd col descending
  
assert t2.cell(1,1) <= t2.cell(2,1)
assert t2.cell(1,2) >= t2.cell(2,2)

t3=newTable(tName+"new2",3,30)
t3.confirmClose(False)
for i in range(1,t3.numRows()+1):
  t3.setCell(i,2,random.uniform(0,1.0))
  t3.setCell(i,2,random.uniform(3.0,1.0))

t3.sort(Table.separate,Table.ascending,"") # sort all columns ascending, separately
assert t3.cell(1,1) <= t3.cell(2,1)
assert t3.cell(1,2) <= t3.cell(2,2)

t3.sort(Table.separate,Table.descending,"") #sort all columns descending, separately
assert t3.cell(1,1) >= t3.cell(2,1)
assert t3.cell(1,2) >= t3.cell(2,2)

for j in range(1,t3.numRows()+1):
  t3.setCell(j,1,random.uniform(0,1.0))
  t3.setCell(j,3,j)

refVal1=t3.cell(1,1) # Reference value for check
refVal2=t3.cell(30,1) # idem
t3.sort(Table.together,Table.ascending,"1") # sort all columns, together, using col(1) as the leading one, ascending
for l in range(1,t3.numRows()+1):
  if t3.cell(3,l) == 1:
    assert t3.cell(1,l) == refVal1
  else:
    if t3.cell(1,3) == 30:
      assert t3.cell(1,l) == refVal2

for j in range(1,t3.numRows()+1):
  t3.setCell(j,2,random.uniform(3.0,1.0))
  t3.setCell(j,3,j)

refVal3=t3.cell(2,1) # Reference value for check
refVal4=t3.cell(30,2) # idem
t3.sortColumns(("2","3"),Table.together,Table.ascending,"2") #sort columns 2 and 3 together, using col(2) as the leading one, ascending
for l in range(1,30+1):
  if t3.cell(3,l) == 1:
    assert t3.cell(2,l) == refVal3
  else:
    if t3.cell(3,l) == 30:
      assert t3.cell(2,l) == refVal4

t3.sortColumns(("1","3"),Table.separate,Table.descending,"2") #sort columns 1 and 3, separately, descending
assert t3.cell(1,1) >= t3.cell(2,1)
assert t3.cell(1,3) >= t3.cell(2,3)

# test massive table creation and removal
ntables = 15
createlist = list(range(1,ntables))
closelist = createlist[:]
random.shuffle(closelist)

for i in createlist:
 t=newTable("Table" + str(i))

for j in closelist:
 t=table("Table" + str(j))
 t.confirmClose(False)
 t.close()

app.exit()
