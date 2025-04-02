#include "../include/user.h"
#include "../include/sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
char getch()
{
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

int saisirChoix(int min, int max)
{
    int choix;
    int result;
    while (1)
    {
        result = scanf("%d", &choix);
        if (result != 1)
        {
            // Entrée non numérique : vider le buffer
            while (getchar() != '\n')
                ;
            printf("Entrée invalide. Veuillez saisir un nombre entre %d et %d : ", min, max);
        }
        else if (choix < min || choix > max)
        {
            printf("Choix invalide. Veuillez entrer un nombre entre %d et %d : ", min, max);
            // Vider le buffer pour être sûr
            while (getchar() != '\n')
                ;
        }
        else
        {
            // Nettoyer la ligne restante (le caractère '\n')
            while (getchar() != '\n')
                ;
            return choix;
        }
    }
}

void choisirPreference(Utilisateur *utilisateur)
{
    printf("Sélectionnez votre préférence principale :\n");
    printf("1 - Musique\n2 - Jeux vidéo\n3 - Lecture\n4 - Films et séries\n5 - Sport\n6 - Technologie\n");
    int choix = saisirChoix(1, 6);

    if (choix == 1)
    {
        printf("Sélectionnez votre genre préféré de musique :\n");
        printf("1 - Pop\n2 - RnB\n3 - Rap\n4 - Kpop\n5 - Autre\n");
        int musiqueChoix = saisirChoix(1, 5);
        const char *genresMusique[] = {"Musique - Pop", "Musique - RnB", "Musique - Rap", "Musique - Kpop", "Musique - Autre"};
        strcpy(utilisateur->preference, genresMusique[musiqueChoix - 1]);
    }
    else if (choix == 3)
    {
        printf("Sélectionnez votre genre préféré de lecture :\n");
        printf("1 - Horreur\n2 - Fiction\n3 - Émotifs\n4 - Autre\n");
        int lectureChoix = saisirChoix(1, 4);
        const char *genresLecture[] = {"Lecture - Horreur", "Lecture - Fiction", "Lecture - Émotifs", "Lecture - Autre"};
        strcpy(utilisateur->preference, genresLecture[lectureChoix - 1]);
    }
    else if (choix == 4)
    {
        printf("Sélectionnez votre genre préféré de films et séries :\n");
        printf("1 - Horreur\n2 - Comédie\n3 - Science-fiction\n4 - Documentaire\n5 - Autre\n");
        int filmsChoix = saisirChoix(1, 5);
        const char *genresFilms[] = {"Films et séries - Horreur", "Films et séries - Comédie", "Films et séries - Science-fiction", "Films et séries - Documentaire", "Films et séries - Autre"};
        strcpy(utilisateur->preference, genresFilms[filmsChoix - 1]);
    }
    else
    {
        const char *categories[] = {"Musique", "Jeux vidéo", "Lecture", "Films et séries", "Sport", "Technologie", "Autre"};
        strcpy(utilisateur->preference, categories[choix - 1]);
    }
}

void suggereUsername(char *username)
{
    int suffixe = 1;
    char original[MAX_USERNAME];
    strcpy(original, username);
    while (verifierUsernameExiste(username))
    {
        snprintf(username, MAX_USERNAME, "%s%d", original, suffixe);
        suffixe++;
    }
    if (strcmp(username, original) != 0)
    {
        printf("Nom d'utilisateur déjà pris. Nom suggéré: %s\n", username);
    }
}

int verifierUsernameExiste(const char *username)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc, exists = 0;

    // Ouvrir la base de données SQLite (ou la créer si elle n'existe pas)
    rc = sqlite3_open("utilisateurs.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("Erreur lors de l'ouverture de la base de données SQLite: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    // Préparer la requête SQL pour vérifier l'existence d'un nom d'utilisateur
    const char *sql = "SELECT 1 FROM utilisateurs WHERE username = ? LIMIT 1;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Erreur lors de la préparation de la requête: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    // Lier le paramètre (le nom d'utilisateur)
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    // Exécuter la requête et vérifier si on obtient un résultat
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        exists = 1;
    }

    // Libérer les ressources et fermer la DB
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return exists;
}

int genererNouvelID()
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    int lastId = 0;

    // Ouvrir la base de données SQLite
    rc = sqlite3_open("utilisateurs.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("Erreur lors de l'ouverture de la base de données SQLite: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Préparer la requête SQL pour trouver le plus grand id dans la table
    const char *sql = "SELECT MAX(id) FROM utilisateurs;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Erreur lors de la préparation de la requête: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Exécuter la requête
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        // Si aucune ligne n'existe, SQLite renvoie NULL, donc lastId reste à 0
        if (sqlite3_column_type(stmt, 0) != SQLITE_NULL)
        {
            lastId = sqlite3_column_int(stmt, 0);
        }
    }
    else
    {
        printf("Erreur lors de la lecture du résultat.\n");
    }

    // Libérer les ressources et fermer la base de données
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return lastId + 1;
}

