/**
 * Proyecto 2 de Sistemas de Operacion 1.
 *
 * La jerarquia de archivos que con la que se trabajara es la siguiente.
 *
 * .
 * ├── johto
 * │   ├── pokemon
 * │   │   ├── main
 * │   │   ├── one_time
 * │   │   └── recurring
 * │   └── trainers
 * │       ├── gym_leaders
 * │       ├── one_time
 * │       └── recurring
 * ├── kanto
 * │   ├── pokemon
 * │   │   ├── main
 * │   │   ├── one_time
 * │   │   └── recurring
 * │   └── trainers
 * │       ├── gym_leaders
 * │       ├── main
 * │       ├── one_time
 * │       └── recurring
 * └── orange_islands
 *     ├── pokemon
 *     │   ├── one_time
 *     │   └── recurring
 *     └── trainers
 *         ├── gym_leaders
 *         ├── main
 *         ├── one_time
 *         └── recurring
 * 
 * El programa se encarga de contar las apariciones de archivos que cumplan con distintas especificaciones.
 * El comando de ejecucion es el siguiente"
 *
 * ./fameChecker [-r <region>] [-s <species>] [-t <type>] [-c|--nocount] [-l|--list] [-s|--size] [name]
 *
 * En donde 
 *  -r              indica la region (johto, janto y orange_islands).
 *  -s              indica la especia a buscar (pokemon, trainers).
 *  -t              indica el tipo de aparicion (main, one_time, recurring).
 *  -c|--nocount    indica que debe aparecer el numero de archivos encontrado.
 *  -l|--list       indica que se debe mostrar una lista de los archivos entonctrados.
 *  -s|--size       indica que se debe mostrar el tamaño del archivo encontrado.
 *  [name]          permite restringir la busque a archivos ue comiencen con el nombre dado.
 *
 * Autores:
 *  Valeria Vera.
 *  Santiago Finamore.
 *  Daniel Robayo.
 */

#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_filters_files(char* type, int argc, char **argv);
int get_filters_output(char* type, int argc, char **argv);

int main(int argc, char **argv) {
    /* Variable para los filtros de la query a realizar*/
    char *region, *specie, *type, *name;
    int nocount, list, size;

    region = get_filters_files("-r", argc, argv);
    specie = get_filters_files("-s", argc, argv);
    type = get_filters_files("-t", argc, argv);

    nocount = get_filters_output("-c", argc, argv);
    list = get_filters_output("-l", argc, argv);
    size = get_filters_output("-s", argc, argv);

    /* en caso de que no se hayan escrito con -c, -l o -s se buscan con la otra opcion*/
    nocount = (nocount == 1) ? nocount : get_filters_output("--nocount", argc, argv);
    list = (list == 1) ? list : get_filters_output("--list", argc, argv);
    size = (size == 1) ? size : get_filters_output("--size", argc, argv);

    /* se verifica que la region ingresada sea valida*/
    if (!verify_input(region, specie, type)) {
        printf("Error in input command.\n");
        printf("Usage: ./fameChecker [-r <region>] [-s <species>] [-t <type>] [-c|--nocount] [-l|--list] [-s|--size] [name]\n");
        printf("<region> = [johto, kanto, orange_islands]\n")
        printf("<species> = [pokemon, trainers]\n");
        printf("<type> = [main, one_time, recurring, gym_leaders]\n");

        exit(1);
    }
}

char* get_filters_files(char* type, int argc, char **argv) {
    /*
     * Obtiene el argumento que correponde al tipo indicado.
     *
     * Los distintos tipos que se pueden recibir son
     *  -r
     *  -s
     *  -t
     *
     * La funcion devuelve el argumento correspondiente al tipo de argumento seleccionado
     * En caso de que no se encuentre un valor en el argumento, se devuelve NULL
     */

    int i = 1;

    while (i < argc) {
        if (!strcmp(type, argv[i])) {
            if (i == argc - 1) {
                return NULL; /* caso en donde el flag se encuentra al final */
            } else {
                return argv[i+1];
            }
        }

        i++;
    }

    return NULL;
}

int get_filters_output(char* type, int argc, char **argv) {
    /*
     * Obtiene el argumento que corresponde al tipo indicado.
     *
     * Los distintos tipos que se pueden recibir son
     *  -c|--nocount
     *  -l|--list
     *  -s|--size
     *
     * La funcion devuelve 1 en caso de encontrar el argumento seleccionado
     * 0 en caso de no encontrarlo
     */

    int i = 0;

    while (i < argc) {
        if (!strcmp(type, argv[i])) {
            return 1;
        }

        i++;
    }

    return 0;
}