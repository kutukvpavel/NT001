CFLAGS = -g

build:
	$(CC) -c reg_desc.cpp -o reg_desc.o
	$(CC) -c main.cpp -o main.o
	$(CC) main.o reg_desc.o -o usermode

tidy:
	$(RM) *.o *.mod *.mod.c *.symvers *.order
	$(RM) -f .??*

clean:
	echo "not impl"