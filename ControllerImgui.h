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
#include "Textura.h"

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

        glm::vec3 translacion = glm::vec3(0.0f);
        glm::vec3 rotacionEjes = glm::vec3(0.0f, 1.0f, 0.0f);  // Eje de rotación
        float anguloRotacion = 0.0f;
        glm::vec3 escala = glm::vec3(1.0f);

        std::string nombreMaterialActual="";

        MetodoRenderizado mActual=MetodoRenderizado::ALAMBRE;

        //Para una luz auxiliar y asi poder pasar los parametros
        glm::vec3 cAmbienteAux = {0.0f,0.0f,0.0f};
        //Para puntual y foco
        glm::vec3 posLuzAux = {0.0f,0.0f,0.0f};
        //para direccional
        glm::vec3 direccionLuzAux = {1.0f,0.0f,0.0f};
        //para foco
        float anguloLuzAux=45;
        float expoSuavizadoLuzAux=2;
        std::string nombreLuz="";

        std::string nombreTextura = "";
        int indiceTexturaActual = 0;

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

        void ventanaModificaModelo();

        void ventanaGestionMateriales();

        void ventanaGestionLuces();

        void ventanaGestionTexturas(ImGui::FileBrowser& fileDialog);
    };
}


#endif //PAG_CONTROLLERIMGUI_H
