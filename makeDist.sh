fullversion=`git describe`
version=${fullversion##-*}
extra=${fullversion%%*-}
name=scidavis-$version~$extra

git archive --format=tar.gz --prefix=$name/ HEAD -o $name.tar.gz
