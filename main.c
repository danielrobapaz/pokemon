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
 *  -r              indica la region (johto, kanto y orange_islands).
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

#define _DEFAULT_SOURCE /* macro añadido para utilizar lstat*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

char* get_filters_files(char* type, int argc, char **argv);
int get_filters_output(char* type, int argc, char **argv);
int traverse_directory(char* dirname, int indent,
    char* r, char* sp, char* t, char *name,
    int c, int l, int sz,
    int r_ch, int sp_ch, int t_ch, int *counter);
int verify_input(char *region, char *species, char *type);
int is_directory(char* path);
int is_reg_file(char* path);
int calc_size(char* path);
void print_paths(int l, int sz, char* d_name, int size);

int main(int argc, char **argv) {
    /* Variable para los filtros de la query a realizar*/
    char *region, *specie, *type, *name;
    int nocount, list, size, count, arg_count;
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
    if (verify_input(region, specie, type)) {
        printf("Error in input command.\n");
        printf("Usage: ./fameChecker [-r <region>] [-s <species>] [-t <type>] [-c|--nocount] [-l|--list] [-s|--size] [name]\n");
        printf("<region> = [johto, kanto, orange_islands]\n");
        printf("<species> = [pokemon, trainers]\n");
        printf("<type> = [main, one_time, recurring, gym_leaders]\n");

        exit(1);
    }

    /*Contabilizacion de argumentos*/
    arg_count = 0;
    arg_count += 2*(strcmp(region, "") != 0);
    arg_count += 2*(strcmp(specie, "") != 0);
    arg_count += 2*(strcmp(type, "") != 0);
    arg_count += nocount + list + size;

    /*Se compara con la cantida de argumentos ingresados en consola*/
    if (arg_count < argc - 1) {
        /*Si sobra algún argumento es que el usuario insertó un nombre a buscar*/
        name = argv[argc-1];
    } else {
        name = ""; /*Si no se ingresó nada se deja un string vacío*/
    }

    count = 0;
    
    traverse_directory("./directorio", 1, region, specie, type, name, nocount, list, size, 0, 0, 0, &count);
    !nocount && printf("\nNumber of files matching criteria: %d\n", count);

    return 0;
}

int verify_input(char *region, char *species, char *type) {
    /**
     * Funcion que recibe el input ingresado al programa y verifica que sea valido para el
     * programa. En caso de que el input ingresado sea valido se retorna 0 y se retorna 1 
     * en caso contrario.
     * 
     * Entrada:
     *      - input: apuntador al arreglo de strings que representa el input
     * Salida: 0 si el input ingresado es valido y 1 en caso contrario
    */

    if (strcmp(region, "kanto") && strcmp(region, "johto") && strcmp(region, "orange_islands") && strcmp(region, "")) {
        return 1;
    }else if (strcmp(species, "pokemon") && strcmp(species, "trainers") && strcmp(species, "")) {
        return 1;
    }else if (strcmp(type, "main") && strcmp(type, "one_time") && strcmp(type, "recurring") && strcmp(type, "gym_leaders") && strcmp(type, "")) {
        return 1;
    }
    return 0;
}

int is_directory(char* path) {
    /**
     * Permite saber si un archivo es un directorio.
     * Entrada:
     *     - path: apuntador a la ruta del archivo.
     * Salida: 1 si es un directorio, 0 si no lo es.
    */

    struct stat st;
    if (stat(path, &st) != 0) return 0;
   
    return S_ISDIR(st.st_mode);
}

int is_reg_file(char* path) {
    /**
     * Nos permite saber si un archivo es regular.
     * Entrada:
     *    - path: ruta del archivo.
     * Salida: 1 si es un archivo regular, 0 si no lo es.
    */
    struct stat st;
    if (stat(path, &st) != 0) return -1;

    return S_ISREG(st.st_mode);
}

