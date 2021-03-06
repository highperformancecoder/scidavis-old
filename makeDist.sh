fullversion=`git describe`
version=${fullversion%%-*}
extra=${fullversion##*-}
if [ $extra = $version ]; then
    name=scidavis-$version
else
    name=scidavis-$version~$extra
fi

git archive --format=tar.gz --prefix=$name/ HEAD -o $name.tar.gz
