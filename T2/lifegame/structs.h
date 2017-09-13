#include <stdbool.h>


struct Matrix {
    int rows, cols;
    bool** cells;
};
typedef struct Matrix Matrix;


Matrix* init_matrix(int n_row, int n_col);
/**
 * @brief resumen
 *
 * @param row descripcion del parametro
 *
 * @return En este caso nada
*/

void* poblate_matrix(Matrix* matrix, int *starter, int size);

void check_neighbours(int row, int col, Matrix* matrix, Matrix* future_m);

void print_matrix(Matrix* matrix);

void destroy_matrix(Matrix* matrix);


// Lifegame
void lifegame(Matrix* matrix, Matrix* future_matrix);
