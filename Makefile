CROSS_COMPILE 	?= arm-linux-gnueabihf-
TARGET		  	?= lucos

V			    := @
CC 				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY 		:= $(CROSS_COMPILE)objcopy
OBJDUMP 		:= $(CROSS_COMPILE)objdump

LIBPATH			:=

INCDIRS 		:= .    \
                    kernel \
					drivers \
					apps \
					include

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
OBJS			:= $(SOBJS) $(COBJS) $(CPPOBJS)

VPATH			:= $(SRCDIRS)

.PHONY: clean

$(TARGET).elf : $(OBJS)
	$(V) echo [LD] $@
	$(V) $(LD) -Tlucos.lds -o $(TARGET).elf $^

$(SOBJS) : obj/%.o : %.S
	$(V) printf "[CC] %-30s -> %-30s\n" $< $@
	$(V) $(CC) -Wall -nostdlib -fno-builtin -c -g -std=gnu99 -O0 $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c
	$(V) printf "[CC] %-30s -> %-30s\n" $< $@
	$(V) $(CC) -Wall -nostdlib -fno-builtin -c -g -std=gnu99 -O0 $(INCLUDE) -o $@ $<

qemu: $(TARGET).elf
	qemu-system-arm -M mcimx6ul-evk -m 1024M  -kernel lucos.elf -serial mon:stdio

qemu-gdb: $(TARGET).elf
	qemu-system-arm -M mcimx6ul-evk -m 1024M  -kernel lucos.elf -S -s -serial mon:stdio

clean:
	$(V) rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(COBJS) $(SOBJS)
