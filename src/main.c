#include <linmath.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int success = 0;
static GLFWwindow* window;
static int program = 0;
static int shader = 0;
static char info[512];
static char *source = NULL;
static unsigned int vao, vbo;

static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) 
{

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
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

  float vertices[] = {
    -0.5f, -0.5f, -0.0f,
    +0.5f, -0.5f, -0.0f,
    +0.0f, +0.5f, -0.0f,
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

  /* set context clear color */
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  /* the main loop */
  while (!glfwWindowShouldClose(window))
  {

    /* clear the color buffer */
    glClear(GL_COLOR_BUFFER_BIT);

    /* draw */
    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0);

    /* swap the buffers */
    glfwSwapBuffers(window);

    /* poll events */
    glfwPollEvents();

  }

  /* terminate GLFW */
  glfwTerminate();

  return 0;
}
