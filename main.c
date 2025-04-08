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
  puts("6) Cargar tickets desde CSV");
  puts("7) Guardar tickets en CSV");
  puts("8) Salir");
}

// Estructura para almacenar la fecha y hora
typedef struct Tiempo
{
  int hora;
  int minuto;
  int segundo;
  int dia;
  int mes;
  int anyo;
} Tiempo;

// Estructura para almacenar los datos de un ticket
typedef struct Ticket
{
  int id;                 // Identificador único del ticket
  char descripcion[100];  // Descripción del problema o solicitud
  int prioridad;          // Nivel de prioridad (0: Baja, 1: Media, 2: Alta)
  Tiempo tiempo;          // Fecha y hora de creación del ticket
} Ticket;

// Obtiene la fecha y hora actual del sistema
Tiempo obtener_tiempo()
{
  // Fecha y hora actual del sistema
  time_t tiempo_actual = time(NULL);
  struct tm *tiempo_info = localtime(&tiempo_actual);

  // Asignación a la estructura Tiempo
  Tiempo tiempo;
  tiempo.hora = tiempo_info->tm_hour;
  tiempo.minuto = tiempo_info->tm_min;
  tiempo.segundo = tiempo_info->tm_sec;

  tiempo.dia = tiempo_info->tm_mday;
  tiempo.mes = tiempo_info->tm_mon + 1;
  tiempo.anyo = tiempo_info->tm_year + 1900;

  return tiempo;
}

// Transfiere todos los tickets de colaA a colaB
void repoblar_cola(Queue *colaA, Queue *colaB)
{
  // Repoblar colaB con los tickets de colaA
  Ticket *ticket;
  while ((ticket = (Ticket *) queue_front(colaA)) != NULL)
  {
    queue_insert(colaB, ticket);
    queue_remove(colaA);
  }
}

// Busca un ticket por su ID en las colas de prioridad
Ticket * buscar_ticket(Queue *tickets_queues[3], int id)
{
  Ticket *ticket;
  Queue *queue_aux = queue_create();
  Ticket *result = NULL;
  int flag = 0;

  // Se recorren las colas de prioridad
  for (int i = 0; i < 3; i++)
  {
    // Se recorren los tickets de la cola de prioridad
    while ((ticket = (Ticket *) queue_front(tickets_queues[i])) != NULL)
    {
      // Si el ticket tiene el id buscado, se muestra
      if (ticket->id == id)
      {
        result = ticket;
        flag = 1;
      }

      queue_insert(queue_aux, ticket);
      queue_remove(tickets_queues[i]);
    }

    repoblar_cola(queue_aux, tickets_queues[i]);
    queue_clean(queue_aux);

    if (flag == 1)
      break;
  }

  return result;
}

// Muestra la información de un ticket en formato tabla
void mostrar_ticket(Ticket *ticket, int show_header)
{
  // Mostrar ticket con su id, descripcion y prioridad
  // Diccionario de prioridades (N° -> Texto de Prioridad)
  char *msg_prioridades[3] = {"Baja", "Media", "Alta"};
  if (show_header)   // Si es el encabezado, se muestra
    printf(" ID | PRIORIDAD |        TIEMPO       | DESCRIPCION  \n");
  
  // Muestra el id, prioridad en formato de tabla
  printf("%3d | %-9s | ",
        ticket->id, msg_prioridades[ticket->prioridad]);

  // Muestra la fecha y hora actual en formato de tabla
  Tiempo now = obtener_tiempo();
  if (ticket->tiempo.anyo == now.anyo && ticket->tiempo.mes == now.mes && ticket->tiempo.dia == now.dia)
    printf("       Hoy %2d:%2d:%2d", ticket->tiempo.hora, ticket->tiempo.minuto, ticket->tiempo.segundo);
  else
    printf("%2d/%2d/%4d %2d:%2d:%2d", ticket->tiempo.dia, ticket->tiempo.mes, ticket->tiempo.anyo, ticket->tiempo.hora, ticket->tiempo.minuto, ticket->tiempo.segundo);

  // Muestra la descripcion del ticket en formato de tabla
  printf(" | %s\n", ticket->descripcion);
}

