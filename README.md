# LucOS
This is a small OS run in micimx6ul-eck qemu, this is a project for myself to pratice operating system, I hope I can help others if I can.

## How to run
You need arm-linux-gnueabihf- toolchain to compiler and qemu-system-arm to run it.  
make: Compile project  
make qemu: Compile and run in qemu  
make qemu-gdb: Compile, run and open gdb server, the program will stop in ResetHandler until you use gdb-multiarch to control it. Use gdb-multiarch and gdb.init to debug it.  
make clean: Clean the project.  
