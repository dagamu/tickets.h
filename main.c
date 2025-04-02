#include "tdas/list.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     SISTEMA DE GESTION DE TICKETS");
  puts("========================================");

  puts("1) Registrar ticket");
  puts("2) Asignar prioridad a ticket");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente ticket");
  puts("5) Mostrar tickets por prioridad");
  puts("6) Salir");
}

typedef struct Ticket {
  int id;
  char descripcion[100];
  int prioridad; // [ Bajo: 1, Medio: 2, Alto: 3 ] default: 'B'
} Ticket;

void registrar_ticket(List *tickets) {
    limpiarPantalla();
    printf("REGISTRAR NUEVO TICKET\n");

    Ticket *ticket = (Ticket *)malloc(sizeof(Ticket));
    if (ticket == NULL) 
        { puts("Error al registrar ticket"); return; }

    printf("Ingrese el ID del ticket: ");
    scanf("%d", &ticket->id);
    printf("Ingrese la descripcion del ticket: \n");
    scanf(" %s", ticket->descripcion);
    ticket->prioridad = 'B';

    list_pushBack(tickets, ticket);
}

void mostrar_lista_ticket(List *tickets) {

    limpiarPantalla();
    puts("TICKETS EN ESPERA");  
    puts("=================");

    Ticket *ticket = (Ticket *)list_first(tickets);
    while (ticket != NULL) {
        printf("ID: %d, Descripcion: %s, Prioridad: %c\n", ticket->id, ticket->descripcion, ticket->prioridad);
        ticket = (Ticket *)list_next(tickets);
    }

    // Mostrar ticket en la cola de espera
    // printf("ticket en espera: \n");
    // Aquí implementarías la lógica para recorrer y mostrar los ticket
}

int main() {
  char opcion;
  List *tickets = list_create(); // puedes usar una lista para gestionar los ticket

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opcion: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior

    switch (opcion) {
    case '1':
      registrar_ticket(tickets);
      break;
    case '2':
      // Lógica para asignar prioridad
      break;
    case '3':
      mostrar_lista_ticket(tickets);
      break;
    case '4':
      // Lógica para atender al siguiente ticket
      break;
    case '5':
      // Lógica para mostrar ticket por prioridad
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
  list_clean(tickets);

  return 0;
}