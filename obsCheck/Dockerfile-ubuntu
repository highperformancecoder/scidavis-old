FROM ubuntu:18.04
ADD . /root
RUN apt-get update -qq -y
RUN apt-get install -y wget gnupg2
RUN wget http://download.opensuse.org/repositories/home:hpcoder1/xUbuntu_18.04/Release.key
RUN apt-key add - < Release.key
RUN echo 'deb http://download.opensuse.org/repositories/home:/hpcoder1/xUbuntu_18.04/ /' >/etc/apt/sources.list.d/hpcoders.list
RUN apt-get update -qq -y
RUN apt-get install -y --allow-unauthenticated scidavis-beta
# scidavis always needs a functional X-server to run in, even for smoke tests
RUN apt-get install -y xvfb
RUN sh /root/scidavisSmoke.sh
