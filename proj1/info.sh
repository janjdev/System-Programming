#!/bin/bash
#January Johnson
#Write a Bash shell script named info.sh to output system information. This project is meant to acclimate students to the submission process.
#The file will remain open

echo "Name of script: $0"

echo
echo -n "Name of current working directory: " 
echo "$PWD" | sed 's!.*/!!'
echo " or " "$PWD" 


echo
echo -n "Name of UNIX machine: " 
hostname

echo
echo -n "Name of login shell: "
echo $SHELL

echo 
echo -n "Total number of parameters of the script: "
echo $#

echo 
echo -n "Current date and time: " 
date 

echo 
echo -n "Name of user: "
whoami

echo 
echo -n "Current number of users in the system: "
who | wc -l

echo
echo -n "Current time: "
date| awk '{print $5, $6 }'

echo
echo "Press enter to close"

read