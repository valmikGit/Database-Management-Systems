CC = gcc

DRIVERS = driver_session1a driver_session1b driver_session1c driver_session1d

all: $(DRIVERS)

$(DRIVERS) : % : %.c IMT2022020_lab1.c
	$(CC) -o $@ $^

clean:
	rm $(DRIVERS) driver_session1a.txt driver_session1c.txt

write_a:
	./driver_session1a driver_session1a.txt
read_a:
	./driver_session1b driver_session1a.txt
write_c:
	./driver_session1c driver_session1c.txt
read_c:
	./driver_session1d driver_session1c.txt