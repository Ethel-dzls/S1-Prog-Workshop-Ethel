#include <sil/sil.hpp>
#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>
#include <array>


//Vérifier si les coordonnées correspondent à un pixel existant
bool est_dans_image(int x, int y, sil::Image const& image)
{
    return x >= 0 && x < image.width()
        && y >= 0 && y < image.height();
}


//Exo 1 : Effet fisheye
void fisheye(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    //Centre de l'image (décalée d'un demi pixel vers la gauche/le bas si la largeur/hauteur est impaire)
    glm::vec2 const centre {image.width()/2, image.height()/2};
   
    //Plus grande distance entre le centre et un pixel de l'image
    float const distance_max = glm::distance(centre, glm::vec2{0.f, 0.f});

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            //Calcul de la position du pixel par rapport au centre
            float const dx{x - centre.x};
            float const dy{y - centre.y};

            //Calcul de sa distance au centre
            float const distance{std::sqrt(dx * dx + dy * dy)};

            //Valeur relative : 0 au centre, 1 dans le coin
            float const rayon{distance / distance_max};

            //PLus on s'éloigne du centre plus il y a déformation
            float const facteur{1.f - 0.85f * rayon * rayon};

            //Coordonnées du pixel source
            int const source_x{static_cast<int>(std::round(centre.x + dx * facteur))};            
            int const source_y {static_cast<int>(std::round(centre.y + dy * facteur))};

            //vérification de la validité du pixel
            if (est_dans_image(source_x, source_y, image))
            {
                nouvelle_image.pixel(x, y) = image.pixel(source_x, source_y);
            }
        }
    }
    nouvelle_image.save("images/exos_perso/fisheye.png");
}


//Exo 2 : Ondulations
void ondulations(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    //Paramètres des ondulations : 
    float const amplitude{15.f}; //distance maximale du décalage de pixels
    float const frequence{0.04f};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            //Décalage horizontal
            int const source_x{static_cast<int>(std::round(x + amplitude * std::sin(y * frequence)))};

            //Décalage vertical
            int const source_y{static_cast<int>(std::round(y + amplitude * std::sin(x * frequence)))};

            //vérification de la validité du pixel
            if (est_dans_image(source_x, source_y, image))
            {
                nouvelle_image.pixel(x, y) = image.pixel(source_x, source_y);
            }
        }
    }
    nouvelle_image.save("images/exos_perso/ondulations.png");
}


//Exo 3 : Peinture à l'huile
void peinture_huile(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    int const rayon{6};
    int const nb_niveaux{6};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            // Tableau qui compte le nombre de pixels dans chaque niveau de luminosité
            std::array<int, 6> nombres{};
            std::array<glm::vec3, 6> couleurs{};

            for (int dx{-rayon}; dx <= rayon; dx++)
            {
                for (int dy{-rayon}; dy <= rayon; dy++)
                {
                    int const px{x + dx};
                    int const py{y + dy};

                    if (est_dans_image(px, py, image))
                    {
                        glm::vec3 const couleur{image.pixel(px, py)};

                        float const luminosite{
                            (couleur.r + couleur.g + couleur.b) / 3.f
                        };

                        int const niveau{
                            std::min(nb_niveaux - 1,
                                static_cast<int>(luminosite * nb_niveaux))
                        };

                        nombres[niveau]++;
                        couleurs[niveau] += couleur;
                    }
                }
            }

            // Recherche du niveau le plus fréquent
            int niveau_max{0};

            for (int i{1}; i < nb_niveaux; i++)
            {
                if (nombres[i] > nombres[niveau_max])
                {
                    niveau_max = i;
                }
            }

            // Moyenne des couleurs du niveau le plus fréquent
            nouvelle_image.pixel(x, y) =
                couleurs[niveau_max] / static_cast<float>(nombres[niveau_max]);
        }
    }

    nouvelle_image.save("images/exos_perso/peinture_huile.png");
}


