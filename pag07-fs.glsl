#version 410
in vec3 vColor;
out vec4 colorFragmento;
subroutine vec4 SeleccionaColor();

subroutine uniform SeleccionaColor srSeleccionaColor;


//color difuso del material (Un ejemplo por ahora naranja)
subroutine(SeleccionaColor)
vec4 colorDifuso() {
return vec4(0.8, 0.4, 0.2, 1.0);
}

//rojo
subroutine(SeleccionaColor)
vec4 colorRojo() {
return vec4(1.0, 0.0, 0.0, 1.0);
}



void main ()
{  colorFragmento = srSeleccionaColor();
};