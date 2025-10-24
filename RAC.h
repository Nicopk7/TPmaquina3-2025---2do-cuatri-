#ifndef RAC_H_INCLUDED
#define RAC_H_INCLUDED
#define MAX_RAC 163 // 130/0.81 ≈ 163
#include "Alumno.h"
#include <string.h>
#include <ctype.h>

typedef struct {
    char estado; // V (virgen) L (libre) O (ocupada)
    Alumno dato;
}BaldeRAC;

typedef struct {
    BaldeRAC arr[MAX_RAC];
    int cantidad;
}RAC;

void inicializarRAC(RAC *rac){
    int i;
    for(i=0;i<MAX_RAC;i++){
        (*rac).arr[i].estado='V';
    }
    (*rac).cantidad=0;
}

int hashingRAC(char* x, int M){
    int longitud, i;
    int contador = 0;
    longitud = strlen(x);
    for(i = 0; i < longitud; i++)
        contador+=((int)x[i] * (i+1));
    return (contador %M);
}

int strcompiRAC(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) return c1 - c2;
        s1++; s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

void localizarRAC(RAC *rac, char codigo[], int *pos, int *exito, float *costo) {
    int pos_actual = hashingRAC(codigo, MAX_RAC);
    int flag = -1;
    int intentos = 0;
    int paso = 0;
    *costo = 0.0;

    while (intentos < MAX_RAC && rac->arr[pos_actual].estado != 'V' &&
           (rac->arr[pos_actual].estado != 'O' || strcmp(rac->arr[pos_actual].dato.codigo, codigo) != 0))
    {
        if (rac->arr[pos_actual].estado == 'L' && flag == -1) {
            flag = pos_actual;
        }
        (*costo)++;
        intentos++;
        paso++;
        pos_actual = (pos_actual + paso) % MAX_RAC;
    }
    (*costo)++;

    if (intentos < MAX_RAC && rac->arr[pos_actual].estado == 'O' && strcompiRAC(rac->arr[pos_actual].dato.codigo, codigo) == 0) {

        *pos = pos_actual;
        *exito = 1; // Encontrado
    } else {

        *pos = (flag == -1) ? pos_actual : flag;
        *exito = 0; // No encontrado
    }
}

int alta_rac(RAC *rac, Alumno a, int *exito, float *costo) {
    int pos;
    float costoaux = 0.0;

    // Tabla llena
    if (rac->cantidad == MAX_RAC) {
        *exito = -1;
        return *exito;
    }

    localizarRAC(rac, a.codigo, &pos, exito, &costoaux);

    if (*exito == 1) {
        *exito = 0;  // Ya existe
        return *exito;
    }

    if (*exito == -1) {
        *exito = -1; // No hay espacio
        return *exito;
    }


    if (*exito == 0) {
        rac->arr[pos].dato = a;
        rac->arr[pos].estado = 'O';
        rac->cantidad++;
        *exito = 1; // Alta exitosa
    }

    return *exito;
}

void baja_rac(RAC *rac, Alumno a, int *exito, float *costo) {
    int pos;
    float costoaux = 0.0;

    localizarRAC(rac, a.codigo, &pos, exito, &costoaux);

    if (*exito == 1 && rac->arr[pos].estado == 'O') {
        if (strcompiRAC(rac->arr[pos].dato.nombreapellido, a.nombreapellido) == 0 &&
            strcompiRAC(rac->arr[pos].dato.correo, a.correo) == 0 &&
            rac->arr[pos].dato.nota == a.nota &&
            strcompiRAC(rac->arr[pos].dato.condicionFinal, a.condicionFinal) == 0) {

            rac->arr[pos].estado = 'L';
            rac->cantidad--;
            *exito = 1; // Baja exitosa
        } else {
            *exito = 0; // No coinciden los datos
        }
    } else {
        *exito = -1; // No se encontró el alumno
    }
}


Alumno evocar_rac(RAC *rac, char codigo[], int *exito, float *costo) {
    Alumno aux;
    int pos;
    *costo = 0.0;

    localizarRAC(rac, codigo, &pos, exito, costo);

    if (*exito == 1) {
        *exito = 1;
        return rac->arr[pos].dato;
    } else {
        *exito = 0;
        memset(&aux, 0, sizeof(Alumno));
        return aux;
    }
}

void mostrar_rac(RAC r){
int i,j=0;
for(i=0;i<MAX_RAC;i++){
    if(r.arr[i].estado=='O'){
        j++;
        printf("Celda numero: %d\n",i);
        printf("codigo: %s \n", r.arr[i].dato.codigo);
        printf("Nombre y Apellido: %s \n", r.arr[i].dato.nombreapellido);
        printf("Correo electrónico: %s \n", r.arr[i].dato.correo);
        printf("Nota: %d \n", r.arr[i].dato.nota);
        printf("Condicion Final: %s \n", r.arr[i].dato.condicionFinal);
        printf("*********************************\n");
        }
    if(r.arr[i].estado=='V'){
        printf("*********************************\n");
        printf("\n\tLa celda numero %d es virgen(V)\n",i);
        printf("*********************************\n");
    }

    if(r.arr[i].estado=='L'){
         printf("*********************************\n");
         printf("\n\tLa celda numero %d es libre(L)\n",i);
         printf("*********************************\n");}
    if(i%5 == 0){
        fflush(stdin);
        printf("Presione ENTER para la página siguiente...\n");
        getchar();
        }
    }
}


#endif // RAC_H_INCLUDED
