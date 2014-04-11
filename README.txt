For questions or bug reports, please contact Jordi Vila (Jordi.Vila@wur.nl)

Creating .DMG on OS X (using QtCreator):
-------------------------------------------------------------
+ Set the "MacOS" switch in plotwindow.cpp to "true"
+ Compile CLASS using QtCreator, this usually places the executable in ../mlmodel_main-...
+ In ../mlmodel_main-..., call "macdeployqt CLASS.app -dmg", this creates the .DMG

Compiling for Linux 
-------------------------------------------------------------
Tested on a clean default installation of Ubuntu 10.04.2 LTS / OpenSUSE 11.4 (KDE):

+ Install required packages (Ubuntu 10.04.2 LTS):
   "sudo apt-get install g++ qt4-qmake libqt4-dev" 
  Or for OpenSUSE (11.4):
   "sudo zypper in libqt4-devel make" 
+ Compile CLASS:
   "qmake"
   "make"
+ This should provide an executable "modelgui".

