#include <complex> //Pour l'utilisation des nombres complexes
#include <sil/sil.hpp> 
#include <cmath> //Pour pow, cbrt, abs, round
#include <glm/gtx/matrix_transform_2d.hpp> //Pour la rotation du vortex





//Exo 1 : Fractale de Mandelbrot

void fractale_Mandelbrot(int const largeur, int const hauteur)
{
    sil::Image image{largeur, hauteur};

    //Nombre maximal d'itérations pour chaque pixel
    int const iterations_max{100};

    for (int x{0}; x < largeur; x++)
    {
        for (int y{0}; y < hauteur; y++)
        {
            //Transformation des coordonnées en nombres complexes
            std::complex<float> c{
                x * 4.f / largeur - 2.f,
                y * 4.f / hauteur - 2.f
            };

            //Valeur de départ de z
            std::complex<float> z{0.f, 0.f};
            int iteration{0};

            //On applique z = z² + c tant que le module de z ne dépasse pas 2 et que le nombre maximal d'itérations n'a pas été atteint
            while (std::abs(z) <= 2.f && iteration < iterations_max)
            {
                z = z * z + c;
                iteration += 1;
            }

            //Si le module de z ne dépasse toujours pas 2,alors le point appartient à l'ensemble et est donc blanc
            if (std::abs(z) <= 2.f)
            {
                image.pixel(x, y) = glm::vec3{1.f};
            }

            //Sinon la nuance de gris dépend du nombre d'itérations nécessaires pour que le point s'échappe 
            else
            {
                float const gris =
                    static_cast<float>(iteration) / iterations_max;

                image.pixel(x, y) = glm::vec3{gris};
            }
        }
    }
    image.save("images/difficulte-4/fractale_Mandelbrot.png");
}





//Exo 2 : Dégradé dans l'espace de couleur lab

// Fonction de base en sRGB pour faire le dégradé rouge-bleu
void degrade_mix_rb(int const largeur, int const hauteur)
{
    sil::Image image {largeur, hauteur};

    for (int x{0}; x < largeur; x++)
    {
        //Coefficient qui varie de 0 à 1 de gauche à droite
        float const coef{
            static_cast<float>(x) / (largeur - 1)
        };

        for (int y{0}; y < hauteur; y++)
        {
            //Le rouge diminue progressivement et le bleu augmente
            image.pixel(x, y) = glm::vec3{1 - coef, 0.f, coef};
        }
    }
    image.save("images/difficulte-4/degrade_mix_rb.png");
}


//Dégradé rouge-bleu avec lab : 

//Structure pour la représentation des couleurs dans les différents espaces
struct Lab {float L; float a; float b;};
struct LinearRGB {float r; float g; float b;};


//Conversion de sRGB vers Linear RGB :
LinearRGB srgb_vers_linear(glm::vec3 const& couleur)
{
    glm::vec3 conversion{};

    //Chaque canal est converti séparément
    for (int i{0}; i < 3; i++)
    {
        if (couleur[i] <= 0.04045f)
        {
            conversion[i] = couleur[i] / 12.92f;
        }
        else
        {
            conversion[i] = std::pow(
                (couleur[i] + 0.055f) / 1.055f,
                2.4f
            );
        }
    }

    //On transforme le glm::vec3 en structure LinearRGB
    LinearRGB conversion_srgb_lin{conversion.r, conversion.g, conversion.b};

    return conversion_srgb_lin;
}


//Conversion de Linear RGB vers sRGB :
glm::vec3 linear_vers_srgb(LinearRGB const& couleur)
{
    glm::vec3 conversion_lin_srgb{};

    //création d'un vecteur pour pouvoir parcourir les 3 canaux
    glm::vec3 const conversion{couleur.r, couleur.g, couleur.b};

    //Chaque canal est converti séparément
    for (int i{0}; i < 3; i++)
    {
        if (conversion[i] <= 0.0031308f)
        {
            conversion_lin_srgb[i] = 12.92f * conversion[i];
        }
        else
        {
            conversion_lin_srgb[i] = 1.055f * std::pow(conversion[i], 1.f/2.4f) - 0.055;
        }
    }
    return conversion_lin_srgb;
}


