//
// Created by Suspr on 12/10/2025.
//

#ifndef PAG_CONTROLLERMENSAJES_H
#define PAG_CONTROLLERMENSAJES_H

#include <string>
#include <vector>

class ControllerMensajes {
private:

    static  ControllerMensajes* instancia;
    std::vector<std::string> mensajes;
    ControllerMensajes();

public:

    static ControllerMensajes& getInstancia ();
    ~ControllerMensajes();

    void anadirMensaje(const std::string& texto);
    const std::vector<std::string>& getMensajes() const;
    void limpiar();

};


#endif //PAG_CONTROLLERMENSAJES_H
