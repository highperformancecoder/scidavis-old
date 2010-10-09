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
##    File: DataSet.py
##    
##    Parse header and data of a DataSet.
##    Many details of the parsing process where obtained from liborigin1
##    http://sourceforge.net/projects/liborigin/
##    and liborigin2 http://soft.proindependent.com/liborigin2/

import struct
smalld=-1.23456789e-300
# 
OPJDataTypes = {
             0x0001: 'Unknown',
             0x0021: 'Unknown',
             0x0121: 'Unknown',
             0x0401: 'Unknown',
             0x1001: 'X Numeric',  # version 5.0
             0x1021: 'X Text',     # version 5.0
             0x1031: 'Unknown', 
             0x1121: 'X Text+Num', # version 5.0
             0x2001: 'Unknown',
             0x2081: 'Unknown',
             0x2121: 'Unknown',
             0x2130: 'Unknown',
             0x4001: 'Unknown',
             0x4021: 'Disregard',
             0x4121: 'Unknown',
             0x5001: 'X Numeric',
             0x5021: 'X Text',
             0x5121: 'X Text+Num',
             0x6000: 'Z Numeric',
             0x6001: 'Y Numeric', # also Matrix:double, see data_type_3 
             0x6003: 'Matrix float',
             0x6010: 'Xerr Num', # also Yerr
             0x6020: 'Z Text', # also just label
             0x6021: 'Y Text',
             0x6081: 'Function',
             0x6120: 'Z Text+Num',
             0x6121: 'Y Text+Num',
             0x6130: 'Xerr Text+Num', # also Yerr
             0x6801: 'Matrix int',
             0x6803: 'Matrix short',
             0x6821: 'Matrix byte',
             0x7001: 'Unknown',
             0x7121: 'Unknown'
            }
# we will restrict the modes to Numeric, Text, Month, Day, DateTime, Unknown
DataModes = {
             0x0001: 'Unknown',
             0x0021: 'Unknown',
             0x0121: 'Unknown',
             0x0401: 'Unknown',
             0x1001: 'Numeric',  
             0x1021: 'Text', 
             0x1031: 'Unknown', 
             0x1121: 'Text', 
             0x2001: 'Unknown',
             0x2081: 'Unknown',
             0x2121: 'Unknown',
             0x2130: 'Unknown',
             0x4001: 'Unknown',
             0x4021: 'Text',
             0x4121: 'Unknown',
             0x5001: 'Numeric',
             0x5021: 'Text',
             0x5121: 'Text',
             0x6000: 'Numeric',
             0x6001: 'Numeric', 
             0x6003: 'Numeric',
             0x6010: 'Numeric',
             0x6020: 'Text',
             0x6021: 'Text',
             0x6081: 'Unknown', # maybe Numeric is better
             0x6120: 'Text',
             0x6121: 'Text',
             0x6130: 'Text', 
             0x6801: 'Numeric',
             0x6803: 'Numeric',
             0x6821: 'Numeric',
             0x7001: 'Unknown',
             0x7121: 'Unknown'
            }

