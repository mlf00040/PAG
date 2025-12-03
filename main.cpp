#include <iostream>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include "imfilebrowser.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "ControllerImgui.h"
#include "ControllerShaders.h"
#include "ControllerMensajes.h"



/**
 * Aspectos a pulir:
 * Lo de los nombres del modelo a la hora de seleccionarlos, ya muestra el nombre a la hora de cargarlo, falta cuando ya estan creados en el combo
 * Lo de que se le asigne un shader program a cada modelo, por ahora el mismo a todos.
 * Limpieza masiva de comentarios y codigo archivo por archivo
 * Agrupar todos los archivos glsl en una carpeta
 * Cambiar el valor de la camara por defecto al del reset
 * Elaborar las instrucciones completas
 *
 *
 */



// - Esta función callback será llamada cuando GLFW produzca algún error
void error_callback ( int errno, const char* desc )
{ std::string aux (desc);
    std::string texto = "Error de GLFW número " + std::to_string(errno) + ": " + std::string(desc);
    ControllerMensajes::getInstancia().anadirMensaje(texto);
}
// - Esta función callback será llamada cada vez que el área de dibujo
// OpenGL deba ser redibujada.
void callbackRefrescoVentana ( GLFWwindow* ventana )
{  PAG::Renderer::getInstancia().refrescar();
    glfwSwapBuffers (ventana);
    //std::string texto = "Finaliza el callback de refresco" ;
    //anadirMensaje(texto);
}
// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void framebuffer_size_callback ( GLFWwindow *window, int width, int height )
{ PAG::Renderer::getInstancia().resizeViewPort(width,height);
    std::string texto = "Resize callback called";
    ControllerMensajes::getInstancia().anadirMensaje(texto);
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
{
    ImGuiIO& io = ImGui::GetIO ();
    io.AddMouseButtonEvent ( button, true );

    if (io.WantCaptureMouse) {
        return;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        // Notifica a ControllerImgui
        GUI::ControllerImgui::getInstancia().pulsarMouse(action == GLFW_PRESS, x, y);
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
// - Esta función callback será llamada cada vez que se mueva el raton sobre el area de dibujo de opengl
void mouse_pos_callback (GLFWwindow *window, double posx, double posy){
    ImGuiIO& io = ImGui::GetIO();
    glfwGetCursorPos(window, &posx, &posy);
    GUI::ControllerImgui::getInstancia().procesaMovimiento(posx, posy);

}


    int main()
{   ControllerMensajes::getInstancia().anadirMensaje("Starting Application PAG - Prueba 01" );
    // - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
    if ( glfwInit () != GLFW_TRUE )
    { ControllerMensajes::getInstancia().anadirMensaje("Failed to initialize GLFW");
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
    {ControllerMensajes::getInstancia().anadirMensaje("Failed to open GLFW window");
        glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
        return -2;
    }
    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent ( window );

    // - Ahora inicializamos GLAD.
    if ( !gladLoadGLLoader ( (GLADloadproc) glfwGetProcAddress ) )
    {ControllerMensajes::getInstancia().anadirMensaje("GLAD initialization failed");
        glfwDestroyWindow ( window ); // - Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate ();
        return -3;
    }

    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    // 3D construido.
    PAG::Renderer::getInstancia().obtenerDatosContexto();

    // - Registramos los callbacks que responderán a los eventos principales
    glfwSetWindowRefreshCallback ( window, callbackRefrescoVentana );
    glfwSetFramebufferSizeCallback ( window, framebuffer_size_callback );
    glfwSetKeyCallback ( window, key_callback );
    glfwSetMouseButtonCallback ( window, mouse_button_callback );
    glfwSetScrollCallback ( window, scroll_callback );
    glfwSetCursorPosCallback(window,mouse_pos_callback);


    //Inicializa IMGUI
    GUI::ControllerImgui::getInstancia().inicializa(window);

    //Llamamamos al iniciador de opgengl del renderer
    PAG::Renderer::getInstancia().inicializaOpenGL();

    std::string ruta="pag03";
    std::string rutaModelo = "ModelosPruebas/vaca.obj";

    // create a file browser instance
    ImGui::FileBrowser fileDialog;

    // (optional) set browser properties
    fileDialog.SetTitle("Selector Modello");

/*
    //Llamamos a crear y cargar los shaders y al modelo
    try {
        anadirMensaje(ControllerShaders::getInstancia().crearPrograma("PAG03",ruta,ruta));
        anadirMensaje(ControllerShaders::getInstancia().usarPrograma("PAG03"));
        PAG::Renderer::getInstancia().creaModelo();
    }catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        anadirMensaje(std::string(e.what()));
    }
*/
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
        std::vector<std::string> todosMensajes = ControllerMensajes::getInstancia().getMensajes();
        GUI::ControllerImgui::getInstancia().ventanaMensajes(todosMensajes);

        //Creamos la ventana del selector de color
        GUI::ControllerImgui::getInstancia().ventanaSelecColor();

        //creamos la ventana del gestor de shaders
        GUI::ControllerImgui::getInstancia().ventanaGestionShaders();

        // Dibujamos los controladores de la camara
        GUI::ControllerImgui::getInstancia().ventanaMovimientosCamara();

        //Dibujamos la gestion de los modelos
        GUI::ControllerImgui::getInstancia().ventanaCargaModelo(fileDialog,rutaModelo);

        GUI::ControllerImgui::getInstancia().ventanaModificaModelo();

        GUI::ControllerImgui::getInstancia().ventanaGestionMateriales();

        GUI::ControllerImgui::getInstancia().ventanaGestionLuces();

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
    ControllerMensajes::getInstancia().anadirMensaje("Finishing application pag prueba");
    glfwDestroyWindow ( window ); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.


}
