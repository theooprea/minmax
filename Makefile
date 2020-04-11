build:
	gcc minimax.c -o minimax
run:minimax
	gcc minimax.c -o minimax
	./minimax -c2 test_simple.in fisier.out
clean:prob
	rm ./minimax