int creerUtilisateur()
{
    Utilisateur utilisateur;
    char tempPassword[MAX_PASSWORD], confirmPassword[MAX_PASSWORD];

    // Initialize utilisateur structure
    memset(&utilisateur, 0, sizeof(Utilisateur));

    printf("=== Création de compte ===\n");

    // Clear input buffer
    while (getchar() != '\n')
        ;
    // Génération d'un nouvel ID
    utilisateur.id = genererNouvelID();
    if (utilisateur.id == -1)
        return -1;

    // Saisie du nom d'utilisateur
    do
    {
        printf("Entrez un nom d'utilisateur : ");
        scanf("%s", utilisateur.username);
        if (verifierUsernameExiste(utilisateur.username))
        {
            char suggested[MAX_USERNAME];
            strcpy(suggested, utilisateur.username);
            suggereUsername(suggested);
            printf("Voulez-vous utiliser '%s'? (1: Oui / 2: Non) : ", suggested);
            int choix = saisirChoix(1, 2);
            if (choix == 1)
            {
                strcpy(utilisateur.username, suggested);
            }
            else
            {
                continue;
            }
        }
    } while (verifierUsernameExiste(utilisateur.username));

    do
    {
        printf("Entrez votre âge : ");
        scanf("%d", &utilisateur.age);
        if (utilisateur.age < 6)
        {
            printf("Vous devez avoir au moins 6 ans pour créer un compte.\n");
        }
    } while (utilisateur.age < 6);

    // Saisie du mot de passe
    printf("Entrez votre mot de passe\n(Doit avoir au moins 8 caractères, 1 majuscule, 1 chiffre et 1 caractère spécial) : ");
    if (fgets(tempPassword, MAX_PASSWORD, stdin) != NULL)
    {
        tempPassword[strcspn(tempPassword, "\n")] = 0;
    }

    if (!verifierForceMotDePasse(tempPassword))
    {
        printf("Mot de passe trop faible.\n");
        return -1;
    }

    printf("Confirmez votre mot de passe : ");
    if (fgets(confirmPassword, MAX_PASSWORD, stdin) != NULL)
    {
        confirmPassword[strcspn(confirmPassword, "\n")] = 0;
    }

    if (strcmp(tempPassword, confirmPassword) != 0)
    {
        printf("Les mots de passe ne correspondent pas.\n");
        return -1;
    }

    strncpy(utilisateur.motDePasse, tempPassword, MAX_PASSWORD - 1);
    utilisateur.motDePasse[MAX_PASSWORD - 1] = '\0';
    chiffrerMotDePasse(utilisateur.motDePasse);

    choisirPreference(&utilisateur);

    return sauvegarderUtilisateurSQL(&utilisateur);
}

void chiffrerMotDePasse(char *motDePasse)
{
    size_t len = strlen(motDePasse);
    size_t max_shift = MAX_PASSWORD - 5;
    for (size_t i = 0; i < len && i < max_shift; i++)
    {
        motDePasse[i] = motDePasse[i] + 1;
    }
    motDePasse[max_shift] = '\0';
    strcat(motDePasse, "HASH");
}

int verifierForceMotDePasse(const char *motDePasse)
{
    if (!motDePasse)
        return 0;

    size_t len = strlen(motDePasse);
    if (len < 8)
        return 0;
    int maj = 0, chiffre = 0, special = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (isupper(motDePasse[i]))
            maj = 1;
        else if (isdigit(motDePasse[i]))
            chiffre = 1;
        else if (!isalnum(motDePasse[i]))
            special = 1;
    }
    return maj && chiffre && special;
}

