#ifndef MRJACK_GRAPHIC_H_INCLUDED
#define MRJACK_GRAPHIC_H_INCLUDED
#include <stdbool.h>
char *TITLE_TEXT = "Mr.Jack FP project Written by k3rn3lPanic with SDL2 in C :P";

char *fonts_segoeui = "segoeui.ttf";
char *fonts_consolas = "consolas.ttf";

char *sounds_button = "Sounds\\button.wav";
char *sounds_button2 = "Sounds\\button2.wav";
char *sounds_button3 = "Sounds\\button3.wav";

char *button_1 = "Images\\button_1.bmp";
char *button_2 = "Images\\button_2.bmp";

char *In_Lestrade = "Images\\INNOCENT_INSP_LESTRADE.bmp";
char *In_Jermy = "Images\\INNOCENT_JERMY_BERT.bmp";
char *In_John_Smith = "Images\\INNOCENT_JOHN_SMITH.bmp";
char *In_John_Watson = "Images\\INNOCENT_JOHN_WATSON.bmp";
char *In_Miss_Stealthy = "Images\\INNOCENT_MISS_STEALTHY.bmp";
char *In_Goodley = "Images\\INNOCENT_SERGENT_GOODLEY.bmp";
char *In_Sherlock = "Images\\INNOCENT_SHERLOCK_HOLMES.bmp";
char *In_William_Gull = "Images\\INNOCENT_WILLIAM_GULL.bmp";


char *point_pic = "Images\\point.bmp";

typedef struct _pos{
int x;
int y;
} _pos;
typedef struct _clickable{
    char *tag;
    bool click_able;
    SDL_Surface *image;
    SDL_Rect pos;
    bool entered;
    bool do_chick;
    bool visible;
} _clickable;

typedef struct Label{
    char *text;
    char *font_path;
    int _font_size;
    SDL_Color back_color;
    SDL_Color fore_color;
    SDL_Rect text_location;
    SDL_Surface *image;
}Label;

typedef struct Drawable{
    _clickable *obj;
    struct Drawable *next;
    struct Drawable *prev;
} Drawable;


SDL_Surface *load_image(char *path);
void Play_voice(void *_voice_path);
void Play_voice_Thread(char *_voice_path);

void Init();
void draw(SDL_Window *window);
void clear_surface(SDL_Surface *window_surface);
void error_and_exit(char *text);
char const *convert_button_number_to_string(int button);
bool is_mouse_inside_Surface(_pos mouse_pos , SDL_Rect Rect);
bool chick(_pos mouse_pos , Drawable *obj);

SDL_Surface *Create_Label(Label label);
Drawable *Create_node(_clickable *_obj);
void Add_obj(Drawable *head , Drawable *obj_);
void DrawStuff(Drawable **head , SDL_Surface *window_surface);

#endif // MRJACK_GRAPHIC_H_INCLUDED
