# Tickets.h

Daniel Gajardo ICD

Tarea I - ICD2344-1 Estructuras de Datos - Prof. Ignacio Araya

---

## Sistema de gestión de tickets de soporte técnico

### Descripción

En esta tarea diseñarás una aplicación para administrar **tickets de soporte técnico** en una empresa, siguiendo un sistema de colas con prioridades. Un *ticket* representa una solicitud de ayuda ingresada por un usuario al sistema de soporte. Cada ticket contiene información relevante como un ID único, una descripción del problema, el nivel de prioridad (Alto, Medio o Bajo) y la hora de registro. Este sistema permite organizar y gestionar de forma eficiente las solicitudes de asistencia técnica, priorizando los casos más urgentes.

La aplicación que desarrollarás permitirá registrar nuevos tickets, modificar su prioridad, procesar el siguiente ticket según urgencia, y mostrar la lista de tickets pendientes.

---

## Inicio Rápido

```shell
$ gcc tdas/*.c main.c -Wno-unused-result -Wall -Wextra -o main
$ ./main
```

---

## Cómo compilar y ejecutar

Este sistema ha sido desarrollado en lenguaje C y puede ejecutarse fácilmente utilizando Visual Studio Code junto con una extensión para C/C++, como C/C++ Extension Pack de Microsoft. Para comenzar a trabajar con el sistema en tu equipo local, sigue estos pasos:

### Requisitos

- VS CODE
- Instalar la extensión **C/C++** (Microsoft).
- Tener instalado un compilador de C (como **gcc**). Si estás en Windows, se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar el entorno [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

### Paso a paso

- Abre el proyecto en Visual Studio Code
    - Inicia Visual Studio Code.
    - Selecciona Archivo > Abrir carpeta... y elige la carpeta donde descomprimiste el proyecto.
- Compila el código
    - Abre el archivo principal (por ejemplo, main.c).
    - Abre la terminal integrada (Terminal > Nueva terminal).
    - En la terminal, compila el programa con el siguiente comando 

```shell
$ gcc tdas/*.c main.c -Wno-unused-result -Wall -Wextra -o main
```

- Ejecuta el programa
    - Una vez compilado, puedes ejecutar la aplicación con:

```shell
$ ./main
```

---

## Ejemplo de Uso

```shell
$ ./main
========================================
     SISTEMA DE GESTION DE TICKETS      
========================================
1) Registrar ticket
2) Asignar prioridad a ticket
3) Mostrar lista de espera
4) Atender al siguiente ticket
5) Buscar ticket
6) Cargar tickets desde CSV
7) Guardar tickets en CSV
8) Salir
Ingrese su opcion:

--> 6

CARGAR TICKETS DESDE CSV
======================
Ingrese el nombre del archivo CSV (0: data/ejemplo1.csv):

--> 0

Tickets cargados correctamente. 

========================================
     SISTEMA DE GESTION DE TICKETS      
========================================
1) Registrar ticket
2) Asignar prioridad a ticket
3) Mostrar lista de espera
4) Atender al siguiente ticket
5) Buscar ticket
6) Cargar tickets desde CSV
7) Guardar tickets en CSV
8) Salir
Ingrese su opcion:

--> 1

REGISTRAR NUEVO TICKET
Ingrese el ID del ticket: --> 11
Ingrese la descripcion del ticket: --> Prueba de Sistema

========================================
     SISTEMA DE GESTION DE TICKETS      
========================================
1) Registrar ticket
2) Asignar prioridad a ticket
3) Mostrar lista de espera
4) Atender al siguiente ticket
5) Buscar ticket
6) Cargar tickets desde CSV
7) Guardar tickets en CSV
8) Salir
Ingrese su opcion:

--> 2

ASIGNAR PRIORIDAD A TICKET
=========================
Ingrese el ID del ticket: --> 11
Ingrese la nueva prioridad (0: Bajo, 1: Medio, 2: Alto): --> 2

========================================
     SISTEMA DE GESTION DE TICKETS      
========================================
1) Registrar ticket
2) Asignar prioridad a ticket
3) Mostrar lista de espera
4) Atender al siguiente ticket
5) Buscar ticket
6) Cargar tickets desde CSV
7) Guardar tickets en CSV
8) Salir
Ingrese su opcion:

--> 3

TICKETS EN ESPERA
=================
 ID | PRIORIDAD |        TIEMPO       | DESCRIPCION
 11 | Alta      |        Hoy  9:14: 0 | Prueba
  4 | Alta      |  3/ 4/2025 10: 5: 0 | Se cayo la conexion WiFi
  1 | Alta      |  3/ 4/2025  8:23:10 | No puedo acceder al correo
  3 | Alta      |  3/ 4/2025  7:58:33 | La PC no enciende
  9 | Alta      |  1/ 4/2025 11: 1:39 | Teclado deja de funcionar aleatoriamente
  8 | Media     |  3/ 4/2025 10:30:55 | Problema con Zoom en videollamada
  5 | Media     |  3/ 4/2025  9:50:18 | No se carga el archivo adjunto
  2 | Media     |  3/ 4/2025  9:12:45 | Error al imprimir en la oficina
  7 | Media     |  3/ 4/2025  8:44: 7 | Actualizacion de software fallida
  6 | Baja      |  3/ 4/2025 11:13:22 | Consulta sobre cambio de password
 10 | Baja      |  2/ 4/2025 10:18:27 | Requiere acceso a carpeta compartida

```

---

## Funcionalidades

### Funcionando correctamente:

- Registrar tickets con sus datos básicos y una prioridad inicial.
- Asignar o modificar la prioridad de los tickets.
- Ver la lista de espera de tickets, ordenada por prioridad y hora de registro.
- Atender el siguiente ticket, respetando el orden de prioridad.

#### Contribuciones

- **Cargar y Guardar CSV**
    - Se añade una opción para guardar la lista de tickets actual en formato CSV con columnas ID, Descripción, Prioridad y Fecha.
    - Se añade una opción para cargar en memoria una lista de tickets en formato CSV.

### A mejorar:

- Permitir la edición de los datos de los tickets.
