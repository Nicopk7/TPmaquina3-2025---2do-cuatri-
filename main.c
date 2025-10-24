//GRUPO 34
//PEREYRA KALUZA NICOLÁS
//VOTTERO ELISEO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "Alumno.h"
#include "RAL.h"
#include "RAC.h"
#include "RS.h"
#define separador printf("____________________________________________________________________________________________ \n")
#define borrador system("cls")
#define pausa system("pause")

/*                                                                  COMPARACION Y ANALISIS

    El Rebalse separado es claramente la opcion mas economica en todas las comparaciones. Mas eficiente si se busca minimizar costos de evocacion.
    El Rebalse Abierto Lineal es la estructura mas costosa.
    El Rebalse Abierto Cuadratico es balanceado entre costos y una estructura abierta, pero sin los costos tan altos del RAL.

                                                                    COMPARACION DE ESTRUCTURAS
                                                                COSTOS          RS      RAL     RAC
                                                            _____________________________________________________
                                                                MAX.EVOEX       5,00    81,00   14,00
                                                                MED.EVOEX       1,69    16,47   4,57
                                                                MAX.EVONOEX     4,00    94,00   15,00
                                                                MED.EVONOEX     1,80    71,44   11,82
                                                            -----------------------------------------------------

*/


typedef struct {
    float max_evo_succ;
    float max_evo_fail;
    float med_evo_succ;
    float med_evo_fail;
    float cant_evo_succ;
    float cant_evo_fail;
}v_costos;

void init_costos(v_costos *c){
    c->max_evo_fail = 0;
    c->max_evo_succ = 0;
    c->med_evo_fail = 0;
    c->med_evo_succ = 0;
    c->cant_evo_succ = 0;
    c->cant_evo_fail = 0;
}

