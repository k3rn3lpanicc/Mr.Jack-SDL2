//SDL_WINDOW_ALWAYS_ON_TOP , SDL_WINDOW_INPUT_GRABBED , SDL_WINDOW_SHOWN
#include "SDL.h"
#include <stdio.h>
#include <stdbool.h>
#include "Graphics/Mrjack_Graphic.h"
#include <Windows.h>
#include "SDL_ttf.h"
typedef SDL_Surface Image;
typedef SDL_Rect Position;
int main(int argc , char** argv){
    // --------- Hiding Console window from user
        //HWND windowHandle = GetConsoleWindow();
        //ShowWindow(windowHandle,SW_HIDE);
    // ---------------------------------------
    SDL_Event event;
    Init();

    SDL_Window *window = SDL_CreateWindow(TITLE_TEXT,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1500, 800,SDL_WINDOW_SHOWN);

    if(!window)
        error_and_exit("Failed to create window\n");
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    if(!window_surface)
        error_and_exit("Failed to get the surface from the window\n");

    // --------------Initialize Inocent Cards
    _clickable Goodley={.tag= "Goodley",.click_able = false,.entered = false,.do_chick = false,.visible = true,.pos = { .x = 950 , .y = 10},.image = load_image(In_Goodley)};
    _clickable Sherlock={.tag = "Sherlock" ,.click_able = false, .entered = false,.do_chick = false,.visible = true,.pos = { .x = 1100 , .y = 10 },.image = load_image(In_Sherlock)};
    _clickable p_point = {.tag = "p_point" ,.click_able = false, .entered = false,.do_chick = false,.visible = false,.pos = { .x = 1150 , .y = 30 },.image = load_image(point_pic)};
    _clickable Button1 = {.tag = "Play" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 50 , .y = 50 },.image = load_image(button_1)};
    _clickable Button2 = {.tag = "exit" ,.click_able = true, .entered = false,.do_chick = true,.visible = true,.pos = { .x = 50 , .y = 125 },.image = load_image(button_2)};
    // --------------------------------------

    bool keep_window_open = true;
    _pos mouse_pos;
    Label test_label = {.text = "Mr.Jack Game" , .font_path = fonts_segoeui , ._font_size = 25 , .back_color={0 ,0 ,0} , .fore_color = {120,120,255} , .text_location = {95,68 ,0, 0} };
    test_label.image = Create_Label(test_label);

    Drawable *head = Create_node(&Goodley);
    Add_obj(head , Create_node(&Button1));
    Add_obj(head , Create_node(&Button2));
    Add_obj(head , Create_node(&Sherlock));
    Add_obj(head , Create_node(&p_point));

    //Play_voice_Thread(sounds_button3);

    while(keep_window_open)
    {
        if(SDL_PollEvent(&event) > 0)
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    keep_window_open = false;
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    //--------Free heap from Drawable Objects before closing the program ---
                    while(head->next){
                        head = head->next;
                        free(head->prev);
                    }
                    //--------------------------------------------------------------------
                    return 0;
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

                //case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    seek = head;
                    while(seek){
                        if((event.button.state != SDL_PRESSED)&&seek->obj->click_able&&(strcmp(convert_button_number_to_string(event.button.button) , "left")==0)&&is_mouse_inside_Surface(mouse_pos, seek->obj->pos)){
                            Play_voice_Thread(sounds_button2);
                            if(strcmp(seek->obj->tag , "exit")==0){
                                keep_window_open = false;
                                exit(0);
                            }
                        }
                        seek = seek->next;
                    }
                break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                printf("Key %c %s\n", (char)event.key.keysym.sym, ((event.key.state == SDL_PRESSED) ? "pressed" : "released"));
                break;
            }
        }
        DrawStuff(&head , window_surface); // Draw Stuff on buffer
        draw(window); //actually draw them on screen (update Screen)
        clear_surface(window_surface); //clear after that
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
