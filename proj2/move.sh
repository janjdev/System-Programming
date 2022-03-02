#!/bin/bash
#January Johnson
#Write a Bash shell script that takes a file as an argument, manipulates data within the file, and writes the output to several other files.
#This script will create five (5) text files in the current directory.

#Insert a heading line as follows: "Name Phone Number Jan Feb Mar", and output the resulting data to a file called move1.txt
sed '1s/^/Name Phone Number Jan Feb Mar\n/'  $1  > move1.txt 

#Duplicate the file from arguments to a new file called move2.txt, replace the name Hauschild with Housechild write the 
sed 's/Hauschild/Housechild/' move1.txt > move2.txt

#Put the list of donors (only their full name, no other data) with area code 916 in a file called move3.txt
grep "(916)" $1 | cut -d "," -f1 > move3.txt

#Anyone who's first names start with a M or R should go into a file called move4.txt, but only their first names.
egrep -o '(^\b[M]\w+)|(^\b[R]\w+)' $1 >  move4.txt

#Find the people who donated over $500.00 in any month, put their name and number in the file move5.txt
grep -oP "^([^,]*\,){2}.*(500)" $1 | egrep -o '(\w+\s\w+.+[0-9]{3}.+[0-9]{3}.+[0-9]{4})' | sort --key=2 > move5.txt
