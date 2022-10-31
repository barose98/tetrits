#include "Tetrits.h"
#include "debug_config.h"
std::vector<Yancey_Vector> TETR_I = { {-1,-1}, {-1,1}, {-1,3}, {-1,-3}};
std::vector<Yancey_Vector> TETR_J = { {0,0}, {0,-2}, {0,2}, {-2, 2}};
std::vector<Yancey_Vector> TETR_L = { {0,0}, {0,-2}, {0,2}, {2, 2}};
std::vector<Yancey_Vector> TETR_O = { {-1,-1}, {1,-1}, {-1,1}, {1,1}};
std::vector<Yancey_Vector> TETR_S = { {0,0}, {-2,0}, {0,-2}, {2,-2}};
std::vector<Yancey_Vector> TETR_T = { {0,0}, {-2,0}, {2,0}, {0,-2}};
std::vector<Yancey_Vector> TETR_Z = { {0,0}, {2,0}, {0,-2}, {-2,-2}};

float BLOCKRAD = sqrt(BLOCKSIZE * BLOCKSIZE * 2);

Tetrits_Block::Tetrits_Block(){}
Tetrits_Block::Tetrits_Block(const Tetrits_Block &t):Yancey_rect(t.id,t.solid,BLOCK *2,t.location)
{
  this->color=t.color;
}
Tetrits_Block::~Tetrits_Block(){}
Tetrits_Block::Tetrits_Block(Yancey_Vector loc):Yancey_rect(0,true,BLOCK *2,loc) { }

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
  t.has_landed = false;
  for(Tetrits_Block &b : t.blocks)
    {
      //     b.color = t.color;
      b.location = b.location * BLOCKSIZE + this->active_tet.location;
      this->obstacles.push_back(b);
    }
  Yancey_Vector  SPAWN;
  uint8_t sh = std::round(std::rand() % 7);
  if(shapes[sh] == 'i'){SPAWN =  Yancey_Vector({11,6}) *BLOCKSIZE;}
  else if(shapes[sh] == 'o'){SPAWN =  Yancey_Vector({11,4}) *BLOCKSIZE;}
  else {SPAWN=  Yancey_Vector({10,5}) *BLOCKSIZE;}
  t = Tetromino(SPAWN,this->shapes[sh]);
  return true;
}

bool Tetromino::lands(void* v)
{
  Tetrits* t = (Tetrits*)v;
  if(this->hits_block(t->floor, Yancey_Vector({2,2})) )return true;
  for(Tetrits_Block b : t->obstacles)
    {
      if(this->hits_block(b, Yancey_Vector({0,2})))
	{
	  return true;
	}
    }
}

bool Tetromino::hits_block(Yancey_rect other, Yancey_Vector ext)
{
  other.size += (BLOCK * 2) + ext;
  for(Tetrits_Block block : this->blocks)
    {
      block.location *=  BLOCKSIZE ;
      block.location += this->location ;
      if(other.collides_with(block.location))
	{
	  return true;
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
       this->active_tet.has_landed = this->active_tet.lands(this);
#if(FALLING)
       if(!this->active_tet.has_landed)
	 {
	   this->active_tet.location += this->FALLS;
	 }else{
	 this->spawn(this->active_tet);
	   }
#endif       
       this->set_render_color(this->active_tet.color);
       for(Tetrits_Block block : this->active_tet.blocks)
	 {
	   //log_i<<block.id<<' ';
	   block.location = block.location * BLOCKSIZE + this->active_tet.location;
	   this->draw_rectangle(block.location - BLOCK , BLOCKSIZE2, BLOCKSIZE2);
	   this->draw_rectangle(block.location - BLOCK / 2 , BLOCKSIZE, BLOCKSIZE);
	 }

       for(Tetrits_Block block : this->obstacles)
	 {
	   this->set_render_color(block.color);
	   this->draw_rectangle(block.location - BLOCK , BLOCKSIZE2, BLOCKSIZE2);
	   this->draw_rectangle(block.location - BLOCK / 2 , BLOCKSIZE, BLOCKSIZE);
	 }
       
       this->set_render_color(FLOORCOLOR);
       //little debug squares
       this->draw_rectangle(this->active_tet.location-Yancey_Vector({2,2}),4,4);
       this->draw_rectangle(this->left_wall.location-Yancey_Vector({2,2}),4,4);
       this->draw_rectangle(this->right_wall.location-Yancey_Vector({2,2}),4,4);
       //walls, floor
       /*
 
	 this->draw_rectangle(this->floor.location - Yancey_Vector({this->floor.size.x/2, this->floor.size.y/2}),
			    this->floor.size.x,this->floor.size.y);
       this->draw_rectangle(this->ceiling.location - Yancey_Vector({this->ceiling.size.x/2, this->ceiling.size.y/2}),
			    this->ceiling.size.x,this->ceiling.size.y);      
       this->set_render_color(WALLCOLOR);
       this->draw_rectangle({BLOCKSIZE * -1,0},
			    this->left_wall.size.x,this->left_wall.size.y);	   
       this->draw_rectangle({this->right_wall.location.x - BLOCKSIZE,0},
			    this->right_wall.size.x,this->right_wall.size.y);	   
	*/		 
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
    this->color = YANCEYCOLOR_Turquoise;
    break;
  case 'j':
    locs = TETR_J  ;
    this->color = YANCEYCOLOR_Blue;
    break;    
  case 'l':
    locs = TETR_L  ;
    this->color = YANCEYCOLOR_Orange;
    break;
  case 'o':
    locs = TETR_O  ;
    this->color = YANCEYCOLOR_Yellow;
    break;
  case 's':
    locs = TETR_S  ;
    this->color = YANCEYCOLOR_Chartreuse;
    break;
  case 't':
    locs = TETR_T  ;
    this->color = YANCEYCOLOR_BrightPurple;
    break;    
  case 'z':
    locs = TETR_Z  ;
    this->color = YANCEYCOLOR_Red;
    break;        
  default:
    break;
  }
  int i=0;
  for(Yancey_Vector l : locs)
    {
    Tetrits_Block b(l);
    b.id = i;
    b.color=this->color;
    blocks.push_back(b);
    i++;
    }
}
Tetromino::Tetromino(){}//:Tetromino(SPAWN,'l'){}
Tetromino::Tetromino(const Tetromino &t):location(t.location),shape(t.shape){}
void Tetromino::rotate(bool cw)
{
this->orientation = this->orientation.get_normal(cw);
 for(Tetrits_Block &b : this->blocks)
   b.location = b.location.get_normal(cw);
}
    
