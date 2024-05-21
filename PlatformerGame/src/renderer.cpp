/*
#include <algorithm>
// Functia se foloseste pentru a desena sau a actualiza fundalul scenei.
internal void // Am adaugat internal pentru a limita vizibilitatea si a preveni conflictele de nume.
render_background()
{
    u32* pixel = (u32*) render_state.memory;
    for (int y = 0; y < render_state.height; y++) // Itereaza prin fiecare pixel pe ecran.
    {
        for (int x = 0; x < render_state.width; x++)
        {
            //Pentru fiecare pixel, se setează culoarea acestuia la valoarea calculată de expresia x*y.
            *pixel++ = x*y; // Indigo 0x4b0082 // BLue 0x0000ff
        }                      // Black 0x000000

    }
    // Această rutină de renderizare a fundalului formează o bază pe care se pot adăuga ulterior alte elemente ale scenei, cum ar fi obiectele jucătorilor sau ale inamicilor.
}
Nu mai avem nevoie de renderer_background*/


#include <corecrt_math.h>
//Are scopul de a curăța ecranul sau o suprafață grafică, setând toți pixelii la o anumită culoare.
internal void
clear_screen(u32 color) // Se utilizeaza la inceputul fiecărui frame într-un joc sau o aplicație grafică, pentru a "șterge" toate elementele desenate în frame-ul anterior înainte de a desena elemente noi.
{
    // La început, facem un cast către u32* a memoriei care conține pixelii ecranului cu (u32*) render_state.memory
    //Acest lucru ne permite să tratăm memoria ca un array de întregi fără semn, unde fiecare întreg reprezintă un pixel.
    u32* pixel = (u32*) render_state.memory;

   //Apoi, avem două bucle for încorporate care parcurg fiecare pixel de pe ecran.

    //Prima buclă for merge prin fiecare rând (y),
    for (int y = 0; y < render_state.height; y++)
    {
        //A doua buclă for parcurge fiecare coloană (x) din acel rând.
        for (int x = 0; x < render_state.width; x++)
        {
            //În interiorul celei de-a doua bucle for, setăm valoarea pixelului curent la culoarea dorită cu *pixel++ = color;.
            //Aici, pixel este un pointer către pixelul curent, deci *pixel este valoarea pixelului.
            //Valoarea pixelului este setată la color
            //apoi pointerul pixel este incrementat pentru a merge la următorul pixel.

            *pixel++ = color; //este o expresie combinată care face două lucruri:
                              //în primul rând, setează valoarea la care pixel este actualmente referit (*pixel)
                              //apoi incrementează pixel cu unul.
        }
    } // Rezultatul final este că toți pixelii ecranului sunt setați la culoarea dorită.
}







