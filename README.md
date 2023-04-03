CLASS model
------
### CLASS: Chemistry Land-surface Atmosphere Soil Slab model

### Book

CLASS is part of a book (soon to be) published at Cambridge University Press: "Atmospheric boundary layer: Integrating chemistry and land interactions" by Jordi Vila-Guerau de Arellano, Chiel C. van Heerwaarden, Bart J.H. van Stratum and Kees van den Dries.

### Obtaining the CLASS software

CLASS can be downloaded as precompiled binaries for [Windows and OS X](https://github.com/classmodel/modelgui/releases). 

For information on compiling CLASS on Linux / OS X, see the notes below.

### Note for OS X users

On recent versions of OS X, opening CLASS for the first time will result in a warning: *"CLASS can’t be opened because it is from an unidentified developer"*. To circumvent this problem, right click the application and select "open" and again "open" in the dialog instead of double clicking the application. This step is only needed once.

### Bugs

Bugs can be reported at https://github.com/classmodel/modelgui/issues?state=open

In order to reproduce and fix the bug, we will most likely need:
* Information on the operating system you are using (Windows or OS X version, Linux distribution)
* The git-hash, shown in the 'about CLASS' window (e.g. git-hash: v1.0_beta2-6-g49b7603)
* A description of the exact steps needed to reproduce the problem.

### Compiling CLASS

The source code of CLASS can be downloaded manually [from GitHub](https://github.com/classmodel/modelgui/archive/master.zip) or (preferably) using GIT:

    git clone https://github.com/classmodel/modelgui.git

Both methods will always provide you the most recent (stable) version of CLASS. There are two ways of compiling CLASS: using [QtCreator](http://qt-project.org/downloads) or using the command line. 

#### QtCreator

After installing QtCreator, open the CLASS project file (mlmodel_main.pro) in QtCreator and select "build" > "run". This should compile and start CLASS.

#### Command line

Without QtCreator installed, some additional packages are needed, e.g.
* Ubuntu (10.04.2 LTS): g++ qt4-qmake libqt4-dev
* OpenSUSE (11.4): libqt4-devel make

After installing the required packages, calling:

    qmake
    make

from the source directory of CLASS should compile the model.

If you are working with Ubuntu for Windows, there are problems with connection to a display, try:
https://techcommunity.microsoft.com/t5/modern-work-app-consult-blog/running-wsl-gui-apps-on-windows-10/ba-p/1493242

In order to build the code on OS X without creating an Xcode project, call:

    qmake -spec macx-g++
    make
