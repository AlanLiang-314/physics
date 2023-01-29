#include <ncurses.h>
#include "verlet.c"
#include "util.h"

#define FRAME_PER_SECOND 30

//int msleep(long msec);
void drawSquare(int y, int x);

int main() {
  float dt = 1000.0/FRAME_PER_SECOND;
  int ch, max_y, max_x, gameStatus = 1;
  const int totalObjNum = 500;
  int objNum = 0;
  long long t = 0;
  vec gravity = {.x = 0, .y = 100};
  object* square[totalObjNum+10];

  initscr();            
  timeout(0);          
  raw();                
  keypad(stdscr, TRUE); 
  noecho();             
  curs_set(FALSE);
  if(has_colors() == FALSE)
	{	endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color();
  getmaxyx(stdscr, max_y, max_x);
  init_pair(3,COLOR_GREEN,COLOR_BLACK);
  square[0] = initObject(max_x/2+20, 5, 0, 0, 0, 0);

  while (gameStatus) {
    msleep(dt);
    ch = getch();

    //get input from user
    switch (ch)
    {
    case '\n':
      gameStatus = 0;
      break;
    default:
      break;
    }

    if(objNum<totalObjNum && t%10 == 0){
      square[objNum++] = initObject(max_x/2+(objNum%40)-20, 5, 0, -1, 0, 0);
    }

    //physics engine
    solve(square, objNum, dt, gravity, max_x, max_y, 5);
    //if(t%2 == 0) clearAcceleration(square, objNum);

    //draw object
    clear();
    attron(COLOR_PAIR(3));
    mvprintw(square[0]->p.y, square[0]->p.x,"O");
    attroff(COLOR_PAIR(3));
    for(int i=1;i<objNum;i++){
      mvprintw(square[i]->p.y, square[i]->p.x,"O");
    }
    mvprintw(0, 0,"The pressed key is %c", ch);
    mvprintw(2, 0,"current t is %lld", t);
    mvprintw(4, 0,"current p.x is %f, p.y is %f", square[0]->p.x, square[0]->p.y);
    mvprintw(6, 0,"current p_old.x is %f, p_old.y is %f", square[0]->p_old.x, square[0]->p_old.y);
    refresh();
    t++;
  }

  endwin();  /* End curses mode		  */

  return 0;
}

void drawSquare(int y, int x){
  mvprintw(y-2, x,"    @    ");
  mvprintw(y-1, x,"  @ @ @  ");
  mvprintw(y  , x,"@ @ @ @ @");
  mvprintw(y+1, x,"  @ @ @  ");
  mvprintw(y+2, x,"    @    ");
}