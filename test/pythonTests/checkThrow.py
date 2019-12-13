def checkThrow(expr):
    try:
        eval(expr)
    except:
        return
    raise Exception(expr+" didn't throw")

# if run on it's own, rather than imported, do a test of this module
if __name__=="scidavis" or __name__=="scidavisUtil":
    checkThrow("raise Exception('hello')")
    try:
        checkThrow("x=1")
        assert False, "Checkthrow didn't throw"
    except:
        pass
    app.exit()
