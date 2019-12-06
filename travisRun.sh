git clone https://github.com/highperformancecoder/classdesc.git
pushd classdesc
make install
popd
qmake-qt5 CONFIG+=aegis
make qmake
make -j2
export TRAVIS=1 QT_QPA_PLATFORM=offscreen
for i in test/00/*; do if xvfb-run sh $i; then true; else exit 1; fi; done
