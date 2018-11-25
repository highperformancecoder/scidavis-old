# runs the coverage test, building HTML display of test-covered code
#qmake CONFIG+=gcov CONFIG+=aegis
#make qmake
#make clean
#make -j4
for i in test/00/*; do
    sh $i
done

# gen_info source path rewriting no longer seems to cope with the
# scidavis file layout, so just fake it by creating some symbolic
# links where lcov and genhtml expect the source files to be
ln -sf `pwd`/libscidavis/src tmp
ln -sf `pwd`/test/*.{h,cpp} tmp/test
dir=tmp/libscidavis

lcov -i -c -d . --exclude tmp/test --no-external -o lcovi.info
lcov -c -d . --exclude tmp/test  --no-external -o lcovt.info
rm lcov.info
lcov -a lcovi.info -a lcovt.info -o lcov.info
genhtml --ignore-errors source -o coverage lcov.info
