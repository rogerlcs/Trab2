all: libbanco.a programa

objetos/%.o: sources/%.c headers/%.h
	gcc -g -c $< -o $@ -I headers

libbanco.a: objetos/bancodedados.o objetos/documento.o objetos/palavra.o objetos/tabela.o
	ar -crs libbanco.a objetos/bancodedados.o objetos/documento.o objetos/palavra.o objetos/tabela.o

programa: indice.c main.c
	gcc -g -o indice indice.c -I headers -L . -lbanco -lm
	gcc -g -o main main.c -I headers -L . -lbanco -lm

clean:
	rm -f main libbanco.a objetos/*.o *.o