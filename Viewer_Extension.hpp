/**
 * @file Viewer_Extension.hpp
 *
 * Author: Zhenyang Pan & Yaxiong Cai
 */


//***************//
//Listener struct//
//***************//


/** @class Pause_Listener
 * @brief Pause or unpause the visualizer
 * @right click: pause; right click again: unpause
 */
struct Pause_listener : public CS207::SDL_Listener {
  void handle(SDL_Event e) { 
    switch (e.type) {
      case SDL_MOUSEBUTTONDOWN: {
        if (e.button.button == SDL_BUTTON_RIGHT ) {
          if (dt_ == original_dt_)
            dt_ = 0;
          else
            dt_ = original_dt_;
        }
      } break;
    }
  }
  
  // constructor
  Pause_listener(double& dt) 
    : dt_(dt), original_dt_(dt) {}
  private:
   double& dt_;
   double original_dt_;
};




/** @class Speed_Listener
 * @brief Adjusts the simulation speed by keyboard.
 * @ Up: accelerate; Down: decelerate; Left: Recover the original speed
 */
struct Speed_listener : public CS207::SDL_Listener {
  private:
    double& dt_;
    double original_dt_;    
  
  public:  
    Speed_listener(double& dt, double  odt) 
      : dt_(dt), original_dt_(odt) {}

    virtual void handle(SDL_Event event) {
      if (event.type == SDL_KEYDOWN) { // keyboard key pressed
        switch (event.key.keysym.sym) {
          case SDLK_DOWN: // down arrow pressed
            dt_ -= 3e-4;
            break;
          case SDLK_UP: // up arrow pressed
            dt_ += 3e-4;
            break;
          case SDLK_LEFT: //recover dt
            dt_ = original_dt_;
            break;

          default: break; 
        }
      } 
  }
  
};  


/** @class XYZ_Listener
 * @brief Changes the position of the graph by keyboard
 * @ w, s: moving along x-axis; a, d: moving along y-axis
 */
template <typename M>
struct XYZ_listener : public CS207::SDL_Listener {

  virtual void handle(SDL_Event event) {

    if (event.type == SDL_KEYDOWN) { 
      switch (event.key.keysym.sym) {
        case SDLK_w: 
          for(auto b=(*mesh_).node_begin();b!=(*mesh_).node_end();++b){
            (*b).position().x += 0.5;
          }
          break;
        case SDLK_d:
          for(auto b=(*mesh_).node_begin();b!=(*mesh_).node_end();++b){
            (*b).position().y += 0.5;
          }
          break;
        case SDLK_s: 
          for(auto b=(*mesh_).node_begin();b!=(*mesh_).node_end();++b){
            (*b).position().x -= 0.5;
          }
          break;
        case SDLK_a:
          for(auto b=(*mesh_).node_begin();b!=(*mesh_).node_end();++b){
            (*b).position().y -= 0.5;
          }
          break;
        case SDLK_z: 
          for(auto b=(*mesh_).node_begin();b!=(*mesh_).node_end();++b){
            (*b).position().z += 0.1;
            std::cout<<(*b).position()<<std::endl;
          }
          break;
        case SDLK_x:
          for(auto b=(*mesh_).node_begin();b!=(*mesh_).node_end();++b){
            (*b).position().z -= 0.1;
            std::cout<<(*b).position()<<std::endl;
          }
          break;
          
        default: break; 
      }
    } 
  }
  
  XYZ_listener(M* mesh) 
    : mesh_(mesh){}
    
  private:
   M* mesh_;
};


/** @class Color_Listener
 * @brief Adjusts the color by keyboard.
 * @ Number key 1 2 3 4 5 6 7: pink, blue, red, cyan, yellow, green 
 */
struct Color_listener : public CS207::SDL_Listener {

