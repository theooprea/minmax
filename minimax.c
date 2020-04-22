#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

int check_point_in_matrix(char **game_matrix, int index_row, int index_column, int nr_rows, int nr_column)
{
    char colour = game_matrix[index_row][2 * index_column];
    //verific in sus
    if(index_row >= 3)
    {
        if(game_matrix[index_row - 1][2 * index_column] == colour && game_matrix[index_row - 2][2 * index_column] == colour 
        && game_matrix[index_row - 3][2 * index_column] == colour)
            return 1;
    }
    //verific in jos
    if(index_row + 3 < nr_rows)
    {
        if(game_matrix[index_row + 1][2 * index_column] == colour && game_matrix[index_row + 2][2 * index_column] == colour 
        && game_matrix[index_row + 3][2 * index_column] == colour)
            return 1;
    }
    //verific in stanga
    if(index_column >= 3)
    {
        if(game_matrix[index_row][2 * index_column - 2] == colour && game_matrix[index_row][2 * index_column - 4] == colour 
        && game_matrix[index_row][2 * index_column - 6] == colour)
            return 1;
    }
    //verific in dreapta
    if(index_column + 3 < nr_column)
    {
        if(game_matrix[index_row][2 * index_column + 2] == colour && game_matrix[index_row][2 * index_column + 6] == colour 
        && game_matrix[index_row][2 * index_column + 4] == colour)
            return 1;
    }
    //verific in dreapta sus pe diagonala
    if(index_row >= 3 && index_column + 3 < nr_column)
    {
        if(game_matrix[index_row - 1][2 * index_column + 2] == colour && game_matrix[index_row - 2][2 * index_column + 4] == colour 
        && game_matrix[index_row - 3][2 * index_column + 6] == colour)
            return 1;
    }
    //verific in dreapta jos pe diagonala
    if(index_row + 3 < nr_rows && index_column + 3 < nr_column)
    {
        if(game_matrix[index_row + 1][2 * index_column + 2] == colour && game_matrix[index_row + 2][2 * index_column + 4] == colour 
        && game_matrix[index_row + 3][2 * index_column + 6] == colour)
            return 1;
    }
    //verific in stanga jos
    if(index_column >= 3 && index_row + 3 < nr_rows)
    {
        if(game_matrix[index_row + 1][2 * index_column - 2] == colour && game_matrix[index_row + 2][2 * index_column - 4] == colour 
        && game_matrix[index_row + 3][2 * index_column - 6] == colour)
            return 1;
    }
    //verific in stanga sus
    if(index_column >= 3 && index_row >= 3)
    {
        if(game_matrix[index_row - 1][2 * index_column - 2] == colour && game_matrix[index_row - 2][2 * index_column - 4] == colour 
        && game_matrix[index_row - 3][2 * index_column - 6] == colour)
            return 1;
    }
    return 0;
}

int is_game_over(arbore_part_1_node *nod, int nr_rows, int nr_columns)
{
    int i,j;
    for(i = 0; i < nr_rows; i++)
    {
        for(j = 0; j < nr_columns; j++)
        {
            if(nod->game_matrix[i][2 * j] != '-' && check_point_in_matrix(nod->game_matrix, i, j, nr_rows, nr_columns))
                return 1;
        }
    }
    return 0;
}

arbore_part_1_node *new_node(int nr_rows, int nr_columns)
{
    arbore_part_1_node *new_node = malloc(sizeof(arbore_part_1_node));
    new_node->game_matrix = malloc(nr_rows * sizeof(char *));
    int i;
    for(i = 0; i < nr_rows; i++)
    {
        new_node->game_matrix[i] = malloc((2 * nr_columns) * sizeof(char));
    }
    new_node->parent = NULL;
    new_node->child = NULL;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->number_of_children = 0;
    new_node->player = '\0';
    return new_node;
}

void create_matrix(char **game_matrix, char **parents_game_matrix, int indx_column, char player, int nr_rows, int nr_columns)
{
    int i, j;
    for(i = 0; i < nr_rows; i++)
    {
        for(j = 0; j < 2 * nr_columns; j++)
        {
            game_matrix[i][j] = parents_game_matrix[i][j];
        }
    }
    i = 0;
    while(i < nr_rows && game_matrix[i][indx_column] == '-')
    {
        i++;
    }
    game_matrix[i - 1][indx_column] = player;
}

