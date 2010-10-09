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
##    File: OPJFile.py
##    opjparse (attempts to) read and parse OPJ files, and perhaps OGG, 
##    OGM, and OGW files also.

import struct
import sys, os
import DataSet as ds
import Window as wd
import Layer as ly
import SubLayer as sl
import Curve as cv
import Axis as ax
import Note as nt
import ProjectTree as pt
import Attachment as at
      
def getObjectSize(fp):
    """
    read a 4-byte integer followed by '\n'
    """
    objsz = fp.read(5)
    assert (len(objsz) == 5) # check for EOF
    if (objsz[-1] != '\n'):
         print >> outfile, "getObjectSize: Possible logic error in file %s position %s" % (fp.name, hex(fp.tell()))
         
    assert (objsz[-1] == '\n')
        
    return struct.unpack('<i',objsz[:4])[0]

def getObject(fp,size):
    """
    read a size-byte blob of data followed by '\n'
    """
    if (size>0):
        objdt = fp.read(size)
        try:
            assert (fp.read(1) == '\n')
        except AssertionError:
            print >> outfile, "getObject: Possible logic error in file %s position %s" % (fp.name, hex(fp.tell()))
            sys.exit()
        return objdt
    return None 

def getGlobalHeader(fp):
    # the global header is (up to now) a list with only one element
    block_size = getObjectSize(fp)
    gh_data = getObject(fp, block_size)
    # i.e., next element size should be zero
    block_size =  getObjectSize(fp)
    assert (block_size == 0)
    
    # see gh_info.txt for details
    return gh_data

def readDataSetList(fp):
    this_dt_list = []
    print >> outfile, "Reading Data sets ..."
    while (1):
        next_dt = readDataSetElement(fp)
        if (next_dt == None):
            print >> outfile, "... done (Data sets)"
            return this_dt_list
        next_dt.oid = len(this_dt_list) # use len of list for oid
        this_dt_list.append(next_dt)

def readDataSetElement(fp):
    dse_header_size = getObjectSize(fp)
    
    if (dse_header_size==0):
        return None
    
    this_dataset = ds.OPJDataSet()
    this_dataset.hd_data = getObject(fp, dse_header_size)
    
    dse_data_size = getObjectSize(fp) # can be zero
    this_dataset.dt_data = getObject(fp, dse_data_size)
    
    # element mask size (often is zero)
    dse_mask_size = getObjectSize(fp)
    this_dataset.ms_data = getObject(fp, dse_mask_size)

    # get relevant items
    this_dataset.parse()

    return this_dataset
    
def readWindowList(fp):
    this_wd_list = []
    print >> outfile, "Reading Windows ..."
    while (1):
        next_wd = readWindowElement(fp)
        if (next_wd == None):
            print >> outfile, "... done (Windows)"
            return this_wd_list
        next_wd.oid = len(this_wd_list) # use len of list for oid
        this_wd_list.append(next_wd)

def readWindowElement(fp):
    we_header_size = getObjectSize(fp)

    if (we_header_size==0):
        return None
    # TODO: deduce what kind of window (Graph, Matrix, ...)
    # and instantiate the appropriate Object derived from OPJWindow
    this_window = wd.OPJWindow()
    this_window.hd_data = getObject(fp, we_header_size)

    this_window.parse()

    this_window.layers = readLayerList(fp)
    return this_window
    
def readLayerList(fp):
    this_ly_list = []
    print >> outfile, "Reading Layers ..."
    while (1):
        next_ly = readLayerElement(fp)
        if (next_ly == None):
            print >> outfile, "... done (Layers)"
            return this_ly_list
        this_ly_list.append(next_ly)

