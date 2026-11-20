#include <sil/sil.hpp>
#include <cmath> //Pour cos, sin et round
#include <utility> //Pour std::swap
#include "random.hpp" //pour random_int
#include <algorithm> //Pour std::sort
#include <vector> //Pour std::vector





//Exo 1 : Rosace

void rosace (int const largeur, int const hauteur, int const rayon, int const epaisseur)
{
    //Création d'une nouvelle image aux dimensions demandées par l'utilisateur
    sil::Image image{largeur, hauteur};

    //Centre de l'image et donc du cercle central (si la largeur/hauteur est impaire alors le centre sera décalé d'un demi pixel vers la gauche/le bas car il faut une valeur entière)
    int const x_0 {largeur/2};
    int const y_0 {hauteur/2};

    //Cercle du centre
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            //Sélection des pixels constituant le cercle grâce aux inéquations du cercle
            if ( ((x-x_0)*(x-x_0) + (y-y_0)*(y-y_0) <= rayon*rayon) && ((x-x_0)*(x-x_0) + (y-y_0)*(y-y_0) > (rayon-epaisseur)*(rayon-epaisseur)) )
            {
                image.pixel(x, y) = glm::vec3{1.f};
            }
        }
    }

    //Les 6 cercles autour dont les centres ont pour coordonnées (x=r*cos(teta_i)+x_0 , y=r*sin(teta_i)+y_0) avec teta_i = pi/3*i
    for (int i{0}; i < 6; i++)
    {
        int const x_centre{static_cast<int>(std::round(rayon * std::cos(std::numbers::pi/3*i) + x_0))};
        int const y_centre{static_cast<int>(std::round(rayon * std::sin(std::numbers::pi/3*i) + y_0))};

        for (int x{0}; x < image.width(); x++)
        {
            for (int y{0}; y < image.height(); y++)
            {
                if ( ((x-x_centre)*(x-x_centre) + (y-y_centre)*(y-y_centre) <= rayon*rayon) && ((x-x_centre)*(x-x_centre) + (y-y_centre)*(y-y_centre) > (rayon-epaisseur)*(rayon-epaisseur)) )
                {
                    image.pixel(x, y) = glm::vec3{1.f};
                }
            }
        }
    }
    image.save("images/difficulte-3/rosace.png");
}





//Exo 2 : Mosaïque

void mosaique (int const nb_largeur, int const nb_hauteur, sil::Image const& image)
{
    //L'image finale contient nb_largeur * nb_hauteur copies de l'image originale
    sil::Image image_mos{nb_largeur*image.width(), nb_hauteur*image.height()};

    //Position de chaque copie
    for (int i{0}; i < nb_largeur; i++)
    {
        for (int j{0}; j < nb_hauteur; j++)
        {
            //On recopie chaque pixel de l'image originale
            for (int x{0}; x < image.width(); x++)
            {
                for (int y{0}; y < image.height(); y++)
                {
                    //On décale les coordonnées du pixel selon la copie
                    image_mos.pixel(x + image.width()*i, y + image.height()*j) = image.pixel(x,y);
                }
            }
        }
    }
    image_mos.save("images/difficulte-3/mosaique.png");
}





//Exo 3 : Mosaïque en miroir

void mosaique_miroir (int const nb_largeur, int const nb_hauteur, sil::Image const& image)
{
    sil::Image image_mos_mir{nb_largeur*image.width(), nb_hauteur*image.height()};

    for (int i{0}; i < nb_largeur; i++)
    {
        for (int j{0}; j < nb_hauteur; j++)
        {
            for (int x{0}; x < image.width(); x++)
            {
                for (int y{0}; y < image.height(); y++)
                {
                    //Ni miroir horizontal ni miroir vertical
                    if((i%2==0) && (j%2==0))
                    {
                        image_mos_mir.pixel(x + image.width()*i, y + image.height()*j) = image.pixel(x,y);
                    }

                    //Miroir horizontal
                    else if((i%2==1) && (j%2==0))
                    {
                        image_mos_mir.pixel(x + image.width()*i, y + image.height()*j) = image.pixel(image.width()-1-x, y);
                    }

                    //Miroir vertical
                    else if((i%2==0) && (j%2==1))
                    {
                        image_mos_mir.pixel(x + image.width()*i, y + image.height()*j) = image.pixel(x, image.height()-1-y);
                    }

                    //Miroir horizontal et vertical
                    else
                    {
                        image_mos_mir.pixel(x + image.width()*i, y + image.height()*j) = image.pixel(image.width()-1-x, image.height()-1-y);
                    }
                }
            }
        }

    }
    image_mos_mir.save("images/difficulte-3/mosaique_miroir.png");
}





//Exo 4 : Glitch

