#ifndef RS_H_INCLUDED
#define RS_H_INCLUDED
#include "Alumno.h"
#define fRS  85    //     130/1.53 ≈ 85
#include <ctype.h>


typedef struct balde{
	Alumno R;
	struct balde *next;
}balde;

typedef struct RS{
	balde *X[fRS];
	int CantR;
}RS;

Alumno vacio = {"","","",-1,""};

void init_RS(RS *lis) {
    for (int i = 0; i < fRS; i++) {
        lis->X[i] = NULL;
    }
    lis->CantR = 0;
}


void borrar_RS(RS *lis) {
    balde *tmp;
    for (int i = 0; i < fRS; i++) {
        while (lis->X[i] != NULL) {
            tmp = lis->X[i];
            lis->X[i] = lis->X[i]->next;
            free(tmp);
        }
    }
    lis->CantR = 0;
}

int hashingRS(char* x, int M){
    int longitud, i;
    int contador = 0;
    longitud = strlen(x);
    for(i = 0; i < longitud; i++)
        contador+=((int)x[i] * (i+1));
    return (contador %M);
}

int strcompiRS(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) return c1 - c2;
        s1++; s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}


int localizarRS(RS *l, char *codigo, int *exito, float *costo, int *h) {
    *h = hashingRS(codigo, fRS);
    *costo = 0.0;

    balde *act = l->X[*h];
    while (act != NULL && strcmp(act->R.codigo, codigo) != 0) {
        (*costo)++;
        act = act->next;
    }

    if (act != NULL && strcmp(act->R.codigo, codigo) == 0) {
        (*costo)++;
        *exito = 1;
    } else {
        *exito = 0;
    }

    return *exito;
}

int altaRS(RS *l, Alumno a, int *exito, float *costo) {
    int i;
    float c = 0.0;

    localizarRS(l, a.codigo, exito, &c, &i);

    // Ya existe el alumno
    if (*exito == 1) {
        *exito = 0;
        *costo = c;
        return *exito;
    }

    // Crear nuevo nodo
    balde *nuevo = (balde *)malloc(sizeof(balde));
    if (nuevo == NULL) {
        *exito = -1;  // sin memoria
        return *exito;
    }

    nuevo->R = a;
    nuevo->next = l->X[i];  // inserta al inicio de la lista
    l->X[i] = nuevo;

    l->CantR++;
    *exito = 1;
    *costo = c;
    return *exito;
}

int baja_RS(RS *l, Alumno a, int *exito, float *costo) {
    int pos;
    *costo = 0.0f;
    *exito = 0;

    pos = hashingRS(a.codigo, fRS);

    balde *cur = l->X[pos];
    balde *ant = NULL;

    while (cur != NULL && strcompiRS(cur->R.codigo, a.codigo) != 0) {
        (*costo) += 1.0f;
        ant = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        *exito = 0; // no encontrado
        return *exito;
    }

    /* comparar datos para confirmar baja */
    if ( strcompiRS(cur->R.nombreapellido, a.nombreapellido) == 0 &&
         strcompiRS(cur->R.correo, a.correo) == 0 &&
         cur->R.nota == a.nota &&
         strcompiRS(cur->R.condicionFinal, a.condicionFinal) == 0 ) {

        /* remover nodo de la lista */
        if (ant == NULL) {
            /* era el primero */
            l->X[pos] = cur->next;
        } else {
            ant->next = cur->next;
        }

        free(cur);
        l->CantR--;
        *exito = 1; /* baja exitosa */
    } else {
        /* datos no coinciden */
        *exito = 0;
    }

    return *exito;
}

Alumno evocar_rs(RS l, const char codigo[], int *exito, float *costo) {
    Alumno aux = vacio;
    int pos;
    *costo = 0.0f;
    *exito = 0;

    pos = hashingRS(codigo, fRS);
    balde *cur = l.X[pos];

    while (cur != NULL && strcompiRS(cur->R.codigo, codigo) != 0) {
        (*costo) += 1.0f;
        cur = cur->next;
    }

    if (cur != NULL) {
        (*costo) += 1.0f;
        aux = cur->R;
        *exito = 1; //Encontrado
    } else {
        *exito = 0; //no encontrado
    }

    return aux;
}

void mostrar_rs(RS l) {
    for (int i = 0; i < fRS; i++) {
        balde *temp = l.X[i];
        printf("\n--- Balde %d ---\n", i);
        if (temp == NULL) {
            printf("Lista vacía.\n");
        } else {
            int c = 1;
            while (temp != NULL) {
                printf("Elemento #%d del bucket %d\n", c++, i);
                printf("Código: %s\n", temp->R.codigo);
                printf("Nombre y Apellido: %s\n", temp->R.nombreapellido);
                printf("Correo: %s\n", temp->R.correo);
                printf("Nota: %d\n", temp->R.nota);
                printf("Condición final: %s\n", temp->R.condicionFinal);
                printf("-------------------------\n");
                temp = temp->next;
            }
        }
    }
}

#endif // RS_H_INCLUDED
