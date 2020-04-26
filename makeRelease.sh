echo "old version=`git describe`"
version=$1
delta=${version##*.}
# strip any leading 0s from delta
delta=`echo $delta|sed -e 's/^0*//'`
branch=${version%.*}
major=${branch%.*}
if [ "$major" = "$branch" ]; then
  # we have a full release, not beta release
  minor=$delta
  delta=0
  sem_version="$major.$minor.0"
else
  # we have a beta release
  minor=${branch##*.}
  sem_version="$branch.0-beta$delta"
fi
echo "semantic version $sem_version"
scidavis_version=$[$major*65536 + $minor*256 + $delta]
rm -f libscidavis/src/version.cpp
cat >libscidavis/src/version.cpp <<EOF
#include "globals.h"
const int SciDAVis::scidavis_versionNo = $scidavis_version;
const char* SciDAVis::scidavis_version = "$sem_version";
const char * SciDAVis::release_date = "`date +"%b %d, %Y"`";
EOF

# also check that all translation files have been included
for i in scidavis/translations/*.ts; do
    if git ls-files --error-unmatch >/dev/null; then
        true;
    else
        echo "translation $i not checked in"
        exit 1
    fi
done

# update Doxyversion
rm -f Doxyversion
echo "PROJECT_NUMBER=$sem_version" >Doxyversion
echo "$sem_version" >doc/version.tex
git commit -a -m "Release $sem_version"
git tag -a -m "" $sem_version
