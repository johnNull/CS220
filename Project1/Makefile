SRC_FILES = ttt.c Makefile

test : ttt
	./ttt X
	./ttt O

ttt : ttt.c
	gcc -Wall -lm -g -o ttt ttt.c
	
clean :
	-rm ttt
	
submit : $(SRC_FILES)
	mkdir $(USER)_proj1
	cp $(SRC_FILES) $(USER)_proj1
	tar -cvzf $(USER)_proj1.tar.gz $(USER)_proj1
	rm -r $(USER)_proj1	
