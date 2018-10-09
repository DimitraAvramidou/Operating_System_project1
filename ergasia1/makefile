all:feeder  consumer

feeder: feeder.o
	gcc feeder.o -o feeder

consumer: consumer.o
	gcc consumer.o -o consumer

feeder.o : feeder.c
		gcc -c feeder.c

consumer.o : consumer.c
		gcc -c consumer.c

clean:
		rm -f *.o feeder consumer
