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
    _clickable Goodley={.tag= "Goodley",.click_able = false,.entered = false,.do_chick = false,.visible = true,.pos = { .x = 950 , .y = 10},.image = load_image(In_Goodley)};
    _clickable Sherlock={.tag = "Sherlock" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1100 , .y = 10 },.image = load_image(In_Sherlock)};
    _clickable Lestrade={.tag= "Lestrade",.click_able = false,.entered = false,.do_chick = false,.visible = true,.pos = { .x = 950 , .y = 260},.image = load_image(In_Lestrade)};
    _clickable Jermy={.tag = "Jermy" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1100 , .y = 260 },.image = load_image(In_Jermy)};
    _clickable Smith={.tag = "Smith" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 950 , .y = 500 },.image = load_image(In_John_Smith)};
    _clickable Watson={.tag = "Watson" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1100 , .y = 500 },.image = load_image(In_John_Watson)};
    _clickable Stealthy={.tag = "Stealthy" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 950 , .y = 740 },.image = load_image(In_Miss_Stealthy)};
    _clickable Gull={.tag = "Gull" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1100 , .y = 740 },.image = load_image(In_William_Gull)};
    //---------------------------------------

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

    //char *tx;
    //sprintf(tx , "Mr.Jack is : %s" ,MrJacksName);
    //SDL_Surface *Jack_label = TTF_RenderText_Solid(font_segoeui_big ,tx,fg);

    _clickable labeld = {.tag = "dd" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x , .y = __y-35 },.image = text_surface};
    _clickable text_box_d = {.tag = "text_box" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x , .y = __y+35 },.image = text_box};
    _clickable text_box_area = {.tag = "text_boxa" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x-5 , .y = __y+27 },.image = load_image("Images\\text_box.png")};
    _clickable Play2 = {.tag = "Play2" ,.click_able = true, .entered = false,.do_chick = true,.visible = false,.pos = { .x = __x , .y = __y+250 },.image = enter_game};

    //-------------------------------------------------------

    SDL_Surface *light_eff = load_image(light_effect);
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
    cell->obj->do_chick = false;
    cell->obj->click_able = true;
    Add_obj(Scene , Create_node(&JW));

    cell = Find_Cell(Scene ,4,10);
    _clickable IL = {.tag = "IL" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos ={cell->obj->pos.x+18,cell->obj->pos.y+13},.image = load_image("Images\\Characters\\Lestrade.png")};
    IL.cell_info = cell->obj->cell_info;
    cell->obj->do_chick = false;
    cell->obj->click_able = true;
    Add_obj(Scene , Create_node(&IL));


    _clickable JS = {.tag = "JS" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\John_Smith.png")};
    cell = Find_Cell(Scene ,4,8);
    JS.pos.x = cell->obj->pos.x+18;
    JS.pos.y = cell->obj->pos.y+13;
    JS.cell_info = cell->obj->cell_info;
    cell->obj->do_chick = false;
    cell->obj->click_able = true;
    Add_obj(Scene , Create_node(&JS));

    _clickable WG = {.tag = "WG" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\Gull.png")};
    cell = Find_Cell(Scene ,4,2);
    WG.pos.x = cell->obj->pos.x+18;
    WG.pos.y = cell->obj->pos.y+13;
    WG.cell_info = cell->obj->cell_info;
    cell->obj->do_chick = false;
    cell->obj->click_able = true;
    Add_obj(Scene , Create_node(&WG));

    _clickable JB = {.tag = "JB" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\Bert.png")};
    cell = Find_Cell(Scene ,8,8);
    JB.pos.x = cell->obj->pos.x+18;
    JB.pos.y = cell->obj->pos.y+13;
    JB.cell_info = cell->obj->cell_info;
    cell->obj->do_chick = false;
    cell->obj->click_able = true;
    Add_obj(Scene , Create_node(&JB));

    _clickable SG = {.tag = "SG" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\goodley.png")};
    cell = Find_Cell(Scene ,12,8);
    SG.pos.x = cell->obj->pos.x+18;
    SG.pos.y = cell->obj->pos.y+13;
    SG.cell_info = cell->obj->cell_info;
    cell->obj->do_chick = false;
    cell->obj->click_able = true;
    Add_obj(Scene , Create_node(&SG));

    _clickable MS = {.tag = "MS" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\Stealthy.png")};
    cell = Find_Cell(Scene ,0,10);
    MS.pos.x = cell->obj->pos.x+18;
    MS.pos.y = cell->obj->pos.y+13;
    MS.cell_info = cell->obj->cell_info;
    cell->obj->do_chick = false;
    cell->obj->click_able = true;
    Add_obj(Scene , Create_node(&MS));

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
    int cntt = 0;
    char *Chosen_Player = "";
    //----------------------------------
    Card *Suspect_cards;
    add_all_characters(&Suspect_cards);
    printf("Suspect Cards : \n");
    print_cards(Suspect_cards);
    //---------------------------------
    bool isGamePlaying = false;
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
                        if((event.button.state != SDL_PRESSED)&&seek->obj->click_able&&(strcmp(convert_button_number_to_string(event.button.button) , "left")==0)&&is_mouse_inside_Surface(mouse_pos, seek->obj->pos)){
                            Play_voice_Thread(sounds_button2);
                            if(strcmp(seek->obj->tag , "MapCell")==0){
                                if(is_mouse_inside_cell(mouse_pos , seek->obj->pos)){
                                printf("Clicked on : %d, %d\n" , seek->obj->cell_info->cell_pos.first,seek->obj->cell_info->cell_pos.second);
                                int first = seek->obj->cell_info->cell_pos.first;
                                int seco = seek->obj->cell_info->cell_pos.second;
                                printf("%d,%d\n" , first , seco);
                                light_cells_dir(head,first,seco , cntt%6,true);
                                cntt++;
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
                                SetConsoleTextAttribute(hConsole, 207);
                                printf("    ");
                                SetConsoleTextAttribute(hConsole, 15);
                                isGamePlaying = true;
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
                        //printf("%s\n" , typed_txt);
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
                        //_pair dest = {12,16};
                        //change_wall(Scene , "wall1" , dest,window_surface , light_eff , window);
                        //_pair l1 = {7,15};
                        //_pair l2 = {5,3};
                       // switch_lamps(l1,l2,Scene,window_surface,light_eff,window);
                        //_pair p1 = {1,15};
                        //_pair p2 = {2,10};
                        //_pair p3 = {1,5};
                        //switch_pits(p1,p2,Scene,window_surface , light_eff , window);
                        //switch_pits(p2,p3,Scene,window_surface , light_eff , window);
                        //JS -> 7,3

                        //change_character_place(Scene,"JW" , dest,window_surface,light_eff,window);
                        //change_character_place(Scene,"MS" , dest,window_surface,light_eff,window);
                        //switch_characters(Scene , "MS" , "JW" , window_surface , light_eff , window);
                        //change_watson_direction(Scene , (++JW.direction)%6);

                        //ShowWindow(windowHandle,SW_SHOW);
                    }
                    else if(event.key.keysym.sym == SDLK_h){
                       // ShowWindow(windowHandle,SW_HIDE);
                    }
                break;
            }
        }
        DrawStuff(&head , window_surface , light_eff); // Draw Stuff on buffer
        draw(window); //actually draw them on screen (update Screen)
        clear_surface(window_surface); //clear after that
    }
    //Free Stuff needs a lot of work to do
    while(Scene->next){
        Scene = Scene->next;
        free(Scene->prev);
    }
    while(Menu->next){
        Menu = Menu->next;
        free(Menu->prev);
    }
    while(new_game_name->next){
        new_game_name = new_game_name->next;
        free(new_game_name->prev);
    }
    while(About->next){
        About = About->next;
        free(About->prev);
    }
    SDL_DestroyWindow(window);
    Mix_FreeMusic(bgm);
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();

    return 0;
}
