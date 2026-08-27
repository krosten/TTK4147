
# TTK4147 - Real-time Systems  

## Exercise - Basic tools and setup

This year you will be programming on a Raspberry Pi (RPi). These will be handed
out during the start of each lab-session, and collected after each lab-session.
Even though the RPi should behave similarly to a PC, there may be some
unforeseen issues, so please read the exercise instructions carefully and ask the assistants
if you still experience any issues. 

This first exercise is made so that everybody can learn and setup the basic tools we
will use during this course. It is intended more as a tutorial, but you are
expected to get the assignment approved by the end.

If you have questions, please apply for a spot in the queue. You will find the form on Canvas.

**It is important that you push your files to GitHub prior to leaving the lab,
and backup any files that you might have stored locally. The computers/RPi may
be reinstalled at any time, and all files deleted without any notice.**

## Raspberry Pi Kit

You should have received a complete set on your station consisting of:

- Raspberry Pi with power supply, HDMI-cable, keyboard, mouse and SD-card

- USB-A to SD-card adapter

- USB-C to USB-C power switch

## Storage

When you are done at the lab, put all the equipment from the RPi into the
box. Please hand in your set to the assistants before you leave.

## Connecting the hardware

Connect the power supply (and the switch), HDMI-cable, keyboard and mouse to the RPi. The RPi can be connected to the network wirelessly or by Ethernet.

## Installing the OS

The RPi does not have any operating system preinstalled, so you will need to do
that yourselves. Additionally, the RPi does not have any "hard-drive" with
non-volatile storage. Instead, we use an SD-card. This SD-card supplied in the
set probably contains an older version of the Raspberry Pi OS. Theoretically,
you could install any supported OS you would like, but we will be using the
Raspberry Pi OS Lite. To install the OS, follow the procedure below:

1. Start the lab PC and select Ubuntu in the GRUB, log in with username
   "student" and password "Sanntid15".

2. Go to <https://www.raspberrypi.com/software/> and download the Raspberry Pi
   Imager for Ubuntu, if it is not already installed.

