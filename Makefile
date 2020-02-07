CFLAGS += -std=c99 -D_GNU_SOURCE

mem-stress: mem-stress.o
	$(CC) -static -static-libgcc -o $@ $^ -lpthread

%.o: Makefile

clean:
	$(RM) mem-stress
	$(RM) mem-stress.o

.PHONY: image

image:
	imagebuilder -t frobware/mem-stress .

run:
	docker run frobware/mem-stress
