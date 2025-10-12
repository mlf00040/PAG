//
// Created by Suspr on 27/09/2025.
//

#include "ControllerImgui.h"
#include "ControllerShaders.h"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>




namespace GUI {

    ControllerImgui *GUI::ControllerImgui::instancia = nullptr;

    /**
    * Constructor por defecto
    */
    ControllerImgui::ControllerImgui()
            : shaderProgramName("pag03"){}

    /**
     * Destructor
     */
    ControllerImgui::~ControllerImgui() {}

    /**
    * Consulta del objeto único de la clase
    * @return La dirección de memoria del objeto
    */
    ControllerImgui &ControllerImgui::getInstancia() {
        if (!instancia)  // Lazy initialization: si aún no existe, lo crea
        {
            instancia = new ControllerImgui();
        }
        return *instancia;
    }
    /**
     * Inicializa el ImGui
     * @param window
     */
    void ControllerImgui::inicializa(GLFWwindow* window){

        IMGUI_CHECKVERSION();
        ImGui::CreateContext ();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        // Aquí w es el puntero a la ventana GLFW (GLFWwindow*)
        ImGui_ImplGlfw_InitForOpenGL ( window, true );
        ImGui_ImplOpenGL3_Init ();

    }
    /**
     * Libera los recursos del ImGui
     */
    void ControllerImgui::liberaRecursos() {

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext ();
    }
    /**
     * Refresca los frames del ImGui
     */
    void ControllerImgui::refrescoFrames() {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

    }

    void ControllerImgui::dibujaControladores() {

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData() );
    }

    /**
     * Crea la ventana que contendra la consola dentro de la aplicacion grafica y muestra los mensajes.
     * @param mensajes
     */
    void ControllerImgui::ventanaMensajes(std::vector<std::string> &mensajes) {

        ImGui::SetNextWindowPos ( ImVec2 (10, 10), ImGuiCond_Once );

        if( ImGui::Begin("Mensajes"))
        { // La ventana está desplegada
            ImGui::SetWindowFontScale ( 1.0f );   // Escalamos el texto si fuera necesario
            // Pintamos los controles
            ImGui::Text("Aplicación PAG");

            for (std::vector<std::string>::const_iterator it = mensajes.begin(); it != mensajes.end(); ++it) {
                ImGui::TextWrapped("%s", it->c_str());
            }

        }
        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    }

    /**
     * Crea la ventana para el selector de color del fondo de la aplicacion grafica
     */
    void ControllerImgui::ventanaSelecColor() {

        ImGui::SetNextWindowPos ( ImVec2 (200, 100), ImGuiCond_Once );

        if( ImGui::Begin("Selector Color"))
        { // La ventana está desplegada
            ImGui::SetWindowFontScale ( 1.0f );   // Escalamos el texto si fuera necesario
            //selector de color
            static ImVec4 clear_color = ImVec4(PAG::Renderer::getInstancia().getRed(), PAG::Renderer::getInstancia().getGreen(), PAG::Renderer::getInstancia().getBlue(), 1.0f);
            ImGui::ColorPicker3("Color de fondo", (float*)&clear_color);

            PAG::Renderer::getInstancia().setRed(clear_color.x);
            PAG::Renderer::getInstancia().setGreen(clear_color.y);
            PAG::Renderer::getInstancia().setBlue(clear_color.z);

            PAG::Renderer::getInstancia().pintarColores();

        }
        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    }

    void ControllerImgui::ventanaGestionShaders(){
        ImGui::SetNextWindowPos ( ImVec2 (300, 100), ImGuiCond_Once );

        if( ImGui::Begin("GestorShaders"))
        { // La ventana está desplegada
            ImGui::SetWindowFontScale ( 1.0f );   // Escalamos el texto si fuera necesario
            //selector de color
            ImGui::InputText ("##", &shaderProgramName, ImGuiInputTextFlags_AutoSelectAll );

            if (ImGui::Button("Load")) {
                if (shaderProgramName.empty()) {
                    ControllerMensajes::getInstancia().anadirMensaje("Nombre vacío. No se puede cargar.");
                } else {
                    // Usamos ControllerShaders para cargar el programa
                    try {
                        ControllerShaders::getInstancia().crearPrograma("PAG03",shaderProgramName,shaderProgramName);
                        ControllerShaders::getInstancia().usarPrograma("PAG03");
                        PAG::Renderer::getInstancia().creaModelo();
                    }catch (const std::exception& e) {
                        ControllerMensajes::getInstancia().anadirMensaje(e.what());
                    }

                }
            }
        }
        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    };
}
