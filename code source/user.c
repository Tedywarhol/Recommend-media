#include "../include/user.h"
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

void choisirPreference(Utilisateur *user)
{

    printf("Sélectionnez votre préférence principale :\n");
    printf("1 - Musique\n");
    printf("2 - Jeux vidéo\n");
    printf("3 - Lecture\n");
    printf("4 - Films et séries\n");
    printf("5 - Sport\n");
    printf("6 - Technologie\n");
    int choix = saisirChoix(1, 6);

    switch (choix)
    {
    case 1: // Musique
        printf("Sélectionnez votre genre préféré de musique :\n");
        printf("1 - Pop\n");
        printf("2 - RnB\n");
        printf("3 - Rap\n");
        printf("4 - Kpop\n");
        printf("5 - Autre\n");
        {
            int musiqueChoix = saisirChoix(1, 5);
            switch (musiqueChoix)
            {
            case 1:
                strcpy(user->preference, "Musique - Pop");
                break;
            case 2:
                strcpy(user->preference, "Musique - RnB");
                break;
            case 3:
                strcpy(user->preference, "Musique - Rap");
                break;
            case 4:
                strcpy(user->preference, "Musique - Kpop");
                break;
            default:
                strcpy(user->preference, "Musique - Autre");
                break;
            }
        }
        break;

    case 2: // Jeux vidéo
        strcpy(user->preference, "Jeux vidéo");
        break;

    case 3: // Lecture
        printf("Sélectionnez votre genre préféré de lecture :\n");
        printf("1 - Horreur\n");
        printf("2 - Fiction\n");
        printf("3 - Émotifs\n");
        printf("4 - Autre\n");
        {
            int lectureChoix = saisirChoix(1, 4);
            switch (lectureChoix)
            {
            case 1:
                strcpy(user->preference, "Lecture - Horreur");
                break;
            case 2:
                strcpy(user->preference, "Lecture - Fiction");
                break;
            case 3:
                strcpy(user->preference, "Lecture - Émotifs");
                break;
            default:
                strcpy(user->preference, "Lecture - Autre");
                break;
            }
        }
        break;

    case 4: // Films et séries
        printf("Sélectionnez votre genre préféré de films et séries :\n");
        printf("1 - Horreur\n");
        printf("2 - Comédie\n");
        printf("3 - Science-fiction\n");
        printf("4 - Documentaire\n");
        printf("5 - Autre\n");
        {
            int filmsChoix = saisirChoix(1, 5);
            switch (filmsChoix)
            {
            case 1:
                strcpy(user->preference, "Films et séries - Horreur");
                break;
            case 2:
                strcpy(user->preference, "Films et séries - Comédie");
                break;
            case 3:
                strcpy(user->preference, "Films et séries - Science-fiction");
                break;
            case 4:
                strcpy(user->preference, "Films et séries - Documentaire");
                break;
            default:
                strcpy(user->preference, "Films et séries - Autre");
                break;
            }
        }
        break;

    case 5: // Sport
        strcpy(user->preference, "Sport");
        break;

    case 6: // Technologie
        strcpy(user->preference, "Technologie");
        break;

    default:
        strcpy(user->preference, "Autre");
        break;
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
    FILE *file = fopen("utilisateurs.txt", "r");
    if (file == NULL)
        return 0;
    Utilisateur user;
    while (fscanf(file, "%d %s %s %s %d %s %s %d %s\n",
                  &user.id, user.username, user.nom, user.prenom, &user.age,
                  user.motDePasse, user.preference, &user.twoFactorEnabled, user.email) != EOF)
    {
        if (strcmp(user.username, username) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int genererNouvelID()
{
    FILE *file = fopen("utilisateurs.txt", "r");
    int lastId = 0;
    if (file != NULL)
    {
        Utilisateur user;
        while (fscanf(file, "%d %s %s %s %d %s %s %d %s\n",
                      &user.id, user.username, user.nom, user.prenom, &user.age,
                      user.motDePasse, user.preference, &user.twoFactorEnabled, user.email) != EOF)
        {
            lastId = user.id;
        }
        fclose(file);
    }
    return lastId + 1;
}

int creerUtilisateur()
{
    Utilisateur user;
    char tempPassword[MAX_PASSWORD], confirmPassword[MAX_PASSWORD];
    printf("=== Création de compte ===\n");

    // Génération d'un nouvel ID
    user.id = genererNouvelID();

    // Saisie du nom d'utilisateur
    do
    {
        printf("Entrez un nom d'utilisateur : ");
        scanf("%s", user.username);
        if (verifierUsernameExiste(user.username))
        {
            printf("Nom d'utilisateur déjà pris.\n");
            suggereUsername(user.username);
        }
    } while (verifierUsernameExiste(user.username));

    // Validation de l'âge (>= 13 ans)
    do
    {
        printf("Entrez votre âge : ");
        scanf("%d", &user.age);
        if (user.age < 13)
        {
            printf("Vous devez avoir au moins 13 ans pour créer un compte.\n");
        }
    } while (user.age < 13);

    // Saisie du mot de passe
    printf("Entrez votre mot de passe\n(must avoir au moins 8 caractères, 1 majuscule, 1 chiffre et 1 caractère spécial) : ");
    fgets(tempPassword, MAX_PASSWORD, stdin);
    if (!verifierForceMotDePasse(tempPassword))
    {
        printf("Mot de passe trop faible.\n");
        return -1;
    }

    // Confirmation du mot de passe
    printf("Confirmez votre mot de passe : ");
    fgets(confirmPassword, MAX_PASSWORD, stdin);
    if (strcmp(tempPassword, confirmPassword) != 0)
    {
        printf("Les mots de passe ne correspondent pas.\n");
        return -1;
    }

    choisirPreference(&user);

    strcpy(user.motDePasse, tempPassword);
    chiffrerMotDePasse(user.motDePasse);

    // Enregistrement dans le fichier
    return enregistrerUtilisateur(&user);
}

void chiffrerMotDePasse(char *motDePasse)
{
    // Remplacement par un hachage SHA256 si besoin
    for (int i = 0; i < strlen(motDePasse); i++)
    {
        motDePasse[i] = motDePasse[i] + 1; // Exemple simple d'encodage
    }
    strcat(motDePasse, "HASH");
}

/* Vérifie si le mot de passe respecte les critères */
int verifierForceMotDePasse(const char *motDePasse)
{
    int len = strlen(motDePasse);
    if (len < 8)
        return 0;
    int maj = 0, chiffre = 0, special = 0;
    for (int i = 0; i < len; i++)
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

int enregistrerUtilisateur(const Utilisateur *user)
{
    FILE *file = fopen("utilisateurs.txt", "a");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier utilisateurs.txt\n");
        return -1;
    }
    fprintf(file, "%d %s %s %s %d %s %s %d %s\n", user->id, user->username,
            user->nom, user->prenom, user->age, user->motDePasse,
            user->preference, user->twoFactorEnabled, user->email);
    fclose(file);
    return 1;
}

int authentifierUtilisateur(const char *username, const char *motDePasse)
{
    return 0;
}

int recupererMotDePasse(const char *username)
{
    return 0;
}

int supprimerUtilisateur(int id)
{
    return 0;
}

int sauvegarderUtilisateurSQL(Utilisateur utilisateur)
{
    return 0;
}

int chargerUtilisateursSQL()
{
    return 0;
}
