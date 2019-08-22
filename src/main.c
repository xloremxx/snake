#include <linmath.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <main.h>

static int success = 0;
static GLFWwindow* window;
static int program = 0;
static int shader = 0;
static char info[512];
static char *source = NULL;
static unsigned int vao, vbo;
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
static float size = 25.0f;

static mat4x4 model;
static mat4x4 view;
static mat4x4 projection;

void program_set_color_3f(float r, float g, float b) 
{
  glUniform3f(glGetUniformLocation(program, "color"), r, g, b);
}

static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) 
{

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
    snake_set_direction(SNAKE_DIRECTION_UP);
  } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    snake_set_direction(SNAKE_DIRECTION_DOWN);
  } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
    snake_set_direction(SNAKE_DIRECTION_LEFT);
  } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
    snake_set_direction(SNAKE_DIRECTION_RIGHT);
  }

}

static char *load_source(const char *path)
{

  char *source = NULL;

  FILE *fp = NULL;

  long bufsize = 0;

  size_t newLen = 0;

  /* open the file */
  fp = fopen(path, "r");

  /* go to the end of the file */
  if (fseek(fp, 0L, SEEK_END) != 0) {
    fprintf(stderr, "Was unable to seek end.\n");
    return NULL;
  }

  /* get the size of the file. */
  bufsize = ftell(fp);

  if (bufsize == -1) {
    fprintf(stderr, "Was unable to get the size of the file.\n");
    return NULL;
  }

  /* allocate our buffer to that size. */
  source = malloc(sizeof(char)*(bufsize+1));

  /* go back to the start of the file. */
  if (fseek(fp, 0L, SEEK_SET) != 0) {
    fprintf(stderr, "Was unable to go back to the start of the file.\n");
    return NULL;
  }

  newLen = fread(source, sizeof(char), bufsize, fp);

  if (ferror(fp) != 0) {
    fprintf(stderr, "Was unable to read file.\n");
    return NULL;
  }

  source[newLen++] = '\0';

  fclose(fp);

  return source;
}

void draw_rect(int x, int y) {

  mat4x4_identity(model);

  glBindVertexArray(vao);

  mat4x4_translate(model, x * 25, y * 25, 0.0f);
  glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (const float *)model);

  glDrawArrays(GL_TRIANGLES, 0, 6);

}

int main(int argc, char *argv[])
{

  /* initialize GLFW */
  if (!glfwInit()) {
    fprintf(stderr, "Was unable to initialize glfw.\n");
    return -1;
  }

  /* OpenGL version core 3.3 */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* turn resize off */
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  /* create the window */
  window = glfwCreateWindow(800, 600, "snake", NULL, NULL);

  if (!window) {
    fprintf(stderr, "Was unable to create window.\n");
    glfwTerminate();
    return -1;
  }

  /* set keyboard event callback */
  glfwSetKeyCallback(window, keyboard);

  /* make the context current */
  glfwMakeContextCurrent(window);

  /* load OpenGL function pointers */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Was unable to initialize GLAD.\n");
    return -1;
  }

  /* create the shader program */
  program = glCreateProgram();

  /* vertex shader */

  source = load_source("shader/default.vs");
  shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(shader, 1, (const char **)&source, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info);
    fprintf(stderr, "Was unable to compile vertex shader: '%s'.\n", info);
    return -1;
  }
  glAttachShader(program, shader);
  glDeleteShader(shader);

  /* fragment shader */

  source = load_source("shader/default.fs");
  shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shader, 1, (const char **)&source, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info);
    fprintf(stderr, "Was unable to compile fragment shader: '%s'.\n", info);
    return -1;
  }
  glAttachShader(program, shader);
  glDeleteShader(shader);

  /* link program */
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, info);
    fprintf(stderr, "Was unable to link program.\n");
    return -1;
  }

  /* set up vertex data */

  float space = 1.0f;

  float vertices[] = {
    space, space, 0.0f,
    size-space, space, 0.0f,
    size-space, size-space, 0.0f,
    size-space, size-space, 0.0f,
    space, size-space, 0.0f,
    space, space, 0.0f,
  };

  /* generate vertex array */
  glGenVertexArrays(1, &vao);

  /* generate buffer */
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  /* load identity matrices */
  mat4x4_identity(model);
  mat4x4_identity(view);
  mat4x4_identity(projection);

  /* set context clear color */
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  snake_main();
  apple_main();

  /* the main loop */
  while (!glfwWindowShouldClose(window))
  {

    float time = glfwGetTime();
    float currentFrame = glfwGetTime();

    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    /*printf("Delta: %f.\n", deltaTime);*/
    /*printf("Time: %f\n", time);*/

    /* clear the color buffer */
    glClear(GL_COLOR_BUFFER_BIT);

    /* draw */
    glUseProgram(program);

    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (const float *)view);
    mat4x4_ortho(projection, 0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (const float *)projection);

    glBindVertexArray(vao);

    static float step = 100.0f;
    static float nextTime = 0.0f;

    if (time*1000 > nextTime) {
      snake_update();
      nextTime += step;
    }

    snake_loop();
    apple_loop();

    /* swap the buffers */
    glfwSwapBuffers(window);

    /* poll events */
    glfwPollEvents();

  }

  /* terminate GLFW */
  glfwTerminate();

  return 0;
}
