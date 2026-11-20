#include <iostream>
#include <sil/sil.hpp>


//Fonctions des exos 1 étoile : 
void garder_que_vert(sil::Image& image);
void echange_vert_bleu(sil::Image& image);
void nuances_gris(sil::Image& image);
void negatif(sil::Image& image);
void degrade(int const largeur, int const hauteur);


//Fonctions des exos 2 étoiles :
void miroir(sil::Image& image); 
void image_bruitee(sil::Image& image, int const max_nb_effet);
void rotation_90(sil::Image& image);
void rotation_270(sil::Image& image);
void rgb_split(sil::Image& image, int decalage);
void eclaircir(sil::Image& image);
void assombrir(sil::Image& image);
void disque(int rayon);
void cercle(int rayon, int epaisseur);
void animation_disques(int rayon);


//Fonctions des exos 3 étoiles : 
void rosace (int const rayon, int const epaisseur);
void mosaique (int const nb_largeur, int const nb_hauteur, sil::Image const& image);
void mosaique_miroir (int const nb_largeur, int const nb_hauteur, sil::Image const& image);
void glitch(sil::Image& image, int max_nb_effet);
void tri_tous_pixels_luminosite(sil::Image& image);
void tri_pixels_colonne_rouge(sil::Image& image);
void tri_pixels_ligne_bleu(sil::Image& image);
void tri_pixels_aleatoire_ligne_vert_dec(sil::Image& image, int const max_nb_effet);


//Fonctions des exos 4 étoiles : 
void fractale_Mandelbrot(int const largeur, int const hauteur);
void degrade_mix_rb(int const largeur, int const hauteur);
void degrade_oklab_rb(int const largeur, int const hauteur);
void ordered_dithering (sil::Image& image);
void normalisation_historigramme (sil::Image& image);
void vortex (sil::Image& image, float const angle_max);
void flou_generalise (sil::Image& image, int const taille_kernel);
void convolution(sil::Image& image, float const kernel[3][3]);
void diff_flou (sil::Image& image, int const taille_kernel, int const diff_kernel);




