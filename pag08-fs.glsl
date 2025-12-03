#version 410
//entradas
in vec3 posicion;
in vec3 normal;
in vec3 vColor;

//salidas
out vec4 colorFragmento;

//Material
uniform vec3 uColorAmbiente; //Ka
uniform vec3 uColorDifuso; //Kd
uniform vec3 uColorEspecular; //Ks
uniform float uExponenteEspecular;

//luz ambiente
uniform vec3 uIntensidadAmbiente; //Ia

//luz puntual/foco
uniform vec3 uIntensidadDifusa; //Id
uniform vec3 uIntensidadEspecular; //Is
uniform vec3 uPosLuz;

//luz direccional/foco
uniform vec3 uDireccionLuz;

//luz foco
uniform float uAnguloApertura;

//subrutina
subroutine vec3 fProcesaLuz();

subroutine uniform fProcesaLuz uMetodoLuzElegido;

subroutine(fProcesaLuz)
vec3 colorRGB() {
    return vColor;
}

//luz ambiente
subroutine(fProcesaLuz)
vec3 luzAmbiente ()
{
    vec3 ambiente=(uIntensidadAmbiente * uColorAmbiente);
    return ambiente;
}
//luz puntual
subroutine(fProcesaLuz)
vec3 luzPuntual ()
{
    vec3 n = normalize(normal);

    vec3 l = normalize ( uPosLuz - posicion);
    vec3 v = normalize(-posicion);
    vec3 r = reflect(-l,n);

    vec3 difusa = (uIntensidadDifusa * uColorDifuso * max( dot(l,n),0.0));
    vec3 especular;
    especular = (uIntensidadEspecular * uColorEspecular * pow( max( dot(r,v), 0.0), uExponenteEspecular));
    return difusa + especular;
}

//luz direccional
subroutine(fProcesaLuz)
vec3 luzDireccional ()
{
    vec3 n = normalize(normal);

    vec3 l = -uDireccionLuz;
    vec3 v = normalize(-posicion);
    vec3 r = reflect(-l,n);

    vec3 difusa = (uIntensidadDifusa * uColorDifuso * max( dot(l,n),0.0));
    vec3 especular;
    especular = (uIntensidadEspecular * uColorEspecular * pow( max( dot(r,v), 0.0), uExponenteEspecular));
    return difusa + especular;
}

//luz foco
subroutine(fProcesaLuz)
vec3 luzFoco ()
{
    vec3 l = normalize (uPosLuz - posicion);
    vec3 d = uDireccionLuz;
    float cosGamma = cos(radians(uAnguloApertura));
    float factorApertura = 1.0;

    if(dot(-l, d) < cosGamma) {factorApertura = 0.0;}

    vec3 n = normalize(normal);
    vec3 v = normalize(-posicion);
    vec3 r = reflect(-l,n);

    vec3 difusa = (uIntensidadDifusa * uColorDifuso * max( dot(l,n),0.0));
    vec3 especular = (uIntensidadEspecular * uColorEspecular * pow( max( dot(r,v), 0.0), uExponenteEspecular));
    return factorApertura * (difusa + especular);
}

void main ()
{  colorFragmento = vec4(uMetodoLuzElegido(), 1.0);
}