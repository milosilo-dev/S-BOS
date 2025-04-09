## S-BOS

### Stupidly Basic Operating System

This is a basic operating system that I made, at the moment it is not much more than a EFI program with some boot loader fetures. As of current is has a basic file system as well as some options to shut down, boot to bios or change the text mode.

I am going to implement GOP support soon but for the moment it uses the Simple Text Output Protocol to display UI as you can see in the image bellow:

![1744016685531](images/ReadMe/1744016685531.png)

### Dependancys

The dependancys for this project are:

```plaintext
qemu
mingw64-gcc
```

### Credit

This project is heavily ins[pired by a tutorial serise from [Queso Fuego](https://www.youtube.com/playlist?list=PLT7NbkyNWaqZYHNLtOZ1MNxOt8myP5K0p) on yt. I tried not to copy it directly, but parts of the project like the write_gpt are strait copys from his project.

A large potion of this project was also done through the use of the UEFI docs and so that is an important source of knolage. If anybody would like to learn about UEFI OS Development i would highly recommend using both the sources mentioned here.