def readLayerElement(fp):
    le_header_size = getObjectSize(fp)
    if (le_header_size==0):
        return None
    this_layer = ly.OPJLayer()
    this_layer.hd_data = getObject(fp, le_header_size)
    
    this_layer.parse()
    
    this_layer.sublayers = readSubLayerList(fp)
    this_layer.curves = readCurveList(fp)
    this_layer.axisbreaks = readAxisBreakList(fp)
    # each AxisParameter list is a 6 element list with parameters for
    # minorgrid, majorgrid, ticklabels_begin, format_begin, ticklabels_end, format_end
    this_layer.xaxis=readAxisParameterList(fp)
    this_layer.yaxis=readAxisParameterList(fp)
    this_layer.zaxis=readAxisParameterList(fp)
    return this_layer
    
def readSubLayerList(fp):
    this_sl_list = []
    while (1):
        next_sl = readSubLayerElement(fp)
        if (next_sl == None):
            return this_sl_list
        this_sl_list.append(next_sl)

def readSubLayerElement(fp):
    sle_header_size = getObjectSize(fp)
    if (sle_header_size==0):
        return None
    this_sl = sl.OPJSubLayer()
    this_sl.hd_data = getObject(fp, sle_header_size)
    
    sle_data_1_size = getObjectSize(fp)
    this_sl.block1_data = getObject(fp, sle_data_1_size)
    
    sle_data_2_size = getObjectSize(fp)
    
    # ATTENTION: grouped plots may have an additional nesting here.
    # Some OPJ (very few) have a subsublayer list
    if ((sle_data_1_size == 0x5e) and (sle_data_2_size == 0x04)):
        print >> outfile, "\nGroup, trying to read next"
        # let this SubLayer have a sublayer list itself
        this_sl.sublayers = readSubLayerList(fp)
        print >> outfile, "End of Group at %06x" %  fp.tell()
        # read and aditional 00 00 00 00 0a group
        eofgroup_size = getObjectSize(fp)
        return this_sl
    this_sl.block2_data = getObject(fp, sle_data_2_size)
    
    sle_data_3_size = getObjectSize(fp)
    this_sl.block3_data = getObject(fp, sle_data_3_size)
    
    this_sl.parse()
    
    return this_sl
    
def readCurveList(fp):
    this_cv_list = []
    print >> outfile, "Reading Curves ..."
    while (1):
        next_cv = readCurveElement(fp)
        if (next_cv == None):
            print >> outfile, "... done (Curves)"
            return this_cv_list
        this_cv_list.append(next_cv)

def readCurveElement(fp):
    ce_header_size = getObjectSize(fp)
    if (ce_header_size==0):
        return None
    this_cv = cv.OPJCurve()
    this_cv.hd_data = getObject(fp, ce_header_size)
    
    ce_data_size = getObjectSize(fp)
    this_cv.dt_data = getObject(fp, ce_data_size)
    
    this_cv.parse()
    
    return this_cv

def readAxisBreakList(fp):
    this_ab_list = []
    print >> outfile, "Reading Axis breaks ..."
    while (1):
        next_ab = readAxisBreakElement(fp)
        if (next_ab == None):
            print >> outfile, "... done (Axis breaks)"
            return this_ab_list
        this_ab_list.append(next_ab)

def readAxisBreakElement(fp):
    ab_data_size = getObjectSize(fp)
    if (ab_data_size==0):
        return None
    this_ab = ax.OPJAxisBreak()
    this_ab.dt_data = getObject(fp, ab_data_size)
    
    this_ab.parse()
    return this_ab

def readAxisParameterList(fp):
    this_ap_list = []
    print >> outfile, "Reading Axis parameters ..."
    while (1):
        next_ap = readAxisParameterElement(fp)
        if (next_ap == None):
            print >> outfile, "... done (Axis parameters)"
            return this_ap_list
        this_ap_list.append(next_ap)

def readAxisParameterElement(fp):
    ap_data_size = getObjectSize(fp)
    if (ap_data_size==0):
        return None
    this_ap = ax.OPJAxisParameter()
    this_ap.dt_data = getObject(fp, ap_data_size)
    
    this_ap.parse()
    return this_ap

