#ifndef CONTROLADOR_ARCHIVO_CPP
#define CONTROLADOR_ARCHIVO_CPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Grafo.hpp"



using namespace std;

/**
 * @class ControladorArchivo
 * 
 * Clase que controla todos los flujos entre archivos externos
*/
class ControladorArchivo 
{
    private:
        /**
         * puntero a un grafo
        */
        Grafo* grafo;

    public:
        /**
         * Constantes estaticas
        */
        inline static const string ARCHIVO_SERVIDORES = "servidores.csv";
        inline static const string ARCHIVO_CONEXION = "conexiones.csv";
        inline static const string ARCHIVO_RECORRIDOS = "Recorridos/recorridos.csv";
        inline static const string RECORRIDO = "Recorridos/recorrido_";


        /**
         * Constructor
        */
        ControladorArchivo(Grafo& grafo1) 
        {
            grafo = &grafo1;
        };

        /**
         * Agrega un recorrido al archivo recorridos.csv con un  string del recorrido
         * el formato del string es el mismo formato que en el retornado por encontrar camino del metodo grafo
        */
        void agregarRecorrido(string recorrido1)
        {
            vector<int> valores = *(getDatos(recorrido1));
            ofstream fich{ARCHIVO_RECORRIDOS, ios::app}; // Solo se agrega una linea mas
            if (!fich)
            {
                cout << "Error al abrir " << ARCHIVO_RECORRIDOS << endl;
                exit(EXIT_FAILURE);
            }
            string recorrido = "";
            for(int i=2; i<valores.size();i+=2)
            {
                string origen = to_string(valores.at(i-2));
                string destino = to_string(valores.at(i));
                string ponderacion = to_string(valores.at(i-1));
                if(i == 2)
                {
                    recorrido += origen + "," + ponderacion + "," + destino + ",";
                }
                else 
                {
                    recorrido += ponderacion + "," + destino + ",";
                }
                
            }
            recorrido += to_string(valores.back()) + "\n";
            fich << recorrido;
            fich.close();
        };

        /**
         * Retorna el recorrido determino del archivo recorridos.csv, mediante un id determino y un contador determinado
         * @param id id del usuario en sesion
         * @param num el usuario escogio un numero determinado, por lo tanto se buscara
         * @return El recorrido resumido Destino y el costo total, -1 si no existe ningun recorrido en id o con num
        */
        string getRecorridoPorContador(int id, int num)
        {
            ifstream fichero(ARCHIVO_RECORRIDOS);
            if(!fichero.is_open())
            {
                cout << "No se pudo abrir el archivo " + ARCHIVO_RECORRIDOS << endl;
                exit(EXIT_FAILURE);
            }
            string recorridos = "";
            string linea;
            char delimitador = ',';
            getline(fichero, linea);
            int idOrigen;
            int contador = 0;
            while(getline(fichero, linea))
            {
                vector<string> partes = split(linea,delimitador);
                string idO = partes[0];
                string idD = partes[partes.size()-2];
                string total = partes[partes.size()-1];
                
                idOrigen = stoi(idO);
                if(idOrigen == id)
                {
                    contador++;
                    if(contador == num) return linea;
                }
            }
            
            return "-1";
        }


        /**
         * funcion split que separa un string con un delimitador
         * @param str el string a separas
         * @param del delimitador
         * @return vector<string> vector de string
        */
        vector<string> split(string str, char del) 
        {
    
        int posIn = 0;
        int posEncontrada = 0;
        string cortado;
        vector<string> resultado;
        
        while(posEncontrada >= 0)
        {
            posEncontrada = str.find(del, posIn);
            cortado = str.substr(posIn, posEncontrada - posIn);
            posIn = posEncontrada + 1;
            resultado.push_back(cortado);
        }
        
            return resultado;
        };


        /**
         * Retorna un string de todos los recorridos realizados por el usuario con id en parametro
         * @param id id del usuario
         *@return string de todos los recorridos enumerados
        */
        string verTodosRecorridosPorID(int id)
        {
            ifstream fichero(ARCHIVO_RECORRIDOS);
            if(!fichero.is_open())
            {
                cout << "No se pudo abrir el archivo " + ARCHIVO_RECORRIDOS << endl;
                exit(EXIT_FAILURE);
            }
            string recorridos = "";
            string linea;
            char delimitador = ',';
            getline(fichero, linea);
            int idOrigen;
            int contador = 0;
            while(getline(fichero, linea))
            {
                vector<string> partes = split(linea,delimitador);
                string idO = partes[0];
                string idD = partes[partes.size()-2];
                string total = partes[partes.size()-1];
                
                idOrigen = stoi(idO);
                if(idOrigen == id)
                {
                    contador++;
                    recorridos += to_string(contador) + ") Destino: " + idD + " Total: " + total + "\n";
                }
            }
            if(contador == 0) return "No has realizado ninguna transferencia..\n";
            return recorridos;
        };


        /**
         * Lee el archivo conexiones que conecta todos los servidores, en este metodo se conectan los nodos con las aristas y se completa el grafo
         * 
        */
        void leerConexiones()
        {
            ifstream fichero(ARCHIVO_CONEXION);
            if(!fichero.is_open())
            {
                cout << "No se pudo abrir el archivo " + ARCHIVO_CONEXION << endl;
                exit(EXIT_FAILURE);
            }
            string linea;
            getline(fichero, linea);
            int id1;
            int id2;
            int velocidad;
            int distancia;
            char delimitador = ',';

            while(getline(fichero,linea))
            {
                stringstream stream(linea);
                string id1S, id2S, velocidadS, distanciaS;
                getline(stream, id1S, delimitador);
                getline(stream, id2S, delimitador);
                getline(stream, velocidadS,delimitador);
                getline(stream, distanciaS, delimitador);
                
                id1 = stoi(id1S);
                id2 = stoi(id2S);
                velocidad = stoi(velocidadS);
                distancia = stoi(distanciaS);
                bool conectado = grafo->agregarArista(id1, id2, distancia, velocidad); // Se creae la arista
                if(conectado == false)
                {
                    cout << "El id: " + id1S + " o el id: " + id2S + " NO EXISTEN en la lista nodos" << endl;
                }
            }
        };