  virtual void handle(SDL_Event event) {

    if (event.type == SDL_KEYDOWN) { 
      switch (event.key.keysym.sym) {
        case SDLK_1: 
          *a_ = 1-*a_;
          break;
        case SDLK_2: 
          *b_ = 1-*b_;
          break;
        case SDLK_3: 
          *c_ = 1-*c_;
          break;
        default: break; 
      }
    } 
  }
  
  Color_listener(int* a, int* b, int* c) 
    : a_(a), b_(b), c_(c){}
    
  private:
   int* a_;
   int* b_;
   int* c_;
};

// Color struct
struct color{
  int a_;
  int b_;
  int c_;
  color(int a, int b, int c): a_(a), b_(b), c_(c){}
  
  template <typename NODE>
  CS207::Color operator()(const NODE& n) {
    return CS207::Color(a_, b_, c_); 
  }
};


struct Force_listener : public CS207::SDL_Listener {
  private:
    int* a_;
    int* b_;    
    int* c_;
  
  public:  
    Force_listener(int* a, int* b, int* c): a_(a), b_(b), c_(c){}

    virtual void handle(SDL_Event event) {
      if (event.type == SDL_KEYDOWN) { // keyboard key pressed
        switch (event.key.keysym.sym) {
          case SDLK_f: 
            // a_ += 550;
            *b_ += 550;
            *c_ += 550;
            break;
          case SDLK_r: 
            *a_ = 0;
            *b_ = 0;
            *c_ = 0;
            break;

          default: break; 
        }
      } 
  }
  
};  




/** @class Move_listener
 * @brief Move one sphere towards another shpere 
 * @ key: m
 */
template <typename M>
struct Move_listener : public CS207::SDL_Listener {

  /**functions to move the sphere: find the center of two sphere and then 
   * set the velocity of the sphere to be moved to the vector of the two center
   * @post for all n belongs to @mesh1_, n.value().velocity is changed toward to @mesh2_ center
   */
  virtual void handle(SDL_Event event) {
      if (event.type == SDL_KEYDOWN) { // keyboard key pressed
        switch (event.key.keysym.sym) {
          case SDLK_m:{ 
            int obj1_x = 0;
            int obj1_y = 0;
            int obj1_z = 0;
            int obj2_x = 0;
            int obj2_y = 0;
            int obj2_z = 0;
            int obj1_n = 0;
            int obj2_n = 0;
            
            
            //compute the center point for mesh1
            for(auto b=(*mesh1_).node_begin();b!=(*mesh1_).node_end();++b){
              obj1_x += (*b).position().x;
              obj1_y += (*b).position().y;
              obj1_z += (*b).position().z;
              obj1_n += 1;
            }
            
            obj1_x /= obj1_n;
            obj1_y /= obj1_n;
            obj1_z /= obj1_n;  
            
           //compute the center point for mesh2
           for(auto b=(*mesh2_).node_begin();b!=(*mesh2_).node_end();++b){
              obj2_x += (*b).position().x;
              obj2_y += (*b).position().y;
              obj2_z += (*b).position().z;
              obj2_n += 1;
            }
            
            obj2_x /= obj2_n;
            obj2_y /= obj2_n;
            obj2_z /= obj2_n;
            
            obj1_x = obj2_x - obj1_x;
            obj1_y = obj2_y - obj1_y;
            obj1_z = obj2_z - obj1_z;
            
            obj1_x *= 10;
            obj1_y *= 10;
            obj1_z *= 10;
            
            //set the velocity of mesh1
            for(auto b=(*mesh1_).node_begin();b!=(*mesh1_).node_end();++b){
              (*b).value().velocity = Point(obj1_x, obj1_y, obj1_z);
            }
            }
           break;
            
          default: break; 
        }
      } 
  }
  
  Move_listener(M* mesh1, M* mesh2) 
  : mesh1_(mesh1), mesh2_(mesh2){}

  private:
   //the object to move
   M* mesh1_;
   M* mesh2_;
};