void add_child_node(arbore_part_1_node **head, arbore_part_1_node *nod_nou, arbore_part_1_node *parent_node)
{
    if((*head) == NULL)
    {
        nod_nou->next = *head;
        *head = nod_nou;
    }
    else
    {
        arbore_part_1_node *i = *head;
        while(i->next)
        {
            i = i->next;
        }
        i->next = nod_nou;
        nod_nou->prev = i;
    }
    nod_nou->parent = parent_node;
    if(parent_node->player == 'R')
    {
        nod_nou->player = 'B';
    }
    else
    {
        nod_nou->player = 'R';
    }
}

void create_all_nodes(arbore_part_1_node *nod, int nr_rows, int nr_columns)
{
    if(nod == NULL)
        return;

    if(is_game_over(nod, nr_rows, nr_columns) == 0)
    {
        int i;
        for(i = 0; i < nr_columns; i++)
        {
            if(nod->game_matrix[0][2 * i] == '-')
            {
                arbore_part_1_node *nod_nou = new_node(nr_rows, nr_columns);
                create_matrix(nod_nou->game_matrix, nod->game_matrix, 2 * i, nod->player, nr_rows, nr_columns);
                add_child_node(&(nod->child), nod_nou, nod);
                nod->number_of_children++;
            }
        }
    }

    create_all_nodes(nod->child, nr_rows, nr_columns);
    create_all_nodes(nod->next, nr_rows, nr_columns);
}

int number_of_needed_tabs(arbore_part_1_node *nod)
{
    int number_of_tabs = 0;
    arbore_part_1_node *aux = nod;
    while(aux->parent != NULL)
    {
        number_of_tabs++;
        aux = aux->parent;
    }
    return number_of_tabs;
}

void print_tabs(arbore_part_1_node *nod, FILE *fisier_out)
{
    int number_of_tabs = number_of_needed_tabs(nod);
    int i;
    for(i = 0; i < number_of_tabs; i++)
    {
        fprintf(fisier_out, "\t");
    }
}

void print_matrix(arbore_part_1_node *nod, int nr_rows, int nr_columns, FILE *fisier_out)
{
    if(nod == NULL)
        return;
    
    int i;
    for(i = 0; i < nr_rows; i++)
    {
        print_tabs(nod, fisier_out);
        fprintf(fisier_out, "%s\n", nod->game_matrix[i]);
    }
    fprintf(fisier_out, "\n");
    print_matrix(nod->child, nr_rows, nr_columns, fisier_out);
    print_matrix(nod->next, nr_rows, nr_columns, fisier_out);
}

void free_everything(arbore_part_1_node *nod, int nr_rows, int nr_colums)
{
    if(nod == NULL)
        return;

    free_everything(nod->next, nr_rows, nr_colums);
    free_everything(nod->child, nr_rows, nr_colums);

    int i;
    for(i = 0; i < nr_rows; i++)
    {
        free(nod->game_matrix[i]);
    }
    free(nod->game_matrix);
    free(nod);
}

void print_single_matrix(arbore_part_1_node *nod, int nr_rows, int nr_columns)
{
    int i;
    for(i = 0; i < nr_rows; i++)
    {
        printf("%s\n", nod->game_matrix[i]);
    }
}

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

int braket_to_int(char *line)
{
    char *p = line + 1;
    p[strlen(p) - 1] = '\0';
    int nr = atoi(p);
    return nr;
}

void transform_row(char *line, int *vector_values, int *vector_type_of_brakets)
{
    int i = 0;
    char *p = strtok(line, " ");
    while(p != NULL)
    {
        if(p[0] == '(')
        {
            vector_type_of_brakets[i] = 1;
        }
        else
        {
            vector_type_of_brakets[i] = 0;
        }
        vector_values[i] = braket_to_int(p);
        p = strtok(NULL, " ");
        i++;
    }
}

int level_from_root(arbore_part_2_node *nod)
{
    int level = 0;
    arbore_part_2_node *aux = nod;
    while(aux->parent != NULL)
    {
        level++;
        aux = aux->parent;
    }
    return level;
}

