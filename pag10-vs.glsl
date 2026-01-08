#version 410

//atributos
layout (location = 0) in vec3 vPosicion;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vcTextura;
layout (location = 3) in vec3 vTangente;


//matrices
uniform mat4 mModelado;
uniform mat4 mVision;
uniform mat4 mProyeccion;

//datos de la luz
uniform vec3 uPosLuz;
uniform vec3 uDireccionLuz;

//salidas
out vec3 posicion;
out vec3 posicionTg;
out vec3 normal;
out vec3 vColor;
out vec2 cTextura;
out vec3 posLuzTg;
out vec3 dirLuzTg;

void main ()
{
   //calculos de matrices necesarias (se podria hacer en el codigo y pasarlo directamente que seria mas eficiente)
   mat4 mMV = mVision * mModelado;

   mat4 matrizMVP = mProyeccion * mVision * mModelado; // Matriz de modelado, visión y proyección
   mat4 matrizMV = mVision * mModelado; // Matriz de modelado y visión
   mat4 matrizMVit = transpose(inverse((matrizMV)));; // Traspuesta de la inversa de matrizMV

   vec3 normalMV = normalize ( vec3 ( matrizMVit * vec4 ( vNormal, 0 ) ) );
   vec3 tangenteMV = normalize ( vec3 ( matrizMVit * vec4 ( vTangente, 0 ) ) );
   vec3 bitangenteMV = normalize ( cross ( normalMV , tangenteMV  ) );
   mat3 TBN = transpose ( mat3 ( tangenteMV, bitangenteMV, normalMV ) );
   posicionTg = TBN * vec3 ( matrizMV * vec4 ( vPosicion, 1 ) );
   posLuzTg = TBN * vec3 ( mVision * vec4 ( uPosLuz, 1.0 ) );
   dirLuzTg = TBN * normalize ( mat3 ( mVision ) * uDireccionLuz );

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
