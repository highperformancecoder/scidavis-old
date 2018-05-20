m=newMatrix("m",2,2)
m.confirmClose(False)

m.setRow(0,0,[2,3]);
m.setRow(1,0,[1,3]);
assert m.determinant()==3,"m.determinant()"

m.transpose()
assert m.cell(1,2)==1,"m.cell(1,2)==1"
assert m.cell(2,1)==3,"m.cell(2,1)==3"
m.transpose()

m1=newMatrix("m1",2,2)
m1.confirmClose(False)

m1.setRow(0,0,[2,3]);
m1.setRow(1,0,[1,3]);
m1.invert()

for i in range(2):
    for j in range(2):
        s=0
        for k in range(2):
            s+=m.cell(i+1,k+1)*m1.cell(k+1,j+1)
        assert s == (i==j),str(s)+" == ("+str(i)+"=="+str(j)+")"

M=newMatrix() #use defaults
M.confirmClose(False)
assert M.numRows() == M.numCols() == 32
nn=5
M.setNumRows(nn)
M.setNumCols(nn)
assert M.numRows() == M.numCols() == nn
nnn=10
M.setDimensions(nnn,nnn)
assert M.numRows() == M.numCols() == nnn

assert M.cell(1,1) == 0.0
M.setCell(1,1,pi)
assert M.cell(1,1) == pi

# I guess that string support in matrices was disabled at some point in SciDAVis.
# Of course, strings in matrices don't make sense for a data analysis software...
# Then, the following will not be tested
# text(row, col)
# setText(row, col, string)

assert M.xStart() == M.yStart() == 1 # 1 is the default start coordinate for matrices in SciDAVis
assert M.xEnd() == M.yEnd() == 10 # 1 is the default end coordinate...
M.setCoordinates(5.0,10.0,50.0,100.0)

M.setFormula("i+j") # I don't know how it works in practice. If one look to the Formula tab he will
# see the that the i+j formula was set correctly. But if click on apply button nothing happens.
# And the command M.recalculate() is crashing the app even if we don't use setFormula (I'll open a
# bug report about this issue)

M.setNumericPrecision(2)
m1.setNumericPrecision(3)
# there isn't a command to check if setNumericPrecision() works - visual check only -> OK

app.exit()
