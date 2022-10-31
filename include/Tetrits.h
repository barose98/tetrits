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

#define FALLING true
//#define FALLING false

#define FRAMERATE 7
#define BLOCKSIZE 20 // * 2
#define BLOCKSIZE2 (BLOCKSIZE * 2) // * 1

#define WIND_H (BLOCKSIZE2 * 21)
#define WIND_W  (BLOCKSIZE2 * 11)

#define BLOCK Yancey_Vector({BLOCKSIZE,BLOCKSIZE})

#define MOVE Yancey_Vector({1 , 0})

#define GAMEBACKGROUND YANCEYCOLOR_Black
#define BLOCKCOLOR YANCEYCOLOR_Blue
#define FLOORCOLOR YANCEYCOLOR_Red
#define WALLCOLOR YANCEYCOLOR_White

class Tetrits_Block : public Yancey_rect{
public:
  Tetrits_Block();
  Tetrits_Block(const Tetrits_Block &b);
  ~Tetrits_Block();
  Tetrits_Block(Yancey_Vector loc);
  Yancey_Color color;
  };

class Tetromino 
{
public:  
  Tetromino(Yancey_Vector location,   uint8_t shape);
  Tetromino();
  Tetromino(const Tetromino &t);
  virtual ~Tetromino(){};
  void rotate(bool cw);
  bool lands(void* t);
  bool hits_wall(Yancey_rect wall);
  bool hits_block(Yancey_rect other, Yancey_Vector ext);
  Yancey_Vector location;
  Yancey_Color color;
  std::vector<Tetrits_Block> blocks;
  bool has_landed;
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

    
  Tetromino active_tet;
  SDL_Joystick *joystick;
  std::vector<uint8_t> shapes = {'i','j','l','o','s','t','z'};
  Yancey_Vector  FALLS = {0,5};
  std::vector<Tetrits_Block> obstacles = {};
  Yancey_rect floor;
  Yancey_rect ceiling;
  Yancey_rect left_wall;
  Yancey_rect right_wall;
  
  void init_floor()
  {
  int i = 0;
  int j=this->wind_h;
  //  for(i; i<=this->wind_w+BLOCKSIZE2;i+=(BLOCKSIZE2))
    //    this->floor.push_back(Tetrits_Block(Yancey_Vector({i,j}) ));
  this->floor = Yancey_rect(0,true,{this->wind_w,BLOCKSIZE2},{this->wind_w/2,this->wind_h});
  this->ceiling = Yancey_rect(0,true,{this->wind_w,BLOCKSIZE2},{this->wind_w/2,0});
  this->left_wall = Yancey_rect(0,true,{BLOCKSIZE2, this->wind_h},{0,this->wind_h/2});
  this->right_wall = Yancey_rect(0,true,{BLOCKSIZE2, this->wind_h},{this->wind_w,this->wind_h/2});
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
          case SDL_JOYDEVICEADDED:
	    this->joystick = SDL_JoystickOpen(ev.jdevice.which);
	    log_i<< "Joystick index "<< ev.jdevice.which <<" added " <<std::endl;
	    break;
	     
           case SDL_JOYAXISMOTION:
	    if(SDL_JoystickGetAxis(this->joystick,0) < 0)
	      {
		if(!this->active_tet.hits_block(this->left_wall, Yancey_Vector({2,2})))
		  this->active_tet.location -= MOVE * (BLOCKSIZE2);
	      }
	    if(SDL_JoystickGetAxis(this->joystick,0) > 0)
	      {
		if(!this->active_tet.hits_block(this->right_wall, Yancey_Vector({2,2})))
		  this->active_tet.location += MOVE * (BLOCKSIZE2);
	      }	    
	    break;
          case SDL_JOYBUTTONDOWN :
	    //	    for(int j=0; j < SDL_JoystickNumButtons(this->joystick); j++){
	    //A=1,B=2,start=9,select=8
	    if(SDL_JoystickGetButton(this->joystick, 1)) this->active_tet.rotate(false);
	    if(SDL_JoystickGetButton(this->joystick, 2)) this->active_tet.rotate(true);
	      //}
	    break;
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
      this->spawn(this->active_tet);
      break;
    case SDLK_LEFT:
    case SDLK_a:
      resolve = MOVE * (BLOCKSIZE);
      if(!this->active_tet.hits_block(this->left_wall, Yancey_Vector({2,2})))
	this->active_tet.location -= MOVE * (BLOCKSIZE2);
      break;
    case SDLK_RIGHT:
    case SDLK_d:
      resolve = MOVE * (BLOCKSIZE) * -1;
      if(!this->active_tet.hits_block(this->right_wall, Yancey_Vector({2,2})))
	      this->active_tet.location += MOVE * (BLOCKSIZE2);
      break;
    case SDLK_q:
    case SDLK_UP:
      //      if(!this->active_tet.hits_block(this->ceiling))      
      //	this->active_tet.location -= this->FALLS * (BLOCKSIZE);
      break;      
    case SDLK_s:
    case SDLK_DOWN:
      resolve = this->FALLS * (BLOCKSIZE) * -1;
      if(!this->active_tet.lands(this))
	this->active_tet.location += this->FALLS * (BLOCKSIZE);
      break;
    
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

