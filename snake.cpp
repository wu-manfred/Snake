/*
CS 349 A1 Skeleton Code - Snake

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o snake snake.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./snake

Note: the -L option and -lstdc++ may not be needed on some machines.
*/

//TODO arrow key inputs
// possibly 2 player

#include <iostream>
#include <list>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>

#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

/*
 * Global game state variables
 */
const int Border = 1;
const int BufferSize = 10;
int FPS = 30;
int speed = 5;
int orig_speed = speed;
const int width = 800;
const int height = 600;
const int PREGAME = 0;
const int INGAME = 1;
const int POSTGAME = 2;
int state = 0; // 0 - pregame, 1 - ingame, 2 - postgame
bool turned = false;
int score = 0;
bool paused = false;
bool restart = false;
const int x_lower = 53;
const int x_upper = 747;
const int y_lower = 52;
const int y_upper = 548;
const int x_midu = 405;
const int x_midl = 395;
const int y_midu = 305;
const int y_midl = 295;
const int NWtoSW = 219; 
const int SWtoSE = 416; 
const int SEtoNE = 571; 
const int NEtoNW = 174; 
const int UP = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int LEFT = 3;
const int SPACING = 11;
int orientation = RIGHT;
bool in_tunnel = false;
const int NW = 0;
const int NE = 1;
const int SE = 2;
const int SW = 3;

// const int

/*
 * Information to draw on the window.
 */
struct XInfo {
        Display  *display;
        int              screen;
        Window   window;
        Pixmap   pixmap;
        GC               gc[3];
        int             width;          // size of window
        int             height;
};


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  cerr << str << endl;
  exit(0);
}


/*
 * An abstract class representing displayable things. 
 */
class Displayable {
        public:
                virtual void paint(XInfo &xinfo) = 0;
};

class TextDisplay : public Displayable {
        public:
                virtual void paint(XInfo &xinfo){
                        stringstream ss_temp;
                        ss_temp << score;
                        string score_tracker = "Score: ";
                        score_tracker += ss_temp.str();
                        int length = score_tracker.length() + 1;
                        char * score_tracker_c = new char[length];
                        strcpy(score_tracker_c, score_tracker.c_str());
                        ss_temp.str("");
                        ss_temp.clear();

                        ss_temp << FPS;
                        string FPS_tracker = "FPS: ";
                        FPS_tracker += ss_temp.str();
                        length = FPS_tracker.length() + 1;
                        char * FPS_tracker_c = new char[length];
                        strcpy(FPS_tracker_c, FPS_tracker.c_str());
                        ss_temp.str("");
                        ss_temp.clear();

                        ss_temp << speed;
                        string speed_tracker = "Speed: ";
                        speed_tracker += ss_temp.str();
                        length = speed_tracker.length() + 1;
                        char * speed_tracker_c = new char[length];
                        strcpy(speed_tracker_c, speed_tracker.c_str());

                        // drawing borders
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_lower, y_lower, x_midl, y_lower);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midu, y_lower, x_upper, y_lower);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_lower, y_upper, x_midl, y_upper);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midu, y_upper, x_upper, y_upper);

                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_lower, y_lower, x_lower, y_midl);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_lower, y_midu, x_lower, y_upper);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_upper, y_lower, x_upper, y_midl);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_upper, y_midu, x_upper, y_upper);

                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_lower, y_midl, x_midl, y_midl);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midu, y_midl, x_upper, y_midl);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_lower, y_midu, x_midl, y_midu);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midu, y_midu, x_upper, y_midu);                       

                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midl, y_lower, x_midl, y_midl);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midl, y_midu, x_midl, y_upper);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midu, y_lower, x_midu, y_midl);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midu, y_midu, x_midu, y_upper);

                        XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 50, 565, score_tracker_c, strlen(score_tracker_c));
                        XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 700, 565, FPS_tracker_c, strlen(FPS_tracker_c));
                        XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[0], 700, 585, speed_tracker_c, strlen(speed_tracker_c));

                        XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], NWtoSW, (y_midl - 10), 10, 10);
                        XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], NWtoSW, y_midu, 10, 10);
                        XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], SEtoNE, (y_midl - 10), 10, 10);
                        XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], SEtoNE, y_midu, 10, 10);
                        XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], (x_midl - 10), SWtoSE, 10, 10);
                        XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midu, SWtoSE, 10, 10);
                        XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], (x_midl - 10), NEtoNW, 10, 10);
                        XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midu, NEtoNW, 10, 10); 
                        XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], (x_midl - 10), y_midu, 10, 10);
                        XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midu, y_midu, 10, 10);
                        XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], (x_midl - 10), (y_midl - 10), 10, 10);
                        XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midu, (y_midl - 10), 10, 10);

                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], NWtoSW + 5, y_midl, NWtoSW + 5, y_midu);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], SEtoNE + 5, y_midl, SEtoNE + 5, y_midu);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midl, NEtoNW + 5, x_midu, NEtoNW + 5);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midl, SWtoSE + 5, x_midu, SWtoSE + 5);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midl, y_midl, x_midu, y_midu);
                        XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[0], x_midl, y_midu, x_midu, y_midl);
                }                
};

