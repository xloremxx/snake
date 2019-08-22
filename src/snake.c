#include <main.h>
#include <stdio.h>
#include <stdlib.h>
#include <linmath.h>

static int tick = 0;

struct node {
  vec2 position;
  struct node * next;
};

struct snake {
  int direction;
};

static struct node * tail;
static struct node * head;

static struct snake * snake;

int snake_main()
{

  snake = malloc(sizeof(struct snake));
  snake->direction = SNAKE_DIRECTION_UP;

  tail = malloc(sizeof(struct node));
  tail->position[0] = 1;
  tail->position[1] = 2;
  tail->next = NULL;

  tail->next = malloc(sizeof(struct node));
  tail->next->position[0] = 2;
  tail->next->position[1] = 2;
  tail->next->next = NULL;

  tail->next->next = malloc(sizeof(struct node));
  tail->next->next->position[0] = 3;
  tail->next->next->position[1] = 2;
  tail->next->next->next = NULL;

  tail->next->next->next = malloc(sizeof(struct node));
  tail->next->next->next->position[0] = 4;
  tail->next->next->next->position[1] = 2;
  tail->next->next->next->next = NULL;

  head = tail->next->next->next;

  return 0;
}

int snake_set_direction(int direction) 
{
  if (snake->direction == SNAKE_DIRECTION_UP && direction == SNAKE_DIRECTION_DOWN) {
    return -1;
  } else if (snake->direction == SNAKE_DIRECTION_DOWN && direction == SNAKE_DIRECTION_UP) {
    return -1;
  } else if (snake->direction == SNAKE_DIRECTION_LEFT && direction == SNAKE_DIRECTION_RIGHT) {
    return -1;
  } else if (snake->direction == SNAKE_DIRECTION_RIGHT && direction == SNAKE_DIRECTION_LEFT) {
    return -1;
  }

  snake->direction = direction;
}

int snake_update()
{

  struct node * current = tail;
  while (current->next != NULL) {
    current->position[0] = current->next->position[0];
    current->position[1] = current->next->position[1];
    current = current->next;
  }
  if (snake->direction == SNAKE_DIRECTION_UP) {
    current->position[1] = current->position[1] + 1;
  } else if (snake->direction == SNAKE_DIRECTION_DOWN) {
    current->position[1] = current->position[1] - 1;
  } else if (snake->direction == SNAKE_DIRECTION_LEFT) {
    current->position[0] = current->position[0] - 1;
  } else if (snake->direction == SNAKE_DIRECTION_RIGHT) {
    current->position[0] = current->position[0] + 1;
  }

  if (head->position[0] < 0) {
    head->position[0] = 32;
  } else if (head->position[0] > 32) {
    head->position[0] = 0;
  } else if (head->position[1] < 0) {
    head->position[1] = 24;
  } else if (head->position[1] > 24) {
    head->position[1] = 0;
  }

  if (tick % 5 == 0) {
    printf("Tick: '%d'.\n", tick);
  }

  if (apple_position_get_x() == head->position[0] && apple_position_get_y() == head->position[1]) {
    struct node * new_node = malloc(sizeof(struct node));
    new_node->position[0] = tail->position[0];
    new_node->position[1] = tail->position[1];
    new_node->next = tail;
    tail = new_node;
    apple_move_to_new_location();
  }

  tick += 1;

  return 0;
}

int snake_loop()
{

  program_set_color_3f(0.0f, 1.0f, 1.0f);

  struct node * current = tail;
  while (current->next != NULL) {
    draw_rect(current->position[0], current->position[1]);
    current = current->next;
  }

  program_set_color_3f(0.1f, 0.5f, 1.0f);
  draw_rect(head->position[0], head->position[1]);

  return 0;
}
