#!/usr/bin/python
from PyQt4 import pyqtconfig
config = pyqtconfig.Configuration()
print " ".join([config.sip_bin, "-I", config.pyqt_sip_dir, config.pyqt_sip_flags])
