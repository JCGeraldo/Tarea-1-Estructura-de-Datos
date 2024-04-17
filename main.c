#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Paciente{
    char nombre[50];
    int edad;
    char sintomas[100];
    int prioridad;
    char hora[9];    
}Paciente;


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

void registrar_paciente(List *pacientes) {
    printf("Registrar nuevo paciente\n");
    Paciente *paciente = (Paciente *)malloc(sizeof(Paciente));
    
    printf("Ingrese el nombre del paciente: ");
    scanf(" %[^\n]", paciente->nombre);
    printf("Ingrese la edad del paciente: ");
    scanf("%d", &paciente->edad);
    printf("Ingrese los síntomas del paciente: ");
    scanf(" %[^\n]", paciente->sintomas);
    printf("Ingrese la hora de llegada del paciente (HH:MM): ");
    scanf(" %[^\n]", paciente->hora);
    paciente->prioridad = 1; // Prioridad inicial es 1
    list_pushBack(pacientes, paciente);
    printf("Paciente registrado con éxito.\n");
}

int mostrarPaciente(List *paciente,char nombre[50]){
    Paciente *paciente_actual = (Paciente *)list_first(paciente);
    while (paciente_actual){
        if (strcmp(paciente_actual->nombre,nombre) == 0){
            printf("Nombre: %s\n", paciente_actual->nombre);
            printf("Edad: %d\n", paciente_actual->edad);
            printf("Síntomas: %s\n", paciente_actual->sintomas);
            printf("Hora de llegada: %s\n", paciente_actual->hora);
            printf("Prioridad: %d\n", paciente_actual->prioridad);
            return 1;
        }
        paciente_actual = (Paciente *)list_next(paciente);
        if(paciente_actual == NULL){
            printf("Paciente no encontrado\n");
            return 0;
        }
    }
}

void asignarPrioridad(List *listaOrigen,List *listaDestino,char nombre[50],int nuevaPrioridad){
    Paciente *aux = list_first(listaOrigen);
    while(aux){
        if(strcmp(aux->nombre,nombre)==0){
            aux->prioridad = nuevaPrioridad;
            list_pushBack(listaDestino, aux);
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


void mostrar_lista_pacientes(List *pacientes) {
  // Mostrar pacientes en la cola de espera
  printf("Pacientes en espera: \n");
  // Aquí implementarías la lógica para recorrer y mostrar los pacientes
}

int main() {
    char opcion;
    List *pacientes = list_create(); // puedes usar una lista para gestionar los pacientes
    List *prioridadMedia = list_create();
    List *prioridadAlta = list_create();
    
    do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior
    
    switch (opcion) {
    case '1':
        registrar_paciente(pacientes);
        break;
    case '2':
        // Lógica para asignar prioridad
        char nombre[50];
        int nuevaPrioridad;
        printf("Ingrese nombre del paciente:");
        scanf(" %[^\n]",nombre);
        if(!mostrarPaciente(pacientes,nombre))
            break;
        printf("Ingrese nueva prioridad (2 - 3):");
        scanf(" %d",&nuevaPrioridad);
        if(nuevaPrioridad == 2)
            asignarPrioridad(pacientes,prioridadMedia,nombre,nuevaPrioridad);
        else
            asignarPrioridad(pacientes, prioridadAlta, nombre, nuevaPrioridad);
        break;
    case '3':
      mostrar_lista_pacientes(pacientes);
      break;
    case '4':
      // Lógica para atender al siguiente paciente
      break;
    case '5':
      // Lógica para mostrar pacientes por prioridad
      break;
    case '6':
      puts("Saliendo del sistema de gestión hospitalaria...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();
    
    } while (opcion != '6');
    
    // Liberar recursos, si es necesario
    list_clean(pacientes);
    
    return 0;
}
