import sys
import os
import re

dirs=[os.environ['HOME']+'/usr/lib', '/usr/local/lib', '/usr/local/lib64', '/usr/lib', '/usr/lib64']

#pattern1=re.compile("lib(boost_python-py"+str(sys.version_info.major)+".*).so$")
pattern1=re.compile("lib(boost_python"+str(sys.version_info.major)+".*).so$")

for dir in dirs:
    for (root,subs,files) in os.walk(dir):
        for file in files:
            match=pattern1.match(file)
#            print(file,match)
#            if match is None:
#                match=pattern2.match(file)
            if match is not None:
                print('-l'+match.group(1))
        
        