def readParameterList(fp):
    this_par_list = dict()
    print >> outfile, "Reading Parameters ..."
    while (1):
        next_par_value_pair = readParameterElement(fp) 
        if (next_par_value_pair == None):
            print >> outfile, "... done (Parameters)"
            return this_par_list
        name,value = next_par_value_pair
        this_par_list[name] = value
    
def readParameterElement(fp):
    # parameter name ends with '\n', so we can use readline
    param_name = fp.readline().strip()
    if (param_name == '\0'):
        # TO CHECK: there is an extra 0x00 0x00 0x00 0x00 0x0A group
        # at end of Parameter List
        # may be there is another (yet unknown) section
        eopl_size = getObjectSize(fp) # should be zero
        return None
    # parameter value is a double, ...
    (param_value,) = struct.unpack('<d',fp.read(8))
    # ... followed by a '\n'
    assert (fp.read(1)=='\n')
    return (param_name,param_value)
    
def readNoteList(fp):
    this_nt_list = []
    print >> outfile, "Reading Notes at %s ..." % hex(fp.tell())
    while (1):
        next_nt = readNoteElement(fp)
        if (next_nt == None):
            print >> outfile, "... done (Notes)" 
            return this_nt_list
        next_nt.oid = len(this_nt_list) # use len of list for oid
        this_nt_list.append(next_nt)
    
def readNoteElement(fp):
    ne_header_size = getObjectSize(fp)
    if (ne_header_size==0):
        return None
    this_note = nt.OPJNote()
    this_note.hd_data = getObject(fp, ne_header_size)

    ne_label_size = getObjectSize(fp)
    this_note.lb_data = getObject(fp, ne_label_size)
    
    ne_contents_size = getObjectSize(fp)
    this_note.ct_data = getObject(fp, ne_contents_size)
    
    this_note.parse()
    
    return this_note

def readProjectTree(fp):
    global folder_id

    print >> outfile, "Reading Project tree at %s ..." % hex(fp.tell())

    pt_depth = 0
    folder_id = 0

    this_tree = pt.OPJProjectTree()
    # read first preamble
    pre_1_size = getObjectSize(fp) # should be 4
    this_tree.pre1_data = getObject(fp, pre_1_size)
    
    # read second preamble
    pre_2_size = getObjectSize(fp) # should be 16
    this_tree.pre2_data = getObject(fp, pre_2_size)
    
    # read root element and children
    rootfolder = readFolderTree(fp, pt_depth)
    
    # TO CHECK, there is an extra 0x00 0x00 0x00 0x00 0x0A group
    # at end of Project Tree
    # may be there is another (yet unknown) section
    eopt_size = getObjectSize(fp) # should be zero
    assert(eopt_size == 0)
    
    this_tree.root = rootfolder
    
    print >> outfile, "... done (Project tree)"
    return this_tree

def readFolderTree(fp, depth):
    global folder_id

    # folder properties
    fld_header_size = getObjectSize(fp)
    this_folder = pt.OPJFolderEntry()
    this_folder.hd_data = getObject(fp, fld_header_size)
    
    # TO CHECK: again a 0x00 0x00 0x00 0x00 0x0A group with apparently no purpose
    # can be combined with previous in a PropertyList-like piece?
    fld_eofh_size = getObjectSize(fp) # should be 0
    assert (fld_eofh_size == 0)
    
    # folder name
    fld_name_size = getObjectSize(fp)
    this_folder.fn_data = getObject(fp, fld_name_size)
    
    folder_name = this_folder.fn_data.partition('\0')[0]
    this_folder.name = folder_name
    
    # TO CHECK: again a 0x00 0x00 0x00 0x00 0x0A group with apparently no purpose
    # can be combined with previous ini a PropertyList like piece?
    fld_eofn_size = getObjectSize(fp) # should be 0
    assert (fld_eofn_size == 0)
    
    # now read file's descriptions
    fld_numf_size = getObjectSize(fp) # should be 4 as num_of_files is an integer
    (num_of_files,) = struct.unpack('<i',getObject(fp,fld_numf_size))
    this_folder.files = []
    for  i in range(num_of_files):
        this_folder.files.append(readProjectLeave(fp))
    
    # now read subfolders
    fld_numsf_size = getObjectSize(fp) # should be 4 as num_of_folders is an integer
    (num_of_folders,) = struct.unpack('<i',getObject(fp,fld_numsf_size))
    this_folder.folders = []
    for  i in range(num_of_folders):
        depth += 1
        folder_id +=1
        this_folder.folders.append(readFolderTree(fp, depth))
        depth -= 1
        
    return this_folder

