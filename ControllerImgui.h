//
// Created by Suspr on 27/09/2025.
//

#ifndef PAG_CONTROLLERIMGUI_H
#define PAG_CONTROLLERIMGUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include "imfilebrowser.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "vector"
#include "string"
#include "Renderer.h"
#include "ControllerMensajes.h"
#include "Camara.h"

namespace GUI {
    class ControllerImgui {
    private:
        static ControllerImgui *instancia;

        std::string shaderProgramName;

        MovCamara movimientoCamaraActual = MovCamara::NO;
        bool pulsado = false;
        double lastx =0;
        double lasty =0;
        float sensibilidad = 0.3;

        int indiceModeloActual=0;


        ControllerImgui();

    public:
        static ControllerImgui& getInstancia();

        virtual ~ControllerImgui();

        void inicializa(GLFWwindow* window);

        void liberaRecursos();

        void refrescoFrames();

        void dibujaControladores();

        void ventanaMensajes (std::vector<std::string> &mensajes);

        void ventanaSelecColor();

        void ventanaGestionShaders();

        void pulsarMouse(bool sePulsa, double x, double y);

        void ventanaMovimientosCamara();

        void procesaMovimiento(double x, double y);

        void ventanaCargaModelo(ImGui::FileBrowser &fileDialog, std::string &ruta);
    };
}


#endif //PAG_CONTROLLERIMGUI_H
