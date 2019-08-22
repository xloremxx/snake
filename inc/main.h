#ifndef MY_MAIN_H
#define MY_MAIN_H

#define SNAKE_DIRECTION_LEFT 1
#define SNAKE_DIRECTION_RIGHT 2
#define SNAKE_DIRECTION_UP 3
#define SNAKE_DIRECTION_DOWN 4

int snake_main();
void draw_rect(int x, int y);
int snake_update();
int snake_loop();
int snake_set_direction(int direction);
void program_set_color_3f(float r, float g, float b);
int apple_main();
int apple_loop();
int apple_position_get_x();
int apple_position_get_y();
int apple_move_to_new_location();

#endif
