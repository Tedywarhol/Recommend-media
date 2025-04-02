#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <string.h>

// Définition des constantes
#define MAX_USERNAME 50
#define MAX_NAME 100
#define MAX_PASSWORD 100
#define MAX_GENRES 5
#define MAX_MUSIC 5
#define MAX_PREFERENCE 50
#define MAX_EMAIL 100
#define MAX_CONFIRM_CODE 10
#define MAX_ATTEMPTS 3

// Structure représentant un utilisateur
typedef struct
{
    int id;                          // Identifiant unique
    char username[MAX_USERNAME];     // Nom d'utilisateur
    char nom[MAX_NAME];              // Nom
    char prenom[MAX_NAME];           // Prénom
    int age;                         // Âge
    char motDePasse[MAX_PASSWORD];   // Mot de passe
    int twoFactorEnabled;            // Activation de l'authentification 2FA (1: activée, 0: non activée)
    char preference[MAX_PREFERENCE]; // Préférences utilisateur
    char email[MAX_EMAIL];           // Adresse e-mail
    char referralCode[20];           // Code de parrainage
} Utilisateur;

/**
 * Demande à l'utilisateur de choisir sa préférence principale et,
 * si nécessaire, des sous-catégories pour l'affiner.
 *
 * @param user Pointeur sur l'utilisateur dont l'attribut 'preference' sera mis à jour.
 */
void choisirPreference(Utilisateur *user);

/**
 * Lit et retourne un entier saisi par l'utilisateur, compris entre min et max.
 * La fonction vide le buffer d'entrée en cas d'erreur.
 *
 * @param min Valeur minimale acceptable.
 * @param max Valeur maximale acceptable.
 * @return Le choix valide saisi par l'utilisateur.
 */
int saisirChoix(int min, int max);

/**
 * Vérifie la force du mot de passe :
 *  - Au moins 8 caractères
 *  - Au moins une majuscule
 *  - Au moins un chiffre
 *  - Au moins un caractère spécial.
 * @param motDePasse Le mot de passe à vérifier
 * @return 1 si le mot de passe est fort, 0 sinon
 */
int verifierForceMotDePasse(const char *motDePasse);

/**
 * Vérifie si le nom d'utilisateur existe déjà dans la base de données.
 * @param username Nom d'utilisateur à vérifier
 * @return 1 si le nom existe, 0 sinon
 */
int verifierUsernameExiste(const char *username);

/**
 * Génère un nouvel identifiant unique en lisant le dernier utilisé dans la base.
 * @return Le nouvel identifiant généré
 */
int genererNouvelID();

/**
 * Suggère automatiquement un nom d'utilisateur.
 * Si le nom est déjà pris, ajoute un suffixe au besoin.
 * @param username Nom d'utilisateur suggéré (modifié en place)
 */
void suggereUsername(char *username);

/**
 * Crée un nouvel utilisateur et l'ajoute à la base de données.
 * @return L'identifiant du nouvel utilisateur ou -1 en cas d'échec
 */
int creerUtilisateur();

/**
 * Chiffre un mot de passe avant son stockage.
 * @param motDePasse Le mot de passe à chiffrer (sera modifié en place)
 */
void chiffrerMotDePasse(char *motDePasse);

/**
 * Authentifie un utilisateur en vérifiant son identifiant et son mot de passe.
 * @param username Nom d'utilisateur
 * @param motDePasse Mot de passe
 * @return L'identifiant de l'utilisateur si authentifié, -1 sinon
 */
int authentifierUtilisateur(const char *username, const char *motDePasse);

/**
 * Permet de récupérer un mot de passe via une question secrète.
 * @param username Nom d'utilisateur
 * @return 1 si la récupération a réussi, 0 sinon
 */
int recupererMotDePasse(const char *username);

/**
 * Supprime un compte utilisateur définitivement.
 * @param id Identifiant de l'utilisateur à supprimer
 * @return 1 si la suppression a réussi, 0 sinon
 */
int supprimerUtilisateur(int id);

/**
 * Sauvegarde un utilisateur dans une base de données SQLite.
 * @param utilisateur L'utilisateur à sauvegarder
 * @return 1 si la sauvegarde a réussi, 0 sinon
 */
int sauvegarderUtilisateurSQL(Utilisateur *utilisateur);

/**
 * Charge tous les utilisateurs depuis une base de données SQLite.
 * @return Le nombre d'utilisateurs chargés ou -1 en cas d'erreur
 */
int chargerUtilisateursSQL();

#endif
