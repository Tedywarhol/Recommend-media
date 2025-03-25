#ifndef GUI_H
#define GUI_H

// Définition des constantes pour les fenêtres
typedef enum {
    FENETRE_CONNEXION,
    FENETRE_INSCRIPTION,
    FENETRE_PRINCIPALE,
    FENETRE_CONSULTATION
} Fenetre;

// Structure de l'interface utilisateur
typedef struct {
    Fenetre fenetreActuelle;
} InterfaceUtilisateur;

// Prototypes des fonctions

/**
 * Changer la fenêtre affichée dans l'application
 * @param interface Pointeur vers l'interface utilisateur
 * @param nouvelleFenetre La fenêtre à afficher
 */
void changerFenetre(InterfaceUtilisateur *interface, Fenetre nouvelleFenetre);

/**
 * Afficher la fenêtre de connexion
 * @return 1 si l'affichage a réussi, 0 sinon
 */
int afficherFenetreConnexion();

/**
 * Afficher la fenêtre d'inscription
 * @return 1 si l'affichage a réussi, 0 sinon
 */
int afficherFenetreInscription();

/**
 * Afficher la page principale avec les recommandations
 * @param idUtilisateur L'identifiant de l'utilisateur connecté
 * @return 1 si l'affichage a réussi, 0 sinon
 */
int afficherFenetrePrincipale(int idUtilisateur);

/**
 * Afficher la page de consultation des médias
 * @param idMedia L'identifiant du média à consulter
 * @return 1 si l'affichage a réussi, 0 sinon
 */
int afficherFenetreConsultation(int idMedia);

/**
 * Mettre à jour dynamiquement l'affichage des recommandations et avis
 * @param idUtilisateur L'identifiant de l'utilisateur connecté
 * @return 1 si la mise à jour a réussi, 0 sinon
 */
int mettreAJourAffichage(int idUtilisateur);

#endif
