#include "../include/media.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <ctype.h>
#define MAX_AVIS_TOTAL (MAX_MEDIAS * MAX_AVIS)
Media medias[MAX_MEDIAS];
int nbMedias = 0;

Avis avisGlobaux[MAX_AVIS_TOTAL];
int compteurAvis = 0;
HistoriqueRecherche historique = {.compteur = 0};

/* ---------------------------------------------------------------------------
   Ajoute un média au tableau global.
   Retourne 1 si l'ajout a réussi, 0 sinon.
   --------------------------------------------------------------------------- */
int ajouterMedia(Media media)
{
    if (nbMedias >= MAX_MEDIAS)
        return 0;
    medias[nbMedias++] = media;
    return 1;
}

/* ---------------------------------------------------------------------------
   Comparaison par année décroissante.
   --------------------------------------------------------------------------- */
int comparerParAnnee(const void *a, const void *b)
{
    const Media *m1 = (const Media *)a;
    const Media *m2 = (const Media *)b;
    return m2->annee - m1->annee;
}

/* ---------------------------------------------------------------------------
   Comparaison par note moyenne (popularité).
   --------------------------------------------------------------------------- */
int comparerParPopularite(const void *a, const void *b)
{
    const Media *m1 = (const Media *)a;
    const Media *m2 = (const Media *)b;
    if (m2->noteMoyenne > m1->noteMoyenne)
        return 1;
    if (m2->noteMoyenne < m1->noteMoyenne)
        return -1;
    return 0;
}

/* ---------------------------------------------------------------------------
   Classe les médias en fonction d'un critère : "annee" ou "popularite".
   Retourne un tableau statique des médias triés ou NULL si le critère n'est pas reconnu.
   --------------------------------------------------------------------------- */
Media *classerMedias(const char *critere)
{
    static Media mediasTries[MAX_MEDIAS];
    memcpy(mediasTries, medias, nbMedias * sizeof(Media));

    if (strcmp(critere, "annee") == 0)
    {
        qsort(mediasTries, nbMedias, sizeof(Media), comparerParAnnee);
    }
    else if (strcmp(critere, "popularite") == 0)
    {
        qsort(mediasTries, nbMedias, sizeof(Media), comparerParPopularite);
    }
    else
    {
        return NULL;
    }
    return mediasTries;
}

/* ---------------------------------------------------------------------------
   Affiche les informations d'un média.
   --------------------------------------------------------------------------- */
void afficherMedia(const Media *m)
{
    printf("\n--- MEDIA ---\n");
    printf("ID           : %d\n", m->id);
    printf("Nom          : %s\n", m->nom);
    printf("Année        : %d\n", m->annee);
    printf("Genre        : %s\n", m->genre);
    printf("Description  : %s\n", m->description);
    printf("Lien         : %s\n", m->lien);
    printf("Note Moyenne : %.2f (%d avis)\n", m->noteMoyenne, m->nombreAvis);
}

/* ---------------------------------------------------------------------------
   Ajoute un avis pour un média.
   Met à jour le tableau global d'avis et recalcule la note moyenne du média concerné.
   Retourne 1 si l'ajout a réussi, 0 sinon.
   --------------------------------------------------------------------------- */
int ajouterAvis(Avis nouvelAvis)
{
    if (compteurAvis >= MAX_AVIS_TOTAL)
        return 0;
    avisGlobaux[compteurAvis++] = nouvelAvis;
    for (int i = 0; i < nbMedias; i++)
    {
        if (medias[i].id == nouvelAvis.idMedia)
        {
            medias[i].noteMoyenne = ((medias[i].noteMoyenne * medias[i].nombreAvis) + nouvelAvis.note) /
                                    (medias[i].nombreAvis + 1);
            medias[i].nombreAvis++;
            break;
        }
    }
    return 1;
}

/* ---------------------------------------------------------------------------
   Convertit une chaîne de caractères en minuscules.
   --------------------------------------------------------------------------- */
void toLower(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }
}

/* ---------------------------------------------------------------------------
   Vérifie si 'recherche' apparaît dans 'source' (comparaison insensible à la casse).
   --------------------------------------------------------------------------- */
int correspondancePartielle(const char *source, const char *recherche)
{
    char srcCpy[MAX_TITRE], rechCpy[MAX_TITRE];

    strncpy(srcCpy, source, MAX_TITRE - 1);
    srcCpy[MAX_TITRE - 1] = '\0';
    strncpy(rechCpy, recherche, MAX_TITRE - 1);
    rechCpy[MAX_TITRE - 1] = '\0';

    toLower(srcCpy);
    toLower(rechCpy);

    return (strstr(srcCpy, rechCpy) != NULL);
}

/* ---------------------------------------------------------------------------
   Effectue une recherche interactive dans la liste des médias.
   Affiche les médias dont le nom, le genre ou l'artiste correspond partiellement
   au critère, ou dont l'année correspond exactement.
   Si aucun résultat n'est trouvé, des suggestions sont proposées,
   et il est possible de trier les résultats.
   --------------------------------------------------------------------------- */
