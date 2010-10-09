#!/usr/bin/scidavis -x
############################################################################
#                                                                          #
# File                 : opj2sciprj.py                                      #
# Project              : SciDAVis                                          #
# Description          : Import script for OPJ files                       #
# Copyright            : (C) 2010 Miquel Garriga                           #
#                        (replace * with @ in the email address)           #
#                                                                          #
############################################################################
#                                                                          #
#  This program is free software; you can redistribute it and/or modify    #
#  it under the terms of the GNU General Public License as published by    #
#  the Free Software Foundation; either version 3 of the License, or       #
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

# usage:  scidavis -x opj2sciprj.py 
#
# opjparse has to be in the PYTHONPATH

try: 
   import opjparse as opj
except:
   try: # if opjparse is not in PYTHONPATH, use script absolute path to find opjparse
      import os, sys 
      sys.path.append(os.path.dirname(os.path.realpath(str(self.name()))))
      import opjparse as opj
   except:
      msg_return = QtGui.QMessageBox(QtGui.QMessageBox.Critical,"Import error",
         "opjparse is not in PYTHONPATH,\nPlease, check your configuration").exec_()
      self.confirmClose(False)
      QtCore.QCoreApplication.quit()

# use a Dialog to choose the File to be converted
FilePath = QtGui.QFileDialog.getOpenFileName(app, "Select Input File")

af=opj.open(FilePath)
# Windows (Worksheets,Matrices,Graphs,...)
for wd in af.windows:
   # Worksheets
   if wd.w_type_s=='Worksheet':
      tb = newTable(wd.window_name)
      tb.setNumCols(len(wd.curves))
      for (i,cv) in enumerate(wd.curves):
         # set column name
         tb.column(i).setName(cv.curve_name)
         # set values
         ds = af.datasets[cv.curve_oid-1]
         if ds.data_mode=="Text":
            tb.column(i).setColumnMode("Text")
            ds_str = map(str,ds.data_values)
            tb.column(i).replaceTexts(0, ds_str)
         elif ds.data_mode=="Numeric":
            tb.column(i).replaceValues(0, ds.data_values)
         elif ds.data_mode=="DateTime":
            tb.column(i).setColumnMode("DateTime")
            tb.column(i).replaceDateTimes(0, ds.data_values)
   # Matrices
   # Functions
   # Graphs

# Notes
for nt in af.notes:
   nw = newNote(nt.note_name)
   nw.setText(nt.note_contents)

app.rootFolder().save(FilePath+".sciprj")

# or alternatively close the Note window of this script
self.confirmClose(False)
self.close()
