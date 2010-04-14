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
##    File: Curve.py

import struct

CurveTypes={0x01: 'G', 0x10: 'W'}

ValueTypes={
            0x00: 'Numeric - Dec1000',
            0x09: 'Text&Numeric - Dec1000',
            0x10: 'Numeric - Scientific',
            0x19: 'Text&Numeric - Scientific',
            0x20: 'Numeric - Engeneering',
            0x29: 'Text&Numeric - Engeneering',
            0x30: 'Numeric - Dec1,000',
            0x39: 'Text&Numeric - Dec1,000',
            0x02: 'Time',
            0x03: 'Date',
            0x04: 'Month',
            0x34: 'Month', # locale ??
            0x05: 'Day of week',
            0x35: 'Day of week', # locale ??
            0x31: 'Text'
           }

GraphTypes={
            200: 'Line',
            201: 'Scatter',
            202: 'LineSymbol',
            203: 'Column',
            204: 'Area',
            205: 'HiLoClose',
            206: 'Box',
            207: 'ColumnFloat',
            208: 'Vector',
            209: 'PlotDot',
            210: 'Wall3D',
            211: 'Ribbon3D',
            212: 'Bar3D',
            213: 'ColumnStack',
            214: 'AreaStack',
            215: 'Bar',
            216: 'BarStack',
            218: 'FlowVector',
            219: 'Histogram',
            220: 'MatrixImage',
            225: 'Pie',
            226: 'Contour',
            230: 'Unknown',
            231: 'ErrorBar',
            232: 'TextPlot',
            233: 'XErrorBar',
            236: 'SurfaceColorMap',
            237: 'SurfaceColorFill',
            238: 'SurfaceWireframe',
            239: 'SurfaceBars',
            240: 'Line3D',
            241: 'Text3D',
            242: 'Mesh3D',
            245: 'XYZTriangular',
            246: 'LineSeries',
            254: 'YErrorBar',
            255: 'XYErrorBar',
            0x8AF0: 'GraphScatter3D',
            0x8AF1: 'GraphTrajectory3D',
            0x00020000: 'Polar',
            0x00040000: 'SmithChart',
            0x00800000: 'FillArea'
           }


class OPJCurve(object):
    curve_name = None
    
    def show(self):
        print "Curve: ",self.curve_name
        print "type: %s" % CurveTypes[self.curve_type],
        print "used: %d" % self.curve_used,
        print "rfid: %d" % self.curve_oid,
        print "vtyp: %d" % self.curve_value_type,
        print "vtsp: %d" % self.curve_value_type_spec,
        print "rfnm: %s" % self.curve_oid_s, # assigned in a second pass
        print "vtys: %s" % ValueTypes[self.curve_value_type]
        if (self.curve_type == 0x01):
            print "rfid1: %d" % self.curve_oid1,
            print "rfnm1: %s" % self.curve_oid1_s
            if (self.curve_oid2 > 0):
                print "rfid2: %d" % self.curve_oid2,
                print "rfnm2: %s" % self.curve_oid2_s


        return
    
    def parse(self):
        cvh_data = self.hd_data
        cvd_data = self.dt_data
        (self.curve_type,) = struct.unpack("<B",cvh_data[0x00:0x01])
        (self.curve_used,) = struct.unpack("<B",cvh_data[0x01:0x02]) # correct??
        (self.curve_oid,) = struct.unpack("<H",cvh_data[0x04:0x06])
        (self.curve_value_type, self.curve_value_type_spec) = struct.unpack("<BB",cvh_data[0x1E:0x20])
        # worksheet columns have name
        if (self.curve_type == 0x10):
            self.curve_name = cvh_data[0x12:0x24] # 12 char string
            self.curve_name = self.curve_name.partition('\0')[0].strip()
        else: # 
            self.curve_name = 'None'
            (self.curve_oid1,) = struct.unpack("<H",cvh_data[0x23:0x25])
            (self.curve_oid2,) = struct.unpack("<H",cvh_data[0x4D:0x4F])

        return
        
if (__name__ == '__main__'):
    import sys, os, glob
    if (len(sys.argv) < 2):
        print "Curve cvh-nnn [cvd-nnn] "
        sys.exit()

    if (len(sys.argv) < 3):
        data_file = 'cvd-'+'-'.join(sys.argv[1].split('-')[1:2])+'-*'
        data_file = glob.glob(data_file)[0]
    else:
        data_file = sys.argv[2]
    
    this_curve = OPJCurve()
        
    with open(sys.argv[1]) as hdf:
        this_curve.hd_data = hdf.read()
        
    with open(data_file) as dtf:
        this_curve.dt_data = dtf.read()
    
    this_curve.parse()
    this_curve.show()
