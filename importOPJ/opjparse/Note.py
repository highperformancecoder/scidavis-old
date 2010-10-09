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
##    File: Note.py

import struct

class OPJNote(object):
    note_name = None
    
    def show(self):
        print "Note    : %s" % self.note_name
        print "contents: %s" % self.note_contents

        return

    def parse(self):
        nthd_data = self.hd_data
        ntlb_data = self.lb_data
        ntct_data = self.ct_data
        
        self.note_name = ntlb_data.partition('\0')[0].strip()
        self.note_contents = ntct_data.partition('\0')[0].strip()
        return

if (__name__ == '__main__'):
    import sys
    if (len(sys.argv) < 4):
        print "Note neh-nnn nld-nnn ncd-nnn "
        sys.exit()
    
    this_note = OPJNote()
    
    with open(sys.argv[1]) as hdf:
        this_note.hd_data = hdf.read()
        
    with open(sys.argv[2]) as lbf:
        this_note.lb_data = lbf.read()
        
    with open(sys.argv[3]) as ctf:
        this_note.ct_data = ctf.read()
    
    this_note.parse()
    this_note.show()
