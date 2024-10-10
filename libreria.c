#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 50
#define SIZE_LIBRERIA 40
#define MAX_TITOLO 50 // Lunghezza massima del titolo cercato
#define CSV_FILENAME "libreria_libri.csv"

// Enumeratore per le categorie del libro
typedef enum {
    NARRATIVO,
    SAGGISTICA,
    ARTE,
    SCIENZA,
    ROMANZO,
    NUM_CATEGORIE // Per tenere traccia del numero di categorie
} Categoria;

// Struttura per un libro
typedef struct {
    char titolo[MAX_SIZE];
    char autore[MAX_SIZE];
    int anno_pubblicazione;
    double prezzo;
    Categoria categoria;
} Libro;

// Funzione per rimuovere spazi bianchi iniziali e finali
void trim(char* str) {
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1])) {
        str[--len] = '\0';
    }
    char* start = str;
    while (*start && isspace(*start)) {
        start++;
    }
    memmove(str, start, strlen(start) + 1);
}

// Funzione per stampare i dettagli di un libro
void stampaLibro(const Libro* libro) {
    const char* nomiCategorie[] = {"Narrativo", "Saggistica", "Arte", "Scienza", "Romanzo"};
    printf("Dettagli del libro:\n");
    printf("- Titolo: %s\n", libro->titolo);
    printf("- Autore: %s\n", libro->autore);
    printf("- Anno: %d\n", libro->anno_pubblicazione);
    printf("- Prezzo: %.2f\n", libro->prezzo);
    printf("- Categoria: %s\n", nomiCategorie[libro->categoria]);
}

// Funzione per convertire una stringa in una categoria
Categoria stringToCategoria(const char* categoriaStr) {
    if (strcmp(categoriaStr, "Narrativo") == 0) {
        return NARRATIVO;
    } else if (strcmp(categoriaStr, "Saggistica") == 0) {
        return SAGGISTICA;
    } else if (strcmp(categoriaStr, "Arte") == 0) {
        return ARTE;
    } else if (strcmp(categoriaStr, "Scienza") == 0) {
        return SCIENZA;
    } else if (strcmp(categoriaStr, "Romanzo") == 0) {
        return ROMANZO;
    } else {
        return NARRATIVO; // Categoria predefinita se non trova corrispondenze
    }
}

// Funzione per caricare i libri dal file CSV
int caricaLibriDaCSV(Libro libreria[], const char* nomeFile) {
    FILE* file = fopen(nomeFile, "r");
    if (!file) {
        printf("Errore nell'apertura del file %s.\n", nomeFile);
        return 0;
    }

    char linea[256];
    int num_libri = 0;

    // Salta l'intestazione del file CSV
    fgets(linea, sizeof(linea), file);

    while (fgets(linea, sizeof(linea), file) && num_libri < SIZE_LIBRERIA) {
        Libro nuovoLibro;
        
        // Variabili temporanee per memorizzare i dati letti
        char autore[MAX_SIZE];
        char titolo[MAX_SIZE];
        char buffer[10]; // Per gestire l'anno di pubblicazione
        char prezzoBuffer[10]; // Per gestire il prezzo
        char categoriaStr[MAX_SIZE]; // Per gestire la categoria

        // Legge il titolo
        int i = 0, j = 0;
        while (linea[i] != ';' && linea[i] != '\0') {
            titolo[j++] = linea[i++];
        }
        titolo[j] = '\0'; // Termina la stringa
        i++; // Salta il punto e virgola

        // Legge l'autore
        j = 0;
        while (linea[i] != ';' && linea[i] != '\0') {
            autore[j++] = linea[i++];
        }
        autore[j] = '\0'; // Termina la stringa
        i++; // Salta il punto e virgola

        // Legge l'anno di pubblicazione
        j = 0;
        while (linea[i] != ';' && linea[i] != '\0') {
            buffer[j++] = linea[i++];
        }
        buffer[j] = '\0'; // Termina la stringa
        nuovoLibro.anno_pubblicazione = atoi(buffer); // Converte in intero
        i++; // Salta il punto e virgola

        // Legge il prezzo
        j = 0;
        while (linea[i] != ';' && linea[i] != '\0') {
            prezzoBuffer[j++] = linea[i++];
        }
        prezzoBuffer[j] = '\0'; // Termina la stringa
        nuovoLibro.prezzo = atof(prezzoBuffer); // Converte in double
        i++; // Salta il punto e virgola

        // Legge la categoria
        j = 0;
        while (linea[i] != '\n' && linea[i] != '\0') {
            categoriaStr[j++] = linea[i++];
        }
        categoriaStr[j] = '\0'; // Termina la stringa

        // Applica trim per rimuovere spazi bianchi
        trim(titolo);
        trim(autore);
        trim(categoriaStr);

        // Copia i dati nel libro
        strncpy(nuovoLibro.titolo, titolo, MAX_SIZE);
        strncpy(nuovoLibro.autore, autore, MAX_SIZE);

        // Assegna la categoria in base alla stringa letta
        nuovoLibro.categoria = stringToCategoria(categoriaStr);

        // Memorizza il libro nell'array
        libreria[num_libri] = nuovoLibro;
        num_libri++;
    }

    fclose(file);
    printf("Caricati %d libri dalla libreria.\n", num_libri);
    return num_libri;
}

// Funzione per visualizzare tutti i libri di una specifica categoria
void visualizzaLibriPerCategoria(Libro libreria[], int num_libri, Categoria categoria) {
    printf("Libri nella categoria selezionata:\n");
    for (int i = 0; i < num_libri; i++) {
        if (libreria[i].categoria == categoria) {
            stampaLibro(&libreria[i]);
            printf("\n");
        }
    }
}

// Funzione per cercare un libro per titolo
void cercaLibroPerTitolo(Libro libreria[], int num_libri, const char* titolo) {
    for (int i = 0; i < num_libri; i++) {
        if (strcmp(libreria[i].titolo, titolo) == 0) {
            stampaLibro(&libreria[i]);
            return;
        }
    }
    printf("Libro non trovato.\n");
}

// Funzione per mostrare il menu
void mostraMenu() {
    printf("---- Menu Libreria ----\n");
    printf("1. Visualizza libri per categoria\n");
    printf("2. Cerca un libro per titolo\n");
    printf("3. Esci\n");
    printf("Seleziona un'opzione: ");
}

int main() {
    Libro libreria[SIZE_LIBRERIA];
    int num_libri = caricaLibriDaCSV(libreria, CSV_FILENAME);
    int scelta;

    do {
        mostraMenu();
        scanf("%d", &scelta);

        switch (scelta) {
            case 1: {
                int categoria;
                printf("Seleziona una categoria:\n0 - Narrativo\n1 - Saggistica\n2 - Arte\n3 - Scienza\n4 - Romanzo\n");
                scanf("%d", &categoria);
                if (categoria >= 0 && categoria < NUM_CATEGORIE) {
                    visualizzaLibriPerCategoria(libreria, num_libri, (Categoria)categoria);
                } else {
                    printf("Categoria non valida.\n");
                }
                break;
            }
            case 2: {
                char titolo[MAX_TITOLO];
                printf("Inserisci il titolo del libro da cercare: ");
                scanf(" %[^\n]", titolo);
                cercaLibroPerTitolo(libreria, num_libri, titolo);
                break;
            }
            case 3:
                printf("Arrivederci!\n");
                break;
            default:
                printf("Opzione non valida. Riprova.\n");
        }
    } while (scelta != 3);

    return 0;
}
