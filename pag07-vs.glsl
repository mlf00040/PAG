#version 410
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 color;

uniform mat4 mVision;
uniform mat4 mProjeccion;
uniform mat4 mModelado;
uniform mat4 mMVP;

uniform vec3 uColorDifuso;

subroutine vec4 fCalcularColor();
subroutine uniform fCalcularColor uMetodoColorElegido;

subroutine ( fCalcularColor )
vec4 colorRGB ()
{  return vec4 ( color, 1 );
}

subroutine ( fCalcularColor )
vec4 colorMaterial ()
{  return vec4 ( uColorDifuso, 1 );
}

out vec3 vColor;

void main ()
{  gl_Position = mMVP * vec4(posicion, 1.0);
   vColor =  uMetodoColorElegido().rgb;
};