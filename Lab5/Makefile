test : fact ackerman
	./fact 10
	./ackerman 3 2
	
fact : fact.c
	gcc -Wall -g -o fact fact.c
	
ackerman : ackerman.c
	gcc -Wall -g -o ackerman ackerman.c	
	
clean :
	-rm fact ackerman 