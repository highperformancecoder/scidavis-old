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
##    File: SubLayer.py
##
##    Parse header of data blocks of a SubLayer.
##    Many details of the parsing process where obtained from liborigin1
##    http://sourceforge.net/projects/liborigin/
##    and liborigin2 http://soft.proindependent.com/liborigin2/

import struct

class OPJSubLayer(object):
    sublayer_name = None
    
    def show(self):
        print "Sublayer   : %s" % self.sublayer_name
        print "kind       : %d" % self.sl_kind
        print "type       : %d" % self.sl_type
        print "rect       : TL (%3d,%3d) BR (%3d,%3d)" % (self.sl_left, self.sl_top,
                                                          self.sl_right, self.sl_bottom)
        print "color      : %d" % self.sl_color
        print "attach     : %d" % self.sl_attach
        print "border     : %d" % self.sl_border
        print " "
        
        return

    def parse(self):
        slh_data = self.hd_data
        sb1_data = self.block1_data
        sb2_data = self.block2_data
        sb3_data = self.block3_data
        
        (self.sl_kind,self.sl_left, self.sl_top,
        self.sl_right, self.sl_bottom) = struct.unpack("<B4H",slh_data[0x02:0x0B])
        (self.sl_attach,self.sl_border) = struct.unpack("<BB",slh_data[0x28:0x2A])
        (self.sl_color,) = struct.unpack("<I",slh_data[0x33:0x37])
        self.sublayer_name = slh_data[0x46:0x5E]
        self.sublayer_name = self.sublayer_name.partition('\0')[0].strip()
        
        (self.sl_type,) = struct.unpack("<B",sb1_data[0x00:0x01])
        (self.sl_rotation, self.sl_fontsize, self.sl_fig_width) = struct.unpack("<HBH",sb1_data[0x02:0x07])
        (self.sl_fig_style,) = struct.unpack("<B",sb1_data[0x08:0x09])
        try: # version < 6.1 does not have this
            (self.sl_tab,) = struct.unpack("<B",sb1_data[0x0A:0x0B])
        except struct.error:
            pass
        self.command={}
        # see slh_info.txt for possible values
        if (len(sb1_data) > 0x14):
            (self.sl_linestyle, self.sl_width) = struct.unpack("<BH",sb1_data[0x12:0x15])
        if (len(sb1_data) > 0x2F): 
            (self.sl_beg_x, self.sl_end_x) = struct.unpack("<dd",sb1_data[0x20:0x30])
            
        if (len(sb1_data) > 0x4F):
            (self.sl_beg_y, self.sl_end_y) = struct.unpack("<dd",sb1_data[0x40:0x50])
            (self.sl_fig_fillarea_color,
             self.sl_fig_fillarea_pattern_width) = struct.unpack("<IH",sb1_data[0x42:0x48])
            (self.sl_fig_fillarea_pattern_color,
             self.sl_fig_fillarea_pattern) = struct.unpack("<IB",sb1_data[0x4A:0x4F])
            
        if (len(sb1_data) > 0x77):
            (self.sl_use_border_color,) = struct.unpack("<B",sb1_data[0x57:0x58])
            (self.sl_beg_shape_type,) = struct.unpack("<B",sb1_data[0x60:0x61])
            (self.sl_beg_shape_width, 
             self.sl_beg_shape_length,
             self.sl_end_shape_type) = struct.unpack("<IIB",sb1_data[0x64:0x6D])
            (self.sl_end_shape_width,
             self.sl_end_shape_length) = struct.unpack("<II",sb1_data[0x70:0x78])
        
        if (self.sublayer_name in ["XB","XT","YL","YR","ZF","ZB","Legend","legend","label"]):
            assert(self.sl_kind==0)
            self.label = self.block2_data.partition('\0')[0].strip()
        elif (self.sublayer_name in  ["3D","ZCOLORS","3DCOLOR","COLORMAP","SPECTRUM1",
                                      "RADIO","IMAGE1"]):
            assert(self.sl_kind==35)
        elif (self.sublayer_name in  ["h_readout","v_readout","Intersect","cntrl"]):
            assert(self.sl_kind==35)
        elif (self.sublayer_name in ["__BCO2","__WIOTN","__WIPR","__LayerInfoStorage", 
                                     "__LayerGridStyle","TemplateEMF"]):
            assert(self.sl_kind==35)
        elif (self.sublayer_name in ["_190","_202","_203","_206","_219","_220","_226",
                                     "_231","_232","_240","_242"]):
            assert(self.sl_kind==7)
        elif ("#Text" in '#'+self.sublayer_name):
            assert(self.sl_kind==0)
        elif ("#Rect" in '#'+self.sublayer_name):
            assert(
                   self.sl_kind==33
                   or 
                   self.sl_kind==1 # only one OPJ
                   or 
                   self.sl_kind==49 # only one OPJ
                  )
        elif ("#Circle" in '#'+self.sublayer_name):
            assert(self.sl_kind==33)
        elif (self.sublayer_name in  ["sline","hline","vline"]):
            assert(self.sl_kind==34)
        elif ("#Arrow" in '#'+self.sublayer_name):
            assert(
                   self.sl_kind==34
                   or
                   self.sl_kind==2 # only one OPJ
                  )
        elif ("#Line" in '#'+self.sublayer_name):
            assert(
                   self.sl_kind==34
                   or
                   self.sl_kind==2 # in complex Graphs
                  )
        elif ("#BMP" in '#'+self.sublayer_name):
            assert(self.sl_kind==35)
        elif ("#EMF" in '#'+self.sublayer_name):
            assert(self.sl_kind==35)
        elif ("#WMF" in '#'+self.sublayer_name):
            assert(self.sl_kind==35)
        elif ("#Polygon" in '#'+self.sublayer_name):
            assert(self.sl_kind==35)
        elif ("#Polyline" in '#'+self.sublayer_name):
            assert(self.sl_kind==35)
        elif (self.sl_kind==6):
            self.command[self.sublayer_name]=self.block1_data.partition('\0')[0].strip()            
        else:
            print "Unknown Sublayer name: %s kind %d " % (self.sublayer_name, self.sl_kind)
        return
    
if (__name__ == '__main__'):
    import sys
    if (len(sys.argv) < 5):
        print "SubLayer slh-nnn eb1d-nnn eb2d-nnn eb3d-nnn"
        sys.exit()
    
    this_sublayer = OPJSubLayer()
    
    with open(sys.argv[1]) as hdf:
        this_sublayer.hd_data = hdf.read()
        
    with open(sys.argv[2]) as ed1f:
        this_sublayer.block1_data = ed1f.read()
        
    with open(sys.argv[3]) as ed2f:
        this_sublayer.block2_data = ed2f.read()
        
    with open(sys.argv[4]) as ed3f:
        this_sublayer.block3_data = ed3f.read()
    
    this_sublayer.parse()
    this_sublayer.show()
