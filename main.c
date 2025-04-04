#include "tdas/queue.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PRIORIDAD_DEFAULT 0 // Baja

// Menú principal
void mostrarMenuPrincipal()
{
  limpiarPantalla();
  puts("========================================");
  puts("     SISTEMA DE GESTION DE TICKETS");
  puts("========================================");

  puts("1) Registrar ticket");
  puts("2) Asignar prioridad a ticket");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente ticket");
  puts("5) Buscar ticket");
  puts("6) Salir");
}

typedef struct Tiempo
{
  int hora;
  int minuto;
  int segundo;
  int dia;
  int mes;
  int anyo;
} Tiempo;

typedef struct Ticket
{
  int id;
  char descripcion[100];
  int prioridad; // [ Bajo: 0, Medio: 1, Alto: 2 ] default: 'B'
  Tiempo tiempo;
} Ticket;

Tiempo obtener_tiempo()
{
  time_t tiempo_actual = time(NULL);
  struct tm *tiempo_info = localtime(&tiempo_actual);

  Tiempo tiempo;
  tiempo.hora = tiempo_info->tm_hour;
  tiempo.minuto = tiempo_info->tm_min;
  tiempo.segundo = tiempo_info->tm_sec;

  tiempo.dia = tiempo_info->tm_mday;
  tiempo.mes = tiempo_info->tm_mon + 1;
  tiempo.anyo = tiempo_info->tm_year + 1900;

  return tiempo;
}

void mostrar_ticket(Ticket *ticket)
{
  // Mostrar ticket con su id, descripcion y prioridad
  // Diccionario de prioridades (N° -> Texto de Prioridad)
  char *msg_prioridades[3] = {"Baja", "Media", "Alta"};
  printf("ID: %d, Descripcion: %s, Prioridad: %s\n",
         ticket->id, ticket->descripcion, msg_prioridades[ticket->prioridad]);
}

void registrar_ticket(Queue *cola)
{
  limpiarPantalla();
  printf("REGISTRAR NUEVO TICKET\n");

  Ticket *ticket = (Ticket *) malloc(sizeof(Ticket));
  if (ticket == NULL)
  {
    puts("Error al registrar ticket");
    return;
  }

  printf("Ingrese el ID del ticket: ");
  scanf("%d", &ticket->id);
  printf("Ingrese la descripcion del ticket: \n");
  scanf(" %s", ticket->descripcion);

  ticket->prioridad = PRIORIDAD_DEFAULT;
  ticket->tiempo = obtener_tiempo();

  queue_insert(cola, ticket);
}

void repoblar_cola(Queue *colaA, Queue *colaB)
{
  // Repoblar colaB con los tickets de colaA
  Ticket *ticket;
  while ((ticket = (Ticket *)queue_front(colaA)) != NULL)
  {
    queue_insert(colaB, ticket);
    queue_remove(colaA);
  }
}

void mostrar_lista_ticket(Queue *tickets_queues[])
{

  limpiarPantalla();
  puts("TICKETS EN ESPERA");
  puts("=================");

  // Mostrar tickets ordenados por prioridad y tiempo
  // Se recorre la cola de prioridad en orden descendente (Alta -> Media -> Baja)
  for (int i = 2; i >= 0; i--)
  {
    Ticket *ticket;
    Queue *queue_aux = queue_create();

    while ((ticket = (Ticket *)queue_front(tickets_queues[i])) != NULL)
    {
      mostrar_ticket(ticket);
      queue_insert(queue_aux, ticket);
      queue_remove(tickets_queues[i]);
    }

    repoblar_cola(queue_aux, tickets_queues[i]);
    queue_clean(queue_aux);
  }
}

int comparar_tickets(Ticket *ticket1, Ticket *ticket2)
{

  if (ticket1->tiempo.anyo != ticket2->tiempo.anyo)
    return ticket1->tiempo.anyo > ticket2->tiempo.anyo;

  if (ticket1->tiempo.mes != ticket2->tiempo.mes)
    return ticket1->tiempo.mes > ticket2->tiempo.mes;

  if (ticket1->tiempo.dia != ticket2->tiempo.dia)
    return ticket1->tiempo.dia > ticket2->tiempo.dia;

  if (ticket1->tiempo.hora != ticket2->tiempo.hora)
    return ticket1->tiempo.hora > ticket2->tiempo.hora;

  if (ticket1->tiempo.minuto != ticket2->tiempo.minuto)
    return ticket1->tiempo.minuto > ticket2->tiempo.minuto;

  if (ticket1->tiempo.segundo != ticket2->tiempo.segundo)
    return ticket1->tiempo.segundo > ticket2->tiempo.segundo;

  return 0;
}

