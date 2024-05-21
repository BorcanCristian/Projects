// Am mutat logica din win32 aici.
// Task: Facem un macro pentru a nu mai face array mereu.

/* De retinut si recapitulat:
 Utilizarea parantezelor în definițiile de preprocesor în C++ este o practică bună din două motive majore:
  - operator precedence și expansion issues.
 && = AND(si)  : Pentru două valori booleane, operatorul && va returna true dacă ambele valori sunt true.
  Dacă fiecare dintre condițiile evaluate cu operatorul && nu este true, rezultatul întregii expresii va fi false.
  In plus, operatorul && prezintă "short-circuiting":
  Acesta înseamnă că dacă prima expresie evaluată este false, a doua expresie nu va fi evaluată, deoarece rezultatul final va fi în mod sigur false,
  indiferent de valoarea celei de-a doua expresii.
  Acest comportament este util pentru prevenirea erorilor în anumite situații,
  cum ar fi verificarea unei condiții care ar putea cauza o eroare dacă este evaluată și prima condiție deja a eșuat.
 */

#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)


// float player_pos_x = 0.f;
// float player_pos_y = 0.f;
float player_1_p, player_1_dp, player_2_p, player_2_dp; // facem update la pozitie
                                                         // Facem alt update pentru un player secundar
float arena_half_size_x = 85;
float arena_half_size_y = 45;
float player_half_size_x = 2.5;
float player_half_size_y = 12;
float ball_p_x;
float ball_p_y;
float ball_dp_x = -50;  // ball speed , daca punem numar negativ bila se duce in stanga
float ball_dp_y; // viteza de inceput a bilei pe verticala
float ball_half_size = 1;
float acceleration_factor = 1.2f; // Marim procentul cu 20% dupa fiecare lovitura.
int last_player_hit = 0; // 0 = nimeni nu a lovit icna mingea, 1 pt player 2 , 2 pt player 2.
/* TUTORIAL DESPRE ACCELERARE SI COLIZIUNE:
 * Pentru o accelerație mai lentă, puteți utiliza un număr mic, dar mai mare decât 1.
 * De exemplu, pentru a adăuga 10% la viteza fiecarei coliziuni, ->
 -> ați putea utiliza un factor de accelerare de 1.1.
 * Acest lucru va mări viteza cu 10% la fiecare coliziune, ->
 -> ducând la o accelerare progresivă de-a lungul mai multor coliziuni.
*/



int player_1_score;
int player_2_score;
const float speed_increment = 200.0f;

internal void
simulate_player(float *p, float *dp, float ddp, float dt)
{
 ddp -= *dp * 10.f; // Decceleration

 *p = *p + *dp * dt + ddp * dt * dt * .5f; // Modificare pentru accelerare in timp real.
 *dp = *dp + ddp * dt; // Problema, fuge prea repede si dispare.


 //Verifică dacă partea de sus a jucătorului 1 (player_1_p + player_half_size_y) depășește partea de sus a arenei (definită de arena_half_size_y).
 //Dacă se întâmplă acest lucru, atunci codul resetează poziția verticală a jucătorului (player_1_p) astfel încât partea de sus a jucătorului să fie exact la marginea superioară a arenei.
 if(*p + player_half_size_y > arena_half_size_y)
 {
  *p = arena_half_size_y - player_half_size_y; // valoarea maximă a lui y pe care o poate avea jucătorul este arena_half_size_y și, pentru a vă asigura că jucătorul nu iese în afara arenei, scădeți din aceasta jumătate din înălțimea jucătorului (player_half_size_y).
  *dp = 0;     //are scopul de a opri mișcarea jucătorului 1, simbolizând efectul de lovire a unui perete sau a unei bariere.
  // Limitare si coliziunea player cu peretele interior al arenei sus
 }                    // daca ii dam *= -1, la coliziune va face bounce(lovituri multiple)
 else if(*p - player_half_size_y < -arena_half_size_y)// înseamnă că partea de jos a jucătorului 1 a trecut de marginea de jos a arenei.
 {
  //De retinut ca avem -arena_half_size_y
  *p = -arena_half_size_y + player_half_size_y; //setează ulterior poziția jucătorului 1 astfel încât partea de jos a lui să fie exact la marginea arenei.
  *dp = 0;   // are scopul de a opri mișcarea jucătorului 1, simbolizând efectul de lovire a unui perete sau a unei bariere daca este setat pe *= -1 sau -2.
 }
}


