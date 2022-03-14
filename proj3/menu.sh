#!/bin/bash
#January Johnson
#Create a command line interface that will continually prompt a user for a command from a list to execute.
#If the user enters option 6 or a  '|' or ctl+c the program will exit.

#menu selection
choice=0

#exit function
function exitProgram()
{
    total=1
    echo -e '\nExiting Program\n'
    while [ $total -le 3 ]; do
        echo .| tr '\n' ' '
        sleep 1
        total=$(($total + 1))
    done
    echo Goodbye
    exit 0
}

#Catch interrupts and exit gracefully
trap exitProgram SIGINT


#Perform commands until the user exits
until [[ $choice = 6 ]]
do
    echo -e 'Select a command from the menu below by entering the corresponding number. Ex: 1.\n'

    #Command Menu
    echo -e '1. List the files in the current directory \n2. List the number of empty directories in a directory.\n3. Navigate to another directory and list the files.\n4. Search for and the list the number of files that contain a specific word.\n5. Move specified files to a specific diretory.\n6. Exit.\n' 
    read choice

    #Check for '|' in input. Exit program if '|' entered
    if  [[ $choice == *"|"* ]]; then
        echo "a | symbol was entered" >> logerr.txt
        echo -e 'A | symbol was found. \nExiting Program\n'
        total=1
        while [ $total -le 3 ]; do
                echo . | tr '\n' ' '
                sleep 2
                total=$(($total + 1))
        done
        echo Goodbye
        sleep 1
        exit 0
    else

    #Menu commands to perform
        case $choice in
        1)
            pwd
            echo
            echo -e 'Files in this directory are:'
            ls -p | grep -v / | tr '\n' ' ' 
            echo
            echo
            ;;

        2)
            echo -e 'Enter the directory. Enter 7 to go to the main menu.\n'
            read dir
            if [ $dir = 7 ]; then
                    continue;
            fi
            if [ ! -d "$dir" ]; then
                find $dir -type d 2>> logerr.txt
                noDir=true
            fi
            while [[ $noDir = true ]]
            do 
                echo -e 'No such directory exist. Please, enter a directory. Enter 7 to go to the main menu.\n'
                read dir
                if [ $dir = 7 ]; then
                    continue 2;
                fi
                if [ ! -d "$dir" ]; then
                    find $dir -type d 2>> logerr.txt
                    noDir=true
                else noDir=false
                fi
            done
            find $dir -empty -type d | wc -l
            find $dir -empty -type d | tr '\n' ' ' 
            echo
            ;;

        3)
            echo -e 'Enter the directory. Enter 7 to go to the main menu.\n'
            read dir
            if [ $dir = 7 ]; then
                    continue;
            fi
            if [ ! -d "$dir" ]; then
                find $dir -type d 2>> logerr.txt
                noDir=true
            fi
            while [[ $noDir = true ]]
            do 
                echo -e 'No such directory exist. Please, enter a directory. Enter 7 to go to the main menu.\n'
                read dir
                if [ $dir = 7 ]; then
                    continue 2;
                fi
                if [ ! -d "$dir" ]; then
                    find $dir -type d 2>> logerr.txt
                    noDir=true
                else noDir=false
                fi
            done
            cd $dir && ls -l
            echo
            ;;

        4)
            echo -e 'Enter the word to look for.\n'
            read word
            grep -r -i -l "$word" * | wc -l
            echo
            ;;

        5)
            echo -e 'Enter the name of the directory.\n'
            read dir
            echo -e 'Enter the file(s) to move separated by a space. '
            read files

            mv $files $dir
            
            ;;
            
        6) 
            echo -e '\nExiting Program\n'
            sleep 1
            echo Goodbye
            exit 0
            ;;
        esac
    fi

done

