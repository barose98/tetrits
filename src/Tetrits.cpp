#include "Tetrits.h"
#include "debug_config.h"
std::vector<Yancey_Vector> TETR_I = { {-1,-1}, {-3,-1}, {1,-1}, {3,-1}};
std::vector<Yancey_Vector> TETR_J = { {0,0}, {-2,0}, {2,0}, {-2, -2}};
std::vector<Yancey_Vector> TETR_L = { {0,0}, {-2,0}, {2,0}, {2, -2}};
std::vector<Yancey_Vector> TETR_O = { {-1,-1}, {1,-1}, {-1,1}, {1,1}};
std::vector<Yancey_Vector> TETR_S = { {0,0}, {-2,0}, {0,-2}, {2,-2}};
std::vector<Yancey_Vector> TETR_T = { {0,0}, {-2,0}, {2,0}, {0,-2}};
std::vector<Yancey_Vector> TETR_Z = { {0,0}, {2,0}, {0,-2}, {-2,-2}};

float BLOCKRAD = sqrt(BLOCKSIZE * BLOCKSIZE * 2);

Tetrits_Block::Tetrits_Block(){}
Tetrits_Block::Tetrits_Block(const Tetrits_Block &t):Yancey_rect(t.id,t.solid,t.size,t.location)
{
  this->color=t.color;
}
Tetrits_Block::~Tetrits_Block(){}
Tetrits_Block::Tetrits_Block(Yancey_Vector loc, Yancey_Vector size):Yancey_rect(0,true,size,loc) { }

Tetrits::Tetrits() : PARENTGAME("tetrits",WIND_W,WIND_H,FRAMERATE){}
Tetrits::~Tetrits(){}
bool Tetrits::init(uint32_t fl)
{
  PARENTGAME::init(WINDOWFLAGS);

  this->init_floor();
  std::srand(time(0));
  this->next_tet = Tetromino(NEXTLOC, BLOCK_N * 2, this->shapes[std::round(std::rand() % 7)]);
  this->spawn();
  this->reset();

  return true;
}

void Tetrits::reset()
{
  this->spawn();
  this->obstacles.clear();
  this->update_score(0);
}

bool Tetrits::spawn()
{
  for(Tetrits_Block &b : this->active_tet.blocks)
    {
      b.location = b.location * BLOCKSIZE + this->active_tet.location;
      this->obstacles.push_back(b);
    }
  Yancey_Vector  spawn;  
  switch(this->next_tet.shape){
  case 'i':
  case 'o':
    spawn = SPAWN_IO;
    break;
  default:
    spawn = SPAWN_JLSTZ;
  }
  Yancey_Vector spawnadj = {WIND_W % (WIND_W - int(this->falls.x)), WIND_H % (WIND_H - int(this->falls.y))};
  this->active_tet = Tetromino(spawn, BLOCK * 2, this->next_tet.shape);

  this->next_tet = Tetromino(NEXTLOC * BLOCKSIZE, BLOCK_N * 2, this->shapes[std::round(std::rand() % 7)]);
  return true;
}


bool Tetrits::hits_obstacle(Tetromino &t, std::vector<Yancey_Vector> vext)
{
  return std::any_of(vext.begin(), vext.end(), [&t,this](Yancey_Vector e){ return this->hits_obstacle(t,e);});
}
bool Tetrits::hits_obstacle(Tetromino &t, Yancey_Vector ext)
{
  return std::any_of(this->obstacles.begin(), this->obstacles.end(), [&t,ext](Tetrits_Block b){ return t.hits_block(b,ext);});
}
bool Tetrits::within_bounds(Tetromino &t, Yancey_Vector &ext)
{
  bool ret = std::all_of(t.blocks.begin(), t.blocks.end(), [this,&t,&ext](Tetrits_Block b){
      b.location *= BLOCKSIZE;
      b.location += t.location;      
      bool ret2 = b.location.x > 0 && b.location.x < WIND_W && b.location.y < WIND_H;
      if(!ret2) ext = b.location;       
      return ret2;
    });
      return ret;
}

bool Tetromino::hits_block(Yancey_rect other, Yancey_Vector ext)
{
  Yancey_Vector overlap = {0,0};
  bool ret = std::any_of(this->blocks.begin(), this->blocks.end(), [this,ext,&other,&overlap](Tetrits_Block b){
      b.location *= BLOCKSIZE;
      b.location += this->location; 
      return b.collides_with(other,overlap);      
    });
     
 if(ret) {
   this->location.y -= overlap.y/2 + BLOCKSIZE;
  }
   
   
  return ret;
 }

