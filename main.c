#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

struct Programari {
  char nume[100], operatie[100];
  int zi,luna,an;
} programari[100];
int nrprogramari;

void afisareMeniu() {
    printf("\n===> Sistem Programare Cabinet Stomatologic <===\n");
    printf("1. Vizualizare oferte operatii\n");
    printf("2. Verificare disponibilitate\n");
    printf("3. Programare operatie\n");
    printf("4. Vizualizare istoric programari\n");
    printf("5. Iesire\n");
    printf("Alegeti o optiune: ");
}

void citire_oferte()
{
    FILE *oferte_file=fopen("oferte.txt","r");
    printf("-> Vizualizeaza tipurile de operatii disponibile <-\n");
    int total_oferte;
    fscanf(oferte_file,"%d",&total_oferte);
    printf("Numarul de oferte: %d\n",total_oferte);
    fgetc(oferte_file);
    for (int i = 0; i <total_oferte; i++) {
        char nume[100];
        int pret,durata;
        fgets(nume, 100, oferte_file);
        nume[strlen(nume) - 1] = '\0';
        fscanf(oferte_file, "%d", &pret);
        fscanf(oferte_file, "%d", &durata);
        fgetc(oferte_file);
        printf("%d. "CYAN"%s "RESET","RED"%d"RESET " lei," GREEN"%d "RESET"min.\n",i+1,nume,pret,durata);
                 }
    fclose(oferte_file);
}
void citire_programari() {
    FILE* programari_file = fopen("programari.txt", "r");

    if (!programari_file) {
        printf("Nu s-a putut deschide fisierul programari.txt\n");
        nrprogramari = 0;
        return;
    }

    fscanf(programari_file, "%d\n", &nrprogramari);

    for (int i = 0; i < nrprogramari; i++) {
        fgets(programari[i].nume, 100, programari_file);
        programari[i].nume[strcspn(programari[i].nume, "\n")] = '\0';

        fgets(programari[i].operatie, 100, programari_file);
        programari[i].operatie[strcspn(programari[i].operatie, "\n")] = '\0';

        fscanf(programari_file, "%d %d %d\n", &programari[i].zi, &programari[i].luna, &programari[i].an);
    }

    fclose(programari_file);
}


void salvare_programari() {
    FILE* f = fopen("programari.txt", "w");
    if (!f) {
        printf("Eroare la deschiderea fisierului pentru scriere.\n");
        return;
    }

    fprintf(f, "%d\n", nrprogramari);
    for (int i = 0; i < nrprogramari; i++) {
        fprintf(f, "%s\n%s\n%d %d %d\n",
            programari[i].nume,
            programari[i].operatie,
            programari[i].zi,
            programari[i].luna,
            programari[i].an
        );
    }

    fclose(f);
}

void verificare_disponibilitate()
{
    int zi, luna, an;
    printf("-> Verificare disponibilitate <-\n");
    printf("Introduceti ziua, luna si anul : ");
    scanf("%d %d %d", &zi, &luna, &an);
    if(zi<=0||luna<=0||an<=0)
    {
        printf("Data introdusa nu este valida!");
        return;
    }
    citire_programari();
    int gasit = 0;
    for (int i = 0; i < nrprogramari; i++) {
        if (programari[i].zi == zi &&
            programari[i].luna == luna &&
            programari[i].an == an) {
            gasit = 1;
            printf("Data %d/%d/%d este deja ocupata.\n",zi, luna, an);
            break;
        }
    }

    if (!gasit) {
        printf("Data %d/%d/%d este libera.\n", zi, luna, an);
    }
}
void adaugare_programari()
{
    citire_programari();
    printf("-> Faceti o programare <-\n");

    struct Programari noua;

    printf("Nume pacient: ");
    getchar();
    fgets(noua.nume, 100, stdin);
    noua.nume[strcspn(noua.nume, "\n")] = '\0';

    printf("Operatie dorita: ");
    fgets(noua.operatie, 100, stdin);
    noua.operatie[strcspn(noua.operatie, "\n")] = '\0';

    printf("Data (zi luna an): ");
    scanf("%d %d %d", &noua.zi, &noua.luna, &noua.an);

    if (noua.zi <= 0 || noua.luna <= 0 || noua.an <= 0) {
        printf("Data invalida. Programarea NU a fost salvata.\n");
                return;
                }
    programari[nrprogramari++] = noua;
    salvare_programari();
    printf("Programarea pentru %s a fost salvata cu succes!\n", noua.nume);
}
void vizualizare_istoric()
{
    printf("-> Vizualizare istoric programari <-\n");
    citire_programari();
    printf("Introduceti numele dumneavoastra: ");
    char nume[100];
    getchar();
    fgets(nume,100,stdin);
    nume[strlen(nume)-1]='\0';
    printf("Istoricul programarilor pentru numele " GREEN "%s" RESET ":\n", nume);
    int gasit=0;
    for(int i=0;i<nrprogramari;i++)
    {
        if(strcmp(programari[i].nume,nume)==0)
        {
            printf("Operatia de " RED "%s" RESET " in data de: " CYAN "%d/%d/%d\n" RESET,programari[i].operatie,programari[i].zi,programari[i].luna,programari[i].an);
            gasit=1;
        }

    }
    if(gasit==0)
    {
        printf("Nu s-a gasit nicio programare pentru %s",nume);
    }
}
int main(int argc, char *argv[]) {
    int choice;
    do {
        system("cls");
        afisareMeniu();
        scanf("%d", &choice);
        system("cls");
        switch (choice) {
            case 1: {
                citire_oferte();
                break;
            }
            case 2: {
                verificare_disponibilitate();
                break;
            }
            case 3: {
                adaugare_programari();
                break;
            }

            case 4:
                vizualizare_istoric();
                break;
            case 5:
                printf("Iesire din aplicatie. La revedere!\n");
                break;
            default:
                printf("Optiune invalida! Incercati din nou.\n");
        }
        if (choice != 5) {
            printf("\nApasati ENTER pentru a reveni la meniu...");
            getchar();
            getchar();
        }
    } while (choice != 5);
    return 0;
}



