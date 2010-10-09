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
##    File: utils.py


def jdt2unixtime(juliantime):
    """
    Julian date to Unix time conversion.
    http://en.wikipedia.org/wiki/Julian_day
    """
    jdn_ue = 2440587.5  # julian date for the unixtime epoch
    return int((juliantime - jdn_ue) * 86400.)

def hexrepr(bdata,sflag=False):
    """
    Make a hex representation of binary data
    
    set sflag = True to add the normal representation of data in result
    
    """
    result = ""
    if (type(bdata)==str): 
        for x in bdata:
            result += "%02x " % ord(x)
        if (sflag): result += ": " + bdata
    elif (type(bdata)==int):
        result = "%X " % bdata
        if (sflag): result += ": %d" % bdata
    elif (type(bdata)==float):
        result = hexrepstruct.pack("<d",bdata)
        if (sflag): result += ": %g" % bdata
    else:
        result = repr(bdata)
    return result