void push_child(arbore_part_2_node **head, arbore_part_2_node *nod_nou, arbore_part_2_node *parent)
{
    if((*head) == NULL)
    {
        nod_nou->next = *head;
        *head = nod_nou;
    }
    else
    {
        arbore_part_2_node *i = *head;
        while(i->next)
        {
            i = i->next;
        }
        i->next = nod_nou;
        nod_nou->prev = i;
    }
    nod_nou->type = -1 * parent->type;
    nod_nou->parent = parent;
    
}

void work_with_line(arbore_part_2_node *root, int index_of_line, int *index_of_vector, int *vector_values, int *vector_type_of_brakets)
{
    if(root == NULL)
        return;
    int i;
    if(level_from_root(root) == index_of_line)
    {
        if(vector_type_of_brakets[(*index_of_vector)] == 1)
        {
            for(i = 0; i < vector_values[(*index_of_vector)]; i++)
            {
                arbore_part_2_node *nod_nou = new_node_part_2();
                push_child(&(root->child), nod_nou, root);
            }
        }
        else
        {
            root->value = vector_values[(*index_of_vector)];
        }
        *index_of_vector = *index_of_vector + 1;
    }

    work_with_line(root->child, index_of_line, index_of_vector, vector_values, vector_type_of_brakets);
    work_with_line(root->next, index_of_line, index_of_vector, vector_values, vector_type_of_brakets);
}

void print_tabs_part_2(arbore_part_2_node *nod, FILE *fisier_out)
{
    int number_of_tabs = level_from_root(nod);
    int i;
    for(i = 0; i < number_of_tabs; i++)
    {
        fprintf(fisier_out, "\t");
    }
}

void print_tree(arbore_part_2_node *root, FILE *fisier_out)
{
    if(root == NULL)
        return;

    print_tabs_part_2(root, fisier_out);
    fprintf(fisier_out, "%d\n", root->value);

    print_tree(root->child, fisier_out);
    print_tree(root->next, fisier_out);
}

int maxim_in_list(arbore_part_2_node *head)
{
    arbore_part_2_node *aux = head->next;
    int maxim = head->value;
    while(aux != NULL)
    {
        if(aux->value > maxim)
        {
            maxim = aux->value;
        }
        aux = aux->next;
    }
    return maxim;
}

int minim_in_list(arbore_part_2_node *head)
{
    arbore_part_2_node *aux = head->next;
    int minim = head->value;
    while(aux != NULL)
    {
        if(aux->value < minim)
        {
            minim = aux->value;
        }
        aux = aux->next;
    }
    return minim;
}

void complete_tree_with_mini_max(arbore_part_2_node *root, int index_of_line)
{
    if(root == NULL)
        return;

    if(level_from_root(root) == index_of_line)
    {
        if(root->child != NULL)
        {
            if(root->type == 1)
            {
                root->value = maxim_in_list(root->child);
            }
            if(root->type == -1)
            {
                root->value = minim_in_list(root->child);
            }
        }
    }

    complete_tree_with_mini_max(root->child, index_of_line);
    complete_tree_with_mini_max(root->next, index_of_line);
}

void free_arbore_part_2(arbore_part_2_node *nod)
{
    if(nod == NULL)
        return;

    free_arbore_part_2(nod->next);
    free_arbore_part_2(nod->child);

    free(nod);
}

