#!/bin/bash
#January Johnson
#Create a command line interface that will continually prompt a user to see if they would like to execute a command from a list

#menu selection
choice=0

until [[ $choice = 6 ]]
do

    echo -e 'Select a command from the menu below by entering the corresponding number. Ex: 1.\n'

    #Command Menu
    echo -e '1. List the files in the current directory \n2. List the number of empty directories in a directory.\n3. Navigate to another directory and list the files.\n4. Search for and the list the number of files that contain a specific word.\n5. Move specified files to a specific diretory.\n6. Exit.\n' 
    read choice

case $choice in
1)
    echo -e 'Files in this directory are:'
    ls
    echo
    ;;

2)
    dir=""
    echo 'Enter the directory\n'
    read $dir

    
    
6) 
    exit 0
    ;;
esac

done
