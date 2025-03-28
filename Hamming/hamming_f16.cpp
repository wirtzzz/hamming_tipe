#include "hamming_f16.hpp"
#include <iostream>

using namespace std;

// construction de la matrice vérificatrice par récurrence

Matrice16 P(int r){ // calcul la matrice vérificatrice pour le code de hamming Hr avec r>=2
    if (r==2){
        Matrice16 H(17,2);
        f16 UN={0,0,0,1};
        for (int i = 0; i < 14; i++)
        {
            H.tableau[i][0]=int_to_f16(i+1);
            H.tableau[i][1]=UN;
        }
        
        H.tableau[15][0]={1,1,1,1};
        H.tableau[16][1]=UN;

        return H;
    }

    Matrice16 V=P(r-1);
    Matrice16 H(16*V.n +1, V.m+1);
    for (int k = 0; k < 16; k++)
    {
        f16 K=int_to_f16(k);
        for (int i = 0; i < V.n; i++)
        {
            H.tableau[k*V.n+i][0]=K;
            for (int j = 0; j < V.m; j++)
            {
                H.tableau[k*V.n+i][j+1]=V.tableau[i][j];
            }
            
        }
    }
    H.tableau[15*V.n][0]={1,1,1,1};
    
    for (int i = 0; i < V.n; i++)
    {
        for (int j = 0; j < V.m; j++)
        {
            H.tableau[15*V.n+1+i][j+1]=V.tableau[i][j];
        }
        
    }
    return H;
}

int main(){
    Matrice16 H=P(2);
    H.afficher();
    return 0;
}