void glitch(sil::Image& image, int const max_nb_effet)
{
    //Nombre aléatoire de zones à échanger (bornées par max_nb_effet pour que l'image reste lisible)
    int const nb_glitch = random_int(1, max_nb_effet + 1);
    
    for (int i{0}; i < nb_glitch; i++)
    {   
        //Dimensions aléatoires de la zone à déplacer
        int const largeur_glitch = random_int(1, image.width()/10);
        int const hauteur_glitch = random_int(1, image.height()/10);

        //Coordonnées du point initial pour chacune des 2 zones
        int const x_1{random_int(0, image.width() - largeur_glitch + 1)};
        int const y_1{random_int(0, image.height() - hauteur_glitch + 1)};
        
        int const x_2{random_int(0, image.width() - largeur_glitch + 1)};
        int const y_2{random_int(0, image.height() - hauteur_glitch + 1)};
        
        //Echange des pixels des 2 zones
        for (int x{0}; x < largeur_glitch; x++)
        {
            for (int y{0}; y < hauteur_glitch; y++)
            {
                std::swap(image.pixel(x_1 + x, y_1 + y) , image.pixel(x_2 + x, y_2 + y));
            }
        }
    }
}





//Exo 5 : Tri de pixels

//Fonction pour calculer la luminosité d'un pixel
float luminosite(glm::vec3 const& color)
{
    //Formule de la luminosité trouvée sur internet
    return 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
}


//Tri de tous les pixels par luminosité croissante
void tri_tous_pixels_luminosite(sil::Image& image)
{
    std::vector<glm::vec3> tri_pix{};

    // Récupération de tous les pixels
    for (int y{0}; y < image.height(); ++y)
    {
        for (int x{0}; x < image.width(); ++x)
        {
            tri_pix.push_back(image.pixel(x, y));
        }
    }

    // Tri par luminosité croissante
    std::sort(tri_pix.begin(), tri_pix.end(), [](glm::vec3 const& color1, glm::vec3 const& color2)
        {
            return luminosite(color1) < luminosite(color2);
        }
    );

    // Replacement des pixels triés dans l'image
    for (int y{0}; y < image.height(); ++y)
    {
        for (int x{0}; x < image.width(); ++x)
        {
            image.pixel(x, y) = tri_pix[y * image.width() + x];
        }
    }
}


//Tri des pixels par colonne par valeur du canal rouge croissante
void tri_pixels_colonne_rouge(sil::Image& image)
{
    for (int x{0}; x < image.width(); x++)
    {
        std::vector<glm::vec3> colonne_image{};

        //Récupération de tous les pixels de la colonne
        for (int y{0}; y < image.height(); y++)
        {
            colonne_image.push_back(image.pixel(x,y));
        }

        //Tri selon la valeur du canal rouge
        std::sort(colonne_image.begin(), colonne_image.end(), [](glm::vec3 const& color1, glm::vec3 const& color2)
        {
            return color1.r < color2.r;
        });

        // Replacement des pixels triés dans la colonne
        for (int y{0}; y < image.height(); y++)
        {
            image.pixel(x,y) = colonne_image[y];
        }
    }
}


//Tri des pixels par ligne par valeur du canal bleu croissante
void tri_pixels_ligne_bleu(sil::Image& image)
{
    for (int y{0}; y < image.height(); y++)
    {
        std::vector<glm::vec3> ligne_image{};

        //Récupération de tous les pixels de la ligne
        for (int x{0}; x < image.width(); x++)
        {
            ligne_image.push_back(image.pixel(x,y));
        }

        //Tri selon la valeur du canal bleu
        std::sort(ligne_image.begin(), ligne_image.end(), [](glm::vec3 const& color1, glm::vec3 const& color2)
        {
            return color1.b < color2.b;
        });

        // Replacement des pixels triés dans la ligne
        for (int x{0}; x < image.width(); x++)
        {
            image.pixel(x,y)=ligne_image[x];
        }
    }
}


//Tri de pixels aléatoires par ligne par valeur du canal vert décroissante
void tri_pixels_aleatoire_ligne_vert_dec(sil::Image& image, int const max_nb_effet)
{
    //Nombre aléatoire de portions de lignes à trier
    int const nb_tri = random_int(100, max_nb_effet*100);
    
    for (int i{0}; i < nb_tri; i++)
    {   
        std::vector<glm::vec3> ligne_image{};

        //Choix aléatoire d'une ligne
        int const y = random_int(0, image.height());

        //Choix aléatoire de la longueur de la portion à trier (pas trop longue pour conserver la lisibilité de l'image donc on divise par 10)
        int const largeur_tri = random_int(1, image.width()/10);

        //Choix aléatoire du début de la portion à trier
        int const x_0 {random_int(0, image.width() - largeur_tri + 1)};

        //Récupération des pixels de la portion choisie
        for (int x{x_0}; x < x_0 + largeur_tri; x++)
        {
            ligne_image.push_back(image.pixel(x,y));
        }

        //Tri par valeur du canal vert décroissante
        std::sort(ligne_image.begin(), ligne_image.end(), [](glm::vec3 const& color1, glm::vec3 const& color2)
        {
            return color1.g > color2.g;
        });

        // Replacement des pixels triés à leur emplacement initial
        for (int x{x_0}; x < x_0 + largeur_tri; x++)
        {
            image.pixel(x,y)=ligne_image[x - x_0];
        }
    }       
}