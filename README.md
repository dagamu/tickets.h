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

## Funcionalidades

### Funcionando correctamente:

- Registrar pacientes con sus datos básicos y una prioridad inicial.
- Asignar o modificar la prioridad de los pacientes.
- Ver la lista de espera de pacientes, ordenada por prioridad y hora de registro.
- Atender al siguiente paciente, respetando el orden de prioridad.

#### Contribuciones

### Problemas conocidos:

- La funcionalidad para exportar e importar la lista de espera desde/hacia un archivo CSV aún está en desarrollo.

### A mejorar:

- Implementar una interfaz de usuario más amigable.
- Permitir la edición de los datos de los pacientes.