//Conversion de Linear RGB vers Oklab :
Lab linear_vers_oklab(LinearRGB const& couleur)
{
    //Calcul intermédiaire de l, m et s :
    float const l = 0.4122214708f * couleur.r + 0.5363325363f * couleur.g + 0.0514459929f * couleur.b;
    float const m = 0.2119034982f * couleur.r + 0.6806995451f * couleur.g + 0.1073969566f * couleur.b;
    float const s = 0.0883024619f * couleur.r + 0.2817188376f * couleur.g + 0.6299787005f * couleur.b;

    //Calcul intermédiaire de la racine cubique de l, m et s :
    float const l_ = std::cbrt(l);
    float const m_ = std::cbrt(m);
    float const s_ = std::cbrt(s);

    //Calcul des 3 composantes de l'espace OkLab L, a et b :
    float const L = 0.2104542553f*l_ + 0.7936177850f*m_ - 0.0040720468f*s_;
    float const a = 1.9779984951f*l_ - 2.4285922050f*m_ + 0.4505937099f*s_;
    float const b = 0.0259040371f*l_ + 0.7827717662f*m_ - 0.8086757660f*s_;

    Lab conversion_lin_oklab {L, a, b};

    return conversion_lin_oklab;
}


//Conversion de Oklab vers Linear RGB :
LinearRGB oklab_vers_linear(Lab const& couleur)
{
    //Calcul intermédiaire des racines cubiques : 
    float const l_ = couleur.L + 0.3963377774f * couleur.a + 0.2158037573f * couleur.b;
    float const m_ = couleur.L - 0.1055613458f * couleur.a - 0.0638541728f * couleur.b;
    float const s_ = couleur.L - 0.0894841775f * couleur.a - 1.2914855480f * couleur.b;

    //Calcul intermédiaire de l, m et s : 
    float const l = l_*l_*l_;
    float const m = m_*m_*m_;
    float const s = s_*s_*s_;

    //Calcul des canaux rouge, vert et bleu :
    float const r = 4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s;
	float const g = -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s;
	float const b = -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s;

    LinearRGB conversion_oklab_lin {r, g, b};

    return conversion_oklab_lin;
}


//Fonction dégradé avec toutes les conversions : 
void degrade_oklab_rb(int const largeur, int const hauteur)
{
    sil::Image image{largeur, hauteur};

    //Couleurs de départ en sRGB
    glm::vec3 rouge{1.f, 0.f, 0.f};
    glm::vec3 bleu{0.f, 0.f, 1.f};

    //Conversion des 2 couleurs en Linear RGB
    LinearRGB rouge_linear = srgb_vers_linear(rouge);
    LinearRGB bleu_linear = srgb_vers_linear(bleu);

    //Conversion des 2 couleurs vers OkLab
    Lab rouge_oklab = linear_vers_oklab(rouge_linear);
    Lab bleu_oklab = linear_vers_oklab(bleu_linear);

    for (int x{0}; x < largeur; x++)
    {
        float const coef = static_cast<float>(x) / (largeur - 1);

        Lab couleur_oklab{
            rouge_oklab.L * (1.f - coef) + bleu_oklab.L * coef,
            rouge_oklab.a * (1.f - coef) + bleu_oklab.a * coef,
            rouge_oklab.b * (1.f - coef) + bleu_oklab.b * coef
        };

        //Conversion vers Linear RGB
        LinearRGB couleur_linear = oklab_vers_linear(couleur_oklab);

        //Conversion vers sRGB
        glm::vec3 couleur_srgb = linear_vers_srgb(couleur_linear);

        //Tous les pixels de la colonne ont la même couleur
        for (int y{0}; y < hauteur; y++)
        {
            image.pixel(x, y) = couleur_srgb;
        }
    }
    image.save("images/difficulte-4/degrade_oklab_rb.png");
}





