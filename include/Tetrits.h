#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "SDLGame.h"
#include "YanceyShape.h"
#include "YanceyDigit.h"
#include "colors_config.h"

#define PARENTGAME SDLGame
#define WINDOWFLAGS SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK

//#define FALLING true
#define USEJOY 

#define FRAMERATE 10
#define FALLS Yancey_Vector({0,20})

#define BLOCKSIZE 20 // * 2
#define BLOCKSIZE2 (BLOCKSIZE * 2) // * 1
#define BLOCKSIZE_N (BLOCKSIZE / 2)

#define WIND_H (BLOCKSIZE2 * 20)
#define WIND_W  (BLOCKSIZE2 * 10)

#define BLOCK Yancey_Vector({BLOCKSIZE,BLOCKSIZE})
#define BLOCK_N Yancey_Vector({BLOCKSIZE_N,BLOCKSIZE_N})
#define SPAWN_IO Yancey_Vector({8,2}) * BLOCKSIZE
#define SPAWN_JLSTZ Yancey_Vector({9,3}) * BLOCKSIZE
#define SETTLE Yancey_Vector({0,BLOCKSIZE2})
#define NEXTLOC Yancey_Vector({17,2})
#define SCORELOC Yancey_Vector({90,30})
#define DIGITSIZE Yancey_Vector({10,20})// x*2 y*2
#define SCORESIZE Yancey_Vector({90,40})

#define MOVE Yancey_Vector({BLOCKSIZE2 , 0})
#define SPEEDINCR 1
#define EXT_L (Yancey_Vector({-BLOCKSIZE , 0})  )
#define EXT_R (Yancey_Vector({BLOCKSIZE , 0})  )
#define EXT_D this->falls
//#define EXT_DB this->falls + Yancey_Vector({0, BLOCKSIZE})


#define LANDSEXT Yancey_Vector({0 , 2})
#define HITSEXT Yancey_Vector({2 , 0})

#define GAMEBACKGROUND YANCEYCOLOR_Black
#define BLOCKCOLOR YANCEYCOLOR_Blue
#define FLOORCOLOR YANCEYCOLOR_SlateGray
#define WALLCOLOR YANCEYCOLOR_White
#define SCORECOLOR YANCEYCOLOR_Cyan

class Tetrits_Block : public Yancey_rect{
public:
  Tetrits_Block();
  Tetrits_Block(const Tetrits_Block &b);
  ~Tetrits_Block();
  Tetrits_Block(Yancey_Vector loc, Yancey_Vector size);
  Yancey_Color color;
  bool operator ==(const Tetrits_Block &b){return this->location.y == b.location.y;}
  bool operator ==(int i){return this->location.y == i;}
  };

class Tetromino 
{
public:  
  Tetromino(Yancey_Vector location, Yancey_Vector size,  uint8_t shape);
  Tetromino();
  Tetromino(const Tetromino &t);
  virtual ~Tetromino(){};
  void rotate(bool cw);
  bool hits_block(Yancey_rect other, Yancey_Vector ext);
  Yancey_Vector location;
  Yancey_Color color;
  Yancey_rect rot_shadow;
  std::vector<Tetrits_Block> blocks;
  uint8_t shape;  
  Yancey_Vector orientation;
};

class Tetrits : public SDLGame{
public:
  Tetrits();
  ~Tetrits();
  bool init() override;
  bool update() override;
  bool spawn();
  void rotate_active(bool cw);
  bool move_active(Yancey_Vector m);
  void settle();
  void reset();
  void draw_score();
  void update_score(int score);
  bool hits_obstacle(Tetromino &t, std::vector<Yancey_Vector> ext);
  bool hits_obstacle(Tetromino &t, Yancey_Vector ext);
  bool within_bounds(Tetromino &t, Yancey_Vector &ext);
  //  bool within_bounds(Tetromino &t, std::vector<Yancey_Vector> ext);
  void draw(Tetromino &t);
  
