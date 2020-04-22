#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

//in functia definita mai jos verific daca punctul din matrice pe care
//il trimit ca parametru  are 4 bile de aceiasi culoare conectate, in cele 8
//directii, sus jos, stanga dreapta si cele 4 diagonale
//daca e gameover, functia intoarce 1, altfel intoarce 0
int check_point_in_matrix(char **game_matrix, int index_row,
int index_column, int nr_rows, int nr_column)
{
	char colour = game_matrix[index_row][2 * index_column];
	//verific in sus
	if (index_row >= 3) {
		if (game_matrix[index_row - 1][2 * index_column] == colour &&
		game_matrix[index_row - 2][2 * index_column] == colour
		&& game_matrix[index_row - 3][2 * index_column] == colour)
			return 1;
	}
	//verific in jos
	if (index_row + 3 < nr_rows) {
		if (game_matrix[index_row + 1][2 * index_column] == colour &&
		game_matrix[index_row + 2][2 * index_column] == colour
		&& game_matrix[index_row + 3][2 * index_column] == colour)
			return 1;
	}
	//verific in stanga
	if (index_column >= 3) {
		if (game_matrix[index_row][2 * index_column - 2] == colour &&
		game_matrix[index_row][2 * index_column - 4] == colour
		&& game_matrix[index_row][2 * index_column - 6] == colour)
			return 1;
	}
	//verific in dreapta
	if (index_column + 3 < nr_column) {
		if (game_matrix[index_row][2 * index_column + 2] == colour &&
		game_matrix[index_row][2 * index_column + 6] == colour
		&& game_matrix[index_row][2 * index_column + 4] == colour)
			return 1;
	}
	//verific in dreapta sus pe diagonala
	if (index_row >= 3 && index_column + 3 < nr_column) {
		if (game_matrix[index_row - 1][2 * index_column + 2] == colour &&
		game_matrix[index_row - 2][2 * index_column + 4] == colour
		&& game_matrix[index_row - 3][2 * index_column + 6] == colour)
			return 1;
	}
	//verific in dreapta jos pe diagonala
	if (index_row + 3 < nr_rows && index_column + 3 < nr_column) {
		if (game_matrix[index_row + 1][2 * index_column + 2] == colour &&
		game_matrix[index_row + 2][2 * index_column + 4] == colour
		&& game_matrix[index_row + 3][2 * index_column + 6] == colour)
			return 1;
	}
	//verific in stanga jos
	if (index_column >= 3 && index_row + 3 < nr_rows) {
		if (game_matrix[index_row + 1][2 * index_column - 2] == colour &&
		game_matrix[index_row + 2][2 * index_column - 4] == colour
		&& game_matrix[index_row + 3][2 * index_column - 6] == colour)
			return 1;
	}
	//verific in stanga sus
	if (index_column >= 3 && index_row >= 3) {
		if (game_matrix[index_row - 1][2 * index_column - 2] == colour &&
		game_matrix[index_row - 2][2 * index_column - 4] == colour
		&& game_matrix[index_row - 3][2 * index_column - 6] == colour)
			return 1;
	}
	return 0;
}

//in functie verific pt fiecare punct din matrice daca indeplineste conditia
//de game over, parcurg intreaga matrice si verific fiecare punct, daca gaseste
//unul bun se opreste
int is_game_over(arbore_part_1_node *nod, int nr_rows, int nr_columns)
{
	int i, j;

	for (i = 0; i < nr_rows; i++) {
		for (j = 0; j < nr_columns; j++) {
			//verific punctul la care am ajuns, tinand cont de faptul ca sunt
			//si spatii " " in matrice, sar peste acelea
			if (nod->game_matrix[i][2 * j] != '-' &&
			check_point_in_matrix(nod->game_matrix, i, j, nr_rows, nr_columns))
				return 1;
		}
	}
	return 0;
}

//functie folosita sa initializez un nod in arborele folosit pt taskul 1 din
//tema, ii aloc dinamic matricea care e pe post de tabla de joc, ii initializez
//legaturile next, prev, child, parent cu NULL si ii pun jucatorul care urmeaza
//sa faca o mutare
arbore_part_1_node *new_node(int nr_rows, int nr_columns)
{
	arbore_part_1_node *new_node = malloc(sizeof(arbore_part_1_node));

	//aloc prima data numarul de linii din matrice
	new_node->game_matrix = malloc(nr_rows * sizeof(char *));
	int i;

	//pe fiecare linie aloc dinamic marimea liniei
	for (i = 0; i < nr_rows; i++) {
		new_node->game_matrix[i] = malloc((2 * nr_columns) * sizeof(char));
	}
	new_node->parent = NULL;
	new_node->child = NULL;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->number_of_children = 0;
	//pun jucatorul care urmeaza sa faca o mutare
	new_node->player = '\0';
	return new_node;
}

//functie care creaza o matrice, pornind de la cea a parintelui si punand in
//functie de parametrii dati, adica pe indexul coloanei dat ca parametru va
//cauta cea mai joasa pozitie (gravitatia care atrage piesele in jos) si face
//modificarea in noua matrice, pe care o va pune in alt nod
void create_matrix(char **game_matrix, char **parents_game_matrix,
int indx_column, char player, int nr_rows, int nr_columns)
{
	int i, j;

	//prima data copiez in matricea noului nod matricea parintelui
	for (i = 0; i < nr_rows; i++) {
		for (j = 0; j < 2 * nr_columns; j++) {
			game_matrix[i][j] = parents_game_matrix[i][j];
		}
	}
	i = 0;

	//caut cea mai joasa pozitie in coloana data ca parametru
	while (i < nr_rows && game_matrix[i][indx_column] == '-') {
		i++;
	}

	//fac modificarea in functie de ce jucator trebuie sa mute
	game_matrix[i - 1][indx_column] = player;
}

