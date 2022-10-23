#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <stdbool.h>
#include "SDLGame.h"
#include "YanceyShape.h"
#include "colors_config.h"


#define PARENTGAME SDLGame
#define WINDOWFLAGS SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK

#define WIND_W 1280
#define WIND_H 640

#define FRAMERATE 10
#define BLOCKSIZE 20 // * 2

#define GAMEBACKGROUND YANCEYCOLOR_DarkBlue
#define BLOCKCOLOR YANCEYCOLOR_White

class Tetromino 
{
public:  
  Tetromino(Yancey_Vector location, uint8_t shape);
  Tetromino();
  Tetromino(const Tetromino &t);
  ~Tetromino(){};
  void rotate(bool cw);
  Yancey_Vector location;
  std::vector<Yancey_rect> blocks;
  uint8_t shape;
  Yancey_Vector orientation;
};

class Tetrits : public SDLGame{
public:
  Tetrits();
  ~Tetrits();
  bool init(uint32_t flags);
  bool update() override;

  Tetromino active_tet;
SDL_Joystick *joystick;
//EVENTS

bool handle_events() 
{
  bool ret = true;
  Yancey_Vector mouse;
  SDL_Event ev; 
   while(SDL_PollEvent(&ev) != 0)
     {
       switch (ev.type) {
          case SDL_QUIT:
	     return false;
          case SDL_WINDOWEVENT:
	    if(ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
	        SDL_GetWindowSize(this->window,&this->wind_w,&this->wind_h);

	    }
	    break;
          case SDL_KEYDOWN:
	    ret = this->handle_key(&ev.key);
	    break;
          case SDL_JOYDEVICEADDED:
	    this->joystick = SDL_JoystickOpen(ev.jdevice.which);
	    log_i<< "Joystick index "<< ev.jdevice.which <<" added " <<std::endl;
	    break;
	    /*
 
      case SDL_JOYAXISMOTION:
	    #ifdef CONTROL2
	    if(SDL_JoystickGetAxis(this->joystick,0) != 0)
	      this->dx = this->dx.get_normal( (SDL_JoystickGetAxis(this->joystick,0) > 0) );
	    #else
	    if(SDL_JoystickGetAxis(this->joystick,0) > 0) this->dx = {1,0};
	    if(SDL_JoystickGetAxis(this->joystick,0) < 0) this->dx = {-1,0};
	    if(SDL_JoystickGetAxis(this->joystick,1) > 0) this->dx = {0,1};
	    if(SDL_JoystickGetAxis(this->joystick,1) < 0) this->dx = {0,-1};
	    #endif
	    break;
          case SDL_JOYBUTTONDOWN :
	    //	    for(int j=0; j < SDL_JoystickNumButtons(this->joystick); j++){
	    //A=1,B=2,start=9,select=8
	    if(SDL_JoystickGetButton(this->joystick, 9)) this->reset();
	      //}
	    break;
	     */
       }
     }
return ret;
};

bool handle_key( SDL_KeyboardEvent *key )
{
  switch(key->keysym.sym)
    {
    case SDLK_ESCAPE:
      break;
    case SDLK_LEFT:
    case SDLK_a:
      this->active_tet.location.x -= 2 * BLOCKSIZE;
      break;
    case SDLK_RIGHT:
    case SDLK_d:
      this->active_tet.location.x += 2 * BLOCKSIZE;
      break;
    case SDLK_w:
      this->active_tet.rotate(false);
      break;
    case SDLK_e:
      this->active_tet.rotate(true);
      break;
    case SDLK_DOWN:
      //this->dx = this->dx.get_normal(true);
      break;
    }

  return true;

};

};