// Exo 4 : Pointillisme
void pointillisme(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    // Fond blanc
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            nouvelle_image.pixel(x, y) = glm::vec3{1.f};
        }
    }

    int const espacement{4};
    int const rayon{2};

    for (int x{rayon}; x < image.width(); x += espacement)
    {
        for (int y{rayon}; y < image.height(); y += espacement)
        {
            // Couleur du point
            glm::vec3 const couleur{image.pixel(x, y)};

            // Dessin d'un cercle
            for (int dx{-rayon}; dx <= rayon; dx++)
            {
                for (int dy{-rayon}; dy <= rayon; dy++)
                {
                    if (dx * dx + dy * dy <= rayon * rayon)
                    {
                        int const px{x + dx};
                        int const py{y + dy};

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
void hachures(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    int const espacement{8};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            glm::vec3 const couleur{image.pixel(x, y)};

            float const luminosite{
                (couleur.r + couleur.g + couleur.b) / 3.f
            };

            // On commence avec un fond blanc
            float valeur{1.f};

            // Plus la zone est sombre, plus on ajoute de hachures
            if (luminosite < 0.75f && (x + y) % espacement < 2)
            {
                valeur = 0.f;
            }

            if (luminosite < 0.5f && (x - y + image.height()) % espacement < 2)
            {
                valeur = 0.f;
            }

            if (luminosite < 0.25f && x % espacement < 2)
            {
                valeur = 0.f;
            }

            nouvelle_image.pixel(x, y) = glm::vec3{valeur};
        }
    }

    nouvelle_image.save("images/exos_perso/hachures.png");
}



// Dessiner un triangle dans une image
void dessiner_triangle(
    sil::Image& destination,
    glm::ivec2 a,
    glm::ivec2 b,
    glm::ivec2 c,
    glm::vec3 couleur)
{
    int const min_x{std::max(0, std::min({a.x, b.x, c.x}))};
    int const max_x{std::min(destination.width() - 1, std::max({a.x, b.x, c.x}))};
    int const min_y{std::max(0, std::min({a.y, b.y, c.y}))};
    int const max_y{std::min(destination.height() - 1, std::max({a.y, b.y, c.y}))};

    for (int x{min_x}; x <= max_x; x++)
    {
        for (int y{min_y}; y <= max_y; y++)
        {
            float const d1{
                static_cast<float>((x - b.x) * (a.y - b.y) - (a.x - b.x) * (y - b.y))
            };

            float const d2{
                static_cast<float>((x - c.x) * (b.y - c.y) - (b.x - c.x) * (y - c.y))
            };

            float const d3{
                static_cast<float>((x - a.x) * (c.y - a.y) - (c.x - a.x) * (y - a.y))
            };

            bool const negatif{d1 < 0 || d2 < 0 || d3 < 0};
            bool const positif{d1 > 0 || d2 > 0 || d3 > 0};

            if (!(negatif && positif))
            {
                destination.pixel(x, y) = couleur;
            }
        }
    }
}


// Exo 6 : Low Poly
void low_poly(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    int const taille{15};

    for (int x{0}; x < image.width(); x += taille)
    {
        for (int y{0}; y < image.height(); y += taille)
        {
            int const x2{std::min(x + taille, image.width() - 1)};
            int const y2{std::min(y + taille, image.height() - 1)};

            glm::ivec2 const a{x, y};
            glm::ivec2 const b{x2, y};
            glm::ivec2 const c{x, y2};
            glm::ivec2 const d{x2, y2};

            // Couleur des deux triangles
            glm::vec3 const couleur_1{
                image.pixel((x + x2 + x) / 3, (y + y + y2) / 3)
            };

            glm::vec3 const couleur_2{
                image.pixel((x2 + x2 + x) / 3, (y + y2 + y2) / 3)
            };

            // On alterne le sens des diagonales
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
void neon(sil::Image const& image, glm::vec3 const& couleur_neon)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    for (int x{1}; x < image.width() - 1; x++)
    {
        for (int y{1}; y < image.height() - 1; y++)
        {
            // Luminosité du pixel et de ses voisins
            float const gauche{
                (image.pixel(x - 1, y).r + image.pixel(x - 1, y).g + image.pixel(x - 1, y).b) / 3.f
            };

            float const droite{
                (image.pixel(x + 1, y).r + image.pixel(x + 1, y).g + image.pixel(x + 1, y).b) / 3.f
            };

            float const haut{
                (image.pixel(x, y - 1).r + image.pixel(x, y - 1).g + image.pixel(x, y - 1).b) / 3.f
            };

            float const bas{
                (image.pixel(x, y + 1).r + image.pixel(x, y + 1).g + image.pixel(x, y + 1).b) / 3.f
            };

            // Intensité du contour
            float const contour{
                std::abs(droite - gauche) + std::abs(bas - haut)
            };

            // On limite l'intensité entre 0 et 1
            float const intensite{std::min(1.f, contour * 3.f)};

            // Couleur néon bleu et violet
            glm::vec3 const couleur{intensite * couleur_neon};

            nouvelle_image.pixel(x, y) = couleur;
        }
    }
    nouvelle_image.save("images/exos_perso/neon.png");
}


// Exo 8 : Vision thermique
void vision_thermique(sil::Image const& image)
{
    sil::Image nouvelle_image{image.width(), image.height()};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            glm::vec3 const couleur{image.pixel(x, y)};

            // Luminosité du pixel
            float const luminosite{
                (couleur.r + couleur.g + couleur.b) / 3.f
            };

            glm::vec3 nouvelle_couleur;

            // Passage du bleu au violet
            if (luminosite < 0.25f)
            {
                float const t{luminosite / 0.25f};

                nouvelle_couleur = glm::vec3{
                    0.3f * t,
                    0.f,
                    0.5f + 0.5f * t
                };
            }
            // Passage du violet au rouge
            else if (luminosite < 0.5f)
            {
                float const t{(luminosite - 0.25f) / 0.25f};

                nouvelle_couleur = glm::vec3{
                    0.3f + 0.7f * t,
                    0.f,
                    1.f - t
                };
            }
            // Passage du rouge au jaune
            else if (luminosite < 0.75f)
            {
                float const t{(luminosite - 0.5f) / 0.25f};

                nouvelle_couleur = glm::vec3{
                    1.f,
                    t,
                    0.f
                };
            }
            // Passage du jaune au blanc
            else
            {
                float const t{(luminosite - 0.75f) / 0.25f};

                nouvelle_couleur = glm::vec3{
                    1.f,
                    1.f,
                    t
                };
            }

            nouvelle_image.pixel(x, y) = nouvelle_couleur;
        }
    }

    nouvelle_image.save("images/exos_perso/vision_thermique.png");
}