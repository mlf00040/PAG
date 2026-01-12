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

#include <filesystem>

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
            ImGui::SetWindowFontScale ( 1.0f );
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
            ImGui::SetWindowFontScale ( 1.0f );
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
            ImGui::SetWindowFontScale ( 1.0f );
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

                        //hacemos la carga de los indices de las subrutinas
                        PAG::Renderer::getInstancia().cargarIndicesSubrutinas();


                        // creamos el modelo
                        //std::string Prueba = "sfsssfs";
                        //PAG::Renderer::getInstancia().creaModelo(Prueba);
                        ControllerMensajes::getInstancia().anadirMensaje("Programa cargado con exito");
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
            ImGui::SetWindowFontScale ( 1.0f );
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

        if( ImGui::Begin("Carga de Modelos")) {

            ImGui::SetWindowFontScale(1.0f);

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

            ImGui::Text("Modelo cargado: ");
            //uso del filysystem unicamente para que muestre el nombre dle objeto y no la ruta completa (estetica :D )
            std::filesystem::path p(ruta);
            std::filesystem::path nombre_archivo_path = p.filename();
            const std::string nombre_archivo_str = nombre_archivo_path.string();
            ImGui::Text("%s", nombre_archivo_str.c_str());

            if (ImGui::Button("Añadir modelo")) {
                PAG::Renderer::getInstancia().creaModelo(ruta);
            }

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

                ImGui::Combo(" ", &indiceModeloActual, items.data(), (int)items.size());

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

    void ControllerImgui::ventanaModificaModelo() {
        ImGui::SetNextWindowPos ( ImVec2 (600, 200), ImGuiCond_Once );

        if( ImGui::Begin("Modifica Modelo")) {

            ImGui::SetWindowFontScale(1.0f);

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


                //Obtener el modelo actual
                if (indiceModeloActual >= 0 && indiceModeloActual < (int)modelos.size()) {
                    auto& modelo = modelos[indiceModeloActual];

                    ImGui::Text("Seleccion Modelo");
                    ImGui::Combo(" ", &indiceModeloActual, items.data(), (int)items.size());
                    ImGui::Text("");

                    //Modo de visualizacion
                    ImGui::Text("Modo visualizacion");
                    const char* modos[] = { "ALAMBRE", "SOLIDO","TEXTURA","NORMALMAPPING" };
                    int modoActual = static_cast<int>(modelo->getMRenderizado());

                    if (ImGui::Combo("##modo_visualizacion", &modoActual, modos, IM_ARRAYSIZE(modos))) {
                        modelo->setMRenderizado(static_cast<MetodoRenderizado>(modoActual));
                    }

                    ImGui::Text("");

                    ImGui::Text("Asignacion Material");

                    //selecciona material
                    if (PAG::Renderer::getInstancia().getMateriales().empty()) {
                        ImGui::TextDisabled("No hay materiales cargados");
                    } else {
                        //Obtenemos la lista de materiales disponibles
                        std::vector<std::string> nombresMateriales;
                        for (const auto &[nombre, _]: PAG::Renderer::getInstancia().getMateriales()) {
                            nombresMateriales.push_back(nombre);
                        }

                        std::vector<const char *> itemsMats;
                        for (const auto &nombreMaterial: nombresMateriales) {
                            itemsMats.push_back(nombreMaterial.c_str());
                        }

                        static int indiceMaterial = 0;
                        ImGui::Combo("##material", &indiceMaterial, itemsMats.data(), (int) itemsMats.size());

                        //boton para aplicar el material al modelo
                        if (ImGui::Button("Asignar material")) {
                            std::string nombreMat = nombresMateriales[indiceMaterial];
                            modelo->setNombreMaterial(nombreMat);
                            ControllerMensajes::getInstancia().anadirMensaje(
                                    "Modelo " + std::to_string(indiceModeloActual) +
                                    "Material asignado: " + nombreMat);
                        }

                    }

                    ImGui::Text("");
                    ImGui::Text("Transformaciones del modelo");
                    ImGui::Text("");

                    ImGui::Text("Traslación:");
                    ImGui::PushID("traslacion");
                    if (ImGui::DragFloat3("##traslacion", &translacion.x, 0.1f, -10.0f, 10.0f)) {
                        modelo->trasladar(translacion);
                        translacion = glm::vec3(0.0f);
                    }
                    ImGui::PopID();
                    ImGui::Text("");

                    ImGui::Text("Rotación:");
                    ImGui::PushID("rotacion");
                    ImGui::DragFloat("Ángulo (grados)", &anguloRotacion, 1.0f, -360.0f, 360.0f);
                    ImGui::DragFloat3("Eje", &rotacionEjes.x, 0.1f, -1.0f, 1.0f);
                    if (ImGui::Button("Aplicar rotación")) {
                        if (glm::length(rotacionEjes) > 0.0f) {
                            modelo->rotar(anguloRotacion, rotacionEjes);
                        }
                    }
                    ImGui::PopID();
                    ImGui::Text("");

                    ImGui::Text("Escalado:");
                    ImGui::PushID("escala");
                    if (ImGui::DragFloat3("##escala", &escala.x, 0.05f, 0.1f, 10.0f)) {
                        modelo->escalar(escala);
                        escala = glm::vec3(1.0f);
                    }
                    ImGui::PopID();
                }
            }
        }
        ImGui::End ();

    }

    void ControllerImgui::ventanaGestionMateriales() {
        ImGui::SetNextWindowPos ( ImVec2 (700, 400), ImGuiCond_Once );

        if( ImGui::Begin("Gestion MAteriales")) {

            ImGui::SetWindowFontScale(1.0f);

            ImGui::InputText ("##", &nombreMaterialActual, ImGuiInputTextFlags_AutoSelectAll );

            ImGui::TextWrapped("Cuando se cree un material se tiene que aplicar al modelo en la ventana de los modelos");

            if (ImGui::Button("Crear material")) {
                if (nombreMaterialActual.empty()) {
                    ControllerMensajes::getInstancia().anadirMensaje("Nombre vacío. No se puede cargar.");
                } else {
                    const auto &mats = PAG::Renderer::getInstancia().getMateriales();
                    if (mats.find(nombreMaterialActual) != mats.end()) {
                        ControllerMensajes::getInstancia().anadirMensaje(
                                "Ya existe un material llamado '" + nombreMaterialActual);
                    } else {
                        Material nuevoMat;
                        PAG::Renderer::getInstancia().addMaterial(nombreMaterialActual, nuevoMat);
                        ControllerMensajes::getInstancia().anadirMensaje("Material creado: " + nombreMaterialActual);
                        nombreMaterialActual.clear();
                    }
                }
            }

            ImGui::Text("");

            if (PAG::Renderer::getInstancia().getMateriales().empty()) {
                ImGui::TextDisabled("No hay materiales creados");
            } else {
                //selector de materiales
                std::vector<std::string> nombresMateriales;
                for (const auto &[nombre, _]: PAG::Renderer::getInstancia().getMateriales()) {
                    nombresMateriales.push_back(nombre);
                }

                std::vector<const char *> itemsMats;
                for (const auto &nombre: nombresMateriales) {
                    itemsMats.push_back(nombre.c_str());
                }

                //Un indice para facilitar las cosas a la hora de mostrar todos los materiales
                static int indiceMaterial = 0;
                if (indiceMaterial >= (int) nombresMateriales.size()) {
                    indiceMaterial = 0;
                }

                ImGui::Text("Editar material:");
                ImGui::SameLine();
                ImGui::Combo("##material", &indiceMaterial, itemsMats.data(), (int) itemsMats.size());

                std::string nombreSeleccionado = nombresMateriales[indiceMaterial];
                //se castea para que se pueda meter bien en el mapa
                Material &material = const_cast<Material &>(PAG::Renderer::getInstancia().getMateriales().at(nombreSeleccionado));

                ImGui::Text("");

                ImGui::Text("Propiedades del material '%s':", nombreSeleccionado.c_str());

                //Color ambiente
                ImGui::ColorEdit3("Ambiente", (float*)&material.getColorAmbiente());

                //Color difuso
                ImGui::ColorEdit3("Difuso", (float*)&material.getColorDifuso());

                //Color especular
                ImGui::ColorEdit3("Especular", (float*)&material.getColorEspecular());

                //Exponente especular
                float exponente = material.getExponente();
                if (ImGui::SliderFloat("Exponente especular", &exponente, 1.0f, 200.0f, "%.1f")) {
                    const_cast<Material &>(PAG::Renderer::getInstancia().getMateriales().at(nombreSeleccionado)).setExponente(exponente);
                }

                //boton para borrar el material
                if (ImGui::Button("Borrar material")) {
                    //Verificamos si el material está en uso en algun modelo
                    bool uso = false;
                    const auto& modelos = PAG::Renderer::getInstancia().getModelos();
                    for (const auto& modelo : modelos) {
                        if (modelo->getNombreMaterial() == nombreSeleccionado) {
                            uso = true;
                            break;
                        }
                    }

                    if (uso) {
                        ControllerMensajes::getInstancia().anadirMensaje("No se puede borrar. El material en uso: " + nombreSeleccionado);
                    } else {

                        //borramos el material
                        PAG::Renderer::getInstancia().borrarMaterial(nombreSeleccionado);
                        ControllerMensajes::getInstancia().anadirMensaje("Material borrado: " + nombreSeleccionado);

                        //Opcional resetear el índice
                        if (!PAG::Renderer::getInstancia().getMateriales().empty()) {
                            indiceMaterial = std::min(indiceMaterial, (int)PAG::Renderer::getInstancia().getMateriales().size() - 1);
                        }
                    }

                }
            }

        }
        ImGui::End ();

    }

    void ControllerImgui::ventanaGestionLuces() {
        ImGui::SetNextWindowPos ( ImVec2 (800, 500), ImGuiCond_Once );

        if( ImGui::Begin("Gestion Luces")) {

            ImGui::SetWindowFontScale(1.0f);

            ImGui::Text("Crear nueva luz:");
            ImGui::InputText("##nombreLuz", &nombreLuz, ImGuiInputTextFlags_AutoSelectAll);

            const char *tipos[] = {"AMBIENTE", "PUNTUAL", "DIRECCIONAL", "FOCO"};
            static int tipoLuzSeleccionado = 0;

            ImGui::Combo("Tipo", &tipoLuzSeleccionado, tipos, IM_ARRAYSIZE(tipos));

            if (ImGui::Button("Crear luz")) {
                if (nombreLuz.empty()) {
                    ControllerMensajes::getInstancia().anadirMensaje("Nombre vacío. No se puede crear.");
                } else {

                    //comprobar que no haya otra luz con ese nombre
                    bool existe = false;

                    for (size_t i = 0; i < PAG::Renderer::getInstancia().getLuces().size(); i++) {
                        if (PAG::Renderer::getInstancia().getLuces()[i]->getNombreLuz() == nombreLuz) {
                            existe = true;
                        }
                    }
                    if (!existe) {
                        //crear una nueva luz
                        std::unique_ptr<Luz> nuevaLuz(new Luz(static_cast<tipoLuz>(tipoLuzSeleccionado)));
                        PAG::Renderer::getInstancia().addLuz(nombreLuz, std::move(nuevaLuz));
                        nombreLuz.clear();
                    }else{
                        ControllerMensajes::getInstancia().anadirMensaje("Ya existe una luz con ese nombre, no se puede crear.");
                    }

                }
            }
            ImGui::Text("");

            const auto &luces= PAG::Renderer::getInstancia().getLuces();
            if(luces.empty()){
                ImGui::Text("No hay luces");
            }else{
                //el combo
                std::vector<std::string> nombres;
                for (const auto& luz : luces) {
                    nombres.push_back(luz->getNombreLuz());
                }

                std::vector<const char*> items;
                for (const auto& n : nombres) {
                    items.push_back(n.c_str());
                }

                // Selector de luz
                static int indiceLuz = 0;
                if (indiceLuz >= (int)nombres.size()) {
                    indiceLuz = 0;
                }

                ImGui::Text("Editar luz:");
                ImGui::SameLine();
                ImGui::Combo("##luz_seleccionada", &indiceLuz, items.data(), (int)items.size());

                std::string nombreSeleccionado = nombres[indiceLuz];
                Luz& luz = *luces[indiceLuz]; // Acceso directo al objeto

                ImGui::Text("");
                ImGui::Text("Propiedades de la luz '%s':", nombreSeleccionado.c_str());

                //Color ambiente que es comun a todas las luces
                glm::vec3 amb = luz.getIAmbiente();
                if (ImGui::ColorEdit3("Color Ambiente", &amb.x)) {
                    luz.setIAmbiente(amb);
                }

                //Color difuso y especular para todas las qu eno son ambiente
                if (luz.getTLuz() != tipoLuz::AMBIENTE) {
                    glm::vec3 dif = luz.getIDifusa();
                    if (ImGui::ColorEdit3("Color Difuso", &dif.x)) {
                        luz.setIDifusa(dif);
                    }

                    glm::vec3 esp = luz.getIEspecular();
                    if (ImGui::ColorEdit3("Color Especular", &esp.x)) {
                        luz.setIEspecular(esp);
                    }
                }

                if (luz.getTLuz() == tipoLuz::PUNTUAL || luz.getTLuz() == tipoLuz::FOCO) {
                    glm::vec3 pos = luz.getPos();
                    if (ImGui::DragFloat3("Posición", &pos.x, 0.1f)) {
                        luz.setPos(pos);
                    }
                }

                if (luz.getTLuz() == tipoLuz::DIRECCIONAL || luz.getTLuz() == tipoLuz::FOCO) {
                    glm::vec3 dir = luz.getDireccion();
                    if (ImGui::DragFloat3("Dirección", &dir.x, 0.1f)) {
                        luz.setDireccion(dir);
                    }
                }

                if (luz.getTLuz() == tipoLuz::FOCO) {
                    float ang = luz.getAngulo();
                    if (ImGui::SliderFloat("Ángulo apertura", &ang, 5.0f, 90.0f)) {
                        luz.setAngulo(ang);
                    }
                    float expo = luz.getExpoSuavizado();
                    if (ImGui::SliderFloat("Exponente suavizado", &expo, 1.0f, 10.0f)) {
                        luz.setExpoSuavizado(expo);
                    }
                }

                //boton para borrar la luz
                if (ImGui::Button("Borrar luz")) {

                    PAG::Renderer::getInstancia().borrarLuz(nombreSeleccionado);

                    if (!PAG::Renderer::getInstancia().getLuces().empty()) {
                        indiceLuz = std::min(indiceLuz, (int)PAG::Renderer::getInstancia().getLuces().size() - 1);
                    }

                }
            }

            ImGui::End();
        }

    }

    void ControllerImgui::ventanaGestionTexturas(ImGui::FileBrowser& fileDialog) {
        ImGui::SetNextWindowPos(ImVec2(900, 500), ImGuiCond_Once);

        if (ImGui::Begin("Gestión Texturas")) {
            ImGui::SetWindowFontScale(1.0f);

            //cargar una nueva textura
            if (ImGui::Button("Cargar PNG")) {
                fileDialog.Open();
            }
            fileDialog.Display();

            if (fileDialog.HasSelected()) {
                std::filesystem::path path = fileDialog.GetSelected();
                std::string nombre = path.filename().stem().string();
                std::string ruta = path.string();

                auto nuevaTextura = std::make_unique<Textura>(nombre);
                nuevaTextura->cargar(ruta);
                PAG::Renderer::getInstancia().addTextura(std::move(nuevaTextura));

                fileDialog.ClearSelected();
            }

            ImGui::Separator();

            // --- Lista de texturas cargadas ---
            const auto& texturas = PAG::Renderer::getInstancia().getTexturas();
            if (texturas.empty()) {
                ImGui::TextDisabled("No hay texturas cargadas");
            } else {
                for (size_t i = 0; i < texturas.size(); ++i) {
                    ImGui::PushID(static_cast<int>(i));
                    auto& tex = *texturas[i];

                    ImGui::BulletText("%s", tex.getNombre().c_str());
                    ImGui::SameLine();
                    if (ImGui::Button("Borrar")) {
                        PAG::Renderer::getInstancia().borrarTextura(i);
                        ImGui::PopID();
                        continue;
                    }

                    ImGui::PopID();
                }
            }

            ImGui::Separator();

            // --- Asignar textura a modelo ---
            const auto& modelos = PAG::Renderer::getInstancia().getModelos();
            if (modelos.empty()) {
                ImGui::TextDisabled("No hay modelos");
            } else {
                ImGui::Text("Asignar a modelo:");

                // Selector de modelo
                std::vector<std::string> nombresModelos;
                for (size_t i = 0; i < modelos.size(); ++i) {
                    nombresModelos.push_back("Modelo " + std::to_string(i));
                }
                std::vector<const char*> items;
                for (const auto& n : nombresModelos) items.push_back(n.c_str());

                static int idxModelo = 0;
                ImGui::Combo("Modelo", &idxModelo, items.data(), (int)items.size());

                if (idxModelo >= 0 && idxModelo < (int)modelos.size()) {
                    auto& modelo = modelos[idxModelo];

                    // Selector de textura
                    std::vector<std::string> nombresTexturas;
                    for (const auto& t : texturas) {
                        nombresTexturas.push_back(t->getNombre());
                    }

                    if (nombresTexturas.empty()) {
                        ImGui::TextDisabled("No hay texturas para asignar");
                    } else {
                        std::vector<const char*> texItems;
                        for (const auto& n : nombresTexturas) texItems.push_back(n.c_str());

                        static int idxTex = 0;
                        ImGui::Combo("Textura", &idxTex, texItems.data(), (int)texItems.size());

                        if (ImGui::Button("Asignar textura")) {
                            if(!modelo.get()->getTextura()) {
                                std::string nombreTex = nombresTexturas[idxTex];
                                Textura *tex = PAG::Renderer::getInstancia().getTexturaPorNombre(nombreTex);
                                modelo->setTextura(tex);
                                ControllerMensajes::getInstancia().anadirMensaje("Textura asignada a Modelo " + std::to_string(idxModelo) + ": " + nombreTex);
                            }else{
                                ControllerMensajes::getInstancia().anadirMensaje("Ese modelo ya cuenta con una textura");
                            }
                        }
                    }
                }
            }
        }
        ImGui::End();
    }

    void ControllerImgui::ventanaGestionTexturasNormal(ImGui::FileBrowser& fileDialog) {
        ImGui::SetNextWindowPos(ImVec2(900, 500), ImGuiCond_Once);

        if (ImGui::Begin("Gestión Texturas Normal Mapping")) {
            ImGui::SetWindowFontScale(1.0f);

            //cargar una nueva textura
            if (ImGui::Button("Cargar textura PNG")) {
                fileDialog.Open();
            }
            fileDialog.Display();

            if (fileDialog.HasSelected()) {
                std::filesystem::path path = fileDialog.GetSelected();
                std::string nombre = path.filename().stem().string();
                std::string ruta = path.string();

                auto nuevaTexturaNormal = std::make_unique<Textura>(nombre);
                nuevaTexturaNormal->cargar(ruta);
                PAG::Renderer::getInstancia().addTexturaNormal(std::move(nuevaTexturaNormal));

                fileDialog.ClearSelected();
            }

            ImGui::Separator();

            // --- Lista de texturasNormales cargadas ---
            const auto& texturasNormales = PAG::Renderer::getInstancia().getTexturasNormal();
            if (texturasNormales.empty()) {
                ImGui::TextDisabled("No hay texturas de Normal Mapping cargadas");
            } else {
                for (size_t i = 0; i < texturasNormales.size(); ++i) {
                    ImGui::PushID(static_cast<int>(i));
                    auto& tex = *texturasNormales[i];

                    ImGui::BulletText("%s", tex.getNombre().c_str());
                    ImGui::SameLine();
                    if (ImGui::Button("Borrar")) {
                        PAG::Renderer::getInstancia().borrarTexturaNormal(i);
                        ImGui::PopID();
                        continue;
                    }

                    ImGui::PopID();
                }
            }

            ImGui::Separator();

            // --- Asignar textura a modelo ---
            const auto& modelos = PAG::Renderer::getInstancia().getModelos();
            if (modelos.empty()) {
                ImGui::TextDisabled("No hay modelos");
            } else {
                ImGui::Text("Asignar a modelo:");

                // Selector de modelo
                std::vector<std::string> nombresModelos;
                for (size_t i = 0; i < modelos.size(); ++i) {
                    nombresModelos.push_back("Modelo " + std::to_string(i));
                }
                std::vector<const char*> items;
                for (const auto& n : nombresModelos) items.push_back(n.c_str());

                static int idxModelo = 0;
                ImGui::Combo("Modelo", &idxModelo, items.data(), (int)items.size());

                if (idxModelo >= 0 && idxModelo < (int)modelos.size()) {
                    auto& modelo = modelos[idxModelo];

                    // Selector de textura
                    std::vector<std::string> nombresTexturas;
                    for (const auto& t : texturasNormales) {
                        nombresTexturas.push_back(t->getNombre());
                    }

                    if (nombresTexturas.empty()) {
                        ImGui::TextDisabled("No hay texturasNormales para asignar");
                    } else {
                        std::vector<const char*> texItems;
                        for (const auto& n : nombresTexturas) texItems.push_back(n.c_str());

                        static int idxTex = 0;
                        ImGui::Combo("Textura", &idxTex, texItems.data(), (int)texItems.size());

                        if (ImGui::Button("Asignar textura Normal Mapping")) {
                            if(!modelo.get()->getTexturaMapaNormal()) {
                                std::string nombreTex = nombresTexturas[idxTex];
                                Textura *tex = PAG::Renderer::getInstancia().getTexturaPorNombreNormal(nombreTex);
                                modelo->setTexturaMapaNormal(tex);
                                ControllerMensajes::getInstancia().anadirMensaje("Textura Normal Mapping asignada a Modelo " + std::to_string(idxModelo) + ": " + nombreTex);
                            }else{
                                ControllerMensajes::getInstancia().anadirMensaje("Ese modelo ya cuenta con una textura de Normal Mapping");
                            }
                        }
                    }
                }
            }
        }
        ImGui::End();
    }

    void ControllerImgui::ventanaSombras() {

        ImGui::SetNextWindowPos(ImVec2(1000, 600), ImGuiCond_Once);

        if (ImGui::Begin("Control de Sombras")) {
            ImGui::SetWindowFontScale(1.0f);

            bool sombrasActivo = PAG::Renderer::getInstancia().isSombras();

            if (ImGui::Checkbox("Activar Sombras", &sombrasActivo)) {

                PAG::Renderer::getInstancia().setSombras(sombrasActivo);

                if(sombrasActivo){
                    ControllerMensajes::getInstancia().anadirMensaje("Sombras ACTIVADAS");
                }else{
                    ControllerMensajes::getInstancia().anadirMensaje("Sombras DESACTIVADAS");
                }

            }
            ImGui::TextWrapped("Cuando está activado, se generan los mapas de sombra "
                               "para luces direccionales y focos.");

            //estadoActual
            if(PAG::Renderer::getInstancia().isSombras()){
                ImGui::Text("Estado actual: %s","Activado");
            }else{
                ImGui::Text("Estado actual: %s","Desactivado");
            }
        }
        ImGui::End();
        
    }

}
