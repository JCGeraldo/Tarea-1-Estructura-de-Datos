#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct Paciente{
    char nombre[60];
    int edad;
    char sintomas[60];
    int prioridad;
    time_t hora;   
}Paciente;

//Función para transformar a mayúsculas
void aMayus(char nombre[60]){
    for(int i = 0; nombre[i]!= '\0'; i++)
        nombre[i] = toupper(nombre[i]);
}

//Función para limpiar caracteres residuales.
void limpiarBuffer(){
    while(getchar() != '\n');
    return;
}

//Esta funcion compara las horas de ingreso
//Se utiliza para insertar ordenado con la funcion list_sortedInsert
int lower_than(void* data1,void* data2){
    Paciente *paciente1 = (Paciente*) data1;
    Paciente *paciente2 = (Paciente*) data2;
    return (paciente1->hora < paciente2->hora);
}

// Función para limpiar la pantalla
void limpiarPantalla() { system("clear");
    puts("========================================");
    puts("     Sistema de Gestión Hospitalaria");
    puts("========================================");
}

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  

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
    limpiarPantalla();
    printf("Registrar nuevo paciente\n");
    Paciente *paciente = (Paciente *)malloc(sizeof(Paciente));
    
    printf("Ingrese el nombre del paciente: ");
    scanf(" %59[^\n]s", paciente->nombre);
    //Se valida que el nombre no esté vacío
    if (strlen(paciente->nombre) == 0){
        printf("Nombre no válido. Volviendo al menú principal...\n");
        return;
    }
    aMayus(paciente->nombre); //Todos los nombres se guardan en mayúsculas
    limpiarBuffer();
    
    printf("Ingrese la edad del paciente: ");
    //Al leer la edad se comprueba si el usuario ingresó un número.
    //Si no lo hizo se cancela el registro y se vuelve al menú principal
    if(!scanf("%d", &paciente->edad ) ||paciente->edad <0|| paciente->edad > 1000){
        printf("Edad no válida. Volviendo al menu principal...\n");
        limpiarBuffer();
        free(paciente);
        return;
    }
    printf("Ingrese los síntomas del paciente: ");
    scanf(" %59[^\n]s", paciente->sintomas);
    limpiarBuffer();
    paciente->prioridad = 1; // Prioridad inicial es 1
    
    time(&paciente->hora);//Se guarda la hora en el momento del registro
    
    list_pushBack(pacientes, paciente);//Se almacenan los datos del paciente en la lista de prioridad baja
    printf("Paciente registrado con éxito.\n");
}

//esta función busca un paciente por su nombre en una lista. Si lo encuentra retorna los datos del paciente
Paciente *buscarPaciente(List *pacientes,char nombre[60]){
    Paciente *paciente_actual = (Paciente *)list_first(pacientes);
    do{
        if(paciente_actual == NULL)
            return NULL;
        if (strcmp(paciente_actual->nombre,nombre) == 0)
            return paciente_actual;
        paciente_actual = (Paciente *)list_next(pacientes);
    }while (paciente_actual);
    
    return NULL;
}

//Esta funcion recibe los datos de un único paciente y los muestra
void mostrarPaciente(Paciente *paciente){
    printf("Nombre:    %s\n", paciente->nombre);
    printf("Edad:      %d\n", paciente->edad);
    printf("Síntomas:  %s\n", paciente->sintomas);

    char hora_str[10];
    strftime(hora_str, sizeof(hora_str), "%H:%M:%S",localtime(&paciente->hora));
    
    printf("Hora:      %s\n",hora_str);
    printf("Prioridad: %d\n\n", paciente->prioridad);
    return;
}

//Esta funcion traslada un paciente desde listaOrigen a listaDestino, actualizando su prioridad
//El paciente se inserta de forma ordenada a la lista destino y se elimina de la lista original
void asignarPrioridad(List *listaBaja, List *listaMedia, List *listaAlta,char nombre[60],int nuevaPrioridad){
    List *listaOrigen = listaBaja;
    Paciente* paciente = NULL;
    

    //Se busca al paciente en las tres listas
    paciente = buscarPaciente(listaBaja,nombre);
    if(!paciente){
        paciente = buscarPaciente(listaMedia, nombre);
        listaOrigen = listaMedia;
    }
    if(!paciente){
        paciente = buscarPaciente(listaAlta, nombre);
        listaOrigen = listaAlta;
    }
    if(!paciente){
        printf("El paciente no está registrado.\n");
        return;
    }
    if(paciente->prioridad == nuevaPrioridad){
        printf("El paciente ya tiene prioridad %d.\n", nuevaPrioridad);
        return;
    }

    paciente -> prioridad = nuevaPrioridad;
    if(nuevaPrioridad == 1)list_sortedInsert(listaBaja,paciente, lower_than);
    else if(nuevaPrioridad == 2)list_sortedInsert(listaMedia,paciente, lower_than);
    else if(nuevaPrioridad == 3) list_sortedInsert(listaAlta,paciente, lower_than);

    
    list_popCurrent(listaOrigen);
    printf("\nPrioridad actualizada con éxito. Mostrando datos actualizados...\n\n");
    mostrarPaciente(paciente);
    return;
}

