#ifndef NODO_HPP
#define NODO_HPP

#include <string>
using namespace std;
/**
 * @class Nodo 
 * Clase nodo donde tiene los atributos necesarios para representar los servidores
*/
class Nodo
{
    private:
        /**
         * Id del servidor
        */
        int id;
        /**
         * Nombre del servidor
        */
        string nombre;
    public:
        /**
         * Constructor de la clase 
         * @param id id del servidor
         * @param nombre nombre del servidor
        */
        Nodo(int id, string nombre) : id(id), nombre(nombre) {};
        /**
         * Destructor
        */
        ~Nodo(){};
        /**
         * getter
        */
        int getId(){return id;};
        /**
         * getter
        */
        string getNombre(){return nombre;};

};


#endif 