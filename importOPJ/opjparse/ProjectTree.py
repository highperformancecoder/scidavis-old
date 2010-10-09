#!/usr/bin/env python

##    Copyright (C) 2010 Miquel Garriga
##    This file is part of opjparse.
##
##    opjparse is free software: you can redistribute it and/or modify
##    it under the terms of the GNU General Public License as published by
##    the Free Software Foundation, either version 3 of the License, or
##    (at your option) any later version.
##
##    opjparse is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##    GNU General Public License for more details.
##
##    You should have received a copy of the GNU General Public License
##    along with opjparse.  If not, see <http://www.gnu.org/licenses/>.
##
##    File: ProjectTree.py

import struct
import sys

class OPJProjectTree(object):
    """
    The project tree has two preambles and the root folder
    with files and folders
    """
    root = None
    
    def show(self):
        self.root.show()

        return

class OPJFolderEntry(object):
    """
    A folder has a header 
    """
    
    def show(self):
        print "Folder: %s" % self.name,
        
        print "has %d files:" % len(self.files)
        for x in self.files:
            x.show()
        print "and %d folders:" % len(self.folders)
        for x in self.folders:
            x.show()
    
class OPJObjectEntry(object):
    """ 
    For non-folder items in project tree 
    """
    
    def show(self):
        print "name    : %s" % self.name
        print "oid     : %s" % self.oid
    
