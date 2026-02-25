## S-BOS

### Stupidly Basic Operating System

This is a basic uefi bootloader, as of current is has a basic file system as well as some options to shut down, boot to bios or change the text mode. It can also boot kernel which is the main feture.

This is the Boot menu of the OS:

![1744016685531](images/ReadMe/1744016685531.png)

This is the loading screen that you can see as it is booting up, It uses GOP to render the BMP image found in the EFI partision of the disk:

![1747760057392](images/ReadMe/1747760057392.png)

### Dependancys

The dependancys for this project are:

```plaintext
qemu
mingw64-gcc
```

### Credit

This project is heavily ins[pired by a tutorial serise from [Queso Fuego](https://www.youtube.com/playlist?list=PLT7NbkyNWaqZYHNLtOZ1MNxOt8myP5K0p) on youtube. I tried not to copy it directly, but parts of the project like the write_gpt are strait copys from his project.

A large potion of this project was also done through the use of the [UEFI docs](https://uefi.org/specs/UEFI/2.11/) and so that is an important source of knolage. If anybody would like to learn about UEFI OS Development I would highly recommend using both the sources mentioned here.
