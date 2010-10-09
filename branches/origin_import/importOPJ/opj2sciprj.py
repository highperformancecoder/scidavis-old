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

def addFolder(opj_folder):
   " add a folder and its subtree "
   parent_folder = activeFolder()        # get active Folder
   sci_folder = Folder(opj_folder.name)  # create new one
   parent_folder.addChild(sci_folder)    # add it to parent
   app.setActiveFolder(sci_folder,True)  # make it the active Folder
   
   for node in opj_folder.files:
      addNode(node)                    # add the windows

   for subfolder in opj_folder.folders:  # add subfolders
      addFolder(subfolder)
   
   app.setActiveFolder(parent_folder)    # make parent active again
   return


def addNode(node):
   " add a window or a note "
   if node.oid < 0x100000:
      wd = af.windows[node.oid-1]
      if wd.w_type_s=='Worksheet':
         addWorksheet(wd)
   else:
      nid = node.oid - 0x100000
      name = af.notes[nid-1].note_name
      nt = newNote(name)
      nt.setText(af.notes[nid-1].note_contents)
   return

def addWorksheet(wd):
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
   return 

# use a Dialog to choose the File to be converted
FilePath = QtGui.QFileDialog.getOpenFileName(app, "Select Input File")

# for the script to work functions defined in it have to go into global namespace
# similar effect can be achieved by 
#   setattr(__main__, "addNode", addNode), ...
global addNode, addFolder, addWorksheet
# the same for the OPJ file object, used in those functions
# if we don't close the script, or in case of error, we can use the 
# scripting facilities of SciDAVis to inspect af, the OPJ file.
global af

af=opj.open(FilePath)

opj_rf=af.projecttree.root

# we will add the OPJ tree as a subfolder of root folder in SciDAVis tree
# because the name of a Folder cannot be changed via script in SciDAVis
addFolder(opj_rf)

# save only that subfolder tree
app.rootFolder().folder(opj_rf.name).save(FilePath+".sciprj")

# or alternatively close the Note window of this script
self.confirmClose(False)
self.close()