def readProjectLeave(fp):
    global folder_id
    this_entry = pt.OPJObjectEntry()
    ptl_pre_size = getObjectSize(fp)    # usually is 0, so we don't read the data
    assert (ptl_pre_size == 0)
    ptl_header_size = getObjectSize(fp) # usually is 8
    this_entry.hd_data = getObject(fp, ptl_header_size)
    
    ptl_post_size = getObjectSize(fp)    # usually is 0
    assert (ptl_post_size == 0)
    
    (filetype, fileid) = struct.unpack('<ii',this_entry.hd_data[0x00:0x08])
    
    if (filetype == 0x00): # an element of the Windows collection
        this_entry.oid = fileid
        # this_entry.name = ObjectNames[fileid]
    elif (filetype == 0x100000): # an element of the Notes collection
        this_entry.oid = fileid+0x100000
        # this_entry.name = NoteNames[fileid]
    else:
        print >> outfile, "Unknown type %3d 0x%X" % (filetype, filetype)
        
    return this_entry

def readAttachmentList(fp):
    #
    # The attachment list is preceeded by an object in the usual form:
    # size_of_object(4 bytes) '\n' object '\n' 
    # where object is a two integer group: first is 0x00 0x10 0x00 0x00 (4096),
    # second is number_of_attachments.
    
    # ATTENTION sometimes there are only attachments of second kind
    # If next 4 bytes are the integer 8 we have type 1 attachments
    # if not, we should skip to type 2.
    # It is not implemented because it happens seldom, 
    # but the code could look like
    #     (atl_header_size,) =  struct.unpack('<i', fp.read(4))
    #     if (atl_header_size == 8):
    #         assert (fp.read(1) == '\n') # get the remaing 0x0A
    #         ... continue with first kind reading
    #     else:
    #         fp.seek(-4,os.SEEK_CUR) # undo reading
    #     ... continue with second kind reading
    print >> outfile, "Reading Attachments at %s ..." % hex(fp.tell())
    print >> outfile, "  first kind"
    atl_header_size = getObjectSize(fp) # should be 8 
    assert (atl_header_size == 8) 
    atl_header_data = getObject(fp, atl_header_size) 
    (atl_data_1, number_of_attachments) = struct.unpack('<ii',atl_header_data)
    assert (atl_data_1 == 4096)
    
    # get the attachments in first group
    kind1_attachments = []
    for i in range(number_of_attachments):
        # header is a group of 7 integers followed by '\n'
        # 1st 0x00 0x10 0x00 0x00 (4096)
        # 2nd attachment number (<number_of_attachments)
        # 3rd attachment size
        # 4th .. 7th TO CHECK
        (atd0, 
         attachment_no, 
         attachment_size,
         atd3,atd4,atd5,atd6) = struct.unpack('<7i',fp.read(7*4))
        # get the '\n'
        assert (fp.read(1)=='\n')
        
        assert (atd0 == 4096)
        assert (attachment_no == i)
        
        this_attach = at.OPJKind1_Attachment()
        this_attach.dt_data = getObject(fp, attachment_size)
        # We need to read an ending '\n' even if attachment_size=0
        if (attachment_size==0):
            assert (fp.read(1)=='\n')
        kind1_attachments.append(this_attach)
    
    print >> outfile, "  now at %d, file size is %d" % (fp.tell(), os.fstat(fp.fileno()).st_size)
    print >> outfile, "  second kind"
    # Second group of attachments is a series of (header, name, data) triplets
    # There is no number of attachments. It stops when we reach EOF.
    #  Header is a group of 3 integers
    #      1st attachment header+name size including itself (always larger than 8)
    #      2nd attachment type (0x59 0x04 0xCA 0x7F for excel workbooks)
    #      3rd size of data
    kind2_attachments = []
    while (True):
        next_triplet = fp.read(3*4)
        if (len(next_triplet) == 0): # we got to EOF
            break
        if (len(next_triplet) < 12): # wrong (unknown) format ?
            break
        (attachment_header_size,
         attachment_type,
         attachment_size) = struct.unpack('<3i',next_triplet)
        attachment_header_size -= 12 # 12 bytes are for the 3 int header
        
        this_attach = at.OPJKind2_Attachment()
        this_attach.hd_data = fp.read(attachment_header_size) # no '\0' at end
        
        this_attach.dt_data = fp.read(attachment_size)  # no '\0' at end
        kind2_attachments.append(this_attach)

    print >> outfile, "... done (Attachments)"
    return (kind1_attachments, kind2_attachments)

