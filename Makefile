CXX=g++
DEFINE=
ALL_FLGS= -O3
CC=-c
OUTPUT=extrapid
LD= -lpthread -ldl -lm
DEBUG=

extrapid:main.o log.o module.o cJSON.o bindthread.o
	$(CXX) main.o bindthread.o log.o module.o cJSON.o $(ALL_FLGS) $(LD) $(DEBUG) -o $(OUTPUT)

main.o:log.h cJSON.h main.cpp bindthread.cpp module.h moduletype.h cJSON.h
	$(CXX) main.cpp  $(ALL_FLGS) $(DEBUG) $(DEFINE) $(CC) -o main.o

bindthread.o:bindthread.cpp log.h module.h moduletype.h
	$(CXX) bindthread.cpp  $(ALL_FLGS) $(DEBUG) $(DEFINE) $(CC) -o bindthread.o
log.o:log.c log.h
	$(CXX) log.c  $(ALL_FLGS) $(DEBUG) $(DEFINE) $(CC) -o log.o

module.o:module.cpp module.h moduletype.h
	$(CXX) module.cpp  $(ALL_FLGS) $(DEBUG) $(DEFINE) $(CC) -o module.o

cJSON.o:cJSON.cpp cJSON.h
	$(CXX) cJSON.cpp $(ALL_FLGS) $(DEBUG) $(DEFINE) $(CC) -o cJSON.o

clean:
	rm *.o
