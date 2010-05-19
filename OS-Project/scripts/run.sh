#!/bin/sh
#
# Argument: MIPS binary to run
#
# Before running this script, the environment variable
# SIMICS should be set to the path where simics is installed.
# For example, 
#
# export SIMICS=~/yourpath/simics/
#

HERE=$PWD
cd $SIMICS
./simics -fast -e "\$mips_binary=$HERE/$1" -x $HERE/run.simics

