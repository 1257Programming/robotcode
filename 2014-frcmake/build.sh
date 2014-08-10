#!/bin/bash
if [ $# -eq 0 ]
then
	cfg="Default"
	tgt="all"
else
	cfg=$1
	shift
	tgt=$@
fi
if [[ $1 == "deploy" ]]
then
	if [ ! -d "bin" ]
	then
		echo "Not built yet!"
		exit 1
	else
		cd bin
		if [[ $cfg == "Default" ]]
		then
			if [ ! -f "FRC_UserProgram.out" ]
			then
				echo "Configuration not built yet!"
				exit 1
			fi
		else
			if [ ! -d $cfg ]
			then
				echo "Configuration not built yet!"
				exit 1
			fi
			cd $cfg
			if [ ! -f "FRC_UserProgram.out" ]
			then
				echo "The binary has been deleted. Please rebuild it."
				exit 1
			fi
		fi
		echo "Pushing code..."
		wput -u FRC_UserProgram.out ftp://anonymous@10.12.57.2/ni-rt/system/FRC_UserProgram.out
	fi
else
	mkdir -p .$cfg
	cp *.cpp .$cfg
	cp *.h .$cfg
	cp CMakeLists.txt .$cfg
	cd .$cfg
	frcmake
	make $tgt
	rm 1257Robot.cpp 1257Robot.h
	if [ $cfg != "Default" ]
	then
		mkdir -p ../bin/$cfg
		cp FRC_UserProgram.out ../bin/$cfg
	else
		mkdir -p ../bin
		cp FRC_UserProgram.out ../bin
	fi
	cd ..
	rm -rf .$cfg
fi
