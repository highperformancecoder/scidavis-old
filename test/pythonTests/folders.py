r=app.rootFolder()
f=Folder("hello")
assert f.name()=="hello"
r.addChild(f)
assert f.rootFolder().name()==r.name()
assert r.rootFolder().name()==r.name()
assert len(r.folders())==1
assert r.folders()[0].name()==f.name()
assert r.folder("hello").name()==f.name()
t=app.newTable("t1")
g=app.newGraph("g1")
n=app.newNote("n1")
m=app.newMatrix("m1")
assert r.table("t1").name()==t.name()
assert r.graph("g1").name()==g.name()
assert r.note("n1").name()==n.name()
assert r.matrix("m1").name()==m.name()
t1=r.findWindow("t1")
assert t1.name()==t.name()

windows=r.windows()
assert t in windows
assert g in windows
assert n in windows
assert m in windows
assert len(windows)>=4
# smoke test the save method
f.save("foo")
app.exit()
