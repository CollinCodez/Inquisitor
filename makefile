CFLAGS = -Wall /O2 /GL

OBJS = main.obj \
   oracle.obj

LIBS = \
   advapi32.lib \
   user32.lib

all: clean $(OBJS)
   $(CC) $(CFLAGS) $(OBJS) $(LIBS) /Fe:inquisitor.exe /LTCG

clean:
   del *.obj
