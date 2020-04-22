build:
	gcc minimax.c -o minimax -Wall
run:minimax
	gcc minimax.c -o minimax
	./minimax -c2 test1_task2.in fisier.out
clean:prob
	rm ./minimax
