#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>



typedef struct Paciente{
    char nombre[30];
    int edad;
    char sintomas[60];
    int prioridad;
    time_t hora;   
}Paciente;

//Función para transformar a mayúsculas
void aMayus(char nombre[30]){
    for(int i = 0; nombre[i]!= '\0'; i++)
        nombre[i] = toupper(nombre[i]);
}

int lower_than(void* data1,void* data2){
    Paciente *paciente1 = (Paciente*) data1;
    Paciente *paciente2 = (Paciente*) data2;
    return (paciente1->hora < paciente2->hora);
}

// Función para limpiar la pantalla
void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestión Hospitalaria");
  puts("========================================");

  puts("1) Registrar paciente");
  puts("2) Asignar prioridad a paciente");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente paciente");
  puts("5) Mostrar pacientes por prioridad");
  puts("6) Salir");
}

//Funcion para registrar nuevos pacientes.
//Todos se almacenan inicialmente en la lista de prioridad baja
void registrar_paciente(List *pacientes) {
    printf("Registrar nuevo paciente\n");
    Paciente *paciente = (Paciente *)malloc(sizeof(Paciente));
    printf("Ingrese el nombre del paciente: ");
    scanf(" %[^\n]s", paciente->nombre);
    aMayus(paciente->nombre); //Todos los nombres se guardan en mayúsculas
    printf("Ingrese la edad del paciente: ");
    scanf("%d", &paciente->edad);
    printf("Ingrese los síntomas del paciente: ");
    scanf(" %[^\n]s", paciente->sintomas);
    paciente->prioridad = 1; // Prioridad inicial es 1
    
    time(&paciente->hora);
    
    list_pushBack(pacientes, paciente);
    printf("Paciente registrado con éxito.\n");
}

//Esta funcion busca y muestra un paciente en una lista dada
//La funcion retorna 0 si no existe el paciente y 1 si es que existe
int mostrarPaciente(List *paciente,char nombre[50]){
    Paciente *paciente_actual = (Paciente *)list_first(paciente);
    do{
        if(paciente_actual == NULL){
            printf("Paciente no encontrado\n");
            return 0;
        }
        if (strcmp(paciente_actual->nombre,nombre) == 0){
            printf("Nombre: %s\n", paciente_actual->nombre);
            printf("Edad: %d\n", paciente_actual->edad);
            printf("Síntomas: %s\n", paciente_actual->sintomas);
            
            char hora_str[10];
            strftime(hora_str, sizeof(hora_str), "%H:%M:%S",localtime(&paciente_actual->hora));
            printf("Hora: %s\n",hora_str);
            printf("Prioridad: %d\n", paciente_actual->prioridad);
            return 1;
        }
        paciente_actual = (Paciente *)list_next(paciente);
        
    }while (paciente_actual);
}


//Esta funcion traslada un paciente desde listaOrigen a listaDestino, actualizando su prioridad
//El paciente se inserta de forma ordenada a la listaDestino y se elimina de listaOrigen
void asignarPrioridad(List *listaOrigen,List *listaDestino,char nombre[50],int nuevaPrioridad){
    Paciente *aux = list_first(listaOrigen);
    while(aux){
        if(strcmp(aux->nombre,nombre)==0){
            aux->prioridad = nuevaPrioridad;
            list_sortedInsert(listaDestino, aux, lower_than);//Se guarda el paciente siguiendo el orden de llegada
            list_popCurrent(listaOrigen);
            if(!mostrarPaciente(listaDestino, nombre))
                printf("Error: vuelva a registrar al paciente\n");
            else
                printf("Prioridad actualizada con éxito.\n");
            break;
        }
        aux = list_next(listaOrigen);
    }
}
//Esta funcion muestra todos los pacientes en una lista dada, ordenados por hora de llegada
void mostrarLista(List* lista){
    Paciente* actual = list_first(lista);
    while(actual){
        char hora_str[10];
        strftime(hora_str, sizeof(hora_str), "%H:%M:%S",localtime(&actual->hora));
        printf("|%35s|%4d|%13d|%8s|\n", actual->nombre,actual->edad,actual->prioridad,hora_str);
        actual = list_next(lista);
    }
}

//Esta funcion muestra los datos del paciente que va a ser atendido
void atenderSiguiente(Paciente* paciente){
    printf("Atendiendo a: %20s\n", paciente -> nombre);
    printf("Edad del paciente: %15d\n", paciente -> edad);
    printf("Síntomas: %24s\n", paciente -> sintomas);
    printf("Prioridad: %23d\n", paciente -> prioridad);
    char hora_str[10];
    strftime(hora_str, sizeof(hora_str), "%H:%M:%S",localtime(&paciente->hora));
    printf("Hora de llegada: %17s\n", hora_str);
}

