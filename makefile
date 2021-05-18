CFLAGS = -Wall /O2 /GL

OBJS = main.obj

all: clean $(OBJS)
   $(CC) $(CFLAGS) $(OBJS) /Fe:inquisitor.exe /LTCG

clean:
   del *.obj
