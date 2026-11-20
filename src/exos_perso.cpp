
#include <sil/sil.hpp>
#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>
#include <array>


// Vérifie si des coordonnées (x, y) correspondent à un pixel situé dans l'image
// Cette fonction permet d'éviter d'accéder à un pixel en dehors des limites de l'image
bool est_dans_image(int x, int y, sil::Image const& image)
{
    return x >= 0 && x < image.width()
        && y >= 0 && y < image.height();
}


// Exo 1 : Effet fisheye
// Déforme l'image pour donner l'impression qu'elle est bombée vers l'observateur
void fisheye(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    // Détermination du centre de l'image.
    glm::vec2 const centre {image.width()/2, image.height()/2};

    // Calcul de la distance entre le centre et le coin supérieur gauche
    // Cette distance sert de référence pour mesurer l'éloignement de chaque pixel au centre
    float const distance_max = glm::distance(centre, glm::vec2{0.f, 0.f});

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            // Calcul de la position du pixel par rapport au centre de l'image
            float const dx{x - centre.x};
            float const dy{y - centre.y};

            // Calcul de la distance entre le pixel et le centre
            float const distance{std::sqrt(dx * dx + dy * dy)};

            // Normalisation de la distance : 0 au centre de l'image et une valeur proche de 1 vers les coins
            float const rayon{distance / distance_max};

            // Calcul du facteur de déformation : Plus le pixel est éloigné du centre, plus ses coordonnées sources sont rapprochées du centre, ce qui crée l'effet de bombement
            float const facteur{1.f - 0.85f * rayon * rayon};

            // Calcul des coordonnées du pixel à récupérer dans l'image originale : On applique le facteur de déformation à la position relative au centre
            int const source_x{static_cast<int>(std::round(centre.x + dx * facteur))};
            int const source_y {static_cast<int>(std::round(centre.y + dy * facteur))};

            // Vérification que les coordonnées calculées correspondent à un pixel existant
            if (est_dans_image(source_x, source_y, image))
            {
                nouvelle_image.pixel(x, y) = image.pixel(source_x, source_y);
            }
        }
    }
    nouvelle_image.save("images/exos_perso/fisheye.png");
}


// Exo 2 : Ondulations
// Déforme l'image en décalant les pixels selon deux mouvements sinusoïdaux
void ondulations(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    // Paramètres des ondulations :
    float const amplitude{15.f}; // Décalage maximal des pixels (exprimé en nombre de pixels)
    float const frequence{0.04f}; // Contrôle la fréquence des ondulations

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            // Calcul de la coordonnée horizontale du pixel source
            // Le sinus dépend de y : le décalage horizontal varie selon la hauteur
            int const source_x{static_cast<int>(std::round(x + amplitude * std::sin(y * frequence)))};

            // Calcul de la coordonnée verticale du pixel source
            // Le sinus dépend de x : le décalage vertical varie selon la position horizontale
            int const source_y{static_cast<int>(std::round(y + amplitude * std::sin(x * frequence)))};

            // Vérification que les coordonnées calculées correspondent à un pixel existant.
            if (est_dans_image(source_x, source_y, image))
            {
                nouvelle_image.pixel(x, y) = image.pixel(source_x, source_y);
            }
        }
    }
    nouvelle_image.save("images/exos_perso/ondulations.png");
}