void complete_with_alpha_beta(arbore_part_2_node *root)
{
    if(root->child == NULL)
        return;
    
    if(root->type == 1)
    {
        int pruned = 0;
        root->child->alpha = root->alpha;
        root->child->beta = root->beta;
        complete_with_alpha_beta(root->child);
        int max_value_children = root->child->value;
        if(max_value_children > root->alpha)
        {
            root->alpha = max_value_children;
        }
        if(root->alpha >= root->beta)
        {
            free_arbore_part_2(root->child->next);
            root->child->next = NULL;
            pruned = 1;
        }
        arbore_part_2_node *nod = root->child->next;
        while(nod != NULL && pruned == 0)
        {
            nod->alpha = root->alpha;
            nod->beta = root->beta;
            complete_with_alpha_beta(nod);
            int value_of_child = nod->value;
            if(value_of_child > max_value_children)
            {
                max_value_children = value_of_child;
            }
            if(max_value_children > root->alpha)
            {
                root->alpha = max_value_children;
            }
            nod = nod->next;
            if(root->alpha >= root->beta && nod != NULL) 
            {
                max_value_children = nod->prev->value;
                nod->prev->next = NULL;
                free_arbore_part_2(nod);
                pruned = 1;
            }
        }
        root->value = max_value_children;
    }

    if(root->type == -1)
    {
        int pruned = 0;
        root->child->alpha = root->alpha;
        root->child->beta = root->beta;
        complete_with_alpha_beta(root->child);
        int min_value_children = root->child->value;
        if(min_value_children < root->beta)
        {
            root->beta = min_value_children;
        }
        if(root->alpha >= root->beta)
        {
            free_arbore_part_2(root->child->next);
            root->child->next = NULL;
            pruned = 1;
        }
        arbore_part_2_node *nod = root->child->next;
        while(nod != NULL && pruned == 0)
        {
            nod->alpha = root->alpha;
            nod->beta = root->beta;
            complete_with_alpha_beta(nod);
            int value_of_child = nod->value;
            if(value_of_child < min_value_children)
            {
                min_value_children = value_of_child;
            }
            if(min_value_children < root->beta)
            {
                root->beta = min_value_children;
            }
            nod = nod->next;
            if(root->alpha >= root->beta && nod != NULL)
            {
                min_value_children = nod->prev->value;
                nod->prev->next = NULL;
                free_arbore_part_2(nod);
                pruned = 1;
            }
        }
        root->value = min_value_children;
    }
}

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

int level_from_root_bonus(arbore_bonus_node *nod)
{
    int level = 0;
    arbore_bonus_node *aux = nod;
    while(aux->parent != NULL)
    {
        level++;
        aux = aux->parent;
    }
    return level;
}

void print_tabs_bonus(arbore_bonus_node *nod, FILE *fisier_out)
{
    int number_of_tabs = level_from_root_bonus(nod);
    int i;
    for(i = 0; i < number_of_tabs; i++)
    {
        fprintf(fisier_out, "\t");
    }
}

void print_with_brakets(arbore_bonus_node *nod, FILE *fisier_out, int nr_players)
{
    fprintf(fisier_out, "[");
    int i;
    for(i = 0; i < nr_players - 1; i++)
    {
        fprintf(fisier_out, "%d,", nod->values[i]);
    }
    fprintf(fisier_out, "%d]\n", nod->values[nr_players - 1]);
}

void print_tree_bonus(arbore_bonus_node *root, FILE *fisier_out, int nr_players)
{
    if(root == NULL)
        return;

    print_tabs_bonus(root, fisier_out);
	print_with_brakets(root, fisier_out, nr_players);

    print_tree_bonus(root->child, fisier_out, nr_players);
    print_tree_bonus(root->next, fisier_out, nr_players);
}

void push_child_bonus(arbore_bonus_node **head, arbore_bonus_node *nod_nou, arbore_bonus_node *parent)
{
    if((*head) == NULL)
    {
        nod_nou->next = *head;
        *head = nod_nou;
    }
    else
    {
        arbore_bonus_node *i = *head;
        while(i->next)
        {
            i = i->next;
        }
        i->next = nod_nou;
        nod_nou->prev = i;
    }
    nod_nou->parent = parent;
}

void braket_to_int_bonus(char *line, int nr_players, int type, int *vector)
{
	char *auxiliar = malloc((strlen(line) + 1) * sizeof(char));
	int i;
	if(type == 1)
	{
		auxiliar[0] = line[1];
		auxiliar[1] = '\0';
		vector[0] = atoi(auxiliar);
	}
	if(type == 0)
	{
		int index = 0;
        char *p = malloc(strlen(line) * sizeof(char));
        strcpy(p, line + 1);
        char *pointer;
        pointer = p;
        while(pointer[0] != '\0')
        {
            char *aux = malloc((strlen(pointer) + 1) * sizeof(char));
            strcpy(aux, pointer);
            for(i = 0; i < strlen(aux); i++)
            {
                if(aux[i] == ',' || aux[i] == ']')
                {
                    aux[i] = '\0';
                    strcpy(pointer, aux + i + 1);
                }
            }
            vector[index] = atoi(aux);
            index++;
            free(aux);
        }
        free(p);
	}
    free(auxiliar);
}

