#ifndef HAMMING_BINAIRE_H
#define HAMMING_BINAIRE_H
#include "outils.hpp"

int w(int*,int*,int);

void print(int*,int);

Matrice V(int);

void echange_colonne(Matrice*,int,int);

void simplifier(Matrice&);

Matrice gen(Matrice);

void produit(Matrice,int*,int*,bool);

void correction(Matrice,int*);

#endif /* HAMMING_BINAIRE_H */