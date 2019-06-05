echo "old version=`git describe`"
version=$1
delta=${version##*.}
# strip any leading 0s from delta
delta=`echo $delta|sed -e 's/^0*//'`
branch=${version%.*}
major=${branch%.*}
echo "$branch minor=$minor"
if [ "$major" = "$branch" ]; then
  # we have a full release, not beta release
  minor=$delta
  delta=0
else
  # we have a beta release
  minor=${branch##*.}
fi
echo $major $minor $delta
scidavis_version=$[$major*65536 + $minor*256 + $delta]
rm -f libscidavis/src/version.cpp
cat >libscidavis/src/version.cpp <<EOF
#include "globals.h"
namespace SciDAVis
{
  const int scidavis_versionNo = $scidavis_version;
  const char* scidavis_version = "$version";
  const char * release_date = "`date +"%b %d, %Y"`";
}
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
echo "PROJECT_NUMBER=$version" >Doxyversion
echo "$1" >doc/version.tex
git commit -a -m "Release $1"
git tag -a -m "" $1
