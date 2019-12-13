FROM highperformancecoder/travisciimage
COPY . /root
RUN zypper install -y -l libqwt-qt5-5-devel libqt5-qtsvg-devel  libqt5-qtwayland libqwtplot3d-qt5-0-devel libtirpc-devel muparser-devel libboost_python3-devel python3-devel glu-devel python3-numpy liborigin-devel libqt5-linguist-devel
RUN cd /root && sh travisRun.sh

