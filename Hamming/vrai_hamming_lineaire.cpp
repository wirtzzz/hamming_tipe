/*
hamming_general_lineaire contient des recherches faites antérieurement avant lecture du tome d'Algèbre de Michel Demazure
nous tentons ici d'utiliser les outils définis dans Algèbre

dans tout le code on s'occupera de codes de Hamming (n,k,d) où n est la longueur des messages, k la dimension, autrement dit la longueur des messages avant d'êtres encodés, et d la distance minimal de Hamming entre deux mots
non nuls

dans un premier temps on travaille sur Z/2Z

utilisation du décalage à droite permet de mieux identifier les différentes colonnes
*/

#include <iostream>
#include <random>
#include "hamming_general_lineaire.hpp"
using namespace std;

// définitions un peu pète couilles, utiliser des polynômes peut être

struct f4 {
    bool X,U;   // booléen des unités et des X
};

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



void print(int* m, int l){
    //  l étant la longueur du message
    for (int i = 0; i < l; i++)
    {
        cout << *(m+i);
    }
    
}


// génération d'une matrice vérificatrice de Hamming
Matrice V(int n){
    if (n==1){
        Matrice A(1,1);
        A.tableau[0][0]=1;
        return A;
    }

    Matrice B=V(n-1);
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

void echange_colonne(Matrice* m, int i, int j){
    for (int k = 0; k < m->n; k++)
    {
        int a=m->tableau[k][i];
        m->tableau[k][i]=m->tableau[k][j];
        m->tableau[k][j]=a;
    }
    
}

// une matrice simplifiant la matrice en question

void simplifier(Matrice& P){
    int a=(P.m+1)/2;
    int b=0;
    for (int i=0; i < P.n; i++){
        b=b+a;
        echange_colonne(&P,i,b-1);
        a=a/2;
    }
}

Matrice gen(Matrice P){       // fonctionne dans le cas des matrices vérificatrices simplifiées
    Matrice g(P.m-P.n,P.m);
    Matrice var(P.n,P.m-P.n);       // matrice initialisée à zéros qui permet d'exprimer la dépendance des n premières valeurs en les m-n autres valeurs (dim Ker P = m-n)
    for (int i = 0; i < P.n; i++)
    {
        for (int j = P.n; j < P.m; j++)
        {
            var.tableau[i][j-P.n]=P.tableau[i][j];
            
        }
        
        
    }

    // on colle la transposée de var et une matrice I_(m-n)
    for (int i = 0; i < P.m-P.n; i++)
    {
        for (int j = 0; j < P.n; j++)
        {
            g.tableau[i][j]=var.tableau[j][i];
        }
        g.tableau[i][P.n+i]=1;
    }
    
    
    return g;
}

void produit(Matrice m, int* vect, int* res, bool t=true){      // t est un booléen qui indique si on considère la matrice comme sa transposée
    for (int i = 0; i < m.n; i++)
    {
        for (int j = 0; j < m.m; j++)
        {
            if (t)
            {
                *(res+j)=(*(res+j)+*(vect+i)*m.tableau[i][j])%2;
            }
            else
                *(res+i)=(*(res+i)+*(vect+j)*m.tableau[i][j])%2;
        }
        
    }
    
}


void correction(Matrice verif, int* message){
    //  Hamming dans cette version est un code 1-correcteur : l'algorithme suivant ne peut naturellement fonctionner que lorsqu'un bit au plus est modifié
    int syndrome[verif.n];

    for (int i = 0; i < verif.n; i++)
    {
        syndrome[i]=0;
    }
    
    produit(verif,message,syndrome,false);

    // le syndrome correspond (dans le cas où un une bit est modifié) à la colonne du bit modifié dans verif (Demazure)
    int s=0;
    // dans un premier cas on vérifie si le syndrome est tout simplement nulle, auquel cas il n'y a en théorie pas d'erreur

    for (int i = 0; i < verif.n; i++)
    {
        s=s+*(syndrome+i);
    }
    if (s==0)
        return;
    
    for (int i = 0; i < verif.m; i++)
    {
        s=0;
        for (int j = 0; j < verif.n; j++)
        {
            s=s+abs(verif.tableau[j][i]-*(syndrome+j));
        }
        
        if(s==0){
            *(message+i)=(*(message+i)+1)%2;

            return;
        }
        
    }
}

int main(){
    Matrice P=V(3);
    P.afficher();


    simplifier(P);
    P.afficher();

    Matrice G=gen(P);
    G.afficher();
    int message[4]={0,0,1,1};   // 4 bits transportés pour 3 bits de redondance idiot ! (en (7,4,3))
    int sortie[7]={0,0,0,0,0,0,0};
    produit(G, message, sortie);

    cout << "Message encodé" << endl;
    print(sortie, 7);
    cout << endl;

    sortie[5]=(sortie[5]+1)%2;
    cout << "Ajout d'une erreur" << endl;
    print(sortie,7);
    cout << endl;

    int syndrome[3]={0,0,0};
    produit(P, sortie,syndrome, false);
    print(syndrome, 3);
    
    cout << endl;

    // correction :)

    correction(P,sortie);
    cout << "Sortie corrigée" << endl;
    print(sortie,7);
    cout << endl;
    return 0;
}

