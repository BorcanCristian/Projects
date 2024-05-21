#include "Utility.cpp"
#include <windows.h>

// Am modificat cmake pentru a putea folosi win32/ int WinMain.



/* Facand variabilă globală, running poate fi accesată și modificată din oriunde în cod,
 * ceea ce înseamnă că orice funcție sau secțiune de cod poate declanșa închiderea jocului
 * prin setarea running la false. */
global_variable bool running = true;

struct Render_State
{
    int height;
    int width;
    void* memory;

    BITMAPINFO bitmap_info;

};
/* Făcând render_state o variabilă globală, ne asiguram că toate aceste funcții pot accesa și modifica starea de randare,
 * fără a fi nevoie să transmitem explicit starea ca un argument fiecărei funcții.
 * Acest lucru poate face codul mai simplu și mai ușor de citit. */
global_variable Render_State render_state;
// void* render_state.memory;
// int render_state.width;
// int render_state.height;
// BITMAPINFO render_state.bitmap_info;
// Am facut struct si comentat vechiul cod.

// declaram Render_state ca fiind globala si eliminam renderer.cpp din cmake pentru a putea sa o folosim si compila.
#include "Input_handler.cpp"
#include "renderer.cpp"
#include "game.cpp"

LRESULT CALLBACK window_callback( HWND hwnd, UINT uMSG, WPARAM wParam, LPARAM lParm)
{
    LRESULT result = 0;  // Variabila result
    switch (uMSG)
    {
        case WM_CLOSE:
            case WM_DESTROY:
        {
         running = false;
        }
            break;

        case WM_SIZE:
        {
            RECT rect; //rect struct
            GetClientRect(hwnd, &rect);

            //Linia de cod int width = rect.right - rect.left; calculează lățimea unui dreptunghi prin scăderea valorii marginii stângi (rect.left) din valoarea marginii drepte (rect.right).
            render_state.width = rect.right - rect.left;



            //Linia render_state.height = rect.bottom - rect.top; calculează înălțimea unui dreptunghi prin substracția valorii marginii superioare (rect.top) din valoarea marginii inferioare (rect.bottom).
            render_state.height = rect.bottom - rect.top;




            //Linia de cod int size = render_state.width * render_state.height*sizeof(unsigned int); înmulțește lățimea (width) cu înălțimea (height) pentru a obține numărul total de pixeli în dreptunghi.
            int size = render_state.width * render_state.height * sizeof(unsigned int);
            //Apoi acest număr de pixeli este înmulțit cu sizeof(unsigned int), care indică dimensiunea în byte a unui tip unsigned int în C++. Prin urmare, size va reprezenta dimensiunea totală în byte necesară pentru a stoca toți pixelii.
            //Această linie de cod este utilă atunci când ai nevoie să aloce memorie pentru a stoca o imagine sau o fereastră de pixeli într-un buffer unsigned int.


            //Linia if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE); verifică mai întâi dacă render_state.memory nu este NULL. Dacă nu este NULL, atunci memtoria alocată anterior pentru render_state.memory este eliberată.
            if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);

            render_state.memory = VirtualAlloc(0,size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        }

render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
        render_state.bitmap_info.bmiHeader.biPlanes = 1;
        render_state.bitmap_info.bmiHeader.biBitCount = 32;
        render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;

        break;


default:
        {
            result = DefWindowProc(hwnd, uMSG, wParam,lParm); // Pentru a intoarce un mesaj, daca lipseste nu compielza.
        }

    }
  return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nShowCmd)
{

    // ShowCursor(FALSE); // Mouse

    // Create a WINDOW CLASS
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = "Game Window Class";
    window_class.lpfnWndProc = window_callback;


    // Register CLASS

    RegisterClass(&window_class);


    // Create WINDOW
    HWND window = CreateWindow(window_class.lpszClassName,                   //lpClassName
        "PlatformerGame!",                   //lpWindowName
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,    //dwStyle
        CW_USEDEFAULT,                       //x
        CW_USEDEFAULT,                       //y
        1280,                                //nWidth
        720,                                 //nHeight
        0,                                   //hWndParent
        0,                                   //hMenu
        hInstance,                           //hInstance
        0                                    //lpParam
        );
// AM pus o vrigula dupa ultimul argument din fucntie ( 0 ) si primeam eroare la compilare desi complia normal
// warning C4002: too many arguments for function-like macro invocation 'CreateWindowA'
// Nu lasati erorile astea nerezolvate sau pe viitor o sa aveti probleme severe.
//Important sa fiti atenti la virgule, am stat ceva sa imi dau seama..
    // {
    //     //FULLSCREEN
    //     SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
    //     MONITORINFO mi = {sizeof(mi)};
    //     GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
    //     SetWindowPos(window, HWND_TOP,mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    // }  // COmenteaza codul sa scoti full screenn

    HDC hdc = GetDC(window);

// Rezolvam cu frame in secunde ( float delta_time)
    Input input = {};

    float delta_time = 0.016666f;
    LARGE_INTEGER frame_begin_time;
    QueryPerformanceCounter(&frame_begin_time);

    float performance_frequency;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performance_frequency = (float)perf.QuadPart;
    }


    while (running)  // Bucla pentru a ramane fereastra pornita.
    {
        // Input
        MSG message;

        for (int i = 0; i < BUTTON_COUNT; i++)
            {
            input.buttons[i].changed = false;
            }                                  // O sa includem input_handler.cpp acum

        while (PeekMessage(&message, window, 0 , 0 , PM_REMOVE ))
        {
            switch(message.message)
            {
                case WM_KEYUP:
                    case WM_KEYDOWN: {
                    u32 vk_code = (u32)message.wParam;
                    bool is_down = ((message.lParam & (1 << 31)) == 0);

//input.buttons[b].changed = true;\ modificata si mutata sus.
// Task terminat, se apasa doar o singura data.
#define process_button(b,vk)\
case vk:{\
input.buttons[b].changed = is_down != input.buttons[b].is_down;\
input.buttons[b].is_down = is_down;\
}break;\

                    switch (vk_code)
                    {
                        process_button(BUTTON_UP,VK_UP);
                        process_button(BUTTON_DOWN,VK_DOWN);
                        process_button(BUTTON_W,'W');   // <- process_button(BUTTON_LEFT,VK_LEFT)
                        process_button(BUTTON_S,'S'); // <- process_button(BUTTON_RIGHT,VK_RIGHT
                        process_button(BUTTON_LEFT,VK_LEFT);
                        process_button(BUTTON_RIGHT,VK_RIGHT);
                        process_button(BUTTON_ENTER,VK_RETURN);
                    }
                } break;


                 default:
                         {
                           TranslateMessage(&message);
                           DispatchMessage(&message);
                         }
            }
        }

        //SIMULATE
        //- adaugam render_background care are logica de la simulate mutata.

        //render_background();
       simulate_game(&input, delta_time);

        // Render
        StretchDIBits(
                    hdc,
                    0,
                    0,
                    render_state.width,
                    render_state.height,
                    0,
                    0,
                    render_state.width,
                    render_state.height,
                    render_state.memory,
                    &render_state.bitmap_info,
                    DIB_RGB_COLORS,
                    SRCCOPY
                    );



        // Marcam si finalul frameului.

/* LARGE_INTEGER este un tip de date definit în Windows API care este utilizat ->
 -> pentru operațiuni pe numere întregi pe 64 de biți.

 * QueryPerformanceCounter(&frame_end_time):
 ->este o apelare a funcției Windows API QueryPerformanceCounter
 ->care preia un șir de caractere LARGE_INTEGER ca argument
 ->si îi setează valoarea la numărul curent de "tick-uri"
 ->(unități de timp specifice sistemului) scurse de la pornirea sistemului.

 *Așa că, în esență, aceste două linii de cod salvează timpul curent ->
 ->(în "tick-uri") în variabila frame_end_time.
 Acest timp poate fi utilizat ulterior pentru a calcula cât timp a trecut de la un anumit punct în timp ->
 ->de exemplu, pentru a calcula durata unui cadru într-un joc în timp real.
 */\
        LARGE_INTEGER frame_end_time; // declară o variabilă frame_end_time de tip LARGE_INTEGER.
        QueryPerformanceCounter(&frame_end_time);
        delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
        frame_begin_time = frame_end_time;



        /*Substracția frame_end_time.QuadPart - frame_begin_time.QuadPart calculează:
         ->diferența de timp dintre începutul și sfârșitul cadrelor, în "ticks".
        Apoi acest rezultat este împărțit la performance_frequency, care reprezintă:
        ->frecvența contorului de performanță, exprimată în "ticks" pe secundă.
        Vă oferă numărul de "ticks" pe secundă pe care le poate efectua cronometrul de înaltă rezoluție.
        Astfel, divizarea diferenței de timp în "ticks" la această frecvență transformă valoarea în secunde.


         Calculează delta_time, care reprezintă cantitatea de timp scursă între frame_begin_time și frame_end_time.
        frame_end_time.QuadPart și frame_begin_time.QuadPart sunt numere întregi pe 64 de biți,
        care prezintă un timp în "tick-uri", unde un "tick" este o unitate de timp definită de sistemul
        (de exemplu, fiecare "tick" poate reprezenta 100 de nano-secunde)
        Substracția frame_end_time.QuadPart - frame_begin_time.QuadPart:
         -> calculează diferența de timp dintre începutul și sfârșitul cadrelor, în "ticks".

        Parte esențială a oricărui buclă principală
        a jocului și este crucială pentru asigurarea unei rate stabile de actualizare a cadrului.

        delta_time este adesea folosit în jocuri și alte aplicații în timp real pentru a se asigura că mișcarea și animațiile sunt fluide.
        De exemplu, dacă doriți ca un personaj să se deplaseze cu o viteză constantă indiferent de rata de împrospătare a cadrului, ați putea scala viteza personajului cu delta_time.

        n final, rezultatul este convertit în float utilizând casting-ul explicit (float)
        ->transformând numărul întreg într-un număr cu virgulă mobilă.
        Acest lucru este util în cazul în care doriți să efectuați ulterior operațiuni cu virgulă mobilă cu delta_time. */

    }
}