class Fruit : public Displayable {
        public:
                virtual void paint(XInfo &xinfo) {
                        XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], x, y, 10, 10);
                }

        Fruit() {
            srand(time(NULL));
            bool success = false;
            int region;
            while (!success){
                region = rand() % 4; 
                if (region == NW){
                    x = x_lower;
                    y = y_lower;
                } else if (region == NE){
                    x = x_midu;
                    y = y_lower;
                } else if (region == SE){
                    x = x_midu;
                    y = y_midu;
                } else{
                    x = x_lower;
                    y = y_midu;
                }
                x += (((rand() % 31) * 11) + 1);
                y += (((rand() % 22) * 11) + 1);
                success = true;
                if ((x > (x_midl - 20) && x < x_midu + 10) || (y > (y_midl - 20) && y < y_midu + 10)) success = false;
                if ((x == 98 && y == 504) || (x == 87 && y == 504)){
                    success = false;
                }
            }
        }

        int getX(){
            return x;
        }

        int getY(){
            return y;
        }

        void new_fruit(vector<int> xs, vector<int> ys, int size){
            // x_midl - 10 <-> x_midu
            // y_midl - 10 <-> y_midu

            // cout << "new fruit" << endl;
            bool success = false;
            int region;
            while(!success){
                region = rand() % 4; 
                if (region == NW){
                    x = x_lower;
                    y = y_lower;
                } else if (region == NE){
                    x = x_midu;
                    y = y_lower;
                } else if (region == SE){
                    x = x_midu;
                    y = y_midu;
                } else{
                    x = x_lower;
                    y = y_midu;
                }
                x += (((rand() % 31) * 11) + 1);
                y += (((rand() % 22) * 11) + 1);            
                success = true;
                if ((x > (x_midl - 20) && x < x_midu + 10) || (y > (y_midl - 20) && y < y_midu + 10)) success = false;

                for (int i = 0; i<size; i++){
                    if (xs[i] == x && ys[i] == y){
                        success = false;
                    }
                }
            }
        }

    private:
        int x;
        int y;
};

Fruit fruit;
TextDisplay text_display;

