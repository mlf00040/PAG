//
// Created by Suspr on 27/09/2025.
//

#ifndef PAG_CONTROLLERIMGUI_H
#define PAG_CONTROLLERIMGUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "vector"
#include "string"
#include "Renderer.h"
#include "ControllerMensajes.h"

namespace GUI {
    class ControllerImgui {
    private:
        static ControllerImgui *instancia;

        std::string shaderProgramName;

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
    };
}


#endif //PAG_CONTROLLERIMGUI_H