//Exo 3 : Tramage (ordered dithering)

void ordered_dithering (sil::Image& image)
{
    //Matrice de Bayer trouvée sur internet
    int const bayer[4][4]
    {
    { 0,  8,  2, 10},
    {12,  4, 14,  6},
    { 3, 11,  1,  9},
    {15,  7, 13,  5}
    };

    for (int x{0}; x < image.width(); x++)
    {
        for(int y{0}; y < image.height(); y++)
        {
            //Calcul de la luminosité du pixel
            float const luminosite = 0.2126f * image.pixel(x,y).r + 0.7152f * image.pixel(x,y).g + 0.0722f * image.pixel(x,y).b;
 
            //Position du pixel dans la matrice de Bayer
            int const ligne = y % 4;  
            int const colonne = x % 4; 
    
            //Normalistation et centrage de la valeur de la matrice
            float const bayer_value = bayer[ligne][colonne] / 16.f - 0.5f;
            
            //Modification du seuil selon la position du pixel
            float const couleur_ajustee = luminosite - bayer_value;

            //Transformation du pixel en blanc ou en noir
            if (couleur_ajustee >= 0.5f)
            {
                image.pixel(x,y) = glm::vec3{1.f};
            }
            else
            {
                image.pixel(x,y) = glm::vec3{0.f};
            }
        }
    }
}





//Exo 4 : Normalisation de l'historigramme

void normalisation_historigramme (sil::Image& image)
{
    //Initialisation des luminosités minimale et maximale avec le 1er pixel
    float luminosite_min = 0.2126f * image.pixel(0,0).r + 0.7152f * image.pixel(0,0).g + 0.0722f * image.pixel(0,0).b;
    float luminosite_max = luminosite_min;

    //Recherche des luminosités minimale et maximale dans toute l'image
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            float const luminosite = 0.2126f * image.pixel(x,y).r + 0.7152f * image.pixel(x,y).g + 0.0722f * image.pixel(x,y).b;
 
            if (luminosite > luminosite_max)
            {
                luminosite_max = luminosite;
            }

            else if (luminosite < luminosite_min)
            {
                luminosite_min = luminosite;
            }
        }
    }

    //Si tous les pixels ont la même luminosité il n'y a pas de contraste à étirer
    if (luminosite_max == luminosite_min)
    {
        return;
    }

    //Modification de chaque pixel
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            //Calcul de la luminosité initiale du pixel
            float const luminosite = 0.2126f * image.pixel(x,y).r + 0.7152f * image.pixel(x,y).g + 0.0722f * image.pixel(x,y).b;
            
            //Etirement de la luminosité entre 0 et 1
            float const nv_luminosite = (luminosite - luminosite_min) / (luminosite_max - luminosite_min);
        
            //Si le pixel est noir, on ne le change pas (on ne divise pas par 0)
            if (luminosite > 0.f)
            {
                //On multiplie les 3 canaux par le même coefficient pour conserver les proportions
                image.pixel(x,y) *= nv_luminosite / luminosite;
            }

        }    
    }
}





//Exo 5 : Vortex

//Fonction pour faire la rotation
glm::vec2 rotated(glm::vec2 point, glm::vec2 center_of_rotation, float angle)
{
    return glm::vec2{glm::rotate(glm::mat3{1.f}, angle) * glm::vec3{point - center_of_rotation, 0.f}} + center_of_rotation;
}


