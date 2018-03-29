CC=mips-openwrt-linux-gcc
OBJS=write_at_read_result.o leka_mz389_ecm.o

all = leka_mz389_ecm
$(all):$(OBJS)
	$(CC) -o $@ $^ -lpthread
write_at_read_result.o:write_at_read_result.c
	$(CC) -c $^ 
leka_mz389_ecm.o:leka_mz389_ecm.c 
	$(CC) -c $^
.PHONY:clean
clean:
		rm -rf *.o *.*~ *~ *.swap $(all)

