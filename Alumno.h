#ifndef ALUMNO_H_INCLUDED
#define ALUMNO_H_INCLUDED

typedef struct{
    char codigo[8];
    char nombreapellido[80];
    char correo[23];
    int nota;
    char condicionFinal[11];
}Alumno;

#endif // ALUMNO_H_INCLUDED
