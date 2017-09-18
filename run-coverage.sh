# runs the coverage test, building HTML display of test-covered code
qmake CONFIG+=gcov CONFIG+=aegis
make qmake
make clean
make -j4
lcov -i -c -b . -d . --no-external -o lcovi.info
for i in test/00/*; do
    sh $i
done
lcov -c -b . -d .  --no-external -o lcovt.info
rm lcov.info
lcov -a lcovi.info -a lcovt.info -o lcov.info
genhtml -o coverage lcov.info
