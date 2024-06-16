# EtherOS
EtherOS is an experimental operating system based on i386.

The goal of this project is to create OS with graphics server
that works as web browser and shell as web application.

### Building
To build this operating system, you must have binutils and gcc
built for i386 (or i686). You can find the build instructions
[here](https://wiki.osdev.org/GCC_Cross-Compiler).

Also, you will need to have qemu installed. Eventually, you
should have qemu-system-x86_64 to run the OS.
The next step is to clone the repository:
```shell
git clone https://github.com/samsonium/ether-os.git
```

Then you can build and run the operating system using:
```
make
```

### Contibuting
If you have ideas for improving this project, please use the issues
tab of this repository to propose your idea or submit a pull request.
