#!/usr/bin/python3
#
# git pre-commit hook for running clang-format at commit time
# To use, link this file to .git/hooks/pre-commit

import os
import re
import filecmp

tmp="tmp.xxx"

root=os.popen("git rev-parse --show-toplevel").readline().rstrip()+'/'
status=0
for i in os.popen("git diff-index --cached --name-only --diff-filter=d HEAD").readlines():
    i=i.rstrip()
    f=root+i
    if re.match(".*\.(cpp|h)",i):
        os.system("clang-format "+f+">"+tmp)
        if not filecmp.cmp(tmp,f):
            os.rename(tmp, f)
            print(i+" is reformatted, try again")
            status+=1

exit(status)

