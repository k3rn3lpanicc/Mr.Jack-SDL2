#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include<stdio.h>
#include<stdbool.h>
#include "SDL_image.h"
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
}Label;
typedef struct Drawable{
    _clickable *obj;
    struct Drawable *next;
    struct Drawable *prev;
} Drawable;
SDL_Surface *load_image(char const *path)
{
    SDL_Surface *image_surface = IMG_Load(path);

    if(!image_surface)
        return 0;
    return image_surface;
}
void Play_voice(void *_voice_path){
    char *voice_path = _voice_path;
    Mix_Chunk *sound_effect = Mix_LoadWAV(_voice_path);
    Mix_PlayChannel(-1 , sound_effect , 0);
    //Mix_HaltChannel(1);
    //Mix_FreeChunk(sound_effect);
}

void Play_voice_Thread(char *_voice_path){
    Play_voice(_voice_path);
}

void Init(){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        exit(-1);
    }
    if(TTF_Init()<0){
        error_and_exit("TTF_not started");
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT , 2 , 2048) < 0){
        error_and_exit("Error Loading Sound Mixer");
    }
    if(IMG_Init(IMG_INIT_PNG)<0){
        error_and_exit("sdl_png");
    }
}
void draw(SDL_Window *window){
    SDL_UpdateWindowSurface(window);
    SDL_Delay(10);
}
void clear_surface(SDL_Surface *window_surface){
    SDL_FillRect(window_surface, NULL, SDL_MapRGB(window_surface->format, 0, 0, 0));
}
void error_and_exit(char *text){
    printf(text);
    exit(-1);
}
char const *convert_button_number_to_string(int button)
{
    char const *result = "NO_BUTTON";
    switch (button)
    {
        case SDL_BUTTON_LEFT:
        result = "left";
        break;

        case SDL_BUTTON_MIDDLE:
        result = "middle";
        break;

        case SDL_BUTTON_RIGHT:
        result = "right";
        break;

        default:
        break;
    }

    return result;
}
bool is_mouse_inside_Surface(_pos mouse_pos , SDL_Rect Rect){
    if(mouse_pos.x >= Rect.x && mouse_pos.x<=Rect.x + Rect.w && mouse_pos.y >= Rect.y && mouse_pos.y<=Rect.y + Rect.h)
        return true;
    return false;
}
bool chick(_pos mouse_pos , Drawable *obj){
    bool inside = is_mouse_inside_Surface(mouse_pos , obj->obj->pos);
    if((!(obj->obj->entered)) && inside){
        obj->obj->entered = true;
        if(obj->obj->do_chick){
            return true;
        }

    }
    else if((obj->obj->entered) && (!inside)){
        obj->obj->entered = false;
    }
    return false;
}
SDL_Surface *Create_Label(Label label){
    TTF_Font* font = TTF_OpenFont(label.font_path, label._font_size);
    SDL_Surface *textSurface = TTF_RenderText_Shaded(font, label.text, label.fore_color, label.back_color);
    return textSurface;
}


Drawable *Create_node(_clickable *_obj){
    Drawable *new_node = (Drawable *)malloc(sizeof(Drawable));
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->obj = _obj;
    return new_node;
}
void DrawStuff(Drawable **head , SDL_Surface *window_surface){
    Drawable *seek = *head;
    while(seek){
        if(seek->obj->visible)
            SDL_BlitSurface(seek->obj->image, NULL, window_surface, &(seek->obj->pos));
        seek = seek->next;
    }
}
void Add_obj(Drawable *head , Drawable *obj_){
    Drawable *seek = head;
    while(seek->next)
        seek = seek->next;
    seek->next = obj_;
    obj_->prev = seek;
    obj_->next = NULL;
}
void Stop_Mixer(){
    Mix_PauseMusic();
}
void Resume_Mixer(){
    Mix_ResumeMusic();
}