//Această funcție are scopul de a desena un dreptunghi de o anumită culoare pe un ecran sau o suprafață grafică.
/* UPDATE: Modificam draw_rect si il facem in pixel si facem un nou draw_rect pentru
a remedia problema obiectului cand se face scale */
internal void
draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color) //Seteaza culoarea fiecărui pixel din interiorul dreptunghiului definit de (x0,y0) și (x1,y1) la valoarea color.
{
    //rezolvare eroare nu se mai stinge programul/ jocul cand schimi size ul.
    //Dacă valorile x0, x1, y0 sau y1 sunt în afara acestor intervale, acestea vor fi setate la valoarea de graniță corespunzătoare.

    x0 = clamp(0, x0, render_state.width); //se asigură că x0 este între 0 și render_state.width.
    x1 = clamp(0, x1, render_state.width); //se asigură că x1 este între 0 și render_state.width.
    y0 = clamp(0, y0, render_state.height);//se asigură că y0 este între 0 și render_state.height.
    y1 = clamp(0, y1, render_state.height);//se asigură că y1 este între 0 și render_state.height.
    //Dacă valorile x0, x1, y0 sau y1 sunt în afara acestor intervale, acestea vor fi setate la valoarea de graniță corespunzătoare.


    //Fragment de cod în C++ pentru a seta culorea unui dreptunghi de pixeli pe o suprafață de randare.
    //Suprafața de randare este un buffer, adică o zonă de memorie, unde sunt stocate informațiile pentru fiecare pixel care trebuie afișat pe ecran.
    //Fiecare pixel este reprezentat ca o valoare întreagă, unde fiecare octet al acestei valori întregi reprezintă o componentă de culoare.
    //Vom parcurge pentru fiecare rând (y) de la y0 la y1 și pentru fiecare coloană (x) de la x0 la x1. Aceasta va forma un dreptunghi de pixeli pe suprafața de randare.

    for (int y = y0; y < y1; y++)
    {
        u32* pixel = (u32*)render_state.memory +x0 + y*render_state.width;
        //ia adresa de început a rândului curent în buffer. render_state.memory este adresa de început a bufferului, x0 este deplasamentul coloanei și y * render_state.width calculează deplasamentul rândului în buffer.
        //Aceasta ne dă adresa pixelului de la coloana x0, rândul y.


            for (int x = x0; x < x1; x++)
            {
                *pixel++ = color; //pentru fiecare pixel de pe rândul y setăm culoarea (color), apoi incrementăm pointerul pixel pentru a trece la următorul pixel pe același rând.
            }                     //Repetăm acest proces pentru fiecare rând de pixeli în dreptunghi.
    }
}


global_variable float render_scale = 0.01f;

internal void
draw_arena_borders(float arena_x, float arena_y, u32 color)
{
 arena_x *= render_state.height * render_scale;
 arena_y *= render_state.height * render_scale;

    int x0 = (int)((float)render_state.width * .5f - arena_x);
    int x1 = (int)((float)render_state.width * .5f + arena_x);
    int y0 = (int)((float)render_state.height * .5f - arena_y);
    int y1 = (int)((float)render_state.height * .5f + arena_y);

    draw_rect_in_pixels(0, 0, render_state.width, y0, color);
    draw_rect_in_pixels(0, y1, x1, render_state.height, color);
    draw_rect_in_pixels(0, 0, x0, y1, color );
    draw_rect_in_pixels(x1, y0, render_state.width, render_state.height, color);
}


internal void
draw_rect(float x, float y, float half_size_x, float half_size_y,  u32 color)
{
    x *= render_state.height * render_scale;
    y *= render_state.height * render_scale;
    half_size_x *= render_state.height * render_scale;
    half_size_y *= render_state.height * render_scale;

    x += render_state.width / 2.f;
    y += render_state.height / 2.f;


    //Change to pixels( Schimbam in pixwli)
    int x0 = static_cast<int>(x - half_size_x);
    int x1 = static_cast<int>(x + half_size_x);
    int y0 = static_cast<int>(y - half_size_y);
    int y1 = static_cast<int>(y + half_size_y); // Dupa static_cast viteza cubului e perfecta.

    // De retinut: Am facut static cast la float pentru ca primeam erori :
    //CLionProjects\PlatformerGame\src\renderer.cpp(110): warning C4244: 'initializing': conversion from 'float' to 'int', possible loss of data

    draw_rect_in_pixels(x0, y0, x1, y1, color);
}

/* EXPLICATIE DREPTUNGHI DIMENSIUNI:
 * x și y reprezintă coordonatele centrului dreptunghiului.
 * size controlează dimensiunea dreptunghiului.
 Este unitară, deci dimensiunea dreptunghiului este proporțională cu aceasta.

 * half_size reprezintă jumătate din dimensiunea dreptunghiului.
 Aceasta este folosită pentru a desena dreptunghiul în jurul punctului central (x, y)

 * color este culoarea dreptunghiului.
 * Folosirea size * 2.f și half_size permit controlul asupra poziționării și dimensiunii segmentelor liniare
 ->pentru a forma "5".
 De asemenea, poate fi folosit pentru a scalarea mărimii numerelor relative la ecran sau a scenei,
 ->asigurând că numerele nu devin prea mari sau prea mici indiferent de dimensiunea ecranului. */


