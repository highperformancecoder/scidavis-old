# SciDAVis - Installation Notes

**Please visit our [homepage](http://scidavis.sourceforge.net) for more information.**

## Generic requirements
In order to compile SciDAVis, you need to install the following
libraries. Easiest is to use your package manager to install prebuilt versions.
- Qt version 4
- Qwt version 5
- QwtPlot3D
- GSL
- muParser
- zlib

For the optional Python scripting feature, you also need:
- Python
- PyQt
- sip

For the default build, you also often need the QtAssistant package. If
not available, you can compile without (see below)

## Linux

1. In the top level directory, run qmake:

- qmake<br>
  Configures the minimal build
- qmake CONFIG+=python CONFIG+=liborigin<br>
  Configure python scripting and Origin import support
- qmake CONFIG+=noassistant<br>
  compile without QtAssistant (documentation browser)
- qmake CONFIG+=aegis<br>
  Build everything, including unit tests

2. type "make qmake" Take careful note of any warning/error messages at this
stage, as it may indicate installation problems that will cause
problems later.

3. type "make"

4. To install, type "make INSTALL_ROOT=/usr/local

## Windows - MXE builds

The distributed windows .msi is built using MXE, a cross compiler
environment creating Windows esecutables on Linux

1. Clone MXE respository. Use highperformancecoders fork, as it
contains needed packages not in the official mxe fork
   - git clone https://github.com/highperformancecoder/mxe.git

2. Install necessary libraries:
   - cd mxe
   - make boost gsl muparser qwt5_qt4 qwtplot3d

3. Configure scidavis for MXE. If the location of your MXE dist is
MXE_HOME, do
   - $MXE_HOME/usr/bin/i686-w64-mingw32.static-qmake-qt4 CONFIG+=mxe

4. Build scidavis
   - make qmake
   - make

5. Copy the scidavis directory to a Windows machine

6. Install [WiX](http://wixtoolset.org/)

7. Install [Cygwin](https://www.cygwin.com/)

8. Create the install shield from scidavis top level directory
   - start cygwin bash shell
   - cd scidavis
   - sh makeMsi.sh

9. Do a test from the command prompt with
   - msiexec -i scidavis-XXX.msi

## Windows - old installation notes for historical reference

1.  Download the "Qt/Windows Open Source Edition" from [http://www.qtsoftware.com/downloads/opensource/appdev/windows-cpp](http://www.qtsoftware.com/downloads/opensource/appdev/windows-cpp). It comes with a graphical installer. When the Qt installer asks you to install MinGW, say yes unless you already have it on your system. In that case you have to tell the Qt installer where to find it. In the case that PyQt (see blow) does not yet support the latest version of Qt, you can get older versions here: [ftp://ftp.trolltech.com/qt/source/](ftp://ftp.trolltech.com/qt/source/).
2.  Download Python from [http://www.python.org/download/](http://www.python.org/download/) and install it.
3.  Download the source code of SciDAVis from [http://scidavis.sourceforge.net/download.html](http://scidavis.sourceforge.net/download.html) and unpack it.
4.  Download the SIP source code for Windows from [http://www.riverbankcomputing.co.uk/software/sip/download](http://www.riverbankcomputing.co.uk/software/sip/download) and unpack it into the 3rdparty subfolder of the SciDAVis package. Rename the "sip-x.y" directory that gets created to "sip".
5.  Download the PyQt v4 source code for Windows from [http://www.riverbankcomputing.co.uk/software/pyqt/download](http://www.riverbankcomputing.co.uk/software/pyqt/download) and unpack it into the 3rdparty subfolder of the SciDAVis package. Rename the "PyQt-win-gpl-x.y.z" directory that gets created to "PyQt-win-gpl".
6.  Download the "Developer files" of the GSL from [http://gnuwin32.sourceforge.net/packages/gsl.htm](http://gnuwin32.sourceforge.net/packages/gsl.htm) and unpack them into the 3rdparty subfolder of the SciDAVis package.
7.  Download the source code of the latest version of Qwt from [http://sourceforge.net/project/showfiles.php?group_id=13693](http://sourceforge.net/project/showfiles.php?group_id=13693) and unpack it into the 3rdparty subfolder of the SciDAVis package. Rename the "qwt-x.y.z" directory that gets created to "qwt".
8.  Download the source code of QwtPlot3D from [http://qwtplot3d.sourceforge.net/web/navigation/download_frame.html](http://qwtplot3d.sourceforge.net/web/navigation/download_frame.html) and unpack it into the 3rdparty subfolder of the SciDAVis package.
9.  Download the source code of muParser from [http://sourceforge.net/project/showfiles.php?group_id=137191](http://sourceforge.net/project/showfiles.php?group_id=137191) and unpack it into the 3rdparty subfolder of the SciDAVis package. Rename the "muparser_vxyz" directory that gets created to "muparser".
10.  Open the start menu, find the folder created by the Qt SDK and start a "Qt Command Prompt". Execute "cd <directory containing SciDAVis sources>", then "build". SciDAVis and its dependencies should be built automatically and installed into the directory "output".

## Mac OS X - MacPorts instructions

These notes refer to use MacPorts. Using other build environments such
as HomeBrew is presumably equally as viable, but you may need to hack
the config files a bit.

1. Install [MacPorts](https://www.macports.org/).

2. Install [XCode and XCode command line
tools](https://guide.macports.org/#installing.xcode)

3. If you want to be able to run your build on an earlier version of
MacOSX than your build machine, see [How to build a Macintosh
executable that will run on older versions of
MacOSX.](http://www.hpcoders.com.au/blog/?p=100)

4. Install the prerequisites for scidavis.
  - port install qt4-mac qwt qwtplot3d boost gsl py27-pyqt4 py-pyqwt py27-sip muparser<br>
  Sadly, you may need to do this step multiple times before eveything
  is installed

5. Add qt's bin directory to your PATH
  - PATH=/opt/local/bin:/opt/local/libexec/qt4/bin:$PATH
  
6. Configure scidavis
  - qmake CONFIG+=osx_dist
  
7. Build scidavis
  - make qmake
  - make

8. Create the installable package
  - sh mkMacDist.sh

## Additional information

### Forums and mailing lists:

If you want to discuss SciDAVis with other users (or the developers) or if you are insterested in its further development, you can go to the [SciDAVis forum](http://sourceforge.net/forum/?group_id=199120) or subscribe to the [SciDAVis mailing list](http://sourceforge.net/mail/?group_id=199120).

### License:

SciDAVis is distributed under the terms of the [GNU General Public License](http://www.gnu.org/licenses/gpl.html) as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version. A copy of this license is provided in the file `gpl.txt`.

Thus it is ["free software"](http://www.fsf.org/licensing/essays/free-sw.html). "Free software" is a matter of liberty, not price. To understand the concept, you should think of "free" as in "free speech", not as in "free beer". "Free software" is also often called [Open Source, FOSS, or FLOSS](http://en.wikipedia.org/wiki/Alternative_terms_for_free_software). When we say that SciDAVis is "free", we are talking about

*   The freedom to run the program, for any purpose (freedom 0).
*   The freedom to study how the program works, and adapt it to your needs (freedom 1). Access to the source code is a precondition for this.
*   The freedom to redistribute copies so you can help your neighbor (freedom 2).
*   The freedom to improve the program, and release your improvements to the public, so that the whole community benefits (freedom 3). Access to the source code is a precondition for this.

That said, SciDAVis is also free as in "free beer". We do not charge money for anything you can download on our homepage and we will not do so in the future. See [here](http://scidavis.sourceforge.net/contributing/index.html) for why we can do this and why we're working on SciDAVis.  

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
