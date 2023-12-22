
#include <iostream>
#include "Grafo.hpp"
#include "ControladorArchivo.hpp"
#include <fstream>
#include <sstream>
using namespace std;



/**
 * Constantes para los menus
*/
int const SALIR = 4, CONTINUAR = 3;
int const ENVIAR = 1, EXTRA = 2;

   




/**
 * ParseInt(): mientras que el usuario no ponga por teclado un entero, continuara
 * @param string el contexto del número que se pide
 * @return un double
 * 
*/
int parseInt(string contexto)
{
    bool flag = true;
    int numero;
    string entrada = "";
    do
    {
        cout << contexto << endl;
        getline(cin, entrada);
        try
        {
            numero = stoi(entrada);
            flag = false;
        }
        catch(const invalid_argument& e)
        {
            cout << "Opcion no valida" << endl;
        }
        catch(const out_of_range& e1)
        {
            std::cerr << e1.what() << "\n";
        }
    } while (flag);
    return numero;
};


/**
 * logout: Pregunta al usuario si quiere cambiar de sesión o salir del programa
 * @return true, si quiere salir, false lo contrario
*/
bool logout()
{
    cout << "Ha salido del programa" << endl;
    cout << "Desea Continuar en el programa?: " << endl;
    cout << CONTINUAR << ") Continuar en el programa" << endl;
    cout << SALIR << ") Salir del programa " << endl;
    int opc = parseInt("Ingrese una opcion: ");
    switch(opc)
    {
        case CONTINUAR:
            return false;
            break;
        case SALIR:
            cout << "Ha salido del programa" << endl;
            return true;
            break;

        default:    
            cout << "Opcion incorrecta, intente nuevamente" << endl;
            return logout();
            break;
    }
    return false;
};

/**
 * login(): Pregunta al usuario el id del cliente 
 * @return el id del usuario, -1 si la lista de usuarios esta vacia
*/
int login(Grafo& grafo)
{
    if(grafo.IDClienteEmpty()) // Si esta vacia retornar -1
    {
        cout << "No existe Clientes en el sistema..." << endl;
        return -1; // Si la lista ID clientes es vacia entonces retornar -1
    }

    cout << "Bienvenidos a: Transferencias de Archivo" << endl;
    int id = parseInt("Ingrese el ID  del cliente: ");
    while(!grafo.existeIDCliente(id))
    {
        cout << "Error, no se encontro el ID en el sistema clientes" << endl;
        id = parseInt("Ingrese el ID  del cliente: ");
    }
    return id;
};


/**
 * funcion que muestra todos los archivos transferidos por un id determinado
 * @param idOrigen el id del usuario
 * @param arch objeto de ControladorArchivo para realizar la consulta
*/
void verArchivosEnviados(int idOrigen, ControladorArchivo& arch)
{
    cout << "Lista de todos los recorridos hechos por usted: " << endl;
    cout << arch.verTodosRecorridosPorID(idOrigen) << endl;
};


/**
 * función que crea un archivo .dot de un determinada transferencia de un usuario
 * @param id el id del usuario
 * @param arh ControladorArchivo para realizar el archivo
 * 
*/
void crearArchivoDOTRecorrido(int id, ControladorArchivo& arch)
{
    verArchivosEnviados(id, arch);
    vector<int>* valores = nullptr;
    int num = parseInt("Escoge un recorrido (por numero)");
    string recorrido = arch.getRecorridoPorContador(id, num); // Retorna "-1" si no existen recorridos por el usuario
    if(recorrido == "-1") // 
    {
        cout << "No tienes recorridos para hacer o opcion incorrecta" << endl;
        return;
    }
    arch.crearArchivoDOTRecorrido(id, recorrido); // Crea el archivo .dot del recorrido, lo guarda en carpeta recorridos
}


/**
 * Función que crea un archivo .dot del grafo completo, pregunta el tamaño del archivo a enviar, para poder utilizar las ponderaciones
 * No necesita de un usuario especifico
 * @param arch ControladorArchivo
 * 
*/
void crearArchivoDOTGrafo(ControladorArchivo& arch)
{
    cout << "Se generara un archivo .dot de todos los servidores, para ello necesito que me diga el peso de un archivo virtual para generar los tiempos" << endl;
    int sizeArchivo = parseInt("Ingrese el size del archivo: ");
    arch.generarDOTGrafo(sizeArchivo);
    cout << "Archivo generado correctamente, puede utilizar compiladores online para visualizar el grafo :) " << endl;

};