//functie care adauga un nod arbore in lista de copii a unui nod, care ii
//asigneaza in acelasi timp ce jucator va trebui sa mute urmatorul
void add_child_node(arbore_part_1_node **head,
arbore_part_1_node *nod_nou, arbore_part_1_node *parent_node)
{
	//pune in lista nodul respectiv
	if ((*head) == NULL) {
		nod_nou->next = *head;
		*head = nod_nou;
	}
	else {
		arbore_part_1_node *i = *head;

		while (i->next) {
			i = i->next;
		}
		i->next = nod_nou;
		nod_nou->prev = i;
	}
	nod_nou->parent = parent_node;
	//ii asigneaza urmatorul jucator
	if (parent_node->player == 'R') {
		nod_nou->player = 'B';
	}
	else {
		nod_nou->player = 'R';
	}
}

//functie care pt o matrice data, adica o stare anume de joc ii creaza toate
//starile urmatoare, toate posibilitatile urmatoare, creaza nodurile respectiva
//si le pune in lsita copiilor intr-o maniera recursiva
void create_all_nodes(arbore_part_1_node *nod, int nr_rows, int nr_columns)
{
	if (nod == NULL)
		return;

	if (is_game_over(nod, nr_rows, nr_columns) == 0) {
		int i;
		//pe fiecare coloana care nu e plina pune tokenul jucatorului care
		//trebuie sa mute
		for (i = 0; i < nr_columns; i++) {
			if (nod->game_matrix[0][2 * i] == '-') {
				//creaza noul nod in functie de indicele coloanei la care
				//se afla
				arbore_part_1_node *nod_nou = new_node(nr_rows, nr_columns);
				create_matrix(nod_nou->game_matrix, nod->game_matrix, 2 * i,
				nod->player, nr_rows, nr_columns);
				add_child_node(&(nod->child), nod_nou, nod);
				nod->number_of_children++;
			}
		}
	}
	//fac apelul recursiv pt a crea toate nodurile, e o parcurgere in adancime,
	//prima data merge pe cat posibil adanc apoi trece la fratii fiecarui nod
	//deja parcurs si creez pt fiecare in parte starile urmatoare pana cad da
	//de stari game over
	create_all_nodes(nod->child, nr_rows, nr_columns);
	create_all_nodes(nod->next, nr_rows, nr_columns);
}

//functie care returneaza ca int numarul de taburi necesare pt a se diferentia
//nivelurile in arbore, cu cat e mai departe de root, cu atat creste numarul de
//taburi
int number_of_needed_tabs(arbore_part_1_node *nod)
{
	int number_of_tabs = 0;

	//iau un nod auxiliar cu care merg continuu spre parinte si numar de cate
	//ori avansez, cand da de NULL a ajuns la radacina si se opreste programul,
	//returnand nr de niveluri pana la root
	arbore_part_1_node *aux = nod;

	while (aux->parent != NULL) {
		number_of_tabs++;
		aux = aux->parent;
	}
	return number_of_tabs;
}

//functie de ajutor pt cea de afisare, care scrie in fisierul de iesire numarul
//necesar de taburi pt fiecare nod, calculat cu functia de sus
void print_tabs(arbore_part_1_node *nod, FILE *fisier_out)
{
	int number_of_tabs = number_of_needed_tabs(nod);

	int i;

	for (i = 0; i < number_of_tabs; i++) {
		fprintf(fisier_out, "\t");
	}
}

//functie care printeaza recursiv in fisier matricile de joc
//folosind functiile de mai sus, apoi face o parcurgere in adancime si pe
//fiecare nod ii afiseaza matricea, incepand cu radacina
void print_matrix(arbore_part_1_node *nod, int nr_rows, int nr_columns,
FILE *fisier_out)
{
	if (nod == NULL)
		return;
	
	int i;

	for (i = 0; i < nr_rows; i++) {
		//printez prima data taburile necesare apoi randurile din matrice
		print_tabs(nod, fisier_out);
		fprintf(fisier_out, "%s\n", nod->game_matrix[i]);
	}
	fprintf(fisier_out, "\n");

	//fac o parcurgere in adancime iar in fiecare nod ii afisez matricea
	print_matrix(nod->child, nr_rows, nr_columns, fisier_out);
	print_matrix(nod->next, nr_rows, nr_columns, fisier_out);
}

//functie de eliberare din memorie a arborului intrucat am alocat dinamic
//aproape tot, trebuie eliberat, am un fel de parcurgere in latime si dupa ce
//am ajuns la frunze eliberez prima data randurile din matrice, apoi matricea
//in sine si apoi nodul respectiv
void free_everything(arbore_part_1_node *nod, int nr_rows, int nr_colums)
{
	if (nod == NULL)
		return;

	//fac parcurgerea recursiva
	free_everything(nod->next, nr_rows, nr_colums);
	free_everything(nod->child, nr_rows, nr_colums);

	int i;

	//eliberez randurile
	for (i = 0; i < nr_rows; i++) {
		free(nod->game_matrix[i]);
	}

	//eliberez matricea complet apoi nodul
	free(nod->game_matrix);
	free(nod);
}

