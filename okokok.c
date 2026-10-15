// operazioni matematiche con i puntatori
// tutti gli altri linguaggi, a basso livello lavorano appoggiandosi con il sistema sizeof, 
// #include <stdio.h>

// int main(void) {
//     int numero;
//     int insieme[5];
    
//     numero = 5;
//     insieme[3] = 5;
    
//     Stampa valore, indirizzo e dimensione di numero
//     printf("numero: %d\n", numero);
//     printf(">> Indirizzo numero: %p\n", (void *)&numero);
//     printf("b: %zu byte\n\n", sizeof(numero));
    
//     Stampa indirizzi e dimensione dell'array insieme
//     printf("Indirizzo base (insieme): %p\n", (void *)insieme);
//     printf(">> Indirizzo array (&insieme): %p\n", (void *)&insieme);
//     printf("b: %zu byte\n\n", sizeof(insieme));
    
//     Stampa valore, indirizzo e dimensione del singolo elemento
//     printf("insieme[3]: %d\n", insieme[3]);
//     printf(">> Indirizzo insieme[3]: %p\n", (void *)&insieme[3]);
//     printf("b: %zu byte\n", sizeof(insieme[3]));
    
//     return 0;
// }

// #include <stdio.h>
// #define N 8

// int main(void)
// {
//     int v[N] = {4, 8, 15, 16, 23, 42, 8, 1};
//     int x, pos = -1;    //-1 non ancora trovato

//     printf("numero da cercare: ");
//     scanf("%d", &x);
    
//     for (int i = 0; i < N && pos == -1; i++)
//         if (v[i] == x)
//             pos = i;    //si ferma al primo trovato
        
//     if (pos != -1) printf("Trovato in posizione %d\n", pos);
//     else           printf("%d non c'e\n", x);
//     return 0;
// }

#include <stdio.h>
#include <stdbool.h>

// Verifica se subArray e sottoinsieme di mainArray
bool eSottoinsieme(const int subArray[], int dimSub, const int mainArray[], int dimMain) {
    for (int i = 0; i < dimSub; i++) {
        bool trovato = false;

        for (int j = 0; j < dimMain; j++) {
            if (subArray[i] == mainArray[j]) {
                trovato = true;
                break; // Elemento trovato: passa all'elemento successivo di subArray
            }
        }

        // Se un solo elemento non viene trovato, NON e un sottoinsieme
        if (!trovato) {
            return false;
        }
    }

    return true; // Tutti gli elementi sono stati trovati
}

int main(void) {
    int insiemeB[] = {10, 20, 30, 40, 50, 60};
    int insiemeA1[] = {20, 50, 30}; // Sottoinsieme valido
    int insiemeA2[] = {20, 70, 30}; // Falso (70 non c'e)

    int dimB = sizeof(insiemeB) / sizeof(insiemeB[0]);
    int dimA1 = sizeof(insiemeA1) / sizeof(insiemeA1[0]);
    int dimA2 = sizeof(insiemeA2) / sizeof(insiemeA2[0]);

    printf("A1 e sottoinsieme di B? %s\n", 
           eSottoinsieme(insiemeA1, dimA1, insiemeB, dimB) ? "SI" : "NO");

    printf("A2 e sottoinsieme di B? %s\n", 
           eSottoinsieme(insiemeA2, dimA2, insiemeB, dimB) ? "SI" : "NO");

    return 0;
}