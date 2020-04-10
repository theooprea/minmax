build:
	gcc minmax.c -o minmax
run:minmax
	gcc minmax.c -o minmax
	./minmax -c1 test1.in fisier.out
clean:prob
	rm ./minmax
