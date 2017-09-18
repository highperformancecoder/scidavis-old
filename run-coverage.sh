# runs the coverage test, building HTML display of test-covered code
qmake CONFIG+=gcov CONFIG+=aegis
make qmake
make clean
make -j4
for i in test/00/*; do
    sh $i
done
lcov --rc "geninfo_adjust_src_path=tmp/" -i -c -d . --no-external -o lcovi.info
lcov --rc "geninfo_adjust_src_path=tmp/" -c -d .  --no-external -o lcovt.info
rm lcov.info
lcov -a lcovi.info -a lcovt.info -o lcov.info
genhtml --ignore-errors source -o coverage lcov.info