3. Go to
   [Raspberry Pi OS Lite](https://downloads.raspberrypi.com/raspios_lite_arm64/images/raspios_lite_arm64-2026-06-19/2026-06-18-raspios-trixie-arm64-lite.img.xz)
   and download the Raspberry Pi OS Lite (64-bit). It is important to use the
   64-bit version!

4. Open the Raspberry Pi Imager.

5. Press "CHOOSE DEVICE", select "Raspberry Pi 4"

6. Press "CHOOSE OS", select "Use custom" at the bottom of the list and select the file you downloaded in
   step 3.

7. Insert the SD-card into the USB-A adapter and connect it to the lab PC.

8. Press "CHOOSE STORAGE" and select the SD-card.

9. Press write and wait until it finishes writing and verifiying the image.

10. Extract the micro-SD card from the SD-card and insert it into the RPi. The
   golden contacts should point upwards.

11. Power up the RPi. 

12. The supplied keyboard is in English (US), remember to set the correct locale during startup! 

13. It is recommended to set “student” as the username and “rts” as the password.

### Raspberry Pi OS Lite

When the RPi has started you can log in. If you used the recommended
setup, you should use “student” as the username and “rts” as the password.

To connect to RPi from another PC, you can use ssh:

    ssh username@ipaddress

Keep in mind, both the PC and RPi needs to be on the same network for ssh to work and you need to know the IP address of the Raspberry Pi to connect to it.
### Shell

The CLI is provided by one of several shell programs, most widely used are bash,
csh and tcsh. The GNU/Linux default is bash, and you can check what type of
shell your environment uses with the following command:

    echo $SHELL

There are many other environment variables such as `$`PATH (where the shell
searches for commands) and `$`HOME (the user home directory). In bash you can
use the `env` and `export` commands to view and modify the environment
variables. Find out more about `apropos`, `help`, `info`, and `man` by typing
for instance:

    man man

Hitting `q` in `man` will exit the program.  
Find out what the following commands do:

- `ls`

- `cd`

- `pwd`

- `cp`

- `mv`

- `rm`

Find a command that tells you the date and the time on the system.

### Directories and files

Your home directory is `/home/student`. You should create the following
structure: `/home/student/groupXX/ex1/` using the command `mkdir`. A simple
text-file can be created by using the `echo` command:

    echo -n "I do really love" > test.txt
    echo " TTK4147" >> test.txt

Try `du`, `file`, `cat` and `wc` on the file you just created:

    du -h test.txt
    file test.txt 
    cat test.txt
    wc test.txt

### Shell-scripts

It is possible to create small programs, or scripts, that are executed by the
shell. Create a file named script.sh containing the following:

```bash
#! /bin/bash
for i in $(seq 1 10); do
    echo "Hello world $i"
done
```

and execute it by typing

    bash script.sh

or

    chmod +x script.sh
    ./script.sh

What does the command `chmod` and the comment on the first line of the script
do?

### Jobs

To see the programs running on the system you can use `top`, `jobs` and `ps`. Do
this now. Execute the command `$ sleep 5`. Then try adding an `&` after the
command. What happens? Try running `jobs` to examine running processes.

Start `sleep 5` again, and press CTRL+C while the process is running. Try again
but use CTRL-Z instead. What is the difference between CTRL+C and CTRL+Z. What
does the command `fg` do?

### Editors

A programmer needs an editor to edit his code files. There are religious wars
among programmers (and especially GNU/Linux programmers) about what is the best
editor. Since Raspberry Pi OS Lite doesn't offer a desktop environment it is recommended to use VS Code or your preferred editor on the lab PC and transfer your files using `scp` to the Raspberry Pi. 

## Programming

### GCC

The first Linux C-program you should create in this course is “Hello world”. Use
your editor to create the "Hello world" C-program shown below.

```c
#include <stdio.h>
int main(int argc, char *argv[])
{
    printf("Hello world\n");
    return 0;
}
```

Store the file `hello.c` in the working directory created for this exercise. We
will learn more about the GNU compiler tools in later exercises. For now, try to
compile the program you just created with GCC:

    gcc -o hello hello.c

and run the program with:

    ./hello

### Makefile

You should now create a more advanced program, inspect the following C-code and
figure out what it does:

```c
#include <stdio.h>

int main(int argc, char *argv[])
{
    while(argc--)
        printf ("%s\n",*argv++);
    return 0;
}
```

Create a file name sort.c containing the following:

```c
#include <stdlib.h>

void sort(int count, char *array[])
{
    int x,y;
    char* holder;
    for(x=1; x<count; x++)
        for(y=1; y<count-1; y++)
            if(atoi(array[y]) > atoi(array[y+1])) {
                holder = array[y+1];
                array[y+1] = array[y];
                array[y] = holder;
            }
}
```

Make a separate file called main.c with a `main()` function, from which you call
to the `sort()` function, in order to sort the arguments list, and then print
out the results. An example of the main-file is:

```c
#include <stdio.h>
#include "sort.h"

int main(int argc, char *argv[])
{
    int i;
    
    printf("Arguments before sort: ");

    for (i=1;i<argc;i++)
        printf("%s ",argv[i]);
    printf("\n");
    
    sort(argc,argv);

    printf("Arguments after sort:  ");

    for (i=1;i<argc;i++)
        printf("%s ",argv[i]);
    printf("\n");

    return 0;
}
```

The sort.h file should only contain a declaration of the function:

```c
void sort(int count, char *array[]);
```

You can compile the files by using:

    gcc -o sort_args sort.c main.c

and run:

    ./sort_args 34 2 919 34 42 51353

This is fine for smaller programs, but with larger programs and multiple files
there is a problem: each time one of the files changes, all files are compiled
again. A waste of computing resources! Instead, create the Makefile below. Make
sure that there are TABs, not spaces in front of `gcc` and `rm`.

```makefile
# program executable name
TARGET = sort_args

# compiler flags
CFLAGS = -g -Wall

# linker flags
LDFLAGS = -g

# list of sources
SOURCES = $(shell find -name "*.c")

# default rule, to compile everything
all: $(TARGET)

# define object files
OBJECTS = $(SOURCES:.c=.o)

# link programs
$(TARGET): $(OBJECTS)
    gcc $(LDFLAGS) -o $@ $^

# compile
%.o : %.c
    gcc $(CFLAGS) -c -o $@ $<

# cleaning
clean:
    rm -f $(TARGET) $(OBJECTS)
```

This Makefile can be used (with minor changes) for all Linux C programs we will
develop in this course. It will compile and link all the source files in its
folder. It will generate an executable that is named `sort_args`. To change
this, edit the first line in the Makefile.

Build your program with the command `make`. Verify that the program is built
correctly. Next, make some changes in either `sort.c` or `main.c`, and verify
that only the changed files are re-compiled. You can also remove all generated
files with the command `make clean`.
