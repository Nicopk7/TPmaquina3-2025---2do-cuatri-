#ifndef RAL_H_INCLUDED
#define RAL_H_INCLUDED
#define MAX_RAL 174//  110/0.93 ≈ 119
#include "Alumno.h"

 typedef struct
 {
    char estado; // V (virgen) L (libre) O (ocupada)
    Alumno dato;
 }Balde;

 typedef struct
 {
    Balde arr[MAX_RAL];
    int cantidad;
 }RAL;

void inicializarRAL(RAL *r){
    int i;
    for(i=0;i<MAX_RAL;i++){
        r->arr[i].estado='V';
    }
    r->cantidad=0;
}

int hashing(char* x, int M){
    int longitud, i;
    int contador = 0:
    longitud = strlen(x);
    for(i = 0; i < longitud; i++)
        contador+=((int)x[i] * (i+1));
    return (contador %M);
}

#include <string.h>

void localizar_ral(RAL ral, char codigo[], int *pos, int *exito, float *costo) {
    int i = hashing(codigo, MAX_RAL);  //posición inicial
    int j = 0;
    int flag = -1;//guarda la primera posición libre encontrada

    while (j < MAX_RAL) {

        // Si el balde está libre
        if ((ral.arr[i].estado == 'L') && (flag == -1)) {
            flag = i;
        }

        // Si el balde está virgen -> fin de búsqueda: no existe el alumno
        if (ral.arr[i].estado == 'V') {
            *costo += 1.0;
            *pos = (flag == -1) ? i : flag; // si no hubo libre, usar esta
            *exito = 0; // no encontrado
            return;
        }

        // Si el balde está ocupado, comparar código
        if ((ral.arr[i].estado == 'O') && (strcmp(ral.arr[i].dato.codigo, codigo) == 0)) {
            *costo += 1.0;
            *pos = i;
            *exito = 1; // encontrado
            return;
        }

        // Avanzar a la siguiente posición (sondeo lineal)
        *costo += 1.0;
        i = (i + 1) % MAX_RAL;
        j++;
    }

    // Si recorrimos todo el arreglo y no encontramos espacio virgen
    if (flag == -1)
        *exito = -1; // lleno, sin espacio
    else {
        *pos = flag;
        *exito = 0;  // no encontrado, pero hay lugar libre
    }
}


int alta_ral(RAL *r, Alumno a, int *exito, float *costo) {
    int pos = 0;

    // Verifica si la estructura está llena
    if (r->cantidad == MAX_RAL) {
        *exito = -1;  // sin espacio
        return *exito;
    }

    // Busca si ya existe el alumno o una posición para insertarlo
    localizar_ral(*r, a.codigo, &pos, exito, costo);

    if (*exito == 1) {
        *exito = 0; // ya existe el alumno
        return *exito;
    }

    if (*exito == -1) {
        *exito = -1; // sin espacio
        return *exito;
    }

    if (*exito == 0) {
        // Inserta el alumno
        r->arr[pos].dato = a;
        r->arr[pos].estado = 'O';
        r->cantidad++;
        *exito = 1; // alta exitosa
    }

    return *exito;
}


#include <string.h>

void baja_ral(RAL *r, Alumno a, int *exito, float *costo) {
    int pos = 0;

    // Buscar el alumno en la estructura
    localizar_ral(*r, a.codigo, &pos, exito, costo);

    if (*exito != 1) {
        *exito = -1; // no se encontró el alumno
        return;
    }

    // Verificar coincidencia completa (para confirmar baja)
    if (strcmp(r->arr[pos].dato.nombreapellido, a.nombreapellido) == 0 &&
        strcmp(r->arr[pos].dato.correo, a.correo) == 0 &&
        r->arr[pos].dato.nota == a.nota &&
        strcmp(r->arr[pos].dato.condicionFinal, a.condicionFinal) == 0) {

        // Marcar el balde como libre
        r->arr[pos].estado = 'L';
        r->cantidad--;
        *exito = 1; // baja exitosa
    } else {
        *exito = -1; // datos no coinciden para confirmar la baja
    }
}


#include <string.h>

Alumno evocar_ral(RAL *r, char codigo[], int *exito, float *costo) {
    int pos = 0;
    Alumno aux; // se devuelve vacío si no se encuentra
    float c = 0.0;

    localizar_ral(*r, codigo, &pos, exito, &c);

    if (*exito == 1) {
        *costo = c;
        return r->arr[pos].dato; // alumno encontrado
    } else {
        *costo = c;
        *exito = 0; // no encontrado
        // Inicializa aux vacío antes de devolverlo (por seguridad)
        strcpy(aux.codigo, "");
        strcpy(aux.nombreapellido, "");
        strcpy(aux.correo, "");
        aux.nota = 0;
        strcpy(aux.condicionFinal, "");
        return aux;
    }
}


#endif // RAL_H_INCLUDED
