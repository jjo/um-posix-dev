CFLAGS=-g -Wall $(COPTS)
TARGETS=hilos-3-condvars hilos-4-socketcli 
TARGETS_PTH=hilos-4-socketcli-pth
LDLIBS=-lpthread
LDLIBS_PTH=-lpth

LIBPTH=$(wildcard /usr/lib/libpth.a)
%-pth: %-pth.c
	$(CC) $(CFLAGS) $? $(LDLIBS_PTH) -o $@ 
ifneq ($(LIBPTH),)
TARGETS+=$(TARGETS_PTH)
else
$(warning OJO: No encontre' -lpth ...)
endif

all: $(TARGETS) 

clean:  
	rm -f *.o *~ $(TARGETS) $(TARGETS_PTH) $(LIBS) core
