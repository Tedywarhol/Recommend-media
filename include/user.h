#ifndef USER_H
#define USER_H

#define MAX_USERNAME 50
#define MAX_NAME 100
#define MAX_PASSWORD 100
#define MAX_GENRES 5
#define MAX_MUSIC 5

// Structure de l'utilisateur
typedef struct {
    int id;
    char username[MAX_USERNAME];
    char nom[MAX_NAME];
    char prenom[MAX_NAME];
    int age;
    char motDePasse[MAX_PASSWORD];
} Utilisateur;

/**
 * Créer un nouvel utilisateur et l'ajouter à la base de données
 * @return L'identifiant du nouvel utilisateur ou -1 en cas d'échec
 */
int creerUtilisateur();

/**
 * Vérifier si un mot de passe respecte les critères de sécurité
 * @param motDePasse Le mot de passe à vérifier
 * @return 1 si le mot de passe est valide, 0 sinon
 */
int verifierMotDePasse(const char *motDePasse);

/**
 * Chiffrer un mot de passe avant stockage
 * @param motDePasse Le mot de passe à chiffrer (sera modifié)
 */
void chiffrerMotDePasse(char *motDePasse);

/**
 * Authentifier un utilisateur en vérifiant son identifiant et son mot de passe
 * @param username Nom d'utilisateur
 * @param motDePasse Mot de passe
 * @return L'identifiant de l'utilisateur si authentifié, -1 sinon
 */
int authentifierUtilisateur(const char *username, const char *motDePasse);

/**
 * Récupérer le mot de passe d'un utilisateur via question secrète
 * @param username Nom d'utilisateur
 * @return 1 si la récupération a réussi, 0 sinon
 */
int recupererMotDePasse(const char *username);

/**
 * Supprimer un compte utilisateur définitivement
 * @param id Identifiant de l'utilisateur à supprimer
 * @return 1 si la suppression a réussi, 0 sinon
 */
int supprimerUtilisateur(int id);

/**
 * Sauvegarder un utilisateur dans la base de données SQLite
 * @param utilisateur L'utilisateur à sauvegarder
 * @return 1 si la sauvegarde a réussi, 0 sinon
 */
int sauvegarderUtilisateurSQL(Utilisateur utilisateur);

/**
 * Charger les utilisateurs depuis la base de données SQLite
 * @return Le nombre d'utilisateurs chargés ou -1 en cas d'erreur
 */
int chargerUtilisateursSQL();

/**
 * Afficher la fenêtre d'inscription (Interface graphique)
 */
void afficherFenetreInscription();

/**
 * Afficher la fenêtre de connexion (Interface graphique)
 */
void afficherFenetreConnexion();

#endif