class OPJDataSet(object):
    data_values = []
    dataset_name = None
    
    def show(self):
        print "DataSet: %s" % self.dataset_name
        print "value_size ",self.value_size
        print "data_type   %5d [0x%04X] %s" % (self.data_type,self.data_type,OPJDataTypes[self.data_type])
        try: # version <6.1 does not have this
            print "data_type_3 %5d [0x%04X]" % (self.data_type_3,self.data_type_3)
        except AttributeError:
            pass
        print "data_type_2 %3d [0x%02X]" % (self.data_type_2,self.data_type_2)
        print "data_type_u %3d [0x%02X]" % (self.data_type_u,self.data_type_u)
        if (self.dt_data==None): return # empty columns, leave data_values empty
        print "num_rows   ", self.num_rows
        print "total_rows ", self.total_rows
        print "first_row  ", self.first_row
        print "used_rows  ", self.used_rows
        print "first values ", self.data_values[0:min(2,len(self.data_values))]
        print "last values  ", self.data_values[max(len(self.data_values)-2,0):]
        return
    
    def parse(self):
        dsh_data = self.hd_data
        dse_data = self.dt_data
        
        # first get parameters from dsh_data
        (self.data_type,self.data_type_2) = struct.unpack("<hB",dsh_data[0x16:0x19])
        (self.total_rows,self.first_row,self.used_rows) = struct.unpack("<3i",dsh_data[0x19:0x25])
        
        # how are values stored
        self.data_mode = DataModes[self.data_type]
        
        (self.data_type_u,) = struct.unpack("<B",dsh_data[0x3F:0x40])
        self.dataset_name = dsh_data[0x58:0x58+25] # 25 char string
        self.dataset_name = self.dataset_name.partition('\0')[0].strip()
        (self.value_size,)  = struct.unpack("<B",dsh_data[0x3D:0x3E])
        try: # for version 5.0 there are only 0x72 bytes
            (self.data_type_3,) = struct.unpack("<H",dsh_data[0x71:0x73])
        except struct.error:
            pass
        
        # now get values
        if (dse_data==None): # empty columns, leave data_values empty
            return
        dse_data_size = len(dse_data)
        assert((dse_data_size % self.value_size) == 0)
        self.num_rows = dse_data_size / self.value_size

        if (self.value_size == 1): # read byte
            upck_format = "<%dB" % self.num_rows
            self.data_values = struct.unpack(upck_format,dse_data)
            
        elif (self.value_size == 2): # read short
            upck_format = "<%dH" % self.num_rows
            self.data_values = struct.unpack(upck_format,dse_data)
            
        elif (self.value_size == 4): # read int
            upck_format = "<%di" % self.num_rows
            self.data_values = struct.unpack(upck_format,dse_data)
        
        elif (self.value_size == 8): # read double
            upck_format = "<%dd" % self.num_rows
            self.data_values = struct.unpack(upck_format,dse_data)
        
        elif (self.value_size > 8): 
            if ((self.data_type & 0x100) == 0x100): # Text and doubles
                upck_format = "<cc%ds" % (self.value_size - 2)
                temp_values=[]
                for i in range(self.num_rows): # CHECK: maybe only used_rows is enough
                    temp_values.append(struct.unpack_from(upck_format,dse_data,self.value_size*i))
                (pre1,pre2,temp_values) = zip(*temp_values)
                self.data_values=[]
                for i in range(self.num_rows): # CHECK: ibidem
                    tmp_s = temp_values[i]
                    if (pre1[i] == '\0'):  # Double
                        self.data_values.append(struct.unpack('d',tmp_s[:8])[0])
                    else: # Text
                        self.data_values.append(tmp_s[:tmp_s.index('\0')])
            else: # Text
                self.data_values=[]
                upck_format = "%ds" % self.value_size
                for i in range(self.num_rows):
                    (tmp_s,) = struct.unpack_from(upck_format,dse_data,self.value_size*i)
                    self.data_values.append(tmp_s[:tmp_s.find('\0')])
            # There is another way to read data when value_size>8 using numpy:
            #    data_values = np.array(dse_data,dtype=np.dtype([('col1','b'),('col2','b'),('col3','d')]))
        
        
        else:
            print "No code for reading size:",self.value_size
        
        self.real_no_rows = self.used_rows
        self.data_values = [ None if (x==smalld) else x for x in self.data_values]
        
        return 


if (__name__ == '__main__'):
    import sys, glob
    if (len(sys.argv) < 2):
        print "DataSet dsh-nnn [dsd-nnn] "
        sys.exit()

    if (len(sys.argv) < 3):
        data_file = 'dsd-'+'-'.join(sys.argv[1].split('-')[1:2])+'-*'
        data_file = glob.glob(data_file)[0]
    else:
        data_file = sys.argv[2]
    
    this_dataset = OPJDataSet()

    with open(sys.argv[1]) as hdf:
        this_dataset.hd_data = hdf.read()
        
    with open(data_file) as dtf:
        this_dataset.dt_data = dtf.read()
    
    this_dataset.parse()
    this_dataset.show()
