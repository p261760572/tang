#!/bin/ksh
#	This is the default standard profile provided to a user.
#	They are expected to edit it to meet their own needs.
#
#	If you have a profile that is attempting to read from the
#	terminal or is using the stty(1) command to set terminal 
#	settings, you may find that GRAPHICAL LOGIN does not give
#	you a desktop metaphor session.
#
#	To update your profile, you should surround any section
#	that reads from the terminal or performs stty(1) commands
#	with the following 'if' statement:
#
#	if [ "$XDM_LOGIN" != "yes" ]
#	then
#		:
#		read from the terminal or stty(1) commands
#	fi


if [ -f $HOME/.kshrc -a -r $HOME/.kshrc ]; then
	ENV=$HOME/.kshrc		# set ENV if there is an rc file
	export ENV
fi
FEP_RUNPATH=$HOME/run/
FEP_SRCPATH=$HOME/src
export FEP_RUNPATH FEP_SRCPATH

POSSIMU_SRCPATH=$HOME/possimu/src
POSSIMU_RUNPATH=$HOME/possimu/run/
export POSSIMU_SRCPATH POSSIMU_RUNPATH
CDPATH=.:$CDPATH:$POSSIMU_RUNPARH:$POSSOMU_SRCPATH:$FEP_RUNPATH:$FEP_SRCPATH:$HOME/run/script
export CDPATH

PATH=$PATH:/usr/sbin:$FEP_RUNPATH/bin:$FEP_RUNPATH/script:$POSSIMU_RUNPATH\bin
export PATH
