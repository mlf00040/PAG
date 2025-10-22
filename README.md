# PAG
Proyecto de la asigantura de Programacion de aplicaciones graficas. 4º año Ingenieria Informatica


# Reflexion sesion 1

yo creo que la clase renderer se deberia de implementar como una clase normal, que tenga un atributo que sea un puntuero a si mismo. Asi si se llama desde cualquier metodo, al invocarlo ya tendriamos acceso a la clase completa y al propio metodo para recargar el renderer desde los callbacks.

![Eje1](ImagenesReadme/Eje1.png)

# Ejercicio 5 Sesion 3

### Si redimensionas la ventana de la aplicación, verás que el triángulo no permanece igual, sino que se deforma al mismo tiempo que la ventana. ¿A qué crees que se debe este comportamiento? 

Creo que este comportamiento se debe al proceso de pipeline que estamos haciendo, ya que se llama al view port para rescalado al mover el tamaño pero en el codigo, no se vuelve a aplicar la transformacion de visualizacion ni la transformación de proyección, deformando asi el triangulo

# Ejercicio 5 Sesion 5

Para aplicar la camara he creado una nueva clase camara con los parametros correspondientes a ella y los metodos para sus respectivos movimientos. He añadido además dos metodos qeu devuelven la matriz de proyeccion y la matriz de vision para facilitar las cosas.

Se conecta de manera que el renderer tiene un atributo camara. El controlador del imgui tiene nuevos parametros para el uso de la camara y tiene dos funciones nuevas, una que es unicamente para dibujar la ventana y la seleccion del movimiento de la camara y la otra que es para aplicar dicho movimiento llamando a la camara del renderer.

He tenido que añadir tambien un callback nuevo para trackear la posicion del raton y modificar el de pulsar el raton para que solo realice movimiento si el raton no se esta usando en el imgui. El propio callback de la posicion es el que llama al aplicar movimiento ya que es el que puede pasarle los parametros x e y.


```mermaid
classDiagram


    class Renderer {
        
    }

    class ControllerImgui {
        
    }

    class ControllerMensajes {
        
    }

    class Camara{

    }

    Renderer -->  Camara : tiene
    ControllerImgui  -->  Renderer : usa
    ControllerImgui  -->  ControllerMensajes : usa
