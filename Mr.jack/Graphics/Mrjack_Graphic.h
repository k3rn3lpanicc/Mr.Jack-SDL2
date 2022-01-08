#ifndef MRJACK_GRAPHIC_H_INCLUDED
#define MRJACK_GRAPHIC_H_INCLUDED
#include <stdbool.h>
typedef struct _pos{
int x;
int y;
} _pos;
typedef struct _pair{
int first;
int second;
}_pair;
typedef struct Map_Cell{
    char *cell_type;
    char *person_name;
    _pair cell_pos;
    bool lighten;
    bool walk_able;
    bool choose_able;
}Map_Cell;
typedef struct _clickable{
    char *tag;
    bool click_able;
    SDL_Surface *image;
    SDL_Rect pos;
    bool entered;
    bool do_chick;
    bool visible;
    Map_Cell *cell_info;
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

char *TITLE_TEXT = "Mr.Jack FP project Written by k3rn3lPanic with SDL2 in C :P";

char *fonts_segoeui = "segoeui.ttf";
char *fonts_consolas = "consolas.ttf";

char *sounds_button = "Sounds\\button.wav";
char *sounds_button2 = "Sounds\\button2.wav";
char *sounds_button3 = "Sounds\\button3.wav";
///Buttons here
char *btn_play = "Images\\btn_play.png";
char *btn_exit = "Images\\btn_exit.png";
char *btn_resume = "Images\\btn_resume.png";
char *btn_back = "Images\\btn_back.png";
char *btn_about = "Images\\btn_about.png";
char *border_pic = "Images\\border.png";
char *map_pic = "Images\\map.png";

///----end----


char *In_Lestrade = "Images\\INNOCENT_INSP_LESTRADE.bmp";
char *In_Jermy = "Images\\INNOCENT_JERMY_BERT.bmp";
char *In_John_Smith = "Images\\INNOCENT_JOHN_SMITH.bmp";
char *In_John_Watson = "Images\\INNOCENT_JOHN_WATSON.bmp";
char *In_Miss_Stealthy = "Images\\INNOCENT_MISS_STEALTHY.bmp";
char *In_Goodley = "Images\\INNOCENT_SERGENT_GOODLEY.bmp";
char *In_Sherlock = "Images\\INNOCENT_SHERLOCK_HOLMES.bmp";
char *In_William_Gull = "Images\\INNOCENT_WILLIAM_GULL.bmp";


char *bg_pic = "Images\\back.jpg";
char *bg_pic2 = "Images\\back2.jpg";

char *mute_btn = "Images\\mute.png";
char *unmute_btn = "Images\\unmute.png";

char *point_pic = "Images\\point.png";
char *about_me = "Images\\about_me.png";
char *home = "Images\\home.png";
char *home_hover = "Images\\home_hover.png";

char *empty_cell = "Images\\Blocks\\empty_block.png";
char *walkable_block = "Images\\Blocks\\walkable_block.png";
char *light_effect = "Images\\Blocks\\light_effect.png";


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
void Stop_Mixer();
void Resume_Mixer();
SDL_Surface *Create_Label(Label label);
Drawable *Create_node(_clickable *_obj);
void Add_obj(Drawable *head , Drawable *obj_);
void DrawStuff(Drawable **head , SDL_Surface *window_surface , SDL_Surface *light_eff);
void clear_scene(Drawable *seek);
void change_scene(Drawable **head , Drawable *scene);
void Draw_map(Drawable *scene , int x , int y);
void Change_Block_pic(Drawable *head, int first , int second , char *new_image);
void Free_List(Drawable **head);
Drawable *Find_Cell(Drawable *head , int first , int second);
void light_cells(Drawable *head , int first , int second , int dir , bool on);
bool is_mouse_inside_cell(_pos mouse_pos , SDL_Rect cell_rect);

#endif // MRJACK_GRAPHIC_H_INCLUDED
