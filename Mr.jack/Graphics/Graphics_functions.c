#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "SDL_image.h"
//map_blocks

char *Cells_Empty_block = "Images\\Blocks\\empty_block.png";
char *Cells_house_block = "Images\\Blocks\\house.png";
char *Cells_house2_block = "Images\\Blocks\\house_2.png";
char *Cells_light = "Images\\Blocks\\light.png";
char *Cells_lighten_block = "Images\\Blocks\\light_block.png";
char *Cells_pit = "Images\\Blocks\\pit.png";
char *Cells_pit_lighten = "Images\\Blocks\\pit_light.png";
char *Cells_Walkable = "Images\\Blocks\\walkable_block.png";
char *Cells_light_effect = "Images\\Blocks\\light_effect.png";

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
    _pair map_pos;
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
    Mix_Chunk *sound_effect = Mix_LoadWAV(_voice_path);
    Mix_PlayChannel(-1 , sound_effect , 0);
    //Mix_HaltChannel(1);
    //Mix_FreeChunk(sound_effect);
}

void Play_voice_Thread(char *_voice_path){
    Play_voice(_voice_path);
}
void error_and_exit(char *text){
    printf(text);
    exit(-1);
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
bool is_mouse_inside_cell(_pos mouse_pos , SDL_Rect cell_rect){
    int mx , my;
    mx = mouse_pos.x;
    my = mouse_pos.y;
    int posx, posy;
    posx = cell_rect.x;
    posy = cell_rect.y;
    SDL_Rect mn;
    mn.x = posx+25;
    mn.w = 50;
    mn.y = posy;
    mn.h = 84;
    SDL_Rect mn2;
    mn2.x = posx+13;
    mn2.w = 73;
    mn2.y = posy+22;
    mn2.h = 41;
    if(is_mouse_inside_Surface(mouse_pos , mn) || is_mouse_inside_Surface(mouse_pos , mn2)){
        return true;
    }
    else{
        ///TODO:We need some calcs for eentire hexagon here
        return false;
    }

}
bool chick(_pos mouse_pos , Drawable *obj){

    bool inside;
    if(strcmp(obj->obj->tag,"MapCell")==0)
        inside = is_mouse_inside_cell(mouse_pos , obj->obj->pos);
    else
        inside = is_mouse_inside_Surface(mouse_pos , obj->obj->pos);
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
    //new_node->obj->cell_info = NULL;
    return new_node;
}
void DrawStuff(Drawable **head , SDL_Surface *window_surface , SDL_Surface *light_eff){
    Drawable *seek = *head;
    while(seek){
        if(seek->obj->visible){
            if(seek->obj->entered && seek->obj->do_chick){
                SDL_Rect rect;
                rect.x= seek->obj->pos.x+3;
                rect.y= seek->obj->pos.y-2;
                SDL_BlitSurface(seek->obj->image, NULL, window_surface, &rect);
                if(strcmp(seek->obj->tag, "MapCell")==0&&seek->obj->cell_info->lighten)
                    SDL_BlitSurface(light_eff, NULL, window_surface, &rect);
            }
            else{
                SDL_BlitSurface(seek->obj->image, NULL, window_surface, &(seek->obj->pos));
                if(strcmp(seek->obj->tag, "MapCell")==0&&seek->obj->cell_info->lighten)
                    SDL_BlitSurface(light_eff, NULL, window_surface, &(seek->obj->pos));
            }
        }
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
void clear_scene(Drawable *seek){
    while(seek){
        seek->obj->entered=false;
        seek = seek->next;
    }
}
void change_scene(Drawable **head , Drawable *scene){
    clear_scene(*head);
    *head = scene;
}
Drawable *Find_Cell(Drawable *head , int first , int second){
    while(head){
        if(head->obj->cell_info)
            if(head->obj->cell_info->cell_pos.first == first && head->obj->cell_info->cell_pos.second == second){
                return head;
            }
        head = head->next;
    }
    return NULL;
}
void Draw_map(Drawable *scene , int x , int y){
    for(int i = 0; i < 9 ; i++){ //arz
        for(int j = 0; j <13 ; j++ ){ //tool
            _clickable *Cell = (_clickable*)malloc(sizeof(_clickable));
            Cell->tag = "MapCell";
            Cell->click_able = true;
            Cell->entered = false;
            Cell->do_chick = true;
            Cell->visible = true;
            Map_Cell *cell_info = (Map_Cell*)malloc(sizeof(Map_Cell));

            cell_info->cell_type = "empty";
            cell_info->choose_able = true;
            cell_info->lighten = false;
            cell_info->person_name = "";
            cell_info->walk_able = true;
            Cell->cell_info = cell_info;
            Cell->pos.x = x+j*75;
            if(j%2 == 0){
            Cell->pos.y = y+i*87;
            Cell->cell_info->cell_pos.second = i*2;
            }
            else{
                Cell->pos.y = y+i*87 + 44;
                Cell->cell_info->cell_pos.second = 2*i+1;
            }
            Cell->cell_info->cell_pos.first = j;
            Cell->image = load_image(Cells_Empty_block);
            Add_obj(scene , Create_node(Cell));
        }
    }
    FILE *file = fopen("Graphics\\Map.jack","r");
    if(file == NULL){
        printf("can not load file!");
        return;
    }
    char data[50];
    while(fgets(data,49 , file)){
        int first , second;
        char tp[6];
        sscanf(data,"%d %d %s" , &first , &second , tp);
        if((strcmp(tp , "H1") == 0) || strcmp(tp , "H2") == 0){
            Drawable *cell = Find_Cell(scene , first , second);
            cell->obj->click_able = false;
            cell->obj->do_chick = false;
            cell->obj->cell_info->cell_type = "house";
            cell->obj->cell_info->choose_able = false;
            SDL_FreeSurface(cell->obj->image);
            if((strcmp(tp , "H1") == 0))
                cell->obj->image = load_image("Images\\Blocks\\house.png");
            else
                cell->obj->image = load_image("Images\\Blocks\\house_2.png");
        }
        else if((strcmp(tp , "PI") == 0)){
            Drawable *cell = Find_Cell(scene , first , second);
            cell->obj->click_able = true;
            cell->obj->do_chick = false;
            cell->obj->cell_info->cell_type = "pit";
            cell->obj->cell_info->choose_able = false;
            SDL_FreeSurface(cell->obj->image);
            cell->obj->image = load_image("Images\\Blocks\\pit.png");
        }
        else if((strcmp(tp , "LI") == 0)){
            Drawable *cell = Find_Cell(scene , first , second);
            cell->obj->click_able = true;
            cell->obj->do_chick = false;
            cell->obj->cell_info->cell_type = "Light";
            cell->obj->cell_info->choose_able = false;
            SDL_FreeSurface(cell->obj->image);
            cell->obj->image = load_image("Images\\Blocks\\light_empt.png");
        }
        else if((strcmp(tp , "Ex") == 0)){
            Drawable *cell = Find_Cell(scene , first , second);
            cell->obj->click_able = true;
            cell->obj->do_chick = false;
            cell->obj->cell_info->cell_type = "town_exit";
            cell->obj->cell_info->choose_able = false;
            SDL_FreeSurface(cell->obj->image);
            cell->obj->image = load_image("Images\\Blocks\\exit.png");
        }
        else if((strcmp(tp , "E") == 0)){
            Drawable *cell = Find_Cell(scene , first , second);
            cell->obj->click_able = true;
            cell->obj->do_chick = false;
            cell->obj->cell_info->cell_type = "EE";
            cell->obj->cell_info->choose_able = false;
            SDL_FreeSurface(cell->obj->image);
            cell->obj->image = load_image("Images\\Blocks\\exit.png");
        }
    }
    fclose(file);

}

void Change_Block_pic(Drawable *head, int first , int second , char *new_image){
    while(head){
        if(strcmp(head->obj->tag,"MapCell") == 0){
            if(head->obj->cell_info->cell_pos.first == first && head->obj->cell_info->cell_pos.second == second){
                SDL_FreeSurface(head->obj->image);
                head->obj->image = load_image(new_image);
                return;
            }
        }
        head = head->next;
    }
}



void Free_List(Drawable **head){
    Drawable *seek = *head;
    if(seek->next==NULL){
        SDL_FreeSurface(seek->obj->image);
        free(*head);
        return;
    }
    while(seek->next){
        seek = seek->next;
        SDL_FreeSurface(seek->prev->obj->image);
        free(seek->prev);
    }
    SDL_FreeSurface(seek->obj->image);
    free(seek);
    return;
}
void light_cells_dir(Drawable *head , int first , int second , int dir , bool on){
    int dx, dy;
    switch(dir){
    case 0:
        dx = 0;
        dy = -2;
        break;
    case 1:
        dx = 1;
        dy = -1;
        break;
    case 2:
        dx = 1;
        dy = 1;
        break;
    case 3:
        dx = 0;
        dy = 2;
        break;
    case 4:
        dx = -1;
        dy = +1;
        break;
    case 5:
        dx = -1;
        dy = -1;
        break;
    }
    first+=dx;
    second+=dy;
    while(first>=0 && first<=12 && second>=0 && second<=17 && strcmp(Find_Cell(head, first , second)->obj->cell_info->cell_type , "empty")==0){
        Drawable *cell = Find_Cell(head,first,second);
        if(cell){
            cell->obj->cell_info->lighten = on;
        }
        first+=dx;
        second+=dy;
    }
    return;
}
void Light_Cell(Drawable *head , int first , int second , bool on){
    Drawable *found = Find_Cell(head, first , second);
    if(found)
        found->obj->cell_info->lighten = on;
}
_pair loc(Drawable *head ,int x , int y){
    while(head){
        if(strcmp(head->obj->tag , "MapCell") == 0){
            if(head->obj->pos.x == x && head->obj->pos.y == y){
                _pair lol = {.first = head->obj->cell_info->cell_pos.first , .second = head->obj->cell_info->cell_pos.second};
                return lol;
            }
        }
        head = head->next;
    }
}
void Map_Lights(Drawable *head){
    Drawable *seek = head;
    while(seek){
        if(seek->obj->tag[0] == 'J' && seek->obj->tag[1] == 'W'){
            int dir = seek->obj->tag[2] - '0';
            _pair place = loc(head , seek->obj->pos.x-18 , seek->obj->pos.y-13);
            light_cells_dir(head, place.first, place.second, dir , true);
        }
        if(seek->obj->tag[0] == 'L' && seek->obj->tag[1] == 'I'){
            _pair place = loc(head , seek->obj->pos.x , seek->obj->pos.y);
            int first = place.first;
            int seco = place.second;
            Light_Cell(head , first , seco,true);
            Light_Cell(head , first , seco-2,true);
            Light_Cell(head , first , seco+2,true);
            Light_Cell(head , first+1 , seco-1,true);
            Light_Cell(head , first-1 , seco-1,true);
            Light_Cell(head , first+1 , seco+1,true);
            Light_Cell(head , first-1 , seco+1,true);
        }
        seek = seek->next;
    }

}

