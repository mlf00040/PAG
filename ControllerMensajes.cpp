//
// Created by Suspr on 12/10/2025.
//

#include "ControllerMensajes.h"

ControllerMensajes *ControllerMensajes::instancia = nullptr;

/**
* Constructor por defecto
*/
ControllerMensajes::ControllerMensajes() {}

/**
 * Destructor
 */
ControllerMensajes::~ControllerMensajes()= default;


ControllerMensajes& ControllerMensajes::getInstancia() {
    if (!instancia) {
        instancia = new ControllerMensajes();
    }
    return *instancia;
}

/**
 * Metodo para añadir mensajes al vector
 * @param texto
 */
void ControllerMensajes::anadirMensaje(const std::string &texto) {
    mensajes.push_back(texto);
}

/**
 * get del vector completo con todos los mensajes
 * @return
 */
const std::vector<std::string>& ControllerMensajes::getMensajes() const {
    return mensajes;
}

void ControllerMensajes::limpiar() {
    mensajes.clear();
}