#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "SDLGame.h"
#include "YanceyShape.h"
#include "colors_config.h"

#define PARENTGAME SDLGame
#define WINDOWFLAGS SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK

#define FRAMERATE 20
#define BLOCKSIZE 20 // * 2

#define WIND_H (BLOCKSIZE * 2 * 21)
#define WIND_W  (BLOCKSIZE * 2 * 11)

#define BLOCK Yancey_Vector({BLOCKSIZE,BLOCKSIZE})

#define SPAWN { (BLOCKSIZE * 10), (BLOCKSIZE * 3)}

#define MOVE Yancey_Vector({1 , 0})

#define GAMEBACKGROUND YANCEYCOLOR_DarkBlue
#define BLOCKCOLOR YANCEYCOLOR_White

class Tetrits_Block : public Yancey_shape2d{
public:
  Tetrits_Block();
  Tetrits_Block(const Tetrits_Block &b);
  ~Tetrits_Block();
  Tetrits_Block(Yancey_Vector loc);
  bool collides_with(Tetrits_Block other);
  Yancey_Vector location;
};

class Tetromino 
{
public:  
  Tetromino(Yancey_Vector location, uint8_t shape);
  Tetromino();
  Tetromino(const Tetromino &t);
  virtual ~Tetromino(){};
  void rotate(bool cw);
  bool lands(std::vector<Tetrits_Block> &f);
  Yancey_Vector location;
  std::vector<Tetrits_Block> blocks;
  uint8_t shape;
  Yancey_Vector orientation;
};

class Tetrits : public SDLGame{
public:
  Tetrits();
  ~Tetrits();
  bool init(uint32_t flags);
  bool update() override;
  bool spawn(Tetromino& t);
  void init_floor()
  {
  int j=this->wind_h;
  for(int i = this->wind_w/2; i<=this->wind_w/2+BLOCKSIZE*2;i+=(BLOCKSIZE * 2))
    this->floor.push_back(Tetrits_Block(Yancey_Vector({i,j}) ));
  
  };
  
  Tetromino active_tet;
  SDL_Joystick *joystick;
  std::vector<uint8_t> shapes = {'i','j','l','o','s','t','z'};
  Yancey_Vector  FALLS = {0,1};

  std::vector<Tetrits_Block> floor = {  };

  
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
      this->spawn(this->active_tet);
      break;
    case SDLK_LEFT:
    case SDLK_a:
      this->active_tet.location -= MOVE * (2 * BLOCKSIZE);
      break;
    case SDLK_RIGHT:
    case SDLK_d:
      this->active_tet.location += MOVE * (BLOCKSIZE);
      break;
      /*
 
    case SDLK_q:
    case SDLK_UP:
      this->active_tet.location -= this->FALLS * (BLOCKSIZE);
      break;      
    case SDLK_s:
    case SDLK_DOWN:
      this->active_tet.location += this->FALLS * (2 * BLOCKSIZE);
      this->active_tet.lands(this->floor);
      //log_i<< (this->active_tet.location.y / BLOCKSIZE) <<" < "<< this->wind_h/BLOCKSIZE <<std::endl;
      break;
       */      
    case SDLK_w:
      this->active_tet.rotate(false);
      break;
    case SDLK_e:
      this->active_tet.rotate(true);
      break;

    }

  return true;

};

};

