#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

#define MAX_GENRES 5
#define MAX_MUSIC 5
#define MAX_RECOMMANDATIONS 10

// Structure pour stocker les préférences utilisateur
typedef struct {
    int idUtilisateur;
    char genresFavoris[MAX_GENRES][30];
    char musiquesFavorites[MAX_MUSIC][30];
} PreferencesUtilisateur;

// Structure pour stocker une recommandation
typedef struct {
    int idUtilisateur;
    int idMedia;
    float score;
} Recommandation;

// Prototypes des fonctions

/**
 * Analyser les recherches des utilisateurs pour identifier leurs centres d'intérêt
 * @param idUtilisateur L'identifiant de l'utilisateur à analyser
 * @return 1 si l'analyse a réussi, 0 sinon
 */
int analyserPreferencesUtilisateur(int idUtilisateur);

/**
 * Calculer la similarité entre plusieurs utilisateurs (collaborative filtering)
 * @param idUtilisateur1 Premier utilisateur à comparer
 * @param idUtilisateur2 Second utilisateur à comparer
 * @return Score de similarité entre 0.0 et 1.0
 */
float calculerSimilariteUtilisateurs(int idUtilisateur1, int idUtilisateur2);

/**
 * Analyser les tendances et habitudes de visionnage
 * @return 1 si l'analyse a réussi, 0 sinon
 */
int analyserTendancesVisionnage();

/**
 * Générer une liste de recommandations pour un utilisateur
 * @param idUtilisateur L'identifiant de l'utilisateur
 * @param recommandations Tableau à remplir avec les recommandations
 * @return Nombre de recommandations générées
 */
int genererRecommandations(int idUtilisateur, Recommandation recommandations[MAX_RECOMMANDATIONS]);

/**
 * Sauvegarder les recommandations dans la base de données SQLite
 * @param idUtilisateur L'identifiant de l'utilisateur
 * @param recommandations Tableau des recommandations à sauvegarder
 * @param nombreRecommandations Nombre de recommandations dans le tableau
 * @return 1 si la sauvegarde a réussi, 0 sinon
 */
int sauvegarderRecommandationsSQL(int idUtilisateur, Recommandation recommandations[MAX_RECOMMANDATIONS], int nombreRecommandations);

/**
 * Charger les recommandations depuis SQLite
 * @param idUtilisateur L'identifiant de l'utilisateur
 * @param recommandations Tableau à remplir avec les recommandations
 * @return Nombre de recommandations chargées ou -1 en cas d'erreur
 */
int chargerRecommandationsSQL(int idUtilisateur, Recommandation recommandations[MAX_RECOMMANDATIONS]);

/**
 * Afficher l'interface graphique des recommandations
 * @param idUtilisateur L'identifiant de l'utilisateur
 */
void afficherInterfaceRecommandations(int idUtilisateur);

#endif
