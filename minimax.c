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
    nod_nou->nr_of_children = 0;
    nod_nou->child = NULL;
    nod_nou->parent = NULL;
    nod_nou->next = NULL;
    nod_nou->prev = NULL;
    return nod_nou;
}

void transform_row(char *line, int *vector_values, int *vector_type_of_brakets)
{
    int i = 0;
    int type_of_braket, value;
    char *p = strtok(line, " ");
    char *to_transform_to_int, *p2;
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
        printf("%s ", p);
        p = strtok(NULL, " ");
        i++;
    }
    printf("\n");
}

int braket_to_int(char *line)
{
    
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
        char input_buffer[500000];
        int vector_values[100000], vector_type_of_brakets[100000];
        arbore_part_2_node *root = new_node_part_2();
        FILE *fisier_in = fopen(argv[2], "r");
        FILE *fisier_out = fopen(argv[3], "w");
        fscanf(fisier_in, "%d", &nr_rows);
        fgetc(fisier_in);
        printf("%d\n", nr_rows);
        fscanf(fisier_in, "(%d)", &(root->nr_of_children));
        fgetc(fisier_in);
        printf("%d\n", root->nr_of_children);
        for(i = 0; i < nr_rows; i++)
        {
            fgets(input_buffer, 500000,fisier_in);
            input_buffer[strlen(input_buffer) - 1] = '\0';
            printf("%s %ld\n", input_buffer, strlen(input_buffer));
            transform_row(input_buffer, vector_values, vector_type_of_brakets);
        }
    }
    if(strstr(argv[1], "-c3"))
    {
        printf("-c3\n");
    }
    if(strstr(argv[1], "b"))
    {
        printf("-b\n");
    }
    return 0;
}