//functie de a afisa o singura matrice, folosita la printarea din timpul
//facerii temei pt debugging relativ mai usor
void print_single_matrix(arbore_part_1_node *nod, int nr_rows, int nr_columns)
{
	int i;

	for (i = 0; i < nr_rows; i++) {
		printf("%s\n", nod->game_matrix[i]);
	}
}

//functie care aloca dinamic si returneaza un nod folosit la taskul 2 si 3,
//initializeaza toate legaturile cu NULL si pune valorile initializate unde e
//nevoie, am initializat cu 0 type pt taskul 2 daca e min sau max dar si alpha
//si beta pt taskul 3, pe care le voi initializa in main si transferate
//recursiv in functia respectiva
arbore_part_2_node *new_node_part_2()
{
	arbore_part_2_node *nod_nou = malloc(sizeof(arbore_part_2_node));

	nod_nou->value = 0;
	nod_nou->type = 0;
	nod_nou->alpha = 0;
	nod_nou->beta = 0;
	nod_nou->child = NULL;
	nod_nou->parent = NULL;
	nod_nou->next = NULL;
	nod_nou->prev = NULL;
	return nod_nou;
}

//functie folosita la taskul 2 pt a trece un sir de caractere de genul (numar)
//sau [numar] intr-un numar int cu valoarea din paranteze, e folosita de alte
//functii pt a prelucra inputul
int braket_to_int(char *line)
{
	//scap de prima paranteza apoi de ultima punand terminator de sir si apoi
	//fac trecerea la int folosind functia predefinita atoi
	char *p = line + 1;
	p[strlen(p) - 1] = '\0';
	int nr = atoi(p);
	return nr;
}

//functie care primeste ca parametru o linie din fisierul de input, imparte
//linia in mai multe siruri de paranteze de genul (numar) sau [numar] care vor
//fi prelucrate cu functia de mai sus si puse in vectorii dati ca parametrii
void transform_row(char *line, int *vector_values, int *vector_type_of_brakets)
{
	int i = 0;

	char *p = strtok(line, " ");
	while (p != NULL) {
		//verific daca e paranteza rotunda sau patrata pt a stii ce sa fac
		//mai departe cu valoarea din vectorul de valori
		if (p[0] == '(') {
			vector_type_of_brakets[i] = 1;
		}
		else {
			vector_type_of_brakets[i] = 0;
		}

		//trec numarul din paranteze in vectorul de valori
		vector_values[i] = braket_to_int(p);
		p = strtok(NULL, " ");
		i++;
	}
}

//functie care verifica cate niveluri sunt pana la root, e aceiasi ca cea pt
//taskul 1 este doar schimbat tipul nodului
int level_from_root(arbore_part_2_node *nod)
{
	int level = 0;

	arbore_part_2_node *aux = nod;

	while (aux->parent != NULL) {
		level++;
		aux = aux->parent;
	}
	return level;
}

//functie care pune in lista de copii un nou nod facand de asemenea si
//asignarea pt type-ul noului nod, inmultind cu -1
void push_child(arbore_part_2_node **head, arbore_part_2_node *nod_nou,
arbore_part_2_node *parent)
{
	//pun in lista noul nod
	if ((*head) == NULL) {
		nod_nou->next = *head;
		*head = nod_nou;
	}
	else {
		arbore_part_2_node *i = *head;
		while (i->next) {
			i = i->next;
		}
		i->next = nod_nou;
		nod_nou->prev = i;
	}
	//ii pun si tipul nodului ca find opusul parintelui
	nod_nou->type = -1 * parent->type;
	nod_nou->parent = parent;
}

//functie care parcurge arborele recursiv si daca e pe linia ceruta, cea data
//prin parametru index_of_line, ii face modificarile luand valorile din 
//vectorul de valori si ce fel de paranteza este din vectorul de paranteze, am
//folosit adresa index of vector pt a se tine cont la parcurgerea recursiva de
//modificarile facute pe un subarbore, intrucat daca nu se da adresa acesta
//ramane neschimbat si functia nu face ce vrem
void work_with_line(arbore_part_2_node *root, int index_of_line,
int *index_of_vector, int *vector_values, int *vector_type_of_brakets)
{
	if (root == NULL)
		return;

	int i;

	//verific daca nodul se afla pe nivelul pe care il vrem
	if (level_from_root(root) == index_of_line) {
		//daca e o paranteza rotunda atunci initializez un numar de copii egal
		//cu cel de pe pozitia *index_of_vector din vector values
		if (vector_type_of_brakets[(*index_of_vector)] == 1) {
			for (i = 0; i < vector_values[(*index_of_vector)]; i++) {
				arbore_part_2_node *nod_nou = new_node_part_2();
				push_child(&(root->child), nod_nou, root);
			}
		}
		//daca e paranteza patrata ii pun valoarea in nod
		else {
			root->value = vector_values[(*index_of_vector)];
		}
		*index_of_vector = *index_of_vector + 1;
	}

	//parcurg in adancime arborele si fac modificarile necesaare daca se afla
	//pe nivelul cerut
	work_with_line(root->child, index_of_line, index_of_vector, vector_values,
	vector_type_of_brakets);
	work_with_line(root->next, index_of_line, index_of_vector, vector_values,
	vector_type_of_brakets);
}