void rechercherMedia(void)
{
    char critere[100];
    int trouver = 0;

    printf("\n===== Recherche de média =====\n");
    printf("Entrez un mot-clé (nom, genre, artiste, année) : ");
    scanf(" %[^\n]", critere); // Correction du format : suppression du 's' superflu

    if (historique.compteur < MAX_RECHERCHE)
    {
        strcpy(historique.historique[historique.compteur++], critere);
    }

    printf("\n-- Simulation d'une requête SQL --\n");
    printf("REQUETE SQL : SELECT * FROM medias WHERE nom LIKE '%%%s%%' OR genre LIKE '%%%s%%' OR artiste LIKE '%%%s%%' OR annee = '%s';\n",
           critere, critere, critere, critere);

    for (int i = 0; i < nbMedias; i++)
    {
        if (correspondancePartielle(medias[i].nom, critere) ||
            correspondancePartielle(medias[i].genre, critere) ||
            correspondancePartielle(medias[i].artiste, critere) ||
            (atoi(critere) == medias[i].annee))
        {
            afficherMedia(&medias[i]);
            trouver = 1;
        }
    }

    if (!trouver)
    {
        printf("Aucun média trouvé pour \"%s\".\n", critere);
        printf("Suggestions similaires :\n");

        char critereCpy[100];
        strcpy(critereCpy, critere);
        char *mot = strtok(critereCpy, " ");
        while (mot != NULL)
        {
            for (int i = 0; i < nbMedias; i++)
            {
                if (correspondancePartielle(medias[i].nom, mot) ||
                    correspondancePartielle(medias[i].genre, mot) ||
                    correspondancePartielle(medias[i].artiste, mot))
                {
                    printf("- %s (artiste : %s, genre : %s)\n",
                           medias[i].nom, medias[i].artiste, medias[i].genre);
                }
            }
            mot = strtok(NULL, " ");
        }
        printf("Conseils :\n - Essayez d'être plus général\n - Vérifiez l'orthographe\n");
    }
    else
    {
        printf("\nSouhaitez-vous trier les résultats ? (1: Nom, 2: Année, 0: Non) : ");
        int tri;
        scanf("%d", &tri);
        if (tri == 1 || tri == 2)
        {
            for (int i = 0; i < nbMedias - 1; i++)
            {
                for (int j = i + 1; j < nbMedias; j++)
                {
                    int condition = (tri == 1 && strcmp(medias[i].nom, medias[j].nom) > 0) ||
                                    (tri == 2 && medias[i].annee > medias[j].annee);
                    if (condition)
                    {
                        Media temp = medias[i];
                        medias[i] = medias[j];
                        medias[j] = temp;
                    }
                }
            }
            printf("Résultats triés :\n");
            for (int i = 0; i < nbMedias; i++)
            {
                if (correspondancePartielle(medias[i].nom, critere) ||
                    correspondancePartielle(medias[i].genre, critere) ||
                    correspondancePartielle(medias[i].artiste, critere) ||
                    (atoi(critere) == medias[i].annee))
                {
                    afficherMedia(&medias[i]);
                }
            }
        }
    }
}

/* ---------------------------------------------------------------------------
   Calcule la note moyenne pour un média donné à partir du tableau global d'avis.
   --------------------------------------------------------------------------- */
float calculerNoteMoyenne(int idMedia)
{
    float somme = 0.0f;
    int count = 0;
    for (int i = 0; i < compteurAvis; i++)
    {
        if (avisGlobaux[i].idMedia == idMedia)
        {
            somme += avisGlobaux[i].note;
            count++;
        }
    }
    return count > 0 ? somme / count : 0.0f;
}

/* ---------------------------------------------------------------------------
   Sauvegarde un média dans la base SQLite.
   Crée la table s'il n'existe pas déjà.
   Retourne 1 si la sauvegarde a réussi, 0 sinon.
   --------------------------------------------------------------------------- */
int sauvegarderMediaSQL(Media media)
{
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open("medias.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur SQLite: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    const char *sql_create = "CREATE TABLE IF NOT EXISTS medias ("
                             "id INTEGER PRIMARY KEY, "
                             "nom TEXT, "
                             "annee INTEGER, "
                             "description TEXT, "
                             "lien TEXT, "
                             "genre TEXT, "
                             "nombreAvis INTEGER, "
                             "noteMoyenne REAL);";
    rc = sqlite3_exec(db, sql_create, 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur création table: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 0;
    }
    char sql_insert[1024];
    snprintf(sql_insert, sizeof(sql_insert),
             "INSERT OR REPLACE INTO medias VALUES (%d, '%s', %d, '%s', '%s', '%s', %d, %f);",
             media.id, media.nom, media.annee, media.description, media.lien,
             media.genre, media.nombreAvis, media.noteMoyenne);
    rc = sqlite3_exec(db, sql_insert, 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur insertion: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    sqlite3_close(db);
    return (rc == SQLITE_OK);
}

/* ---------------------------------------------------------------------------
   Charge les médias stockés dans la base SQLite.
   Les médias chargés sont enregistrés dans le tableau global et nbMedias est mis à jour.
   Retourne le nombre de médias chargés ou -1 en cas d'erreur.
   --------------------------------------------------------------------------- */
int chargerMediasSQL()
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("medias.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur d'ouverture de la BDD: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    const char *sql = "SELECT * FROM medias;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur de préparation de la requête: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    nbMedias = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && nbMedias < MAX_MEDIAS)
    {
        Media m;
        m.id = sqlite3_column_int(stmt, 0);
        strncpy(m.nom, (const char *)sqlite3_column_text(stmt, 1), MAX_NOM - 1);
        m.nom[MAX_NOM - 1] = '\0';
        m.annee = sqlite3_column_int(stmt, 2);
        strncpy(m.description, (const char *)sqlite3_column_text(stmt, 3), MAX_DESC - 1);
        m.description[MAX_DESC - 1] = '\0';
        strncpy(m.lien, (const char *)sqlite3_column_text(stmt, 4), MAX_LIEN - 1);
        m.lien[MAX_LIEN - 1] = '\0';
        strncpy(m.genre, (const char *)sqlite3_column_text(stmt, 5), MAX_GENRE - 1);
        m.genre[MAX_GENRE - 1] = '\0';
        m.nombreAvis = sqlite3_column_int(stmt, 6);
        m.noteMoyenne = (float)sqlite3_column_double(stmt, 7);
        medias[nbMedias++] = m;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return nbMedias;
}