int authentifierUtilisateur(const char *username, const char *motDePasse)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("database.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    const char *sql = "SELECT motDePasse FROM utilisateurs WHERE username = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    int auth = 0;
    if (rc == SQLITE_ROW)
    {
        const unsigned char *motDePasseBDD = sqlite3_column_text(stmt, 0);
        if (strcmp((const char *)motDePasseBDD, motDePasse) == 0)
            auth = 1;
    }
    else
    {
        fprintf(stderr, "Utilisateur '%s' non trouvé.\n", username);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return auth;
}

int recupererMotDePasse(const char *username)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("database.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    const char *sql = "SELECT motDePasse FROM utilisateurs WHERE username = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        const unsigned char *motDePasseBDD = sqlite3_column_text(stmt, 0);
        printf("Mot de passe pour l'utilisateur '%s' : %s\n", username, motDePasseBDD);
    }
    else
    {
        printf("Utilisateur '%s' non trouvé.\n", username);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int supprimerUtilisateur(int id)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("database.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    const char *sql = "DELETE FROM utilisateurs WHERE id = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Erreur lors de la suppression de l'utilisateur : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int sauvegarderUtilisateurSQL(Utilisateur *utilisateur)
{
    if (!utilisateur)
        return -1;

    sqlite3 *db;
    char *errMsg = NULL;
    int rc;

    // Ouvrir la base de données SQLite (ou la créer si elle n'existe pas)
    rc = sqlite3_open("utilisateurs.db", &db);
    if (rc)
    {
        printf("Erreur lors de l'ouverture de la base de données SQLite : %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Créer la table "utilisateurs" si elle n'existe pas
    const char *createTableSQL =
        "CREATE TABLE IF NOT EXISTS utilisateurs ("
        "id INTEGER PRIMARY KEY, "
        "username TEXT NOT NULL, "
        "nom TEXT, "
        "prenom TEXT, "
        "age INTEGER, "
        "motDePasse TEXT, "
        "preference TEXT, "
        "twoFactorEnabled INTEGER, "
        "email TEXT NOT NULL);";

    rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        printf("Erreur lors de la création de la table : %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    // Préparer la requête pour insérer un utilisateur
    const char *insertSQL =
        "INSERT INTO utilisateurs (id, username, nom, prenom, age, motDePasse, preference, twoFactorEnabled, email) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Lier les valeurs des utilisateurs aux paramètres de la requête
    sqlite3_bind_int(stmt, 1, utilisateur->id);
    sqlite3_bind_text(stmt, 2, utilisateur->username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, utilisateur->nom, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, utilisateur->prenom, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, utilisateur->age);
    sqlite3_bind_text(stmt, 6, utilisateur->motDePasse, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, utilisateur->preference, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 8, utilisateur->twoFactorEnabled);
    sqlite3_bind_text(stmt, 9, utilisateur->email, -1, SQLITE_STATIC);

    // Exécuter la requête d'insertion
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        printf("Erreur lors de l'insertion des données : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    // Libérer les ressources et fermer la base de données
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    printf("Utilisateur enregistré avec succès dans la base de données SQLite.\n");
    return 1;
}

int chargerUtilisateursSQL(Utilisateur utilisateurs[], size_t maxUtilisateurs)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    size_t count = 0; // Nombre d'utilisateurs chargés

    // Ouvrir la base de données SQLite
    rc = sqlite3_open("utilisateurs.db", &db);
    if (rc)
    {
        printf("Erreur lors de l'ouverture de la base de données SQLite : %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Préparer la requête pour récupérer tous les utilisateurs
    const char *selectSQL = "SELECT id, username, nom, prenom, age, motDePasse, preference, twoFactorEnabled, email FROM utilisateurs;";
    rc = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Parcourir les résultats
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        if (count >= maxUtilisateurs)
        {
            printf("Limite d'utilisateurs atteinte (%zu).\n", maxUtilisateurs);
            break;
        }

        // Charger les valeurs dans la structure Utilisateur
        utilisateurs[count].id = sqlite3_column_int(stmt, 0);
        strncpy(utilisateurs[count].username, (const char *)sqlite3_column_text(stmt, 1), MAX_USERNAME - 1);
        utilisateurs[count].username[MAX_USERNAME - 1] = '\0';
        strncpy(utilisateurs[count].nom, (const char *)sqlite3_column_text(stmt, 2), MAX_NAME - 1);
        utilisateurs[count].nom[MAX_NAME - 1] = '\0';
        strncpy(utilisateurs[count].prenom, (const char *)sqlite3_column_text(stmt, 3), MAX_NAME - 1);
        utilisateurs[count].prenom[MAX_NAME - 1] = '\0';
        utilisateurs[count].age = sqlite3_column_int(stmt, 4);
        strncpy(utilisateurs[count].motDePasse, (const char *)sqlite3_column_text(stmt, 5), MAX_PASSWORD - 1);
        utilisateurs[count].motDePasse[MAX_PASSWORD - 1] = '\0';
        strncpy(utilisateurs[count].preference, (const char *)sqlite3_column_text(stmt, 6), MAX_PREFERENCE - 1);
        utilisateurs[count].preference[MAX_PREFERENCE - 1] = '\0';
        utilisateurs[count].twoFactorEnabled = sqlite3_column_int(stmt, 7);
        strncpy(utilisateurs[count].email, (const char *)sqlite3_column_text(stmt, 8), MAX_EMAIL - 1);
        utilisateurs[count].email[MAX_EMAIL - 1] = '\0';

        count++; // Incrémenter le compteur
    }

    // Libérer les ressources
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    printf("Nombre d'utilisateurs chargés : %zu\n", count);
    return count; // Retourner le nombre d'utilisateurs chargés
}