//functie de printare a taburilor ca la taskul 1, fiind diferit tipul nodului
void print_tabs_part_2(arbore_part_2_node *nod, FILE *fisier_out)
{
	int number_of_tabs = level_from_root(nod);

	int i;
	
	for (i = 0; i < number_of_tabs; i++) {
		fprintf(fisier_out, "\t");
	}
}

//imi afiseaza arborele parcurgand in adancime si punand in fisier taburile
//necesare urmate de valoarea din noduri
void print_tree(arbore_part_2_node *root, FILE *fisier_out)
{
	if (root == NULL)
		return;

	print_tabs_part_2(root, fisier_out);
	fprintf(fisier_out, "%d\n", root->value);

	print_tree(root->child, fisier_out);
	print_tree(root->next, fisier_out);
}

//functie care calculeaza maximul din lista de copii a unui nod, va fi folosita
//pt nodurile de tipul max
int maxim_in_list(arbore_part_2_node *head)
{
	arbore_part_2_node *aux = head->next;

	int maxim = head->value;
	
	while (aux != NULL) {
		if (aux->value > maxim) {
			maxim = aux->value;
		}
		aux = aux->next;
	}
	return maxim;
}

//ca cea precedenta doar ca verifica pt minim
int minim_in_list(arbore_part_2_node *head)
{
	arbore_part_2_node *aux = head->next;

	int minim = head->value;
	
	while (aux != NULL) {
		if (aux->value < minim) {
			minim = aux->value;
		}
		aux = aux->next;
	}
	return minim;
}

//functie care incepe de la cele mai jos niveluri, incep de la nr de linii - 1
//si in functie de tipul nodului actual ii pun valorile minime/maxime din
//din listele de copii
void complete_tree_with_mini_max(arbore_part_2_node *root, int index_of_line)
{
	if (root == NULL)
		return;

	//daca se afla pe nivelul dorit
	if (level_from_root(root) == index_of_line) {
		//daca nu e frunza
		if (root->child != NULL) {
			//daca e max va lua valoarea maxima din lista copiilor
			if (root->type == 1) {
				root->value = maxim_in_list(root->child);
			}
			//daca e min va lua valoarea de minim din lista copiilor
			if (root->type == -1) {
				root->value = minim_in_list(root->child);
			}
		}
	}

	//se face o parcurgere recursiva in adancime
	complete_tree_with_mini_max(root->child, index_of_line);
	complete_tree_with_mini_max(root->next, index_of_line);
}

//functie folosita la eliberarea memoriei din arborele de la taskurile 2 si 3
void free_arbore_part_2(arbore_part_2_node *nod)
{
	if (nod == NULL)
		return;

	//se face o parcurgere oarecum in latime
	free_arbore_part_2(nod->next);
	free_arbore_part_2(nod->child);

	//se elibereaza nodul respectiv
	free(nod);
}

