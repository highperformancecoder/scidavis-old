#!/usr/bin/python
from PyQt4 import pyqtconfig
config = pyqtconfig.Configuration()
flags = []
if config.sip_version >= 0x040a00:
	# make use of docstring generation feature in SIP >= 4.10
	flags.append("-o")
print " ".join([config.sip_bin, "-I", config.pyqt_sip_dir, config.pyqt_sip_flags] + flags)