        /**
         * Lee el archivo servidores.csv en este metodo se agregan todos los nodos al grafo, y se rellena la lista ID clientes
         * 
        */
        void leerServidores()
        {
            ifstream fichero(ARCHIVO_SERVIDORES);
            if(!fichero.is_open())
            {
                cout << "No se pudo abrir el archivo " + ARCHIVO_SERVIDORES << endl;
                exit(EXIT_FAILURE);
            }
            string linea;
            getline(fichero, linea);
            int id;
            char delimitador = ',';

            while(getline(fichero,linea))
            {
                stringstream stream(linea);
                string idS,nombre,tipo;
                
                getline(stream, idS, delimitador);
                getline(stream, nombre,delimitador);
                getline(stream, tipo, delimitador);
                
                id = stoi(idS);
                grafo->agregarNodo(id,nombre,tipo);
            }
        };  


        /**
         * Crea un archivo .dot de un determinado recorrido por parametro
         * @param idOrigen el id origen del destino
         * @param recorrido el recorrido determinado
         * El archivo lo guarda en la carpeta recorridos
        */
        void crearArchivoDOTRecorrido(int idOrigen, string recorrido)
        {
            static int contador = 1;
            vector<int>* valores = getDatos(recorrido);

            string textoDOT = "graph Recorrido_" + to_string(idOrigen) + "_TO_" + to_string(valores->at(valores->size() - 2)) + "\n";
            textoDOT += "{\n";

            for(int i=2; i<valores->size();i+=2)
            {
                string origen = to_string(valores->at(i-2));
                string destino = to_string(valores->at(i));
                string ponder = to_string(valores->at(i-1));
                textoDOT += "\t" + origen + " -- " + destino + " [label=\""  + to_string(valores->at(i-1)) + "\" ,color = \"red\"];\n"; 
            }
            textoDOT += "}\n";

            ofstream archivoDOT;
            archivoDOT.open(RECORRIDO + to_string(contador++) + ".dot");
            archivoDOT << textoDOT << endl;
            archivoDOT.close();
        };

        /**
         * Verifica si un char es un numero
         * @return true si es numero , false lp contrario
        */
        static bool esNumero(char c)
        {
            string str(1,c);
            int numero = 0;

            try
            {
                numero = stoi(str);
                return true;
            }
            catch(const invalid_argument& e)
            {
            }
            catch(const out_of_range& e1)
            {
            }
            return false;
        }

    /**
     * Obtienes un vector de enteros, que representa todos los valores importantes de un recorrido 
     * EJ: Origen: 0 -> costo(5) -> 2 -> costo(4) -> 3 Total: 9
     * valores -> [0,5,2,4,3,9]
     * @return puntero a vector de enteros con los valores importantes
    */
    static vector<int>* getDatos(string recorrido)
    {
        vector<int> *lista = new vector<int>(0);
        bool anterior = false;
        string numero = "";

        for(char c : recorrido)
        {
            if(esNumero(c)) // si es numero lo agrega
            {
                if(anterior)
                {
                    numero = to_string(lista->back()) + string(1,c);
                    lista->at(lista->size() - 1) = stoi(numero);
                }
                else 
                {
                    numero = string(1,c);
                    lista->push_back(stoi(numero));
                    anterior = true;
                }
            }
            else 
            {
                anterior = false;
            }
        }
        return lista;
    };

    /**
     * Genra un archivo .dot con el grafo completo, este grafo depende del archivo a enviar, por lo tanto por parametro se pide el tamaño del archivo
     * @param sizeArchivo tamaño del archivo
     * 
    */
    void generarDOTGrafo(int sizeArchivo)
    {
        bool pass = false;
        vector<Arista*>* listaAristas = this->grafo->listaArista;
        string grafoDOT = "graph Grafo_Size_" + to_string(sizeArchivo) + "\n";
        grafoDOT += "{\n";
        string formatoNodos = "";
        for(Arista* arista : *listaAristas)
        {
            if(pass) // pass, debido a que el siguiente es la misma arista pero en sentido contrario , por eso se pasa la siguiente
            {
                pass = false;
                continue;
            }
            string origen = to_string(arista->getOrigen()->getId());
            string destino = to_string(arista->getDestino()->getId());
            string peso = to_string(arista->getPeso(sizeArchivo));

            grafoDOT += "\t"  + origen + " -- " + destino + " [label = \"" + peso + "\"];\n";
            pass = true;  
        }
        // Se crea una nueva forma rectangulo a los clientes
        for(int idC : *grafo->listaIDClientes)
        {
            formatoNodos += "\t" + to_string(idC) + " [shape = \"square\"];\n";
        }

        grafoDOT += formatoNodos;
        grafoDOT += "}\n";
        ofstream archivoDOT;
        archivoDOT.open(RECORRIDO + "Grafo_size_" + to_string(sizeArchivo) + ".dot");
        archivoDOT << grafoDOT << endl;
        archivoDOT.close();

    };








};



#endif