//functie folosita la taskul 3 care va initializa valorile din noduri cu cel
//mai mare respectiv cel mai mic(in functie de min max al nodului) deja gasit
//sau elibereaza subarborii inutili daca valorile gasite sunt mai mari sau mai
//mici (depinzand de typul nodului, daca acesta este min sau max) ca valorile
//deja puse in alpha sau beta in nodul respectiv
void complete_with_alpha_beta(arbore_part_2_node *root)
{
	//daca a ajuns la un nod frunza se intarce
	if (root->child == NULL)
		return;
	
	//daca nodul e max
	if (root->type == 1) {
		//iau o variabila de control si asignez copilului cap de lista valorile
		//alpha si beta ale nodului curent apoi parcurg si mai in adancime
		int pruned = 0;

		root->child->alpha = root->alpha;
		root->child->beta = root->beta;
		complete_with_alpha_beta(root->child);

		//initializez valoarea de maxim cu valoarea primului copil
		int max_value_children = root->child->value;

		//daca valoarea de maxim e mai mare ca cea a alpha, alpha devine 
		//aceasta intrucat alpha vizeaza cea mai mare valoare posibila curenta
		if (max_value_children > root->alpha) {
			root->alpha = max_value_children;
		}
		//daca valoarea abia descoperita e mai mare ca cea descoperita pana
		//atunci pe un alt subarbore (e indeplinita conditia de pruning) se va
		//sterge urmatorul fiu din lista de copii intrucat nu e necesar sa mai
		//se verifice acea cale intrucat competitorul pc-ului nu va alege
		//niciodata acea cale
		if (root->alpha >= root->beta) {
			free_arbore_part_2(root->child->next);
			root->child->next = NULL;
			pruned = 1;
		}
		//dupa initializarea maximului parcurg toata lista de copii urmand sa
		//folosesc aceiasi logica pt fiecare nod in parte
		arbore_part_2_node *nod = root->child->next;

		while (nod != NULL && pruned == 0) {
			//pe fiecare nod dau mai departe valorile alpha si beta de la
			//parinte
			nod->alpha = root->alpha;
			nod->beta = root->beta;

			//parcurg in adancime pe nodul curent din lista de copii
			complete_with_alpha_beta(nod);
			int value_of_child = nod->value;

			//daca valoarea copilului e mai mare ca cea mai mare de pana acum,
			//aceasta ia valoarea curenta, alpha crescand o data cu max_value
			//de la copii
			if (value_of_child > max_value_children) {
				max_value_children = value_of_child;
			}
			if (max_value_children > root->alpha) {
				root->alpha = max_value_children;
			}
			nod = nod->next;
			//daca este indeplinita conditia de pruning, se sterge arborele ce
			//incepe din nodul urmator nodului care alpha si beta indeplinesc
			//conditia, dar cum am facut deja trecerea la next se va elibera
			//nodul "curent"
			if (root->alpha >= root->beta && nod != NULL) {
				max_value_children = nod->prev->value;
				nod->prev->next = NULL;
				free_arbore_part_2(nod);
				pruned = 1;
			}
		}
		//nodul curent ia valoarea cea mai mare din lista de copii
		root->value = max_value_children;
	}

	//daca nodul este de tipul min, se urmeaza logica utilizata mai sus, luand
	//in calcul insa valoarea de minim, nu de maxim
	if (root->type == -1) {
		//se ia variabila de control si se dau mai departe valorile deja
		//descoperite pt alfa si beta
		int pruned = 0;

		root->child->alpha = root->alpha;
		root->child->beta = root->beta;

		//se parcurge in depth
		complete_with_alpha_beta(root->child);

		//se initializeaza valoarea de minim cu valoarea capului de la lista de
		//copii
		int min_value_children = root->child->value;

		if (min_value_children < root->beta) {
			root->beta = min_value_children;
		}
		//daca se indeplineste conditia de pruning se sterge arborele ce incepe
		//de la urmatorul nod din lista de copii, nu mai trebuie sa verificam
		//acel subarbore intrucat oponentul nu va considera niciodata acea
		//varianta fiindca il dezavantajeaza
		if (root->alpha >= root->beta) {
			free_arbore_part_2(root->child->next);
			root->child->next = NULL;
			pruned = 1;
		}
		//verifica pt fiecare copil din lista
		arbore_part_2_node *nod = root->child->next;
		while (nod != NULL && pruned == 0) {
			nod->alpha = root->alpha;
			nod->beta = root->beta;
			//parcurge in adancime prima daata
			complete_with_alpha_beta(nod);
			int value_of_child = nod->value;

			//se schimba valorile pt beta si minim daca valoarea curent
			//explorata este mai mica decat acestea
			if (value_of_child < min_value_children) {
				min_value_children = value_of_child;
			}
			if (min_value_children < root->beta) {
				root->beta = min_value_children;
			}
			nod = nod->next;
			//daca se indeplineste conidtia de pruning se sterge subarborele
			//care incepe din nodul "curent" intrucat deja s-a trecut la nodul
			//de dupa cel care indeplineste conditia
			if (root->alpha >= root->beta && nod != NULL) {
				min_value_children = nod->prev->value;
				nod->prev->next = NULL;
				free_arbore_part_2(nod);
				pruned = 1;
			}
		}
		//nodul curent ia valoarea minima din lista copiilor
		root->value = min_value_children;
	}
}

//functie care aloca un nod folosit in taskul de bonus
arbore_bonus_node *new_node_bonus(int nr_players)
{
	arbore_bonus_node *nod_nou = malloc(sizeof(arbore_bonus_node));

	nod_nou->values = calloc(nr_players, sizeof(int));
	nod_nou->child = NULL;
	nod_nou->parent = NULL;
	nod_nou->next = NULL;
	nod_nou->prev = NULL;
	nod_nou->type = 0;
	return nod_nou;
}

//functie care calculeaza nivelurile pana la root, ca cele de pana acum
//adaptate la taskul bonus care are un nod cu o alta structura
int level_from_root_bonus(arbore_bonus_node *nod)
{
	int level = 0;

	arbore_bonus_node *aux = nod;
	while (aux->parent != NULL) {
		level++;
		aux = aux->parent;
	}
	return level;
}

//ca cele de mai sus, printeaza in fisier nr de taburi necesare
void print_tabs_bonus(arbore_bonus_node *nod, FILE *fisier_out)
{
	int number_of_tabs = level_from_root_bonus(nod);

	int i;

	for (i = 0; i < number_of_tabs; i++) {
		fprintf(fisier_out, "\t");
	}
}

//functie care afiseaza in fisier cu formatul cerut, pun parantezele patrate
//apoi intercalez valorile din nodul respectiv cu virgulele
void print_with_brakets(arbore_bonus_node *nod, FILE *fisier_out,
int nr_players)
{
	fprintf(fisier_out, "[");

	int i;

	for (i = 0; i < nr_players - 1; i++) {
		fprintf(fisier_out, "%d,", nod->values[i]);
	}
	fprintf(fisier_out, "%d]\n", nod->values[nr_players - 1]);
}

//functie care afiseaza arborele, e asemanatoare celei de la taskurile 2 si 3
//doar ca am adaugat formatul cu parantezele patrate
void print_tree_bonus(arbore_bonus_node *root, FILE *fisier_out,
int nr_players)
{
	if (root == NULL)
		return;

	print_tabs_bonus(root, fisier_out);
	print_with_brakets(root, fisier_out, nr_players);

	//parcurgere in adancime a arborelui
	print_tree_bonus(root->child, fisier_out, nr_players);
	print_tree_bonus(root->next, fisier_out, nr_players);
}

