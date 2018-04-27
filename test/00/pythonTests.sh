#! /bin/sh

here=`pwd`
if test $? -ne 0; then exit 2; fi
tmp=/tmp/$$
mkdir $tmp
if test $? -ne 0; then exit 2; fi
cd $tmp
if test $? -ne 0; then exit 2; fi

fail()
{
    echo "FAILED" 1>&2
    cd $here
    chmod -R u+w $tmp
    rm -rf $tmp
    exit 1
}

pass()
{
    echo "PASSED" 1>&2
    cd $here
    chmod -R u+w $tmp
    rm -rf $tmp
    exit 0
}

trap "fail" 1 2 3 15

# insert test commands here
cp $here/test/pythonTests/* .
for i in *.py; do
    echo $i
    $here/scidavis/scidavis -l=en -x $i
    if test $? -ne 0; then fail; fi
done

if [ -z "$TRAVIS" ]; then
    for i in *.png; do
        diff $i $here/test/renderedImages/$i
        if test $? -ne 0; then fail; fi
    done
fi

pass
