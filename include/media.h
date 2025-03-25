#ifndef MEDIA_H
#define MEDIA_H

#define MAX_NOM 100
#define MAX_DESC 255
#define MAX_LIEN 255
#define MAX_GENRE 30
#define MAX_AVIS 100

// Structure de Média
typedef struct {
    int id;
    char nom[MAX_NOM];
    int annee;
    char description[MAX_DESC];                                                                                                                                                                                                                           
    char lien[MAX_LIEN];
    char genre[MAX_GENRE];
    int nombreAvis;
    float noteMoyenne;
} Media;

// Structure d'Avis utilisateur
typedef struct {
    int idMedia;
    int idUtilisateur;
    float note;
    char commentaire[MAX_DESC];
} Avis;


/**
 * Ajouter un nouveau média à la base de données
 * @param media Le média à ajouter
 * @return 1 si l'ajout a réussi, 0 sinon
 */
int ajouterMedia(Media media);

/**
 * Classer les médias en fonction du genre, année ou popularité
 * @param critere Le critère de classement
 * @return Tableau de médias classés ou NULL en cas d'erreur
 */
Media* classerMedias(const char *critere);

/**
 * Rechercher un média par nom ou genre
 * @param nomOuGenre Le nom ou genre à rechercher
 * @return Tableau des médias trouvés ou NULL si aucun résultat
 */
Media* rechercherMedia(const char *nomOuGenre);

/**
 * Afficher les détails d'un média
 * @param media Le média à afficher
 */
void afficherMedia(Media media);

/**
 * Ajouter un avis sur un média
 * @param avis L'avis à ajouter
 * @return 1 si l'ajout a réussi, 0 sinon
 */
int ajouterAvis(Avis avis);

/**
 * Calculer la note moyenne d'un média
 * @param idMedia L'identifiant du média
 * @return La note moyenne calculée
 */
float calculerNoteMoyenne(int idMedia);

/**
 * Sauvegarder un média dans la base de données SQLite
 * @param media Le média à sauvegarder
 * @return 1 si la sauvegarde a réussi, 0 sinon
 */
int sauvegarderMediaSQL(Media media);

/**
 * Charger la liste des médias depuis SQLite
 * @return Le nombre de médias chargés ou -1 en cas d'erreur
 */
int chargerMediasSQL();

/**
 * Afficher l'interface graphique pour consulter un média
 */
void afficherInterfaceConsultation();

#endif