//functie care adauga in lista de copii un nou nod
void push_child_bonus(arbore_bonus_node **head,
arbore_bonus_node *nod_nou, arbore_bonus_node *parent)
{
	if ((*head) == NULL) {
		nod_nou->next = *head;
		*head = nod_nou;
	}
	else {
		arbore_bonus_node *i = *head;

		while (i->next) {
			i = i->next;
		}
		i->next = nod_nou;
		nod_nou->prev = i;
	}
	nod_nou->parent = parent;
}

//functie care trece valorile din paranteze de la siruri de caractere in valori
//in vectorul transmis prin parametru
void braket_to_int_bonus(char *line, int nr_players, int type, int *vector)
{
	char *auxiliar = malloc((strlen(line) + 1) * sizeof(char));

	int i;
	
	//daca e o paranteza rotunda 
	if (type == 1) {
		//copiez in auxiliar sirul de la linie + 1 pt a scapa de prima 
		///paranteza
		strcpy(auxiliar, line + 1);
		//sterg si ultima paranteza
		auxiliar[strlen(auxiliar) - 1] = '\0';
		//pun pe prima pozitie din vector numarul transformat prin atoi
		vector[0] = atoi(auxiliar);
	}
	if (type == 0) {
		//cu index tin cont de pozitia numarului curent de pus in vector
		int index = 0;

		//imi fac o copie a sirului linie + 1 in p, pentru a scapa de prima
		//paranteza
		char *p = malloc(strlen(line) * sizeof(char));
		strcpy(p, line + 1);

		//imi iau un pointer pt a ma deplasa prin sirul de caractere copiat
		char *pointer;
		pointer = p;

		//cat timp nu am terminat de scos numere din sir continui sa fac asta
		//pointer va avea rolul de a retine inceputurile numerelor dupa ce
		//transform unul in int
		while (pointer[0] != '\0') {
			//copiez in sirul aux sirul din pointer
			char *aux = malloc((strlen(pointer) + 1) * sizeof(char));
			strcpy(aux, pointer);

			//parcurg pt a gasi prima virgula sau paranteza dreapta, daca am
			//gasit una pun terminator de sir si cu pointer trec la urmatorul
			//numar ca sir de caractere
			for (i = 0; i < strlen(aux); i++) {
				if (aux[i] == ',' || aux[i] == ']') {
					aux[i] = '\0';
					strcpy(pointer, aux + i + 1);
				}
			}
			//transform sirul de caractere si pun pe pozitia index in vector
			//si eliberez aux, apoi dupa ce ies din while eliberez p
			vector[index] = atoi(aux);
			index++;
			free(aux);
		}
		free(p);
	}
	//eliberez vectorul alocat dinamic auxiliar
	free(auxiliar);
}

//functie care primeste ca parametru sirul de input care va fi transformat in
//valori pt vectorii type of brakets si cele din vector values, care se 
//comporta ca o matrice
void transform_row_bonus(char *line, int **vector_values,
int *vector_type_of_brakets, int nr_players)
{
	int i = 0;

	char *p = strtok(line, " ");

	while (p != NULL) {
		//similar cu cea de la taskurile 2 si 3 trece in vectori variantele 
		//prelucrate ale sirurilor din input
		if (p[0] == '(') {
			vector_type_of_brakets[i] = 1;
		}
		else {
			vector_type_of_brakets[i] = 0;
		}
		braket_to_int_bonus(p, nr_players, vector_type_of_brakets[i],
		vector_values[i]);
		p = strtok(NULL, " ");
		i++;
	}
}

//similar cu cea de la taskul 2, imi creaza scheletele arborelui, cel in care
//se gasesc umplute doar frunzele, pe masura ce parcurg arborele fie creez nr
//de fii necesar fie pun valoarea in nod
void work_with_line_bonus(arbore_bonus_node *root, int index_of_line,
int *index_of_vector, int **vector_values, int *vector_type_of_brakets,
int nr_players)
{
	if (root == NULL)
		return;
	
	int i;

	if (level_from_root_bonus(root) == index_of_line) {
		if (vector_type_of_brakets[(*index_of_vector)] == 1) {
			for (i = 0; i < vector_values[(*index_of_vector)][0]; i++) {
				arbore_bonus_node *nod_nou = new_node_bonus(nr_players);

				if ((index_of_line + 1) % nr_players == 0) {
					nod_nou->type = 1;
				}
				else {
					nod_nou->type = -1;
				}
				push_child_bonus(&(root->child), nod_nou, root);
			}
		}
		else {
			for (i = 0; i < nr_players; i++) {
				root->values[i] = vector_values[(*index_of_vector)][i];
			}
		}
		*index_of_vector = *index_of_vector + 1;
	}
	
	//fac o parcurgere in adancime si daca se afla la nivelul cerut prin
	//parametru face modificarile necesare
	work_with_line_bonus(root->child, index_of_line, index_of_vector,
	vector_values, vector_type_of_brakets, nr_players);
	work_with_line_bonus(root->next, index_of_line, index_of_vector,
	vector_values, vector_type_of_brakets, nr_players);
}

//intoarce un vector care are pe prima pozitie cea mai mare valoare din
//vectorii continuti in nodurile din lista de copii
int *maxim_in_list_bonus(arbore_bonus_node *head)
{
	arbore_bonus_node *aux = head->next;

	int *adress = head->values;
	
	int maxim = head->values[0];
	
	while (aux != NULL) {
		if (aux->values[0] > maxim) {
			maxim = aux->values[0];
			adress = aux->values;
		}
		aux = aux->next;
	}
	return adress;
}