def getFileVersion(fp):
    # see gh_info.txt for details
    sversion = fp.readline().strip()
    try:
        assert (sversion[-1]=='#')
    except AssertionError:
        print "Weird version string >>>%s<<<" % sversion
        sversion = sversion[:sversion.find('#')+1]
        
    (magick,fileversion,filebuildno) = sversion.split()[:3]
    
    try:
        assert(magick == 'CPYA')
    except AssertionError:
        print "This is not an OPJ file"
        print "Version string >>>%s<<< does not starts with CPYA" % sversion
        sys.exit()

    return (fileversion,filebuildno)

def readFile(path):
    global outfile
    fp = open(path,'r')
    filesize = os.fstat(fp.fileno()).st_size
    
    ofname = fp.name+'.log'
    outfile = open(ofname,'w')
    
    this_file = OPJFile()
    this_file.path = fp.name
    (this_file.version,this_file.buildno) = getFileVersion(fp)
    print >> outfile, "File %s has version %s, build no. %s" % (fp.name,this_file.version,this_file.buildno)

    this_file.global_header = getGlobalHeader(fp)
    # For files written with release 6.1 or later  global_header[0x1b:0x23] is 
    # a double  a.brrr indicating the file was written with program version number a.b release rrr
    try: # version < 6.1 does not have these
        (version_and_release,) = struct.unpack("<d",this_file.global_header[0x1b:0x23])
        program_version=(int(version_and_release*10))/10.
        program_release=int((version_and_release-program_version)*10000+0.5)
        print >> outfile, "It was written with program version %.1f, release %d" % (program_version, program_release)
        this_file.program_version = program_version
        this_file.program_release = program_release
    except struct.error:
        pass
    
    print >> outfile, "Now at %d, file size is %d" % (fp.tell(), filesize)
    
    this_file.datasets = readDataSetList(fp)
    
    print >> outfile, "%d datasets read" % len(this_file.datasets)
    print >> outfile, "Now at %d, file size is %d" % (fp.tell(), filesize)
    
    this_file.windows = readWindowList(fp)
    
    print >> outfile, "%d windows read" % len(this_file.windows)   
    print >> outfile, "Now at %d, file size is %d" % (fp.tell(), filesize)
    
    if (fp.tell() != filesize):
        this_file.parameters=readParameterList(fp)
    else:
        print >> outfile, "Now at %d, file size is %d" % (fp.tell(), filesize)
    
    # Note windows were added between version >4.141 and 4.210, 
    # i.e., with Release 5.0
    if (fp.tell() != filesize):
        this_file.notes = readNoteList(fp)
    else:
        print >> outfile, "Now at %d, file size is %d" % (fp.tell(), filesize)
    
    # Project Tree was added between version >4.210 and 4.2616,
    # i.e., with Release 6.0 
    if (fp.tell() != filesize):
        this_file.projecttree = readProjectTree(fp)
    else:
        print >> outfile, "Now at %d, file size is %d" % (fp.tell(), filesize)
    
    # Attachments were added between version >4.2673_558 and 4.2764_623,
    # i.e., with Release 7.0
    if (fp.tell() != filesize):
        this_file.attachments = readAttachmentList(fp)
    else:
        print >> outfile, "Now at %d, file size is %d" % (fp.tell(), filesize)
    
    outfile.close()
    fp.close()
    this_file.makelists()
    print "File %s " % this_file.path
    # old OPJ do not have notes
    n_notes = 0
    if hasattr(this_file,'notes'):
        n_notes = len(this_file.notes)
    print "has %d datasets, %d window(s), and %d note(s)" % (len(this_file.datasets), len(this_file.windows), n_notes)
    print "it is a version %s build %s file" % (this_file.version, this_file.buildno)
    try:
        print "it was written with program version %.1f build %d " %(this_file.program_version, this_file.program_release)
    except AttributeError:
        pass
    
    return this_file

