# Femto Shell is a program in c language that echoes any command entered by the user

## The shell will have only one built-in command "exit". If the user entered "exit", the shell will reply with "Good Bye" and terminate

### To Compile Program

    gcc -c Femto_Shell.c -o Femto_Shell.o

### To Generate Executable

    gcc Femto_Shell.o -o myFemtoShell

### To Run Executable

    ./myFemtoShell


## Example

    my prompt > Hellooooo
    You Said: Hellooooo
    my prompt > How are u?
    You Said: How are u?
    my prompt > Why So Serious!!
    You Said: Why So Serious!!
    my prompt > exit
    GoodBye :)
