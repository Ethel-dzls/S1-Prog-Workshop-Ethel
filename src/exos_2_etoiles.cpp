#include <sil/sil.hpp>
#include <utility> //Pour std::swap
#include "random.hpp" //Pour random_int et random_float
#include <sstream> //Pour construire le nom des images de l'animation
#include <iomanip> //Pour afficher les numéros d'images avec des 0





//Exo 1 : Miroir

void miroir(sil::Image& image)
{
    //On parcourt uniquement la moitié gauche de l'image : chaque pixel est échangé avec son symétrique à droite
    for (int x{0}; x < image.width() / 2; x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            std::swap(image.pixel(x, y) , image.pixel(image.width() - 1 - x, y));
        }
    }
}





//Exo 2 : Image bruitée

void image_bruitee(sil::Image& image, int const max_nb_effet)
{
    //Il ne s'agit pas du nombre précis de pixels différents mais du nombre de fois où on associe une valeur aléatoire à un pixel
    //Donc, le même pixel peut-être modifié plusieurs fois
    //On prend une valeur entre 1000 et max_nb_effet*1000 pour que l'effet soit très visible (la valeur de max_nb_effet étant assez petite car elle sert dans d'autres fonctions)
    int const nb_pixels_bruit = random_int(1000, max_nb_effet*1000);
    
    for (int i{0}; i < nb_pixels_bruit; i++)
    {
        //On choisit une position aléatoire dans toute l'image (la valeur maximale est exclue)
        int const x{random_int(0, image.width())};
        int const y{random_int(0, image.height())};

        //On attribue une couleur aléatoire au pixel
        image.pixel(x,y) = glm::vec3{random_float(0.f, 1.f) , random_float(0.f, 1.f) , random_float(0.f, 1.f)};
    }
}





//Exo 3 : Rotation de 90°

//Rotation de 90° dans le sens direct :
void rotation_90(sil::Image& image)
{
    //Rotation de 90° donc largeur et hauteur inversées
    sil::Image nv_image{image.height(), image.width()};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            //Nouvelle position du pixel
            nv_image.pixel(image.height() - 1 - y, x) = image.pixel(x, y);
        }
    }
    image = nv_image;
}

//Rotation de 90° dans le sens indirect : 
void rotation_270(sil::Image& image)
{
    sil::Image nv_image{image.height(), image.width()};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            nv_image.pixel(y, image.width() - 1 - x) = image.pixel(x, y);
        }
    }
    image = nv_image;
}





//Exo 4 : RGB split

void rgb_split(sil::Image& image, int decalage)
{
    //On fait une copie de l'image originale pour que les déplacements des canaux ne se basent pas sur des pixels déjà modifiés
    sil::Image image_originale = image;

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            // Le vert ne bouge pas
            image.pixel(x, y).g = image_originale.pixel(x, y).g;

            // Le rouge vient de la droite
            if (x + decalage < image.width())
                image.pixel(x, y).r = image_originale.pixel(x + decalage, y).r;
            else
                image.pixel(x, y).r = 0.f;

            // Le bleu vient de la gauche
            if (x - decalage >= 0)
                image.pixel(x, y).b = image_originale.pixel(x - decalage, y).b;
            else
                image.pixel(x, y).b = 0.f;
        }
    }
}





//Exo 5 : Luminosité

void eclaircir(sil::Image& image)
{
    for (glm::vec3& color : image.pixels())
    {
        color = glm::sqrt(color);
    }
}


void assombrir(sil::Image& image)
{
    for (glm::vec3& color : image.pixels())
    {
        color *= color;
    }
}





//Exo 6 : Disque

void disque(int const rayon)
{
    //Taille de l'image définie
    sil::Image image{500, 500};

    //Centre de l'image
    int const x_0 {250};
    int const y_0 {250};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            //Sélection des pixels à l'intérieur du disque grâce à l'inéquation du disque
            if ( (x-x_0)*(x-x_0) + (y-y_0)*(y-y_0) <= rayon*rayon){
                image.pixel(x, y) = glm::vec3{1.f};
            }
        }
    }
    image.save("images/difficulte-2/disque.png");
}





//Exo 7 : Cercle

void cercle(int const rayon, int const epaisseur)
{
    sil::Image image{500, 500};

    //Centre de l'image
    int const x_0 {250};
    int const y_0 {250};

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
    image.save("images/difficulte-2/cercle.png");
}





//Exo 8 : Animation disques

void animation_disques(int rayon)
{
    int const hauteur{500};
    int const largeur{500};

    //Position verticale fixe du centre du disque au milieu de l'image
    int const y_0{hauteur / 2};

    //On crée une image à chaque position du cercle (juste avant le bord gauche à juste après le bord droit)
    for (int i{0}; i<=largeur+2*rayon; i++)
    {
        sil::Image image{largeur, hauteur};
        //Position horizontale du centre du disque
        int const x_0{i - rayon};

        for (int x{0}; x < image.width(); x++)
        {
            for (int y{0}; y < image.height(); y++)
            {
                //Sélection des pixels constituant le disque grâce à l'inéquation du disque
                if ( (x-x_0)*(x-x_0) + (y-y_0)*(y-y_0) <= rayon*rayon)
                {
                    image.pixel(x, y) = glm::vec3{1.f};
                }
            }
        }

        //Sauvegarde de toutes les images (rédaction trouvée sur internet)
        std::ostringstream nom_fichier;
        nom_fichier << "images/difficulte-2/animation/frame_"
                    << std::setw(4) << std::setfill('0') << i
                    << ".png";

        image.save(nom_fichier.str());
    }
}