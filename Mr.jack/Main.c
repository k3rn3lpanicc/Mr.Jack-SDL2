#include "SDL.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include<stdlib.h>
#include "Graphics/Mrjack_Graphic.h"
#include <Windows.h>
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
int _min(int a,int b){return a<b?a:b;}
typedef SDL_Surface Image;
typedef SDL_Rect Position;
_pair main_path[13*9];
int min_len=100000;
void walkable_character(Drawable *Scene , char *name, int l);
void unwalkable_character(Drawable *Scene , char *name, int l);
void walkable_character_MS(Drawable *Scene , char *name, int l);
void get_shortest_path(Drawable *head , int first , int second , int gfirst , int gsecond , int l ,int ll , _pair path[13*9] , _pair visited_pits[9] , int visited_pits_cnt, int martabe);
bool move_character_in_shortest_path(Drawable *head , char *name, int l,SDL_Surface *window_surface , SDL_Surface *light_eff,SDL_Surface *walkable_effect,SDL_Window *window);
int main(int argc , char** argv){
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(0));
    char *MrJacksName = Choose_random_character();
    HWND windowHandle = GetConsoleWindow();
    SetConsoleTextAttribute(hConsole , 12);
    printf("Please Enter your telegram username : ");
    SetConsoleTextAttribute(hConsole , 15);
    char username[100];
    scanf("%s" , username);
    SDL_Event event;
    Init();
    font_segoeui = TTF_OpenFont("segoeui.ttf" , 20);
    font_segoeui_big = TTF_OpenFont("segoeui.ttf" , 30);
    font_consolas_big = TTF_OpenFont("ALGER.ttf" , 60);
    font_consolas = TTF_OpenFont("ALGER.TTF" , 18);
    int Volume = 50;
    Mix_Music *bgm = Mix_LoadMUS("Sounds\\bg1.mp3");
    Mix_PlayMusic(bgm,-1);
    Mix_VolumeMusic(Volume);
    SDL_Window *window = SDL_CreateWindow(TITLE_TEXT,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1500, 850,SDL_WINDOW_SHOWN);
    if(!window)
        error_and_exit("Failed to create window\n");
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    if(!window_surface)
        error_and_exit("Failed to get the surface from the window\n");
    // --------------Initialize Inocent Cards
    int CX=1345 , CY=22;
    int ddx , ddy;
    ddx = 4;
    ddy = 0;
    _clickable Goodley={.tag= "Goodley",.click_able = false,.entered = false,.do_chick = false,.visible = true,.pos = { .x = CX , .y = CY},.image = load_image(unknown_card)};
    _clickable Sherlock={.tag = "Sherlock" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = CX+72+ddx , .y = CY },.image = load_image(unknown_card)};
    _clickable Lestrade={.tag= "Lestrade",.click_able = false,.entered = false,.do_chick = false,.visible = true,.pos = { .x = CX , .y = CY+(108+ddy)},.image = load_image(unknown_card)};
    _clickable Jermy={.tag = "Jermy" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = CX+72+ddx , .y = CY+108+ddy },.image = load_image(unknown_card)};
    _clickable Smith={.tag = "Smith" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = CX , .y = CY+(108+ddy)*2 },.image = load_image(unknown_card)};
    _clickable Watson={.tag = "Watson" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = CX+72+ddx , .y = CY+(108+ddy)*2 },.image = load_image(unknown_card)};
    _clickable Stealthy={.tag = "Stealthy" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = CX , .y = CY+(108+ddy)*3 },.image = load_image(unknown_card)};
    _clickable Gull={.tag = "Gull" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = CX+72+ddx , .y = CY+(108+ddy)*3 },.image = load_image(unknown_card)};
    int __x = 585;
    int __y = 215;
    _clickable Border = {.tag = "Border" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x-35 , .y = __y-75},.image = load_image(border_pic)};
    _clickable Play = {.tag = "Play" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y+45},.image = load_image(btn_play)};
    _clickable Exit = {.tag = "exit" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y+225 },.image = load_image(btn_exit)};
    _clickable About_me = {.tag = "About Me" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y+135 },.image = load_image(btn_about)};
    _clickable mum_btn = {.tag = "mute" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image(unmute_btn)};
    _clickable cmd_btn = {.tag = "cmd" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 1450 , .y = 700 },.image = load_image("Images\\cmd.png")};
    _clickable tlgrm = {.tag = "telegram" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 1400 , .y = 750 },.image = load_image("Images\\telegram.png")};
    _clickable undo = {.tag = "undo" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 1400 , .y = 800 },.image = load_image("Images\\undo.png")};
    _clickable background_picture = {.tag = "bground" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 0 , .y = 0 },.image = load_image(bg_pic)};
    _clickable background_picture2 = {.tag = "bground2" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 0 , .y = 0 },.image = load_image(bg_pic2)};
    _clickable Back_btn = {.tag = "Back" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y+300 },.image = load_image(btn_back)};
    _clickable About_Label = {.tag = "About_me" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x-10 , .y = __y-30 },.image = load_image(about_me)};
    _clickable Home = {.tag = "Home" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 800 },.image = load_image(home)};
    bool keep_window_open = true;
    _pos mouse_pos;
    TTF_SetFontStyle(font_segoeui ,TTF_STYLE_ITALIC|TTF_STYLE_BOLD|TTF_STYLE_UNDERLINE);
    SDL_Surface *text_surface = TTF_RenderText_Solid(font_segoeui , "Type your save's Name : ",fg);
    TTF_SetFontStyle(font_segoeui ,TTF_STYLE_BOLD);
    SDL_Surface *text_box = TTF_RenderText_Solid(font_consolas , "_",fg);
    SDL_Surface *enter_game = TTF_RenderText_Solid(font_consolas , "Save and Continue",fg);
    SDL_Surface *information_label = TTF_RenderText_Solid(font_consolas , "Choose the Player",fg2);
    SDL_Color fgg = { .r=0 , .g=100 , .b=255 };
    SDL_Surface *game_round_label = TTF_RenderText_Solid(font_consolas , "Round 1",fgg);
    SDL_Surface *turn_label = TTF_RenderText_Solid(font_consolas , "Turn1/Detective",fg2);
    SDL_Surface *Cards_in_this_round_label = TTF_RenderText_Solid(font_consolas , "This round : ",fg2);
    SDL_Surface *before_label = TTF_RenderText_Solid(font_consolas , "Before",fgg);
    SDL_Surface *after_label = TTF_RenderText_Solid(font_consolas , "After",fgg);
    SDL_Surface *move = TTF_RenderText_Solid(font_consolas , "move",fgg);
    SDL_Surface *action = TTF_RenderText_Solid(font_consolas , "action",fgg);
    SDL_Surface *_one = TTF_RenderText_Solid(font_consolas , "one",fgg);
    SDL_Surface *_two = TTF_RenderText_Solid(font_consolas , "two",fgg);
    SDL_Surface *_three = TTF_RenderText_Solid(font_consolas , "three",fgg);
    SDL_Surface *linfo_SG = TTF_RenderText_Blended_Wrapped(font_consolas , "                      **Sergent Goodley**\n\n-Can use it's action before or after movement\n\n-Moves one-three Cells\n-Action : move players near to him with maximum\n   movements of three." , fg , 1000);
    _clickable info_SG = {.tag = "info_SG" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 1032 , .y = 500 },.image = linfo_SG};
    SDL_Surface *linfo_JW = TTF_RenderText_Blended_Wrapped(font_consolas , "                               **John Watson**\n\n-Can use it's Action only after movement.\n\n-Moves one-three Cells\n-Action : Choose the Direction of the light\n   to light that direction." , fg , 1000);
    _clickable info_JW = {.tag = "info_JW" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 1032 , .y = 500 },.image = linfo_JW};
    SDL_Surface *linfo_JS = TTF_RenderText_Blended_Wrapped(font_consolas , "                               **John Smith**\n\n-Can use it's action before or after movement\n\n-Moves one-three Cells\n-Action : He Can move the lights." , fg , 1000);
    _clickable info_JS = {.tag = "info_JS" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 1032 , .y = 500 },.image = linfo_JS};
    SDL_Surface *linfo_JB = TTF_RenderText_Blended_Wrapped(font_consolas , "                               **Jermy Bert**\n\n-Can use it's action before or after movement\n\n-Moves one-three Cells\n-Action : He Can move pitholds." , fg , 1000);
    _clickable info_JB = {.tag = "info_JB" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 1032 , .y = 500 },.image = linfo_JB};
    SDL_Surface *linfo_MS = TTF_RenderText_Blended_Wrapped(font_consolas , "                             **Miss Stealthy**\n\n-Can use it's action during movement\n\n-Moves one-four Cells\n-Action : She Can move through houses." , fg , 1000);
    _clickable info_MS = {.tag = "info_MS" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 1032 , .y = 500 },.image = linfo_MS};
    SDL_Surface *linfo_IL = TTF_RenderText_Blended_Wrapped(font_consolas , "                        **Inspector Lestrade**\n\n-Can use it's action after movement\n\n-Moves one-three Cells\n-Action : He Can move walls of town." , fg , 1000);
    _clickable info_IL = {.tag = "info_IL" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 1032 , .y = 500 },.image = linfo_IL};
    SDL_Surface *linfo_SH = TTF_RenderText_Blended_Wrapped(font_consolas , "                           **Sherlock Holmes**\n\n-Can use it's action after movement\n\n-Moves one-three Cells\n-Action : you can pick a cart\n    from suspect cards." , fg , 1000);
    _clickable info_SH = {.tag = "info_SH" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 1032 , .y = 500 },.image = linfo_SH};
    SDL_Surface *linfo_WG = TTF_RenderText_Blended_Wrapped(font_consolas , "                             **William Gull**\n\n-Can use it's Action 'OR' Movemen.\n\n-Moves one-three Cells\n-Action : Can Switch his location with\n  another character." , fg , 1000);
    _clickable info_WG = {.tag = "info_WG" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 1032 , .y = 500 },.image = linfo_WG};
    SDL_Surface *won_label = TTF_RenderText_Solid(font_consolas_big , "Detective Won",fg2);
    _clickable info_border = {.tag = "info_br" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1028 , .y = 490 },.image = load_image("Images\\info_border.png")};

    _clickable labeld = {.tag = "dd" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x , .y = __y-35 },.image = text_surface};
    _clickable text_box_d = {.tag = "text_box" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x , .y = __y+35 },.image = text_box};
    _clickable text_box_area = {.tag = "text_boxa" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x-5 , .y = __y+27 },.image = load_image("Images\\text_box.png")};
    _clickable Play2 = {.tag = "Play2" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = __x , .y = __y+250 },.image = enter_game};
    _clickable information = {.tag = "information" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1024 , .y = 795 },.image = information_label};
    _clickable before = {.tag = "before" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = 1024 , .y = 820 },.image = before_label};
    _clickable after = {.tag = "after" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = 1094 , .y = 820 },.image = after_label};
    _clickable _move = {.tag = "move" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = 1024 , .y = 820 },.image = move};
    _clickable _action = {.tag = "action" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = 1094 , .y = 820 },.image = action};
    _clickable one_ = {.tag = "one" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = 1024 , .y = 820 },.image = _one};
    _clickable two_ = {.tag = "two" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = 1094 , .y = 820 },.image = _two};
    _clickable three_ = {.tag = "three" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = 1164 , .y = 820 },.image = _three};
    _clickable Won_Label = {.tag = "Won_Label" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 380 , .y = 330 },.image = won_label};
    _clickable round_bel = {.tag = "game_round" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1030 , .y = 10 },.image = game_round_label};
    _clickable turn_bel = {.tag = "round_turn" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1030 , .y = 30 },.image = turn_label};
    _clickable card_bel = {.tag = "round_cards" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1030 , .y = 50 },.image = Cards_in_this_round_label};
    int xx , yy;
    xx = 1260;
    yy = 760;
    bool arrow_vis = false;
    _clickable arrow0 = {.tag = "arrow_0" ,.click_able = true, .entered = false,.do_chick = true,.visible = arrow_vis,.pos = { .x = xx , .y = yy-30 },.image = load_image("Images\\up.png")};
    _clickable arrow1 = {.tag = "arrow_1" ,.click_able = true, .entered = false,.do_chick = true,.visible = arrow_vis,.pos = { .x = xx+40 , .y = yy-30},.image = load_image("Images\\ur.png")};
    _clickable arrow2 = {.tag = "arrow_2" ,.click_able = true, .entered = false,.do_chick = true,.visible = arrow_vis,.pos = { .x = xx+40 , .y = yy+30 },.image = load_image("Images\\dr.png")};
    _clickable arrow3 = {.tag = "arrow_3" ,.click_able = true, .entered = false,.do_chick = true,.visible = arrow_vis,.pos = { .x = xx , .y = yy+30 },.image = load_image("Images\\down.png")};
    _clickable arrow4 = {.tag = "arrow_4" ,.click_able = true, .entered = false,.do_chick = true,.visible = arrow_vis,.pos = { .x = xx-40 , .y = yy+30 },.image = load_image("Images\\dl.png")};
    _clickable arrow5 = {.tag = "arrow_5" ,.click_able = true, .entered = false,.do_chick = true,.visible = arrow_vis,.pos = { .x = xx-40 , .y = yy-30 },.image = load_image("Images\\ul.png")};
    //-------------------------------------------------------
    SDL_Surface *light_eff = load_image(light_effect);
    SDL_Surface *walk_able_eff = load_image("Images\\Blocks\\walkable.png");
    //----------------------------------------------------
    Drawable *Menu = Create_node(&background_picture);
    Add_obj(Menu , Create_node(&Border));
    Add_obj(Menu , Create_node(&Play));
    Add_obj(Menu , Create_node(&Exit));
    Add_obj(Menu , Create_node(&About_me));
    //----------------------------------------------------
    Drawable *About = Create_node(&background_picture);
    Add_obj(About , Create_node(&Border));
    Add_obj(About , Create_node(&Back_btn));
    Add_obj(About , Create_node(&mum_btn));
    Add_obj(About , Create_node(&About_Label));
    //---------------------------------------------------
    _clickable card_1 = {.tag = "card_1" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1027 , .y = 78 },.image = load_image("Images\\disabled_card.png")};
    _clickable card_2 = {.tag = "card_2" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1182 , .y = 78 },.image = load_image("Images\\disabled_card.png")};
    _clickable card_3 = {.tag = "card_3" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1027 , .y = 273 },.image = load_image("Images\\disabled_card.png")};
    _clickable card_4 = {.tag = "card_4" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1182 , .y = 273 },.image = load_image("Images\\disabled_card.png")};
    _clickable _card_1 = {.tag = "1card_1" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1027 , .y = 78 },.image = load_image("Images\\disabled_card.png")};
    _clickable _card_2 = {.tag = "1card_2" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1182 , .y = 78 },.image = load_image("Images\\disabled_card.png")};
    _clickable _card_3 = {.tag = "1card_3" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1027 , .y = 273 },.image = load_image("Images\\disabled_card.png")};
    _clickable _card_4 = {.tag = "1card_4" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1182 , .y = 273 },.image = load_image("Images\\disabled_card.png")};
    _clickable Map_Border = {.tag = "Map_Border" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 6 , .y = 3 },.image = load_image("Images\\map_border.png")};
    Drawable *Scene=Create_node(&background_picture2);
    Add_obj(Scene , Create_node(&Map_Border));
    Draw_map(Scene,10,10);

    Add_obj(Scene , Create_node(&_card_1));
    Add_obj(Scene , Create_node(&_card_2));
    Add_obj(Scene , Create_node(&_card_3));
    Add_obj(Scene , Create_node(&_card_4));
    Add_obj(Scene , Create_node(&card_1));
    Add_obj(Scene , Create_node(&card_2));
    Add_obj(Scene , Create_node(&card_3));
    Add_obj(Scene , Create_node(&card_4));

    Add_obj(Scene , Create_node(&Home));
    Add_obj(Scene , Create_node(&mum_btn));
    Add_obj(Scene , Create_node(&info_border));
    Add_obj(Scene , Create_node(&info_SG));
    Add_obj(Scene , Create_node(&info_JW));
    Add_obj(Scene , Create_node(&info_JS));
    Add_obj(Scene , Create_node(&info_JB));
    Add_obj(Scene , Create_node(&info_MS));
    Add_obj(Scene , Create_node(&info_IL));
    Add_obj(Scene , Create_node(&info_SH));
    Add_obj(Scene , Create_node(&info_WG));
    Add_obj(Scene , Create_node(&undo));

    _clickable light1 = {.tag = "LI1" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\light.png")};
    Drawable *cell = Find_Cell(Scene , 1,13);
    light1.pos.x = cell->obj->pos.x;
    light1.pos.y = cell->obj->pos.y;
    light1.cell_info = cell->obj->cell_info;
    cell->obj->cell_info->lighten = true;
    Add_obj(Scene , Create_node(&light1));

    _clickable light2 = {.tag = "LI2" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\light.png")};
    cell = Find_Cell(Scene ,2,4);
    light2.pos.x = cell->obj->pos.x;
    light2.pos.y = cell->obj->pos.y;
    light2.cell_info = cell->obj->cell_info;
    cell->obj->cell_info->lighten = true;
    Add_obj(Scene , Create_node(&light2));

    _clickable light3 = {.tag = "LI3" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\light.png")};
    cell = Find_Cell(Scene ,5,11);
    light3.pos.x = cell->obj->pos.x;
    light3.pos.y = cell->obj->pos.y;
    light3.cell_info = cell->obj->cell_info;
    cell->obj->cell_info->lighten = true;
    Add_obj(Scene , Create_node(&light3));

    _clickable light4 = {.tag = "LI4" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\light.png")};
    cell = Find_Cell(Scene ,7,15);
    light4.pos.x = cell->obj->pos.x;
    light4.pos.y = cell->obj->pos.y;
    light4.cell_info = cell->obj->cell_info;
    cell->obj->cell_info->lighten = true;
    Add_obj(Scene , Create_node(&light4));

    _clickable light5 = {.tag = "LI5" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\light.png")};
    cell = Find_Cell(Scene ,10,14);
    light5.pos.x = cell->obj->pos.x;
    light5.pos.y = cell->obj->pos.y;
    light5.cell_info = cell->obj->cell_info;
    cell->obj->cell_info->lighten = true;
    Add_obj(Scene , Create_node(&light5));

    _clickable light6 = {.tag = "LI6" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\light.png")};
    cell = Find_Cell(Scene ,11,5);
    light6.pos.x = cell->obj->pos.x;
    light6.pos.y = cell->obj->pos.y;
    light6.cell_info = cell->obj->cell_info;
    cell->obj->cell_info->lighten = true;
    Add_obj(Scene , Create_node(&light6));

    _clickable pit_hold1 = {.tag = "pit_hold1" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\pit_hold.png")};
    cell = Find_Cell(Scene ,11,3);
    pit_hold1.pos.x = cell->obj->pos.x;
    pit_hold1.pos.y = cell->obj->pos.y;
    pit_hold1.cell_info = cell->obj->cell_info;
    Add_obj(Scene , Create_node(&pit_hold1));

    _clickable pit_hold2 = {.tag = "pit_hold2" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\pit_hold.png")};
    cell = Find_Cell(Scene ,1,15);
    pit_hold2.pos.x = cell->obj->pos.x;
    pit_hold2.pos.y = cell->obj->pos.y;
    pit_hold2.cell_info = cell->obj->cell_info;
    Add_obj(Scene , Create_node(&pit_hold2));

    _clickable wall1 = {.tag = "wall1" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\exit_closed.png")};
    cell = Find_Cell(Scene ,1,17);
    wall1.pos.x = cell->obj->pos.x;
    wall1.pos.y = cell->obj->pos.y;
    wall1.cell_info = cell->obj->cell_info;
    cell->obj->do_chick = false;
    cell->obj->click_able = true;
    Add_obj(Scene , Create_node(&wall1));

    _clickable wall2 = {.tag = "wall2" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\exit_closed.png")};
    cell = Find_Cell(Scene ,11,1);
    wall2.pos.x = cell->obj->pos.x;
    wall2.pos.y = cell->obj->pos.y;
    wall2.cell_info = cell->obj->cell_info;
    cell->obj->do_chick = false;
    cell->obj->click_able = true;
    Add_obj(Scene , Create_node(&wall2));

    cell = Find_Cell(Scene ,8,16);
    _clickable JW = {.tag = "JW" ,.click_able = true, .entered = false,.do_chick = false,.visible = true, .direction = 1,.pos = {.x = cell->obj->pos.x+18 , .y = cell->obj->pos.y+13},.image = load_image("Images\\Characters\\watson_1.png")};
    JW.cell_info = cell->obj->cell_info;
    Add_obj(Scene , Create_node(&JW));

    cell = Find_Cell(Scene ,4,10);
    _clickable IL = {.tag = "IL" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos ={cell->obj->pos.x+18,cell->obj->pos.y+13},.image = load_image("Images\\Characters\\Lestrade.png")};
    IL.cell_info = cell->obj->cell_info;
    Add_obj(Scene , Create_node(&IL));

    _clickable JS = {.tag = "JS" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\John_Smith.png")};
    cell = Find_Cell(Scene ,4,8);
    JS.pos.x = cell->obj->pos.x+18;
    JS.pos.y = cell->obj->pos.y+13;
    JS.cell_info = cell->obj->cell_info;
    Add_obj(Scene , Create_node(&JS));

    _clickable WG = {.tag = "WG" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\Gull.png")};
    cell = Find_Cell(Scene ,4,2);
    WG.pos.x = cell->obj->pos.x+18;
    WG.pos.y = cell->obj->pos.y+13;
    WG.cell_info = cell->obj->cell_info;
    Add_obj(Scene , Create_node(&WG));

    _clickable JB = {.tag = "JB" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\Bert.png")};
    cell = Find_Cell(Scene ,8,8);
    JB.pos.x = cell->obj->pos.x+18;
    JB.pos.y = cell->obj->pos.y+13;
    JB.cell_info = cell->obj->cell_info;
    Add_obj(Scene , Create_node(&JB));

    _clickable SG = {.tag = "SG" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\goodley.png")};
    cell = Find_Cell(Scene ,12,8);
    SG.pos.x = cell->obj->pos.x+18;
    SG.pos.y = cell->obj->pos.y+13;
    SG.cell_info = cell->obj->cell_info;
    Add_obj(Scene , Create_node(&SG));

    _clickable MS = {.tag = "MS" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\Stealthy.png")};
    cell = Find_Cell(Scene ,0,10);
    MS.pos.x = cell->obj->pos.x+18;
    MS.pos.y = cell->obj->pos.y+13;
    MS.cell_info = cell->obj->cell_info;
    Add_obj(Scene , Create_node(&MS));

    _clickable SH = {.tag = "SH" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\Sherlock.png")};
    cell = Find_Cell(Scene ,6,12);
    SH.pos.x = cell->obj->pos.x+18;
    SH.pos.y = cell->obj->pos.y+13;
    SH.cell_info = cell->obj->cell_info;
    Add_obj(Scene , Create_node(&SH));

    _clickable Suspects_border = {.tag = "susborder" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1337 , .y = 4 },.image = load_image("Images\\suspects.png")};


    Add_obj(Scene , Create_node(&information));
    Add_obj(Scene , Create_node(&before));
    Add_obj(Scene , Create_node(&after));
    Add_obj(Scene , Create_node(&arrow0));
    Add_obj(Scene , Create_node(&arrow1));
    Add_obj(Scene , Create_node(&arrow2));
    Add_obj(Scene , Create_node(&arrow3));
    Add_obj(Scene , Create_node(&arrow4));
    Add_obj(Scene , Create_node(&arrow5));
    Add_obj(Scene , Create_node(&_move));
    Add_obj(Scene , Create_node(&_action));
    Add_obj(Scene , Create_node(&Gull));
    Add_obj(Scene , Create_node(&Stealthy));
    Add_obj(Scene , Create_node(&Watson));
    Add_obj(Scene , Create_node(&Smith));
    Add_obj(Scene , Create_node(&Jermy));
    Add_obj(Scene , Create_node(&Lestrade));
    Add_obj(Scene , Create_node(&Sherlock));
    Add_obj(Scene , Create_node(&Goodley));
    Add_obj(Scene , Create_node(&one_));
    Add_obj(Scene , Create_node(&two_));
    Add_obj(Scene , Create_node(&three_));
    Add_obj(Scene , Create_node(&round_bel));
    Add_obj(Scene , Create_node(&turn_bel));
    Add_obj(Scene , Create_node(&card_bel));
    Add_obj(Scene , Create_node(&Suspects_border));
    Add_obj(Scene , Create_node(&cmd_btn));
    Add_obj(Scene , Create_node(&tlgrm));
    _clickable black_effect = {.tag = "blackeff" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 0 , .y = 0 },.image = load_image("Images\\black_effect.png")};
    Add_obj(Scene , Create_node(&black_effect));
    Add_obj(Scene , Create_node(&Won_Label));
    bool shown = true;
    Map_Lights(Scene);
    Drawable *seek = Scene;
    while(seek){
        seek = seek->next;
    }
    Drawable *new_game_name = Create_node(&background_picture);
    Add_obj(new_game_name , Create_node(&Border));
    Add_obj(new_game_name , Create_node(&Back_btn));
    Add_obj(new_game_name , Create_node(&labeld));
    Add_obj(new_game_name , Create_node(&text_box_area));
    Add_obj(new_game_name , Create_node(&text_box_d));
    Add_obj(new_game_name , Create_node(&Play2));
    Drawable *head = Menu;
    bool Playing = true;
    bool is_typing = false;
    char typed_txt[100];
    typed_txt[0] = '\0';
    int cursor_pos = 0;
    bool _blink = true;
    int cmn=0;
    char *chosen_wall = "";
    char *Chosen_Player = "";
    char *state = "";
    int two_rund = 0;
    bool undod;
    undod = false;
    //----------------------------------
    Card *Suspect_cards;
    add_all_characters(&Suspect_cards);
    shuffle_cards(Suspect_cards);
    //---------------------------------
    Card *Mr_jack_susCards = NULL;
    Card *Detective_susCards = NULL;
    _pair first_lamp = {-1,-1};
    _pair second_lamp = {-1 , -1};
    int selected_lamp_cnt = 0;
    bool isGamePlaying;
    isGamePlaying = false;
    Find_Cell(Scene , 0 , 0)->obj->visible = false;
    Find_Cell(Scene , 12 , 0)->obj->visible = false;
    bool is_jack_turn = false;
    char *goodley_chosen = "";
    int steps = 0;
    int game_round = 1;
    int round_turn = 1;
    Card *First_half;
    Card *Second_half;
    Create_Cards(&First_half , &Second_half);
    int save_no = 1;
    while(keep_window_open)
    {
        Map_Lights(Scene);
        DrawStuff(&head , window_surface , light_eff , walk_able_eff);
        draw(window);
        clear_surface(window_surface);
        cmn++;
        if(cmn%30 == 0){
            _blink = !_blink;
            cmn = 0;
        }
        if(_blink){
            typed_txt[cursor_pos] = '_';
            typed_txt[cursor_pos+1] = '\0';
        }
        else{
            typed_txt[cursor_pos] = ' ';
            typed_txt[cursor_pos+1] = '\0';
        }
        SDL_Surface *tttt = TTF_RenderText_Solid(font_segoeui , typed_txt , fg);
        SDL_FreeSurface(text_box_d.image);
        text_box_d.image = tttt;
        if(SDL_PollEvent(&event) > 0)
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    keep_window_open = false;
                    break;
                case SDL_MOUSEMOTION:
                    mouse_pos.x = event.motion.x;
                    mouse_pos.y = event.motion.y;
                    Drawable *seek = head;
                    while(seek!=NULL){
                        if(chick(mouse_pos,seek)){
                            Play_voice_Thread(sounds_button3);
                        }
                        seek = seek->next;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
                case SDL_MOUSEBUTTONUP:
                    seek = head;
                    while(seek){
                        if((event.button.state != SDL_PRESSED)&&seek->obj->visible&&seek->obj->click_able&&(strcmp(convert_button_number_to_string(event.button.button) , "left")==0)&&is_mouse_inside_Surface(mouse_pos, seek->obj->pos)){
                            if(Search_by_tag(Scene , "blackeff")->obj->visible){
                                break;
                            }
                            Play_voice_Thread(sounds_button2);
                            if(strcmp(seek->obj->tag, "action")==0){
                                if(strcmp(Chosen_Player , "WG")==0){
                                    state = "WG_action";
                                    _action.visible = false;
                                    _move.visible = false;
                                    change_information_label(Scene , &information , "Select Player to switch with" , window_surface , light_eff , walk_able_eff , window,fg2);
                                }
                            }
                            else if(strcmp(seek->obj->tag , "move")==0){
                                if(strcmp(Chosen_Player , "WG")==0){
                                    state = "moving_player";
                                    _action.visible = false;
                                    _move.visible = false;
                                    walkable_character(Scene , "WG" , 3);
                                    change_information_label(Scene , &information , "Select the Cell to move" , window_surface , light_eff , walk_able_eff , window,fg2);
                                }
                            }
                            else if(strcmp(seek->obj->tag , "one")==0){
                                if(strcmp(Chosen_Player , "SG")==0){
                                    bool moved = move_character_in_shortest_path(Scene , goodley_chosen , 1 , window_surface, light_eff , walk_able_eff , window);
                                    if(moved){
                                        goodley_chosen = "";
                                        steps+=1;
                                        if(steps==3){
                                            steps = 0;
                                           if(two_rund==1){
                                                Chosen_Player = "";
                                                state = "";
                                                two_rund = 0;
                                                Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                            }
                                            else{
                                                two_rund++;
                                                walkable_character(Scene , "SG" , 3);
                                                state = "moving_player";
                                                change_information_label(Scene , &information , "Select the Cell to move" , window_surface , light_eff , walk_able_eff, window,fg2);
                                            }
                                        }
                                        else{
                                            goodley_chosen = "";
                                            change_information_label(Scene , &information , "Select Character to move",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }
                                    }
                                    else{
                                            goodley_chosen = "";
                                            change_information_label(Scene , &information , "Failed/Select character to move",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                    one_.visible = false;
                                    two_.visible = false;
                                    three_.visible = false;
                                }
                            }
                            else if(strcmp(seek->obj->tag , "two")==0){
                                if(strcmp(Chosen_Player , "SG")==0){
                                    bool moved = move_character_in_shortest_path(Scene , goodley_chosen , 2 , window_surface, light_eff , walk_able_eff , window);
                                    if(moved){
                                        steps+=2;
                                        goodley_chosen = "";
                                        if(steps==3){
                                            if(two_rund==1){
                                                Chosen_Player = "";
                                                state = "";
                                                two_rund = 0;
                                                Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                            }
                                            else{
                                                two_rund++;
                                                walkable_character(Scene , "SG" , 3);
                                                state = "moving_player";
                                                change_information_label(Scene , &information , "Select the Cell to move" , window_surface , light_eff , walk_able_eff, window,fg2);
                                            }
                                            steps = 0;

                                        }
                                        else{
                                            goodley_chosen = "";
                                            change_information_label(Scene , &information , "Select Character to move",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }
                                    }
                                    else{
                                        goodley_chosen = "";
                                        change_information_label(Scene , &information , "Failde/Select Character to move",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                    one_.visible = false;
                                    two_.visible = false;
                                    three_.visible = false;
                                }
                            }
                            else if(strcmp(seek->obj->tag , "three")==0){
                                if(strcmp(Chosen_Player , "SG")==0){
                                    bool moved = move_character_in_shortest_path(Scene , goodley_chosen , 3 , window_surface, light_eff , walk_able_eff , window) ;
                                    if(moved){
                                        steps+=3;
                                        if(steps==3){
                                            steps = 0;
                                           if(two_rund==1){
                                                Chosen_Player = "";
                                                state = "";
                                                two_rund = 0;
                                                Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                            }
                                            else{
                                                two_rund++;
                                                walkable_character(Scene , "SG" , 3);
                                                change_information_label(Scene , &information , "Select the Cell to move" , window_surface , light_eff , walk_able_eff, window,fg2);
                                                state = "moving_player";
                                            }
                                        }
                                        else{
                                            goodley_chosen = "";
                                            change_information_label(Scene , &information , "Select Character to move",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }
                                    }
                                    else{
                                        goodley_chosen = "";
                                        change_information_label(Scene , &information , "Failed/Select Character to move",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                    one_.visible = false;
                                    two_.visible = false;
                                    three_.visible = false;
                                }
                            }
                            if(strcmp(seek->obj->tag , "before")==0){
                                if(strcmp(Chosen_Player , "JS")==0){
                                    change_information_label(Scene , &information , "Select the Light" , window_surface , light_eff , walk_able_eff , window,fg2);
                                    state = "moving_light";
                                }
                                else if(strcmp(Chosen_Player , "SG")==0){
                                    state = "SG_action";
                                    change_information_label(Scene , &information , "Select the player" , window_surface , light_eff , walk_able_eff , window,fg2);
                                }
                                else if(strcmp(Chosen_Player , "JB")==0){
                                    change_information_label(Scene , &information , "Select the pithold" , window_surface , light_eff , walk_able_eff , window,fg2);
                                    state = "JB_action";
                                }
                                before.visible = false;
                                after.visible  = false;
                            }
                            else if (strcmp(seek->obj->tag , "after")==0){
                                if(strcmp(Chosen_Player , "JS")==0){
                                    state = "moving_player";
                                    walkable_character(Scene , "JS" , 3);
                                    change_information_label(Scene , &information , "Select the Cell to move" , window_surface , light_eff , walk_able_eff , window,fg2);
                                }
                                else if(strcmp(Chosen_Player , "SG")==0){
                                    state = "moving_player";
                                    walkable_character(Scene , "SG" , 3);
                                    change_information_label(Scene , &information , "Select the Cell to move" , window_surface , light_eff , walk_able_eff , window,fg2);
                                }
                                else if(strcmp(Chosen_Player , "JB")==0){
                                    change_information_label(Scene , &information , "Select the Cell to move" , window_surface , light_eff , walk_able_eff, window,fg2);
                                    walkable_character(Scene , "JB" , 3);
                                    state = "moving_player";
                                }
                                after.visible = false;
                                before.visible = false;
                            }
                            else if(strcmp(Chosen_Player, "JW")==0&&strcmp(state ,"change_dir")==0 && StartsWith(seek->obj->tag , "arrow_")){
                                change_watson_direction(Scene , seek->obj->tag[strlen(seek->obj->tag)-1]-'0');
                                arrow0.visible = false;
                                arrow1.visible = false;
                                arrow2.visible = false;
                                arrow3.visible = false;
                                arrow4.visible = false;
                                arrow5.visible = false;
                                Chosen_Player = "";
                                state = "";
                                Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                            }
                            if(strcmp(seek->obj->tag , "MapCell")==0){
                                if(is_mouse_inside_cell(mouse_pos , seek->obj->pos)){
                                int first = seek->obj->cell_info->cell_pos.first;
                                int seco = seek->obj->cell_info->cell_pos.second;
                                if(strcmp(Chosen_Player , "JS")==0&&strcmp(state ,"moving_light")==0){
                                    if(selected_lamp_cnt==0){
                                        first_lamp.first = first;
                                        first_lamp.second = seco;
                                        selected_lamp_cnt++;
                                        change_information_label(Scene , &information , "Select Place to move light to",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                    else{
                                        second_lamp.first = first;
                                        second_lamp.second = seco;
                                        bool is_moved = switch_lamps(first_lamp,second_lamp , Scene , window_surface , light_eff , walk_able_eff , window);
                                        if(is_moved){
                                            selected_lamp_cnt = 0;
                                            if(two_rund==1){
                                                Chosen_Player = "";
                                                state = "";
                                                two_rund = 0;
                                                Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                            }
                                            else{
                                                two_rund++;
                                                state = "moving_player";
                                                walkable_character(Scene , "JS" , 3);
                                                change_information_label(Scene , &information , "Select the Cell to move",window_surface , light_eff , walk_able_eff , window,fg2);
                                            }
                                        }
                                        else{
                                            selected_lamp_cnt = 0;
                                            change_information_label(Scene , &information , "Select the Light",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }

                                    }
                                }
                                if(strcmp(Chosen_Player , "SG")==0&&strcmp(state ,"SG_action")==0){
                                    char *clicked_on = get_clicked_character(Scene , first , seco);
                                    if(strcmp(clicked_on,"")==0){
                                        change_information_label(Scene , &information , "No character in this place",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                    else{
                                        goodley_chosen = clicked_on;
                                        change_information_label(Scene , &information , "How many cells u want to move?",window_surface , light_eff , walk_able_eff , window,fg2);
                                        if(steps==0){
                                            one_.visible = true;
                                            two_.visible = true;
                                            three_.visible = true;
                                        }
                                        if(steps==1){
                                            one_.visible = true;
                                            two_.visible = true;
                                        }
                                        if(steps==2){
                                            one_.visible = true;
                                        }

                                    }
                                }
                                 if(strcmp(Chosen_Player , "JB")==0&&strcmp(state ,"JB_action")==0){
                                    if(selected_lamp_cnt==0){
                                        first_lamp.first = first;
                                        first_lamp.second = seco;
                                        selected_lamp_cnt++;
                                        change_information_label(Scene , &information , "Select Place to move pit to",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                    else{
                                        second_lamp.first = first;
                                        second_lamp.second = seco;
                                        bool is_moved = switch_pits(first_lamp,second_lamp , Scene , window_surface , light_eff , walk_able_eff , window);
                                        if(is_moved){
                                            selected_lamp_cnt = 0;
                                            if(two_rund==1){
                                                Chosen_Player = "";
                                                state = "";
                                                two_rund = 0;
                                                Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);

                                            }
                                            else{
                                                two_rund++;
                                                state = "moving_player";
                                                walkable_character(Scene , "JB" , 3);
                                                change_information_label(Scene , &information , "Select the Cell to move",window_surface , light_eff , walk_able_eff , window,fg2);
                                            }
                                        }
                                        else{
                                            selected_lamp_cnt = 0;
                                            change_information_label(Scene , &information , "Select the pithold",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }
                                    }
                                }
                                else if(strcmp(Chosen_Player , "JB")==0&&strcmp(state ,"moving_player")==0){
                                    if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            if(is_cell_empty_wall(Scene , dest.first , dest.second)&&is_jack_turn){
                                                    if((strcmp("JB" , MrJacksName)==0))
                                                        Jack_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                    else
                                                        break;
                                            }
                                             if(is_cell_wall(dest.first , dest.second)){
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                                break;
                                            }

                                            bool moved = change_character_place(Scene , "JB" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                if(two_rund==1){
                                                    state = "";
                                                    Chosen_Player = "";
                                                    two_rund = 0;
                                                    Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                                }
                                                else{
                                                    two_rund++;
                                                    state = "JB_action";
                                                    change_information_label(Scene , &information , "Select pithold",window_surface , light_eff , walk_able_eff , window,fg2);
                                                }
                                            }
                                            else{
                                                char *tmpname = dest_char_name(Scene , dest.first , dest.second);
                                                if(strcmp(tmpname , MrJacksName)==0&&(!is_jack_turn)){
                                                    Detective_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff, window);
                                                }
                                                else if((!is_jack_turn) && strcmp(tmpname , MrJacksName)!=0){
                                                    Jack_Wins(Scene ,&Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                }
                                                else
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                }
                                else if(strcmp(Chosen_Player , "WG")==0&&strcmp(state ,"WG_action")==0){
                                    bool moved = switch_characters(Scene , "WG" , get_clicked_character(Scene , first , seco) , window_surface , light_eff , walk_able_eff , window);
                                    if(moved){
                                        Set_all_cells_unwalkable(Scene);
                                        state = "";
                                        Chosen_Player = "";
                                        Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                    }
                                    else{
                                        change_information_label(Scene , &information , "Select Character again",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                }
                                else if(strcmp(Chosen_Player , "WG")==0&&strcmp(state ,"moving_player")==0){
                                    if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            if(is_cell_empty_wall(Scene , dest.first , dest.second)&&is_jack_turn){
                                                    if((strcmp("WG" , MrJacksName)==0))
                                                        Jack_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                    else
                                                        break;
                                            }
                                             if(is_cell_wall(dest.first , dest.second)){
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                                break;
                                            }
                                            bool moved = change_character_place(Scene , "WG" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                state = "";
                                                Chosen_Player = "";
                                                Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                            }
                                           else{
                                                char *tmpname = dest_char_name(Scene , dest.first , dest.second);
                                                if(strcmp(tmpname , MrJacksName)==0&&(!is_jack_turn)){
                                                    Detective_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff, window);
                                                }
                                                else if((!is_jack_turn) && strcmp(tmpname , MrJacksName)!=0){
                                                    Jack_Wins(Scene ,&Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                }
                                                else
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                }
                                else if(strcmp(Chosen_Player , "JS")==0&&strcmp(state ,"moving_player")==0){
                                    if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            if(is_cell_empty_wall(Scene , dest.first , dest.second)&&is_jack_turn){
                                                    if((strcmp("JS" , MrJacksName)==0))
                                                        Jack_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                    else
                                                        break;
                                            }
                                             if(is_cell_wall(dest.first , dest.second)){
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                                break;
                                            }
                                            bool moved = change_character_place(Scene , "JS" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                if(two_rund==1){
                                                    state = "";
                                                    Chosen_Player = "";
                                                    two_rund = 0;
                                                    Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                                }
                                                else{
                                                    two_rund++;
                                                    state = "moving_light";
                                                    change_information_label(Scene , &information , "Select Light",window_surface , light_eff , walk_able_eff , window,fg2);
                                                }
                                            }
                                            else{
                                                char *tmpname = dest_char_name(Scene , dest.first , dest.second);
                                                if(strcmp(tmpname , MrJacksName)==0&&(!is_jack_turn)){
                                                    Detective_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff, window);
                                                }
                                                else if((!is_jack_turn) && strcmp(tmpname , MrJacksName)!=0){
                                                    Jack_Wins(Scene ,&Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                }
                                                else
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                }
                                else if(strcmp(Chosen_Player, "IL")==0&&strcmp(state ,"moving_player")==0){
                                        if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            if(is_cell_empty_wall(Scene , dest.first , dest.second)&&is_jack_turn){
                                                    if((strcmp("IL" , MrJacksName)==0))
                                                        Jack_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                    else
                                                        break;
                                            }
                                             if(is_cell_wall(dest.first , dest.second)){
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                                break;
                                            }
                                            bool moved = change_character_place(Scene , "IL" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                state = "moving_wall";
                                                change_information_label(Scene , &information , "Select Wall",window_surface , light_eff , walk_able_eff , window,fg2);
                                            }
                                            else{
                                                char *tmpname = dest_char_name(Scene , dest.first , dest.second);
                                                if(strcmp(tmpname , MrJacksName)==0&&(!is_jack_turn)){
                                                    Detective_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff, window);
                                                }
                                                else if((!is_jack_turn) && strcmp(tmpname , MrJacksName)!=0){
                                                    Jack_Wins(Scene ,&Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                }
                                                else
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                }
                                else if(strcmp(Chosen_Player, "SH")==0&&strcmp(state ,"moving_player")==0){
                                        if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            if(is_cell_empty_wall(Scene , dest.first , dest.second)&&is_jack_turn){
                                                    if((strcmp("SH" , MrJacksName)==0))
                                                        Jack_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                    else
                                                        break;
                                            }
                                            if(is_cell_wall(dest.first , dest.second)){
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                                break;
                                            }
                                            bool moved = change_character_place(Scene , "SH" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                state = "";
                                                Chosen_Player = "";
                                                if(Suspect_cards){
                                                    if(is_jack_turn){
                                                        if(Mr_jack_susCards==NULL){
                                                            Mr_jack_susCards = Create_card(Suspect_cards->name);
                                                        }
                                                        else{
                                                            append_card(Mr_jack_susCards , Suspect_cards->name);
                                                        }
                                                        burn_card(Scene , Suspect_cards->name);
                                                        remove_card(&Suspect_cards , Suspect_cards->name);
                                                    }
                                                    else{
                                                        if(Detective_susCards==NULL){
                                                            Detective_susCards = Create_card(Suspect_cards->name);
                                                        }
                                                        else{
                                                            append_card(Detective_susCards , Suspect_cards->name);
                                                        }
                                                        load_card(Scene,Suspect_cards->name);
                                                        remove_card(&Suspect_cards , Suspect_cards->name);
                                                    }
                                                    Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                                }
                                                else{
                                                    change_information_label(Scene , &information , "No Cards left/Select Character",window_surface , light_eff , walk_able_eff , window,fg2);
                                                }
                                            }
                                            else{
                                                char *tmpname = dest_char_name(Scene , dest.first , dest.second);
                                                if(strcmp(tmpname , MrJacksName)==0&&(!is_jack_turn)){
                                                    Detective_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff, window);
                                                }
                                                else if((!is_jack_turn) && strcmp(tmpname , MrJacksName)!=0){
                                                    Jack_Wins(Scene ,&Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                }
                                                else
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                }
                                else if(strcmp(Chosen_Player, "MS")==0&&strcmp(state ,"moving_player")==0){
                                        if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            if(is_cell_empty_wall(Scene , dest.first , dest.second)&&is_jack_turn){
                                                    if((strcmp("MS" , MrJacksName)==0))
                                                        Jack_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                    else
                                                        break;
                                            }
                                            if(is_cell_wall(dest.first , dest.second)){
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                                break;
                                            }
                                            bool moved = change_character_place(Scene , "MS" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                state = "";
                                                Chosen_Player = "";
                                                Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                            }
                                            else{
                                                char *tmpname = dest_char_name(Scene , dest.first , dest.second);
                                                if(strcmp(tmpname , MrJacksName)==0&&(!is_jack_turn)){
                                                    Detective_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff, window);
                                                }
                                                else if((!is_jack_turn) && strcmp(tmpname , MrJacksName)!=0){
                                                    Jack_Wins(Scene ,&Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                }
                                                else
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                }
                                else if(strcmp(Chosen_Player, "JW")==0&&strcmp(state ,"moving_player")==0){
                                        if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            if(is_cell_empty_wall(Scene , dest.first , dest.second)&&is_jack_turn){
                                                    if((strcmp("JW" , MrJacksName)==0))
                                                        Jack_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                    else
                                                        break;
                                            }
                                             if(is_cell_wall(dest.first , dest.second)){
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                                break;
                                            }
                                            bool moved = change_character_place(Scene , "JW" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                state = "change_dir";
                                                arrow0.visible = true;
                                                arrow1.visible = true;
                                                arrow2.visible = true;
                                                arrow3.visible = true;
                                                arrow4.visible = true;
                                                arrow5.visible = true;
                                                change_information_label(Scene , &information , "Select Direction : ",window_surface , light_eff , walk_able_eff , window,fg2);
                                            }
                                            else{
                                                char *tmpname = dest_char_name(Scene , dest.first , dest.second);
                                                if(strcmp(tmpname , MrJacksName)==0&&(!is_jack_turn)){
                                                    Detective_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff, window);
                                                }
                                                else if((!is_jack_turn) && strcmp(tmpname , MrJacksName)!=0){
                                                    Jack_Wins(Scene ,&Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                }
                                                else
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                }
                                else if(strcmp(Chosen_Player, "SG")==0&&strcmp(state ,"moving_player")==0){
                                        if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            if(is_cell_empty_wall(Scene , dest.first , dest.second)&&is_jack_turn){
                                                    if((strcmp("SG" , MrJacksName)==0))
                                                        Jack_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                    else
                                                        break;
                                            }
                                             if(is_cell_wall(dest.first , dest.second)){
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                                break;
                                            }
                                            bool moved = change_character_place(Scene , "SG" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                if(two_rund==1){
                                                    state = "";
                                                    Chosen_Player = "";
                                                    two_rund = 0;
                                                    Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                                }
                                                else{
                                                    two_rund++;
                                                    state = "SG_action";
                                                    change_information_label(Scene , &information , "Select Character to move",window_surface , light_eff , walk_able_eff , window,fg2);
                                                }
                                            }
                                            else{
                                                char *tmpname = dest_char_name(Scene , dest.first , dest.second);
                                                if(strcmp(tmpname , MrJacksName)==0&&(!is_jack_turn)){
                                                    Detective_Wins(Scene , &Won_Label , window_surface , light_eff , walk_able_eff, window);
                                                }
                                                else if((!is_jack_turn) && strcmp(tmpname , MrJacksName)!=0){
                                                    Jack_Wins(Scene ,&Won_Label , window_surface , light_eff , walk_able_eff , window);
                                                }
                                                else
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window,fg2);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window,fg2);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }
                                }
                                else if(strcmp(Chosen_Player, "IL")==0&&strcmp(state ,"moving_wall")==0){
                                    if(selected_lamp_cnt==0){
                                        first_lamp.first = first;
                                        first_lamp.second = seco;
                                        selected_lamp_cnt++;
                                        Drawable *cell = Find_Cell(Scene , first , seco);
                                        Drawable *wall_1 = Search_by_tag(Scene , "wall1");
                                        Drawable *wall_2 = Search_by_tag(Scene , "wall2");
                                        if(cell->obj->pos.x==wall_1->obj->pos.x && cell->obj->pos.y==wall_1->obj->pos.y){
                                            chosen_wall = "wall1";
                                        }
                                        else if(cell->obj->pos.x==wall_2->obj->pos.x && cell->obj->pos.y==wall_2->obj->pos.y){
                                            chosen_wall = "wall2";
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Select Wall again",window_surface , light_eff , walk_able_eff , window,fg2);
                                            continue;
                                        }
                                        change_information_label(Scene , &information , "Select Place to move wall to",window_surface , light_eff , walk_able_eff , window,fg2);
                                    }

                                    else{
                                        second_lamp.first = first;
                                        second_lamp.second = seco;
                                        bool is_moved = change_wall(Scene, chosen_wall , second_lamp , window_surface , light_eff , walk_able_eff , window);
                                        if(is_moved){
                                            selected_lamp_cnt = 0;
                                            Chosen_Player = "";
                                            state = "";
                                            chosen_wall = "";
                                            Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);

                                        }
                                        else{
                                            chosen_wall = "";
                                            change_information_label(Scene , &information , "Select the Wall",window_surface , light_eff , walk_able_eff , window,fg2);
                                            selected_lamp_cnt = 0;
                                        }
                                    }
                                }
                                }
                            }
                            if(strcmp(seek->obj->tag , "exit")==0)
                                keep_window_open = false;
                            else if(strcmp(seek->obj->tag , "mute")==0){
                                if(Playing){
                                    SDL_FreeSurface(seek->obj->image);
                                    seek->obj->image = load_image(mute_btn);
                                    Playing = false;
                                    Stop_Mixer();
                                }
                                else{
                                    SDL_FreeSurface(seek->obj->image);
                                    seek->obj->image = load_image(unmute_btn);
                                    Resume_Mixer();
                                    Playing = true;
                                }
                            }
                            else if(strcmp(seek->obj->tag , "telegram")==0){
                                send_jack_name(username, MrJacksName,typed_txt);
                            }
                            else if(strcmp(seek->obj->tag , "undo")==0){
                                if(true){
                                    save_no-=2;
                                    undod = true;
                                }
                                else{
                                    save_no-=1;
                                }
                                for(int i = 0; i<strlen(typed_txt);i++){
                                    if(typed_txt[i]==' ')
                                        typed_txt[i]='_';
                                }
                                if(typed_txt[strlen(typed_txt)-1]==' '||typed_txt[strlen(typed_txt)-1]=='_')
                                    typed_txt[strlen(typed_txt)-1]='\0';
                                char save_name[100];
                                char file_name[200];
                                char file_name2[200];
                                strcpy(save_name , typed_txt);
                                sprintf(file_name , "Saves\\%s_%d.save" , save_name , save_no);
                                sprintf(file_name2 , "del Saves\\%s_%d.save" , save_name , save_no+1);
                                system(file_name2);
                                set_new_move(false);
                                printf("loding : %s\n" , file_name);
                                load_game(file_name , &game_round , &round_turn , Scene , First_half , Second_half , &Detective_susCards, &Mr_jack_susCards , &Suspect_cards , &MrJacksName);
                                save_no++;
                                change_scene(&head , Scene);
                                sprintf(save_name , "%s_" , save_name);
                                strcpy(typed_txt , save_name);
                                cursor_pos = strlen(save_name)+1;
                                Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                            }
                            else if(strcmp(seek->obj->tag , "cmd")==0){
                                if(shown){
                                    ShowWindow(windowHandle,SW_HIDE);
                                    shown = false;
                                }
                                else if(!shown){
                                    ShowWindow(windowHandle,SW_SHOW);
                                    shown = true;
                                }
                            }
                            else if(strcmp(seek->obj->tag , "About Me")==0)
                                change_scene(&head , About);
                            else if(strcmp(seek->obj->tag , "Back")==0 || strcmp(seek->obj->tag , "Home")==0)
                                change_scene(&head , Menu);
                            else if(strcmp(seek->obj->tag , "Play")==0){
                                typed_txt[0] = '\0';
                                is_typing = true;
                                cursor_pos = 0;
                                change_scene(&head , new_game_name);
                            }
                            else if(strcmp(seek->obj->tag , "Play2") == 0){
                                is_typing = false;
                                for(int i = 0; i<strlen(typed_txt);i++){
                                    if(typed_txt[i]==' ')
                                        typed_txt[i]='_';
                                }
                                if(typed_txt[strlen(typed_txt)-1]==' '||typed_txt[strlen(typed_txt)-1]=='_')
                                    typed_txt[strlen(typed_txt)-1]='\0';
                                char save_name[100];
                                strcpy(save_name , typed_txt);
                                int last = get_last_save(save_name);
                                if(last == 0){
                                    MrJacksName = Choose_random_character();
                                    printf("Sending Jack's name ...");
                                    send_jack_name(username, MrJacksName,typed_txt);
                                    isGamePlaying = true;
                                    remove_card(&Suspect_cards , MrJacksName);
                                    change_scene(&head , Scene);
                                    burn_card(Scene , MrJacksName);
                                    save_no = 1;
                                    undod = false;
                                    Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                }
                                else{
                                    char file_name[200];
                                    sprintf(file_name , "Saves\\%s_%d.save" , save_name , last);
                                    set_new_move(false);
                                    load_game(file_name , &game_round , &round_turn , Scene , First_half , Second_half , &Detective_susCards, &Mr_jack_susCards , &Suspect_cards , &MrJacksName);
                                    save_no = last+1;
                                    change_scene(&head , Scene);
                                    sprintf(save_name , "%s_" , save_name);
                                    strcpy(typed_txt , save_name);
                                    cursor_pos = strlen(save_name)+1;
                                    Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                }


                            }
                        }
                        seek = seek->next;
                    }
                break;
            case SDL_KEYDOWN:
                 if(is_typing){
                        if(event.key.keysym.sym == SDLK_RETURN){
                                if(cursor_pos==0)
                                    break;
                                is_typing = false;
                                for(int i = 0; i<strlen(typed_txt);i++){
                                    if(typed_txt[i]==' ')
                                        typed_txt[i]='_';
                                }
                                if(typed_txt[strlen(typed_txt)-1]==' '||typed_txt[strlen(typed_txt)-1]=='_')
                                    typed_txt[strlen(typed_txt)-1]='\0';
                                char save_name[100];
                                strcpy(save_name , typed_txt);
                                int last = get_last_save(save_name);
                                if(last == 0){
                                    MrJacksName = Choose_random_character();
                                    printf("Sending Jack's name ...");
                                    send_jack_name(username, MrJacksName,typed_txt);
                                    isGamePlaying = true;
                                    burn_card(Scene , MrJacksName);
                                    remove_card(&Suspect_cards , MrJacksName);
                                    save_no = 1;
                                    Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                    undod = false;
                                    change_scene(&head , Scene);
                                }
                                else{
                                    char file_name[200];
                                    sprintf(file_name , "Saves\\%s_%d.save" , save_name , last);
                                    set_new_move(false);
                                    load_game(file_name , &game_round , &round_turn , Scene , First_half , Second_half , &Detective_susCards, &Mr_jack_susCards , &Suspect_cards , &MrJacksName);
                                    save_no = last+1;
                                    change_scene(&head , Scene);
                                    sprintf(save_name , "%s_" , save_name);
                                    strcpy(typed_txt , save_name);
                                    cursor_pos = strlen(save_name)+1;
                                    Choose_Next(Scene,Detective_susCards , Mr_jack_susCards,Suspect_cards , typed_txt , &save_no , MrJacksName , &state , &Chosen_Player , &information , &before , &after , &_move , &_action , &First_half , &Second_half , &game_round , &round_turn,&is_jack_turn , window_surface , light_eff , walk_able_eff , window,&round_bel , &turn_bel , &card_bel);
                                }

                        }
                        if(event.key.keysym.sym == SDLK_BACKSPACE){
                            if(cursor_pos>0){
                                typed_txt[cursor_pos-1] = '\0';
                                cursor_pos--;
                                if(cursor_pos == 0){
                                    Change_visibility(head , "Play2" , false);
                                    cursor_pos = 0;
                                }
                            }
                        }
                        else{
                            if(cursor_pos<20){
                                if((event.key.keysym.sym>='a' && event.key.keysym.sym<='z') || event.key.keysym.sym == SDLK_SPACE || (event.key.keysym.sym>='0'&&event.key.keysym.sym<='9')){
                                    if(cursor_pos==0){
                                        Change_visibility(head , "Play2" , true);
                                    }
                                    typed_txt[cursor_pos] = event.key.keysym.sym;
                                    typed_txt[cursor_pos+1] = '\0';
                                    cursor_pos++;
                                }
                            }
                        }
                    }

                    if(event.key.keysym.sym == SDLK_ESCAPE)
                        keep_window_open = false;
                    else if(event.key.keysym.sym == SDLK_DOWN){
                        if(Volume>0){
                            Volume-=2;
                            Mix_VolumeMusic(Volume);
                        }
                    }
                    else if(event.key.keysym.sym == SDLK_UP){
                        if(Volume<100){
                            Volume+=2;
                            Mix_VolumeMusic(Volume);
                        }
                    }
                break;
            }
        }
    }
    while(Scene->next){
        Scene = Scene->next;
        free(Scene->prev);
    }
    while(Menu->next){
        Menu = Menu->next;
        free(Menu->prev);
    }
    free(Menu);
    while(new_game_name->next){
        new_game_name = new_game_name->next;
        free(new_game_name->prev);
    }
    free(new_game_name);
    while(About->next){
        About = About->next;
        free(About->prev);
    }
    free(About);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(bgm);
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
    return 0;
}
bool move_character_in_shortest_path(Drawable *head , char *name, int l,SDL_Surface *window_surface , SDL_Surface *light_eff,SDL_Surface *walkable_effect,SDL_Window *window){
    Drawable *ff = Search_by_tag(head , name);
    Drawable *goodley = Search_by_tag(head , "SG");
    _pair lol = loc(head , ff->obj->pos.x-18 , ff->obj->pos.y-13);
    _pair lol2 = loc(head , goodley->obj->pos.x-18 , goodley->obj->pos.y-13);
    Drawable *ll = cell_under_character(head, "SG");
    walkable_character(head , name , l);
    unwalkable_character(head , name , l-1);
    Drawable *seek = head;
    Drawable *hadaf = NULL;
    int min_fasele = dist(head , lol.first , lol.second , lol2.first , lol2.second);
    while(seek){
        if(strcmp(seek->obj->tag , "MapCell")==0 ){
            if(seek->obj->cell_info->walk_able){
                int _dist = dist(head , (seek->obj->cell_info->cell_pos.first),(seek->obj->cell_info->cell_pos.second) , (ll->obj->cell_info->cell_pos.first) ,(ll->obj->cell_info->cell_pos.second));
                if(min_fasele>_dist){
                    if(!is_cell_full(head, seek->obj->pos.x+18 , seek->obj->pos.y+13)){
                        min_fasele = _dist;
                        hadaf = seek;
                    }
                }
            }
        }
        seek = seek->next;
    }
    Set_all_cells_unwalkable(head);
    if(!hadaf){
        return false;
    }
    _pair dest;
    dest.first = hadaf->obj->cell_info->cell_pos.first;
    dest.second = hadaf->obj->cell_info->cell_pos.second;
    if(is_cell_wall(dest.first , dest.second)){
        return false;
    }
    change_character_place(head , name , dest,window_surface , light_eff , walkable_effect , window);
    return true;
}
void walkable_character(Drawable *Scene , char *name, int l){
    Drawable *ff = Search_by_tag(Scene , name);
    _pair lol = loc(Scene , ff->obj->pos.x-18 , ff->obj->pos.y-13);
    Set_all_cells_unwalkable(Scene);
    calculate_walkable_cells(Scene,lol.first , lol.second , l);
    cell_under_character(Scene , name)->obj->cell_info->walk_able = false;
}
void unwalkable_character(Drawable *Scene , char *name, int l){
    Drawable *ff = Search_by_tag(Scene , name);
    _pair lol = loc(Scene , ff->obj->pos.x-18 , ff->obj->pos.y-13);
    calculate_walkable_cells_2(Scene,lol.first , lol.second , l);
}
void walkable_character_MS(Drawable *Scene , char *name, int l){
    Drawable *ff = Search_by_tag(Scene , name);
    _pair lol = loc(Scene , ff->obj->pos.x-18 , ff->obj->pos.y-13);
    Set_all_cells_unwalkable(Scene);
    calculate_walkable_cells_MS(Scene,lol.first , lol.second , l);
    cell_under_character(Scene , name)->obj->cell_info->walk_able = false;
    calculate_unwalkable_cells_MS(Scene);
}
