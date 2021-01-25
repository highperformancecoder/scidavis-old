# -*- coding: utf-8 -*-
"""
Created on Wed Jan 20 18:17:43 2021

@author: Suthiro
"""

import xml.etree.ElementTree as ET

    
def recursive(elem, func):
    for subelem in elem:
        func(subelem)
        recursive(subelem, func)
        
def obtainStrings(subelem):
    global tempString
    global translationStrings
    if ('source'==subelem.tag):
        tempString = subelem.text
    elif ('translation'==subelem.tag):
        if '' != tempString and '' != subelem.text and None != subelem.text:
            if tempString not in translationStrings:
                translationStrings[tempString] = subelem.text
                tempString = ''
            
def replaceStrings(subelem):
    global tempString
    if ('source'==subelem.tag):
        if subelem.text in translationStrings:
            tempString = translationStrings[subelem.text]
    elif ('translation'==subelem.tag):
        if '' != tempString:
            subelem.text = tempString
            subelem.attrib = {}
            tempString = ''
    return
    
def updateTranslation(old,new):
    global tempString
    global translationStrings
    translationStrings = {}
    tempString = ''     
    
    tree = ET.parse(old)
    root = tree.getroot()
    recursive(root, obtainStrings)
    
    tempString = ''  
    tree = ET.parse(new)
    root = tree.getroot()
    recursive(root, replaceStrings)
    
    with open(new, "w", encoding = "utf-8") as output:
        output.write(ET.tostring(root, encoding="unicode"))
        
from os import listdir
from os.path import isfile, join, splitext, isdir
import sys

if len(sys.argv)==1:
    print('Usage: translation_updater OLD NEW\nOLD - path to directory with old (obsolete) translation files\n"\
          "NEW - path to directory with new (generated) translation files\n')
    sys.exit(0)
elif len(sys.argv)!=3:
    print('Invalid number of arguments passed: ' + str(len(sys.argv))+ ', expected 2!')
    sys.exit(1)

old_folder = str(sys.argv[1])
new_folder = str(sys.argv[2])

if not isdir(old_folder):
    print(old_folder+' is an invalid path!')
    sys.exit(1)
if not isdir(new_folder):
    print(new_folder+' is an invalid path!')
    sys.exit(1)

oldFiles = [f for f in listdir(old_folder) if isfile(join(old_folder, f))]
newFiles = [f for f in listdir(new_folder) if isfile(join(new_folder, f)) and '.ts'==splitext(join(new_folder, f))[-1].lower()]
for f in newFiles:
    if f in oldFiles:
        updateTranslation(join(old_folder, f), join(new_folder, f))