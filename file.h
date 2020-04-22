#ifndef __FILE_H__
#define __FILE_H__
//structura definita pt arborele de la taskul 1, contine o matrice care va avea
//rolul de tabla de joc, pointer la lista de copii, la parinte, la fratele next
//la fratele prev (in lista de copii a parintelui), numarul de copii dar si
//jucatorul care va avea de pus o bila pe tabla
typedef struct arbore_part_1 {
	char **game_matrix;
	struct arbore_part_1 *child;
	struct arbore_part_1 *next;
	struct arbore_part_1 *prev;
	struct arbore_part_1 *parent;
	int number_of_children;
	char player;
} arbore_part_1_node;
//structura pentru taskul 2 si 3, contine valoarea pe care o va avea nodul,
//valoarea alpha, valoarea beta si tipul nodului (min/max), pointeri la lista
//de copii, la parinti, la next si prev in lista de copii a parintelui
typedef struct arbore_part_2 {
	int value;
	int alpha, beta;
	struct arbore_part_2 *child;
	struct arbore_part_2 *next;
	struct arbore_part_2 *prev;
	struct arbore_part_2 *parent;
	//tipul de calcul de facut, -1 pt mini, 1 pt max, 0 pt frunza
	int type;
} arbore_part_2_node;
//structura de nod in arbore pt bonus, are un vector de valori ce va fi alocat
//dinamic, un pointer la lista de copii, la parinte, la next si prev si tipul
//de nod care este, min sau max
typedef struct arbore_bonus {
	int *values;
	struct arbore_bonus *child;
	struct arbore_bonus *next;
	struct arbore_bonus *prev;
	struct arbore_bonus *parent;
	//tipul de calcul de facut, -1 pt mini, 1 pt max
	int type;
} arbore_bonus_node;
#endif /* __FILE_H__ */
