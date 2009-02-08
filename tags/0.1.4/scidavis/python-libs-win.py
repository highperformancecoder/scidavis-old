#!/usr/bin/python
from PyQt4 import pyqtconfig
config = pyqtconfig.Configuration()

print "-L%s -lpython%d%d" % (config.py_lib_dir, config.py_version >> 16, (config.py_version  >> 8) & 0xff)