class OPJFile(object):
        
    def makelists(self):
        # propagate lists to project and parent window
        self.layers = []
        self.sublayers = []
        self.curves = []
        for wd in self.windows:
            wd.sublayers = []
            wd.curves = []
            for lyr in wd.layers:
                self.layers.append(lyr)
                for sly in lyr.sublayers:
                    self.sublayers.append(sly)
                    wd.sublayers.append(sly)
                for cv in lyr.curves:
                    cv.curve_oid_s = self.datasets[cv.curve_oid-1].dataset_name
                    if (cv.curve_type == 0x01):
                        cv.curve_oid1_s = self.datasets[cv.curve_oid1-1].dataset_name
                        if (cv.curve_oid2 > 1):
                            cv.curve_oid2_s = self.datasets[cv.curve_oid2-1].dataset_name
                    self.curves.append(cv)
                    wd.curves.append(cv)
            if (wd.w_type_s == 'Worksheet'):
                # make a table joining data in columns from the needed datasets
                table_header = []
                table_data = []
                for cv in wd.curves:
                    table_header.append(cv.curve_name)
                    table_data.append(self.datasets[cv.curve_oid-1].data_values)
                wd.table = zip(*table_data)
                wd.table_header = table_header
        return
        
    def show(self):
        print "File: %s" % self.path
        for wd in self.windows:
            print "%2d Window: %s<" % (wd.oid, wd.window_name)
            for lyr in wd.layers:
                print "    Layer: %s<" % lyr.layer_name
                pass
                for sly in lyr.sublayers:
                    print "      Sublayer: %s<" % sly.sublayer_name
                    pass
                for cv in lyr.curves:
                    print "        Curve: %s %s<" % (cv.curve_name,cv.curve_oid_s)
                    pass
                for ab in lyr.axisbreaks:
                    pass
                for x in lyr.xaxis:
                    pass
                for x in lyr.yaxis:
                    pass
                for x in lyr.zaxis:
                    pass
                
        for ds in self.datasets:
            print "%2d Dataset: %s<" % (ds.oid, ds.dataset_name)
            pass
        for nt in self.notes:
            print "%2d Note: %s<" % (nt.oid, nt.note_name)
            pass 
        for pr in self.parameters:
            print pr,"=",self.parameters[pr]
            pass
        
        self.projecttree.root.show()
        
        if (hasattr(self,'attachments')):
            for at in self.attachments[0]:
                pass
            for at in self.attachments[1]:
                pass 
        return

if __name__ == "__main__":
    if (len(sys.argv) < 2):
        print "OPJFile filename.opj"
        sys.exit()

    print >> sys.stderr, "Processing file: %s " % sys.argv[1]
    this_file = readFile(sys.argv[1])
    for ds in this_file.datasets:
        ds.show()
    for wd in this_file.windows:
        print wd.window_name
        for ly in wd.layers:
            ly.show()

