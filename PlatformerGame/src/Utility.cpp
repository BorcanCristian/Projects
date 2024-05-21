//DESPRE typedef si s , u
// s = signed
// u = unsigned
typedef char s8;          // s8 și u8: Întregi de 8 biți, semnați și nesemnați, respectiv.
typedef unsigned char u8;//De obicei pot reprezenta valori între -128 și 127 pentru s8, și între 0 și 255 pentru u8.

typedef short s16;          // s16 și u16: Întregi de 16 biți, semnați și nesemnați, respectiv.
typedef unsigned short u16; // Pot reprezenta valori între -32768 și 32767 pentru s16, și între 0 și 65535 pentru u16.

typedef int s32;             //s32 și u32: Întregi de 32 de biți, semnați și nesemnați, respectiv.
typedef unsigned int u32;    //Pot reprezenta valori între -2147483648 și 2147483647 pentru s32, și între 0 și 4294967295 pentru u32.
//  typedef: u32 este un allyas pentru unsigned int (am inlocuit peste tot in renderer.cpp)

typedef long long s64;          //s64 și u64: Întregi de 64 de biți, semnați și nesemnați, respectiv
typedef unsigned long long u64; //Întreaga valoare este prea mare pentru a fi trecută aici, dar s64 poate reprezenta valori negative, iar u64 poate reprezenta valori foarte mari.
/* Aceste aliasuri fac codul mai ușor de înțeles și ajută la asigurarea că datele au dimensiunea corectă,
indiferent de platforma pe care rulează codul. */



// DESPRE #define si cod:
//Definițiile de preprocesor sunt direcții date compilatorului care procesează codul înainte de compilarea propriu-zisă.
/*#define este o directivă de preprocesor care definește un macro.
 *În acest caz, global_variable și internal sunt definite ca static. */

/*Inseamnă că oriunde în cod unde veți scrie global_variable,
 preprocesorul îl va înlocui cu static înainte ca codul să fie compilat. */
#define global_variable static //global_variable devine un alias pentru static în acest context.
#define internal static        //La fel, aici internal este un alias pentru static.
/* Cuvântul cheie static în C++ este folosit în mai multe contexte, dar în general,
 face ca o variabilă sau o funcție să fie vizibilă doar în fișierul sursă unde este definită,
 limitând astfel domeniul vizibilității.
   Utilizarea acestor aliasuri pentru static, cum ar fi global_variable și internal, poate face codul mai ușor de înțeles,
 deoarece aceste nume oferă o idee despre cum și unde sunt destinate să fie utilizate aceste variabile sau funcții. */


// Primeam eroare la modificarea casutei si am facut clamp pentru a rezolva eroarea sub 500.
// Despre functia clamp si inline:

//Folosirea funcției inline îi spune compilatorului să insereze codul corpului funcției direct în fiecare loc în care funcția este utilizată în program, în loc să facă un apel de funcție obișnuit.
//Acest lucru poate îmbunătăți performanța prin eliminarea overhead-ului asociat cu apelurile de funcții, dar va crește dimensiunea binarului final.
//Este un exemplu bun de utilizare prudentă a unei funcții inline: funcția este suficient de mică încât overhead-ul unui apel de funcție ar putea fi comparabil cu timpul de execuție al funcției însăși.

inline int
clamp (int min, int val, int max) //Această funcție primește trei argumente: min, val și max.
                                  //Scopul său este de a asigura că val se încadrează în intervalul specificat de min și max.
{
    if (val < min) return min;  //Primul if în funcție verifică dacă val este mai mică decât min. Dacă este, valoarea returnată de funcție este min.
    if  (val > max) return max; //Al doilea if verifică dacă val este mai mare decât max. Dacă este, valoarea returnată de funcție este max.
    return val;                 //Dacă val este între min și max, funcția va returna val.
}
// AM mutat functia intr-un nou fisier.cpp