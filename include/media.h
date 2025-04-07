#ifndef MEDIA_H
#define MEDIA_H

#ifdef __cplusplus
extern "C"
{
#endif

// Définition des constantes
#define MAX_NOM 100
#define MAX_DESC 255
#define MAX_LIEN 255
#define MAX_GENRE 30
#define MAX_AVIS 100
#define MAX_TITRE 100
#define MAX_ARTISTE 100
#define MAX_RECHERCHE 100

    // Structure de Média
    typedef struct
    {
        int id;
        char nom[MAX_NOM];
        int annee;
        char description[MAX_DESC];
        char lien[MAX_LIEN];
        char genre[MAX_GENRE];
        char artiste[MAX_ARTISTE];
        int nombreAvis;
        float noteMoyenne;
    } Media;

    // Structure d'Avis utilisateur
    typedef struct
    {
        int idMedia;
        int idUtilisateur;
        float note;
        char commentaire[MAX_DESC];
    } Avis;

    // Structure d'Historique de recherches
    typedef struct
    {
        char historique[MAX_RECHERCHE][MAX_TITRE];
        int compteur;
    } HistoriqueRecherche;

    // Déclarations des variables globales (à définir dans media.c)
    extern Media medias[MAX_NOM];
    extern int nbMedias;
    extern HistoriqueRecherche historique;

    /**
     * Ajouter un nouveau média à la base de données.
     * @param media Le média à ajouter.
     * @return 1 si l'ajout a réussi, 0 sinon.
     */
    int ajouterMedia(Media media);

    /**
     * Comparer deux médias par année.
     * @param a Pointeur vers le premier média.
     * @param b Pointeur vers le deuxième média.
     * @return Une valeur négative, nulle ou positive selon que le premier média soit antérieur, égal ou postérieur au deuxième.
     */
    int comparerParAnnee(const void *a, const void *b);

    /**
     * Comparer deux médias par popularité.
     * Cette fonction peut se baser sur la note moyenne ou le nombre d'avis.
     * @param a Pointeur vers le premier média.
     * @param b Pointeur vers le deuxième média.
     * @return Une valeur négative, nulle ou positive indiquant l'ordre de popularité.
     */
    int comparerParPopularite(const void *a, const void *b);

    /**
     * Convertir une chaîne de caractères en minuscules.
     * @param str La chaîne à convertir.
     */
    void toLower(char *str);

    /**
     * Vérifier la correspondance partielle entre deux chaînes.
     * @param source La chaîne source.
     * @param recherche La chaîne à rechercher.
     * @return 1 si une correspondance partielle est trouvée, 0 sinon.
     */
    int correspondancePartielle(const char *source, const char *recherche);

    /**
     * Classer les médias en fonction d'un critère spécifique (par exemple, genre, année ou popularité).
     * @param critere Le critère de classement sous forme de chaîne ("genre", "annee", "popularite").
     * @return Un tableau de médias classés ou NULL en cas d'erreur.
     */
    Media *classerMedias(const char *critere);

    /**
     * Rechercher un média par nom ou genre.
     * @param nomOuGenre Le nom ou le genre à rechercher.
     * @return Un tableau des médias trouvés ou NULL si aucun résultat.
     */
    Media *rechercherMedia(const char *nomOuGenre);

    /**
     * Afficher les détails d'un média.
     * @param media Le média à afficher.
     */
    void afficherMedia(Media media);

    /**
     * Ajouter un avis sur un média.
     * @param avis L'avis à ajouter.
     * @return 1 si l'ajout a réussi, 0 sinon.
     */
    int ajouterAvis(Avis avis);

    /**
     * Calculer la note moyenne d'un média.
     * @param idMedia L'identifiant du média.
     * @return La note moyenne calculée pour le média.
     */
    float calculerNoteMoyenne(int idMedia);

    /**
     * Sauvegarder un média dans la base de données SQLite.
     * @param media Le média à sauvegarder.
     * @return 1 si la sauvegarde a réussi, 0 sinon.
     */
    int sauvegarderMediaSQL(Media media);

    /**
     * Charger la liste des médias depuis la base de données SQLite.
     * @return Le nombre de médias chargés ou -1 en cas d'erreur.
     */
    int chargerMediasSQL();

#ifdef __cplusplus
}
#endif

#endif
