r=app.rootFolder()
f=Folder("hello")
assert f.name()=="hello"
r.addChild(f)
assert f.rootFolder()==r
assert r.rootFolder()==r
assert r.folders()==[f]
assert r.folder("hello")==f
t=app.newTable("t1")
g=app.newGraph("g1")
n=app.newNote("n1")
m=app.newMatrix("m1")
assert r.table("t1")==t
assert r.graph("g1")==g
assert r.note("n1")==n
assert r.matrix("m1")==m
assert r.findWindow("t1")==t
windows=r.windows()
assert t in windows
assert g in windows
assert n in windows
assert m in windows
assert len(windows)>=4
# smoke test the save method
f.save("foo")
app.exit()
