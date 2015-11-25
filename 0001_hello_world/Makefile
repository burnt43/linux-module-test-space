obj-m+=hello.o

all:
	make -C /lib/modules/4.1.13-1-ARCH/build/ M=`pwd` modules
clean:
	make -C /lib/modules/4.1.13-1-ARCH/build/ M=`pwd` clean
