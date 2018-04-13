t1=app.newTable()
assert t1.name()!="xxx"
t1.setName("xxx")
assert t1.name()=="xxx"
app.exit()