  int score;
  std::vector<Yancey_Digit> score_digits;
  Tetromino active_tet;
  Tetromino next_tet;
  SDL_Joystick *joystick;
  std::vector<uint8_t> shapes = {'i','j','l','o','s','t','z'};
  bool paused = false;
  Yancey_Vector  falls = FALLS;

  std::vector<Tetrits_Block> obstacles = {};
  Yancey_rect floor;

  void init_floor()
  {
  int i = 0;
  int j=WIND_H;
  this->floor = Yancey_rect(0,true,{WIND_W,WIND_H},{WIND_W/2,WIND_H/2});
    };
  
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
#ifdef USEJOY
          case SDL_JOYDEVICEADDED:
	    this->joystick = SDL_JoystickOpen(ev.jdevice.which);
	    log_i<< "Joystick index "<< ev.jdevice.which <<" added " <<std::endl;
	    break;
	     
           case SDL_JOYAXISMOTION:
	    if(SDL_JoystickGetAxis(this->joystick,0) < 0)
	      {
		this->move_active(MOVE * -1);
	      }
	    if(SDL_JoystickGetAxis(this->joystick,0) > 0)
	      {
		this->move_active(MOVE);
	      }
	    if(SDL_JoystickGetAxis(this->joystick,1) < 0)
	      {
		//this->falls += SPEEDINC;
		//PARENTGAME::frames.framerate += SPEEDINCR;
		//log_i<<"FALLS: "<<this->falls.y<<" RATE: "<<PARENTGAME::frames.framerate<<std::endl;
		this->move_active(this->falls * -1);
	      }
	    if(SDL_JoystickGetAxis(this->joystick,1) > 0)
	      {
		//if(PARENTGAME::frames.framerate > 1)PARENTGAME::frames.framerate -= SPEEDINCR;
		//log_i<<"FALLS: "<<this->falls.y<<" RATE: "<<PARENTGAME::frames.framerate<<std::endl;
		 this->move_active(this->falls);
	      }	  	    
	    break;
          case SDL_JOYBUTTONDOWN :
	    //	    for(int j=0; j < SDL_JoystickNumButtons(this->joystick); j++){
	    //A=1,B=2,start=9,select=8
	    if(SDL_JoystickGetButton(this->joystick, 1)) this->rotate_active(false);
	    if(SDL_JoystickGetButton(this->joystick, 2)) this->rotate_active(true);
	    if(SDL_JoystickGetButton(this->joystick, 8)) this->reset();
	    if(SDL_JoystickGetButton(this->joystick, 9)) this->paused = !this->paused;
	      
	    break;
#endif
       }
     }
return ret;
};

bool handle_key( SDL_KeyboardEvent *key )
{
  Yancey_Vector resolve;
  switch(key->keysym.sym)
    {
    case SDLK_ESCAPE:
      this->reset();
      break;
    case SDLK_l:
      PARENTGAME::frames.framerate += SPEEDINCR;
      log_i<<"FALLS: "<<this->falls.y<<" RATE: "<<PARENTGAME::frames.framerate<<std::endl;      break;
    case SDLK_k:
      if(PARENTGAME::frames.framerate > 1)PARENTGAME::frames.framerate -= SPEEDINCR;
      log_i<<"FALLS: "<<this->falls.y<<" RATE: "<<PARENTGAME::frames.framerate<<std::endl;
      break;
    case SDLK_LEFT:
    case SDLK_a:
      this->move_active(MOVE *-1) ;
      break;
    case SDLK_RIGHT:
    case SDLK_d:
      this->move_active(MOVE);
      break;
    case SDLK_q:
    case SDLK_UP:    
      this->move_active(this->falls * -1);
      break;      
    case SDLK_s:
    case SDLK_DOWN:
      this->move_active(this->falls);
      break;
    case SDLK_n:
    this->spawn();
    break;
    case SDLK_w:
      this->rotate_active(false);
      break;
    case SDLK_e:
      this->rotate_active(true);
      break;
    case SDLK_SPACE:
      this->paused = !this->paused;
      break;
    default:
      log_func;
    }

  return true;

};

};

