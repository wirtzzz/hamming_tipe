#ifndef HAMMING_F16_H
#define HAMMING_F16_H
#include <vector>
#include <cstdio>

// corps à 16 éléments généré à partir de X^4 + X^3 + 1
// != correspond à l'addition de deux bits et && à la multiplication
struct f16 {
    bool X3,X2,X,U;   // booléen des unités, des X, des X^2

    void operator=(f16 const& x){
        this->X3=x.X3;
        this->X2=x.X2;
        this->X=x.X;
        this->U=x.U;
    }
    void afficher(){
        printf("%d%d%d%d", this->X3,this->X2,this->X,this->U);
    }
    void afficher_int(){
        int s=0;
        if (this->X3)
            s++;
        s=s*2;
        if (this->X2)
            s++;
        s=s*2;
        if (this->X)
            s++;
        s=s*2;
        if (this->U)
            s++;
        printf("%d", s);
    }
};

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

class Matrice16 {       // matrices dans le corps à 16 éléments
    public:
        int n,m; // taille de la matrice

        std::vector<std::vector<f16>> tableau; // tableau contenant les valeurs des cases de la matrice
        Matrice16(){
            this->n=0;
            this->m=0;
            tableau={};
        }
        Matrice16(int n, int m){
            this->n = n;
            this->m = m;
            tableau={};
            for (int i = 0; i < n; i++)     // initialisation du tableau
            {
                tableau.push_back({});
                for (int j = 0; j < m; j++)
                {
                    (tableau.back()).push_back({0,0,0,0});
                }
            }
        }

        void afficher(){
            for (int i=0; i<this->n;i++){
                for (int j = 0; j < this->m; j++)
                {
                    this->tableau[i][j].afficher_int();
                    printf(" ");
                }
                printf("\n");
            }
            printf("\n");
        }

        void operator=(Matrice16 const& mat){
            this->n=mat.n;
            this->m=mat.m;
            this->tableau=mat.tableau;
        }
};

#endif /*HAMMING_F16_H*/