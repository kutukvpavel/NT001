KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`

tidy:
	$(RM) *.o *.mod *.mod.c *.symvers *.order
	$(RM) -f .??*

clean:
	make -C $(KDIR) M=`pwd` clean