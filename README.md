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

## Assignment 2 Steps
question 2
```
# on every change to the cpuid.c and vmx.c code... 
$ make -j [cpus] modules
$ make -j [cpus] INSTALL_MOD_STRIP=1 modules_install
$ rmmod kvm_intel
$ rmmod kvm
$ modprobe kvm
$ lsmod | grep kvm
$ modprobe kvm_intel
$ lsmod | grep kvm_intel
# start the vm (so actually this can be challenging but depends on your machine)
#    for me I had to directly start it from qemu cli
```
modified cpuid.c and vmx.c to make implement handlers for 0x4fffffff 0x4ffffffe.
