SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

TARGET= lucas_kanade
%.o : %.c 
	gcc -c $^ -o $@ -g

$(TARGET): $(OBJECTS)
	gcc $^ -o $@

clean: 
	rm $(TARGET) $(OBJECTS)