class Snake : public Displayable {
        public:
                virtual void paint(XInfo &xinfo) {
                    for (int i = 0; i<size; i++){
                        XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], xlist[i], ylist[i], blockSize, blockSize);
                    }
                }

                void move(XInfo &xinfo) {
                    turned = false;
                    int grow_x = xlist.back();
                    int grow_y = ylist.back();

                    // moving head of snake
                    int hitbox = 5;
                    if ((abs(xlist[0] - NWtoSW) < hitbox) && (abs(ylist[0] - y_midu) < hitbox && !in_tunnel)){
                        xlist.insert(xlist.begin(), NWtoSW);
                        ylist.insert(ylist.begin(), (y_midl - 10));
                        area = NW;
                        in_tunnel = true;
                    } else if ((abs(xlist[0] - NWtoSW) < hitbox) && (abs(ylist[0] - (y_midl - 10)) < hitbox) && !in_tunnel){
                        xlist.insert(xlist.begin(), NWtoSW);
                        ylist.insert(ylist.begin(), y_midu);
                        area = SW;
                        in_tunnel = true;
                    } else if ((abs(xlist[0] - SEtoNE) < hitbox) && (abs(ylist[0] - y_midu) < hitbox) && !in_tunnel){
                        xlist.insert(xlist.begin(), SEtoNE);
                        ylist.insert(ylist.begin(), (y_midl - 10));
                        area = NE;
                        in_tunnel = true;
                    } else if ((abs(xlist[0] - SEtoNE) < hitbox) && (abs(ylist[0] - (y_midl - 10)) < hitbox) && !in_tunnel){
                        xlist.insert(xlist.begin(), SEtoNE);
                        ylist.insert(ylist.begin(), y_midu);
                        area = SE;
                        in_tunnel = true;
                    } else if ((abs(xlist[0] - (x_midl - 10)) < hitbox) && (abs(ylist[0] - NEtoNW) < hitbox) && !in_tunnel){
                        xlist.insert(xlist.begin(), x_midu);
                        ylist.insert(ylist.begin(), NEtoNW);
                        area = NE;
                        in_tunnel = true;
                    } else if ((abs(xlist[0] - x_midu) < hitbox) && (abs(ylist[0] - NEtoNW) < hitbox) && !in_tunnel){
                        xlist.insert(xlist.begin(), (x_midl - 10));
                        ylist.insert(ylist.begin(), NEtoNW);
                        area = NW;
                        in_tunnel = true;
                    } else if ((abs(xlist[0] - (x_midl - 10)) < hitbox) && (abs(ylist[0] - SWtoSE) < hitbox) && !in_tunnel){
                        xlist.insert(xlist.begin(), x_midu);
                        ylist.insert(ylist.begin(), SWtoSE);
                        area = SE;
                        in_tunnel = true;
                    } else if ((abs(xlist[0] - x_midu) < hitbox) && (abs(ylist[0] - SWtoSE) < hitbox) && !in_tunnel){ 
                        xlist.insert(xlist.begin(), (x_midl - 10));
                        ylist.insert(ylist.begin(), SWtoSE);
                        area = SW;
                        in_tunnel = true;
                    } else if ((abs(xlist[0] - (x_midl - 10)) < hitbox) && (abs(ylist[0] - y_midu) < hitbox) && !in_tunnel){
                        xlist.insert(xlist.begin(), x_midu);
                        ylist.insert(ylist.begin(), (y_midl - 10));
                        area = NE;
                        in_tunnel = true;
                    } else if ((abs(xlist[0] - x_midu) < hitbox) && (abs(ylist[0] - (y_midl - 10)) < hitbox) && !in_tunnel){
                        xlist.insert(xlist.begin(), (x_midl - 10));
                        ylist.insert(ylist.begin(), y_midu);
                        area = SW;
                        in_tunnel = true;
                    } else if ((abs(xlist[0] - (x_midl - 10)) < hitbox) && (abs(ylist[0] - (y_midl - 10)) < hitbox) && !in_tunnel){
                        xlist.insert(xlist.begin(), x_midu);
                        ylist.insert(ylist.begin(), y_midu);
                        area = SE;
                        in_tunnel = true;
                    } else if ((abs(xlist[0] - x_midu) < hitbox) && (abs(ylist[0] - y_midu) < hitbox) && !in_tunnel){ 
                        xlist.insert(xlist.begin(), (x_midl - 10));
                        ylist.insert(ylist.begin(), (y_midl - 10));
                        area = NW;
                        in_tunnel = true;
                    } else {
                        if (orientation == UP){
                            xlist.insert(xlist.begin(), xlist[0]);
                            ylist.insert(ylist.begin(), ylist[0] - SPACING);
                        } else if (orientation == RIGHT){
                            xlist.insert(xlist.begin(), xlist[0] + SPACING);
                            ylist.insert(ylist.begin(), ylist[0]);
                        } else if (orientation == DOWN){
                            xlist.insert(xlist.begin(), xlist[0]);
                            ylist.insert(ylist.begin(), ylist[0] + SPACING);
                        } else {
                            xlist.insert(xlist.begin(), xlist[0] - SPACING);
                            ylist.insert(ylist.begin(), ylist[0]);
                        }   

                        in_tunnel = false;

                        if (area == SW){
                            if (ylist[0] < y_midu || ylist[0] > (y_upper - 10) || xlist[0] < x_lower || xlist[0] > (x_midl - 10)){
                                didHitObstacle();
                            }
                        } else if (area == SE){
                            if (ylist[0] < y_midu || ylist[0] > (y_upper - 10) || xlist[0] < x_midu || xlist[0] > (x_upper - 10)){
                                didHitObstacle();
                            }
                        } else if (area == NE){
                            if (ylist[0] < y_lower || ylist[0] > (y_midl - 10) || xlist[0] < x_midu || xlist[0] > (x_upper - 10)){
                                didHitObstacle();
                            }
                        } else{
                            if (ylist[0] < y_lower || ylist[0] > (y_midl - 10) || xlist[0] < x_lower || xlist[0] > (x_midl - 10)){
                                didHitObstacle();
                            }
                        }
                    }
                
                    xlist.pop_back();
                    ylist.pop_back();

                    // checking if snake ate a fruit 
                    if ((abs(xlist[0] - fruit.getX()) < 8) && (abs(ylist[0] - fruit.getY()) < 8)){
                        didEatFruit(grow_x, grow_y);
                    } 

                    for (int i = 1; i<size; i++){
                        if ((abs(xlist[0] - xlist[i]) < 5) && (abs(ylist[0] - ylist[i]) < 5)){
                            didHitObstacle();
                        }
                    }

                    // checking if snake hit a wall


            }

                vector<int> getXs() {
                    return xlist;
                }

                vector<int> getYs() {
                    return ylist;
                }

                int getSize() {
                    return size;
                }

        /*
         * ** ADD YOUR LOGIC **
         * Use these placeholder methods as guidance for implementing the snake behaviour. 
         * You do not have to use these methods, feel free to implement your own.
         */
        void didEatFruit(int new_x, int new_y) {
            size++;
            score++;
            speed = orig_speed + (score / 5);
            xlist.push_back(new_x);
            ylist.push_back(new_y);
            fruit.new_fruit(xlist, ylist, size);
        }

        void didHitObstacle() {
            state = POSTGAME;
        }

        void turnLeft() {
            if (turned == true) return;
            orientation--;
            if (orientation == -1){
                orientation = LEFT;
            }  
            turned = true;
        }

        void turnRight() {
            if (turned == true) return;
            orientation++;
            if (orientation == 4){
                orientation = UP;
            }
            turned = true;
        }

        void reset() {
            xlist.clear();
            ylist.clear();
            xlist.push_back(98);
            xlist.push_back(98-SPACING);
            ylist.push_back(450);
            ylist.push_back(450);
            size = 2;
            area = SW;
        }

        Snake(int x, int y) {
            xlist.push_back(x);
            xlist.push_back(x-SPACING);
            ylist.push_back(y);
            ylist.push_back(y);
            blockSize = 10;
            size = 2;
            area = SW;
        }

        private:
                vector<int> xlist;
                vector<int> ylist;
                int blockSize;
                int size;
                int area;
};

