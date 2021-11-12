# CMPE283

## Assignment 1 Questions
1. Team Members and Work Division: I did this by myself.
2. Steps for assignment
```
# the following steps will require installing packages that may not be included with the distro

# 1. build the linux kernel
# from linux directory
$ make -j 4 modules # build the modules
$ make -j 4 # build the kernel
$ sudo make INSTALL_MOD_STRIP=1 modules_install # install the built modules
$ sudo make install # install the kernel
$ sudo reboot # reboot for changes to take

# 2. build the module cmpe283-1.ko
$ make # in directory linux/cmpe283
$ chmod +x update-module.sh
$ ./update-module.sh (makes, removes old version, inserts and starts new version)

```
