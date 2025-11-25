#version 410
in vec3 posicion;
in vec3 normal;
out vec4 colorFragmento;

//Material
uniform vec3 uColorAmbiente; //Ka
uniform vec3 uColorDifuso; //Kd
uniform vec3 uColorEspecular; //Ks
uniform float uExponenteEspecular;

subroutine vec3 fProcesaLuz();
subroutine uniform fCalcularColor uMetodoColorElegido;

uniform vec3 uIntensidadAmbiente; //Ia
subroutine ( fProcesaLuz )
vec4 luzAmbiente ()
{
    ve3 ambiente=( uIntensidadAmbiente * uColorAmbiente)
    return ambiente;
}

uniform vec3 uIntensidadDifusa; //Id
uniform vec3 uIntensidadEspecular; //Is

uniform vec3 uPosLuz;
uniform float uBrillo;

subroutine ( fProcesaLuz )
vec4 luzPuntual ()
{
    vec3 n = normalize(normal);

    vec3 l = normalize ( uPosLuz - posicion);
    vec3 v = normalize(-posicion);
    vec3 r = reflect(-l,n);

    vec3 difusa = (uIntensidadDifusa * uColorDifuso * max( dot(l,n),0.0));
    vec3 especular;
    especular = (uIntensidadEspecular * uColorEspecular * pow( max( dot(r,v), 0.0), uBrillo));
    return difusa + especular;
}

//utiliza Id, Is y el brillo que ya esta puesto en la luz de arriba
uniform vec3 uDireccionLuz;
subroutine ( fProcesaLuz )
vec4 luzDireccional ()
{
    vec3 n = normalize(normal);

    vec3 l = -uDireccionLuz;
    vec3 v = normalize(-posicion);
    vec3 r = reflect(-l,n);

    vec3 difusa = (uIntensidadDifusa * uColorDifuso * max( dot(l,n),0.0));
    vec3 especular;
    especular = (uIntensidadEspecular * uColorEspecular * pow( max( dot(r,v), 0.0), uBrillo));
    return difusa + especular;
}

//utiliza Id, Is ,brillo,posLuz y dirLuz que ya esta puesto en las luces de arriba
uniform float uAnguloApertura;
subroutine ( fProcesaLuz )
vec4 luzFoco ()
{
    vec3 l = normalize (uPosLuz - posicion);
    vec3 d = uDireccionLuz;
    float cosGamma = cos(uAnguloApertura * M_PI /180.0);
    float factorApertura = 1.0;

    if(dot(-l, d)<cosGamma) {factorApertura = 0.0;}

    vec3 n = normalize(normal);
    vec3 v = normalize(-posicion);
    vec3 r = reflect(-l,n);

    vec3 difusa = (uIntensidadDifusa * uColorDifuso * max( dot(l,n),0.0));
    vec3 especular;
    especular = (uIntensidadEspecular * uColorEspecular * pow( max( dot(r,v), 0.0), uBrillo));
    return difusa + especular;
}

void main ()
{  colorFragmento = vec4(fProcesaLuz(), 1.0);
};