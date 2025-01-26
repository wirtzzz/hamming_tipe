/*
hamming_general_lineaire contient des recherches faites antérieurement avant lecture du tome d'Algèbre de Michel Demazure
nous tentons ici d'utiliser les outils définis dans Algèbre

dans tout le code on s'occupera de codes de Hamming (n,k,d) où n est la longueur des messages, k la dimension, autrement dit la longueur des messages avant d'êtres encodés, et d la distance minimal de Hamming entre deux mots
non nuls

dans un premier temps on travaille sur Z/2Z

utilisation du décalage à droite permet de mieux identifier les différentes colonnes
*/

#include <iostream>
#include "hamming_general_lineaire.hpp"
using namespace std;

// définitions un peu pète couilles

enum F4 {E, A, B, C};   // E=0, A=1, B=X, C=X+1 dans l'extension (Z/2Z)/(X^2+X+1)

F4 operator+(F4 const& x, F4 const& y){
    switch (x)
    {
    case E:
        return y;
    
    case A:
        switch (y)
        {
        case E:
            return A;
        
        case A:
            return E;

        case B:
            return C;
        
        default:
            return B;
        }
    
    case B:
        switch (y)
        {
        case E:
            return B;
        
        case A:
            return C;

        case B:
            return E;
        
        default:
            return A;
        }

    
    default:
       switch (y)
        {
        case E:
            return C;
        
        case A:
            return B;

        case B:
            return A;
        
        default:
            return E;
        }
    }
}

F4 operator*(F4 const& x, F4 const& y){
    switch (x)
    {
    case E:
        return E;
    
    case A:
        return y;
    
    case B:
        switch (y)
        {
        case E:
            return E;

        case A:
            return B;
        
        case B:
            return C;
        default:
            return A;
        }

    default:
        switch (y)
        {
        case E:
            return E;
        
        case A:
            return C;
        
        case B:
            return A;
        default:
            return B;
        }
    }
}

// faire avec des bool dans le cas binaire, beaucoup plus simple

int w(int* x, int* y, int l){
    int s=0;
    for (int i = 0; i < l; i++)
    {
        s=s+abs(*(x+i)-*(y+i));
    }
    
    return s;
}

// génération d'une matrice vérificatrice de Hamming
Matrice P(int n){
    if (n==1){
        Matrice A(1,1);
        A.tableau[0][0]=1;
        return A;
    }

    Matrice B=P(n-1);
    Matrice A(B.n+1,2*B.m+1);

    for (int i = 0; i < B.m+1; i++)
    {
        A.tableau[0][i]=1;
    }
    
    for (int i=0; i<B.n; i++){
        for (int j = 0; j < B.m; j++)
        {
            A.tableau[i+1][j]=B.tableau[i][j];
            A.tableau[i+1][j+B.m+1]=B.tableau[i][j];
        }
    }

    return A;
}

Matrice G(Matrice P){
    Matrice g(P.m,P.n);
    Matrice var(P.m-P.n,P.n);       // matrice initialisée à zéros qui permet d'exprimer la dépendance des n-m dernières valeurs en les n autres valeurs (dim Ker P = m) puis des m premières

    for (int i = 0; i < P.m; i++)
    {
        for (int j = 0; i < count; i++)
        {
            /* code */
        }
        
    }
    
    return g;
}

void produit(Matrice m, int* vect, int* res){
    for (int i = 0; i < m.n; i++)
    {
        for (int j = 0; j < m.m; j++)
        {
            if (*(vect+i)==1)
            {
                *(res+j)=(*(res+j)+m.tableau[i][j])%2;
            }
            
        }
        
    }
    
}

int main(){
    P(3).afficher();
    int message[3]={1,0,1};
    int sortie[7]={0,0,0,0,0,0,0};
    produit(P(3), message, sortie);
    for (int i = 0; i < 7; i++)
    {
        cout << sortie[i];
    }
    
    return 0;
}