// Registra un nuevo ticket con prioridad baja por defecto
void registrar_ticket(Queue *queues[3])
{
  limpiarPantalla();
  printf("REGISTRAR NUEVO TICKET\n");

  Queue *cola = queues[PRIORIDAD_DEFAULT];

  // Reserva memoria para un nuevo ticket
  Ticket *ticket = (Ticket *) malloc(sizeof(Ticket));
  if (ticket == NULL)
  {
    puts("Error al registrar ticket");
    return;
  }

  // Solicita el id del ticket
  printf("Ingrese el ID del ticket: ");
  scanf("%d", &ticket->id);

  // Validar que el id no este repetido
  if (buscar_ticket(queues, ticket->id) != NULL)
  {
    puts("El ID del ticket ya existe");
    free(ticket);
    return;
  }

  // Solicita la descripcion del ticket
  printf("Ingrese la descripcion del ticket: \n");
  scanf(" %s", ticket->descripcion);

  // Asigna prioridad y tiempo por defecto
  ticket->prioridad = PRIORIDAD_DEFAULT;
  ticket->tiempo = obtener_tiempo();

  queue_insert(cola, ticket);
}


// Muestra todos los tickets en espera, ordenados por prioridad y antigüedad
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

    // Muestra el encabezado solo si es la cola de prioridad más alta (la primera)
    int show_header = 1;
    while ((ticket = (Ticket *) queue_front(tickets_queues[i])) != NULL)
    {
      mostrar_ticket(ticket, show_header && i == 2);
      queue_insert(queue_aux, ticket);
      queue_remove(tickets_queues[i]);
      show_header = 0;
    }

    repoblar_cola(queue_aux, tickets_queues[i]);
    queue_clean(queue_aux);
  }
}

// Compara dos tickets según su fecha y hora de creación (antigüedad)
// Devuelve 1 si ticket1 es más antiguo que ticket2, 0 en caso contrario
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

// Inserta un ticket en una cola de manera ordenada por antigüedad
void insertar_ticket(Queue *cola, Ticket *ticket)
{
  Ticket *ticket_aux;
  Queue *queue_aux = queue_create();

  if( queue_front(cola) == NULL )
  { 
    queue_insert(cola, ticket);
    return;
  }

  int flag = 0;
  while ((ticket_aux = (Ticket *) queue_front(cola)) != NULL)
  {
    if (comparar_tickets(ticket, ticket_aux) && flag == 0)
    {
      queue_insert(queue_aux, ticket);
      flag = 1;
    }
    
    queue_insert(queue_aux, ticket_aux);
    queue_remove(cola);
  }

  if (flag == 0)
    queue_insert(queue_aux, ticket);

  repoblar_cola(queue_aux, cola);
  queue_clean(queue_aux);
}

// Cambia la prioridad de un ticket existente y lo reubica en la cola correspondiente
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
        if (ticket->prioridad != nueva_prioridad)
        {
          ticket->prioridad = nueva_prioridad;
          insertar_ticket(tickets_queues[nueva_prioridad], ticket);
        }
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

// Función de menú para buscar un ticket por ID
void menu_buscar_ticket(Queue *tickets_queues[])
{
  limpiarPantalla();
  puts("BUSCAR TICKET");
  puts("=============");

  int id;
  printf("Ingrese el ID del ticket: ");
  scanf("%d", &id);

  Ticket *ticket = buscar_ticket(tickets_queues, id);
  if (ticket != NULL)
    mostrar_ticket(ticket, 1);
  else
    puts("Ticket no encontrado.");

}

// Atiende al siguiente ticket según la prioridad (Alta > Media > Baja) y antigüedad
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
      mostrar_ticket(ticket, 1);
      puts("Ticket atendido correctamente.");
      queue_remove(tickets_queues[i]);
      flag = 1;
      break;
    }
  }

  if (flag == 0)
    puts("No hay tickets en espera.");
}

