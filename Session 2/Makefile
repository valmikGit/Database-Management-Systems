CC = gcc

DRIVERS = driver_session2a driver_session2b driver_session2c driver_session2d

all: $(DRIVERS)

$(DRIVERS) : % : %.c IMT2022020_lab2.c
	$(CC) -o $@ $^

clean:
	rm $(DRIVERS) driver_session2a.bin driver_session2c.bin

write_a:
	./driver_session2a driver_session2a.bin
read_a:
	./driver_session2b driver_session2a.bin
write_c:
	./driver_session2c driver_session2c.bin
read_c:
	./driver_session2d driver_session2c.bin