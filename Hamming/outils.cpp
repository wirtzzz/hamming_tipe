#include "outils.hpp"
#include <random>
using namespace std;

// TODO :   définir opérations sur les matrices
//          création du tableau de redondance   ->  on veut un nombre de bits de redondance tel que la distance entre deux messages corrects codés soit >=3
//          code de type [n,k,3] privilégié n=nombre total de bits, k=bits du message
//          voir après avec un corps à p éléments (Z/pZ)

//          on cherche le plus petit nombre de bits m tel que 2^m >= m + k + 1
//          définir options sur l'ensemble des entiers utilisés


//  essayer sur des corps à 2^k éléments plutôt que p (Legros)


const int N = 2;    // définit le cardinal du corps commutatif, doit être premier
random_device r;
default_random_engine e(r());

int parite(int k){  //  calcul du nombre de bits de parite
    int p=1,s=0;
    while(p < s+k+1){
        s++;
        p=p*N;
    }
    return s;
}

Matrice matrice_codage(int n){      // prend en argumant la taille du message considéré et une matrice et la modifie en y stockant la matrice de codage de Hamming
    int m=parite(n);
    Matrice code(m+n,n);

    for (int i = 0; i < n; i++)     // on commence par remplir le haut de la matrice avec la matrice In
    {
        code.tableau[i][i]=1;
    }

    //  on crée des familles de vecteurs de différence de poids >=2 (maximiser distance de Hamming)
    //  dans les futures versions devrait dépendre de la caractéristique du corps
    int a=0;
    int b=1;
    for (int j = 0; j < n; j++)
    {
        code.tableau[n+a][j]=1;
        code.tableau[n+b][j]=1;

        if(b<m-1){
            b++;
        }
        else{
            a++;
            b=a;
        }

        if (b==a&&b<m-1)
            b++;
    }
    
    return code;
}

// ^ pour créer matrice vérificatrice et de code, voir méthode p.173 Algèbre Demazure (plus méthodique)
// important de créer les deux simultanément

void c(Matrice code, int* mes, int* mess_code){
    for (int i = 0; i < code.n; i++)
    {
        *(mess_code+i)=0;
    }
    
    for (int i = 0; i < code.m; i++)
    {
        for (int j=0; j<code.n; j++)
            *(mess_code+j)= *(mess_code+j) + mes[i] * code.tableau[j][i];
    }

    for (int i = 0; i < code.n; i++)
    {
        *(mess_code+i)=*(mess_code+i) % N;
    }

}

int h(int* x, int* y, int l){   // renvoie la distance de Hamming entre deux messages de longueur l
    int s=0;
    for (int i=0; i<l; i++){
        s=s+abs(*(x+i)-*(y+i));
    }
    return s;
}

void recu(Matrice code, int* mess_code, int* mess_recu){    // on va multiplier par la transposée du code
    for (int i = 0; i < code.m; i++)
    {
        *(mess_recu+i)=0;
    }
    
    for (int i = 0; i < code.n; i++)
    {
        for (int j = 0; j < code.m ; j++)
        {
            *(mess_recu+j)=*(mess_recu+j) + mess_code[i] * code.tableau[i][j];
        }
        
    }
    
    for (int i = 0; i < code.m; i++)
    {
        *(mess_recu+i)=*(mess_recu+i)%N;
    }
    
}

void print(int* str, int l){
    for (int i = 0; i < l; i++)
    {
        printf("%d", *(str+i));
    }
    printf("\n");
}

void bruitage_unique(int* entrée, int* sortie, int l){
    uniform_int_distribution<int> rand1(0,l-1);
    int i=rand1(e);
    uniform_int_distribution<int> rand2(1,N-1);
    int n=rand2(e);
    for (int j = 0; j<l; j++){
        if (j==i)
        {
            *(sortie+i)=(*(entrée+i)+n)%N;
        }
        else
            *(sortie+j)=*(entrée+j);
    }
}



int main(){
    int m = parite(7);
    Matrice H(m+7,7);
    int mess[7] = {0,1,1,0,0,1,1};    // toujours veiller à avoir un message de longueur adaptée à la matrice de codage bien sûr

    H=matrice_codage(7);
    H.afficher();

    print(mess, 7);
    int mess_code[11];
    c(H, mess, mess_code);
    print(mess_code, 11);

    int code_recu[11];
    bruitage_unique(mess_code, code_recu, 11);
    printf("\n\n");
    print(code_recu,11);
    int mess_recu[7];

    for (int i = 0; i < 7; i++)
    {
        mess_recu[i]=code_recu[i];
    }
    
    int attendu[11];
    c(H,mess_recu, attendu);

    // recu(H, mess_code, mess_recu);
    print(attendu,11);
    printf("le message reçu et le code attendu sont distants de : %d", h(code_recu,attendu,11));

    return 0;
}