list<Displayable *> dList;           // list of Displayables
Snake snake(98, 504);
//Fruit fruit;


/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xInfo) {
        XSizeHints hints;
        unsigned long white, black;
        if (argc == 3){
            istringstream(argv[1]) >> FPS;
            istringstream(argv[2]) >> speed;
        }
        orig_speed = speed;
   /*
        * Display opening uses the DISPLAY      environment variable.
        * It can go wrong if DISPLAY isn't set, or you don't have permission.
        */
        xInfo.display = XOpenDisplay( "" );
        if ( !xInfo.display )   {
                error( "Can't open display." );
        }

        /*
        * Find out some things about the display you're using.
        */
        xInfo.screen = DefaultScreen( xInfo.display );

        white = XWhitePixel( xInfo.display, xInfo.screen );
        black = XBlackPixel( xInfo.display, xInfo.screen );

        hints.x = 100;
        hints.y = 100;
        hints.width = 800;
        hints.height = 600;
        hints.flags = PPosition | PSize;


        xInfo.window = XCreateSimpleWindow(
                xInfo.display,                          // display where window appears
                DefaultRootWindow( xInfo.display ), // window's parent in window tree
                hints.x, hints.y,                       // upper left corner location
                hints.width, hints.height,      // size of the window
                Border,                                         // width of window's border
                black,                                          // window border colour
                white );                                        // window background colour

        XSetStandardProperties(
                xInfo.display,          // display containing the window
                xInfo.window,           // window whose properties are set
                "animation",            // window's title
                "Animate",                      // icon's title
                None,                           // pixmap for the icon
                argv, argc,                     // applications command line args
                &hints );                       // size hints for the window

        int depth = DefaultDepth(xInfo.display, DefaultScreen(xInfo.display));
        xInfo.pixmap = XCreatePixmap(xInfo.display, xInfo.window, hints.width, hints.height, depth);

        /* 
         * Create Graphics Contexts
         */
        int i = 0;
        xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
        XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
        XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
        XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
        XSetLineAttributes(xInfo.display, xInfo.gc[i],
                             1, LineSolid, CapButt, JoinRound);

        i++;
        xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
        XSetForeground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
        XSetBackground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
        XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
        XSetLineAttributes(xInfo.display, xInfo.gc[i],
                             1, LineSolid, CapButt, JoinRound);


        XSelectInput(xInfo.display, xInfo.window,
                ButtonPressMask | KeyPressMask |
                PointerMotionMask |
                EnterWindowMask | LeaveWindowMask |
                StructureNotifyMask);  // for resize events

        /*
         * Put the window on the screen.
         */
        XMapRaised( xInfo.display, xInfo.window );
        XFlush(xInfo.display);
}