int main(void)
{
    setlocale(LC_ALL, "");
    int opcion = 0, opcion_muestra = 0, operacion = 0, exito = 0;
    float costo = 0.0f;
    v_costos rs_c, ral_c, rac_c;
    init_costos(&rs_c);
    init_costos(&ral_c);
    init_costos(&rac_c);
    Alumno aux;
    RS r;
    RAL lineal;
    RAC cuadratico;

    inicializarRAL(&lineal);
    inicializarRAC(&cuadratico);
    init_RS(&r);

    do {
        printf("******* Menu Principal ******\n");
        printf("1.Mostrar Estructuras\n");
        printf("2. Comparar Estructuras\n");
        printf("3. Salir\n");
        printf("Elija una opcion:\n");
        if (scanf("%d", &opcion) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            opcion = 0;
        }

        switch (opcion) {
            case 1: {
                do {
                    borrador;
                    separador;
                    printf("\tElija la Estructura a mostrar\n");
                    printf("1.Rebalse Abierto Lineal\n");
                    printf("2.Rebalse Abierto Cuadratico\n");
                    printf("3.Rebalse separado\n");
                    printf("4.Volver\n");
                    separador;
                    fflush(stdin);
                    if (scanf("%d", &opcion_muestra) != 1) {
                        int ch;
                        while ((ch = getchar()) != '\n' && ch != EOF) {}
                        opcion_muestra = 0;
                    }

                    switch (opcion_muestra) {
                        case 1: {
                            printf("Rebalse Abierto Lineal\n");
                            mostrar_ral(lineal);
                            pausa;
                            break;
                        }
                        case 2: {
                            printf("Rebalse Abierto Cuadratico \n");
                            mostrar_rac(cuadratico);
                            pausa;
                            break;
                        }
                        case 3: {
                            printf("Rebalse Separado\n");
                            mostrar_rs(&r);
                            pausa;
                            break;
                        }
                        case 4:
                        default:
                            break;
                    }
                } while (opcion_muestra != 4);
                break;
            } // fin case 1

            case 2: {

                FILE *fp;
                Alumno aux;
                int operacion;
                int exito;
                init_costos(&rs_c);
                init_costos(&ral_c);
                init_costos(&rac_c);
                borrar_RS(&r);
                init_RS(&r);
                inicializarRAC(&cuadratico);
                inicializarRAL(&lineal);

                fp = fopen("Operaciones-Alumnos.txt", "r");
                if (fp == NULL) {
                    printf("\tNo se pudo abrir el archivo 'Operaciones-Alumnos.txt'\n");
                    system("pause");
                    break;
                }


                while (fscanf(fp, "%d", &operacion) == 1) {

                    fscanf(fp, " %[^\n]", aux.codigo);
                    strupr(aux.codigo);

                    switch (operacion) {
                        case 1: { // ALTA
                            fscanf(fp, " %[^\n]", aux.nombreapellido);
                            fscanf(fp, " %[^\n]", aux.correo);
                            fscanf(fp, "%d", &aux.nota);
                            fscanf(fp, " %[^\n]", aux.condicionFinal);

                            strupr(aux.nombreapellido);
                            strupr(aux.correo);
                            strupr(aux.condicionFinal);

                            alta_ral(&lineal, aux, &exito, &costo);
                            alta_rac(&cuadratico, aux, &exito, &costo);
                            altaRS(&r, aux, &exito, &costo);
                            break;
                        }
                        case 2: { // BAJA
                            fscanf(fp, " %[^\n]", aux.nombreapellido);
                            fscanf(fp, " %[^\n]", aux.correo);
                            fscanf(fp, "%d", &aux.nota);
                            fscanf(fp, " %[^\n]", aux.condicionFinal);

                            strupr(aux.nombreapellido);
                            strupr(aux.correo);
                            strupr(aux.condicionFinal);

                            baja_ral(&lineal, aux, &exito, &costo);
                            baja_rac(&cuadratico, aux, &exito, &costo);
                            baja_RS(&r, aux, &exito, &costo);
                            break;
                        }
                        case 3: {

                            costo = 0.0f;
                            exito = 0;
                            evocar_ral(&lineal, aux.codigo, &exito, &costo);
                            if (exito == 1) {
                                ral_c.cant_evo_succ++;
                                ral_c.med_evo_succ += costo;
                                if (costo > ral_c.max_evo_succ) ral_c.max_evo_succ = costo;
                            } else {
                                ral_c.cant_evo_fail++;
                                ral_c.med_evo_fail += costo;
                                if (costo > ral_c.max_evo_fail) ral_c.max_evo_fail = costo;
                            }

                            costo = 0.0f;
                            exito = 0;
                            evocar_rac(&cuadratico, aux.codigo, &exito, &costo);
                            if (exito == 1) {
                                rac_c.cant_evo_succ++;
                                rac_c.med_evo_succ += costo;
                                if (costo > rac_c.max_evo_succ) rac_c.max_evo_succ = costo;
                            } else {
                                rac_c.cant_evo_fail++;
                                rac_c.med_evo_fail += costo;
                                if (costo > rac_c.max_evo_fail) rac_c.max_evo_fail = costo;
                            }

                            costo = 0.0f;
                            exito = 0;
                            evocar_rs(&r, aux.codigo, &exito, &costo);
                            if (exito == 1) {
                                rs_c.cant_evo_succ++;
                                rs_c.med_evo_succ += costo;
                                if (costo > rs_c.max_evo_succ) rs_c.max_evo_succ = costo;
                            } else {
                                rs_c.cant_evo_fail++;
                                rs_c.med_evo_fail += costo;
                                if (costo > rs_c.max_evo_fail) rs_c.max_evo_fail = costo;
                            }
                            break;
                        }
                    } // fin switch
                } // fin while

                fclose(fp);

                printf("Lineal: %d\n", lineal.cantidad);
                printf("Cuadratico: %d\n", cuadratico.cantidad);
                printf("Separado: %d\n\n", r.CantR);

                float med_ral_succ = (ral_c.cant_evo_succ > 0) ? (ral_c.med_evo_succ / ral_c.cant_evo_succ) : 0.0f;
                float med_ral_fail = (ral_c.cant_evo_fail > 0) ? (ral_c.med_evo_fail / ral_c.cant_evo_fail) : 0.0f;

                float med_rac_succ = (rac_c.cant_evo_succ > 0) ? (rac_c.med_evo_succ / rac_c.cant_evo_succ) : 0.0f;
                float med_rac_fail = (rac_c.cant_evo_fail > 0) ? (rac_c.med_evo_fail / rac_c.cant_evo_fail) : 0.0f;

                float med_rs_succ = (rs_c.cant_evo_succ > 0) ? (rs_c.med_evo_succ / rs_c.cant_evo_succ) : 0.0f;
                float med_rs_fail = (rs_c.cant_evo_fail > 0) ? (rs_c.med_evo_fail / rs_c.cant_evo_fail) : 0.0f;

                printf("\t\tCOMPARACION DE ESTRUCTURAS\n");
                printf("+----------------+-----+------------+------------+------------+\n");
                printf("|                |     | %-10s | %-10s | %-10s |\n", "RAL", "RAC", "RS");
                printf("+----------------+-----+------------+------------+------------+\n");
                printf("| EVOC. EXITO    | MAX | %10.2f | %10.2f | %10.2f |\n", ral_c.max_evo_succ, rac_c.max_evo_succ, rs_c.max_evo_succ);
                printf("|                | MED | %10.2f | %10.2f | %10.2f |\n", med_ral_succ, med_rac_succ, med_rs_succ);
                printf("+----------------+-----+------------+------------+------------+\n");
                printf("| EVOC. NO EXITO | MAX | %10.2f | %10.2f | %10.2f |\n", ral_c.max_evo_fail, rac_c.max_evo_fail, rs_c.max_evo_fail);
                printf("|                | MED | %10.2f | %10.2f | %10.2f |\n", med_ral_fail, med_rac_fail, med_rs_fail);
                printf("+----------------+-----+------------+------------+------------+\n");

                system("pause");
                break;
            }
        case 3:
                printf("Saliendo...\n");
                break;

            default:
                printf("Opcion no valida. Intente de nuevo.\n");
                break;
        } /* fin switch(opcion) */
        }while (opcion != 3);

    return 0;
}

