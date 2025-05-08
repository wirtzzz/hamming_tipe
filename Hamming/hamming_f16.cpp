#include "hamming_f16.h"
#include <iostream>

using namespace std;

f16 operator+(f16 const& x, f16 const& y){
    return {x.X3 != y.X3, x.X2 != y.X2, x.X != y.X, x.U != y.U}; 
}

f16 operator*(f16 const& x, f16 const& y){
    bool A6,A5,A4,A3,A2,A1,A0;
    A6=x.X3&&y.X3;
    A5=(x.X3&&y.X2) != (x.X2&&y.X3);
    A4=((x.X3&&y.X) != (x.X2&&y.X2)) != (x.X && y.X3);
    A3=((x.X3&&y.U)!=(x.X2&&y.X))!=((x.X&&y.X2)!=(x.U&&y.X3));
    A2=((x.X2&&y.U)!=(x.X&&y.X))!=(x.U&&y.X2);
    A1=((x.X&&y.U)!=(x.U&&y.X));
    A0=x.U&&y.U;

    if (A6){
        A5=!A5;
        A2=!A2;
    }

    if(A5){
        A4=!A4;
        A1=!A1;
    }
    
    if(A4){
        A3=!A3;
        A0=!A0;
    }
    return {A3,A2,A1,A0};
}

bool operator==(f16 const& x, f16 const& y){
    return (x.U==y.U)&&(x.X==y.X)&&(x.X2==y.X2)&&(x.X3==y.X3);
}


bool operator!=(f16 const& x, f16 const& y){
    return !(x==y);
}

f16 inv(f16 const& x){                          // l'ensemble des x^n pour n entier est un sous groupe de f16 contenant x
    if (x==f16({false,false,false,true}))        // si x est l'élément neutre
        return x;

    f16 inverse=x;
    while (inverse*x != f16({false,false,false,true}))  // tant que inverse n'est pas l'inverse de x
    {
        inverse = inverse*x;
    }
    return inverse;
}

f16 operator/(f16 const& x, f16 const& y){
    return x*inv(y);
}

f16 int_to_f16(int x){  // prend en argument un entier entre 0 et 15 et le décompose en base 2 dans f16
    int a=x;
    bool A0=x%2;
    a=a/2;
    bool A1=a%2;
    a=a/2;
    bool A2=a%2;
    a=a/2;
    bool A3=a;
    return {A3,A2,A1,A0};
}

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

Matrice16 P16(int r){ // construit une matrice vérificatrice de rang r et lui donne une forme plus simple qui pourra être utilisée pour construire G_r
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

Matrice16 G16(Matrice16 A){ // A doit être de la forme [I B]
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


void produit16(Matrice16 m, f16* vect, f16* res, bool t=true){      // t est un booléen qui indique si on considère la matrice comme sa transposée
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

void correction16(Matrice16 verif, f16* message){
    f16 syndrome[verif.n];

    for (int i = 0; i < verif.n; i++)
    {
        syndrome[i]={false,false,false,false};
    }
    
    produit16(verif, message, syndrome, false);

    int s=0;
    for (int i = 0; i < verif.n; i++)
    {
        if (((syndrome+i)->U) || ((syndrome+i)->X)  || ((syndrome+i)->X2)  || ((syndrome+i)->X3)){
                s=1;
                break;
        }
            
    }

    if (s==0)
    {
        return;
    }
    
    for (int i = 0; i < verif.m; i++)
    {
        s=0;

        int i0=0;

        while (i0<verif.n && *(syndrome+i0)==f16({false,false,false,false}) && verif.tableau[i0][i]== f16({false,false,false,false}))
        {
            i0++;
        }
        
        if (i0 < verif.n && verif.tableau[i0][i]!= f16({false,false,false,false}))
        {
            f16 r = (*(syndrome+i0))/verif.tableau[i0][i];
            for (int j = i0; j < verif.n; j++)
            {
                if (*(syndrome+j) != r*verif.tableau[j][i])
                    s++;
            }
            
            if (s==0)
            {
                *(message+i)=*(message+i) + r;
                return;
            }
            
        }
    }
    
}