//Esta funcion muestra todos los pacientes en una lista dada, ordenados por hora de llegada
void mostrarLista(List* lista){
    Paciente* actual = list_first(lista);
    while(actual){
        //Transformamos hora a str e imprimimos formateado
        char hora_str[10];
        strftime(hora_str, sizeof(hora_str), "%H:%M:%S",localtime(&actual->hora));
        
        printf("|%35s|%4d|%13d|%8s|\n", actual->nombre,actual->edad,actual->prioridad,hora_str);
        actual = list_next(lista);
    }
}

//Esta funcion muestra los datos del paciente que va a ser atendido

int main() {
    char opcion;
    List *pacientes = list_create(); // lista para pacientes de prioridad baja (1)
    List *prioridadMedia = list_create(); // lista para pacientes de prioridad media (2)
    List *prioridadAlta = list_create(); // lista para pacientes de prioridad alta (3)
    
    do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);
    limpiarBuffer();
    switch (opcion) {
    case '1':
        //Opción para registrar nuevos pacientes, pide nombre, edad y síntomas
        registrar_paciente(pacientes);
        break;
    case '2':
        //Opción para cambiar prioridad a los pacientes, pide nombre y nueva prioridad
        limpiarPantalla();
        
        //Se solicita nombre para encontrar al paciente
        char nombre[60];
        int nuevaPrioridad;
        printf("Ingrese nombre del paciente: ");
        scanf(" %59[^\n]s",nombre);
        //Se valida que el nombre no esté vacío
        if (strlen(nombre) == 0){
            printf("Nombre no válido. Volviendo al menú principal...\n");
            limpiarBuffer();
            break;
        }
        aMayus(nombre);
        limpiarBuffer();
        printf("Ingrese nueva prioridad: ");
        //Se comprueba que se haya ingresado una prioridad válida
        if(!scanf(" %d", &nuevaPrioridad) ||(nuevaPrioridad < 1) || (nuevaPrioridad > 3)){
            printf("Prioridad no válida. Volviendo al menú principal...\n");
            limpiarBuffer();
            break;
        }
        limpiarBuffer();
        asignarPrioridad(pacientes, prioridadMedia, prioridadAlta, nombre, nuevaPrioridad);
        break;
        
    case '3':
        //Opción para mostrar la lista de espera por completo.
        
        limpiarPantalla();
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
        //Opción para atender al siguiente paciente, respetando orden de prioridad y hora de llegada
        
        limpiarPantalla();
        /*Empezando por la prioridad más alta, se comprueba si existen pacientes.
        Si no existen pacientes se pasa a la siguiente prioridad.
        Si existen pacientes se muestra el primero de la lista y se eliminan sus datos
        Si ninguna lista tiene pacientes, se muestra un aviso*/
        Paciente* next = list_first(prioridadAlta);
        if(!(list_first(prioridadAlta) || list_first(prioridadMedia) || list_first(pacientes))){
            printf("No hay pacientes en espera.\n");
            break;
        }
        else
            printf("Atendiendo al siguiente en la lista...\n");
        if(!next){
            next = list_first(prioridadMedia);
            if(!next){
                next = list_first(pacientes);
                mostrarPaciente(next);
                list_popCurrent(pacientes);
            }
            else{
                mostrarPaciente(next);
                list_popCurrent(prioridadMedia);
            }
}
        else{
            mostrarPaciente(next);
            list_popCurrent(prioridadAlta);
        }
        break;
        
    case '5':
        //Opción para mostrar la lista de una prioridad específica, se solicita la prioridad.
        limpiarPantalla();
        
        /*Se solicita una prioridad (1 - 3) y se comprueba si la lista correspondiente tiene pacientes
        si los tiene se muestra toda la lista
        si no, se muestra un aviso*/
        int prioridad;
        printf("Ingrese prioridad (1 - 3): ");
        
        if(!scanf("%d", &prioridad) ||(prioridad != 1 && prioridad != 2 && prioridad != 3)){
            printf("Prioridad no válida.Volviendo al menú principal\n");
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
        //Opción para finalizar la ejecución del programa.
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
