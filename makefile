CFLAGS = -Wall /O2

OBJS = main.obj

all: clean $(OBJS)
   $(CC) $(CFLAGS) $(OBJS) /Fe:inquisitor.exe

clean:
   del *.obj