int main()
{
    //Pour éviter des affichages étranges, je ne mets pas d'accent dans les messages à afficher.
    //Paramètres choisis par l'utilisateur : 

    // Nombre maximal d'effets aléatoires pour l'image bruitée, le glitch et le tri de pixels aléatoires
    std::cout << "Nombre maximal de la repetition de l'effet (image bruitee, glitch et tri de pixels aléatoires) : ";
    int max_nb_effet;
    std::cin >> max_nb_effet;

    //On impose un nombre strictement positif pour qu'il y ait au moins un effet
    while (max_nb_effet < 1)
    {
        std::cout << "Veuillez entrer un entier STRICTEMENT POSITIF : ";
        std::cin>>max_nb_effet;
        std::cout<<"\n";
    }
   
    //Décalage horizontal des canaux rouge et bleu pour le RGB split
    std::cout<<"Decalage souhaite pour l'image de l'exo RGB Split : ";
    int decalage;
    std::cin>>decalage;
    std::cout<<"\n";

    while (decalage <= 0)
    {
        std::cout << "Veuillez entrer un entier STRICTEMENT POSITIF : ";
        std::cin>>decalage;
        std::cout<<"\n";
    }

    //Rayon utilisé pour les disques, les cercles, l'animation et la rosace
    std::cout << "Rayon des disques et des cercles : ";
    int rayon;
    std::cin >> rayon;

    while (rayon <= 0)
    {
        std::cout << "Veuillez entrer un entier STRICTEMENT POSITIF : ";
        std::cin>>rayon;
        std::cout<<"\n";
    }

    //Epaisseur pour les cercles et la rosace
    std::cout << "Epaisseur des cercles : ";
    int epaisseur;
    std::cin >> epaisseur;

    while (epaisseur <= 0 || epaisseur > rayon)
    {
        std::cout << "Veuillez entrer un entier STRICTEMENT POSITIF INFERIEUR AU RAYON : ";
        std::cin>>epaisseur;
        std::cout<<"\n";
    }

    //Nombre de répétitions de l'image pour la mosaïque et la mosaïque miroir
    std::cout << "Nombre d'images a afficher sur la largeur : ";
    int nb_largeur;
    std::cin >> nb_largeur;

    while (nb_largeur <= 0)
    {
        std::cout << "Veuillez entrer un entier STRICTEMENT POSITIF : ";
        std::cin>>nb_largeur;
        std::cout<<"\n";
    }

    std::cout << "Nombre d'images a afficher sur la hauteur : ";
    int nb_hauteur;
    std::cin >> nb_hauteur;

    while (nb_hauteur <= 0)
    {
        std::cout << "Veuillez entrer un entier STRICTEMENT POSITIF : ";
        std::cin>>nb_hauteur;
        std::cout<<"\n";
    }

    //Dimensions des images créées pour le dégradé, les dégradés colorés et la fractale de Mandelbrot
    std::cout << "Largeur de l'image : ";
    int largeur;
    std::cin >> largeur;

    while (largeur < 100)
    {
        std::cout << "Veuillez entrer un entier SUPERIEUR A 100 : ";
        std::cin>>largeur;
        std::cout<<"\n";
    }

    std::cout << "Hauteur de l'image : ";
    int hauteur;
    std::cin >> hauteur;

    while (hauteur < 100)
    {
        std::cout << "Veuillez entrer un entier SUPERIEUR A 100 : ";
        std::cin>>hauteur;
        std::cout<<"\n";
    }

    //Angle maximal du vortex (en rad)
    std::cout << "Valeur de l'angle maximal de rotation des pixels (en rad) : ";
    float angle_max;
    std::cin >> angle_max;

    //Taille du kernel utilisé pour le flou généralisé
    std::cout << "Taille du kernel : ";
    int taille_kernel;
    std::cin >> taille_kernel;

    //Il faut une taille de kernel impaire pour avoir un pixel central
    while (taille_kernel % 2 ==0)
    {
        std::cout << "Veuillez entrer un entier IMPAIR : ";
        std::cin>>taille_kernel;
        std::cout<<"\n";
    }

    //Différence entre les tailles des 2 kernels utilisés pour la différence des flous
    std::cout << "Différence entre les tailles des 2 kernels utilises pour la difference des flous : ";
    int diff_kernel;
    std::cin >> diff_kernel;

    //Il faut une différence paire pour que la taille du 2e kernel reste impaire
    while (diff_kernel % 2 ==1)
    {
        std::cout << "Veuillez entrer un entier PAIR : ";
        std::cin>>diff_kernel;
        std::cout<<"\n";
    }



    //Exos 1 étoile : 
    
    {
        sil::Image image{"images/logo.png"};
        garder_que_vert(image);
        image.save("images/difficulte-1/garder_que_vert.png");
        
    }

    {
        sil::Image image{"images/logo.png"};
        echange_vert_bleu(image);
        image.save("images/difficulte-1/echange_vert_bleu.png");
        
    }

    {
        sil::Image image{"images/logo.png"};
        nuances_gris(image);
        image.save("images/difficulte-1/nuances_gris.png");
        
    }

    {
        sil::Image image{"images/logo.png"};
        negatif(image);
        image.save("images/difficulte-1/negatif.png");
        
    }

    {
        degrade(largeur, hauteur);      
    }
    

//Exos 2 étoiles : 
    {
        sil::Image image{"images/logo.png"};
        miroir(image);
        image.save("images/difficulte-2/miroir.png");
        
    }

    {
        sil::Image image{"images/logo.png"};
        image_bruitee(image, max_nb_effet);
        image.save("images/difficulte-2/image_bruitee.png");
        
    }

    {
        sil::Image image{"images/logo.png"};
        rotation_90(image);
        image.save("images/difficulte-2/rotation_90.png");
        
    }

    {
        sil::Image image{"images/logo.png"};
        rotation_270(image);
        image.save("images/difficulte-2/rotation_270.png");
        
    }

    {
        sil::Image image{"images/logo.png"};
        rgb_split(image, decalage);
        image.save("images/difficulte-2/rgb_split.png");
    }

    {
        sil::Image image{"images/photo.jpg"};
        eclaircir(image);
        image.save("images/difficulte-2/eclaircir.png");
        
    }

    {
        sil::Image image{"images/photo.jpg"};
        assombrir(image);
        image.save("images/difficulte-2/assombrir.png");
        
    }

    {
        disque(rayon);
    }

    {    
        cercle(rayon, epaisseur);
    }

    {
        animation_disques(rayon);
    }


//Exos 3 étoiles :  
    {       
        rosace(rayon, epaisseur);
    }

    {
        sil::Image image{"images/logo.png"};
        mosaique(nb_largeur, nb_hauteur, image);
    }

    {
        sil::Image image{"images/logo.png"};
        mosaique_miroir(nb_largeur, nb_hauteur, image);
    }

    {
        sil::Image image{"images/logo.png"};
        glitch(image, max_nb_effet);
        image.save("images/difficulte-3/glitch.png");
    }

    {
        sil::Image image{"images/logo.png"};
        tri_tous_pixels_luminosite(image);
        image.save("images/difficulte-3/tri_tous_pixels_luminosite.png");
        
    }

    {
        sil::Image image{"images/logo.png"};
        tri_pixels_colonne_rouge(image);
        image.save("images/difficulte-3/tri_pixels_colonne_rouge.png");
        
    }

    {
        sil::Image image{"images/logo.png"};
        tri_pixels_ligne_bleu(image);
        image.save("images/difficulte-3/tri_pixels_ligne_bleu.png");
        
    }

    {
        sil::Image image{"images/logo.png"};
        tri_pixels_aleatoire_ligne_vert_dec(image, max_nb_effet);
        image.save("images/difficulte-3/tri_pixels_aleatoire_ligne_vert_dec.png");
        
    }


//Exos 4 étoiles : 
    {
        fractale_Mandelbrot(largeur, hauteur);
    }


    {
        degrade_mix_rb(largeur, hauteur);
    }

    {
        degrade_oklab_rb(largeur, hauteur);
    }

    {
        sil::Image image{"images/photo.jpg"};
        ordered_dithering(image);
        image.save("images/difficulte-4/ordered_dithering.png");
    }

    {
        sil::Image image{"images/photo_faible_contraste.jpg"};
        normalisation_historigramme(image);
        image.save("images/difficulte-4/normalisation_historigramme.png");
    }

    {
        sil::Image image{"images/logo.png"};
        vortex (image, angle_max);
        image.save("images/difficulte-4/vortex.png");
    }

    {
        sil::Image image{"images/logo.png"};
        flou_generalise (image, taille_kernel);
        image.save("images/difficulte-4/flou_generalise.png");
    }


    //Application de différents kernels de convolution
    {
        sil::Image image{"images/logo.png"};
        float kernel_outline[3][3]{
            {-1.f, -1.f, -1.f},
            {-1.f,  8.f, -1.f},
            {-1.f, -1.f, -1.f}
        };
        convolution(image, kernel_outline);
        image.save("images/difficulte-4/outline.png");
    }

    {
        sil::Image image{"images/logo.png"};
        float kernel_sharpen[3][3]{
            {0.f, -1.f, 0.f},
            {-1.f, 5.f, -1.f},
            {0.f, -1.f, 0.f}
        };
        convolution(image, kernel_sharpen);
        image.save("images/difficulte-4/sharpen.png");
    }

    {
        sil::Image image{"images/photo.jpg"};
        float kernel_bottom_sobel[3][3]{
            {-1.f, -2.f, -1.f},
            {0.f, 0.f, 0.f},
            {1.f, 2.f, 1.f}
        };
        convolution(image, kernel_bottom_sobel);
        image.save("images/difficulte-4/bottom_sobel.png");
    }

    {
        sil::Image image{"images/photo.jpg"};
        float kernel_emboss[3][3]{
            {-2.f, -1.f, 0.f},
            {-1.f, 1.f, 1.f},
            {0.f, 1.f, 2.f}
        };
        convolution(image, kernel_emboss);
        image.save("images/difficulte-4/emboss.png");
    }

    {
        sil::Image image{"images/photo.jpg"};
        float kernel_left_sobel[3][3]{
            {1.f, 0.f, -1.f},
            {2.f, 0.f, -2.f},
            {1.f, 0.f, -1.f}
        };
        convolution(image, kernel_left_sobel);
        image.save("images/difficulte-4/left_sobel.png");
    }

    {
        sil::Image image{"images/photo.jpg"};
        float kernel_right_sobel[3][3]{
            {-1.f, 0.f, 1.f},
            {-2.f, 0.f, 2.f},
            {-1.f, 0.f, 1.f}
        };
        convolution(image, kernel_right_sobel);
        image.save("images/difficulte-4/right_sobel.png");
    }

    {
        sil::Image image{"images/photo.jpg"};
        float kernel_top_sobel[3][3]{
            {1.f, 2.f, 1.f},
            {0.f, 0.f, 0.f},
            {-1.f, -2.f, -1.f}
        };
        convolution(image, kernel_top_sobel);
        image.save("images/difficulte-4/top_sobel.png");
    }

    {
        sil::Image image{"images/photo.jpg"};
        diff_flou (image, taille_kernel, diff_kernel);
        image.save("images/difficulte-4/diff_flou.png");
    }


    return 0;
}