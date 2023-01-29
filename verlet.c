#include <math.h>
#include <stdlib.h>

typedef struct vec vec;
struct vec
{
    float x;
    float y;
};

typedef struct object object;
struct object
{
    vec p;
    vec p_old;
    vec a;
    float radius;
};

void updatePos(object* obj, float dt);
void applyGravity(object* obj, vec acc);
float getDistance(vec* a, vec* b);
object* initObject(float posx, float posy, float vecx, float vecy, float accx, float accy);
float length(vec* a);
void solveCollision(object* obj[], int objNum);

object* initObject(float posx, float posy, float vecx, float vecy, float accx, float accy){
  object* obj = malloc(sizeof(object));
  obj->p.x  = posx;
  obj->p.y = posy;
  obj->p_old.x = obj->p.x - vecx;
  obj->p_old.y = obj->p.y - vecy;
  obj->a.x = accx;
  obj->a.y = accy;
  obj->radius = 0.5;
  return obj;
}

void updatePos(object* obj, float dt){
  vec velocity = {.x = obj->p.x - obj->p_old.x, .y = obj->p.y - obj->p_old.y};
  obj->p_old.x = obj->p.x; obj->p_old.y = obj->p.y;
  obj->p.x = obj->p.x + velocity.x + obj->a.x*dt*dt;
  obj->p.y = obj->p.y + velocity.y + obj->a.y*dt*dt;
  obj->a.x = obj->a.y = 0;
}

void applyGravity(object* obj, vec grav){
  obj->a.x += grav.x;
  obj->a.y += grav.y;
}

float getDistance(vec* a, vec* b){
    return pow(pow((a->x - b->x), 2) + pow((a->y - b->y), 2), 0.5);
}

float length(vec* a){
    return pow(pow(a->x, 2) + pow(a->y, 2), 0.5);
}

void applyConstraint(object* obj, int maxX, int maxY){
  vec con_pos = {.x = maxX/2, .y = maxY/2-90};
  const float radius = 100;
  vec to_obj = {.x = obj->p.x - con_pos.x, .y = obj->p.y - con_pos.y};
  const float len = length(&to_obj);
  if(len > radius - obj->radius){
    vec norm = {.x = to_obj.x/len, .y = to_obj.y/len};
    obj->p.x = con_pos.x + norm.x*(radius - obj->radius);
    obj->p.y = con_pos.y + norm.y*(radius - obj->radius);
  }
}

void solveCollision(object* obj[], int objNum){
  for(int i=0;i<objNum;i++){
    for(int j = i+1;j<objNum;j++){
      vec collisionAxis  = {.x = obj[i]->p.x - obj[j]->p.x, .y = obj[i]->p.y - obj[j]->p.y};
      float distance = length(&collisionAxis);

      // if two objects distance smaller then their sum of radius 
      if(distance < (obj[i]->radius + obj[j]->radius)){
        vec normal = {.x = collisionAxis.x/distance, .y = collisionAxis.y/distance};
        float delta = (obj[i]->radius + obj[j]->radius) - distance;

        // move them back (not overlap)
        obj[i]->p.x += 0.5 * delta * normal.x;
        obj[i]->p.y += 0.5 * delta * normal.y;
        obj[j]->p.x -= 0.5 * delta * normal.x;
        obj[j]->p.y -= 0.5 * delta * normal.y;
      }
    }
  }
}

void solve(object* obj[], int objNum, int dt, vec gravity, int maxX, int maxY, int substeps){
  float sub_dt = dt/substeps;
  for(;substeps;substeps--){
    for(int i=0;i<objNum;i++){
      applyGravity(obj[i], gravity);
      applyConstraint(obj[i], maxX, maxY);
    }
    solveCollision(obj, objNum);
    for(int i=0;i<objNum;i++){
      updatePos(obj[i], sub_dt/1000.0);
    }
  }
}