void transform_row_bonus(char *line, int **vector_values, int *vector_type_of_brakets, int nr_players)
{
    int i = 0;
    char *p = strtok(line, " ");
    while(p != NULL)
    {
        if(p[0] == '(')
        {
            vector_type_of_brakets[i] = 1;
        }
        else
        {
            vector_type_of_brakets[i] = 0;
        }
        braket_to_int_bonus(p, nr_players, vector_type_of_brakets[i], vector_values[i]);
        p = strtok(NULL, " ");
        i++;
    }
}

void work_with_line_bonus(arbore_bonus_node *root, int index_of_line, int *index_of_vector, int **vector_values, int *vector_type_of_brakets, int nr_players)
{
    if(root == NULL)
        return;
	
    int i;
    if(level_from_root_bonus(root) == index_of_line)
    {
        if(vector_type_of_brakets[(*index_of_vector)] == 1)
        {
            for(i = 0; i < vector_values[(*index_of_vector)][0]; i++)
            {
                arbore_bonus_node *nod_nou = new_node_bonus(nr_players);
				if((index_of_line + 1) % nr_players == 0)
				{
					nod_nou->type = 1;
				}
				else
				{
					nod_nou->type = -1;
				}
                push_child_bonus(&(root->child), nod_nou, root);
            }
        }
        else
        {
			for(i = 0; i < nr_players; i++)
			{
				root->values[i] = vector_values[(*index_of_vector)][i];
			}
        }
        *index_of_vector = *index_of_vector + 1;
    }
	
    work_with_line_bonus(root->child, index_of_line, index_of_vector, vector_values, vector_type_of_brakets, nr_players);
    work_with_line_bonus(root->next, index_of_line, index_of_vector, vector_values, vector_type_of_brakets, nr_players);
}

int *maxim_in_list_bonus(arbore_bonus_node *head)
{
    arbore_bonus_node *aux = head->next;
	int *adress = head->values;
    int maxim = head->values[0];
    while(aux != NULL)
    {
        if(aux->values[0] > maxim)
        {
            maxim = aux->values[0];
			adress = aux->values;
        }
        aux = aux->next;
    }
    return adress;
}

int *minim_in_list_bonus(arbore_bonus_node *head)
{
    arbore_bonus_node *aux = head->next;
	int *adress = head->values;
    int minim = head->values[0];
    while(aux != NULL)
    {
        if(aux->values[0] < minim)
        {
            minim = aux->values[0];
			adress = aux->values;
        }
        aux = aux->next;
    }
    return adress;
}

void complete_tree_with_mini_max_bonus(arbore_bonus_node *root, int index_of_line, int nr_players)
{
    if (root == NULL)
        return;

    if (level_from_root_bonus(root) == index_of_line)
    {
        if (root->child != NULL)
        {
            if (root->type == 1)
            {
				int i;
				int *vector = maxim_in_list_bonus(root->child);
                for(i = 0; i < nr_players; i++)
				{
					root->values[i] = vector[i];
				}
            }
            if (root->type == -1)
            {
				int i;
                int *vector = minim_in_list_bonus(root->child);
				for(i = 0; i < nr_players; i++)
				{
					root->values[i] = vector[i];
				}
            }
        }
    }

    complete_tree_with_mini_max_bonus(root->child, index_of_line, nr_players);
    complete_tree_with_mini_max_bonus(root->next, index_of_line, nr_players);
}

void free_arbore_bonus(arbore_bonus_node *nod)
{
    if(nod == NULL)
        return;

    free_arbore_bonus(nod->next);
    free_arbore_bonus(nod->child);

    free(nod->values);
    free(nod);
}