/* EXPLCIATIE LOOP NUMBER DIGIT:
  Despre WHILE(number):
rimul pas în funcție este un bloc while care rulează cât timp variabila number este diferită de zero.
Acesta este un model obișnuit în C++ pentru a extrage fiecare cifră dintr-un număr în timp ce împărțim numărul
-> original la 10 (adică îl împingem la dreapta în sistemul zecimal).
Acest lucru este posibil deoarece în C++ (și mulți alți limbaje de programare),
-> 0 este tratat ca false iar orice alt număr este tratat ca true într-un context boolean.

int digit = number % 10;
number /= 10; :
 * number % 10 calculează restul împărțirii numărului la 10, care va fi întotdeauna ultima cifră a numărului.
 * Variabila digit stochează această cifră.
 *La următorul pas, number este împărțit la 10 (number /= 10).
 *Acest lucru îndepărtează ultima cifră din number împingând fiecare cifră în dreapta.
 *De exemplu, dacă number este 123, după această linie number va fi 12.

 *Prin repetarea acestor două operațiuni, bucla va extrage fiecare cifră din number, începând de la dreapta,
 *și va face ceva cu ea (în acest caz, va desena reprezentarea sa grafică) până când number devine 0 și bucla se oprește.

 *De exemplu, dacă numărul inițial este 123, primul digit va fi 3, apoi 2, apoi 1.
 *După ce toate cifrele au fost procesate, number devine 0 și bucla se termină. */

/* EXPLICATIE BUCLA bool drew_number = false;
 .. while(number || !drew_number) // buclea va continua să ruleze dacă number este diferit de zero (adică mai sunt cifre pentru a fi desenate), sau dacă niciun număr nu a fost încă desenat (!drew_number va fi adevărat dacă drew_number este false).
{
drew_number = true;   // a fost desenat cel puțin un număr.
int digit = number % 10;
number = number / 10; ..

Acest cod vă permite să desenați "0" corect.
Fără acest cod, dacă numărul introdus este "0", bucla while nu va fi executată niciodată, ->
->deoarece condiția while(number) este falsă când numărul este zero.
 Prin adăugarea variabilei drew_number, "0" va fi desenat încă o dată înainte de a ieși din buclă.
 */

