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
##    File: Layer.py

import struct

LayerTypes = {
              0x01: 'Unknown',
              0x05: 'Unknown',
              0x07: 'Unknown',
              0x0F: 'Empty', 
              0x17: 'Function Graph', 
              0x1F: 'Graph',
              0x5E: 'Worksheet', 
              0x5F: 'Complex Graph', 
              0x7E: 'Excel', 
              0xDE: 'Matrix'
             }

class OPJLayer(object):
    layer_name = None
    
    def show(self):
        print "Layer      : %s " % self.layer_name
        print "type       : %3d 0x%2X %s" % (self.layer_type,self.layer_type, LayerTypes[self.layer_type])
        print "x-range    : %g %g %g" % (self.xmin,self.xmax, self.xstep)
        print "y-range    : %g %g %g" % (self.ymin,self.ymax, self.ystep)
        print "u          : %g" % self.umin

        return
    
    def show_long(self):
        self.show()
        for x in self.sublayers:
            x.show()
        for x in self.curves:
            x.show()
        for x in self.axisbreaks:
            x.show()
        for x in self.xaxis:
            x.show()
        for x in self.yaxis:
            x.show()
        for x in self.zaxis:
            x.show()

        return

    def parse(self):
        leh_data = self.hd_data
        leh_data_size = len(leh_data)
        (self.layer_type,) = struct.unpack("<B",leh_data[0x02:0x03])
        self.layer_name = leh_data[0xD2:0xF7]
        self.layer_name = self.layer_name.partition('\0')[0].strip()
        (self.xmin,self.xmax,self.xstep) = struct.unpack("<ddd",leh_data[0x0F:0x27])
        (self.umin,) = struct.unpack("<d",leh_data[0x2F:0x37])
        (self.ymin,self.ymax,self.ystep) = struct.unpack("<ddd",leh_data[0x3A:0x52])
        return
    
if (__name__ == '__main__'):
    import sys
    if (len(sys.argv) < 2):
        print "Layer leh-nnn"
        sys.exit()
    
    this_layer = OPJLayer()
    with open(sys.argv[1]) as hdf:
        this_layer.hd_data = hdf.read()
    this_layer.parse()
    this_layer.show()