// Exo 3 : Peinture à l'huile
// Remplace chaque pixel par une couleur moyenne issue du groupe de luminosité le plus représenté dans son voisinage, afin de créer un aspect peint
void peinture_huile(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    // Rayon du voisinage étudié autour de chaque pixel
    int const rayon{6};

    // Nombre de catégories utilisées pour regrouper les pixels selon leur luminosité
    int const nb_niveaux{6};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            // Tableau qui compte le nombre de pixels appartenant à chaque niveau de luminosité (les valeurs sont initialisées à 0)
            std::array<int, 6> nombres{};

            // Tableau qui additionne les couleurs des pixels de chaque niveau
            std::array<glm::vec3, 6> couleurs{};

            // Parcours des pixels situés dans le voisinage du pixel actuel
            for (int dx{-rayon}; dx <= rayon; dx++)
            {
                for (int dy{-rayon}; dy <= rayon; dy++)
                {
                    // Coordonnées du pixel voisin étudié
                    int const px{x + dx};
                    int const py{y + dy};

                    // On ignore les voisins qui se trouvent en dehors de l'image
                    if (est_dans_image(px, py, image))
                    {
                        // Couleur du pixel voisin
                        glm::vec3 const couleur{image.pixel(px, py)};

                        // Calcul de sa luminosité
                        float const luminosite = 0.2126f * couleur.r + 0.7152f * couleur.g + 0.0722f * couleur.b;

                        // Attribution du pixel à un niveau de luminosité : la multiplication par nb_niveaux répartit les luminosités entre les différentes catégories
                        // std::min évite de dépasser le dernier indice du tableau
                        int const niveau{std::min(nb_niveaux - 1, static_cast<int>(luminosite * nb_niveaux))};
                        
                        nombres[niveau]++;
                        couleurs[niveau] += couleur;
                    }
                }
            }
            // Recherche du niveau de luminosité qui contient le plus de pixels
            int niveau_max{0};

            for (int i{1}; i < nb_niveaux; i++)
            {
                if (nombres[i] > nombres[niveau_max])
                {
                    niveau_max = i;
                }
            }

            // Calcul de la couleur moyenne du niveau le plus fréquent 
            nouvelle_image.pixel(x, y) = couleurs[niveau_max] / static_cast<float>(nombres[niveau_max]);
        }
    }
    nouvelle_image.save("images/exos_perso/peinture_huile.png");
}


// Exo 4 : Pointillisme
// Reconstitue l'image à l'aide de petits cercles colorés séparés par un fond blanc
void pointillisme(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    // Remplissage de la nouvelle image avec du blanc
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            nouvelle_image.pixel(x, y) = glm::vec3{1.f};
        }
    }

    // Espacement entre les centres des points
    int const espacement{4};

    // Rayon des cercles dessinés
    int const rayon{2};

    // Parcours de l'image en avançant de "espacement" à chaque point
    for (int x{rayon}; x < image.width(); x += espacement)
    {
        for (int y{rayon}; y < image.height(); y += espacement)
        {
            // Récupération de la couleur de l'image originale à l'emplacement du centre du point
            glm::vec3 const couleur{image.pixel(x, y)};

            // Parcours des pixels autour du centre pour dessiner un cercle
            for (int dx{-rayon}; dx <= rayon; dx++)
            {
                for (int dy{-rayon}; dy <= rayon; dy++)
                {
                    // Conserve uniquement les pixels situés à l'intérieur du cercle
                    if (dx * dx + dy * dy <= rayon * rayon)
                    {
                        // Coordonnées du pixel à colorer dans la nouvelle image
                        int const px{x + dx};
                        int const py{y + dy};

                        // Vérification que le pixel appartient bien à l'image
                        if (est_dans_image(px, py, image))
                        {
                            nouvelle_image.pixel(px, py) = couleur;
                        }
                    }
                }
            }
        }
    }
    nouvelle_image.save("images/exos_perso/pointillisme.png");
}


// Exo 5 : Hachures
// Transforme l'image en dessin noir et blanc en utilisant des lignes orientées : la densité augmente dans les zones les plus sombres
void hachures(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    // Espacement utilisé pour dessiner les motifs de hachures
    int const espacement{4};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            // Couleur du pixel original
            glm::vec3 const couleur{image.pixel(x, y)};

            // Calcul de sa luminosité
            float const luminosite = 0.2126f * couleur.r + 0.7152f * couleur.g + 0.0722f * couleur.b;

            // La couleur de départ est le blanc
            float valeur{1.f};

            // Première direction de hachures : diagonale (les pixels suffisamment sombres sont noircis lorsque leur position respecte le motif périodique défini par le modulo)
            if (luminosite < 0.75f && (x + y) % espacement < 2)
            {
                valeur = 0.f;
            }

            // Deuxième direction de hachures : diagonale opposée (elle est ajoutée uniquement dans les zones plus sombres)
            if (luminosite < 0.5f && (x - y + image.height()) % espacement < 2)
            {
                valeur = 0.f;
            }

            // Troisième direction de hachures : verticale (elle renforce les zones les plus sombres de l'image)
            if (luminosite < 0.25f && x % espacement < 2)
            {
                valeur = 0.f;
            }

            // Pixel en niveaux de gris :
            nouvelle_image.pixel(x, y) = glm::vec3{valeur};
        }
    }
    nouvelle_image.save("images/exos_perso/hachures.png");
}


