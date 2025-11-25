#version 410
layout (location = 0) in vec3 vPosicion;
layout (location = 1) in vec3 vNormal;
//  layout (location = 1) in vec3 vColor;


uniform mat4 mVision;
uniform mat4 mProjeccion;
uniform mat4 mModelado;
uniform mat4 mMVP;

//  uniform vec3 uColorDifuso;

/*
subroutine vec4 fCalcularColor();
subroutine uniform fCalcularColor uMetodoColorElegido;

subroutine ( fCalcularColor )
vec4 colorRGB ()
{  return vec4 ( vColor, 1 );
}

subroutine ( fCalcularColor )
vec4 colorMaterial ()
{  return vec4 ( uColorDifuso, 1 );
}
*/

//  out vec3 color;
out vec3 posicion;
out vec3 normal;

void main ()
{
   mat3 mNormal = transpose(inverse(mat3(mModelado)));
   normal = vec3(mNormal * vec4(vNormal,0.0));
   posicion = vec3( mModelado * vec4(vPosicion,1.0));
   gl_Position = mMVP * vec4(vPosicion, 1.0);
   //  color =  uMetodoColorElegido().rgb;
};