HexaMonkey
==========

HexaMonkey is a freely distributed framework which goal is to provide tools to view the internal structure of any binary file and navigate through it seamlessly. It can be used to facilitate the development and debugging of encoding and decoding software.

It relies on lazy and asynchronous parsing to offer a pleasant experience to the user. It is highly exentensible thanks to its modular architecture and a powerful description langage : HMDL.

The formats already supported are :

    Video :
        mpeg4
        mpeg2-ts
        matroska
        avi
        asf/wmw (incomplete)
    Image :
        png
        jpeg (incomplete)
        gif
    Archive :
        zip
    Database :
        sqlite


We strongly encourage you to implement other formats and share them : see the HMScript documentation (http://www.hexamonkey.com/hmdoc.php) for more information.

Installing the project on Windows
=================================
There is an installer available on http://www.hexamonkey.com/, browse the download section.

Building the project
====================
Under Linux
-----------

You need qt5, gcc>=4.7, bison and flex on your computer, make sure qmake is available in your PATH. At the root of the project, type:

`qmake -r`

to prepare the build. You may specify your lib and bin directory (default /usr/lib and /usr/bin) by typing :

`qmake -r LIBDIR=/my/own/lib BINDIR=/my/own/bin`

then type

`make`

to build hexamonkey. and

`sudo make install`

to install it on your system. Then type

`hexamonkey`

to start the program.


Under Windows
-------------

Build the .exe files
You need the version of qt5 with the MinGW compiler to build the project and also cygwin with gcc3, flex, bison and make. Sadly the most recent version of cygwin don't ship gcc3, so you'll have to use the Cygwin time machine (www.fruitbat.org/Cygwin/) to get older version of the packages. So download the cygwin setup file from the official site (www.cygwin.com) and then run 

setup-x86.exe -X -S ftp://www.fruitbat.org/pub/cygwin/circa/2012/08/20/222043 -O

Follow the installer instructions and select the packages gcc, flex, bison and make. Then you should have all the tools you need to build the project.

First step : in cygwin browse the compiler directory of the source then run

make CFLAGS=-mno-cygwin

if it doesn't work you may check that gcc in cygwin is pointing to /usr/bin/gcc-3.exe if this is not the case you may change the CC variable in the Makefile and put it to /usr/bin/gcc-3 and then rerun the command.
Then use qt5 on the file gui/gui.pro open wiht qt creator, then choose a Release configuration before compiling check in your project configuration that shadow build is unchecked then compile with the button or Ctrl+B. 

Build the installer
Check that all the required DLLs are in a dll directory you have created at the root of the project.

Required DLLs : 

Found in %QtINSTALLDIR%\Qt%VERSION%\%VERSION\mingw47_32\bin:

- Qt5Core.dll 
- Qt5Gui.dll 
- Qt5Widgets.dll 
- D3DCompiler_43.dll
- icudt49.dll
- icuin49.dll
- icuuc49.dll
- libEGL.dll
- libgcc_s_sjlj-1.dll 
- libGLESv2.dll
- libstdc++-6.dll
- libwinpthread-1.dll

Found in %QtINSTALLDIR%\Qt%VERSION%\%VERSION%\mingw47_32\plugins\platform:

- qminimal.dll
- qwindows.dll


You need to install wix (available at http://wix.sourceforge.net/) then run in the root of the project (after adding the bin dir of wix to your PATH or run full path to the candle.exe and light.exe instead of just their names.)

candle.exe installer.wxs

light.exe installer.wixobj
