SRC_FILES = warehouse.c slots.c slots.h Makefile

test : packem
	./packem <order1.txt
	
packem : warehouse.c slots.h slots.c
	gcc -Wall -g -o packem warehouse.c slots.c
	
submit : $(SRC_FILES)
	mkdir $(USER)_proj2
	cp $(SRC_FILES) $(USER)_proj2
	tar -cvzf $(USER)_proj2.tar.gz $(USER)_proj2
	rm -r $(USER)_proj2	
	
clean : 
	-rm packem $(USER)_proj2.tar.gz