int main() {
    char opcion;
    List *pacientes = list_create(); // lista para pacientes de prioridad baja (1)
    List *prioridadMedia = list_create(); // lista para pacientes de prioridad media (2)
    List *prioridadAlta = list_create(); // lista para pacientes de prioridad alta (3)
    
    do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);
    
    switch (opcion) {
    case '1':
        registrar_paciente(pacientes);
        break;
    case '2':
        //Se solicita nombre y prioridad actual para encontrar al paciente
        char nombre[50];
        int prioridadActual;
        int nuevaPrioridad;
        printf("Ingrese nombre del paciente:");
        scanf(" %[^\n]s",nombre);
        aMayus(nombre); 
        printf("Ingrese la prioridad actual (1 - 3): ");
        scanf("%d",&prioridadActual);

        //Se busca al paciente y se muestran sus datos de ser encontrados
        if(prioridadActual == 1 && !mostrarPaciente(pacientes,nombre))
            break;
        else if(prioridadActual == 2 && !mostrarPaciente(prioridadMedia,nombre))
            break; 
        else if(prioridadActual == 3 && !mostrarPaciente(prioridadAlta,nombre))
            break;
        else if(prioridadActual!=1&&prioridadActual!=2&&prioridadActual!=3){
            printf("Prioridad Inválida");
            break;
        }
        //Se solicita nueva prioridad y se realiza el cambio
        printf("Ingrese nueva prioridad (1 - 3): ");
        scanf(" %d",&nuevaPrioridad);
        if(prioridadActual == nuevaPrioridad || nuevaPrioridad!=1&&nuevaPrioridad!=2&&nuevaPrioridad!=3)
            printf("Prioridad Inválida.\n");
        else if(prioridadActual==1)
                nuevaPrioridad == 2?asignarPrioridad(pacientes,prioridadMedia,nombre,nuevaPrioridad):
                    asignarPrioridad(pacientes,prioridadAlta,nombre,nuevaPrioridad);
        else if(prioridadActual== 2)
            nuevaPrioridad==1?asignarPrioridad(prioridadMedia,pacientes,nombre,nuevaPrioridad):
                asignarPrioridad(prioridadMedia,prioridadAlta,nombre,nuevaPrioridad);

        else
            nuevaPrioridad==1?asignarPrioridad(prioridadAlta, pacientes, nombre, nuevaPrioridad):
                asignarPrioridad(prioridadAlta,prioridadMedia,nombre,nuevaPrioridad);
        break;
        
    case '3':
        //Se comprueba si hay pacientes en espera
        if(!list_first(prioridadAlta) && !list_first(prioridadMedia)&& !list_first(pacientes)){
            printf("No hay pacientes en espera.\n");
            break;
        }
        //De haber pacientes, se muestran los datos de cada uno, ordenados por prioridad y 
        //por orden de llegada
        printf("|                             Nombre|Edad|    Prioridad|    Hora|\n");
        if(list_first(prioridadAlta))
            mostrarLista(prioridadAlta); 
        if(list_first(prioridadMedia))
            mostrarLista(prioridadMedia); 
        if(list_first(pacientes))
            mostrarLista(pacientes); 
        break;
        
    case '4':
        //Empezando por la prioridad más alta, se comprueba si existen pacientes.
        //Si no existen pacientes se pasa a la siguiente prioridad.
        //Si existen pacientes se muestra el primero de la lista y se eliminan sus datos
        //Si ninguna lista tiene pacientes, se muestra un aviso
        Paciente* next = list_first(prioridadAlta);
        if(!next){
            next = list_first(prioridadMedia);
            if(!next){
                next = list_first(pacientes);
                if(!next){
                    printf("No hay pacientes en espera.\n");
                    break;
                }
                else{
                    atenderSiguiente(next);
                    list_popCurrent(pacientes);
                }
            }
            else{
                atenderSiguiente(next);
                list_popCurrent(prioridadMedia);
            }
}
        else{
            atenderSiguiente(next);
            list_popCurrent(prioridadAlta);
        }
        break;
        
    case '5':
        /*Se solicita una prioridad (1 - 3) y se comprueba si la lista correspondiente tiene pacientes
        si los tiene se muestra toda la lista
        si no, se muestra un aviso*/
        int prioridad;
        printf("Ingrese prioridad (1 - 3): ");
        scanf("%d", &prioridad);
        if(prioridad != 1 && prioridad != 2 && prioridad != 3){
            printf("Prioridad inválida\n");
            break;
        }
        else{
            if(prioridad == 1){
                if(!list_first(pacientes)){
                    printf("No hay pacientes con prioridad baja\n");
                    break;
                }
                printf("Mostrando lista de prioridad baja...\n");
                printf("|                             Nombre|Edad|    Prioridad|    Hora|\n");
                mostrarLista(pacientes);
                }
            else if(prioridad == 2){
                if(!list_first(prioridadMedia)){
                    printf("No hay pacientes con prioridad media\n");
                    break;
                }
                printf("Mostrando lista de prioridad media...\n");
                printf("|                             Nombre|Edad|    Prioridad|    Hora|\n");
                mostrarLista(prioridadMedia);
            }
            else{
                if(!list_first(prioridadAlta)){
                    printf("No hay pacientes con prioridad alta\n");
                    break;
                }
                printf("Mostrando lista de prioridad alta...\n");
                printf("|                             Nombre|Edad|    Prioridad|    Hora|\n");
                mostrarLista(prioridadAlta);
            }
        }
        break;
    case '6':
      puts("Saliendo del sistema de gestión hospitalaria...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();
    
    } while (opcion != '6');

    //Se libera la memoria de las tres listas
    list_clean(pacientes);
    list_clean(prioridadMedia);
    list_clean(prioridadAlta);
    
    return 0;
}