/* Cod inlocuit in functia return:
 * p1x <- ball_p_x
 * p1y <- ball_p_y
 * p2x <- 80
 * p2y <- player_1_p
 * hs1x <- ball_half_size
 * hs1y <- ball_half_size
 * hs2x <- player_half_size_x
 * hs2y <- player_half_size_y
*/

internal bool // Noua functie de coliziune
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y)
{
     return (p1x + hs1x > p2x - hs2x &&
      p1x - hs1x < p2x + hs2x &&
      p1y + hs1y > p2y - hs2y &&
      p1y + hs1y < p2y + hs2y);
}

// Meniul pentru a separa codul
enum GameMode
{
GM_MENU,
GM_GAMEPLAY,
};

GameMode current_gamemode;
int hot_button;
bool enemy_is_AI;
static int goluri_primite = 0; // variabila retine cate goluri au fost primite

internal void
simulate_game(Input* input,float dt) {
 // clear_screen(0x4b0082);
 draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x00008B); //cusca
draw_arena_borders(arena_half_size_x, arena_half_size_y, 0x4b0082 );


 if(current_gamemode == GM_GAMEPLAY) {
  /*
  // Dupa implementarea delta_time, impartirea lui si adaugarea speed * dt la if, problema vitezei in scalare s-a rezolvat.
  // Pentru a verifica du-te in win32 si in game sub acest comentariu. (stiu ca uiti repede)
 */
  // Player 1 settings:
  // float speed =50.f; // unitati pe secunda
  float player_1_ddp = 0.f; //  <-float speed =50.f;




  /*    // if (input->buttons[BUTTON_UP].is_down) //Am inlocuit . cu ->

         // if(is_down(BUTTON_UP)) player_pos_y += speed * dt;
         // if(is_down(BUTTON_DOWN)) player_pos_y -= speed * dt;
         // if(is_down(BUTTON_RIGHT)) player_pos_x += speed * dt;
         // if(is_down(BUTTON_LEFT)) player_pos_x -= speed * dt;

        // In momentul de fata avem units per frame, deci cand modificam chenarul cubul o sa devina mai mare
       // sau mai mic si viteza lui se adapteaza.
  // Task: Fa viteza cubul la secunda constanta. Converteste din frame la secunde.( sec_per_frame)




 // Task: Schimba logica pentru a evita multiplele apasari.
 // Hint: Nu vrem sa avem multiple time press.
 /* Rezolvare: Du-te si schimba variabila in case:vk  din win32 si o muti deasupra.
  input.buttons[b].changed = true -> input.buttons[b].changed = is_down != unput.buttons[b].is_down;\ */
  /*
 // Vechiul cod
          // if(pressed(BUTTON_UP)) player_p += speed;
          // if(pressed(BUTTON_DOWN)) player_p -= speed;
          // if(pressed(BUTTON_RIGHT)) player_p += speed;
          // if(pressed(BUTTON_LEFT)) player_p -= speed;    */


 // else if  (ball_p_x - ball_half_size < -arena_half_size_x)
 // {
 //
 //  // Jucatorul 2 marcheaza un gol
 //  player_2_score++;
 //
 //  // Crește viteza pentru jucatorul 1
 //  player_1_speed += speed_increment;





  if
   (!enemy_is_AI)
   {// # if 0 -> Efect inainte sa compileze, la preprocesor Deci daca conditia e falsa codul din #if .. #endif nu o sa fie compilat.
    //Am nevoie doar up si down si is_down *dt pentru a misca fluent un player.
    if(is_down(BUTTON_UP)) player_1_ddp += 2000;
    if(is_down(BUTTON_DOWN)) player_1_ddp -= 2000;
   }
  else if (ball_p_x - ball_half_size < -arena_half_size_x)
  {
   // Jucatorul 2 marcheaza un gol
   player_2_score++;

   // Mărim numărul de goluri primite
     goluri_primite++;

   // Crește viteza pentru jucatorul 1
   player_1_ddp += goluri_primite * speed_increment;
  }
  else {
   // #else il scoatem si punem else.
   // if (ball_p_y > player_1_p+2.f) player_1_ddp += 1300; // aceste doua cazul sunt pt LVL HARD/ EXTREME (2000)
   // if (ball_p_y < player_1_p-2.f) player_1_ddp -= 1300; // 1300 - este Medium
   player_1_ddp = (ball_p_y - player_1_p) * speed_increment;
   // se misca foate slow :))) asa ca il inmultim cu *100. merue la inmultiri folosesti paranteze

   if (player_1_ddp > 2500) player_1_ddp = 2500;
   if (player_1_ddp < -2500) player_1_ddp = -2500; // Daca uiti sa pui - la 1300 in paranteza nu se misca cum trebuie robotu
  }                                       // #endif il scoatem si pe el
  /* Modificam player 1 in functia simulate_player

   player_1_ddp -= player_1_dp * 10.f; // Decceleration

   player_1_p = player_1_p + player_1_dp * dt + player_1_ddp * dt * dt * .5f; // Modificare pentru accelerare in timp real.
   player_1_dp = player_1_dp + player_1_ddp * dt; // Problema, fuge prea repede si dispare.


   //Verifică dacă partea de sus a jucătorului 1 (player_1_p + player_half_size_y) depășește partea de sus a arenei (definită de arena_half_size_y).
  //Dacă se întâmplă acest lucru, atunci codul resetează poziția verticală a jucătorului (player_1_p) astfel încât partea de sus a jucătorului să fie exact la marginea superioară a arenei.
   if(player_1_p + player_half_size_y > arena_half_size_y)
   {
    player_1_p = arena_half_size_y - player_half_size_y; // valoarea maximă a lui y pe care o poate avea jucătorul este arena_half_size_y și, pentru a vă asigura că jucătorul nu iese în afara arenei, scădeți din aceasta jumătate din înălțimea jucătorului (player_half_size_y).
    player_1_dp = 0;     //are scopul de a opri mișcarea jucătorului 1, simbolizând efectul de lovire a unui perete sau a unei bariere.
                         // Limitare si coliziunea player cu peretele interior al arenei sus
   }                    // daca ii dam *= -1, la coliziune va face bounce(lovituri multiple)
   else if(player_1_p - player_half_size_y < -arena_half_size_y)// înseamnă că partea de jos a jucătorului 1 a trecut de marginea de jos a arenei.
   {
    //De retinut ca avem -arena_half_size_y
    player_1_p = -arena_half_size_y + player_half_size_y; //setează ulterior poziția jucătorului 1 astfel încât partea de jos a lui să fie exact la marginea arenei.
    player_1_dp = 0;   // are scopul de a opri mișcarea jucătorului 1, simbolizând efectul de lovire a unui perete sau a unei bariere daca este setat pe *= -1 sau -2.
   }
  */
  // Player 2 settings
  float player_2_ddp = 0.f; //  <-float speed =50.f;

  if(is_down(BUTTON_W)) player_2_ddp += 2000;  // trebuie sa schimbam si in Enum din Input.
  if(is_down(BUTTON_S)) player_2_ddp -= 2000;   // Apoi in win32 in switch(vs_code)

  simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
  simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);


  /*
  // Cod coliziune player 2 cu arena:  / Am sistematizat player 1 si player 2 impreuna in simulate_player

  player_2_ddp -= player_2_dp * 10.f; // Decceleration

  player_2_p = player_2_p + player_2_dp * dt + player_2_ddp * dt * dt * .5f; // Modificare pentru accelerare in timp real.
  player_2_dp = player_2_dp + player_2_ddp * dt; // Problema, fuge prea repede si dispare.


  if(player_2_p + player_half_size_y > arena_half_size_y)
  {
   player_2_p = arena_half_size_y - player_half_size_y; // valoarea maximă a lui y pe care o poate avea jucătorul este arena_half_size_y și, pentru a vă asigura că jucătorul nu iese în afara arenei, scădeți din aceasta jumătate din înălțimea jucătorului (player_half_size_y).
   player_2_dp = 0;     //are scopul de a opri mișcarea jucătorului 2, simbolizând efectul de lovire a unui perete sau a unei bariere.
   // Limitare si coliziunea player cu peretele interior al arenei sus
  }                    // daca ii dam *= -1, la coliziune va face bounce(lovituri multiple)
  else if(player_2_p - player_half_size_y < -arena_half_size_y)// înseamnă că partea de jos a jucătorului 2 a trecut de marginea de jos a arenei.
  {
   //De retinut ca avem -arena_half_size_y
   player_2_p = -arena_half_size_y + player_half_size_y; //setează ulterior poziția jucătorului 2 astfel încât partea de jos a lui să fie exact la marginea arenei.
   player_2_dp = 0;   // are scopul de a opri mișcarea jucătorului 2, simbolizând efectul de lovire a unui perete sau a unei bariere daca este setat pe *= -1 sau -2.
  }
 //... */
  // MEGA HINT : Niciodata sa nu desenati mingea peste pozitie..
  // PRobleme la miscare si orientare, sirul codului este esential.

  //Simulate Ball
  {
   // Pozitie ball in joc:
   ball_p_x += ball_dp_x * dt;
   ball_p_y += ball_dp_y * dt;

   // COliziune ball AABB:
   // Coliziune player Dreapta
   // coliziunea este un succes pentru partea din dreapta, facem niste copy si marim functia
   /* //Problema la coliziune, ramane salvata chiar daca te muti.
    if (ball_p_x + ball_half_size > 80 - player_half_size_x &&
        ball_p_x - ball_half_size < 80 + player_half_size_x &&
        ball_p_y + ball_half_size > player_1_p - player_half_size_y &&
        ball_p_y + ball_half_size < player_1_p + player_half_size_y ) // pentru playerul din dreapta
    {
     ball_dp_x = 80 - player_half_size_x - ball_half_size; // Limitare pozitie minge
     ball_dp_x *= -1; // Cum am mai zis *= -1 ofera efectul de hit and back
     // ball_dp_y = player_1_dp* .75f; // Ajustează viteza pe axa y a mingii (ball_dp_y) în funcție de viteza actuală a jucătorului 1 pe axa y (player_1_dp).
                               // Practic, acest lucru înseamnă că atunci când minge se ciocnește cu jucătorul 1-
                               //->direcția de deplasare a mingii pe axa verticală va fi influențată de direcția în care se mișcă jucătorul în acel moment.
                               //* .75f; -> reduce viteza verticală a mingii la 75% din viteza jucătorului la momentul impactului.
                               //Asta efectiv face ca traiectoria mingii să fie influențată de mișcarea jucătorului, oferind astfel jucătorul un mai mare control asupra jocului.
    */


   // ADaugam un factor de multiplicara la ball_dp_x si ball_dp_y
   //Noul If aabb pentru player 1(dreapta)
   if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y))
   {
    ball_dp_x = 80 - player_half_size_x - ball_half_size; // Limitare pozitie minge
    ball_dp_x *= -1 * acceleration_factor; // Adaugam un acceleration pentru a mari viteza dupa impact
    //BAll UP DOWN player dreapta
    ball_dp_y =  (ball_p_y - player_1_p) * 2 + player_1_dp * .75f;
    ball_dp_y *= acceleration_factor;  // inmultim si ball dp y
    if(last_player_hit != 1)
     { // Dacă ultimul jucător a fost altul decât player_1, accelerați mingea
     acceleration_factor = 1.1f * acceleration_factor;
    }
    last_player_hit = 1;
   }
   /** Vechiul cod inainte de aabb
   // Colizune pentru player 2 stranga
   else if(ball_p_x + ball_half_size > -80 - player_half_size_x &&
     ball_p_x - ball_half_size < -80 + player_half_size_x &&
     ball_p_y + ball_half_size > player_2_p - player_half_size_y &&
     ball_p_y + ball_half_size < player_2_p + player_half_size_y ) // pentru playerul din dreapta
   {
    ball_dp_x = -80 + player_half_size_x + ball_half_size; // Limitare pozitie minge
    ball_dp_x *= -1; // Cum am mai zis *= -1 ofera efectul de hit and back
    ball_dp_y =  (ball_p_y - player_2_p)*2 + player_2_dp* .75f;
   }  */
   else  if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y))
   {
    ball_dp_x = -80 + player_half_size_x + ball_half_size; // Limitare pozitie minge
    ball_dp_x *= -1 * acceleration_factor; // Cum am mai zis *= -1 ofera efectul de hit and back
    ball_dp_y =  (ball_p_y - player_2_p) * 2 + player_2_dp * .75f;
    ball_dp_y *= acceleration_factor; // viteza lui y cu factorul de acc la coliziune
     if(last_player_hit != 2)
     { // Dacă ultimul jucător a fost altul decât player_2, accelerați mingea
        acceleration_factor = 1.1f * acceleration_factor;
    }
    last_player_hit = 2;
   }

   // Limitare arena top
   if(ball_p_y + ball_half_size > arena_half_size_y)
   {
    ball_p_y = arena_half_size_y - ball_half_size;
    ball_dp_y *= -1;                             // bounce top recoil.
   }

   //Limitare arena down
   else if(ball_p_y - ball_half_size < -arena_half_size_y)
   {
    ball_p_y = -arena_half_size_y + ball_half_size;
    ball_dp_y *= -1;                                  // bounce bouce ball for down
   }

   /* // Update introducem sistemul de score (player_1_score++)
    // Incepe de la playerul care pierde dupa ce ia gol
    // player dreapta (jucator 1) */
   if (ball_p_x + ball_half_size > arena_half_size_x) // Verifica daca mingea a depasit limitele arenei de joc
   {
    ball_dp_x *= 1;                              // lasam pe -1 ca sa inceapa de la el
    ball_dp_y = 0;
    ball_p_x = 0;
    ball_p_y = 0;
    acceleration_factor = 1.2f; // Reset acceleration after gol.
    player_1_score++;
   }
   //player stranga ( jucator 2) + sistem gol p2
   else  if (ball_p_x - ball_half_size < -arena_half_size_x)
   {
    ball_dp_x *= 1;                               // daca vrem sa inceapa de la adversar lasam pe 1
    ball_dp_y = 0;
    ball_p_x = 0;
    ball_p_y = 0;
    acceleration_factor = 1.2f; // Adaugam pentru a reseta accelerare mereu dupa gol.
    player_2_score++;   // right now all we can drow recangles so maybe we can do like one reangle for every point score
   }
  }

  draw_number(player_1_score, -20, 40, 1.f, 0x999999);
  draw_number(player_2_score, 20, 40, 1.f, 0x999999);

  /* Sistem trivial pentru loading si score
 // Scor pentru player 1
  float at_x = -80;  //pozitie pentru rect
 for (int i= 0; i < player_1_score; i++)
 {
  draw_rect(at_x, 47.f, 1.f, 1.f, 0xffffff);
  at_x += 2.5f;
 }

  //Scor pentru player 2               //note: see declaration of 'at_x' , float at_x = 80 -> at_x = 80.
  at_x = 80;  //pozitie pentru rect   // Daca pui float in fata iar primesti  error C2374: 'at_x': redefinition; multiple initialization
  for (int i= 0; i < player_2_score; i++)
  {
   draw_rect(at_x, 47.f, 1.f, 1.f, 0xffffff);
   at_x -= 2.5f;
  }
  */

  // Rendering - Aici midificam player-ul si bila.

  // Deseneaza mingea
  draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);
  // Aveam eroare, disparea mingea la impact cand mutam player 1 (y) din cauza ca foloseam ball_dp_y in draw_rect.


  // draw_rect(player_pos_x, player_pos_y, 1, 1, 0x00ff22) - Cod anterior
  draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0x00ff22); // ii dam paramentru de player_p pentru a il misca
  draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0x00ff22);
  // Am modificat x si y si am facut centralizarea in renderer.cpp /2.

 } else
 {
  if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT))      // adaugi butoane in enum din input_handler.
  {
   hot_button = !hot_button;
  }


  if (pressed(BUTTON_ENTER))
  {
   current_gamemode = GM_GAMEPLAY;
   enemy_is_AI = hot_button? 0 : 1;
  }

  if (hot_button == 0)
  {
   draw_text("TRY ME BRO", -80, -5, 1.2, 0xff0000); // cand modifici la if pe primul mofici in else pe al doilea
   draw_text("MULTYPLAYER", 5, -10, 1, 0xaaaaaa);
   // draw_rect(20, 0, 10, 10, 0xFF0000 + 0X8B0000); // SInglePlayer
   // draw_rect(-20, 0, 10, 10, 0xcccccc);// MultiPlayer player
  }else
  {
   draw_text("TRY ME BRO", -70, -10, 1,0xaaaaaa);  // ma jucai cu valorile incat sa fac un efect de tranzitie
   draw_text("MULTYPLAYER", 2, -5, 1.2,0xff0000);      //draw_rect schita pentru test patrate
   // draw_rect(20, 0, 10, 10, 0xcccccc); // SInglePlayer
   // draw_rect(-20, 0, 10, 10, 0xFF0000 + 0X8B0000);// MultiPlayer player
  }

  // draw_text("TUTORIAL", 0, 0, 1, 0xff00ff);  //Nu uita sa comentezi despre array-ul pentru litere facut in renderer.cpp
  // L-am mutat la final pentru a randa textul peste mapa.

draw_text("PROJECT IT SCHOOL", -73, 40, 1.5, 0xff00ff);
draw_text("FOR MORE INFORMATION CONTACT US ON DISCORD", -70, 22, .55, 0xff00ff);
  draw_text("DISCORD.GG/BORCANCRISTIAN", -28, 15.5, .35, 0xff00ff);

  draw_text("MENU", -20, 8.5, 1.5, 0xff00ff);

  draw_text("MORE INFORMATION ON THE GITHUB:", -55.5, -35, .65, 0xff00ff); //jos pagina
  draw_text("HTTPS://GITHUB.COM/BORCANCRISTIAN/", -40, -41, .45, 0xff00ff);


 }
}
