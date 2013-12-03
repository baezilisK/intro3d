#ifndef TREE_H
#define TREE_H

int tree_n; /* number of trees */
float *tree_x, *tree_y; /* tree coordinates */

void tree_displayall (void); /* display all trees */

void tree_free (void); /* free any memory associated with this module */

#endif /* TREE_H */
