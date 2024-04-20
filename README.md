# Sistema de Gestión de Pacientes

Este proyecto consiste en un sistema de gestión de pacientes desarrollado en C. El sistema permite registrar, priorizar y atender pacientes en un entorno hospitalario simulado.

El programa almacena la información de los pacientes, incluyendo su nombre, edad, síntomas, prioridad y hora de registro. Además, cuenta con un menú interactivo que permite al usuario registrar nuevos pacientes, asignarles prioridad, mostrar la lista de espera, atender al siguiente paciente, y mostrar pacientes por prioridad.

## Ejecución

Para probar este código siga estos pasos:
* Importe el repositorio desde su cuenta en repl.it
* Presione el botón **RUN** para ejecutar el código.
* Si esto no funciona, vaya a la terminal shell y compile el código con el siguiente comando:
  ```
  gcc tdas/*.c main.c -Wno-unused-result -o main
  ```
* Luego utilice el comando `./main` y debería ver de inmediato el menú principal.
* Ahora está todo listo para empezar a probar el programa.

## Utilizando el programa

Al ejecutar el programa usted verá un menú como el siguiente:
```
========================================
     Sistema de Gestión Hospitalaria
========================================
1) Registrar paciente
2) Asignar prioridad a paciente
3) Mostrar lista de espera
4) Atender al siguiente paciente
5) Mostrar pacientes por prioridad
6) Salir
Ingrese su opción: 
```
Usted debe ingresar un número entre 1 y 6 para elegir una funcionalidad del programa. Cada vez que elija una opción que no sea 6, el programa ejecutará la opción elegida y volverá automáticamente al menú principal al terminar.

## Ejemplo de Uso

A continuación se muestran ejemplos de lo que debería ver al utilizar estas opciones. Se incluye una explicación de cada una de las opciones.
<details>
<summary>Opción 1 - Registrar paciente:</summary>
Esta opción comienza el registro de un paciente. Se le solicitará el nombre del paciente, junto con su edad y sus síntomas. Todo paciente tiene una prioridad inicial baja.

```
Ingrese su opción: 1
Registrar nuevo paciente
Ingrese el nombre del paciente: juan geraldo
Ingrese la edad del paciente: 24
Ingrese los síntomas del paciente: fiebre
Paciente registrado con éxito.
```
</details>

<details>
<summary> Opción 2 - Asignar prioridad a paciente:</summary>
Esta opción permite cambiar la prioridad de un paciente ya registrado. Se solicita el nombre del paciente, su prioridad actual y su nueva prioridad. Finalmente se mostrarán los nuevos datos del paciente.

  ```
Ingrese su opción: 2
Ingrese nombre del paciente:juan
Ingrese la prioridad actual (1 - 3): 1
Nombre: JUAN
Edad: 24
Síntomas: fiebre
Hora: 02:49:18
Prioridad: 1
```
> Datos actuales del paciente.

```
Ingrese nueva prioridad (1 - 3): 2
Nombre: JUAN
Edad: 24
Síntomas: fiebre
Hora: 02:49:18
Prioridad: 2
Prioridad actualizada con éxito.
```
> Datos actualizados.
</details>

 <details>
<summary>Opción 3 - Mostrar lista de espera:</summary>
Esta opción permite visualizar la lista de espera por completo. Se mostrarán en la pantalla los datos de todos los pacientes en espera, ordenados por prioridad y por orden de llegada dentro de cada prioridad.

   ```
Ingrese su opción: 3
|                             Nombre|Edad|    Prioridad|    Hora|
|                       JUAN GERALDO|  24|            2|02:54:58|
|                      MARCELA ULLOA|  43|            1|02:55:25|
```
 </details>

<details>
<summary>Opción 4 - Atender al siguiente paciente:</summary>
  
Esta opción mostrara los datos del siguiente paciente que debe ser atendido, respetando el orden de prioridad y de hora de llegada. Esto eliminará los registros del paciente, asumiendo que ha sido atendido.

```
Ingrese su opción: 4
Atendiendo a:         JUAN GERALDO
Edad del paciente:              24
Síntomas:                   fiebre
Prioridad:                       2
Hora de llegada:          02:54:58
```
</details>

<details>
<summary>Opción 5 - Mostrar pacientes por prioridad:</summary>
Esta opción solicitará una prioridad al usuario y mostrará la lista de espera correspondiente a esa prioridad.

  ```
Ingrese su opción: 5
Ingrese prioridad (1 - 3): 1
Mostrando lista de prioridad baja...
|                             Nombre|Edad|    Prioridad|    Hora|
|                      MARCELA ULLOA|  43|            1|02:55:25|
```

</details>

<details>
<summary>
  Opción 6 - Salir:
</summary>
  
Finaliza la ejecución actual del programa.

```
Ingrese su opción: 6
Saliendo del sistema de gestión hospitalaria...
Presione una tecla para continuar...
```
</details>

## Observaciones
En este sistema se utiliza un orden de prioridad numérico, donde 1 es la prioridad más baja, 2 es la prioridad media y 3 es la prioridad más alta.

Cualquier otra prioridad ingresada se considera inválida y provoca que el sistema muestre un aviso y vuelva al menu principal.


## Problemas Conocidos
> [!CAUTION]
>  El sistema no permite guardar los datos trabajados antes de salir del sistema, por lo que todo trabajo se debe realizar durante una única sesión.

* Al ingresar caracteres especiales, la función mostrar lista de espera, mostrara una lista descuadrada. Esto no afecta a la funcionalidad del programa.
* Entradas inesperadas (letras en la edad, números negativos) provocan que el programa muestre un aviso y vuelva al menú principal.

<details>
<summary>Tabla descuadrada</summary>
  
```
|                             Nombre|Edad|    Prioridad|    Hora|
|                       JUAN GERALDO|  24|            3|04:44:09|
|                       PABLO YAñEZ|  18|            1|04:43:56|
|                   PATRICIO ESPAñA|  45|            1|04:45:29|
|                      MARCELA ULLOA|  45|            1|04:45:48|
```

</details>

## A Mejorar
* Permitir al usuario editar datos de los pacientes, o eliminar pacientes de las listas de espera.
* Mejorar la interacción con el usuario.
* Permitir guardar los datos de los pacientes antes de salir del sistema.
  
