#include "screen.h"


#define SHAPE_INDEX(name) ((name == 'O') ? O_SHAPE : (name == 'L') ? L_SHAPE : \
                           (name == 'J') ? J_SHAPE : (name == 'I') ? I_SHAPE : \
                           (name == 'S') ? S_SHAPE : (name == 'Z') ? Z_SHAPE : \
                           (name == 'T') ? T_SHAPE : 0)
#define COLLISION_OTHER_SHAPE(x,y,s) (!is_point_shape(x, y, s) && board[x][y] != '\0')

enum Shapes {O_SHAPE, L_SHAPE, J_SHAPE, I_SHAPE, S_SHAPE, Z_SHAPE, T_SHAPE};

struct shape {
   char name;
   char structure[4][4];
   int color;
   int init_position;
};

struct point {
   int row;
   int col;
};

struct c_shape {
   struct shape ishape;
   struct point points[4];
   int direction; // 0-4
};


int move_down(void);
void move_right(void);
void move_left(void);
int shape_rotate_right(void);
int shape_rotate_left(void);
int point_rotate_right(struct point*, struct point*, int);
int point_rotate_left(struct point*, struct point*, int);
struct point get_origin_point(struct c_shape*);
void drop_shape(void);
int is_point_shape(int, int, struct c_shape*);