/**
 * Función que realiza las operaciones para mostrar las opciones del menu extra
 * @param idOrigen id del usuario en sesion
 * @param grafo Grafo con la unión de todos los servidores
 * @param arch Controlador Archivo
 * 
*/
void menuExtra(int idOrigen, Grafo& grafo, ControladorArchivo& arch)
{   
    const int OPC1=1,OPC2=2,OPC3=3;
    cout << "Menu Extras " << endl;
    cout << OPC1 << ") Ver todos los archivos enviados por el usuario" << endl;
    cout << OPC2 << ") Crear Archivo .dot de un recorrido especifico" << endl;
    cout << OPC3 << ") Crear Archivo .dot de todo el grafo" << endl;
    int opc = parseInt("Escoge una opcion: ");

    switch (opc)
    {
    case OPC1:
        verArchivosEnviados(idOrigen, arch);
        break;
    
    case OPC2:
        crearArchivoDOTRecorrido(idOrigen, arch);
        break;

    case OPC3:
        crearArchivoDOTGrafo(arch);
        break;
    
    default:
        break;
    }
};


/**
 * Función que realiza la operación de enviar un archivo a un determinado id de destino
 * @param idOrigen id del usuario en sesion
 * @param grafo grafo 
 * @param arch 
 * 
*/
void menuEnviarArchivo(int& idOrigen, Grafo& grafo, ControladorArchivo& arch)
{
    const int SI=1,NO=2; 
    int opc = 0;
    bool continuar = true;
    int sizeArchivo;
    int idDestino;
    vector<pair<Nodo*, int>> distancias;
    vector<int> *valores;
    string recorrido = "";
    
    while(continuar)
    {
        cout << "Ha escogido la opcion enviar archivo a otro servidor" << endl;
        sizeArchivo = parseInt("Ingrese el size del archivo a enviar: ");
        idDestino = parseInt("Ingrese el id del cliente a enviar el archivo: ");

        if(grafo.getIndexPorId(idDestino) == -1) // Verificar si el id existe
        {
            cout << "No se encontro el id en el sistema, no se realizara la transferencia " << endl;
            return;
        }
        if(idDestino == idOrigen) // Si los id son lo mismos cancelar
        {
            cout << "No puedes enviarte a ti mismo la transferencia" << endl;
            return;
        }
        if(!grafo.existeIDCliente(idDestino)) // Si existe en lista de clientes
        {
            cout << "No existe un cliente con ese id, se cancela la transferencia" << endl;
            return;
        }

        distancias = grafo.bellmanFord(idOrigen, sizeArchivo);


        cout << "Archivo enviado correctamente, El camino usado fue: " << endl;
        recorrido = grafo.encontrarCamino(distancias, idOrigen, idDestino);
        arch.agregarRecorrido(recorrido);
        
        cout << recorrido << endl;

        cout << "Desea enviar otro archivo?: " << endl;
        cout << SI << ") Si, enviar otro archivo" << endl;
        cout << NO << ") No, salir del menu enviar archivo" << endl;
        opc = parseInt("Ingrese una opcion: ");
        switch (opc)
        {
        case SI:
            continuar = true;
            break;
        case NO:
            continuar = false;
            break;
        
        default:
            cout << "Opcion incorrecta " << endl;

            break;
        }

    }

};

/**
 * Función de menu principal, realizar todas las operaciones para el menu principal
 * @param id id del usuario en sesión
 * @param grafo grafo
 * @param arch controladorArchivo
 * 
*/
void menuPrincipal(int idOrigen, Grafo& grafo, ControladorArchivo& arch)
{
    while(true)
    {
        cout << "\t\t\t\tMenu Transferencias de Archivos\t\t" << endl;
        cout << ENVIAR << ") Enviar un archivo a un servidor" << endl;
        cout << EXTRA << ") opciones extras del programa" << endl;
        cout << SALIR << ") Salir del programa" << endl;
        int opc = parseInt("Ingrese una opcion: ");

        switch(opc)
        {
            case ENVIAR:
                menuEnviarArchivo(idOrigen, grafo, arch); // Menu enviar archivo
                break;
            
            case EXTRA:
                menuExtra(idOrigen, grafo, arch); // Menu extra
                break;
            
            case SALIR:
                return;
                break;
            
            default:
                cout << "Error al ingresar la opcion, intente nuevamente" << endl;
                menuPrincipal(idOrigen, grafo, arch); // Repetir proceso
                break;
        }
    }
    
};

/**
 * Main
*/
int main()
{

    Grafo grafo;
    ControladorArchivo controlArchivo(grafo);
    controlArchivo.leerServidores();
    controlArchivo.leerConexiones();
    bool flag = false;
    int id;
    while(!flag && (id = login(grafo)) != -1)
    {
        menuPrincipal(id, grafo, controlArchivo);        
        flag = logout();
    }
    return 0;
}