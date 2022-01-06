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
typedef SDL_Surface Image;
typedef SDL_Rect Position;
int main(int argc , char** argv){
    // --------- Hiding Console window from user
        //HWND windowHandle = GetConsoleWindow();
        //ShowWindow(windowHandle,SW_HIDE);
    // ---------------------------------------
    SDL_Event event;
    Init();
    int Volume = 100;
    Mix_Music *bgm = Mix_LoadMUS("Sounds\\bg1.mp3");
    Mix_PlayMusic(bgm,-1);
    Mix_VolumeMusic(Volume);

    SDL_Window *window = SDL_CreateWindow(TITLE_TEXT,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1500, 800,SDL_WINDOW_SHOWN);

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
    //-------------Menu Items --------------
    int __x = 585;
    int __y = 190;
    _clickable Border = {.tag = "Border" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x-35 , .y = __y-75},.image = load_image(border_pic)};
    _clickable Play = {.tag = "Play" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y},.image = load_image(btn_play)};
    _clickable Exit = {.tag = "exit" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y+270 },.image = load_image(btn_exit)};
    _clickable Resume = {.tag = "Resume" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y+90 },.image = load_image(btn_resume)};
    _clickable About_me = {.tag = "About Me" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y+180 },.image = load_image(btn_about)};
    //-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=
    _clickable mum_btn = {.tag = "mute" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 10 , .y = 10 },.image = load_image(unmute_btn)};
    _clickable background_picture = {.tag = "bground" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 0 , .y = 0 },.image = load_image(bg_pic)};
    //------------About Page Items---------
    _clickable Back_btn = {.tag = "Back" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = __x , .y = __y+300 },.image = load_image(btn_back)};
    _clickable About_Label = {.tag = "About_me" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = __x-10 , .y = __y-30 },.image = load_image(about_me)};

    // -------------------------------------
    _clickable p_point = {.tag = "p_point" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 1150 , .y = 30 },.image = load_image(point_pic)};

    bool keep_window_open = true;
    _pos mouse_pos;
    Label test_label = {.text = "Mr.Jack Game\nCreated by K3rn3lPanic in SDL2" , .font_path = fonts_segoeui , ._font_size = 25 , .back_color={0 ,0 ,0} , .fore_color = {120,120,255} , .text_location = {__x+35,__y+35 ,0, 0} };
    test_label.image = Create_Label(test_label);

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


    Drawable *head = Menu;

    bool Playing = true;
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
                        if((event.button.state != SDL_PRESSED)&&seek->obj->click_able&&(strcmp(convert_button_number_to_string(event.button.button) , "left")==0)&&is_mouse_inside_Surface(mouse_pos, seek->obj->pos)){
                            Play_voice_Thread(sounds_button2);
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
                            else if(strcmp(seek->obj->tag , "About Me")==0){
                                head = About;
                            }
                            else if(strcmp(seek->obj->tag , "Back")==0){
                                head = Menu;
                            }


                        }
                        seek = seek->next;
                    }
                break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                printf("Key %c %s\n", (char)event.key.keysym.sym, ((event.key.state == SDL_PRESSED) ? "pressed" : "released"));
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

        DrawStuff(&head , window_surface); // Draw Stuff on buffer
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