int main(int argc, char **argv)
{
    if(strstr(argv[1], "-c1"))
    {
        int nr_rows, nr_columns, i;
        char player;
        FILE *fisier_in = fopen(argv[2], "r");
        FILE *fisier_out = fopen(argv[3], "w");
        fscanf(fisier_in,"%d %d %c",&nr_rows, &nr_columns, &player);
        fgetc(fisier_in);
        arbore_part_1_node *root = new_node(nr_rows, nr_columns);
        for(i = 0; i < nr_rows; i++)
        {
            fgets(root->game_matrix[i], 2 * nr_columns, fisier_in);
            fgetc(fisier_in);
            root->game_matrix[i][2 * nr_columns - 1] = '\0';
        }
        root->player = player;
        create_all_nodes(root, nr_rows, nr_columns);
        print_matrix(root, nr_rows, nr_columns, fisier_out);
        fclose(fisier_in);
        fclose(fisier_out);
        free_everything(root, nr_rows, nr_columns);
    }
    if(strstr(argv[1], "-c2"))
    {       
        int nr_rows, i;
        char *input_buffer = malloc(10000000 * sizeof(char));
        int *vector_values = malloc(5000000 * sizeof(int));
        int *vector_type_of_brakets = malloc(5000000 * sizeof(int));
        arbore_part_2_node *root = new_node_part_2();
        root->type = 1;
        FILE *fisier_in = fopen(argv[2], "r");
        FILE *fisier_out = fopen(argv[3], "w");
        fscanf(fisier_in, "%d", &nr_rows);
        fgetc(fisier_in);
        for(i = 0; i < nr_rows; i++)
        {
            fgets(input_buffer, 10000000,fisier_in);
            input_buffer[strlen(input_buffer) - 1] = '\0';
            transform_row(input_buffer, vector_values, vector_type_of_brakets);
            int index = 0;
            work_with_line(root, i, &index, vector_values, vector_type_of_brakets);
        }
        for(i = nr_rows - 2; i >= 0; i--)
        {
            complete_tree_with_mini_max(root, i);
        }
        print_tree(root, fisier_out);
        free_arbore_part_2(root);
        fclose(fisier_in);
        fclose(fisier_out);
        free(input_buffer);
        free(vector_values);
        free(vector_type_of_brakets);
    }
    if(strstr(argv[1], "-c3"))
    {
        int nr_rows, i;
        char *input_buffer = malloc(10000000 * sizeof(char));
        int *vector_values = malloc(5000000 * sizeof(int));
        int *vector_type_of_brakets = malloc(5000000 * sizeof(int));
        arbore_part_2_node *root = new_node_part_2();
        root->type = 1;
        FILE *fisier_in = fopen(argv[2], "r");
        FILE *fisier_out = fopen(argv[3], "w");
        fscanf(fisier_in, "%d", &nr_rows);
        fgetc(fisier_in);
        for(i = 0; i < nr_rows; i++)
        {
            fgets(input_buffer, 10000000,fisier_in);
            input_buffer[strlen(input_buffer) - 1] = '\0';
            transform_row(input_buffer, vector_values, vector_type_of_brakets);
            int index = 0;
            work_with_line(root, i, &index, vector_values, vector_type_of_brakets);
        }
        root->alpha = -999999999;
        root->beta =999999999;
        complete_with_alpha_beta(root);
        print_tree(root, fisier_out);
        free_arbore_part_2(root);
        fclose(fisier_in);
        fclose(fisier_out);
        free(input_buffer);
        free(vector_values);
        free(vector_type_of_brakets);
    }
    if(strstr(argv[1], "b"))
    {
        int nr_rows, nr_players, i;
        FILE *fisier_in = fopen(argv[2], "r");
        FILE *fisier_out = fopen(argv[3], "w");
        fscanf(fisier_in, "%d %d", &nr_rows, &nr_players);
        fgetc(fisier_in);
        char *input_buffer = malloc(1000000 * sizeof(char));
        int **vector_values = malloc(500000 * sizeof(int *));
        int *vector_type_of_brakets = malloc(500000 * sizeof(int));
        arbore_bonus_node *root = new_node_bonus(nr_players);
        root->type = 1;
        for(i = 0; i < 500000; i++)
        {
            vector_values[i] = malloc(nr_players * sizeof(int));
        }
        for(i = 0; i < nr_rows; i++)
        {
            fgets(input_buffer, 1000000,fisier_in);
            input_buffer[strlen(input_buffer) - 1] = '\0';
            transform_row_bonus(input_buffer, vector_values, vector_type_of_brakets, nr_players);
            int index = 0;
            work_with_line_bonus(root, i, &index, vector_values, vector_type_of_brakets, nr_players);
        }
		for(i = nr_rows - 2; i >= 0; i--)
        {
            complete_tree_with_mini_max_bonus(root, i, nr_players);
        }
		print_tree_bonus(root, fisier_out, nr_players);
        free_arbore_bonus(root);
        fclose(fisier_in);
        fclose(fisier_out);
        for(i = 0; i < 500000; i++)
        {
            free(vector_values[i]);
        }
        free(input_buffer);
        free(vector_values);
        free(vector_type_of_brakets);
    }
    return 0;
}