/*
 * Function to repaint a display list
 */
void repaint( XInfo &xinfo) {
        list<Displayable *>::const_iterator begin = dList.begin();
        list<Displayable *>::const_iterator end = dList.end();

        XWindowAttributes windowInfo;
        XGetWindowAttributes(xinfo.display, xinfo.window, &windowInfo);
        unsigned int height = windowInfo.height;
        unsigned int width = windowInfo.width;

        // XClearWindow( xinfo.display, xinfo.window );
        // get height and width of window (might have changed since last repaint)
        XSetForeground(xinfo.display, xinfo.gc[0], WhitePixel(xinfo.display, DefaultScreen(xinfo.display)));
        XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 0, 0, width, height);

        // big black rectangle to clear background
        XSetForeground(xinfo.display, xinfo.gc[0], BlackPixel(xinfo.display, DefaultScreen(xinfo.display)));
        // draw display list
        while( begin != end ) {
                Displayable *d = *begin;
                d->paint(xinfo);
                begin++;
        }
        XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 0, 0, width, height, 0, 0);

        XFlush( xinfo.display );
}

void handleKeyPress(XInfo &xinfo, XEvent &event) {
        KeySym key;
        char text[BufferSize];
        /*
         * Exit when 'q' is typed.
         * This is a simplified approach that does NOT use localization.
         */
        int i = XLookupString(
                (XKeyEvent *)&event,    // the keyboard event
                text,                                   // buffer when text will be written
                BufferSize,                     // size of the text buffer
                &key,                                   // workstation-independent key symbol
                NULL );                                 // pointer to a composeStatus structure (unused)

        if (state == PREGAME){
            switch(key){
                case XK_Return:
                    state = INGAME;
                    paused = false;
                    break;
            }
            return;
        } 
        if (state == INGAME && !paused){
            switch(key){
                case XK_Up:
                    if (orientation == RIGHT){
                        snake.turnLeft();
                    } else if (orientation == LEFT){
                        snake.turnRight();
                    }
                    break;
                case XK_Right:
                    if (orientation == DOWN){
                        snake.turnLeft();
                    } else if (orientation == UP){
                        snake.turnRight();
                    }
                    break;
                case XK_Down:
                    if (orientation == LEFT){
                        snake.turnLeft();
                    } else if (orientation == RIGHT){
                        snake.turnRight();
                    }
                    break;
                case XK_Left:
                    if (orientation == UP){
                        snake.turnLeft();
                    } else if (orientation == DOWN){
                        snake.turnRight();
                    }
                    break;
            }
        }

        if ( i == 1) {
            if (state == INGAME){
                if (!paused && ((text[0] == 'w' && orientation == RIGHT) ||
                                (text[0] == 'a' && orientation == UP) ||
                                (text[0] == 's' && orientation == LEFT) ||
                                (text[0] == 'd' && orientation == DOWN))){
                    snake.turnLeft();
                } else if (!paused && ((text[0] == 'w' && orientation == LEFT) ||
                                       (text[0] == 'a' && orientation == DOWN) ||
                                       (text[0] == 's' && orientation == RIGHT) ||
                                       (text[0] == 'd' && orientation == UP))){
                    snake.turnRight();
                }
                if (text[0] == 'p') {
                    paused = !paused;
                }
            }
            if (text[0] == 'r') {
                restart = true;
            }
            if (text[0] == 'q') {
                error("Terminating normally.");
            }
        } 
}