const char* letters[][7] = {
    {
    " 00",
    "0  0",
    "0  0",
    "0000",
    "0  0",
    "0  0",
    "0  0"
},
{
    "000",
    "0  0",
    "0  0",
    "000",
    "0  0",
    "0  0",
    "000"
},
{
    " 000",
    "0   0",
    "0",
    "0",
    "0",
    "0   0",
    " 000"
},
{
    "000",
    "0  0",
    "0   0",
    "0   0",
    "0   0",
    "0  0",
    "000"
},
{
    "0000",
    "0",
    "0",
    "0000",
    "0",
    "0",
    "0000"
},
{
    "0000",
    "0",
    "0",
    "000",
    "0",
    "0",
    "0"
},
{
    " 000",
    "0   0",
    "0",
    "0 000",
    "0   0",
    "0   0",
    " 000"
},
{
    "0   0",
    "0   0",
    "0   0",
    "00000",
    "0   0",
    "0   0",
    "0   0"
},
{
    "000",
    " 0",
    " 0",
    " 0",
    " 0",
    " 0",
    "000"
},
{
    "    0",
    "    0",
    "    0",
    "    0",
    "    0",
    "0   0",
    " 000"
},
    {
    "0   0",
    "0  0",
    "0 0",
    "00",
    "0 0",
    "0  0",
    "0   0"
},
{
    "0",
    "0",
    "0",
    "0",
    "0",
    "0",
    "0000"
},
    {
    "0   0",
    "00 00",
    "0 0 0",
    "0   0",
    "0   0",
    "0   0",
    "0   0"
},
    {
    "0   0",
    "00  0",
    "0 0 0",
    "0  00",
    "0   0",
    "0   0",
    "0   0"
},
    {
    " 000",
    "0   0",
    "0   0",
    "0   0",
    "0   0",
    "0   0",
    " 000"
},
    {
    "0000",
    "0   0",
    "0   0",
    "0000",
    "0",
    "0",
    "0"
},
    {
    " 000",
    "0   0",
    "0   0",
    "0   0",
    "0 0 0",
    "0  0 0",
    " 000 0"
},
    {
    "0000",
    "0   0",
    "0   0",
    "0000",
    "0  0",
    "0   0",
    "0   0"
},
    {
    " 000",
    "0   0",
    "0",
    " 000",
    "    0",
    "0   0",
    " 000"
},
    {
    "00000",
    "  0",
    "  0",
    "  0",
    "  0",
    "  0",
    "  0"
},
    {
    "0   0",
    "0   0",
    "0   0",
    "0   0",
    "0   0",
    "0   0",
    " 000"
},
    {
    "0   0",
    "0   0",
    "0   0",
    "0   0",
    "0   0",
    " 0 0",
    " 0 0"
},
    {
    "0   0",
    "0   0",
    "0   0",
    "0   0",
    "0 0 0",
    "00 00",
    "0   0"
},
{
    "0   0",
    "0   0",
    " 0 0",
    " 0 0",
    " 0 0",
    "0   0",
    "0   0"
},
    {
    "0   0",
    "0   0",
    " 0 0",
    "  0",
    "  0",
    "  0",
    "  0"
},
{
    "0000",
    "   0",
    "  0",
    " 0",
    "0",
    "0",
    "0000"
},
{
    // For "."
   "",
   "",
   "",
   "",
   "",
   "",
   " 0"
},
{
    // For "/"
    "     0",
    "    0",
    "   0",
    "  0",
    " 0",
    "0",
    ""
},
{
    // For ":"
    "",
    " 0",
    "",
    "",
    "",
    " 0",
    ""
},
};
    // Un array care conține matricele de caractere pentru literele alfabetului de la A la Z, fiecare reprezentând literele sub formă de caractere ASCII art.
//ar trebui să conțină doar șapte rânduri, nu opt, deoarece fiecare șir de caractere ASCII art trebuie să reprezinte o linie separată în array-ul bidimensional.
    // Ca sa il citim facem un internal void, draw_text.

internal void
draw_text(const char *text, float x, float y, float size, u32 color)      // functia o sa primeasca o pizitie, un size si o culoare)
{
    float half_size = size * .5f;
    float original_y = y;

    while (*text)
    {
        if (*text != 32) {
           // const char ** a_letter= letters[*text -'A']; // este un array care are un pointer catre un caracter poiter string
            // Litera A este 65
            const char ** letter;
            if (*text == 47) letter = letters[27];   // for "/"
            else if ( *text == 46) letter = letters[26];  // for "."
            else if (*text == 58) letter = letters[28]; // For ":"
            else letter = letters[*text -'A'];
            float original_x = x;

            for (int i = 0; i < 7; i++) {
                //const char *row = a_letter[i];
                const char *row = letter[i];
                while (*row)
                {
                    if (*row == '0')
                    {
                        draw_rect(x, y, half_size, half_size, color);
                    }
                    x += size;
                    row++;
                }
                y -= size;
                x = original_x;
            }
        }
        text++;
        x += size * 6.f;
        y = original_y;
    }


}



