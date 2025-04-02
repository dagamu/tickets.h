# Tickets.h

Daniel Gajardo ICD
Tarea I - ICD2344-1 Estructuras de Datos - Prof. Ignacio Araya

---

Sistema de gestión de tickets de soporte técnico

En esta tarea diseñarás una aplicación para administrar **tickets de soporte técnico** en una empresa, siguiendo un sistema de colas con prioridades. Un *ticket* representa una solicitud de ayuda ingresada por un usuario al sistema de soporte. Cada ticket contiene información relevante como un ID único, una descripción del problema, el nivel de prioridad (Alto, Medio o Bajo) y la hora de registro. Este sistema permite organizar y gestionar de forma eficiente las solicitudes de asistencia técnica, priorizando los casos más urgentes.

La aplicación que desarrollarás permitirá registrar nuevos tickets, modificar su prioridad, procesar el siguiente ticket según urgencia, y mostrar la lista de tickets pendientes.

---

## Inicio Rápido

```shell
$ gcc tdas/*.c main.c -Wno-unused-result -o main
$ ./main
```
