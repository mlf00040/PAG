#version 410

//atributos
layout (location = 0) in vec3 vertice;

//matrices
uniform mat4 matrizModVisProy;

//datos de la luz

//salidas

void main ()
{
   gl_Position = matrizModVisProy * vec4(vertice,1.0);
}