void handleAnimation(XInfo &xinfo, int inside) {
    if (!paused){
        snake.move(xinfo);
    }
}

void checkReset(XInfo &xinfo, int inside) {
    if (restart){
        snake.reset();
        fruit.new_fruit(snake.getXs(), snake.getYs(), snake.getSize());
        score = 0;
        orientation = RIGHT;
        speed = orig_speed;
        state = INGAME;
        restart = false;
    }
}

// get microseconds
unsigned long now() {
        timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000000 + tv.tv_usec;
}

void draw_intro(XInfo &xinfo){
    const char * ascii_1 = "                   /^\\/^\\";
    const char * ascii_2 = "                 _|__|  O|";
    const char * ascii_3 = "        \\/     /~     \\_/ \\";
    const char * ascii_4 = "         \\____|__________/  \\";
    const char * ascii_5 = "                \\_______      \\";
    const char * ascii_6 = "                        `\\     \\                    \\";
    const char * ascii_7 = "                         |     |                     \\";
    const char * ascii_8 = "                        /     /                       \\";
    const char * ascii_9 = "                       /     /                         \\\\";
    const char * ascii_10 = "                      /     /                           \\ \\";
    const char * ascii_11 = "                     /     /                             \\  \\";
    const char * ascii_12 = "                    /     /             _----_            \\   \\";
    const char * ascii_13 = "                   /     /           _-~      ~-_         |   |";
    const char * ascii_14 = "                  (      (        _-~    _--_    ~-_     _/   |";
    const char * ascii_15 = "                   \\      ~-____-~    _-~    ~-_    ~-_-~    /";
    const char * ascii_16 = "                     ~-_           _-~          ~-_       _-~   ";
    const char * ascii_17 = "                        ~--______-~                ~-___-~";

    const char * snake = "TUNNEL SNAKE";
    const char * name = "Manfred Wu";
    const char * user_id = "m84wu";
    const char * stud_id = "20614707";
    const char * instructions_one = "Use WASD or the arrow keys to move, P to pause, and R to restart the game";
    const char * instructions_two = "Collect fruits:   to increase your score";
    const char * instructions_three = "Don't hit the walls or your body";
    const char * instructions_four = "Take tunnels:      to enter different regions";
    const char * instructions_five = "You will speed up for every 5 fruit you collect";
    const char * instructions_six = "Good luck and have fun!";
    const char * instructions_seven = "Press Enter to begin";

    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 100, ascii_1, strlen(ascii_1));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 110, ascii_2, strlen(ascii_2));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 120, ascii_3, strlen(ascii_3));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 130, ascii_4, strlen(ascii_4));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 140, ascii_5, strlen(ascii_5));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 150, ascii_6, strlen(ascii_6));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 160, ascii_7, strlen(ascii_7));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 170, ascii_8, strlen(ascii_8));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 180, ascii_9, strlen(ascii_9));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 190, ascii_10, strlen(ascii_10));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 200, ascii_11, strlen(ascii_11));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 210, ascii_12, strlen(ascii_12));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 220, ascii_13, strlen(ascii_13));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 230, ascii_14, strlen(ascii_14));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 240, ascii_15, strlen(ascii_15));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 250, ascii_16, strlen(ascii_16));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 190, 260, ascii_17, strlen(ascii_17));

    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 370, 330, snake, strlen(snake));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 374, 345, name, strlen(name));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 387, 360, user_id, strlen(user_id));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 378, 375, stud_id, strlen(stud_id));

    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 230, 400, instructions_one, strlen(instructions_one));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 290, 415, instructions_two, strlen(instructions_two));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 310, 430, instructions_three, strlen(instructions_three));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 270, 445, instructions_four, strlen(instructions_four));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 265, 460, instructions_five, strlen(instructions_five));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 330, 475, instructions_six, strlen(instructions_six));
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 338, 490, instructions_seven, strlen(instructions_seven));

    XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 383, 406, 10, 10);
    XDrawRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 350, 436, 10, 10);
    XDrawRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 368, 436, 10, 10);
    XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], 360, 441, 368, 441);
}