void insertar_ticket(Queue *cola, Ticket *ticket)
{
  Ticket *ticket_aux;
  Queue *queue_aux = queue_create();
  while ((ticket_aux = (Ticket *)queue_front(cola)) != NULL)
  {
    if (comparar_tickets(ticket, ticket_aux))
      queue_insert(queue_aux, ticket);
    else
      queue_insert(queue_aux, ticket_aux);

    queue_remove(cola);
  }

  repoblar_cola(queue_aux, cola);
  queue_clean(queue_aux);
}

void asignar_prioridad(Queue *tickets_queues[])
{
  limpiarPantalla();
  puts("ASIGNAR PRIORIDAD A TICKET");
  puts("=========================");

  int id;
  printf("Ingrese el ID del ticket: ");
  scanf("%d", &id);

  int nueva_prioridad;
  printf("Ingrese la nueva prioridad (0: Bajo, 1: Medio, 2: Alto): ");
  scanf("%d", &nueva_prioridad);

  Ticket *ticket;
  Queue *queue_aux = queue_create();

  // Validar prioridad
  if (nueva_prioridad < 0 || nueva_prioridad > 2)
  {
    puts("Prioridad no válida. Debe ser 0, 1 o 2.");
    return;
  }

  // Buscar ticket recorriendo colas y asignar prioridad al encontrar ID
  int flag = 0;
  for (int i = 0; i < 3; i++)
  {
    flag = 0;
    while ((ticket = (Ticket *)queue_front(tickets_queues[i])) != NULL)
    {

      if (ticket->id == id)
      {
        // Asignar prioridad y reorganizar cola
        ticket->prioridad = nueva_prioridad;
        insertar_ticket(tickets_queues[nueva_prioridad], ticket);
        flag = 1;
      }
      else
        queue_insert(queue_aux, ticket);

      queue_remove(tickets_queues[i]);
    }

    repoblar_cola(queue_aux, tickets_queues[i]);
    queue_clean(queue_aux);

    if (flag == 1)
      break;
  }

  if (flag == 0)
    puts("Ticket no encontrado.");
}

void buscar_ticket(Queue *tickets_queues[])
{
  limpiarPantalla();
  puts("BUSCAR TICKET");
  puts("=============");

  int id;
  printf("Ingrese el ID del ticket: ");
  scanf("%d", &id);

  Ticket *ticket;
  Queue *queue_aux = queue_create();
  // Se recorren las colas de prioridad
  for (int i = 0; i < 3; i++)
  {
    // Se recorren los tickets de la cola de prioridad
    while ((ticket = (Ticket *)queue_front(tickets_queues[i])) != NULL)
    {
      // Si el ticket tiene el id buscado, se muestra
      if (ticket->id == id)
        mostrar_ticket(ticket);

      queue_insert(queue_aux, ticket);
      queue_remove(tickets_queues[i]);
    }

    repoblar_cola(queue_aux, tickets_queues[i]);
    queue_clean(queue_aux);
  }
}

void atender_ticket(Queue *tickets_queues[])
{
  limpiarPantalla();
  puts("ATENDER TICKET");
  puts("=============");

  Ticket *ticket;
  int flag = 0;

  for (int i = 2; i >= 0; i--)
  {
    ticket = (Ticket *)queue_front(tickets_queues[i]);
    if (ticket != NULL)
    {
      mostrar_ticket(ticket);
      puts("Ticket atendido correctamente.");
      queue_remove(tickets_queues[i]);
      flag = 1;
      break;
    }
  }

  if (flag == 0)
    puts("No hay tickets en espera.");
}

int main()
{
  // Colas de tickets (Baja, Media, Alta)
  Queue *tickets_queues[3] = {queue_create(), queue_create(), queue_create()};

  // Bucle principal
  char opcion;
  do
  {
    mostrarMenuPrincipal();

    printf("Ingrese su opcion: ");
    scanf(" %c", &opcion);

    switch (opcion)
    {
    case '1':
      registrar_ticket(tickets_queues[PRIORIDAD_DEFAULT]);
      break;
    case '2':
      asignar_prioridad(tickets_queues);
      break;
    case '3':
      mostrar_lista_ticket(tickets_queues);
      break;
    case '4':
      atender_ticket(tickets_queues);
      break;
    case '5':
      buscar_ticket(tickets_queues);
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
  for (int i = 0; i < 3; i++)
    queue_clean(tickets_queues[i]);

  return 0;
}