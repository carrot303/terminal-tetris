#include "tetrominoes.h"

void display_grid();
void display_cell(int, int);
struct shape pick_shape(void);
struct c_shape insert_shape(struct shape);


int remove_filled_rows();