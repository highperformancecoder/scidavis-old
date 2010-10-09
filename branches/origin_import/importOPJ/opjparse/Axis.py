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
##    File: Axis.py

import struct

class OPJAxisBreak(object):
    def show(self):
        print "AxisBreak: "
        print "from : ", self.ab_from
        print "to   : ", self.ab_to
        print "pos  : ", self.ab_position
        return
    
    def parse(self):
        ab_data = self.dt_data
        (self.ab_from, self.ab_to, 
         self.ab_incrementafter, self.ab_position) = struct.unpack("<4d",ab_data[0x0B:0x2B])
        (self.ab_logafter, self.ab_minorticsafter) = struct.unpack("<BB",ab_data[0x2B:0x2D])
        return
        
class OPJAxisParameter(object):
    def show(self):
        print "AxisParameter, size: ", self.width
        return
    
    def parse(self):
        ap_data = self.dt_data
        # CHECK: grid.style uses color last byte ([3]) ?
        (self.color,) = struct.unpack('<I', ap_data[0x0F:0x13])
        # position 0x15:0x17 is used both for fontsize or linewidth 
        (self.rotation,self.fontsize) = struct.unpack('<HH', ap_data[0x13:0x17])
        self.rotation = self.rotation/10.
        self.width = self.fontsize/500.
        (self.fontbold,) = struct.unpack('<B', ap_data[0x1A:0x1B]) 
        self.fontbold = self.fontbold & 0x08
        (self.dataset_id, tmpbyte) = struct.unpack('<HB', ap_data[0x23:0x26])
         # CHECK, liborigin 1 uses ap_data[0x26] for both hidden and value_type_spec
        (self.hidden,) = struct.unpack('<B', ap_data[0x26:0x27])
        h1 = self.hidden
        # axis format parameters
        self.minorTicksType = tmpbyte >> 6
        self.majorTicksType = (tmpbyte >> 4) & 0x03
        self.axisPosition = (tmpbyte & 0x0F)
        if (self.axisPosition == 1):
            self.axisPositionValue = struct.unpack('<B',ap_data[0x37:0x38])
        elif (self.axisPosition == 2):
            self.axisPositionValue = struct.unpack('<d',ap_data[0x2F:0x37])
        (self.majorTickLength,) = struct.unpack('<H',ap_data[0x4A:0x4C])
        # tick labels parameters
        self.value_type = tmpbyte & 0x0F
        if (self.value_type == 0): # Numeric
            if ((tmpbyte>>4) > 7):
                self.valueTypeSpecification = (tmpbyte>>4) - 8
                self.decimalPlaces = h1 - 0x40
            else:
                self.valueTypeSpecification = (tmpbyte>>4)
                self.decimalPlaces = -1
        elif ((self.value_type > 1) and (self.value_type<7)): # Time, Date, Month, Day, ColumnHeading
            self.valueTypeSpecification =  h1 - 0x40
        elif ((self.value_type == 1) or # Text
              (self.value_type == 7) or # TickIndexedDataset
              (self.value_type == 10)): # Categorical
            # we have already read self.dataset_id, names are assigned later
            pass
        elif (self.value_type == 9): # TextNumeric
            self.value_type = 0 # make it Numeric
            self.valueTypeSpecification = 0
        # grid parameters
        # CHECK: grid.style uses color last byte ([3]) according to readGraphGridInfo
        # TO BE COMPLETED

        return
