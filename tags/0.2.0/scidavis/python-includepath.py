#!/usr/bin/python
from distutils import sysconfig
from PyQt4 import pyqtconfig
config = pyqtconfig.Configuration()
print " ".join([sysconfig.get_python_inc(), config.sip_inc_dir])
