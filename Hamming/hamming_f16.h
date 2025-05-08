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

f16 operator+(f16 const&,f16 const&);
f16 operator*(f16 const&,f16 const&);
bool operator==(f16 const&,f16 const&);
bool operator!=(f16 const&,f16 const&);
f16 inv(f16 const&);
f16 operator/(f16 const&,f16 const&);
f16 int_to_f16(int);

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

Matrice16 F(int);
void echange_colonne(Matrice16*,int,int);
Matrice16 P16(int);
Matrice16 G16(Matrice16);
void produit16(Matrice16,f16*,f16*,bool);
void correction16(Matrice16,f16*);

#endif /*HAMMING_F16_H*/