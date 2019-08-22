#version 330 core

out vec4 FragColor;

uniform vec3 color = vec3(1.0f, 0.0f, 0.0f);

void main()
{
  FragColor = vec4(color.x, color.y, color.z, 1.0f);
}
