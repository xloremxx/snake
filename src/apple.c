#include <stdio.h>
#include <stdlib.h>
#include <main.h>
#include <linmath.h>
#include <time.h>

static vec2 position = { 0, 0 };

static int local_rand_i(int min, int max)
{
  return rand() % max;
}

int apple_main()
{

  srand(time(NULL));

  position[0] = local_rand_i(0, 32);
  position[1] = local_rand_i(0, 24);

  return 0;
}

int apple_loop()
{
  program_set_color_3f(1.1f, 0.1f, 0.3f);
  draw_rect(position[0], position[1]);
}

int apple_position_get_x()
{
  return (int) position[0];
}

int apple_position_get_y()
{
  return (int) position[1];
}

int apple_move_to_new_location()
{
  position[0] = local_rand_i(0, 32);
  position[1] = local_rand_i(0, 24);
}
