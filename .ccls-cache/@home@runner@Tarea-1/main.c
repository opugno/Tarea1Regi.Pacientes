#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   //hora interna del ingreso de pacientes

//stuct tipoPaciente que guarde los datos de los pacientes
typedef struct 
{
  char name[100];
  int age;
  char simptoms[10000];
  int priority;
  time_t arrivalTime; //la hora en que llego el paciente.

} tipoPaciente;

void limpiarBufferEntrada() //limpiar el buffer
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {}
}

// Función para limpiar la pantalla
void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("\nPresione una tecla para continuar...");
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

void registrar_paciente(List *pacientes) 
{
  // Aquí implementarías la lógica para registrar un nuevo paciente
  tipoPaciente *newPaciente = (tipoPaciente *)malloc(sizeof(tipoPaciente));
  if (newPaciente == NULL) 
  {
    return;
  }

  printf("\nRegistrar nuevo paciente\n\n");

  
  limpiarBufferEntrada();
  printf("Ingrese el nombre del paciente:\n");
  fgets(newPaciente -> name, sizeof(newPaciente -> name), stdin);
  newPaciente -> name [strcspn(newPaciente -> name, "\n")] = '\0';
  
  printf("Ingrese la edad del paciente.\n");
  scanf ("%d", &newPaciente -> age);
  getchar();

  printf("Ingrese los síntomas del paciente.\n");
  fgets(newPaciente -> simptoms, sizeof(newPaciente -> simptoms), stdin);
  newPaciente -> simptoms [strcspn(newPaciente -> simptoms, "\n")] = '\0';
  
  newPaciente -> priority = 1; // se inicializa prioridad en baja (1);
  newPaciente -> arrivalTime = time(NULL); // se inicializa el tiempo de ingreso del paciente

  list_pushBack(pacientes, newPaciente); //se guardan los datos
}

//ordenamiento de la prioridades
int lower_than(void *data1, void *data2) 
{
  tipoPaciente *paciente1 = (tipoPaciente *) data1;
  tipoPaciente *paciente2 = (tipoPaciente *) data2;
  if (paciente1 -> priority != paciente2 -> priority)  //primero por prioridad.
    return paciente1 -> priority > paciente2 -> priority;
  else
    return difftime(paciente1 -> arrivalTime, paciente2 -> arrivalTime) < 0; //en caso de tener misma prioridad, se ordena por tiempo de ingreso.
}

//funcion para asignar la nueva prioridad a los pacientes
void asignar_prioridad(List *pacientes) 
{
  tipoPaciente *paciente = list_first(pacientes);  //validacion de la lista.
  if (paciente == NULL) 
  {
    printf("\n*~No hay pacientes*~\n");
    return;
  }
  
  limpiarBufferEntrada();
  printf("\nIngrese el nombre del paciente:\n");
  char nombrePaciente[100];
  fgets(nombrePaciente, sizeof(nombrePaciente), stdin);
  nombrePaciente [strcspn(nombrePaciente, "\n")] = '\0';
  
  while (paciente != NULL) 
  {
    if (strcmp(paciente -> name, nombrePaciente) == 0) //ver si el paciente existe.
    {
      printf("\nIngrese la nueva prioridad del paciente.\nRecuerde que prioridad BAJA (1), prioridad MEDIA (2) y prioridad ALTA (3).\n");
      int actualPriority;
      scanf("%d", &actualPriority);
      if (actualPriority < 1 || actualPriority > 3)  //validar prioridad.
      {
        printf("\n°--Por favor ingrese una prioridad válida--°\n");
        continue;
      }
      getchar();
      paciente -> priority = actualPriority;
      
      //aqui se  tiene que ordenar por prioridad de mayor prioridad a menor prioridad
      tipoPaciente *pacienteEliminado = list_popCurrent(pacientes);
      list_sortedInsert(pacientes, pacienteEliminado, lower_than);
      return;
    } 
    paciente = list_next(pacientes);
  }
  printf("No se encontró el paciente %s. Intente con otro nombre.\n" , nombrePaciente);
  return;
}

//mostrar pacientes.
void mostrar_lista_pacientes(List *pacientes) 
{
  // Mostrar pacientes en la cola de espera
  printf("\nPacientes en espera: \n\n");
  
  tipoPaciente *current = list_first(pacientes);
  if (current == NULL)                                 //validacion de la lista.
  {
    printf("*~No hay pacientes en lista de espera*~\n"); 
    return;
  }

  //pacientes ordenador por prioridad y si en caso de repetirse la prioridad se muestra ordenado por hora de llegada.
  printf("°**~===========================~**°\n");
  while (current != NULL)
  {
    printf("Nombre del Paciente    : %s.\n", current -> name);
    switch (current -> priority)
    {
      case 1:
        printf("El paciente tiene prioridad BAJA. \n");
        break;
      case 2:
        printf("El paciente tiene prioridad MEDIA.\n");
        break;
      case 3:
        printf("El paciente tiene prioridad ALTA. \n");
        break;
    }
    current = list_next(pacientes);
    printf("°**~===========================~**°\n");
  }
  
  return;
}

//mostramos el primer paciente para luego sacarlo y mostrar el siguente.
void siguiente_paciente(List *pacientes)
{
  tipoPaciente *current = list_first(pacientes);
  if (current == NULL)                                   //validacion.
  {
    printf("\n*~No hay pacientes en la lista de espera~*\n");
    return;
  }

  current = list_popFront(pacientes);                                   //se saca de la lista.
  printf("\nEl siguiente paciente en la lista de espera es:\n\n");      //mostrar paciente.
  printf("°**~===========================~**°\n");
  printf("Paciente: %s.\n", current -> name);
  printf("Edad    : %d años.\n", current -> age);
  printf("Sintomas: %s.\n", current -> simptoms);
  printf("°**~===========================~**°\n");
  printf("El paciente esta siendo atendido.\n");
}

//mostrar pacientes por prioridad deseada.
void pacientes_prioridad(List*pacientes)
{
  int prioridad;
  printf("\nIngrese la prioridad que desea ver\n");
  scanf("%d", &prioridad);
  if (prioridad < 1 || prioridad > 3)                //validar prioridad
  {
    printf("\n°--Por favor ingrese una prioridad válida--°\n");
    return;
  }
  
  tipoPaciente *current = list_first(pacientes);  //validar lista.
  if (current == NULL)
  {
    printf("\n*~No hay pacientes en lista*~\n");
  }

  if (current != NULL)  
  {
    printf("\nPaciente(s) con la prioridad deseada:\n\n");
    printf("°**~===========================~**°\n");
    while (current != NULL)                               //mostrar.
    {
      if (current -> priority == prioridad)
      {
        printf("- %s, edad %d.\n", current -> name, current -> age);
      }
      current = list_next(pacientes);
    }
    printf("°**~===========================~**°\n");
  }
}

int main() 
{
  char opcion;
  List *pacientes =
      list_create(); // puedes usar una lista para gestionar los pacientes

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
      asignar_prioridad(pacientes); // Lógica para asignar prioridad
      break;
    case '3':
      mostrar_lista_pacientes(pacientes);
      break;
    case '4':
      siguiente_paciente(pacientes);// Lógica para atender al siguiente paciente
      break;
    case '5':
      pacientes_prioridad(pacientes);// Lógica para mostrar pacientes por prioridad
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