void Tetrits::settle()
{
  auto row_compare = [] (Tetrits_Block &b1, Tetrits_Block &b2) {return b1.location.y < b2.location.y;};
  std::sort(this->obstacles.begin(),this->obstacles.end(),row_compare);
  
  for(int i = WIND_H - BLOCKSIZE;i > 0 ;i -= BLOCKSIZE2)
    {
      auto lambda_obj = [i] (Tetrits_Block b) {return b == i;};      
      int count = std::count_if(this->obstacles.begin(), this->obstacles.end(), lambda_obj);
      // row i is empty
      if(count == 0){
	break;
      }
      // row i is full
      else  if(count == 10)
	{
	  auto row_start = std::find_if(this->obstacles.begin(), this->obstacles.end(), lambda_obj);
	  this->obstacles.erase(row_start,row_start+count);
	  std::for_each(this->obstacles.begin(),row_start,[](Tetrits_Block &b) {b.location += SETTLE;});
	  this->score += count;
	  this->update_score(this->score);
	}
      
    }

}
bool Tetrits::update()
{
  if(PARENTGAME::frames.ready && !this->paused)
    {
      // this->update_score(PARENTGAME::frames.count);
      
       this->render_clear( GAMEBACKGROUND );
       this->set_render_color(BLOCKCOLOR);
	this->settle();
#ifdef FALLING
	if(!this->move_active(this->falls))
	      this->spawn();	   
#endif       

       this->draw(this->active_tet);
       this->draw(this->next_tet);
       
       for(Tetrits_Block block : this->obstacles)
	 {
	   this->set_render_color(block.color);
	   this->draw_rectangle(block.location - BLOCK , BLOCKSIZE2, BLOCKSIZE2);
	   this->draw_rectangle(block.location - BLOCK / 2 , BLOCKSIZE, BLOCKSIZE);
	 }
       
       this->set_render_color(FLOORCOLOR);
       //little debug squares
       this->draw_rectangle(this->active_tet.location-Yancey_Vector({2,2}),4,4);
       
       //walls, floor
        this->draw_rectangle(this->floor.location - this->floor.size/2, this->floor.size.x,this->floor.size.y);
	
	//score
	this->draw_score();
	
       this->render_present(0);

       PARENTGAME::frames.ready = false;
    }
  return true;
}


Tetromino::Tetromino(Yancey_Vector location, Yancey_Vector blocksize, uint8_t shape):location(location),shape(shape),orientation({1,0})
{
  std::vector<Yancey_Vector> locs;
  switch(shape){
  case 'i':
    locs = TETR_I;
    this->color = YANCEYCOLOR_Cyan;
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
      Tetrits_Block b(l,blocksize);
      b.id = i;
      b.color=this->color;
      this->blocks.push_back(b);
      i++;
    }
}
Tetromino::Tetromino(){}

Tetromino::Tetromino(const Tetromino &t):location(t.location),shape(t.shape),blocks(t.blocks){}

void Tetrits::rotate_active(bool cw)
{  
  Tetromino test = this->active_tet;
  test.rotate(cw);
  Yancey_Vector bloc = {0,0};
  bool ib =this->within_bounds(test, bloc);    
  if(ib)
     {
       if(!hits_obstacle(test,{EXT_L,EXT_R,EXT_D}))
	   this->active_tet.rotate(cw);	 
     }else{
	Yancey_Vector move = {0,0};
	if(bloc.x < 0)     test.location.x  += (0- bloc.x)*2;
	if(bloc.x > WIND_W)test.location.x  += (WIND_W - bloc.x)*2;
	if(bloc.y > WIND_H)test.location.y  += (WIND_H - bloc.y)*2;
	
       if(!hits_obstacle(test,{EXT_L,EXT_R,EXT_D}))
	 {	   
	   log_i<<WIND_H <<'-'<< bloc.y*2 <<'='<< move.x  <<std::endl;
	   this->active_tet.rotate(cw);
	   this->active_tet.location = test.location;
	 }
       }
}
bool Tetrits::move_active(Yancey_Vector move)
  {
      Tetromino test = this->active_tet;
      test.location += move;
      Yancey_Vector adj = {0,0};
      if(this->within_bounds(test,adj) && !this->hits_obstacle(test,adj))
	{
	this->active_tet.location += move;
	return true;
	}else{
	return false;
      }
  }
void Tetromino::rotate(bool cw)
{  
this->orientation = this->orientation.get_normal(cw);
for(Tetrits_Block &b : this->blocks)
   b.location = b.location.get_normal(cw);
}
    
void Tetrits::draw_score()
{
  this->set_render_color(SCORECOLOR);
  // this->draw_rectangle(SCORELOC - SCORESIZE/2, SCORESIZE.x, SCORESIZE.y);
  int pl = SCORELOC.x + SCORESIZE.x/2 - DIGITSIZE.x;
  for(Yancey_Digit d : this->score_digits)
    {      
      d.location = {pl,SCORELOC.y};

      //this->draw_rectangle(d.location - Yancey_Vector({2,2}), 4,4);
      for(Yancey_Segment s : d.segments)
	{
	  if(s.illum)
	    {      
	      std::vector<Yancey_Vector> p =  s.get_points();
	      for(Yancey_Vector &v : p){
		v += d.location;
		//if(s.location.x == pl)
		//log_i<<pl<<' '<<v<<std::endl;
	      }
	      
	      this->draw_lines(p);
	      //this->draw_rectangle(s.location - Yancey_Vector({2,2}), 4,4);
	      //this->draw_rectangle(s.location - s.size/2, s.size.x,s.size.y);
	    }
	  
	}
      pl -= DIGITSIZE.x*2 +8;
    }
}
void Tetrits::update_score(int score)
{
  this->score = score;
  this->score_digits.clear();
   uint8_t digit_array[] = {this->score % 10, this->score / 10 % 10, this->score / 100 % 10};
   for(int i = 0;i<3;i++)
     {
       Yancey_Digit d(digit_array[i],DIGITSIZE);
       this->score_digits.push_back(d);  
       //
     }
     
   
}

void Tetrits::draw(Tetromino &t)
{
  this->set_render_color(t.color);
       for(Tetrits_Block block : t.blocks)
	 {
	   block.location = block.location * block.size.x /2 + t.location;
	   this->draw_rectangle(block.location - block.size / 2 , block.size.x, block.size.y);
	   this->draw_rectangle(block.location - block.size / 4 , block.size.x /2, block.size.y /2);
	 }

}
