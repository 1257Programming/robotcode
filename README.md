robotcode
=========

Nathan, Rohan and Brian's code

2014-frcmake
------------

Version we all wrote together to be the standard, official code for competition.
build on frcmake after the WindRiver license expires.
<br>
NOTE ABOUT FRCMAKE:
Use the shell script in the root directory of the repo to install the frc c++11 toolchain and frcmake on linux. You can then go into 2014-frcmake and run:
```shell
# create makefiles
frcmake
# build
make
# push code
make deploy
```

2014-ucpp
---------

Version we all wrote together to be the standard, official code for competition.
build on ucpp after the WindRiver license expires.
<br>
NOTE ABOUT UCPP:
to install UCPP, first download it by cloning:
```shell
git clone https://github.com/nikitakit/ucpp.git
```
or download it as a zip at https://github.com/nikitakit/ucpp/archive/master.zip. To install, put the ucpp files somewhere you won't delete them, then add it to you path:
```shell
export PATH=/path/to/ucpp:$PATH
```
to set it up, use
```shell
ucpp setup -t1257
```
to initialize a project, use
```shell
ucpp init
# configure the project once you've made it:
ucpp configure
```
The project in 2014-ucpp is ready to go, but if necessary you can always re-configure it.
to build:
```shell
make
make deploy
```
(2014)\<Name\>
-----------

Version worked on by \<Name\>
