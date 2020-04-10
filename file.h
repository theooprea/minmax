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

#endif /* __FILE_H__ */