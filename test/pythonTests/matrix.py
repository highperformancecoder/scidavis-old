m=newMatrix("m",2,2)

m.setRow(0,0,[2,3]);
m.setRow(1,0,[1,3]);
assert m.determinant()==3,"m.determinant()"

m.transpose()
assert m.cell(1,2)==1,"m.cell(1,2)==1"
assert m.cell(2,1)==3,"m.cell(2,1)==3"
m.transpose()

m1=newMatrix("m1",2,2)

m1.setRow(0,0,[2,3]);
m1.setRow(1,0,[1,3]);
m1.invert()

for i in range(2):
    for j in range(2):
        s=0
        for k in range(2):
            s+=m.cell(i+1,k+1)*m1.cell(k+1,j+1)
        assert s == (i==j),str(s)+" == ("+str(i)+"=="+str(j)+")"

app.exit()
