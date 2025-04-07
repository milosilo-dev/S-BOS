## S-BOS

##### Stupidly Basic Operating System

This is a basic operating system that I made, at the moment it is not much more than a EFI program with some boot loader fetures. As of current is has a basic file system as well as some options to shut down, boot to bios or change the text mode.

I am going to implement GOP support soon but for the moment it uses the Simple Text Output Protocol to display UI as you can see in the image bellow:

![1744016685531](images/ReadMe/1744016685531.png)

The dependancys for this project are:

```plaintext
qemu
mingw64-gcc
```

The name of this repo also needs to be chnaged at some point to S-BOS as this is the name that i actualy want to use for the project rather than the non descript "UEFI OS".
