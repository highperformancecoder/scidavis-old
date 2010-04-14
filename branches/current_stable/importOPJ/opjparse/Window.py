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
##    File: Window.py
##    
##    Parse header of a Window.
##    Many details of the parsing process where obtained from liborigin1
##    http://sourceforge.net/projects/liborigin/
##    and liborigin2 http://soft.proindependent.com/liborigin2/

import struct
import time
import utils

class OPJWindow(object):
    window_name = None
    
    def show(self):
        print "Window     : %s." % self.window_name
        print "rect       : TL (%3d,%3d) BR (%3d,%3d) XT (%d,%d)" % (self.w_left, self.w_top,
                                                                     self.w_right, self.w_bottom,
                                                                     self.w_page_width, self.w_page_height)
        print "type       : %d 0x%X %s" % (self.w_type, self.w_type, self.w_type_s)
        print "kind       : %s"      % self.w_kind
        print "title_type : %d 0x%X" % (self.w_title_type, self.w_title_type)
        print "state      : %d 0x%X" % (self.w_state, self.w_state)
        if (hasattr(self, 'cdate')):
            print "cdate      : %.2f %s" % (self.w_cdate, self.w_cdate_s)
            print "mdate      : %.2f %s" % (self.w_mdate, self.w_mdate_s)
        if (hasattr(self, 'all_lines')):
            print "%d lines EXTRA" % len(self.all_lines)
        return

    def parse(self):
        
        weh_data = self.hd_data
        self.window_name = weh_data[0x02:0x1B] # 25 char string
        self.window_name = self.window_name.partition('\0')[0].strip()
    
        (self.w_left, self.w_top,
        self.w_right, self.w_bottom,
        self.w_page_width, self.w_page_height) = struct.unpack("<6H",weh_data[0x1B:0x27])
        (self.w_type,) = struct.unpack("<B",weh_data[0x32:0x33])
        self.w_kind = weh_data[0x45:0x4F]
        self.w_kind = self.w_kind.partition('\0')[0].strip()
        try:
            (self.w_title_type, self.w_state) = struct.unpack("<BB",weh_data[0x69:0x6B])
        except struct.error:
            pass
        
        if (self.w_type < 0x18):
            self.w_type_s = 'Graph'
        elif (self.w_type < 0x50):
            self.w_type_s = 'Layout'
        elif (self.w_type < 0x54):
            self.w_type_s = 'Worksheet'
        else:
            self.w_type_s = 'Matrix'

        # creation and modification times
        try: # version < 6.0 does not have these
            (self.w_cdate, self.w_mdate) = struct.unpack("<2d",weh_data[0x73:0x83])
            self.w_cdate_s = time.strftime('%F %T',time.localtime(utils.jdt2unixtime(self.w_cdate)))
            self.w_mdate_s = time.strftime('%F %T',time.localtime(utils.jdt2unixtime(self.w_mdate)))
        except struct.error:
            pass
        
        
        if (len(weh_data) > 0xC3):
            self.all_lines = weh_data[0xC3:].splitlines()
        
        return 
    
if (__name__ == '__main__'):
    import sys
    if (len(sys.argv) < 2):
        print "Window weh-nnn"
        sys.exit()
    
    this_window = OPJWindow()
    with open(sys.argv[1]) as hdf:
        this_window.hd_data = hdf.read()
    this_window.parse()
    this_window.show()
