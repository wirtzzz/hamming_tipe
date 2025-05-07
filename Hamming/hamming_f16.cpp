#include "hamming_f16.hpp"
#include <iostream>

using namespace std;

// construction de la matrice vérificatrice par récurrence

Matrice16 F(int r){ // calcul la matrice vérificatrice pour le code de hamming Hr avec r>=2
    if (r==2){
        Matrice16 H(2,17);
        f16 UN={0,0,0,1};
        for (int i = 0; i < 15; i++)
        {
            H.tableau[0][i]=int_to_f16(i+1);
            H.tableau[1][i]=UN;
        }
        
        H.tableau[0][15]=UN;
        H.tableau[1][16]=UN;

        return H;
    }
    Matrice16 V=F(r-1);
    Matrice16 H(V.n +1, 16*V.m+1);
    for (int k = 0; k < 15; k++)
    {
        f16 K=int_to_f16(k+1);
        for (int i = 0; i < V.m; i++)
        {
            H.tableau[0][k*V.m+i]=K;
            for (int j = 0; j < V.n; j++)
            {
                H.tableau[j+1][k*V.m+i]=V.tableau[j][i];
            }
            
        }
    }
    H.tableau[0][15*V.m]={0,0,0,1};
    
    for (int i = 0; i < V.m; i++)
    {
        for (int j = 0; j < V.n; j++)
        {
            H.tableau[j+1][15*V.m+1+i]=V.tableau[j][i];
        }
        
    }
    return H;
}

void echange_colonne(Matrice16* m, int i, int j){
    for (int k = 0; k < m->n; k++)
    {
        f16 a=m->tableau[k][i];
        m->tableau[k][i]=m->tableau[k][j];
        m->tableau[k][j]=a;
    }
    
}

Matrice16 P(int r){ // construit une matrice vérificatrice de rang r et lui donne une forme plus simple qui pourra être utilisée pour construire G_r
    Matrice16 P=F(r);
    int a=(P.m-1)/16;
    int b=0;
    for (int i = 0; i < P.n; i++)
    {
        b=b+15*a+1;
        echange_colonne(&P, i, b-1);
        a=(a-1)/16;
    }
    return P;
    
}

Matrice16 G(Matrice16 A){ // A doit être de la forme [I B]
    // dans un corps de caractéristique 2 comme F16, pas besoin de soustraction (additionner revient à soustraire)
    Matrice16 G(A.m,A.m-A.n);
    for (int j = 0; j < A.m-A.n; j++)
    {
        G.tableau[A.n+j][j]={0,0,0,1};

        for (int i=0; i<A.n; i++){
            G.tableau[i][j]=A.tableau[i][A.n+j];
        }
    }
    return G;
}


void produit(Matrice16 m, f16* vect, f16* res, bool t=true){      // t est un booléen qui indique si on considère la matrice comme sa transposée
    for (int i = 0; i < m.n; i++)
    {
        for (int j = 0; j < m.m; j++)
        {
            if (t)
            {
                *(res+j)=*(res+j)+*(vect+i)*m.tableau[i][j];
            }
            else
                *(res+i)=*(res+i)+*(vect+j)*m.tableau[i][j];
        }
        
    }
}

int main(){
    F(3).afficher();
    Matrice16 A=P(2);
    cout << "====================================" << endl;
    A.afficher();
    cout << "longueur en bits de l'espace d'arrivée = "<< 4*A.m << endl;
    Matrice16 g=G(A);
    g.afficher();

    f16 m[15]={{0,0,0,0},{0,0,0,0},{0,1,1,1},{0,0,1,1},{0,0,0,0},{1,1,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,1,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,1,1,1}};
    f16 sortie[17];
    for (int i=0; i < 17; i++){
        sortie[i]={0,0,0,0};
    }
    
    cout << "message original : ";

    for (int i=0; i < 15; i++){
        m[i].afficher();
    }

    cout << " ou encore " ;

    for (int i = 0; i < 15; i++)
    {
        m[i].afficher_int();
        printf(" ");
    }
    cout << endl;

    produit(g, m, sortie,false);
    cout << "message en sortie :";

    for (int i = 0; i < 17; i++)
    {
        sortie[i].afficher();
    }
    cout << "ou encore ";
    for (int i = 0; i<17; i++){
        sortie[i].afficher_int();
        cout << " ";
    }

    f16 syndrome[2]={{0,0,0,0},{0,0,0,0}};
    produit(A,sortie, syndrome,false);

    cout << endl;
    syndrome[0].afficher_int();
    syndrome[1].afficher_int();

    return 0;
}