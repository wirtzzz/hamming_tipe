#ifndef HAMMING_GENERAL_LINEAIRE_H
#define HAMMING_GENERAL_LINEAIRE_H

#include <iostream>
#include <vector>

class Matrice {
    public:
        int n,m; // taille de la matrice

        std::vector<std::vector<int>> tableau; // tableau contenant les valeurs des cases de la matrice
        Matrice(){
            this->n=0;
            this->m=0;
            tableau={};
        }
        Matrice(int n, int m){
            this->n = n;
            this->m = m;
            tableau={};
            for (int i = 0; i < n; i++)     // initialisation du tableau
            {
                tableau.push_back({});
                for (int j = 0; j < m; j++)
                {
                    (tableau.back()).push_back(0);
                }
            }
        }

        void afficher(){
            for (int i=0; i<this->n;i++){
                for (int j = 0; j < this->m; j++)
                {
                    printf("%d ", this->tableau[i][j]);
                }
                printf("\n");
            }
            printf("\n");
        }
};

#endif /*HAMMING_GENERAL_LINEAIRE_H*/