// Dessine un triangle rempli dans une image : a, b et c représentent les trois sommets du triangle
void dessiner_triangle(sil::Image& destination, glm::ivec2 a, glm::ivec2 b, glm::ivec2 c, glm::vec3 couleur)
{
    // Calcul de la boîte englobante du triangle :
    // on détermine les coordonnées minimales et maximales de ses sommets
    // Les bornes sont limitées aux dimensions de l'image pour éviter les accès invalides
    int const min_x{std::max(0, std::min({a.x, b.x, c.x}))};
    int const max_x{std::min(destination.width() - 1, std::max({a.x, b.x, c.x}))};
    int const min_y{std::max(0, std::min({a.y, b.y, c.y}))};
    int const max_y{std::min(destination.height() - 1, std::max({a.y, b.y, c.y}))};

    // On examine uniquement les pixels situés dans la boîte englobante du triangle
    for (int x{min_x}; x <= max_x; x++)
    {
        for (int y{min_y}; y <= max_y; y++)
        {
            // Calcul de trois déterminants permettant de savoir de quel côté de chaque arête du triangle se trouve le pixel
            // Le signe de ces valeurs permet de tester si le pixel est à l'intérieur
            float const d1{
                static_cast<float>((x - b.x) * (a.y - b.y) - (a.x - b.x) * (y - b.y))
            };

            float const d2{
                static_cast<float>((x - c.x) * (b.y - c.y) - (b.x - c.x) * (y - c.y))
            };

            float const d3{
                static_cast<float>((x - a.x) * (c.y - a.y) - (c.x - a.x) * (y - a.y))
            };

            // On vérifie si les déterminants présentent à la fois des signes positifs et négatifs : si c'est le cas, le pixel est à l'extérieur du triangle
            bool const negatif{d1 < 0 || d2 < 0 || d3 < 0};
            bool const positif{d1 > 0 || d2 > 0 || d3 > 0};

            // Si les déterminants ne présentent pas de signes opposés, le pixel appartient au triangle ou à l'une de ses arêtes
            if (!(negatif && positif))
            {
                destination.pixel(x, y) = couleur;
            }
        }
    }
}


// Exo 6 : Low Poly
// Simplifie l'image en la découpant en triangles, chacun rempli avec une couleur prélevée dans l'image originale
void low_poly(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    // Taille des cellules de la grille qui sert à construire les triangles : une petite valeur produit davantage de triangles et conserve plus de détails
    int const taille{12};

    // Parcours de l'image par cellules carrées de taille "taille"
    for (int x{0}; x < image.width(); x += taille)
    {
        for (int y{0}; y < image.height(); y += taille)
        {
            // Coordonnées du coin inférieur droit de la cellule
            // std::min permet de ne pas dépasser les bords de l'image.
            int const x2{std::min(x + taille, image.width() - 1)};
            int const y2{std::min(y + taille, image.height() - 1)};

            // Définition des quatre sommets de la cellule
            glm::ivec2 const a{x, y};
            glm::ivec2 const b{x2, y};
            glm::ivec2 const c{x, y2};
            glm::ivec2 const d{x2, y2};

            // Calcul de la couleur du premier triangle en prélevant un pixel proche de son centre dans l'image originale
            glm::vec3 const couleur_1{image.pixel((x + x2 + x) / 3, (y + y + y2) / 3)};

            // Calcul de la couleur du deuxième triangle en prélevant un pixel proche de son centre dans l'image originale
            glm::vec3 const couleur_2{
                image.pixel((x2 + x2 + x) / 3, (y + y2 + y2) / 3)
            };

            // On alterne le sens de la diagonale qui sépare les deux triangles d'une cellule à l'autre, pour varier la disposition des triangles
            if ((x / taille + y / taille) % 2 == 0)
            {
                dessiner_triangle(nouvelle_image, a, b, c, couleur_1);
                dessiner_triangle(nouvelle_image, b, d, c, couleur_2);
            }
            else
            {
                dessiner_triangle(nouvelle_image, a, b, d, couleur_1);
                dessiner_triangle(nouvelle_image, a, c, d, couleur_2);
            }
        }
    }
    nouvelle_image.save("images/exos_perso/low_poly.png");
}