//ca functia precedenta doar ca face pt minim
int *minim_in_list_bonus(arbore_bonus_node *head)
{
	arbore_bonus_node *aux = head->next;

	int *adress = head->values;
	
	int minim = head->values[0];
	
	while (aux != NULL) {
		if (aux->values[0] < minim) {
			minim = aux->values[0];
			adress = aux->values;
		}
		aux = aux->next;
	}
	return adress;
}

//functie care completeaza arborele similar cu crea de la taskul 2 doar ca in
//loc de valori unice copiaza un vector
void complete_tree_with_mini_max_bonus(arbore_bonus_node *root,
int index_of_line, int nr_players)
{
	if (root == NULL)
		return;

	if (level_from_root_bonus(root) == index_of_line) {
		if (root->child != NULL) {
			//daca nodul e de tipul max va face o copie dupa vectorul care
			//contine maximul din lista de copii
			if (root->type == 1) {
				int i;
				int *vector = maxim_in_list_bonus(root->child);
				//copiaza valorile in vectorul nodului actual
				for (i = 0; i < nr_players; i++) {
					root->values[i] = vector[i];
				}
			}
			//daca nodul e de tipul minim va face acelasi lucru ca mai sus
			//doar ca pt minim
			if (root->type == -1) {
				int i;

				int *vector = minim_in_list_bonus(root->child);

				for (i = 0; i < nr_players; i++) {
					root->values[i] = vector[i];
				}
			}
		}
	}

	//face o parcurgere de tipul depth
	complete_tree_with_mini_max_bonus(root->child, index_of_line, nr_players);
	complete_tree_with_mini_max_bonus(root->next, index_of_line, nr_players);
}

//functie care elibereaza arborele
void free_arbore_bonus(arbore_bonus_node *nod)
{
	if (nod == NULL)
		return;

	//se face o parcurgere recursiva asemanatoare cu cea in latime
	free_arbore_bonus(nod->next);
	free_arbore_bonus(nod->child);

	//prima data se elibereaza vectorul din nodul curent, apoi nodul curent
	free(nod->values);
	free(nod);
}

