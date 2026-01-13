#version 410

//atributos
layout (location = 0) in vec3 vPosicion;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vcTextura;

//matrices
uniform mat4 mModelado;
uniform mat4 mVision;
uniform mat4 mProyeccion;


//salidas
out vec3 posicion;
out vec3 normal;
out vec3 vColor;
out vec2 cTextura;

void main ()
{
   //transformacion a posicion del mundo
   vec4 posicionW = mModelado * vec4(vPosicion, 1.0);
   posicion = posicionW.xyz;

   vColor=vNormal;

   cTextura = vcTextura;

   //transformacion de las normales
   mat3 normalMatrix = transpose(inverse(mat3(mModelado)));
   normal = normalize(normalMatrix * vNormal);

   gl_Position = mProyeccion * mVision * posicionW;

}