// Carga tickets desde un archivo CSV, limpiando las colas actuales
void cargar_csv(Queue *tickets_queues[])
{
  limpiarPantalla();
  puts("CARGAR TICKETS DESDE CSV");
  puts("======================");

  for (int i = 0; i < 3; i++)
    queue_clean(tickets_queues[i]);
  
  char nombre_archivo[50];
  printf("Ingrese el nombre del archivo CSV (0: data/ejemplo1.csv): ");
  scanf(" %s", nombre_archivo);

  if (strcmp(nombre_archivo, "0") == 0)
    strcpy(nombre_archivo, "data/ejemplo1.csv");

  FILE *archivo = fopen(nombre_archivo, "r");
  if (archivo == NULL)
  {
    puts("Error al abrir el archivo CSV.");
    return;
  }

  char linea[256];
  while (fgets(linea, sizeof(linea), archivo))
  {
    // Separar los campos por comas
    char *token = strtok(linea, ",");
    if (token == NULL)
      continue;

    if (strcmp(token, "ID") == 0)
      continue;

    Ticket *ticket = (Ticket *)malloc(sizeof(Ticket));
    if (ticket == NULL)
    {
      puts("Error al crear ticket."); 
      fclose(archivo);
      return;
    }

    ticket->id = atoi(token);
    token = strtok(NULL, ",");
    if (token == NULL)
    {
      puts("Error al leer la descripcion del ticket.");
      free(ticket);
      continue;
    } 

    strcpy(ticket->descripcion, token);
    token = strtok(NULL, ",");
    if (token == NULL)
    {
      puts("Error al leer la prioridad del ticket.");     
      free(ticket);
      continue;
    }

    ticket->prioridad = atoi(token);
    token = strtok(NULL, ",");
    if (token == NULL)
    {
      puts("Error al leer la fecha del ticket.");
      free(ticket);
      continue;
    }   

    sscanf(token, "%d/%d/%d %d:%d:%d", &ticket->tiempo.dia, &ticket->tiempo.mes, &ticket->tiempo.anyo, &ticket->tiempo.hora, &ticket->tiempo.minuto, &ticket->tiempo.segundo);

    insertar_ticket(tickets_queues[ticket->prioridad], ticket);
  }

  fclose(archivo);
  puts("Tickets cargados correctamente.");
}

// Guarda los tickets actuales en un archivo CSV
void guardar_csv(Queue *tickets_queues[])
{
  limpiarPantalla();
  puts("GUARDAR TICKETS EN CSV");
  puts("=====================");
  
  // Solicita el nombre del archivo CSV
  char nombre_archivo[50];
  printf("Ingrese el nombre del archivo CSV (0: data/data.csv): ");
  scanf(" %s", nombre_archivo);

  // Si el nombre es 0, se guarda en el archivo data/data.csv
  if (strcmp(nombre_archivo, "0") == 0)
    strcpy(nombre_archivo, "data/data.csv");
    
  // Abre el archivo CSV en modo escritura
  FILE *archivo = fopen(nombre_archivo, "w");
  if (archivo == NULL){
    puts("Error al abrir el archivo CSV.");
    return;
  }

  // Escribe el encabezado del archivo CSV
  fprintf(archivo, "ID,Descripcion,Prioridad,Fecha\n");

  // Recorre las colas de prioridad y escribe los tickets en el archivo CSV
  for (int i = 0; i < 3; i++)
  {
    Ticket *ticket;
    Queue *queue_aux = queue_create();
    while ((ticket = (Ticket *)queue_front(tickets_queues[i])) != NULL)
    { 
      fprintf(archivo, "%d,%s,%d,%d/%d/%d %d:%d:%d\n", ticket->id, ticket->descripcion, ticket->prioridad, ticket->tiempo.dia, ticket->tiempo.mes, ticket->tiempo.anyo, ticket->tiempo.hora, ticket->tiempo.minuto, ticket->tiempo.segundo);
      queue_insert(queue_aux, ticket);
      queue_remove(tickets_queues[i]);
    }

    repoblar_cola(queue_aux, tickets_queues[i]);
    queue_clean(queue_aux);
  }

  fclose(archivo);
  puts("Tickets guardados correctamente.");
}

// Función principal del programa
int main()
{
  // Colas de tickets (Baja, Media, Alta)
  Queue *tickets_queues[3] = {queue_create(), queue_create(), queue_create()};

  // Bucle principal
  char opcion;
  int running = 1;
  do
  {
    mostrarMenuPrincipal();

    printf("Ingrese su opcion: ");
    scanf(" %c", &opcion);

    // Selecciona la opción del menú
    switch (opcion)
    {
    case '1':
      registrar_ticket(tickets_queues);
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
      menu_buscar_ticket(tickets_queues);
      break;
    case '6':
      cargar_csv(tickets_queues);
      break;
    case '7':
      guardar_csv(tickets_queues);
      break;
    case '8':
      running = 0;
      puts("Saliendo del sistema de gestión de tickets...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (running);

  // Liberar recursos, si es necesario
  for (int i = 0; i < 3; i++)
    queue_clean(tickets_queues[i]);

  return 0;
}