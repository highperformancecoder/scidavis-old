############################################################################
#                                                                          #
# File                 : python-sipcmd.py                                  #
# Project              : SciDAVis                                          #
# Description          : Auto-detect SIP/PyQt location; dump commandline   #
# Copyright            : (C) 2007-2010 Knut Franke (knut.franke*gmx.de)    #
#                        (replace * with @ in the email address)           #
#                                                                          #
############################################################################
#                                                                          #
#  This program is free software; you can redistribute it and/or modify    #
#  it under the terms of the GNU General Public License as published by    #
#  the Free Software Foundation; either version 2 of the License, or       #
#  (at your option) any later version.                                     #
#                                                                          #
#  This program is distributed in the hope that it will be useful,         #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of          #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           #
#  GNU General Public License for more details.                            #
#                                                                          #
#   You should have received a copy of the GNU General Public License      #
#   along with this program; if not, write to the Free Software            #
#   Foundation, Inc., 51 Franklin Street, Fifth Floor,                     #
#   Boston, MA  02110-1301  USA                                            #
#                                                                          #
############################################################################


import os
import warnings
import sys, sipconfig
config = sipconfig.Configuration()

try:
    pyqt = sys.argv[1]
except IndexError:
    pyqt = 'PyQt4'

if (pyqt not in ['PyQt4','PyQt5']):
    pyqt = 'PyQt4'

try:
    exec("from "+pyqt+".QtCore import PYQT_CONFIGURATION")
except ImportError:
    pass

sipBin = config.sip_bin
sipDir = config.default_sip_dir+'/'+pyqt
if sys.version_info[0] >= 3 and sys.version_info[1] >= 7 and pyqt == 'PyQt5':
    from importlib.metadata import distribution
    dist = distribution(pyqt)
    sip = [p for p in dist.files if p.name == 'QtCoremod.sip']
    assert len(sip) == 1
    sipDir = str(dist.locate_file(sip[0]).parent.parent)
else:
    if not os.path.exists(sipDir):
        warnings.warn('sipDir does not exists, %s' % sipDir)

sipFlags =  PYQT_CONFIGURATION['sip_flags']

flags = ["-I ../scidavis"]
if config.sip_version >= 0x040a00:
	# make use of docstring generation feature in SIP >= 4.10
	flags.append("-o")

sys.stdout.write(" ".join([sipBin, '-I', sipDir, sipFlags]+flags))