// Exo 7 : Néon
// Détecte les contours de l'image et les colore avec la couleur choisie, sur un fond sombre, pour imiter un éclairage néon
void neon(sil::Image const& image, glm::vec3 const& couleur_neon)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    // On commence à 1 et on s'arrête avant le dernier pixel pour accéder aux voisins gauche, droite, haut et bas sans sortir de l'image
    for (int x{1}; x < image.width() - 1; x++)
    {
        for (int y{1}; y < image.height() - 1; y++)
        {
            // Calcul de la luminosité du pixel situé à gauche
            float const gauche{(image.pixel(x - 1, y).r + image.pixel(x - 1, y).g + image.pixel(x - 1, y).b) / 3.f};

            // Calcul de la luminosité du pixel situé à droite
            float const droite{(image.pixel(x + 1, y).r + image.pixel(x + 1, y).g + image.pixel(x + 1, y).b) / 3.f};

            // Calcul de la luminosité du pixel situé au-dessus
            float const haut{(image.pixel(x, y - 1).r + image.pixel(x, y - 1).g + image.pixel(x, y - 1).b) / 3.f};

            // Calcul de la luminosité du pixel situé en dessous
            float const bas{(image.pixel(x, y + 1).r + image.pixel(x, y + 1).g + image.pixel(x, y + 1).b) / 3.f};

            // Mesure de la variation de luminosité dans les directions horizontale et verticale : une forte variation indique généralement la présence d'un contour
            float const contour{std::abs(droite - gauche) + std::abs(bas - haut)};

            // Amplification de la variation puis limitation de l'intensité à 1 (pour conserver une intensité comprise entre 0 et 1)
            float const intensite{std::min(1.f, contour * 3.f)};

            // La couleur du contour est obtenue en multipliant la couleur choisie par l'intensité détectée : les contours faibles sont peu lumineux et les contours marqués sont plus visibles
            glm::vec3 const couleur{intensite * couleur_neon};

            nouvelle_image.pixel(x, y) = couleur;
        }
    }
    nouvelle_image.save("images/exos_perso/neon.png");
}


// Exo 8 : Vision thermique
// Remplace les couleurs de l'image par une palette inspirée des caméras thermiques
// La luminosité de chaque pixel détermine sa couleur dans la palette
void vision_thermique(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            // Couleur du pixel original
            glm::vec3 const couleur{image.pixel(x, y)};

            // Calcul de sa luminosité : sert à choisir la couleur thermique de remplacement
            float const luminosite = 0.2126f * couleur.r + 0.7152f * couleur.g + 0.0722f * couleur.b;

            glm::vec3 nouvelle_couleur;

            // Pour les pixels les plus sombres : transition progressive du bleu vers le violet
            if (luminosite < 0.25f)
            {
                // Normalisation de la luminosité dans l'intervalle [0, 1]
                float const t{luminosite / 0.25f};

                nouvelle_couleur = glm::vec3{0.3f * t, 0.f, 0.5f + 0.5f * t};
            }
            // Pour les luminosités intermédiaires : transition du violet vers le rouge
            else if (luminosite < 0.5f)
            {
                // Normalisation de la luminosité dans l'intervalle [0, 1]
                float const t{(luminosite - 0.25f) / 0.25f};

                nouvelle_couleur = glm::vec3{0.3f + 0.7f * t, 0.f, 1.f - t};
            }
            // Pour les luminosités plus élevées : transition du rouge vers le jaune
            else if (luminosite < 0.75f)
            {
                // Normalisation de la luminosité dans l'intervalle [0, 1]
                float const t{(luminosite - 0.5f) / 0.25f};

                nouvelle_couleur = glm::vec3{1.f, t, 0.f};
            }
            // Pour les pixels les plus lumineux : transition du jaune vers le blanc
            else
            {
                // Normalisation de la luminosité dans l'intervalle [0, 1]
                float const t{(luminosite - 0.75f) / 0.25f};

                nouvelle_couleur = glm::vec3{1.f, 1.f, t};
            }
            nouvelle_image.pixel(x, y) = nouvelle_couleur;
        }
    }
    nouvelle_image.save("images/exos_perso/vision_thermique.png");
}
