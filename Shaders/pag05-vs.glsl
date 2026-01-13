#version 410
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 color;

uniform mat4 mVision;
uniform mat4 mProjeccion;

out vec3 vColor;
void main ()
{  gl_Position = mProjeccion * mVision * vec4(posicion, 1.0);
   vColor = color;
};