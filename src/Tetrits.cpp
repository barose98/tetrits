#include "Tetrits.h"
#include "debug_config.h"
std::vector<Yancey_Vector> TETR_I = { {-1,-1}, {-1,1}, {-1,3}, {-1,-3}};
std::vector<Yancey_Vector> TETR_J = { {0,0}, {0,-2}, {0,2}, {-2, 2}};
std::vector<Yancey_Vector> TETR_L = { {0,0}, {0,-2}, {0,2}, {2, 2}};
std::vector<Yancey_Vector> TETR_O = { {-1,-1}, {1,-1}, {-1,1}, {1,1}};
std::vector<Yancey_Vector> TETR_S = { {0,0}, {-2,0}, {0,-2}, {2,-2}};
std::vector<Yancey_Vector> TETR_T = { {0,0}, {-2,0}, {2,0}, {0,-2}};
std::vector<Yancey_Vector> TETR_Z = { {0,0}, {-2,0}, {0,2}, {2,2}};

Tetrits::Tetrits() : PARENTGAME("tetrits",WIND_W,WIND_H,FRAMERATE){}
Tetrits::~Tetrits(){}
bool Tetrits::init(uint32_t fl)
{
  PARENTGAME::init(WINDOWFLAGS);
  std::srand(time(0));
  this->spawn(this->active_tet);
  return true;
}
bool Tetrits::spawn(Tetromino& t)
{
  uint8_t sh = std::round(std::rand() % 7);
  t = Tetromino(SPAWN,this->shapes[sh]);
  return true;
}
bool Tetrits::update()
{
  if(PARENTGAME::frames.ready)
    {
       // log_i<< "upd" <<std::endl;
       this->render_clear( GAMEBACKGROUND );
       this->set_render_color(BLOCKCOLOR);

       for(Yancey_rect block : this->active_tet.blocks)
	 {
	   block.location = block.location + this->active_tet.location;
	   std::vector<Yancey_Vector> points = block.get_polygon();
	   this->draw_lines(points);
	 }
       
       this->set_render_color(YANCEYCOLOR_Red);
       this->draw_points({this->active_tet.location});
       
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
  for(Yancey_Vector b : locs)
	 blocks.push_back(Yancey_rect(0,true,{BLOCKSIZE,BLOCKSIZE},b * BLOCKSIZE));
}
Tetromino::Tetromino():Tetromino(SPAWN,'l'){}
Tetromino::Tetromino(const Tetromino &t):location(t.location),shape(t.shape){}
void Tetromino::rotate(bool cw)
{
this->orientation = this->orientation.get_normal(cw);
 for(Yancey_rect &b : this->blocks)
   b.location = b.location.get_normal(cw);
}
    
