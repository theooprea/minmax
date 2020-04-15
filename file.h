#ifndef __FILE_H__
#define __FILE_H__

typedef struct arbore_part_1 {
    char **game_matrix;
    struct arbore_part_1 *child;
    struct arbore_part_1 *next;
    struct arbore_part_1 *prev;
    struct arbore_part_1 *parent;
    int number_of_children;
    char player;
} arbore_part_1_node;

typedef struct arbore_part_2 {
    int value;
    struct arbore_part_2 *child;
    struct arbore_part_2 *next;
    struct arbore_part_2 *prev;
    struct arbore_part_2 *parent;
    //tipul de calcul de facut, -1 pt mini, 1 pt max, 0 pt frunza
    int type;
} arbore_part_2_node;


#endif /* __FILE_H__ */