void draw_gameover(XInfo &xinfo){
    const char * gameover_msg = "Game over, press \"r\" to restart, or \"q\" to quit";
    XDrawString(xinfo.display, xinfo.window, xinfo.gc[0], 250, 570, gameover_msg, strlen(gameover_msg));
}

void eventLoop(XInfo &xinfo) {
        // Add stuff to paint to the display list
        dList.push_front(&snake);
        dList.push_front(&fruit);
        dList.push_front(&text_display);

        XEvent event;
        unsigned long lastRepaint_snake = 0;
        unsigned long lastRepaint_fps = 0;
        int inside = 0;

        while( true ) {

                if (XPending(xinfo.display) > 0) {
                        XNextEvent( xinfo.display, &event );
                        switch( event.type ) {
                                case KeyPress:
                                        handleKeyPress(xinfo, event);
                                        break;
                                case EnterNotify:
                                        inside = 1;
                                        break;
                                case LeaveNotify:
                                        inside = 0;
                                        break;
                        }
                }

                if (state == INGAME){
                    unsigned long end = now();

                    checkReset(xinfo, inside);

                    if (end - lastRepaint_snake > 700000/speed) {
                        handleAnimation(xinfo, inside);
                        lastRepaint_snake = now();
                    }

                    if (end - lastRepaint_fps > 1000000/FPS) { // repaint at FPS
                        repaint(xinfo); // my repaint
                        lastRepaint_fps = now(); // remember when the paint happened
                    }
                
                    if (XPending(xinfo.display) == 0){
                        usleep(1000000 / FPS - (end - lastRepaint_fps));
                    }
                } else if (state == PREGAME){
                    draw_intro(xinfo);
                } else{
                    checkReset(xinfo, inside);
                    draw_gameover(xinfo);
                }
        }
}

/*
 * Start executing here.
 *       First initialize window.
 *       Next loop responding to events.
 *       Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char *argv[] ) {
        XInfo xInfo;

        initX(argc, argv, xInfo);
        eventLoop(xInfo);

        XCloseDisplay(xInfo.display);
}