void vortex (sil::Image& image, float const angle_max)
{
    sil::Image nv_image {image.width(), image.height()};

    //Centre de l'image utilisé comme centre de rotation
    glm::vec2 const centre_rotation {image.width()/2, image.height()/2};
   
    //Plus grande distance entre le centre et un pixel de l'image
    float const distance_max = glm::distance(centre_rotation, glm::vec2{0.f, 0.f});

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            //Position du pixel de destination
            glm::vec2 point {x,y};

            //Distance entre ce pixel et le centre de l'image
            float const distance = glm::distance(point, centre_rotation);
            
            //Plus le pixel est loin du centre, plus l'angle de rotation est important
            float const angle = angle_max * distance / distance_max;

            //On cherche le pixel correspondant dans l'image originale
            glm::vec2 nv_coord = rotated(point, centre_rotation, angle);

            //On vérifie que le pixel source se trouve bien dans l'image
            if (nv_coord.x >= 0.f && nv_coord.x < image.width()
                && nv_coord.y >= 0.f && nv_coord.y < image.height())
            {
                //on copie la couleur du pixel source dans le pixel de destination
                nv_image.pixel(x, y) = image.pixel(nv_coord.x, nv_coord.y);
            }
        }
    }
    image = nv_image;
}





//Exo 6 : Convolutions 

//Formule générale pour toutes les convolutions pour des kernel de taille 3x3
void convolution(sil::Image& image, float const kernel[3][3])
{
    sil::Image nv_image{image.width(), image.height()};

    int const taille_kernel = 3;

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            //Somme des couleurs pondérées par les coefficients du kernel
            glm::vec3 somme{0.f};

            for (int i{0}; i < taille_kernel; i++)
            {
                for (int j{0}; j < taille_kernel; j++)
                {
                    //Coordonnées du pixel voisin dans l'image                    
                    int const voisin_x = x + j - 1;
                    int const voisin_y = y + i - 1;

                    //On vérifie que le voisin se trouve dans l'image
                    if (voisin_x >= 0 && voisin_x < image.width() 
                    && voisin_y >= 0 && voisin_y < image.height())
                    {
                        //On multiplie la couleur du voisin par le coefficient correspondant du kernel
                        somme += image.pixel(voisin_x, voisin_y) * kernel [i][j];
                    }
                }
            }
            nv_image.pixel(x,y) = somme;
        }
    }
    image = nv_image;
}


//Généralisation à n'importe quelle taille de kernel pour le flou 
void flou_generalise (sil::Image& image, int const taille_kernel)
{
    sil::Image nv_image {image.width(), image.height()};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            //Somme des couleurs des pixels voisins
            glm::vec3 somme{0.f};

            //Nombre de pixels pris en compte (diminue plus il est près des bords de l'image)
            int nb_voisins{0};

            //Parcours des décalages autour du pixel central
            for (int dx{-taille_kernel/2}; dx <= taille_kernel/2; dx++)
            {
                for (int dy{-taille_kernel/2}; dy <= taille_kernel/2; dy++)
                {
                    int const voisin_x = x +dx;
                    int const voisin_y = y + dy;

                    //On ignore les positions situées hors de l'image
                    if (voisin_x >= 0 && voisin_x < image.width() && voisin_y >= 0 && voisin_y < image.height())
                    {
                        somme += image.pixel(voisin_x, voisin_y);
                        nb_voisins +=1;
                    }
                }
            }
        //Moyenne des couleurs des voisins valides
        nv_image.pixel(x,y) = somme / static_cast<float>(nb_voisins);
        }
    }
    image = nv_image;
}


void diff_flou (sil::Image& image, int const taille_kernel, int const diff_kernel)
{
    //Création des 2 images floutées
    sil::Image image_flou1 = image;
    sil::Image image_flou2 = image;
    flou_generalise(image_flou1, taille_kernel);
    flou_generalise(image_flou2, taille_kernel + diff_kernel);

    sil::Image diff_image{image.width(), image.height()};
    
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            //Différence absolue entre les 2 couleurs
            glm::vec3 diff = glm::abs(image_flou1.pixel(x,y) - image_flou2.pixel(x,y));
            
            //Calcul de la luminosité de cette différence
            float const luminosite = 0.2126f * diff.r + 0.7152f * diff.g + 0.0722f * diff.b;
            
            //Les grandes différences sont blanches et les petites sont noires
            if (luminosite >= 0.01f)
            {
                diff_image.pixel(x,y) = glm::vec3{0.f};
            }
            else
            {
                diff_image.pixel(x,y) = glm::vec3{1.f};
            }
        }
    }
    image = diff_image;
}