name=scidavis-`git describe`
git archive --format=tar.gz --prefix=$name/ HEAD -o $name.tar.gz
