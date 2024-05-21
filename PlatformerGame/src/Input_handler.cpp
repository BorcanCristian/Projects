struct Button_State
{
 bool is_down;
 bool changed;
};

enum
{
 BUTTON_UP,
 BUTTON_DOWN,
 BUTTON_W,  // <-BUTTON_LEFT
 BUTTON_S, // <- BUTTON_RIGHT astea au fost inainte
 BUTTON_LEFT,
 BUTTON_RIGHT,
 BUTTON_ENTER, // sA NU UITI SA ADAUGI SI IN WIN32 LA CASE VK swich (vk_Code)

 BUTTON_COUNT, // Ar trebui sa fie ultimul item
};

struct Input
{
 Button_State buttons[BUTTON_COUNT];
};

// NE DUCEM IN WIN32 SA IMPLEMENTEM LOGICA la while running