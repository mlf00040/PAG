//
// Created by Suspr on 27/09/2025.
//

#include "ControllerImgui.h"
#include "ControllerShaders.h"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include "imfilebrowser.h"
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
                        //Creamos el programa y lo usamos
                        ControllerShaders::getInstancia().crearPrograma(shaderProgramName,shaderProgramName,shaderProgramName);
                        ControllerShaders::getInstancia().usarPrograma(shaderProgramName);
                        // mandamos al renderer cual es el id del programa que estamos usando
                        PAG::Renderer::getInstancia().setProgramIdActivo(ControllerShaders::getInstancia().getProgramId(shaderProgramName));
                        // creamos el modelo
                        //std::string Prueba = "sfsssfs";
                        //PAG::Renderer::getInstancia().creaModelo(Prueba);
                    }catch (const std::exception& e) {
                        ControllerMensajes::getInstancia().anadirMensaje(e.what());
                    }

                }
            }
        }
        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    };

    void ControllerImgui::pulsarMouse(bool sePulsa, double x, double y) {
        pulsado=sePulsa;
        if(pulsado){
            lastx=x;
            lasty=y;
        }
    }

    void ControllerImgui::ventanaMovimientosCamara(){

        ImGui::SetNextWindowPos ( ImVec2 (300, 100), ImGuiCond_Once );

        if( ImGui::Begin("Movimientos Camara"))
        { // La ventana está desplegada
            ImGui::SetWindowFontScale ( 1.0f );   // Escalamos el texto si fuera necesario
            //selector de color
            const char* items[] = {
                        "NO", "PAN", "TILT", "DOLLY", "CRANE", "ORBIT", "ZOOM"
            };
            int itemCurrent = static_cast<int>(movimientoCamaraActual);

            if (ImGui::Combo("##movement", &itemCurrent, items, IM_ARRAYSIZE(items))) {
                movimientoCamaraActual = static_cast<MovCamara>(itemCurrent);
            }

            if (ImGui::Button("Reset Camera")) {
                PAG::Renderer::getInstancia().getCamara().reset();
                ControllerMensajes::getInstancia().anadirMensaje("Cámara reseteada.");
            }
            ImGui::Text("Sensibilidad: ");
            ImGui::Text(std::to_string(sensibilidad).c_str());

            if (ImGui::Button("+ 0.1")) {
                sensibilidad+=0.1;
            }
            if (ImGui::Button("- 0.1")) {
                sensibilidad-=0.1;
            }
        }
        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    }

    void ControllerImgui::procesaMovimiento(double x, double y) {

        if(!pulsado || movimientoCamaraActual==MovCamara::NO){
            return;
        }

        float dx = static_cast<float>(x - lastx);
        float dy = static_cast<float>(y - lasty);
        lastx = x;
        lasty = y;

        switch (movimientoCamaraActual) {
            case MovCamara::NO:
                break;

            case MovCamara::PAN:
                PAG::Renderer::getInstancia().getCamara().pan(dx * sensibilidad);
                break;

            case MovCamara::TILT:
                PAG::Renderer::getInstancia().getCamara().tilt(-dy * sensibilidad);
                break;

            case MovCamara::DOLLY:
                PAG::Renderer::getInstancia().getCamara().dolly(dx * sensibilidad * 0.1,dy * sensibilidad * 0.1);
                break;

            case MovCamara::CRANE:
                PAG::Renderer::getInstancia().getCamara().crane(dy * sensibilidad * 0.1);
                break;

            case MovCamara::ORBIT:
                PAG::Renderer::getInstancia().getCamara().orbit(dx * sensibilidad,-dy * sensibilidad);
                break;

            case MovCamara::ZOOM:
                PAG::Renderer::getInstancia().getCamara().zoom(-dy * sensibilidad);
                break;

        }
    }

    void ControllerImgui::ventanaCargaModelo(ImGui::FileBrowser &fileDialog, std::string &ruta) {
        ImGui::SetNextWindowPos ( ImVec2 (500, 100), ImGuiCond_Once );

        if( ImGui::Begin("Carga de Modelos")) { // La ventana está desplegada

            ImGui::SetWindowFontScale(1.0f);   // Escalamos el texto si fuera necesario

            if (ImGui::Button("open file dialog")) {
            fileDialog.Open();
            }

            fileDialog.Display();

            if(fileDialog.HasSelected())
            {
                ControllerMensajes::getInstancia().anadirMensaje(fileDialog.GetSelected().string());
                ruta = fileDialog.GetSelected().string();
                fileDialog.ClearSelected();
            }

            if (ImGui::Button("Añadir modelo")) {
                PAG::Renderer::getInstancia().creaModelo(ruta);
            }

            ImGui::SameLine();

            const auto& modelos = PAG::Renderer::getInstancia().getModelos();
            if (modelos.empty()) {
                ImGui::TextDisabled("No hay modelos cargados");
            } else {

                //generar los nombres para el selector
                std::vector<std::string> nombres;
                for (size_t i = 0; i < modelos.size(); ++i) {
                    nombres.push_back("Modelo " + std::to_string(i));
                }

                //convertir a array de C-string
                std::vector<const char*> items;
                for (const auto& nombre : nombres) {
                    items.push_back(nombre.c_str());
                }

                ImGui::Combo("Selecciona modelo", &indiceModeloActual, items.data(), (int)items.size());

                if (ImGui::Button("Borrar modelo seleccionado")) {
                    if (indiceModeloActual >= 0 && indiceModeloActual < (int)modelos.size()) {
                        PAG::Renderer::getInstancia().borrarModelo(indiceModeloActual);
                        //reiniciar el indice en caso de que se borre el ultimo
                        if (indiceModeloActual >= (int)PAG::Renderer::getInstancia().getModelos().size()) {
                            indiceModeloActual = (int)PAG::Renderer::getInstancia().getModelos().size() - 1;
                        }
                        ControllerMensajes::getInstancia().anadirMensaje("Modelo eliminado: " + std::to_string(indiceModeloActual));
                    }
                }
            }


        }
        ImGui::End ();
    }
}
