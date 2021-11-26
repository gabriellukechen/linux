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

## Assignment 2 Questions 
### Question 1
I worked by myself

### Question 2
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
# ran test script that prints out all of the 70 exits into files.
```
#### testing the code, and running exit experiments
1. start the vm (so actually this can be challenging but depends on your machine) for me I had to directly start it from qemu cli
2. ran cronjob on reboot to see how many run on reboot 
3. ran test scripts that write files to see how exits scale over time, etc.
4. ran test script that prints out all the 70 exits into files
5. copied these values into spread sheets, raw files can be seen in .txt files in the cmpe283 folder

## Assignment 3 Questions
### Question 3
Frequency of exits
- Exits increase at steady rate?
```
No, the rate of increase depends on the processes being run by the CPU. For example, 10 cpuid, only increases when cpuid is called. 
```

- More exits performed during certain VM operations?
```
The most exits occur during I/O operations, exit reason 30 
```

- Exits per boot?
To examime this, I ran a cronjob on reboot. Did this by modifying /etc/crontab
```
# CPUID Output
CPU 0:
   0x4fffffff 0x00: eax=0x00a10cc5 ebx=0xffffa61f ecx=0x00000000 edx=0xffff96af
```
Approximately 10554565

### Question 4
Most and Least Frequent?
The most frequent exits are for I/O operations (exit reason 30). Other frequent ones included EPT Violation 48, and WRMSR 32. CPUID was also high but that's because it was called many times for the same of this experiment. There were also many exit reasons that were never recorded because there were no occurences. Exit reasons that occurred very few times but still were recorded were 54 WBINVD and 29 MOV DR, both occurring only 63 and 14 times respectively over a period of a few minutes.
