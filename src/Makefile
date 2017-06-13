PROG = spirest

CFLAGS = -g -W -Wall -Werror -Wno-unused-function -D MG_DISABLE_MQTT -D MG_DISABLE_COAP
LDFLAGS += -pthread

all: $(PROG)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)

spirest: spirest.o mongoose.o si7021.o

si7021.o: si7021.c si7021.h

spirest.o: spirest.c mongoose.h si7021.h

mongoose.o: mongoose.c mongoose.h

mongoose.c:
	wget https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.c

mongoose.h:
	wget https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.h
