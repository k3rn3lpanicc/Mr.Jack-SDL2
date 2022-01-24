//SDL_WINDOW_ALWAYS_ON_TOP , SDL_WINDOW_INPUT_GRABBED , SDL_WINDOW_SHOWN
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
typedef SDL_Surface Image;
typedef SDL_Rect Position;
void walkable_character(Drawable *Scene , char *name, int l){
    Drawable *ff = Search_by_tag(Scene , name);
    _pair lol = loc(Scene , ff->obj->pos.x-18 , ff->obj->pos.y-13);
    Set_all_cells_unwalkable(Scene);
    calculate_walkable_cells(Scene,lol.first , lol.second , l);
    cell_under_character(Scene , name)->obj->cell_info->walk_able = false;
}
void walkable_character_MS(Drawable *Scene , char *name, int l){
    Drawable *ff = Search_by_tag(Scene , name);
    _pair lol = loc(Scene , ff->obj->pos.x-18 , ff->obj->pos.y-13);
    Set_all_cells_unwalkable(Scene);
    calculate_walkable_cells_MS(Scene,lol.first , lol.second , l);
    cell_under_character(Scene , name)->obj->cell_info->walk_able = false;
    calculate_unwalkable_cells_MS(Scene);
}

int main(int argc , char** argv){
    srand(time(0));
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char *MrJacksName = Choose_random_character();
    // --------- Hiding Console window from user
        HWND windowHandle = GetConsoleWindow();
        //ShowWindow(windowHandle,SW_HIDE);
    // ---------------------------------------
    SDL_Event event;
    Init();
    font_segoeui = TTF_OpenFont("segoeui.ttf" , 20);
    font_segoeui_big = TTF_OpenFont("segoeui.ttf" , 30);
    font_consolas = TTF_OpenFont("consolas.ttf" , 18);
    int Volume = 100;
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
    int CX=1260 , CY=8;
    int ddx , ddy;
    ddx = 12;
    ddy = 10;

    _clickable Goodley={.tag= "Goodley",.click_able = false,.entered = false,.do_chick = false,.visible = true,.pos = { .x = CX , .y = CY},.image = load_image(unknown_card)};
    _clickable Sherlock={.tag = "Sherlock" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = CX+98+ddx , .y = CY },.image = load_image(unknown_card)};
    _clickable Lestrade={.tag= "Lestrade",.click_able = false,.entered = false,.do_chick = false,.visible = true,.pos = { .x = CX , .y = CY+156+ddy},.image = load_image(unknown_card)};
    _clickable Jermy={.tag = "Jermy" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = CX+98+ddx , .y = CY+156+ddy },.image = load_image(unknown_card)};
    _clickable Smith={.tag = "Smith" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = CX , .y = CY+(156+ddy)*2 },.image = load_image(unknown_card)};
    _clickable Watson={.tag = "Watson" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = CX+98+ddx , .y = CY+(156+ddy)*2 },.image = load_image(unknown_card)};
    _clickable Stealthy={.tag = "Stealthy" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = CX , .y = CY+(156+ddy)*3 },.image = load_image(unknown_card)};
    _clickable Gull={.tag = "Gull" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = CX+98+ddx , .y = CY+(156+ddy)*3 },.image = load_image(unknown_card)};
    //----------InitializeMenu Items ------
    int __x = 585;
    int __y = 215;
    _clickable Border = {.tag = "Border" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x-35 , .y = __y-75},.image = load_image(border_pic)};
    _clickable Play = {.tag = "Play" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y},.image = load_image(btn_play)};
    _clickable Exit = {.tag = "exit" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y+270 },.image = load_image(btn_exit)};
    _clickable Resume = {.tag = "Resume" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y+90 },.image = load_image(btn_resume)};
    _clickable About_me = {.tag = "About Me" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y+180 },.image = load_image(btn_about)};
    //-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=
    _clickable mum_btn = {.tag = "mute" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 10 , .y = 10 },.image = load_image(unmute_btn)};
    _clickable background_picture = {.tag = "bground" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 0 , .y = 0 },.image = load_image(bg_pic)};
    _clickable background_picture2 = {.tag = "bground2" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 0 , .y = 0 },.image = load_image(bg_pic2)};
    //------------Aboutme Page Items-------
    _clickable Back_btn = {.tag = "Back" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y+300 },.image = load_image(btn_back)};
    _clickable About_Label = {.tag = "About_me" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x-10 , .y = __y-30 },.image = load_image(about_me)};
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    //=-----------Game Scene Items ------=
    _clickable Home = {.tag = "Home" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 800 },.image = load_image(home)};
    // -------------------------------------
    bool keep_window_open = true;
    _pos mouse_pos;
    TTF_SetFontStyle(font_segoeui ,TTF_STYLE_ITALIC|TTF_STYLE_BOLD|TTF_STYLE_UNDERLINE);
    SDL_Surface *text_surface = TTF_RenderText_Solid(font_segoeui , "Type your save's Name : ",fg);
    TTF_SetFontStyle(font_segoeui ,TTF_STYLE_BOLD);
    SDL_Surface *text_box = TTF_RenderText_Solid(font_consolas , "_",fg);
    SDL_Surface *enter_game = TTF_RenderText_Solid(font_consolas , "Save and Continue",fg2);
    SDL_Surface *information_label = TTF_RenderText_Solid(font_consolas , "Choose the Player",fg2);
    SDL_Surface *before_label = TTF_RenderText_Solid(font_consolas , "Before",fg2);
    SDL_Surface *after_label = TTF_RenderText_Solid(font_consolas , "After",fg2);
    SDL_Surface *arrow_0 = TTF_RenderText_Solid(font_consolas , "Up",fg2);
    SDL_Surface *arrow_1 = TTF_RenderText_Solid(font_consolas , "UR",fg2);
    SDL_Surface *arrow_2 = TTF_RenderText_Solid(font_consolas , "DR",fg2);
    SDL_Surface *arrow_3 = TTF_RenderText_Solid(font_consolas , "Down",fg2);
    SDL_Surface *arrow_4 = TTF_RenderText_Solid(font_consolas , "DL",fg2);
    SDL_Surface *arrow_5 = TTF_RenderText_Solid(font_consolas , "UL",fg2);
    SDL_Surface *move = TTF_RenderText_Solid(font_consolas , "move",fg2);
    SDL_Surface *action = TTF_RenderText_Solid(font_consolas , "action",fg2);
    _clickable labeld = {.tag = "dd" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x , .y = __y-35 },.image = text_surface};
    _clickable text_box_d = {.tag = "text_box" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x , .y = __y+35 },.image = text_box};
    _clickable text_box_area = {.tag = "text_boxa" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x-5 , .y = __y+27 },.image = load_image("Images\\text_box.png")};
    _clickable Play2 = {.tag = "Play2" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = __x , .y = __y+250 },.image = enter_game};
    _clickable information = {.tag = "information" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1100 , .y = 800 },.image = information_label};
    _clickable before = {.tag = "before" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = 1100 , .y = 820 },.image = before_label};
    _clickable after = {.tag = "after" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = 1170 , .y = 820 },.image = after_label};
    _clickable _move = {.tag = "move" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = 1100 , .y = 820 },.image = move};
    _clickable _action = {.tag = "action" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = 1170 , .y = 820 },.image = action};
    int xx , yy;
    xx = 1350;
    yy = 760;
    _clickable arrow0 = {.tag = "arrow_0" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = xx , .y = yy-60 },.image = arrow_0};
    _clickable arrow1 = {.tag = "arrow_1" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = xx+50 , .y = yy-30},.image = arrow_1};
    _clickable arrow2 = {.tag = "arrow_2" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = xx+50 , .y = yy+30 },.image = arrow_2};
    _clickable arrow3 = {.tag = "arrow_3" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = xx-10 , .y = yy+60 },.image = arrow_3};
    _clickable arrow4 = {.tag = "arrow_4" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = xx-50 , .y = yy+30 },.image = arrow_4};
    _clickable arrow5 = {.tag = "arrow_5" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = xx-50 , .y = yy-30 },.image = arrow_5};

    //-------------------------------------------------------

    SDL_Surface *light_eff = load_image(light_effect);
    SDL_Surface *walk_able_eff = load_image("Images\\Blocks\\walkable.png");
    //----------------------------------------------------
    Drawable *Menu = Create_node(&background_picture);
    Add_obj(Menu , Create_node(&Border));
    Add_obj(Menu , Create_node(&Play));
    Add_obj(Menu , Create_node(&Exit));
    Add_obj(Menu , Create_node(&Resume));
    Add_obj(Menu , Create_node(&About_me));
    Add_obj(Menu , Create_node(&mum_btn));
    //----------------------------------------------------
    Drawable *About = Create_node(&background_picture);
    Add_obj(About , Create_node(&Border));
    Add_obj(About , Create_node(&Back_btn));
    Add_obj(About , Create_node(&mum_btn));
    Add_obj(About , Create_node(&About_Label));
    //---------------------------------------------------
    Drawable *Scene=Create_node(&background_picture2);
    Draw_map(Scene,50,10);
    Add_obj(Scene , Create_node(&Home));
    Add_obj(Scene , Create_node(&mum_btn));
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


    Map_Lights(Scene);
    Drawable *seek = Scene;
    while(seek){
        seek = seek->next;
    }
    Drawable *new_game_name = Create_node(&background_picture);
    Add_obj(new_game_name , Create_node(&Border));
    Add_obj(new_game_name , Create_node(&Back_btn));
    Add_obj(new_game_name , Create_node(&mum_btn));
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
    bool Choosing_Player = false;
    char *state = "";
    int two_rund = 0;
    //----------------------------------
    Card *Suspect_cards;
    add_all_characters(&Suspect_cards);
    shuffle_cards(Suspect_cards);
    printf("Suspect Cards : \n");
    print_cards(Suspect_cards);
    //---------------------------------
    Card *Mr_jack_susCards = NULL;
    Card *Detective_susCards = NULL;

    _pair first_lamp = {-1,-1};
    _pair second_lamp = {-1 , -1};
    int selected_lamp_cnt = 0;

    bool isGamePlaying = false;

    Find_Cell(Scene , 0 , 0)->obj->visible = false;
    Find_Cell(Scene , 12 , 0)->obj->visible = false;

    bool is_jack_turn = true;

    while(keep_window_open)
    {
        Map_Lights(Scene);
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
                    printf("Pos : (%d , %d) \n" , mouse_pos.x , mouse_pos.y);
                    seek = head;
                    while(seek){
                        ///clicking bug
                        if((event.button.state != SDL_PRESSED)&&seek->obj->visible&&seek->obj->click_able&&(strcmp(convert_button_number_to_string(event.button.button) , "left")==0)&&is_mouse_inside_Surface(mouse_pos, seek->obj->pos)){
                            Play_voice_Thread(sounds_button2);
                            if(strcmp(seek->obj->tag, "action")==0){
                                if(strcmp(Chosen_Player , "WG")==0){
                                    state = "WG_action";
                                    _action.visible = false;
                                    _move.visible = false;
                                    change_information_label(Scene , &information , "Select Player to switch with" , window_surface , light_eff , walk_able_eff , window);
                                }
                            }
                            else if(strcmp(seek->obj->tag , "move")==0){
                                if(strcmp(Chosen_Player , "WG")==0){
                                    state = "moving_player";
                                    _action.visible = false;
                                    _move.visible = false;
                                    walkable_character(Scene , "WG" , 3);
                                    change_information_label(Scene , &information , "Select the Cell to move" , window_surface , light_eff , walk_able_eff , window);
                                }
                            }
                            if(strcmp(seek->obj->tag , "before")==0){
                                if(strcmp(Chosen_Player , "JS")==0){
                                    change_information_label(Scene , &information , "Select the Light" , window_surface , light_eff , walk_able_eff , window);
                                    state = "moving_light";
                                }
                                else if(strcmp(Chosen_Player , "SG")==0){
                                    //TODO
                                }
                                else if(strcmp(Chosen_Player , "JB")==0){
                                    change_information_label(Scene , &information , "Select the pithold" , window_surface , light_eff , walk_able_eff , window);
                                    state = "JB_action";
                                }
                                before.visible = false;
                                after.visible  = false;
                            }
                            else if (strcmp(seek->obj->tag , "after")==0){
                                if(strcmp(Chosen_Player , "JS")==0){
                                    state = "moving_player";
                                    walkable_character(Scene , "JS" , 3);
                                    change_information_label(Scene , &information , "Select the Cell to move" , window_surface , light_eff , walk_able_eff , window);
                                }
                                else if(strcmp(Chosen_Player , "SG")==0){
                                    //TODO
                                }
                                else if(strcmp(Chosen_Player , "JB")==0){
                                    change_information_label(Scene , &information , "Select the Cell to move" , window_surface , light_eff , walk_able_eff, window);
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
                                Choosing_Player = true;
                                Chosen_Player = "";
                                state = "";
                                change_information_label(Scene , &information , "Select Character",window_surface , light_eff , walk_able_eff , window);
                            }
                            if(strcmp(seek->obj->tag , "MapCell")==0){
                                if(is_mouse_inside_cell(mouse_pos , seek->obj->pos)){
                                printf("Clicked on : %d, %d\n" , seek->obj->cell_info->cell_pos.first,seek->obj->cell_info->cell_pos.second);
                                int first = seek->obj->cell_info->cell_pos.first;
                                int seco = seek->obj->cell_info->cell_pos.second;
                                if(Choosing_Player){
                                    char *clicked_on = get_clicked_character(Scene , first , seco);
                                    if(strcmp(clicked_on,"")==0){
                                        change_information_label(Scene , &information , "No character in this place",window_surface , light_eff , walk_able_eff , window);
                                    }
                                    else{
                                        Chosen_Player = clicked_on;
                                        printf("Chosen_Player : %s\n" , Chosen_Player);
                                        if(strcmp(Chosen_Player , "JS")==0){
                                            change_information_label(Scene , &information , "Move lights before or after ? ",window_surface , light_eff , walk_able_eff , window);
                                            before.visible = true;
                                            after.visible = true;
                                        }
                                        if(strcmp(Chosen_Player , "JB")==0){
                                            change_information_label(Scene , &information , "Move pit before or after ? ",window_surface , light_eff , walk_able_eff , window);
                                            before.visible = true;
                                            after.visible = true;
                                        }
                                        if(strcmp(Chosen_Player , "WG")==0){
                                            change_information_label(Scene , &information , "Use Action or move ?",window_surface , light_eff , walk_able_eff , window);
                                            _action.visible = true;
                                            _move.visible = true;

                                            state = "action_move";

                                        }

                                        if(strcmp(Chosen_Player , "IL")==0){
                                            state = "moving_player";
                                            walkable_character(Scene , "IL" , 3);
                                            change_information_label(Scene , &information , "Select Cell to Move",window_surface , light_eff , walk_able_eff , window);
                                        }
                                        if(strcmp(Chosen_Player , "SH")==0){
                                            state = "moving_player";
                                            walkable_character(Scene , "SH" , 3);
                                            change_information_label(Scene , &information , "Select Cell to Move",window_surface , light_eff , walk_able_eff , window);
                                        }
                                        if(strcmp(Chosen_Player , "JW")==0){
                                            state = "moving_player";
                                            walkable_character(Scene , "JW" , 3);
                                            change_information_label(Scene , &information , "Select Cell to Move",window_surface , light_eff , walk_able_eff , window);
                                        }
                                        if(strcmp(Chosen_Player , "MS")==0){
                                            state = "moving_player";
                                            walkable_character_MS(Scene , "MS" , 4);
                                            change_information_label(Scene , &information , "Select Cell to Move",window_surface , light_eff , walk_able_eff , window);
                                        }

                                        Choosing_Player = false;
                                    }
                                }
                                if(strcmp(Chosen_Player , "JS")==0&&strcmp(state ,"moving_light")==0){
                                    if(selected_lamp_cnt==0){
                                        first_lamp.first = first;
                                        first_lamp.second = seco;
                                        selected_lamp_cnt++;
                                        change_information_label(Scene , &information , "Select Place to move light to",window_surface , light_eff , walk_able_eff , window);
                                    }
                                    else{
                                        second_lamp.first = first;
                                        second_lamp.second = seco;
                                        bool is_moved = switch_lamps(first_lamp,second_lamp , Scene , window_surface , light_eff , walk_able_eff , window);
                                        if(is_moved){
                                            selected_lamp_cnt = 0;
                                            if(two_rund==1){
                                                Chosen_Player = "";
                                                Choosing_Player = true;
                                                state = "";
                                                two_rund = 0;
                                                change_information_label(Scene , &information , "Select Character",window_surface , light_eff , walk_able_eff , window);
                                            }
                                            else{
                                                two_rund++;
                                                state = "moving_player";
                                                walkable_character(Scene , "JS" , 3);
                                                change_information_label(Scene , &information , "Select the Cell to move",window_surface , light_eff , walk_able_eff , window);
                                            }
                                        }
                                        else{
                                            selected_lamp_cnt = 0;
                                            change_information_label(Scene , &information , "Select the Light",window_surface , light_eff , walk_able_eff , window);
                                        }

                                    }
                                }

                                 if(strcmp(Chosen_Player , "JB")==0&&strcmp(state ,"JB_action")==0){
                                    if(selected_lamp_cnt==0){
                                        first_lamp.first = first;
                                        first_lamp.second = seco;
                                        selected_lamp_cnt++;
                                        change_information_label(Scene , &information , "Select Place to move pit to",window_surface , light_eff , walk_able_eff , window);
                                    }
                                    else{
                                        second_lamp.first = first;
                                        second_lamp.second = seco;
                                        bool is_moved = switch_pits(first_lamp,second_lamp , Scene , window_surface , light_eff , walk_able_eff , window);
                                        if(is_moved){
                                            selected_lamp_cnt = 0;
                                            if(two_rund==1){
                                                Chosen_Player = "";
                                                Choosing_Player = true;
                                                state = "";
                                                two_rund = 0;
                                                change_information_label(Scene , &information , "Select Character",window_surface , light_eff , walk_able_eff , window);
                                            }
                                            else{
                                                two_rund++;
                                                state = "moving_player";
                                                walkable_character(Scene , "JB" , 3);
                                                change_information_label(Scene , &information , "Select the Cell to move",window_surface , light_eff , walk_able_eff , window);
                                            }
                                        }
                                        else{
                                            selected_lamp_cnt = 0;
                                            change_information_label(Scene , &information , "Select the pithold",window_surface , light_eff , walk_able_eff , window);
                                        }
                                    }
                                }
                                else if(strcmp(Chosen_Player , "JB")==0&&strcmp(state ,"moving_player")==0){
                                    if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            bool moved = change_character_place(Scene , "JB" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                if(two_rund==1){
                                                    state = "";
                                                    Chosen_Player = "";
                                                    Choosing_Player = true;
                                                    change_information_label(Scene , &information , "Select Character",window_surface , light_eff , walk_able_eff , window);
                                                    two_rund = 0;
                                                }
                                                else{
                                                    two_rund++;
                                                    state = "JB_action";
                                                    change_information_label(Scene , &information , "Select pithold",window_surface , light_eff , walk_able_eff , window);
                                                }
                                            }
                                            else{
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window);
                                    }
                                }
                                else if(strcmp(Chosen_Player , "WG")==0&&strcmp(state ,"WG_action")==0){
                                    bool moved = switch_characters(Scene , "WG" , get_clicked_character(Scene , first , seco) , window_surface , light_eff , walk_able_eff , window);
                                    if(moved){
                                        Set_all_cells_unwalkable(Scene);
                                        state = "";
                                        Chosen_Player = "";
                                        Choosing_Player = true;
                                        change_information_label(Scene , &information , "Select Character",window_surface , light_eff , walk_able_eff , window);
                                    }
                                    else{
                                        change_information_label(Scene , &information , "Select Character again",window_surface , light_eff , walk_able_eff , window);
                                    }
                                }

                                else if(strcmp(Chosen_Player , "WG")==0&&strcmp(state ,"moving_player")==0){
                                    if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            bool moved = change_character_place(Scene , "WG" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                state = "";
                                                Chosen_Player = "";
                                                Choosing_Player = true;
                                                change_information_label(Scene , &information , "Select Character",window_surface , light_eff , walk_able_eff , window);
                                            }
                                            else{
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window);
                                            }

                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window);
                                    }
                                }
                                else if(strcmp(Chosen_Player , "JS")==0&&strcmp(state ,"moving_player")==0){
                                    if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            bool moved = change_character_place(Scene , "JS" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                if(two_rund==1){
                                                    state = "";
                                                    Chosen_Player = "";
                                                    Choosing_Player = true;
                                                    change_information_label(Scene , &information , "Select Character",window_surface , light_eff , walk_able_eff , window);
                                                    two_rund = 0;
                                                }
                                                else{
                                                    two_rund++;
                                                    state = "moving_light";
                                                    change_information_label(Scene , &information , "Select Light",window_surface , light_eff , walk_able_eff , window);
                                                }
                                            }
                                            else{
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window);
                                    }

                                }
                                else if(strcmp(Chosen_Player, "IL")==0&&strcmp(state ,"moving_player")==0){
                                        if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            bool moved = change_character_place(Scene , "IL" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                state = "moving_wall";
                                                change_information_label(Scene , &information , "Select Wall",window_surface , light_eff , walk_able_eff , window);
                                            }
                                            else{
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window);
                                    }
                                }
                                else if(strcmp(Chosen_Player, "SH")==0&&strcmp(state ,"moving_player")==0){
                                        if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            bool moved = change_character_place(Scene , "SH" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                state = "";
                                                Chosen_Player = "";
                                                Choosing_Player = true;
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
                                                change_information_label(Scene , &information , "Select Character",window_surface , light_eff , walk_able_eff , window);
                                                }
                                                else{
                                                    change_information_label(Scene , &information , "No Cards left/Select Character",window_surface , light_eff , walk_able_eff , window);
                                                }
                                                printf("Cards Jack picked : \n");
                                                print_cards(Mr_jack_susCards);
                                                printf("\nCards Detective picked : \n");
                                                print_cards(Detective_susCards);
                                            }
                                            else{
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window);
                                    }
                                }
                                else if(strcmp(Chosen_Player, "MS")==0&&strcmp(state ,"moving_player")==0){
                                        if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
                                            bool moved = change_character_place(Scene , "MS" , dest , window_surface , light_eff , walk_able_eff , window);
                                            if(moved){
                                                Set_all_cells_unwalkable(Scene);
                                                state = "";
                                                Chosen_Player = "";
                                                Choosing_Player = true;
                                                change_information_label(Scene , &information , "Select Character",window_surface , light_eff , walk_able_eff , window);
                                            }
                                            else{
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window);
                                    }
                                }
                                else if(strcmp(Chosen_Player, "JW")==0&&strcmp(state ,"moving_player")==0){
                                        if(!is_cell_full(Scene,seek->obj->pos.x+18 , seek->obj->pos.x+13)){
                                        _pair dest = {first , seco};
                                        if(Find_Cell(Scene , first , seco)->obj->cell_info->walk_able){
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
                                                change_information_label(Scene , &information , "Select Direction : ",window_surface , light_eff , walk_able_eff , window);
                                            }
                                            else{
                                                change_information_label(Scene , &information , "Full/Select again",window_surface , light_eff , walk_able_eff , window);
                                            }
                                        }
                                        else{
                                            change_information_label(Scene , &information , "Selected Block is not walkable",window_surface , light_eff , walk_able_eff , window);
                                        }
                                    }
                                    else{
                                        change_information_label(Scene , &information , "The Destination Cell can't be full",window_surface , light_eff , walk_able_eff , window);
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
                                                change_information_label(Scene , &information , "Select Wall again",window_surface , light_eff , walk_able_eff , window);
                                                continue;
                                            }
                                            printf("selected wall : %s\n" , chosen_wall);
                                            change_information_label(Scene , &information , "Select Place to move wall to",window_surface , light_eff , walk_able_eff , window);
                                        }
                                        else{
                                            second_lamp.first = first;
                                            second_lamp.second = seco;
                                            bool is_moved = change_wall(Scene, chosen_wall , second_lamp , window_surface , light_eff , walk_able_eff , window);
                                            if(is_moved){
                                                selected_lamp_cnt = 0;
                                                Chosen_Player = "";
                                                Choosing_Player = true;
                                                state = "";
                                                chosen_wall = "";
                                                change_information_label(Scene , &information , "Select Character",window_surface , light_eff , walk_able_eff , window);
                                            }
                                            else{
                                                chosen_wall = "";
                                                change_information_label(Scene , &information , "Select the Wall",window_surface , light_eff , walk_able_eff , window);
                                                selected_lamp_cnt = 0;
                                            }
                                        }
                                }
                                printf("%d,%d\n" , first , seco);
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
                                MrJacksName = Choose_random_character();
                                SetConsoleTextAttribute(hConsole, 207);
                                printf("    ");
                                SetConsoleTextAttribute(hConsole, 12);
                                printf(" Mr.Jack in this round is : " );
                                SetConsoleTextAttribute(hConsole, 10);
                                printf("%s " , MrJacksName);
                                burn_card(Scene , MrJacksName);
                                SetConsoleTextAttribute(hConsole, 207);
                                printf("    ");
                                SetConsoleTextAttribute(hConsole, 15);
                                isGamePlaying = true;
                                Choosing_Player = true;
                                remove_card(&Suspect_cards , MrJacksName);
                                //TODO:we need to initialize the suspect cards here
                                change_scene(&head , Scene);
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
                                MrJacksName = Choose_random_character();
                                SetConsoleTextAttribute(hConsole, 207);
                                printf("    ");
                                SetConsoleTextAttribute(hConsole, 12);
                                printf(" Mr.Jack in this round is : " );
                                SetConsoleTextAttribute(hConsole, 10);
                                printf("%s " , MrJacksName);
                                SetConsoleTextAttribute(hConsole, 207);
                                printf("    ");
                                SetConsoleTextAttribute(hConsole, 15);
                                isGamePlaying = true;
                                Choosing_Player = true;
                                burn_card(Scene , MrJacksName);
                                remove_card(&Suspect_cards , MrJacksName);
                                change_scene(&head , Scene);
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
                    else if(event.key.keysym.sym == SDLK_s){
                        //ShowWindow(windowHandle,SW_SHOW);
                    }
                    else if(event.key.keysym.sym == SDLK_h){
                       // ShowWindow(windowHandle,SW_HIDE);
                    }
                break;
            }
        }
        DrawStuff(&head , window_surface , light_eff , walk_able_eff); // Draw Stuff on buffer
        draw(window); //actually draw them on screen (update Screen)
        clear_surface(window_surface); //clear after that
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
