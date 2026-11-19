#include <sil/sil.hpp>
#include <utility> //Pour utiliser std::swap





//Exo 1 : Ne garder que le vert 

void garder_que_vert(sil::Image& image)
{
    //On parcourt tous les pixels de l'image
    for (glm::vec3& color : image.pixels())
    {
        //On met les canaux rouge et bleu à 0 : seul le canal vert est conservé
        color.r = 0.f;
        color.b = 0.f;
    }
}





//Exo 2 : Echanger 2 canaux (vert et bleu)

//Je ne connaissais pas la fonction swap donc j'avais fait une fonction moins optimale sans
void echange_vert_bleu(sil::Image& image)
{
    for (glm::vec3& color : image.pixels())
    {
        //On stocke temporairement le vert pour ne pas le perdre
        float const intermediaire = color.g;

        //On place le bleu dans le canal vert
        color.g = color.b;

        //On place l'ancienne valeur du vert dans le canal bleu
        color.b = intermediaire;
    }
}


//Voici la version optimisée avec la fonction swap donnée en indice
void echange_vert_bleu_optimisee(sil::Image& image)
{
    for (glm::vec3& color : image.pixels())
    {
        //Echange direct des 2 canaux (vert et bleu)
        std::swap(color.g, color.b);
    }
}





// Exo 3 : Noir et blanc

void nuances_gris(sil::Image& image)
{
    for (glm::vec3& color : image.pixels())
    {
        //Formule trouvée sur internet
        float const gris{0.299f * color.r + 0.587f * color.g + 0.114f * color.b};

        //Le pixel devient une nuance de gris
        color = glm::vec3{gris};
    }
}





//Exo 4 : Négatif

void negatif(sil::Image& image)
{
    for (glm::vec3& color : image.pixels())
    {
        //On inverse chaque canal (0 devient 1 et 1 devient 0)
        color = 1.f-color;
    }
}





//Exo 5 : Dégradé

void degrade(int const largeur, int const hauteur)
{
    //Création d'une nouvelle image aux dimensions demandées par l'utilisateur
    sil::Image image {largeur, hauteur};

    for (int x{0}; x < largeur; x++)
    {
        //Le coefficient ne dépend que de la position horizontale
        float const gris{
            static_cast<float>(x) / (largeur - 1)
        };

        //C'est la même valeur pour tous les pixels de la colonne
        for (int y{0}; y < hauteur; y++)
        {
            image.pixel(x, y) = glm::vec3{gris};
        }
    }
    image.save("images/difficulte-1/degrade.png");
}