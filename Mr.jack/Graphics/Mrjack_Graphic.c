#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "SDL_image.h"
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
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
int G_JW = 0;
int G_JS = 0;
int G_JB = 0;
int G_MS = 0;
int G_IL = 0;
int  G_SH = 0;
int G_WG = 0;
int G_SG = 0;

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
    int is_walled;
}Map_Cell;
typedef struct _clickable{
    char *tag;
    bool click_able;
    SDL_Surface *image;
    SDL_Rect pos;
    bool entered;
    bool do_chick;
    bool visible;
    int direction;
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
typedef struct Card{
    char *name;
    struct Card *next;
    struct Card *prev;
}Card;
bool StartsWith(const char *a, const char *b)
{
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}
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
bool new_move = false;
void set_new_move(bool a){
    new_move = a;
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
void DrawStuff(Drawable **head , SDL_Surface *window_surface , SDL_Surface *light_eff , SDL_Surface *walk_able){
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
                if(strcmp(seek->obj->tag, "MapCell")==0&&seek->obj->cell_info->walk_able)
                    SDL_BlitSurface(walk_able, NULL, window_surface, &rect);
            }
            else{
                SDL_BlitSurface(seek->obj->image, NULL, window_surface, &(seek->obj->pos));
                if(strcmp(seek->obj->tag, "MapCell")==0&&seek->obj->cell_info->lighten)
                    SDL_BlitSurface(light_eff, NULL, window_surface, &(seek->obj->pos));
                if(strcmp(seek->obj->tag, "MapCell")==0&&seek->obj->cell_info->walk_able)
                    SDL_BlitSurface(walk_able, NULL, window_surface, &(seek->obj->pos));
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
            cell_info->walk_able = false;
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
            if((cell->obj->cell_info->cell_pos.first==0&&cell->obj->cell_info->cell_pos.second==2)||(cell->obj->cell_info->cell_pos.first==12&&cell->obj->cell_info->cell_pos.second==16))
                cell->obj->image = load_image("Images\\Blocks\\exit2.png");
            else
                cell->obj->image = load_image("Images\\Blocks\\exit1.png");

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
    while((first>=0 && first<=12 && second>=0 && second<=17) && (strcmp(Find_Cell(head, first , second)->obj->cell_info->cell_type , "empty")==0 || strcmp(Find_Cell(head, first , second)->obj->cell_info->cell_type , "pit")==0||strcmp(Find_Cell(head, first , second)->obj->cell_info->cell_type , "Light")==0)){
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
    _pair nn = {-1 , -1};
    return nn;
}
Drawable *loc_lamp(Drawable *head , int x , int y){
    while(head){
        if(StartsWith(head->obj->tag,"LI")){
            if(head->obj->pos.x == x && head->obj->pos.y == y){
                return head;
            }
        }
        head = head->next;
    }
    return NULL;
}
Drawable *loc_pit(Drawable *head , int x , int y){
    while(head){
        if(StartsWith(head->obj->tag,"pit_hold")){
            if(head->obj->pos.x == x && head->obj->pos.y == y){
                return head;
            }
        }
        head = head->next;
    }
    return NULL;
}
void Map_Lights(Drawable *head){
    Drawable *seek = head;
    while(seek){
        if(StartsWith(seek->obj->tag , "MapCell"))
            Light_Cell(head , seek->obj->cell_info->cell_pos.first , seek->obj->cell_info->cell_pos.second,false);
        seek = seek->next;
    }
    seek = head;
    while(seek){
        if(seek->obj->tag[0] == 'J' && seek->obj->tag[1] == 'W'){
            int dir = seek->obj->direction;
            _pair place = loc(head , seek->obj->pos.x-18 , seek->obj->pos.y-13);
            light_cells_dir(head, place.first, place.second, dir , true);
        }
        if(seek->obj->tag[0] == 'L' && seek->obj->tag[1] == 'I'){
            _pair place = loc(head , seek->obj->pos.x , seek->obj->pos.y);
            if(place.first !=-1){
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
        }
        seek = seek->next;
    }

}
void Change_visibility(Drawable *head , char *tag, bool on){
    Drawable *seek = head;
    while(seek){
        if(strcmp(seek->obj->tag , tag) == 0){
            seek->obj->visible = on;
            return;
        }
        seek = seek->next;
    }
}
_pos get_location_animation(double time , _pos mabda , _pos maghsad){
    _pos location;
    location.x = (double)mabda.x+((double)(maghsad.x-mabda.x)/50)*time;
    location.y = (double)mabda.y+((double)(maghsad.y-mabda.y)/50)*time;
    return location;
}
bool switch_lamps(_pair lamp1 , _pair lamp2,Drawable *head,SDL_Surface *window_surface , SDL_Surface *light_eff,SDL_Surface *walkable_effect,SDL_Window *window){
    Drawable *lamp_1 = Find_Cell(head , lamp1.first, lamp1.second);
    Drawable *lamp_2 = Find_Cell(head , lamp2.first , lamp2.second);
    if(lamp_1 && lamp_2){
        if(strcmp(lamp_2->obj->tag , "MapCell")==0 && strcmp(lamp_2->obj->cell_info->cell_type,"Light")==0){
            if(strcmp(lamp_1->obj->tag,"MapCell")==0 && strcmp(lamp_1->obj->cell_info->cell_type,"Light")==0){
                Drawable *llmp = loc_lamp(head , lamp_1->obj->pos.x , lamp_1->obj->pos.y);
                Drawable *llmp2 = loc_lamp(head , lamp_2->obj->pos.x , lamp_2->obj->pos.y);
                if(llmp2){
                    return false;
                }
                if(!llmp){
                    return false;
                }
                _pos mabda = {llmp->obj->pos.x, llmp->obj->pos.y};
                _pos maghsad = {lamp_2->obj->pos.x, lamp_2->obj->pos.y};
                for(int i = 0 ; i<=50 ; i++){
                    _pos get_lc = get_location_animation(i , mabda , maghsad);
                    llmp->obj->pos.x = get_lc.x;
                    llmp->obj->pos.y = get_lc.y;
                    DrawStuff(&head , window_surface , light_eff,walkable_effect);
                    draw(window);
                    clear_surface(window_surface);
                }
                llmp->obj->pos.x = lamp_2->obj->pos.x;
                llmp->obj->pos.y = lamp_2->obj->pos.y;
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

bool switch_pits(_pair pit1 , _pair pit2,Drawable *head,SDL_Surface *window_surface , SDL_Surface *light_eff,SDL_Surface *walkable_effect,SDL_Window *window){
    Drawable *pit_1 = Find_Cell(head , pit1.first, pit1.second);
    Drawable *pit_2 = Find_Cell(head , pit2.first , pit2.second);
    if(pit_1 && pit_2){
        if(strcmp(pit_2->obj->tag , "MapCell")==0 && strcmp(pit_2->obj->cell_info->cell_type,"pit")==0){
            if(strcmp(pit_1->obj->tag , "MapCell")==0 &&strcmp(pit_1->obj->cell_info->cell_type,"pit")==0){
                Drawable *llmp = loc_pit(head , pit_1->obj->pos.x , pit_1->obj->pos.y);
                Drawable *llmp2 = loc_pit(head , pit_2->obj->pos.x , pit_2->obj->pos.y);
                if(llmp2){
                    return false;
                }
                if(!llmp){
                    return false;
                }
                _pos mabda = {llmp->obj->pos.x, llmp->obj->pos.y};
                _pos maghsad = {pit_2->obj->pos.x, pit_2->obj->pos.y};
                for(int i = 0 ; i<=50 ; i++){
                    _pos get_lc = get_location_animation(i , mabda , maghsad);
                    llmp->obj->pos.x = get_lc.x;
                    llmp->obj->pos.y = get_lc.y;
                    DrawStuff(&head , window_surface , light_eff,walkable_effect);
                    draw(window);
                    clear_surface(window_surface);
                }
                llmp->obj->pos.x = pit_2->obj->pos.x;
                llmp->obj->pos.y = pit_2->obj->pos.y;
                return true;
            }
            else{
               return false;
            }
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}
Drawable *Search_by_tag(Drawable *head , char *tag){
    while(head){
        if(strcmp(head->obj->tag,tag)==0)
            return head;
        head = head->next;
    }
    return NULL;
}
Drawable *Search_by_tag_2(Drawable *head , char *tag){
    while(head){
        if(StartsWith(head->obj->tag,tag))
            return head;
        head = head->next;
    }
    return NULL;
}
bool is_cell_full(Drawable *head , int x , int y){
    while(head){
        if(strlen(head->obj->tag)==2 && x == head->obj->pos.x && y==head->obj->pos.y){
            return true;
        }
        head = head->next;
    }
    return false;
}
bool switch_characters(Drawable *head , char *name1 , char *name2 , SDL_Surface *window_surface , SDL_Surface *light_eff,SDL_Surface *walkable_effect,SDL_Window *window){
    Drawable *Player_1 = Search_by_tag(head , name1);
    Drawable *Player_2 = Search_by_tag(head , name2);
    if(!(Player_1 && Player_2)){
        return false;
    }
    _pos P1 = {Player_1->obj->pos.x , Player_1->obj->pos.y};
    _pos P2 = {Player_2->obj->pos.x , Player_2->obj->pos.y};
    for(int i = 0 ; i<=50 ; i++){
        _pos get_lc = get_location_animation(i , P1 , P2);
        _pos get_lc2 = get_location_animation(i , P2 , P1);
        Player_1->obj->pos.x = get_lc.x;
        Player_1->obj->pos.y = get_lc.y;
        Player_2->obj->pos.x = get_lc2.x;
        Player_2->obj->pos.y = get_lc2.y;
        DrawStuff(&head , window_surface , light_eff,walkable_effect);
        draw(window);
        clear_surface(window_surface);
    }
    Player_1->obj->pos.x = P2.x;
    Player_1->obj->pos.y = P2.y;
    Player_2->obj->pos.x = P1.x;
    Player_2->obj->pos.y = P1.y;
    return true;
}
bool change_character_place(Drawable *head ,char *name , _pair dest,SDL_Surface *window_surface , SDL_Surface *light_eff,SDL_Surface *walkable_effect,SDL_Window *window){
    Drawable *found = Search_by_tag_2(head , name);
    if(found){
        Drawable *cell = Find_Cell(head ,dest.first,dest.second);
        if(!cell){
            return false;
        }
        _pos mabda = {found->obj->pos.x, found->obj->pos.y};
        _pos maghsad = {cell->obj->pos.x+18, cell->obj->pos.y+13};
        if(mabda.x==maghsad.x && mabda.y==maghsad.y){
            return false;
        }
        if(is_cell_full(head,maghsad.x,maghsad.y)){
            return false;
        }
        for(int i = 0 ; i<=50 ; i++){
            _pos get_lc = get_location_animation(i , mabda , maghsad);
            found->obj->pos.x = get_lc.x;
            found->obj->pos.y = get_lc.y;
            DrawStuff(&head , window_surface , light_eff,walkable_effect);
            draw(window);
            clear_surface(window_surface);
        }
        found->obj->pos.x = cell->obj->pos.x+18;
        found->obj->pos.y = cell->obj->pos.y+13;
        return true;
    }
    else{
        return false;
    }

}

void change_watson_direction(Drawable *head , int _direction){
    Drawable *found = Search_by_tag_2(head , "JW");
    if(found){
        SDL_FreeSurface(found->obj->image);
        char path[100];
        sprintf(path , "Images\\Characters\\watson_%d.png" , _direction);
        found->obj->image = load_image(path);
        found->obj->direction = _direction;
        return;
    }
    else{
        return;
    }
}
bool change_wall(Drawable *head , char *name , _pair dest,SDL_Surface *window_surface , SDL_Surface *light_eff,SDL_Surface *walkable_effect,SDL_Window *window){
    Drawable *cell = Search_by_tag(head , name);
    char *next_wall = strcmp(name , "wall1")==0 ? "wall2" : "wall1";
    Drawable *next_wall_cell = Search_by_tag(head , next_wall);
    if(!next_wall_cell){
        return false;
    }
    Drawable *cell_dest = Find_Cell(head , dest.first , dest.second);
    if(!cell){
        return false;
    }
    if(cell_dest->obj->pos.x == next_wall_cell->obj->pos.x && cell_dest->obj->pos.y == next_wall_cell->obj->pos.y){
        return false;
    }
    if(cell->obj->pos.x==cell_dest->obj->pos.x&&cell->obj->pos.y==cell_dest->obj->pos.y){
        return false;
    }
    if(!cell_dest){
         return false;
    }
    if(strcmp(cell_dest->obj->tag , "MapCell") == 0 && strcmp(cell_dest->obj->cell_info->cell_type,"town_exit") == 0){
        Drawable *CC = Find_Cell(head ,dest.first , dest.second);
        if(!CC){
            return false;
        }
        _pos mabda = {cell->obj->pos.x, cell->obj->pos.y};
        _pos maghsad = {CC->obj->pos.x, CC->obj->pos.y};
        for(int i = 0 ; i<=50 ; i++){
            _pos get_lc = get_location_animation(i , mabda , maghsad);
            cell->obj->pos.x = get_lc.x;
            cell->obj->pos.y = get_lc.y;
            DrawStuff(&head , window_surface , light_eff,walkable_effect);
            draw(window);
            clear_surface(window_surface);
        }
        cell->obj->pos.x = CC->obj->pos.x;
        cell->obj->pos.y = CC->obj->pos.y;
        return true;
    }
    else{
        return false;
    }

}

char *Choose_random_character(){
    char *charctrs[8] = {"JW" , "JS" , "MS" , "WG" , "SH" , "IL" , "SG" , "JB"};
    srand(time(0));
    return charctrs[rand()%8];
}
char *get_clicked_character(Drawable *head , int first , int second){
    Drawable *cell = Find_Cell(head , first , second);
    if(!cell){
        return "";
    }
    while(head){
        if(strlen(head->obj->tag) == 2 && head->obj->pos.x == cell->obj->pos.x+18 && head->obj->pos.y == cell->obj->pos.y+13){
            return head->obj->tag;
        }
        head = head->next;
    }
    return "";
}
Card *Create_card(char *name){
    Card *new_node = (Card*)malloc(sizeof(Card));
    new_node->name = name;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}
void append_card(Card *head,char *name){
    while(head->next)
        head = head->next;
    Card *new_card = Create_card(name);
    new_card->prev = head;
    head->next = new_card;
}
void remove_card(Card **head , char *name){
    Card *seek = *head;
    if(strcmp(seek->name , name)==0){
        *head = (*head)->next;
        free(seek);
        return;
    }
    while(seek){
        if(strcmp(seek->name , name)==0){
            if(seek->next){
                Card *nex = seek->next;
                Card *prev = seek->prev;
                prev->next = nex;
                nex->prev = prev;
                free(seek);
            }
            else{
                seek->prev->next = NULL;
                free(seek);
            }
            return;
        }
        seek = seek->next;
    }
}
void print_cards(Card *head){
    while(head){
        printf("%s\n" , head->name);
        head = head->next;
    }
    printf("\n");
}
void add_all_characters(Card **head){
    *head = Create_card("JW");
    char *charctrs[8] = {"JW" , "JS" , "MS" , "WG" , "SH" , "IL" , "SG" , "JB"};
    for(int i = 1 ; i<8 ; i++)
        append_card(*head , charctrs[i]);
}
void change_information_label(Drawable *head ,_clickable *label , char *new_text , SDL_Surface *window_surface , SDL_Surface *light_eff,SDL_Surface *walkable_effect,SDL_Window *window,SDL_Color fg2){
    TTF_Font *font_consolas;
    font_consolas = TTF_OpenFont("ALGER.TTF" , 18);
    for(int i = 0; i<strlen(new_text);i++){
        char show[strlen(new_text)+1];
        for (int j = 0 ; j<=i ; j++){
            show[j] = new_text[j];
        }
        show[i+1] = '\0';
        SDL_Surface *new_surface = TTF_RenderText_Solid(font_consolas , show,fg2);
        SDL_FreeSurface(label->image);
        label->image = new_surface;
        DrawStuff(&head , window_surface , light_eff,walkable_effect);
        draw(window);
        clear_surface(window_surface);
    }
    TTF_CloseFont(font_consolas);
}
void Set_all_cells_unwalkable(Drawable *head){
    while(head){
        if(strcmp(head->obj->tag , "MapCell")==0){
            head->obj->cell_info->walk_able = false;
        }
        head = head->next;
    }
}
void set_walkable(Drawable *head , int first , int second){
    Drawable *found = Find_Cell(head , first , second);
    if(!found)
        return;
    found->obj->cell_info->walk_able = true;
}
bool is_pit_empty(Drawable *head , Drawable *cell){
    Drawable *pit_1 = Search_by_tag(head , "pit_hold1");
    Drawable *pit_2 = Search_by_tag(head , "pit_hold2");
    return !((cell->obj->pos.x == pit_1->obj->pos.x && cell->obj->pos.y == pit_1->obj->pos.y)||(cell->obj->pos.x == pit_2->obj->pos.x && cell->obj->pos.y == pit_2->obj->pos.y));
}
void calculate_walkable_cells(Drawable *head , int first , int second , int len){
    if(len==0)
        return;
    Drawable *N = Find_Cell(head , first , second);
    Drawable *N1 = Find_Cell(head , first , second-2);
    Drawable *N2 = Find_Cell(head , first , second+2);
    Drawable *N3 = Find_Cell(head , first-1 , second-1);
    Drawable *N4 = Find_Cell(head , first+1 , second-1);
    Drawable *N5 = Find_Cell(head , first-1 , second+1);
    Drawable *N6 = Find_Cell(head , first+1 , second+1);
    if(strcmp(N->obj->tag, "MapCell")== 0 && strcmp(N->obj->cell_info->cell_type, "pit")==0 && is_pit_empty(head, N)){
        Drawable *seek = head;
        while(seek){
            if(strcmp(seek->obj->tag , "MapCell")==0 && strcmp(seek->obj->cell_info->cell_type , "pit")==0 && is_pit_empty(head, seek)){
                seek->obj->cell_info->walk_able = true;
                calculate_walkable_cells(head , seek->obj->cell_info->cell_pos.first , seek->obj->cell_info->cell_pos.second , len-1);
            }
            seek = seek->next;
        }
    }
    if(N1)
    if(strcmp(N1->obj->cell_info->cell_type , "house")!=0 && strcmp(N1->obj->cell_info->cell_type , "Light")!=0){
        N1->obj->cell_info->walk_able = true;
        calculate_walkable_cells(head , first , second-2 , len-1);
    }
    if(N2)
    if(strcmp(N2->obj->cell_info->cell_type , "house")!=0 && strcmp(N2->obj->cell_info->cell_type , "Light")!=0){
        N2->obj->cell_info->walk_able = true;
        calculate_walkable_cells(head , first , second+2 , len-1);
    }
    if(N3)
    if(strcmp(N3->obj->cell_info->cell_type , "house")!=0 && strcmp(N3->obj->cell_info->cell_type , "Light")!=0){
        N3->obj->cell_info->walk_able = true;
        calculate_walkable_cells(head , first-1 , second-1 , len-1);
    }
    if(N4)
    if(strcmp(N4->obj->cell_info->cell_type , "house")!=0 && strcmp(N4->obj->cell_info->cell_type , "Light")!=0){
        N4->obj->cell_info->walk_able = true;
        calculate_walkable_cells(head , first+1 , second-1 , len-1);
    }
    if(N5)
    if(strcmp(N5->obj->cell_info->cell_type , "house")!=0 && strcmp(N5->obj->cell_info->cell_type , "Light")!=0){
        N5->obj->cell_info->walk_able = true;
        calculate_walkable_cells(head , first-1 , second+1 , len-1);
    }
    if(N6)
    if(strcmp(N6->obj->cell_info->cell_type , "house")!=0 && strcmp(N6->obj->cell_info->cell_type , "Light")!=0){
        N6->obj->cell_info->walk_able = true;
        calculate_walkable_cells(head , first+1 , second+1 , len-1);
    }
}
void calculate_walkable_cells_2(Drawable *head , int first , int second , int len){
    if(len==0)
        return;
    Drawable *N = Find_Cell(head , first , second);
    Drawable *N1 = Find_Cell(head , first , second-2);
    Drawable *N2 = Find_Cell(head , first , second+2);
    Drawable *N3 = Find_Cell(head , first-1 , second-1);
    Drawable *N4 = Find_Cell(head , first+1 , second-1);
    Drawable *N5 = Find_Cell(head , first-1 , second+1);
    Drawable *N6 = Find_Cell(head , first+1 , second+1);
    if(strcmp(N->obj->tag, "MapCell")== 0 && strcmp(N->obj->cell_info->cell_type, "pit")==0 && is_pit_empty(head, N)){
        Drawable *seek = head;
        while(seek){
            if(strcmp(seek->obj->tag , "MapCell")==0 && strcmp(seek->obj->cell_info->cell_type , "pit")==0 && is_pit_empty(head, seek)){
                seek->obj->cell_info->walk_able = false;;
                calculate_walkable_cells_2(head , seek->obj->cell_info->cell_pos.first , seek->obj->cell_info->cell_pos.second , len-1);
            }
            seek = seek->next;
        }
    }
    if(N1)
    if(strcmp(N1->obj->cell_info->cell_type , "house")!=0 && strcmp(N1->obj->cell_info->cell_type , "Light")!=0){
        N1->obj->cell_info->walk_able = false;
        calculate_walkable_cells_2(head , first , second-2 , len-1);
    }
    if(N2)
    if(strcmp(N2->obj->cell_info->cell_type , "house")!=0 && strcmp(N2->obj->cell_info->cell_type , "Light")!=0){
        N2->obj->cell_info->walk_able = false;
        calculate_walkable_cells_2(head , first , second+2 , len-1);
    }
    if(N3)
    if(strcmp(N3->obj->cell_info->cell_type , "house")!=0 && strcmp(N3->obj->cell_info->cell_type , "Light")!=0){
        N3->obj->cell_info->walk_able = false;
        calculate_walkable_cells_2(head , first-1 , second-1 , len-1);
    }
    if(N4)
    if(strcmp(N4->obj->cell_info->cell_type , "house")!=0 && strcmp(N4->obj->cell_info->cell_type , "Light")!=0){
        N4->obj->cell_info->walk_able = false;
        calculate_walkable_cells_2(head , first+1 , second-1 , len-1);
    }
    if(N5)
    if(strcmp(N5->obj->cell_info->cell_type , "house")!=0 && strcmp(N5->obj->cell_info->cell_type , "Light")!=0){
        N5->obj->cell_info->walk_able = false;
        calculate_walkable_cells_2(head , first-1 , second+1 , len-1);
    }
    if(N6)
    if(strcmp(N6->obj->cell_info->cell_type , "house")!=0 && strcmp(N6->obj->cell_info->cell_type , "Light")!=0){
        N6->obj->cell_info->walk_able = false;
        calculate_walkable_cells_2(head , first+1 , second+1 , len-1);
    }
}
void calculate_walkable_cells_MS(Drawable *head , int first , int second, int len){
    if(len==0)
        return;
    Drawable *N = Find_Cell(head , first , second);
    Drawable *N1 = Find_Cell(head , first , second-2);
    Drawable *N2 = Find_Cell(head , first , second+2);
    Drawable *N3 = Find_Cell(head , first-1 , second-1);
    Drawable *N4 = Find_Cell(head , first+1 , second-1);
    Drawable *N5 = Find_Cell(head , first-1 , second+1);
    Drawable *N6 = Find_Cell(head , first+1 , second+1);
    if(strcmp(N->obj->tag, "MapCell")== 0 && strcmp(N->obj->cell_info->cell_type, "pit")==0 && is_pit_empty(head, N)){
        Drawable *seek = head;
        while(seek){
            if(strcmp(seek->obj->tag , "MapCell")==0 && strcmp(seek->obj->cell_info->cell_type , "pit")==0 && is_pit_empty(head, seek)){
                seek->obj->cell_info->walk_able = true;
                calculate_walkable_cells_MS(head , seek->obj->cell_info->cell_pos.first , seek->obj->cell_info->cell_pos.second , len-1);
            }
            seek = seek->next;
        }
    }
    if(N1){
        N1->obj->cell_info->walk_able = true;
        calculate_walkable_cells_MS(head , first , second-2 , len-1);
    }
    if(N2){
        N2->obj->cell_info->walk_able = true;
        calculate_walkable_cells_MS(head , first , second+2 ,len-1);
    }
    if(N3){
        N3->obj->cell_info->walk_able = true;
        calculate_walkable_cells_MS(head , first-1 , second-1, len-1);
    }
    if(N4){
        N4->obj->cell_info->walk_able = true;
        calculate_walkable_cells_MS(head , first+1 , second-1 , len-1);
    }
    if(N5){
        N5->obj->cell_info->walk_able = true;
        calculate_walkable_cells_MS(head , first-1 , second+1 , len-1);
    }
    if(N6){
        N6->obj->cell_info->walk_able = true;
        calculate_walkable_cells_MS(head , first+1 , second+1  , len-1);
    }
}
void calculate_unwalkable_cells_MS(Drawable *head){
    while(head){
        if(strcmp(head->obj->tag , "MapCell")==0){
            if(strcmp(head->obj->cell_info->cell_type , "house")==0 || strcmp(head->obj->cell_info->cell_type , "Light")==0){
                head->obj->cell_info->walk_able = false;
            }
        }
        head = head->next;
    }
}
Drawable *cell_under_character(Drawable *head , char *char_name){
    Drawable *ch = Search_by_tag(head , char_name);
    while(head){
        if(strcmp(head->obj->tag , "MapCell")==0){
            if(head->obj->pos.x == ch->obj->pos.x-18&&head->obj->pos.y == ch->obj->pos.y-13){
                return head;
            }
        }
        head = head->next;
    }
    return NULL;
}
void load_card(Drawable *head , char *card_name){
    char *In_Lestrade = "Images\\INNOCENT_INSP_LESTRADE.png";
    char *In_Jermy = "Images\\INNOCENT_JERMY_BERT.png";
    char *In_John_Smith = "Images\\INNOCENT_JOHN_SMITH.png";
    char *In_John_Watson = "Images\\INNOCENT_JOHN_WATSON.png";
    char *In_Miss_Stealthy = "Images\\INNOCENT_MISS_STEALTHY.png";
    char *In_Goodley = "Images\\INNOCENT_SERGENT_GOODLEY.png";
    char *In_Sherlock = "Images\\INNOCENT_SHERLOCK_HOLMES.png";
    char *In_William_Gull = "Images\\INNOCENT_WILLIAM_GULL.png";
    Drawable *cell;
    if(strcmp(card_name , "JW")==0){
        cell = Search_by_tag(head , "Watson");
        SDL_FreeSurface(cell->obj->image);
        cell->obj->image = load_image(In_John_Watson);
        G_JW = 1;
    }
    if(strcmp(card_name , "JS")==0){
        cell = Search_by_tag(head , "Smith");
        SDL_FreeSurface(cell->obj->image);
        cell->obj->image = load_image(In_John_Smith);
        G_JS = 1;
    }
    if(strcmp(card_name , "JB")==0){
        cell = Search_by_tag(head , "Jermy");
        SDL_FreeSurface(cell->obj->image);
        cell->obj->image = load_image(In_Jermy);
        G_JB = 1;
    }
    if(strcmp(card_name , "MS")==0){
        cell = Search_by_tag(head , "Stealthy");
        SDL_FreeSurface(cell->obj->image);
        cell->obj->image = load_image(In_Miss_Stealthy);
        G_MS = 1;
    }
    if(strcmp(card_name , "SG")==0){
        cell = Search_by_tag(head , "Goodley");
        SDL_FreeSurface(cell->obj->image);
        cell->obj->image = load_image(In_Goodley);
        G_SG =1;
    }
    if(strcmp(card_name , "IL")==0){
        cell = Search_by_tag(head , "Lestrade");
        SDL_FreeSurface(cell->obj->image);
        cell->obj->image = load_image(In_Lestrade);
        G_IL = 1;
    }
    if(strcmp(card_name , "SH")==0){
        cell = Search_by_tag(head , "Sherlock");
        SDL_FreeSurface(cell->obj->image);
        cell->obj->image = load_image(In_Sherlock);
        G_SH = 1;
    }
    if(strcmp(card_name , "WG")==0){
        cell = Search_by_tag(head , "Gull");
        SDL_FreeSurface(cell->obj->image);
        cell->obj->image = load_image(In_William_Gull);
        G_WG = 1;
    }
}
void shuffle(void *array, int n, size_t size) {
    char tmp[size];
    char *arr = array;
    size_t stride = size * sizeof(char);
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; ++i) {
            size_t rnd = (size_t) rand();
            size_t j = i + rnd / (RAND_MAX / (n - i) + 1);
            memcpy(tmp, arr + j * stride, size);
            memcpy(arr + j * stride, arr + i * stride, size);
            memcpy(arr + i * stride, tmp, size);
        }
    }
}
void shuffle_cards(Card *head){
    int n = 0;
    Card *seek = head;
    while(seek){
        n++;
        seek = seek->next;
    }
    char *addr[n];
    int cnt = 0;
    seek = head;
    while(seek){
        addr[cnt] = seek->name;
        cnt++;
        seek = seek->next;
    }
    seek = head;
    shuffle(addr , n , sizeof(char*));
    for(int i = 0; i<n ; i++){
    seek->name = addr[i];
    seek = seek->next;
    }
}
void burn_card(Drawable *head , char *card_name){
    char *file_path = "Images\\Unknown_card_burned.png";
    if(strcmp(card_name , "JW")==0 && G_JW != 1){
        Search_by_tag(head , "Watson")->obj->image = load_image(file_path);
        G_JW = -1;
    }
    if(strcmp(card_name , "JS")==0 && G_JS != 1){
        Search_by_tag(head , "Smith")->obj->image = load_image(file_path);
        G_JS = -1;
    }
    if(strcmp(card_name , "JB")==0 && G_JB != 1){
        Search_by_tag(head , "Jermy")->obj->image = load_image(file_path);
        G_JB = -1;
    }
    if(strcmp(card_name , "MS")==0&& G_MS != 1){
        Search_by_tag(head , "Stealthy")->obj->image = load_image(file_path);
        G_MS = -1;
    }
    if(strcmp(card_name , "SG")==0 && G_SG != 1){
        Search_by_tag(head , "Goodley")->obj->image = load_image(file_path);
        G_SG = -1;
    }
    if(strcmp(card_name , "IL")==0 && G_IL != 1){
        Search_by_tag(head , "Lestrade")->obj->image = load_image(file_path);
        G_IL = -1;
    }
    if(strcmp(card_name , "SH")==0 && G_SH != 1){
        Search_by_tag(head , "Sherlock")->obj->image = load_image(file_path);
        G_SH = -1;
    }
    if(strcmp(card_name , "WG")==0 && G_WG != 1){
        Search_by_tag(head , "Gull")->obj->image = load_image(file_path);
        G_WG = -1;
    }
}
int distance(int x1 , int y1 , int x2 , int y2){
    return ((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1));
}
int distance_2(Drawable *node1 , Drawable *node2){
    return distance(node1->obj->pos.x+50 , node1->obj->pos.y+44 , node2->obj->pos.x+50 , node2->obj->pos.y+44);
}
int dist(Drawable *head , int first , int second , int gfirst , int gsecond){
    Drawable *cell1 = Find_Cell(head , first , second);
    Drawable *cell2 = Find_Cell(head , gfirst , gsecond);
    return distance_2(cell1 , cell2);
}
bool is_cell_wall(int first , int second){
    return ((first==0 && second==2) || (first==11 &&second == 1)||(first==12 && second == 16)||(first==1&&second == 17));
}

void Create_Cards(Card **First_half , Card **Second_half){
    Card *Main_Cards;
    add_all_characters(&Main_Cards);
    shuffle_cards(Main_Cards);
    *First_half = Create_card(Main_Cards->name);
    remove_card(&Main_Cards,Main_Cards->name);
    for(int i =0;i<3;i++){
        append_card(*First_half,Main_Cards->name);
        remove_card(&Main_Cards,Main_Cards->name);
    }
    *Second_half = Create_card(Main_Cards->name);
    remove_card(&Main_Cards,Main_Cards->name);
    for(int i =0;i<3;i++){
        append_card(*Second_half,Main_Cards->name);
        remove_card(&Main_Cards,Main_Cards->name);
    }

}
void kmkm(Drawable *head, Card *kkd){
    for(int i = 1 ; i<=4 ;i++){
        char search[20];
        sprintf(search , "card_%d" , i);
        Search_by_tag(head , search)->obj->visible = true;
    }
    int cnt = 1;
    while(kkd){
        char search[20];
        sprintf(search , "1card_%d" , cnt);
        char file[20];
        sprintf(file , "Images\\%s.png" , (kkd->name));
        SDL_FreeSurface(Search_by_tag(head , search)->obj->image);
        Search_by_tag(head , search)->obj->image = load_image(file);
        cnt++;
        kkd = kkd->next;
    }
}
char *get_next_turn(Drawable *head, Card **First_half , Card **Second_half , int *round , int *turn , bool *is_jacks_turn ,SDL_Surface *window_surface ,SDL_Surface *light_eff,SDL_Surface *walk_able_eff,SDL_Window *window ){
    if((*round)==9 && (*turn)==1){
        printf("lol");
        Drawable *lol = head;
        while(lol){
            if(strcmp(lol->obj->tag,"Won_Label")==0){
                Jack_Wins(head , lol->obj , window_surface , light_eff , walk_able_eff , window);
            }
            lol = lol->next;
        }
    }
    if((*turn)==5){
        (*round)++;
        (*turn) = 1;
        if((*round)%2==1){
            (*First_half) = NULL;
            (*Second_half) = NULL;
            Create_Cards(First_half , Second_half);
        }
        return get_next_turn(head , First_half , Second_half,round, turn , is_jacks_turn , window_surface , light_eff , walk_able_eff , window);
    }
    if((*round)%2==1){

        if((*turn) == 1 || (*turn)==4)
            (*is_jacks_turn) = false;
        else
            (*is_jacks_turn) = true;
        char *anser="";
        Card *seek = *First_half;
        for(int i = 0 ;i<(*turn)-1 ;i++)
            seek = seek->next;
        anser = seek->name;
        (*turn)++;
        kmkm(head , *First_half);
        char se[20];
        sprintf(se , "card_%d" , (*turn)-1);
        Search_by_tag(head , se)->obj->visible = false;
        return anser;
    }
    else{
        if((*turn) == 1 || (*turn) == 4)
            (*is_jacks_turn) = true;
        else
            (*is_jacks_turn) = false;
        char *anser="";
        Card *seek = *Second_half;
        for(int i = 0 ;i<(*turn)-1 ;i++)
            seek = seek->next;
        anser = seek->name;
        (*turn)++;
        kmkm(head , *Second_half);
        char se[20];
        sprintf(se , "card_%d" , (*turn)-1);
        Search_by_tag(head , se)->obj->visible = false;
        return anser;
    }
}
char *get_moadel(char *name){
    if(strcmp(name , "JW")==0)
        return "Watson";
    if(strcmp(name , "MS")==0)
        return "Stealthy";
    if(strcmp(name , "WG")==0)
        return "Gull";
    if(strcmp(name , "JS")==0)
        return "Smith";
    if(strcmp(name , "JB")==0)
        return "Bert";
    if(strcmp(name , "SG")==0)
        return "Goodley";
    if(strcmp(name , "SH")==0)
        return "Sherlock";
    if(strcmp(name , "IL")==0)
        return "Lestrade";

}
int Len(Card *var){
    int cnt = 0;
    while(var){
        cnt++;
        var = var->next;
    }
    return cnt;
}
int name_to_int(char *name){
    if(strcmp(name,"JW")==0)
        return 0;
    if(strcmp(name,"JS")==0)
        return 1;
    if(strcmp(name,"JB")==0)
        return 2;
    if(strcmp(name,"MS")==0)
        return 3;
    if(strcmp(name,"IL")==0)
        return 4;
    if(strcmp(name,"SG")==0)
        return 5;
    if(strcmp(name,"WG")==0)
        return 6;
    if(strcmp(name,"SH")==0)
        return 7;
}
char *_int_to_name(int a){
    if(a==0)
        return "JW";
    if(a==1)
        return "JS";
    if(a==2)
        return "JB";
    if(a==3)
        return "MS";
    if(a==4)
        return "IL";
    if(a==5)
        return "SG";
    if(a==6)
        return "WG";
    if(a==7)
        return "SH";
}
void load_game(char *save_name , int *round , int *turn , Drawable *head , Card *first_half , Card *second_half , Card **Detective_sus , Card **JackSus , Card **Sus , char **Jackname){
    FILE *file = fopen(save_name ,"rb");
    int kk;
    Search_by_tag(head , "move")->obj->visible = false;
    Search_by_tag(head , "action")->obj->visible = false;
    Search_by_tag(head , "one")->obj->visible = false;
    Search_by_tag(head , "two")->obj->visible = false;
    Search_by_tag(head , "three")->obj->visible = false;
    Search_by_tag(head , "before")->obj->visible = false;
    Search_by_tag(head , "after")->obj->visible = false;

    Set_all_cells_unwalkable(head);
    fread(&kk , sizeof(int) , 1, file);
    (*Jackname) = _int_to_name(kk);
    fread(round , sizeof(int) , 1,file);
    fread(turn , sizeof(int) , 1,file);
    fread(&(Search_by_tag(head , "JW")->obj->direction) , sizeof(int) , 1,file);
    Drawable *cell;
    int x, y;
    cell =  Search_by_tag(head, "JW");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    change_watson_direction(head , Search_by_tag(head , "JW")->obj->direction);
    cell =  Search_by_tag(head, "JB");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "MS");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "WG");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "IL");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "SH");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "JS");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "SG");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "wall1");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "wall2");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "LI1");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "LI2");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "LI3");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "LI4");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "LI5");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "LI6");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "pit_hold1");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    cell =  Search_by_tag(head, "pit_hold2");
    fread(&x , sizeof(int) , 1 , file);
    fread(&y , sizeof(int) , 1 , file);
    cell->obj->pos.x = x;
    cell->obj->pos.y = y;
    int Detective_sus_num;
    fread(&Detective_sus_num , sizeof(int) , 1 , file);
    char *name;
    int tmp;
    if(Detective_sus_num!=0){
        fread(&tmp , sizeof(int),1, file);
        name = _int_to_name(tmp);
        (*Detective_sus) = NULL;
        (*Detective_sus) = Create_card(name);
        for(int i = 1; i<Detective_sus_num;i++){
            fread(&tmp , sizeof(int),1, file);
            name = _int_to_name(tmp);
            append_card((*Detective_sus) , name);
        }
    }
    int Jack_sus_num;
    fread(&Jack_sus_num , sizeof(int) , 1 , file);
    if(Jack_sus_num!=0){
        fread(&tmp , sizeof(int),1, file);
        name = _int_to_name(tmp);
        (*JackSus) = NULL;
        (*JackSus) = Create_card(name);
        printf("lol");
        for(int i = 1; i<Jack_sus_num;i++){
            fread(&tmp , sizeof(int),1, file);
            name = _int_to_name(tmp);
            append_card((*JackSus) , name);
        }
    }
    int Sus_num;
    fread(&Sus_num , sizeof(int) , 1 , file);
    if(Sus_num!=0){
    fread(&tmp , sizeof(int),1, file);
    name = _int_to_name(tmp);
    (*Sus) = NULL;
    (*Sus) = Create_card(name);
        for(int i = 1; i<Sus_num;i++){
            fread(&tmp , sizeof(int),1, file);
            name = _int_to_name(tmp);
            append_card((*Sus) , name);
        }
    }
    Card *first = first_half;
    for(int i = 0 ; i<4;i++){
        fread(&tmp , sizeof(int) ,1 , file);
        name = _int_to_name(tmp);
        first->name = name;
        first = first->next;
    }
    Card *second = second_half;
    for(int i = 0 ; i<4;i++){
        fread(&tmp , sizeof(int) ,1 , file);
        name = _int_to_name(tmp);
        second->name = name;
        second = second->next;
    }
    Search_by_tag(head , "Goodley")->obj->image = load_image("Images\\Unknown_card.png");
    Search_by_tag(head , "Sherlock")->obj->image = load_image("Images\\Unknown_card.png");
    Search_by_tag(head , "Lestrade")->obj->image = load_image("Images\\Unknown_card.png");
    Search_by_tag(head , "Jermy")->obj->image = load_image("Images\\Unknown_card.png");
    Search_by_tag(head , "Smith")->obj->image = load_image("Images\\Unknown_card.png");
    Search_by_tag(head , "Watson")->obj->image = load_image("Images\\Unknown_card.png");
    Search_by_tag(head , "Stealthy")->obj->image = load_image("Images\\Unknown_card.png");
    Search_by_tag(head , "Gull")->obj->image = load_image("Images\\Unknown_card.png");
    fread(&G_JW , sizeof(int) , 1 , file);
    fread(&G_JS , sizeof(int) , 1 , file);
    fread(&G_JB , sizeof(int) , 1 , file);
    fread(&G_MS , sizeof(int) , 1 , file);
    fread(&G_IL , sizeof(int) , 1 , file);
    fread(&G_SH , sizeof(int) , 1 , file);
    fread(&G_WG , sizeof(int) , 1 , file);
    fread(&G_SG , sizeof(int) , 1 , file);
    if(G_JW==-1)
        burn_card(head , "JW");
    if(G_JW==1)
        load_card(head , "JW");
    if(G_JS==-1)
        burn_card(head , "JS");
    if(G_JS==1)
        load_card(head , "JS");
    if(G_JB==-1)
        burn_card(head , "JB");
    if(G_JB==1)
        load_card(head , "JB");
    if(G_MS==-1)
        burn_card(head , "MS");
    if(G_MS==1)
        load_card(head , "MS");
    if(G_IL==-1)
        burn_card(head , "IL");
    if(G_IL==1)
        load_card(head , "IL");
    if(G_SH==-1)
        burn_card(head , "SH");
    if(G_SH==1)
        load_card(head , "SH");
    if(G_WG==-1)
        burn_card(head , "WG");
    if(G_WG==1)
        load_card(head , "WG");
    if(G_SG==-1)
        burn_card(head , "SG");
    if(G_SG==1)
        load_card(head , "SG");
    burn_card(head , (*Jackname));
    fclose(file);
}
void save_game(char save_name[] , int save_no , int round , int turn , Drawable *head , Card *first_half , Card *second_half , Card *Detective_sus , Card *JackSus , Card *Sus , char *Jack_name){
    char file_name[100];
    if(save_name[strlen(save_name)-1]==' '||save_name[strlen(save_name)-1]=='_'){
            save_name[strlen(save_name)-1]='\0';
    }
    char file_name2[100];

    sprintf(file_name , "Saves\\%s_%d.save" , save_name , save_no);
    for(int i = 0; i<strlen(file_name);i++){
        if(file_name[i]==' ')
            file_name[i]='_';
    }
    FILE *file = fopen(file_name,"wb");
    int kk = name_to_int(Jack_name);
    fwrite(&kk , sizeof(int),1 , file);
    fwrite(&round , sizeof(int),1 , file);
    fwrite(&turn , sizeof(int),1 , file);
    int dir = Search_by_tag(head,  "JW")->obj->direction;
    fwrite(&dir , sizeof(int),1 , file);
    Drawable *cell;
    int x , y;

    cell = Search_by_tag(head , "JW");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "JB");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "MS");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "WG");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "IL");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "SH");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "JS");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "SG");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "wall1");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "wall2");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "LI1");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "LI2");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "LI3");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "LI4");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "LI5");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "LI6");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "pit_hold1");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    cell = Search_by_tag(head , "pit_hold2");
    x = cell->obj->pos.x;
    y = cell->obj->pos.y;
    fwrite(&x , sizeof(int) , 1 , file);
    fwrite(&y , sizeof(int) , 1 , file);

    int tmp = Len(Detective_sus);
    fwrite(&tmp , sizeof(int) , 1 , file);
    for(int i = 0; i<tmp;i++){
        int shit = name_to_int(Detective_sus->name);
        fwrite(&shit,sizeof(int),1,file);
        Detective_sus = Detective_sus->next;
    }
    int tmp2 = Len(JackSus);
    fwrite(&tmp2 , sizeof(int) , 1 , file);
    for(int i = 0; i<tmp2 ; i++){
        int shit = name_to_int(JackSus->name);
        fwrite(&shit,sizeof(int),1,file);
        JackSus = JackSus->next;
    }
    int tmp3 = Len(Sus);
    fwrite(&tmp3 , sizeof(int) , 1 , file);
    for(int i = 0; i<tmp3 ; i++){
        int shit = name_to_int(Sus->name);
        fwrite(&shit,sizeof(int),1,file);
        Sus = Sus->next;
    }
    for(int i = 0; i<4 ; i++){
        int shit = name_to_int(first_half->name);
        fwrite(&shit,sizeof(int),1,file);
        first_half = first_half->next;
    }
    for(int i = 0; i<4 ; i++){
        int shit = name_to_int(second_half->name);
        fwrite(&shit,sizeof(int),1,file);
        second_half = second_half->next;
    }
    fwrite(&G_JW , sizeof(int) , 1 , file);
    fwrite(&G_JS , sizeof(int) , 1 , file);
    fwrite(&G_JB , sizeof(int) , 1 , file);
    fwrite(&G_MS , sizeof(int) , 1 , file);
    fwrite(&G_IL , sizeof(int) , 1 , file);
    fwrite(&G_SH , sizeof(int) , 1 , file);
    fwrite(&G_WG , sizeof(int) , 1 , file);
    fwrite(&G_SG , sizeof(int) , 1 , file);

    fclose(file);
}
void unvisible_all(Drawable *head){
    Search_by_tag(head , "info_JS")->obj->visible = false;
    Search_by_tag(head , "info_JB")->obj->visible = false;
    Search_by_tag(head , "info_JW")->obj->visible = false;
    Search_by_tag(head , "info_MS")->obj->visible = false;
    Search_by_tag(head , "info_IL")->obj->visible = false;
    Search_by_tag(head , "info_SH")->obj->visible = false;
    Search_by_tag(head , "info_WG")->obj->visible = false;
    Search_by_tag(head , "info_SG")->obj->visible = false;
}

