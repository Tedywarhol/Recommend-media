import tkinter as tk
from tkinter import ttk
from PIL import Image, ImageTk

try:
    resample_method = Image.Resampling.LANCZOS
except AttributeError:
    resample_method = Image.ANTIALIAS


class MultimediaApp:
    def __init__(self, root):
        """
        Initialisation de l'application multimédia.
        Ajout de fonctionnalité pour basculer entre un thème sombre et un thème clair.
        """
        self.root = root
        self.root.title("Bibliothèque Multimédia Moderne")
        self.root.geometry("1200x800")

        # Variables pour les couleurs des thèmes
        self.light_theme = {
            "background": "#f8f9fa",
            "foreground": "#343a40",
            "button_bg": "#007BFF",
            "button_fg": "#ffffff",
        }
        self.dark_theme = {
            "background": "#343a40",
            "foreground": "#f8f9fa",
            "button_bg": "#6c757d",
            "button_fg": "#ffffff",
        }
        self.current_theme = self.light_theme  # Thème par défaut : clair

        # Configuration d'un style moderne
        self.style = ttk.Style()
        self.apply_theme()

        # Conteneur principal
        self.main_frame = ttk.Frame(root, padding=10)
        self.main_frame.pack(fill=tk.BOTH, expand=True)

        # Barre de navigation avec champ de recherche et menu pour le choix du thème
        self.nav_frame = ttk.Frame(self.main_frame, padding=5)
        self.nav_frame.pack(fill=tk.X, pady=(0, 10))

        # Champ de recherche
        self.search_label = ttk.Label(self.nav_frame, text="Recherche :", foreground=self.current_theme["foreground"])
        self.search_label.pack(side=tk.LEFT, padx=(0, 5))

        self.search_var = tk.StringVar()
        self.search_entry = ttk.Entry(self.nav_frame, textvariable=self.search_var, width=30)
        self.search_entry.pack(side=tk.LEFT, padx=(0, 5))
        self.search_entry.bind("<KeyRelease>", self.filter_media)

        self.refresh_button = ttk.Button(self.nav_frame, text="Actualiser", command=self.refresh_media)
        self.refresh_button.pack(side=tk.LEFT, padx=(5, 0))

        # Menu déroulant pour choisir le thème
        self.theme_var = tk.StringVar(value="Clair")
        self.theme_menu = ttk.OptionMenu(self.nav_frame, self.theme_var, "Clair", "Clair", "Sombre", command=self.change_theme)
        self.theme_menu.pack(side=tk.RIGHT, padx=(5, 0))

        # Zone défilable pour afficher les médias
        self.canvas = tk.Canvas(self.main_frame, highlightthickness=0, bg=self.current_theme["background"])
        self.scrollbar = ttk.Scrollbar(self.main_frame, orient=tk.VERTICAL, command=self.canvas.yview)
        self.canvas.configure(yscrollcommand=self.scrollbar.set)
        self.scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        self.canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        self.content_frame = ttk.Frame(self.canvas, padding=10)
        self.canvas.create_window((0, 0), window=self.content_frame, anchor="nw")
        self.content_frame.bind("<Configure>", self.on_frame_configure)

        # Liste des fichiers multimédias
        self.all_media_files = [
            {
                "title": "Snowfall",
                "cover": "../assets/snowfall.png",
                "details": "Détails sur Snowfall : série dramatique sur l'essor du crack à Los Angeles dans les années 80."
            },
            {
                "title": "Blacklist",
                "cover": "../assets/blacklist.png",
                "details": "Détails sur Blacklist : série policière où un criminel notoire aide le FBI à traquer des fugitifs."
            },

            {
                "title": "Destin - Ninho",
                "cover": "../assets/destin.png",
                "details": "Détails sur Destin : album du rappeur Ninho sorti en 2019, incluant des titres à succès."
            },
            # Ajout des séries
            {
                "title": "Power",
                "cover": "../assets/power.png",
                "details": "Détails sur Power : un drame criminel autour de la vie de James St. Patrick, un homme déchiré entre le crime organisé et son entreprise légitime."
            },
            {
                "title": "Power Book II: Ghost",
                "cover": "../assets/power_book2.png",
                "details": "Détails sur Power Book II : la suite de Power suivant les pas de Tariq St. Patrick et ses luttes complexes."
            },
            {
                "title": "One Piece",
                "cover": "../assets/one_piece.png",
                "details": "Détails sur One Piece : l'histoire épique de Monkey D. Luffy et de son équipage en quête du légendaire trésor One Piece."
            },
            {
                "title": "Hunter x Hunter",
                "cover": "../assets/hunter_x_hunter.png",
                "details": "Détails sur Hunter x Hunter : les aventures de Gon Freecss qui cherche à devenir un Hunter légendaire."
            },
            {
                "title": "Naruto Shippuden",
                "cover": "../assets/naruto_shippuden.png",
                "details": "Détails sur Naruto Shippuden : l'histoire de Naruto Uzumaki, un ninja en quête de reconnaissance et de pouvoir."
            },
            {
                "title": "Legacies",
                "cover": "../assets/legacies.png",
                "details": "Détails sur Legacies : spin-off de The Vampire Diaries et The Originals, mettant en scène de jeunes créatures surnaturelles."
            },
            {
                "title": "The Chi",
                "cover": "../assets/the_chi.png",
                "details": "Détails sur The Chi : un drame explorant la vie dans le sud de Chicago et les défis quotidiens de ses résidents."
            },
            {
                "title": "Prison Break",
                "cover": "../assets/prison_break.png",
                "details": "Détails sur Prison Break : l'histoire de Michael Scofield orchestrant une évasion complexe pour sauver son frère accusé à tort."
            },
            {
                "title": "Game of Thrones",
                "cover": "../assets/game_of_thrones.png",
                "details": "Détails sur Game of Thrones : une lutte épique entre grandes maisons pour le Trône de Fer."
            },
            # Ajout des films
            {
                "title": "Fast and Furious",
                "cover": "../assets/fast_and_furious.png",
                "details": "Détails sur Fast and Furious : une franchise autour de courses automobiles, de braquages et de relations familiales."
            },
            {
                "title": "John Wick",
                "cover": "../assets/john_wick.png",
                "details": "Détails sur John Wick : un assassin légendaire cherchant à se venger de ceux qui lui ont tout pris."
            },
            {
                "title": "Creed",
                "cover": "../assets/creed.png",
                "details": "Détails sur Creed : l'histoire d'Adonis Creed, fils d'Apollo Creed, et son ascension dans le monde de la boxe."
            },
            {
                "title": "Bad Boys for Life",
                "cover": "../assets/bad_boys_for_life.png",
                "details": "Détails sur Bad Boys for Life : les détectives Mike et Marcus dans une nouvelle mission explosive."
            },
            {
                "title": "Tenet",
                "cover": "../assets/tenet.png",
                "details": "Détails sur Tenet : un thriller scientifique complexe jouant avec l'entropie et le temps."
            },
            {
                "title": "Red One",
                "cover": "../assets/red_one.png",
                "details": "Détails sur Red One : un film d'action et d'aventure à haut risque."
            },
            {
                "title": "After",
                "cover": "../assets/after.png",
                "details": "Détails sur After : une romance tumultueuse entre Tessa et Hardin."
            },
            {
                "title": "50 nuances de Grey",
                "cover": "../assets/50_nuances_de_grey.png",
                "details": "Détails sur 50 nuances de Grey : une romance explorant le lien passionné entre Anastasia Steele et Christian Grey."
            },
            {
                "title": "365 jours",
                "cover": "../assets/365_jours.png",
                "details": "Détails sur 365 jours : une histoire intense d'amour et de captivité."
            },
            # Ajout des albums
            {
                "title": "Ipséité - Damso",
                "cover": "../assets/ipseite.png",
                "details": "Pistes : Nwaar is the New Black, Mosaïque Solitaire, Macarena, Kin la Belle..."
            },
            {
                "title": "Mon cœur avait raison - Gims",
                "cover": "../assets/mon_coeur_avait_raison.png",
                "details": "Pistes : Est-ce que tu m'aimes ?, Sapés comme jamais, Brisé, Zouker..."
            },
            {
                "title": "Mr Sal - Niska",
                "cover": "../assets/mr_sal.png",
                "details": "Pistes : Médicament, Du lundi au lundi, Giuseppe, Réseaux..."
            },
            {
                "title": "La menace fantôme - Freeze Corleone",
                "cover": "../assets/la_menace_fantome.png",
                "details": "Pistes : Freeze Raël, Hors Ligne, Desiigner, Talisman..."
            },
            {
                "title": "L'apogée - Sexion d'Assaut",
                "cover": "../assets/l_apogee.png",
                "details": "Pistes : Avant qu'elle parte, Balader, Wati House, Ma direction..."
            },
            {
                "title": "Fame - Lefa",
                "cover": "../assets/fame.png",
                "details": "Pistes : Smile, Paradise, CDM, Fautes de français..."
            }
        ]

        self.media_files = list(self.all_media_files)
        self.thumbnails = []  # Stockage des images pour éviter leur suppression

        # Affichage initial
        self.populate_content()

    def apply_theme(self):
        """
        Applique le thème sélectionné en modifiant les styles de l'interface.
        """
        self.style.configure("TLabel", background=self.current_theme["background"], foreground=self.current_theme["foreground"], font=("Arial", 10))
        self.style.configure("TButton", font=("Arial", 10), foreground=self.current_theme["button_fg"], background=self.current_theme["button_bg"])
        self.style.configure("TFrame", background=self.current_theme["background"])
        self.style.map("TButton", background=[("active", self.current_theme["button_bg"])])

    def change_theme(self, selected_theme):
        """
        Change le thème de l'interface en fonction de la sélection utilisateur.
        """
        if selected_theme == "Clair":
            self.current_theme = self.light_theme
        elif selected_theme == "Sombre":
            self.current_theme = self.dark_theme
        self.apply_theme()  # Appliquer les styles du nouveau thème
        self.refresh_theme()

    def refresh_theme(self):
        """
        Met à jour les éléments de l'interface pour refléter le nouveau thème.
        """
        self.canvas.config(bg=self.current_theme["background"])
        self.search_label.config(foreground=self.current_theme["foreground"])
        self.populate_content()

    def on_frame_configure(self, event):
        """
        Met à jour la région de défilement en fonction de la taille du contenu.
        """
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))

    def refresh_media(self):
        """
        Réinitialise le filtre et affiche tous les médias.
        """
        self.search_var.set("")
        self.media_files = list(self.all_media_files)
        self.populate_content()

    def filter_media(self, event=None):
        """
        Filtre les médias en fonction du texte recherché.
        """
        search_text = self.search_var.get().lower()
        self.media_files = [
            media for media in self.all_media_files if search_text in media["title"].lower()
        ]
        self.populate_content()

    def populate_content(self):
        """
        Remplit la grille avec les fichiers multimédias disponibles.
        """
        for widget in self.content_frame.winfo_children():
            widget.destroy()
        self.thumbnails.clear()

        columns = 4  # Nombre d'éléments par ligne
        for idx, media in enumerate(self.media_files):
            try:
                image = Image.open(media["cover"])
                image = image.resize((150, 225), resample_method)
                photo = ImageTk.PhotoImage(image)
                self.thumbnails.append(photo)
            except Exception as e:
                print(f"Erreur lors du chargement de l'image {media['cover']}: {e}")
                continue

            media_frame = ttk.Frame(self.content_frame, padding=5, relief="solid")
            media_frame.grid(row=(idx // columns) * 2, column=idx % columns, padx=10, pady=10, sticky="n")

            btn = tk.Button(media_frame, image=photo, relief="flat",
                            command=lambda m=media: self.show_details(m))
            btn.pack()

            title_label = ttk.Label(media_frame, text=media["title"], anchor="center", font=("Arial", 10, "bold"))
            title_label.pack(pady=(5, 0))

    def show_details(self, media):
        """
        Affiche une fenêtre avec les détails du média.
        """
        detail_win = tk.Toplevel(self.root)
        detail_win.title(media["title"])
        detail_win.geometry("600x500")

        main_detail = ttk.Frame(detail_win, padding=10)
        main_detail.pack(fill=tk.BOTH, expand=True)

        img_frame = ttk.Frame(main_detail)
        img_frame.grid(row=0, column=0, sticky="n")
        text_frame = ttk.Frame(main_detail)
        text_frame.grid(row=0, column=1, sticky="n", padx=(10, 0))

        try:
            image = Image.open(media["cover"])
            image = image.resize((250, 375), resample_method)
            photo = ImageTk.PhotoImage(image)
            detail_win.photo = photo
            img_label = ttk.Label(img_frame, image=photo)
            img_label.pack()
        except Exception as e:
            print(f"Erreur lors du chargement de l'image dans la fenêtre de détails: {e}")
            ttk.Label(img_frame, text="Image non disponible").pack()

        details_label = ttk.Label(
            text_frame,
            text=media["details"],
            wraplength=300,
            justify="left",
            font=("Arial", 10)
        )
        details_label.pack()

        # Bouton "Fermer" pour quitter la fenêtre de détail
        close_button = ttk.Button(main_detail, text="Fermer", command=detail_win.destroy)
        close_button.grid(row=1, column=0, columnspan=2, pady=(10, 0))

        # Configuration des colonnes de la fenêtre principale de détail
        main_detail.columnconfigure(0, weight=1)
        main_detail.columnconfigure(1, weight=1)

        # Ajouter une section pour les avis utilisateurs
        opinion_frame = ttk.Frame(main_detail, padding=5)
        opinion_frame.grid(row=2, column=0, columnspan=2)

        ttk.Label(opinion_frame, text="Donnez votre avis :", font=("Arial", 10, "bold"),
                  foreground=self.current_theme["foreground"]).pack(pady=(10, 0))
        opinion_var = tk.StringVar()
        opinion_entry = ttk.Entry(opinion_frame, textvariable=opinion_var, width=50)
        opinion_entry.pack(pady=(5, 5))

        def submit_opinion():
            # Capture de l'avis utilisateur
            print(f"Avis sur {media['title']} : {opinion_var.get()}")
            opinion_entry.delete(0, tk.END)  # Réinitialisation du champ après soumission

        # Bouton pour soumettre l'avis
        submit_button = ttk.Button(opinion_frame, text="Soumettre", command=submit_opinion)
        submit_button.pack(pady=(5, 10))
if __name__ == "__main__":
    # Création de la fenêtre principale Tkinter
    root = tk.Tk()
    # Instanciation de l'application multimédia avec la fenêtre principale
    app = MultimediaApp(root)
    # Démarrage de la boucle d'événements Tkinter (gestion interactive de l'interface)
    root.mainloop()
