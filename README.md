rOS
===

A scratch operating system designed and developed in C.

Initial inspiration includes [ToAruOS](https://github.com/klange/osdev.git) as well as various articles at [OSDev.org](http://wiki.osdev.org/Main_Page).

To run, the [bochs x86 emulator](http://bochs.sourceforge.net/) will be required along with the SDL plugin, which can be obtained using `sudo apt-get install bochs-sdl`. Then, simply download the source and run the following commands:

```
make
cd bin
bochs
```

Then the bochs environment will start and when prompted by grub, enter the following:

```
kernel 200+100
boot
```
