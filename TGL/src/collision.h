#ifndef TILE_COLLISIONS
#define TILE_COLLISIONS

bool collision_with_point(SDL_Surface *s1,float x1,float y1,int hot_x1,int hot_y1,float a1,float scale1,float x2,float y2);
bool collision(SDL_Surface *s1,float x1,float y1,int hot_x1,int hot_y1,float a1,float scale1,SDL_Surface *s2,float x2,float y2,int hot_x2,int hot_y2,float a2,float scale2);
bool collision_vector(SDL_Surface *s1,float x1,float y1,int hot_x1,int hot_y1,float a1,float scale1,SDL_Surface *s2,float x2,float y2,int hot_x2,int hot_y2,float a2,float scale2,float *collision_vector_x,float *collision_vector_y);

#endif