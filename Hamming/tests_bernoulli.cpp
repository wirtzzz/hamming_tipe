// erreurs indépendantes définies sur chaque bit

#include <iostream>
#include <random>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "hamming_binaire.h"
#include "hamming_f16.h"
using namespace std;

float rho;
int r_max;
int N;                  // nombre de tests

float randf();
void bernoulli_binaire(int*,int);
void bernoulli_16(f16*,int);
bool est_nul_b(int*,int);
bool est_nul_16(f16*,int);

int main(){
    srand(time(NULL));
    cout << "r_max = ";
    cin >> r_max;       // plus grand que 2, pas trop grand (moins que 10 de préf)
    cout << "rho = ";   // 0.0001 dans la vraie vie, doit être plus petit que 1/2
    cin >> rho;
    cout << "N = ";
    cin >> N;

    ofstream log("log.txt");

    log << "Tests avec une loi de Bernoulli sur chaque bit pour N=" << N << ", rho=" << rho << ", r_max=" << r_max << endl;
    log << "P(chi2) est la proportion de tests concluants pour hamming binaire, P(chi16) le nombre de tests concluants pour hamming sur le corps f16" << endl;
    log << "/////////////////////////////////////////////////////" << endl << endl;

    int succ_b, succ_16;    // nombre de messages corrigés avec succès en Hamming binaire et sur f16
    for (int r = 2; r <= r_max; r++)
    {
        succ_16=0, succ_b=0;

        Matrice Pbin = V(r);
        simplifier(Pbin);
        Matrice Gbin = gen(Pbin);


        int mbin[Gbin.m];
        if (r<=3)
        {
            Matrice16 Pf16 = P16(r); 
            Matrice16 Gf16 = G16(Pf16);
            f16 m16[Gf16.m];
            for (int i = 0; i < N; i++)
            {
                bernoulli_binaire(mbin,Gbin.m);
                bernoulli_16(m16,Gf16.m);
    
                correction(Gbin, mbin);
                correction16(Gf16, m16);
    
                if (est_nul_b(mbin, Gbin.m))
                {
                    succ_b++;
                }
                
                if (est_nul_16(m16, Gf16.m))
                {
                    succ_16++;
                }
                
                system("clear");
                cout << endl << endl << endl << endl;
                cout << "           Calcul en cours (" << (r-2)*N + i << "/" <<  (r_max-2)*N << ")" << endl;
            }
            log << "-> si r=" << r <<", P(chi2)=" << succ_b << "/" << N << " P(chi16)=" << succ_16 << "/" << N << endl;
            log << "                    R2=" << Pbin.m-r << "/" << Pbin.m <<" R16=" << Pf16.m - r << "/" << Pf16.m << endl;
    
        }
        
        else{
        for (int i = 0; i < N; i++)
        {
            bernoulli_binaire(mbin,Gbin.m);

            correction(Gbin, mbin);

            if (est_nul_b(mbin, Gbin.m))
            {
                succ_b++;
            }
            
            system("clear");
            cout << endl << endl << endl << endl;
            cout << "           Calcul en cours (" << (r-2)*N + i << "/" <<  (r_max-2)*N << ")" << endl;
        }

        log << "-> si r=" << r <<", P(chi2)=" << succ_b << "/" << N << endl;
        log << "                    R2=" << Pbin.m-r << "/" << Pbin.m  << endl;
    }
    }

    log.close();
    return 0;
}

float randf(){
    return ((float)rand()/RAND_MAX);
}

void bernoulli_binaire(int* m, int l){  // génère [0 ... 0] avec du bruit
    for (int i=0; i<l; i++){
        if(randf()>rho)
            *(m+i)=false;
        else
            *(m+i)=true;
    }
}

void bernoulli_16(f16* m, int l){ // même chose sur le corps f16
    for (int i = 0; i <l; i++)
    {
    if(randf()>rho)
        (m+i)->U=false;
    else
        (m+i)->U=true;

    if(randf()>rho)
        (m+i)->X=false;
    else
        (m+i)->X=true;
    
    if(randf()>rho)
        (m+i)->X2=false;
    else
        (m+i)->X2=true;

    if(randf()>rho)
        (m+i)->X3=false;
    else
        (m+i)->X3=true;     
    }
}

bool est_nul_b(int* message, int l){
    for (int i = 0; i < l; i++)
    {
        if (*(message+i)==1)
            return false;
    }
    return true;
}

bool est_nul_16(f16* message, int l){
    for (int i = 0; i < l; i++)
    {
        if (*(message+i) != f16({false,false,false,false}))
        {
            return false;
        }
    }
    
    return true;
}