int calc_size(char* path) {
    /**
     * Nos permite calcular el tamaño en bytes
     * de un archivo.
     * Entrada:
     *   - path: ruta del archivo.
     * Salida: tamaño en bytes del archivo.
    */

    struct stat st;

    lstat(path, &st);
    return st.st_size;
}

char* get_filters_files(char* type, int argc, char **argv) {
    /**
     * Obtiene el argumento que corresponde al tipo indicado.
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
                return argv[i + 1];
            }
        }

        i++;
    }

    return "";
}

int get_filters_output(char* type, int argc, char **argv) {
    /**
     * Obtiene el argumento que corresponde al tipo indicado.
     *
     * Los distintos tipos que se pueden recibir son
     *  -c|--nocount
     *  -l|--list
     *  -s|--size
     *
     * La funcion devuelve 1 en caso de encontrar el argumento seleccionado
     * 0 en caso de no encontrarlo.
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

void print_paths(int l, int sz, char* d_name, int size) {
    if (l) {
        printf("%s", d_name);
        if (sz) {
            printf(": %d kB", size);
        }
        printf("\n");
    }

    if (!l && sz) {
        printf("- %d kB\n", size);
    }
}

int traverse_directory(char* dirname, int indent,
    char* r, char* sp, char* t, char *name,
    int c, int l, int sz,
    int r_ch, int sp_ch, int t_ch, int *counter) {

    /**
     * Entrada:
     *  -dirname: nombre del directorio ppal que se traversa.
     *  -indent: valor de indentación que crece con cada hoja
     *           nueva del árbol visitado.
     *  -r: flag "region" en caso de existir.
     *  -sp: flag "specie" en caso de existir.
     *  -t: flag "type" en caso de existir.
     *  -c: flag "no count" en caso de existir.
     *  -l: flag "list" en caso de existir.
     *  -sz: flag "size" en caso de existir.
     *  -r_ch: flag que representa si ya fue chequeada la región.
     *  -sp_ch: flag que representa si ya fue chequeada la especie.
     *  -t_ch: flag que representa si ya fue chequeado el tipo.
    */
    DIR* dir = opendir(dirname);
    struct dirent* entry;

    if (!dir) return -1;

    while ((entry = readdir(dir))) {
        char* d_name = entry->d_name;

        if (strcmp(d_name, ".") == 0 || strcmp(d_name, "..") == 0) {
            continue;
        } else {
            char* path = malloc(sizeof(char) *               
                (strlen(d_name) + strlen(dirname) + 4));
            int new_indent = 1 + indent;

            if (!path) return -1;

            sprintf(path, "%s/%s", dirname, d_name);

            /* Se traversan los directorios en el orden: región -> especie -> tipo. */
            if (is_directory(path)) {
                if (!r_ch) {
                    if (!strcmp(d_name, r) || !strcmp(r, "")) {
                        traverse_directory(path, new_indent, r, sp, t, name, c, l, sz, 1, 0, 0, counter);
                    }
                }
                if (!sp_ch) {
                    if (strstr(path, r)) {
                        if (!strcmp(d_name, sp) || !strcmp(sp, "")) {
                            traverse_directory(path, new_indent, r, sp, t, name, c, l, sz, 1, 1, 0, counter);
                        }
                    }
                }
                if (!t_ch) {
                    if (strstr(path, sp)) {
                        if (!strcmp(d_name, t) || !strcmp(t, "")) {
                            traverse_directory(path, new_indent, r, sp, t, name, c, l, sz, 1, 1, 1, counter);
                        }
                    }
                }
            }

            if (is_reg_file(path) && t_ch) {
                if (strcmp(name, "")) {
                    if (strstr(d_name, name) != NULL) {
                        *counter += 1;
                        if (l) {
                            print_paths(l, sz, d_name, calc_size(path));
                        }
                    }
                }else {
                    *counter += 1;
                    if (l) {
                        print_paths(l, sz, d_name, calc_size(path));
                    }
                }
            }
        }
    }

    closedir(dir);

    return 0;
}