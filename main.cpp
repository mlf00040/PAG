#include <iostream>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "ControllerImgui.h"


/**
 * todo hacer  que lo lean de un archivo externo, esta en las diapositivas
 */

std::vector<std::string> mensajes;

void anadirMensaje(const std::string& texto) {
    mensajes.push_back(texto);
}

// - Esta función callback será llamada cuando GLFW produzca algún error
void error_callback ( int errno, const char* desc )
{ std::string aux (desc);
    std::string texto = "Error de GLFW número " + std::to_string(errno) + ": " + std::string(desc);
    anadirMensaje(texto);
}
// - Esta función callback será llamada cada vez que el área de dibujo
// OpenGL deba ser redibujada.
void callbackRefrescoVentana ( GLFWwindow* ventana )
{  PAG::Renderer::getInstancia().refrescar();
    glfwSwapBuffers (ventana);
    std::string texto = "Finaliza el callback de refresco" ;
    anadirMensaje(texto);
}
// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void framebuffer_size_callback ( GLFWwindow *window, int width, int height )
{ PAG::Renderer::getInstancia().resizeViewPort(width,height);
    std::string texto = "Resize callback called";
    anadirMensaje(texto);
}
// - Esta función callback será llamada cada vez que se pulse una tecla
// dirigida al área de dibujo OpenGL.
void key_callback ( GLFWwindow *window, int key, int scancode, int action, int mods )
{ if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
    { glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    //std::string texto = "Key callback called";
    //anadirMensaje(texto);
}
// - Esta función callback será llamada cada vez que se pulse algún botón
// del ratón sobre el área de dibujo OpenGL.
void mouse_button_callback ( GLFWwindow *window, int button, int action, int mods )
{ if ( action == GLFW_PRESS )
    {   //std::string texto = "Pulsado el botón: " + std::to_string(button) ;
        //anadirMensaje(texto);
        //comunicar al evento del raton de imGui
        ImGuiIO& io = ImGui::GetIO ();
        io.AddMouseButtonEvent ( button, true );
    }
    else if ( action == GLFW_RELEASE )
    {   //std::string texto = "Soltado el botón: " + std::to_string(button);
        //anadirMensaje(texto);
        //comunicar al evento del raton de imGui
        ImGuiIO& io = ImGui::GetIO ();
        io.AddMouseButtonEvent ( button, false );
    }
}
// - Esta función callback será llamada cada vez que se mueva la rueda
// del ratón sobre el área de dibujo OpenGL.
void scroll_callback ( GLFWwindow *window, double xoffset, double yoffset )
{ /*
    std::string texto = "Movida la rueda del ratón "+ std::to_string(xoffset)
            +" Unidades en horizontal y " + std::to_string(yoffset)
                +" unidades en vertical";
    anadirMensaje(texto);
    */
/*
    if(yoffset==1){
        if(red<1 ){
            red=red+0.1;

        }else if(green <1){
            green=green+0.1;

        }else if(blue<1){
            blue=blue+0.1;

        }
    }else if(yoffset==-1){
        if(blue>0 ){
            blue=blue-0.1;

        }else if(green>0){
            green=green-0.1;

        }else if(red>0){
            red=red-0.1;

        }
    }
*/
}


    int main()
{   anadirMensaje("Starting Application PAG - Prueba 01" );
    // - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
    if ( glfwInit () != GLFW_TRUE )
    { anadirMensaje("Failed to initialize GLFW");
        return -1;
    }
    // - Definimos las características que queremos que tenga el contexto gráfico
    // OpenGL de la ventana que vamos a crear. Por ejemplo, el número de muestras o el
    // modo Core Profile.
    glfwWindowHint ( GLFW_SAMPLES, 4 ); // - Activa antialiasing con 4 muestras.
    glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // - Esta y las 2
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 4 ); // siguientes activan un contexto
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 1 ); // OpenGL Core Profile 4.1.
    // - Definimos el puntero para guardar la dirección de la ventana de la aplicación y
    // la creamos
    GLFWwindow *window;
    // - Tamaño, título de la ventana, en ventana y no en pantalla completa,
    // sin compartir recursos con otras ventanas.
    window = glfwCreateWindow ( 1024, 576, "PAG Introduction", nullptr, nullptr );
    // - Comprobamos si la creación de la ventana ha tenido éxito.
    if ( window == nullptr )
    {anadirMensaje("Failed to open GLFW window");
        glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
        return -2;
    }
    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent ( window );

    // - Ahora inicializamos GLAD.
    if ( !gladLoadGLLoader ( (GLADloadproc) glfwGetProcAddress ) )
    {anadirMensaje("GLAD initialization failed");
        glfwDestroyWindow ( window ); // - Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate ();
        return -3;
    }

    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    // 3D construido.
    std::ostringstream oss;
    oss << glGetString(GL_RENDERER) << "\n"
        << glGetString(GL_VENDOR) << "\n"
        << glGetString(GL_VERSION) << "\n"
        << glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::string info = oss.str();
    anadirMensaje(info);

    // - Registramos los callbacks que responderán a los eventos principales
    glfwSetWindowRefreshCallback ( window, callbackRefrescoVentana );
    glfwSetFramebufferSizeCallback ( window, framebuffer_size_callback );
    glfwSetKeyCallback ( window, key_callback );
    glfwSetMouseButtonCallback ( window, mouse_button_callback );
    glfwSetScrollCallback ( window, scroll_callback );


    //Inicializa IMGUI
    GUI::ControllerImgui::getInstancia().inicializa(window);

    //Llamamamos al iniciador de opgengl del renderer
    PAG::Renderer::getInstancia().inicializaOpenGL();

    //Llamamos a crear y cargar los shaders y al modelo
    try {
        PAG::Renderer::getInstancia().creaShaderProgram();
        PAG::Renderer::getInstancia().creaModelo();
    }catch (const std::exception& e) {
        anadirMensaje(std::string(e.what()));
    }


    // - Ciclo de eventos de la aplicación. La condición de parada es que la
    // ventana principal deba cerrarse. Por ejemplo, si el usuario pulsa el
    // botón de cerrar la ventana (la X).
    while ( !glfwWindowShouldClose ( window ) )
    {
    // - Obtiene y organiza los eventos pendientes, tales como pulsaciones de
    // teclas o de ratón, etc. Siempre al final de cada iteración del ciclo
    // de eventos y después de glfwSwapBuffers(window);
        glfwPollEvents ();

        //Refresco del frame del IMGUI
        GUI::ControllerImgui::getInstancia().refrescoFrames();

        // - Borra los buffers (color y profundidad)
        PAG::Renderer::getInstancia().refrescar();

        //Creamos la ventana de la consola de imgui
        GUI::ControllerImgui::getInstancia().ventanaMensajes(mensajes);

        //Creamos la ventana del selector de color
        GUI::ControllerImgui::getInstancia().ventanaSelecColor();

        // Se dibujan los controles de Dear ImGui
        // Aquí va el dibujado de la escena con instrucciones OpenGL
        GUI::ControllerImgui::getInstancia().dibujaControladores();

        // - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
        // intercambia el buffer back (en el que se ha estado dibujando) por el
        // que se mostraba hasta ahora (front).
        glfwSwapBuffers ( window );
    }

    //Liberacion de recursos de ImGui
    GUI::ControllerImgui::getInstancia().liberaRecursos();

    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    anadirMensaje("Finishing application pag prueba");
    glfwDestroyWindow ( window ); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.


}