void Choose_Next(Drawable *Scene  , Card *Detective_sus , Card *JackSus , Card *Sus , char *save_name , int *save_no, char *Jacks_name , char **state , char **Chosen_Player, _clickable *information , _clickable *before , _clickable *after , _clickable *_move , _clickable *_action , Card **First_half , Card **Second_half , int *round , int *turn , bool *is_jacks_turn , SDL_Surface *window_surface ,SDL_Surface *light_eff,SDL_Surface *walk_able_eff,SDL_Window *window,_clickable *round_bel , _clickable *turn_bel , _clickable *card_bel){
    DrawStuff(&Scene , window_surface , light_eff,walk_able_eff);
    draw(window);
    clear_surface(window_surface);
    int cround = (*round);
    if(new_move||(*save_no)==1){
        save_game(save_name , (*save_no) , (*round) , (*turn) , Scene, *First_half , *Second_half , Detective_sus , JackSus , Sus , Jacks_name);
        (*save_no)++;
    }
    if(!new_move){
        new_move = true;
    }
    char *clicked_on = get_next_turn(Scene , First_half , Second_half , round , turn , is_jacks_turn,window_surface , light_eff , walk_able_eff , window);

    char kkm[15];
    sprintf(kkm , "info_%s" , clicked_on);
    unvisible_all(Scene);
    Search_by_tag(Scene , kkm)->obj->visible = true;
    char rr[200];
    char gg[200];
    sprintf(rr , "Round %d\0" , (*round));
    sprintf(gg,"Turn %d/%s - %s" ,  ((*turn)-1) , (*is_jacks_turn)?"Jack":"Detective" , get_moadel(clicked_on));
    rr[strlen(rr)]='\0';
    gg[strlen(gg)]='\0';
    char bb[200];
    bb[0]='\0';
    Card *ff = *First_half;
    for(int i = 0; i<4 ; i++){
        if(i!=0)
            sprintf(bb, "%s-%s" ,bb, ff->name);
        else
            sprintf(bb, "%s", ff->name);
        ff = ff->next;
    }
    ff = *Second_half;
    sprintf(bb , "%s || " , bb);
    for(int i = 0; i<4 ; i++){
        if(i!=0)
            sprintf(bb, "%s-%s" ,bb, ff->name);
        else
            sprintf(bb, "%s%s" ,bb, ff->name);
        ff = ff->next;
    }
    bb[strlen(bb)]='\0';
    SDL_Color fgg = { .r=0 , .g=100 , .b=255 };
    SDL_Color fgg2 = { .r=200 , .g=100 , .b=255 };
    SDL_Color fgg3 = { .r=200 , .g=0 , .b=255 };
    SDL_Color fg2 = { .r=50 , .g=255 , .b=50 };

    change_information_label(Scene , round_bel , rr,window_surface , light_eff , walk_able_eff , window,fgg);
    if(cround!=(*round)){
        Shohud(Scene,Jacks_name);
    }
    change_information_label(Scene , turn_bel , gg,window_surface , light_eff , walk_able_eff , window,fgg2);
    //if((*round)%2==1 && (*turn)==2)
        change_information_label(Scene , card_bel , bb,window_surface , light_eff , walk_able_eff , window,fgg3);

    if(strcmp(clicked_on,"")==0){
        change_information_label(Scene , information , "No character in this place",window_surface , light_eff , walk_able_eff , window,fg2);
    }
    else{
        (*Chosen_Player) = clicked_on;

        if(strcmp((*Chosen_Player) , "JS")==0){
            change_information_label(Scene , information , "Move lights before or after ? ",window_surface , light_eff , walk_able_eff , window,fg2);
            (*before).visible = true;
            (*after).visible = true;
        }
        if(strcmp((*Chosen_Player) , "JB")==0){
            change_information_label(Scene , information , "Move pit before or after ? ",window_surface , light_eff , walk_able_eff , window,fg2);
            (*before).visible = true;
            (*after).visible = true;
        }
        if(strcmp((*Chosen_Player) , "WG")==0){
            change_information_label(Scene , information , "Use Action or move ?",window_surface , light_eff , walk_able_eff , window,fg2);
            (*_action).visible = true;
            (*_move).visible = true;
            (*state) = "action_move";
        }
        if(strcmp((*Chosen_Player) , "SG")==0){
            change_information_label(Scene , information , "Action before or after ? ",window_surface , light_eff , walk_able_eff , window,fg2);
            (*before).visible = true;
            (*after).visible = true;
        }
        if(strcmp((*Chosen_Player) , "IL")==0){
            (*state) = "moving_player";
            walkable_character(Scene , "IL" , 3);
            change_information_label(Scene , information , "Select Cell to Move",window_surface , light_eff , walk_able_eff , window,fg2);
        }
        if(strcmp((*Chosen_Player) , "SH")==0){
            (*state) = "moving_player";
            walkable_character(Scene , "SH" , 3);
            change_information_label(Scene , information , "Select Cell to Move",window_surface , light_eff , walk_able_eff , window,fg2);
        }
        if(strcmp((*Chosen_Player) , "JW")==0){
            (*state) = "moving_player";
            walkable_character(Scene , "JW" , 3);
            change_information_label(Scene , information , "Select Cell to Move",window_surface , light_eff , walk_able_eff , window,fg2);
        }
        if(strcmp((*Chosen_Player) , "MS")==0){
            (*state) = "moving_player";
            walkable_character_MS(Scene , "MS" , 4);
            change_information_label(Scene , information , "Select Cell to Move",window_surface , light_eff , walk_able_eff , window,fg2);
        }
    }

}
void change_information_label_2(Drawable *head ,_clickable *label , char *new_text , SDL_Surface *window_surface , SDL_Surface *light_eff,SDL_Surface *walkable_effect,SDL_Window *window,SDL_Color fg2){
    TTF_Font *font_consolas;
    font_consolas = TTF_OpenFont("ALGER.TTF" , 100);
    for(int i = 0; i<strlen(new_text);i++){
        char show[strlen(new_text)+1];
        for (int j = 0 ; j<=i ; j++){
            show[j] = new_text[j];
        }
        show[i+1] = '\0';
        SDL_Surface *new_surface = TTF_RenderText_Blended_Wrapped(font_consolas , show,fg2,1000);
        SDL_FreeSurface(label->image);
        label->image = new_surface;
        DrawStuff(&head , window_surface , light_eff,walkable_effect);
        draw(window);
        clear_surface(window_surface);
        SDL_Delay(10);
    }
    TTF_CloseFont(font_consolas);
}
void Detective_Wins(Drawable *head , _clickable *Win_Label ,SDL_Surface *window_surface ,SDL_Surface *light_eff,SDL_Surface *walk_able_eff,SDL_Window *window){
    Stop_Mixer();
    Play_voice("Sounds\\Yeay.mp3");
    Win_Label->visible = true;
    SDL_Color fg2 = { .r=50 , .g=255 , .b=50 };
    Search_by_tag(head, "blackeff")->obj->visible = true;
    change_information_label_2(head , Win_Label , "Detective Won!",window_surface , light_eff , walk_able_eff , window,fg2);
    end_game();
}
void Jack_Wins(Drawable *head , _clickable *Win_Label ,SDL_Surface *window_surface ,SDL_Surface *light_eff,SDL_Surface *walk_able_eff,SDL_Window *window){
    Stop_Mixer();
    Play_voice("Sounds\\Game_over.mp3");
    Win_Label->visible = true;
    SDL_Color fg2 = { .r=255 , .g=50 , .b=50 };
    Search_by_tag(head, "blackeff")->obj->visible = true;
    change_information_label_2(head , Win_Label , "Mr.Jack Won!",window_surface , light_eff , walk_able_eff , window,fg2);
    end_game();
}
char *dest_char_name(Drawable *head , int first , int second){
    Drawable *cell = Find_Cell(head , first , second);
    while(head){
        if(strlen(head->obj->tag)==2){
            if(head->obj->pos.x-18 == cell->obj->pos.x && head->obj->pos.y-13 == cell->obj->pos.y){
                return head->obj->tag;
            }
        }
        head = head->next;
    }
    return "";
}
void end_game(){

}
bool is_cell_empty_wall(Drawable *head,  int first , int second){
    if(!is_cell_wall(first , second))
        return false;
    else{
        Drawable *wall1 = Search_by_tag(head,  "wall1");
        Drawable *wall2 = Search_by_tag(head,  "wall2");
        _pair wall1_pos = loc(head , wall1->obj->pos.x , wall1->obj->pos.y);
        _pair wall2_pos = loc(head , wall2->obj->pos.x , wall2->obj->pos.y);
        if((wall1_pos.first == first && wall1_pos.second == second)||((wall2_pos.first == first && wall2_pos.second == second))){
            return false;
        }
        return true;
    }
}
void* func(void* arg)
{
    pthread_detach(pthread_self());
    system((char*)arg);
    // exit the current thread
    pthread_exit(NULL);
}
void send_jack_name(char username[100] , char name[10] , char save_name[60]){
    char lol[200];
    if(save_name[strlen(save_name)-1]=='_'||save_name[strlen(save_name)-1]==' ')
        save_name[strlen(save_name)-1]='\0';
    sprintf(lol , "req.exe %s %s %s" ,username , name,save_name);
    system(lol);
    printf("\nSent!");
}
bool is_cell_in_map(int first , int second){
    if(first<0 || first>12)
        return false;
    if(first%2==0){
        if(second<0||second>16)
            return false;
    }
    else{
        if(second<1 || second>17)
            return false;
    }
    return true;
}
bool is_cell_full_2(Drawable *head , int first, int second){
    if(!is_cell_in_map(first , second))
        return false;
    Drawable *cell = Find_Cell(head , first , second);
    return is_cell_full(head , cell->obj->pos.x+18 , cell->obj->pos.y+13);
}
bool is_seen(Drawable *head , char *name){
    Drawable *cell = cell_under_character(head , name);
    if(cell->obj->cell_info->lighten)
        return true;
    _pair lol = {cell->obj->cell_info->cell_pos.first , cell->obj->cell_info->cell_pos.second};
    int first , second;
    first = lol.first;
    second = lol.second;
    if(is_cell_full_2(head, first , second-2)||is_cell_full_2(head, first , second+2)||is_cell_full_2(head, first-1 , second-1)||is_cell_full_2(head, first+1 , second-1)||is_cell_full_2(head, first-1 , second+1)||is_cell_full_2(head, first+1 , second+1))
        return true;
    return false;
}
void Shohud(Drawable *head , char *JacksName){
    bool is_jack_seen = is_seen(head, JacksName);
    //load_card(head, JacksName);

    if(is_jack_seen != is_seen(head, "JW")){
        load_card(head , "JW");
    }
    if(is_jack_seen != is_seen(head, "JS")){
        load_card(head , "JS");
    }
    if(is_jack_seen != is_seen(head, "JB")){
        load_card(head , "JB");
    }
    if(is_jack_seen != is_seen(head, "MS")){
        load_card(head , "MS");
    }
    if(is_jack_seen != is_seen(head, "SG")){
        load_card(head , "SG");
    }
    if(is_jack_seen != is_seen(head, "WG")){
        load_card(head , "WG");
    }
    if(is_jack_seen != is_seen(head, "IL")){
        load_card(head , "IL");
    }
    if(is_jack_seen != is_seen(head, "SH")){
        load_card(head , "SH");
    }
}

int get_last_save(char name[100]){
    int no = 1;
    char file_name[200];
    sprintf(file_name , "Saves\\%s_%d.save" , name , no);
    FILE *file = fopen(file_name, "r");
    if(!file)
        return 0;
    else{
        fclose(file);
        while(true){
            no++;
            char file_name2[200];
            sprintf(file_name , "Saves\\%s_%d.save" , name , no);
            FILE *file = fopen(file_name, "r");
            if(!file)
                break;
            else
                fclose(file);
        }
        return no-1;
    }

}