//in main compar daca argumentul contine vreuna dintre cerinte, -c1, -c2 samd
int main(int argc, char **argv)
{
	//pt taskul 1
	if (strstr(argv[1], "-c1")) {
		int nr_rows, nr_columns, i;

		char player;
		
		//deschid fisierele date in linie de comanda
		FILE *fisier_in = fopen(argv[2], "r");
		FILE *fisier_out = fopen(argv[3], "w");

		//citesc numarul de linii, de coloane si jucatorul care trebuie sa
		//faca miscarea
		fscanf(fisier_in, "%d %d %c", &nr_rows, &nr_columns, &player);

		//preiau enterul din fisier
		fgetc(fisier_in);

		//fac un nod root pe care il alloc cu functia definita pt a face noi
		//noduri
		arbore_part_1_node *root = new_node(nr_rows, nr_columns);

		//citesc liniile din matrice
		for (i = 0; i < nr_rows; i++) {
			fgets(root->game_matrix[i], 2 * nr_columns, fisier_in);
			fgetc(fisier_in);
			root->game_matrix[i][2 * nr_columns - 1] = '\0';
		}
		//setez playerul care trebuie sa faca mutarea
		root->player = player;

		//imi creaza toate nodurile pe toate cazurile si le afiseaza, apoi
		//inchide fisierele si elibereaza memoria din arbore
		create_all_nodes(root, nr_rows, nr_columns);
		print_matrix(root, nr_rows, nr_columns, fisier_out);

		fclose(fisier_in);
		fclose(fisier_out);
		free_everything(root, nr_rows, nr_columns);
	}
	if (strstr(argv[1], "-c2")) {   
		//aloc dinamic multa memorie pt vectori pt a fi sigur ca e destul 
		//spatiu pt testele mari de pe vmchecker
		int nr_rows, i;
		char *input_buffer = malloc(10000000 * sizeof(char));
		int *vector_values = malloc(5000000 * sizeof(int));
		int *vector_type_of_brakets = malloc(5000000 * sizeof(int));

		//aloc un nod pt root
		arbore_part_2_node *root = new_node_part_2();

		//il declar ca fiind max
		root->type = 1;

		//deschid fisierele pt citire si afisare
		FILE *fisier_in = fopen(argv[2], "r");
		FILE *fisier_out = fopen(argv[3], "w");

		//citesc nr de linii si preiau enterul
		fscanf(fisier_in, "%d", &nr_rows);
		fgetc(fisier_in);

		for (i = 0; i < nr_rows; i++) {
			//citesc linie cu linie, transfer valorile citite intr-un vector de
			//valori si tipurile de paranteze intr-un vector de paranteze pe
			//care la voi folosi dupa pt a le trece in formatul cerut in arbore
			//daca paranteza este rotunda pun atatia fii, daca e patrata pun 
			//valoarea respectiva in nod
			fgets(input_buffer, 10000000, fisier_in);
			input_buffer[strlen(input_buffer) - 1] = '\0';
			transform_row(input_buffer, vector_values, vector_type_of_brakets);

			//ma folosesc de un index pt a parcurge vectorii si pt a prelucra
			//in arbori
			int index = 0;

			//apelez functia cu care voi pune in arbori cate noduri se cere dar
			//si valorile date
			work_with_line(root, i, &index, vector_values,
			vector_type_of_brakets);
		}
		//pt fiecare nivel, incepand de la cel imediat superior celui maxim
		//(frunzele) fiecare nod ia fie valorile fie maxime fie minime din cele
		//ale copiilor
		for (i = nr_rows - 2; i >= 0; i--) {
			complete_tree_with_mini_max(root, i);
		}
		//imi afiseaza arborele, apoi elibereaza memoria si inchid fisierele 
		print_tree(root, fisier_out);
		free_arbore_part_2(root);

		fclose(fisier_in);
		fclose(fisier_out);

		free(input_buffer);
		free(vector_values);
		free(vector_type_of_brakets);
	}
	if (strstr(argv[1], "-c3")) {
		//aloc vectorii pt valorile citite, tipurile de paranteze si un buffer
		//ca sa citesc fiecare linie
		int nr_rows, i;
		char *input_buffer = malloc(10000000 * sizeof(char));
		int *vector_values = malloc(5000000 * sizeof(int));
		int *vector_type_of_brakets = malloc(5000000 * sizeof(int));

		//aloc rootul si il pun sa fie max
		arbore_part_2_node *root = new_node_part_2();

		root->type = 1;

		//deschid fisierele transmise prin linia de comanda
		FILE *fisier_in = fopen(argv[2], "r");
		FILE *fisier_out = fopen(argv[3], "w");

		fscanf(fisier_in, "%d", &nr_rows);
		fgetc(fisier_in);

		for (i = 0; i < nr_rows; i++) {
			//citeste fiecare linie si la fel ca la taskul 2 transforma inputul
			//in nodurile din arbore
			fgets(input_buffer, 10000000, fisier_in);
			input_buffer[strlen(input_buffer) - 1] = '\0';
			transform_row(input_buffer, vector_values, vector_type_of_brakets);
			int index = 0;
			work_with_line(root, i, &index, vector_values,
			vector_type_of_brakets);
		}
		//initializeaza valorile pt alpha si beta ca -inf si +inf, ca cea mai
		//mare si cea mai mica varianta posibila, initial nu are alegeri
		//explorate si de aceea cea mai mare posibilitate numerica e -inf
		root->alpha = -999999999;
		root->beta = 999999999;

		//completeaza arborele si sterge subarborii de care nu mai ai nevoie
		//apoi elibereaza memoria si inchide fisierele
		complete_with_alpha_beta(root);
		print_tree(root, fisier_out);

		free_arbore_part_2(root);

		fclose(fisier_in);
		fclose(fisier_out);
		free(input_buffer);
		free(vector_values);
		free(vector_type_of_brakets);
	}
	if(strstr(argv[1], "b")) {
		int nr_rows, nr_players, i;

		FILE *fisier_in = fopen(argv[2], "r");
		FILE *fisier_out = fopen(argv[3], "w");

		fscanf(fisier_in, "%d %d", &nr_rows, &nr_players);
		fgetc(fisier_in);

		//in aceeasi maniera ca la taskurile 2 si 3 aloc dinamic vectorii de
		//citit sirurile, cel pt tipurile de paranteze si un vector de vectori
		//care sunt de lungime nr de jucatori, pt fiecare decizie a fiecarui
		//jucator
		char *input_buffer = malloc(1000000 * sizeof(char));
		int **vector_values = malloc(500000 * sizeof(int *));
		int *vector_type_of_brakets = malloc(500000 * sizeof(int));

		//aloc rootul si ii pus sa fie de tip max
		arbore_bonus_node *root = new_node_bonus(nr_players);
		root->type = 1;

		//aloca dinamic vectorul de vectori
		for (i = 0; i < 500000; i++) {
			vector_values[i] = malloc(nr_players * sizeof(int));
		}

		for (i = 0; i < nr_rows; i++) {
			//la fiecare linie citita pun in arbore nodurile si pe masura ce
			//pun in arbore trec si tipul de jucator care este, daca nu e pc-ul
			//cel care muta pune -1 la type, daca e calculatorul cel care muta
			//va pune 1
			fgets(input_buffer, 1000000, fisier_in);
			input_buffer[strlen(input_buffer) - 1] = '\0';
			transform_row_bonus(input_buffer, vector_values,
			vector_type_of_brakets, nr_players);
			int index = 0;
			work_with_line_bonus(root, i, &index, vector_values,
			vector_type_of_brakets, nr_players);
		}
		//completeaza asemanator taskului 2 doar ca lucreaza cu vectori de
		//valori, adica in loc de 1 un nod are valorile 1,2,3 sau alte
		//exemple
		for (i = nr_rows - 2; i >= 0; i--) {
			complete_tree_with_mini_max_bonus(root, i, nr_players);
		}
		//imi afiseaza arborele, elibereaza memoria si inchide fisierele
		print_tree_bonus(root, fisier_out, nr_players);

		free_arbore_bonus(root);

		fclose(fisier_in);
		fclose(fisier_out);

		for (i = 0; i < 500000; i++) {
			free(vector_values[i]);
		}
		free(input_buffer);
		free(vector_values);
		free(vector_type_of_brakets);
	}
	return 0;
}