internal void
draw_number(int number, float x, float y, float size, u32 color)
{
    float half_size = size * .5f;

    bool drew_number = false;
    while(number || !drew_number) // buclea va continua să ruleze dacă number este diferit de zero (adică mai sunt cifre pentru a fi desenate), sau dacă niciun număr nu a fost încă desenat (!drew_number va fi adevărat dacă drew_number este false).
    {
        drew_number = true;   // a fost desenat cel puțin un număr.
        int digit = number % 10;
        number = number / 10;

 switch(digit) {
     case 0 :  // In case-ul asta desenam un 0. Cand luam un gol nu vedem restul numerelor.
     {
         draw_rect(x - size, y, half_size, 2.5f * size, color);
         draw_rect(x + size, y, half_size, 2.5f * size, color);
         draw_rect(x, y + size * 2.f, half_size, half_size,color);
         draw_rect(x, y - size * 2.f, half_size, half_size,color);
          x -= size * 4.f; // trebuie sa mutam pozitia lui x mereu cand desenam digital, la fel ca la patrat.
     } break;

     case 1:
     {
         draw_rect(x + size, y, half_size, 2.5f * size, color);
         x -= size * 2.f; // case 1 este exceptia de la patrat, este suficient de 2 ori mai mare.
     } break;

     case 2:
     {
         draw_rect(x, y + size * 2.f, 1.5f * size, half_size,color);
         draw_rect(x, y, 1.5f * size, half_size, color);
         draw_rect(x, y -size * 2.f, 1.5f * size, half_size, color);
         draw_rect(x + size, y +size, half_size, half_size, color);
         draw_rect(x - size, y - size, half_size, half_size, color);
         x -= size * 4.f;
     } break;;

     case 3:
     {
         draw_rect(x - half_size, y + size *2.f, size, half_size, color);
         draw_rect(x - half_size, y, size , half_size, color);
         draw_rect(x - half_size, y - size *2.f, size, half_size, color);
         draw_rect(x + size, y, half_size, 2.5f *size, color);
         x -= size * 4.f;
     } break;

     case 4:
     {
         draw_rect(x + size, y ,half_size, 2.5f *size, color);
         draw_rect(x - size, y + size ,half_size, 1.5f *size, color);
         draw_rect(x, y, half_size, half_size, color);
         x -= size * 4.f;
     } break;

     case 5:
     {
         draw_rect(x, y + size * 2.f, 1.5f *size, half_size,color); //desenează linia de sus a cifrei "5".
         draw_rect(x, y, 1.5f * size, half_size, color); //desenează partea din mijloc a cifrei "5"
         draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);//desenează linia de jos a cifrei "5".
         draw_rect(x - size, y + size, half_size, half_size, color);//desenează linia din stânga sus a cifrei "5"
         draw_rect(x + size, y - size, half_size, half_size, color);//desenează linia din dreapta jos a cifrei "5".
         x -= size * 4.f;
     } break;

     case 6:
     {
         draw_rect(x + half_size, y + size * 2.f, size, half_size, color);
         draw_rect(x + half_size, y, size, half_size, color);
         draw_rect(x + half_size, y - size * 2.f, size, half_size, color);
         draw_rect(x - size, y, half_size, 2.5f * size, color);
         draw_rect(x + size, y - size, half_size, half_size, color);
         x -= size * 4.f;
     } break;

     case 7:
     {
         draw_rect(x + size, y, half_size, 2.5f * size, color);
         draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
         x -= size * 4.f;
     } break;

     case 8:
     {
         draw_rect(x - size, y, half_size, 2.5f * size, color);
         draw_rect(x + size, y, half_size, 2.5f * size, color);
         draw_rect(x, y +size * 2.f, half_size, half_size, color);
         draw_rect(x, y - size * 2.f, half_size, half_size, color);
         draw_rect(x, y, half_size, half_size, color);
         x -= size * 4.f;
     }break;

     case 9:
     {
         draw_rect(x - half_size, y + size *2.f, size, half_size, color);
         draw_rect(x - half_size, y, size, half_size, color);
         draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
         draw_rect(x + size, y, half_size, 2.5f * size, color);
         draw_rect(x - size, y + size, half_size, half_size, color);
         x -= size * 4.f;
     } break; // Mentionez ca 0 nu apare dar o sa modific functia while probabil in do while sau ceva bool drew_number = false;
   }
 }
}