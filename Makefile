SOURCES=$(wildcard *.c)

lucas_kanade: main.c lucas_kanade.c
	gcc $^ -o $@

clean: 
	rm $(lucas_kanade) 
