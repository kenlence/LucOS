CROSS_COMPILE 	?= arm-linux-gnueabihf-
TARGET		  	?= lucos

CC 				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY 		:= $(CROSS_COMPILE)objcopy
OBJDUMP 		:= $(CROSS_COMPILE)objdump

LIBPATH			:=

INCDIRS 		:= .    \
                    kernel \
					drivers \
					apps

SRCDIRS			:= .    \
                    arm \
                    kernel \
					drivers \
					apps

INCLUDE			:= $(patsubst %, -I %, $(INCDIRS))

SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILENDIR		:= $(notdir  $(SFILES))
CFILENDIR		:= $(notdir  $(CFILES))

SOBJS			:= $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS			:= $(patsubst %, obj/%, $(CFILENDIR:.c=.o))
OBJS			:= $(SOBJS) $(COBJS)

VPATH			:= $(SRCDIRS)

.PHONY: clean

$(TARGET).elf : $(OBJS)
	$(LD) -Tlucos.lds -o $(TARGET).elf $^

$(SOBJS) : obj/%.o : %.S
	$(CC) -Wall -nostdlib -fno-builtin -c -g -std=gnu99 -O0 $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c
	$(CC) -Wall -nostdlib -fno-builtin -c -g -std=gnu99 -O0 $(INCLUDE) -o $@ $<

qemu: $(TARGET).elf
	qemu-system-arm -M mcimx6ul-evk -m 1024M  -kernel lucos.elf -serial mon:stdio

qemu-gdb: $(TARGET).elf
	qemu-system-arm -M mcimx6ul-evk -m 1024M  -kernel lucos.elf -S -s -serial mon:stdio

clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(COBJS) $(SOBJS)


