#include <stdio.h>

/*
 * =====================================================================
 *  VENDING MACHINE - LIVELLO 1: funzionamento di base
 * =====================================================================
 *
 *  Il programma simula un distributore di bevande:
 *    1) mostra il menu con le bevande e i prezzi;
 *    2) controlla che la bevanda scelta non sia esaurita;
 *    3) fa inserire le monete finche' l'importo non basta;
 *    4) eroga la bevanda, toglie una cialda dal magazzino e calcola il resto;
 *    5) permette di annullare e riavere le monete inserite.
 *
 *  Costrutti usati: variabili int, if, while, switch, scanf, printf.
 *  Niente array e nessuna funzione oltre a main.
 *
 *  PERCHE' I CENTESIMI?
 *  Tutti gli importi sono numeri interi in centesimi: 60 vuol dire 0,60 EUR.
 *  I numeri con la virgola (float, double) non rappresentano esattamente
 *  valori come 0,10: sommando tante monete si accumulano piccoli errori.
 *  Con gli interi i conti sono sempre esatti.
 *
 *  In questo livello si suppone che la macchina abbia sempre le monete
 *  per dare il resto: la gestione della cassa arriva nel livello 2.
 * =====================================================================
 */

int main(void)
{
    /* -----------------------------------------------------------------
     * MAGAZZINO
     * Una variabile per ogni bevanda: conta quante cialde sono rimaste.
     * Senza array servono variabili separate, con nomi chiari.
     * ----------------------------------------------------------------- */
    int cialde_caffe = 5, cialde_cappuccino = 5, cialde_the = 5, cialde_latte = 5;

    /* -----------------------------------------------------------------
     * VARIABILI DI LAVORO
     * ----------------------------------------------------------------- */
    int attivo = 1;            /* 1 = la macchina e' accesa, 0 = spenta: controlla il ciclo principale */
    int letti;                 /* valore restituito da scanf: quanti dati ha letto correttamente */
    int scelta;                /* numero della bevanda scelta nel menu */
    int prezzo;                /* prezzo della bevanda scelta, in centesimi */
    int cialde_disponibili;    /* cialde rimaste della bevanda scelta */
    int moneta;                /* valore dell'ultima moneta inserita */
    int inserito;              /* totale inserito finora, in centesimi */
    int annullato;             /* 1 se il cliente ha annullato l'acquisto */
    int resto;                 /* importo da restituire, in centesimi */

    /* Quante monete di ogni taglio ha inserito il cliente in questo acquisto.
     * Servono per restituire ESATTAMENTE le stesse monete se annulla. */
    int inseriti10, inseriti20, inseriti50, inseriti100, inseriti200;

    /* =================================================================
     * CICLO PRINCIPALE
     * Ogni giro del while e' un "turno" della macchina: mostra il menu,
     * legge la scelta e, se e' una bevanda, gestisce l'acquisto.
     * Il ciclo termina quando attivo diventa 0 (scelta "Spegni").
     * ================================================================= */
    while (attivo) {

        /* --- Stampa del menu ---
         * Accanto alle bevande senza cialde compare [ESAURITO].
         * Stampiamo la riga a pezzi: prima nome e prezzo, poi (solo se
         * serve) la scritta, infine l'a capo all'inizio della riga dopo. */
        printf("\n========= VENDING MACHINE =========\n");
        printf("1) Caffe'      0,60 EUR");
        if (cialde_caffe == 0) printf("  [ESAURITO]");
        printf("\n2) Cappuccino  0,90 EUR");
        if (cialde_cappuccino == 0) printf("  [ESAURITO]");
        printf("\n3) The         0,50 EUR");
        if (cialde_the == 0) printf("  [ESAURITO]");
        printf("\n4) Latte       0,40 EUR");
        if (cialde_latte == 0) printf("  [ESAURITO]");
        printf("\n0) Spegni\n");
        printf("Scelta: ");

        /* --- Lettura sicura della scelta ---
         * scanf restituisce quanti valori e' riuscito a leggere:
         *   1   -> ha letto un numero: tutto bene;
         *   0   -> l'utente ha scritto del testo (es. "abc"): il testo resta
         *          nel buffer e, se non lo togliamo, scanf fallirebbe per
         *          sempre. scanf("%*s") legge una parola e la scarta
         *          (l'asterisco vuol dire "leggi ma non salvare");
         *   EOF -> l'input e' finito (es. file terminato): spegniamo. */
        letti = scanf("%d", &scelta);
        if (letti == EOF) {
            scelta = 0;
        } else if (letti != 1) {
            scanf("%*s");
            scelta = -1;               /* valore non valido: finira' nel default */
        }

        /* --- Interpretazione della scelta ---
         * Per le bevande impostiamo prezzo e cialde disponibili.
         * prezzo rimane 0 se la scelta non e' una bevanda: lo usiamo
         * subito dopo per capire se c'e' un acquisto da gestire. */
        prezzo = 0;
        cialde_disponibili = 0;

        switch (scelta) {
        case 1: prezzo = 60; cialde_disponibili = cialde_caffe;      break;
        case 2: prezzo = 90; cialde_disponibili = cialde_cappuccino; break;
        case 3: prezzo = 50; cialde_disponibili = cialde_the;        break;
        case 4: prezzo = 40; cialde_disponibili = cialde_latte;      break;
        case 0:
            attivo = 0;                /* il while terminera' alla prossima verifica */
            printf("Macchina spenta. Arrivederci!\n");
            break;
        default:
            printf("Scelta non valida.\n");
        }

        /* Nessuna bevanda da vendere: "continue" salta il resto del corpo
         * del ciclo e torna all'inizio del while (cioe' al menu). */
        if (prezzo == 0)
            continue;

        /* La bevanda e' finita: rifiutiamo l'acquisto PRIMA di chiedere soldi. */
        if (cialde_disponibili == 0) {
            printf("Spiacenti, bevanda esaurita.\n");
            continue;
        }

        /* =============================================================
         * INSERIMENTO MONETE
         * Si azzerano i contatori di questo acquisto, poi si chiedono
         * monete finche' l'importo inserito non raggiunge il prezzo
         * oppure il cliente annulla (moneta 0).
         * ============================================================= */
        inseriti10 = 0; inseriti20 = 0; inseriti50 = 0; inseriti100 = 0; inseriti200 = 0;
        inserito = 0;
        annullato = 0;

        while (inserito < prezzo && !annullato) {
            /* %02d stampa almeno due cifre, con lo zero davanti:
             * 5 centesimi -> "05", cosi' 0,05 non diventa 0,5 */
            printf("Prezzo %d,%02d - inserito %d,%02d EUR\n",
                   prezzo / 100, prezzo % 100, inserito / 100, inserito % 100);
            printf("Inserisci moneta (10, 20, 50, 100, 200 - 0 per annullare): ");

            /* stessa lettura sicura vista per il menu */
            letti = scanf("%d", &moneta);
            if (letti == EOF) {
                moneta = 0;                 /* input finito: lo trattiamo come annullamento */
            } else if (letti != 1) {
                scanf("%*s");
                moneta = -1;                /* finira' nel default: moneta rifiutata */
            }

            /* Solo i tagli previsti vengono accettati: per ognuno aumentiamo
             * il contatore di quel taglio e il totale inserito.
             * Qualsiasi altro valore (es. 30) finisce nel default. */
            switch (moneta) {
            case 10:  inseriti10++;  inserito += 10;  break;
            case 20:  inseriti20++;  inserito += 20;  break;
            case 50:  inseriti50++;  inserito += 50;  break;
            case 100: inseriti100++; inserito += 100; break;
            case 200: inseriti200++; inserito += 200; break;
            case 0:   annullato = 1; break;
            default:  printf("Moneta non valida: rifiutata.\n");
            }
        }

        /* Acquisto annullato: restituiamo le monete inserite.
         * Il magazzino non e' stato modificato, quindi non c'e' nulla da
         * "rimettere a posto": e' tutto come prima dell'acquisto. */
        if (annullato) {
            printf("Acquisto annullato. Restituite: 10c x%d, 20c x%d, 50c x%d, 1E x%d, 2E x%d\n",
                   inseriti10, inseriti20, inseriti50, inseriti100, inseriti200);
            continue;
        }

        /* =============================================================
         * EROGAZIONE
         * Arrivati qui l'importo e' sufficiente: si toglie una cialda
         * della bevanda scelta (operatore --) e si eroga.
         * ============================================================= */
        switch (scelta) {
        case 1: cialde_caffe--;      printf("Erogazione caffe'... pronto!\n");     break;
        case 2: cialde_cappuccino--; printf("Erogazione cappuccino... pronto!\n"); break;
        case 3: cialde_the--;        printf("Erogazione the... pronto!\n");        break;
        case 4: cialde_latte--;      printf("Erogazione latte... pronto!\n");      break;
        }

        /* Il resto e' la differenza tra quanto inserito e il prezzo.
         * Non puo' essere negativo: il while delle monete termina solo
         * quando inserito >= prezzo. */
        resto = inserito - prezzo;
        if (resto == 0) {
            printf("Nessun resto.\n");
        } else {
            printf("Resto: %d,%02d EUR\n", resto / 100, resto % 100);
        }
    }

    return 0;
}
