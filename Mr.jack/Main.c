//SDL_WINDOW_ALWAYS_ON_TOP , SDL_WINDOW_INPUT_GRABBED , SDL_WINDOW_SHOWN
#include "SDL.h"
#include <stdio.h>
#include <stdbool.h>
#include "Graphics/Mrjack_Graphic.h"
#include <Windows.h>
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "SDL_opengl.h"
#define abs(a) a>0?a:-a
typedef SDL_Surface Image;
typedef SDL_Rect Position;
int main(int argc , char** argv){
    // --------- Hiding Console window from user
        HWND windowHandle = GetConsoleWindow();
        //ShowWindow(windowHandle,SW_HIDE);
    // ---------------------------------------
    SDL_Event event;
    Init();
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
    //=-----------Game Scene Items ------=
    _clickable Home = {.tag = "Home" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 800 },.image = load_image(home)};


    // -------------------------------------
    _clickable p_point = {.tag = "p_point" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 1150 , .y = 30 },.image = load_image(point_pic)};

    bool keep_window_open = true;
    _pos mouse_pos;

    Label test_label = {.text = "Mr.Jack Game\nCreated by K3rn3lPanic in SDL2" , .font_path = fonts_segoeui , ._font_size = 25 , .back_color={0 ,0 ,0} , .fore_color = {120,120,255} , .text_location = {__x+35,__y+35 ,0, 0} };
    TTF_Font *font = TTF_OpenFont("segoeui.ttf" , 20);
    SDL_Color fg;
    fg.a = 100;
    fg.r = 0;
    fg.g =0;
    fg.b = 0;
    SDL_Surface *text_surface = TTF_RenderText_Solid(font , "hello",fg);
    _clickable labeld = {.tag = "dd" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 300 , .y = 200 },.image = text_surface};


    SDL_Surface *light_eff = load_image(light_effect);
    //----------------------------------------------------
    Drawable *Menu = Create_node(&background_picture);
    Add_obj(Menu , Create_node(&Border));
    Add_obj(Menu , Create_node(&Play));
    Add_obj(Menu , Create_node(&Exit));
    Add_obj(Menu , Create_node(&Resume));
    Add_obj(Menu , Create_node(&About_me));
    Add_obj(Menu , Create_node(&mum_btn));
    Add_obj(Menu , Create_node(&labeld));

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
    _clickable light1 = {.tag = "LI1" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\light.png")};
    Drawable *cell = Find_Cell(Scene , 1,13);
    light1.pos.x = cell->obj->pos.x;
    light1.pos.y = cell->obj->pos.y;
    light1.cell_info = cell->obj->cell_info;
    cell->obj->cell_info->lighten = true;
    Add_obj(Scene , Create_node(&light1));

    _clickable light2 = {.tag = "LI2" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\light.png")};
    cell = Find_Cell(Scene ,2,4);
    light2.pos.x = cell->obj->pos.x;
    light2.pos.y = cell->obj->pos.y;
    light2.cell_info = cell->obj->cell_info;
    cell->obj->cell_info->lighten = true;
    Add_obj(Scene , Create_node(&light2));

    _clickable light3 = {.tag = "LI3" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\light.png")};
    cell = Find_Cell(Scene ,5,11);
    light3.pos.x = cell->obj->pos.x;
    light3.pos.y = cell->obj->pos.y;
    light3.cell_info = cell->obj->cell_info;
    cell->obj->cell_info->lighten = true;
    Add_obj(Scene , Create_node(&light3));

    _clickable light4 = {.tag = "LI4" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\light.png")};
    cell = Find_Cell(Scene ,7,15);
    light4.pos.x = cell->obj->pos.x;
    light4.pos.y = cell->obj->pos.y;
    light4.cell_info = cell->obj->cell_info;
    cell->obj->cell_info->lighten = true;
    Add_obj(Scene , Create_node(&light4));

    _clickable light5 = {.tag = "LI5" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\light.png")};
    cell = Find_Cell(Scene ,10,14);
    light5.pos.x = cell->obj->pos.x;
    light5.pos.y = cell->obj->pos.y;
    light5.cell_info = cell->obj->cell_info;
    cell->obj->cell_info->lighten = true;
    Add_obj(Scene , Create_node(&light5));


    _clickable light6 = {.tag = "LI6" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Blocks\\light.png")};
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


    _clickable JW = {.tag = "JW1" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\watson_1.png")};
    cell = Find_Cell(Scene ,8,16);
    JW.pos.x = cell->obj->pos.x+18;
    JW.pos.y = cell->obj->pos.y+13;
    JW.cell_info = cell->obj->cell_info;
    cell->obj->do_chick = false;
    cell->obj->click_able = true;
    Add_obj(Scene , Create_node(&JW));

    _clickable IL = {.tag = "IL" ,.click_able = true, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1450 , .y = 750 },.image = load_image("Images\\Characters\\Lestrade.png")};
    cell = Find_Cell(Scene ,4,10);
    IL.pos.x = cell->obj->pos.x+18;
    IL.pos.y = cell->obj->pos.y+13;
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

    Drawable *head = Menu;

    bool Playing = true;
    int cntt = 0;
    //Play_voice_Thread(sounds_button3);

    while(keep_window_open)
    {
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

                                //Light_Cell(head , first , seco,true);

                                //Change_Block_pic(head,first , seco-2,walkable_block);
                                //Change_Block_pic(head,first , seco+2,walkable_block);
                                //Change_Block_pic(head,first-1 , seco-1,walkable_block);
                                //Change_Block_pic(head,first+1 , seco-1,walkable_block);
                                //Change_Block_pic(head,first+1 , seco+1,walkable_block);
                                //Change_Block_pic(head,first-1 , seco+1,walkable_block);
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
                                //Scene = Create_node(&background_picture2);
                                //Draw_map(Scene,50,0);
                                //Add_obj(Scene , Create_node(&Home));
                                //Add_obj(Scene , Create_node(&mum_btn));
                                change_scene(&head , new_game_name);
                            }

                        }
                        seek = seek->next;
                    }
                break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                //printf("Key %c %s\n", (char)event.key.keysym.sym, ((event.key.state == SDL_PRESSED) ? "pressed" : "released"));
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
                    ShowWindow(windowHandle,SW_SHOW);
                }
                else if(event.key.keysym.sym == SDLK_h){
                    ShowWindow(windowHandle,SW_HIDE);
                }

                break;
            }
        }

        DrawStuff(&head , window_surface , light_eff); // Draw Stuff on buffer
        draw(window); //actually draw them on screen (update Screen)
        clear_surface(window_surface); //clear after that
    }
    //Free Stuff needs a lot of work to do
    while(head->next){
        head = head->next;
        free(head->prev);
    }
    SDL_DestroyWindow(window);
    Mix_FreeMusic(bgm);
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();

    return 0;
}
