#include "Tetrits.h"
#include "debug_config.h"
std::vector<Yancey_Vector> TETR_I = { {-1,-1}, {-1,1}, {-1,3}, {-1,-3}};
std::vector<Yancey_Vector> TETR_J = { {0,0}, {0,-2}, {0,2}, {-2, 2}};
std::vector<Yancey_Vector> TETR_L = { {0,0}, {0,-2}, {0,2}, {2, 2}};
std::vector<Yancey_Vector> TETR_O = { {-1,-1}, {1,-1}, {-1,1}, {1,1}};
std::vector<Yancey_Vector> TETR_S = { {0,0}, {-2,0}, {0,-2}, {2,-2}};
std::vector<Yancey_Vector> TETR_T = { {0,0}, {-2,0}, {2,0}, {0,-2}};
std::vector<Yancey_Vector> TETR_Z = { {0,0}, {-2,0}, {0,2}, {2,2}};

float BLOCKRAD = sqrt(BLOCKSIZE * BLOCKSIZE * 2);

Tetrits_Block::Tetrits_Block(){}
Tetrits_Block::Tetrits_Block(const Tetrits_Block &t):Tetrits_Block(t.location){}
Tetrits_Block::~Tetrits_Block(){}

Tetrits_Block::Tetrits_Block(Yancey_Vector loc):Yancey_shape2d(0,true,4,BLOCKRAD,loc)
 {
   this->location = loc;
 }
bool Tetrits_Block::collides_with(Tetrits_Block other)
{
  //log_i<< this->location.y <<" "<< other.location.y <<std::endl;
  return (this->location.y + BLOCKSIZE) > (other.location.y - BLOCKSIZE);
}

Tetrits::Tetrits() : PARENTGAME("tetrits",WIND_W,WIND_H,FRAMERATE){}
Tetrits::~Tetrits(){}
bool Tetrits::init(uint32_t fl)
{
  PARENTGAME::init(WINDOWFLAGS);
  std::srand(time(0));
  this->init_floor();
  this->spawn(this->active_tet);
  return true;
}
bool Tetrits::spawn(Tetromino &t)
{
  //drop
  for(Tetrits_Block b : t.blocks)
    {
      b.location = b.location * BLOCKSIZE + this->active_tet.location;
      this->floor.push_back(b);
    }
  
  uint8_t sh = std::round(std::rand() % 7);
  t = Tetromino(SPAWN,this->shapes[sh]);
  return true;
}

bool Tetromino::lands(std::vector<Tetrits_Block> &floor)
{
  for(Tetrits_Block block : this->blocks)
    {
      block.location = block.location * BLOCKSIZE + this->location;
      for(Tetrits_Block f : floor)
	{
	  log_i<<block.location.y<<std::endl;
	  if(block.collides_with(f))
	    {
	    return true;
	    }
	}
    }
  return false;
}
bool Tetrits::update()
{
  if(PARENTGAME::frames.ready)
    {
       this->render_clear( GAMEBACKGROUND );
       this->set_render_color(BLOCKCOLOR);
       
       if(!this->active_tet.lands(this->floor))
	 this->active_tet.location += this->FALLS * 9;
       
       for(Tetrits_Block block : this->active_tet.blocks)
	 {
	   block.location = block.location * BLOCKSIZE + this->active_tet.location;
	   this->draw_rectangle(block.location - BLOCK , 2 * BLOCKSIZE, 2 * BLOCKSIZE);
	   //this->draw_rectangle(block.location - Yancey_Vector({1,1}) , 2, 2);
	 }
       
       this->set_render_color(YANCEYCOLOR_Red);
       this->draw_rectangle(this->active_tet.location-Yancey_Vector({2,2}),4,4);
       
       for(Tetrits_Block f : this->floor)
	 {
	   this->draw_rectangle(f.location - BLOCK, 2 * BLOCKSIZE, 2 * BLOCKSIZE);
	   this->draw_rectangle(f.location - BLOCK * .5 , BLOCKSIZE, BLOCKSIZE);
	 }
	 
       this->render_present(0);

       PARENTGAME::frames.ready = false;
    }
  return true;
}



Tetromino::Tetromino(Yancey_Vector location, uint8_t shape):location(location),shape(shape),orientation({1,0})
{
  std::vector<Yancey_Vector> locs;
  switch(shape){
  case 'i':
    locs = TETR_I;
    break;
  case 'j':
    locs = TETR_J  ;
    break;    
  case 'l':
    locs = TETR_L  ;
    break;
  case 'o':
    locs = TETR_O  ;
    break;
  case 's':
    locs = TETR_S  ;
    break;
  case 't':
    locs = TETR_T  ;
    break;    
  case 'z':
    locs = TETR_Z  ;
    break;        
  default:
    break;
  }
  for(Yancey_Vector l : locs)
    blocks.push_back(Tetrits_Block(l));
}
Tetromino::Tetromino(){}//:Tetromino(SPAWN,'l'){}
Tetromino::Tetromino(const Tetromino &t):location(t.location),shape(t.shape){}
void Tetromino::rotate(bool cw)
{
this->orientation = this->orientation.get_normal(cw);
 for(Tetrits_Block &b : this->blocks)
   b.location = b.location.get_normal(cw);
}
    
