#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <iostream>
#include <string>

#define radAsDeg(x) x * 57.29578

constexpr int WINDOW_HEIGHT = 512;
constexpr int WINDOW_WIDTH = 512;
constexpr int WALL_AMOUNT = 18;

constexpr int PIXEL_DISTANCE_HORIZONTAL = 4;
constexpr int FLOOR_LIGHT_DROP_OFF = 2;




class PlayerClass {
public:
    double x = 1.0;
    double y = 1.0;

    int speed = 5;
    double look_speed = M_PI / 100;

    int ray_lenght = sqrt(WINDOW_WIDTH * WINDOW_WIDTH + WINDOW_HEIGHT * WINDOW_HEIGHT);
    double ray_density = 80;

    double view_angle = 0;

    void warpArountScreen() {
        if (y < 0) { y = WINDOW_HEIGHT; }
        if (x < 0) { x = WINDOW_WIDTH; }
        if (y > WINDOW_HEIGHT) { y = 0; }
        if (x > WINDOW_WIDTH) { x = 0; }
    }
};

class MarkerClass {
public:
    double x = 64.0;
    double y = 64.0;

    double size = 5.0;

    int Markerspace[4] = { (int) x, (int)y, (int)size, (int)size };
};

PlayerClass Player;
MarkerClass Marker;

int allWalls[WALL_AMOUNT][18] = {
    //x   y   width height
    { 80,  80, 100,10 },
    { 180, 30, 10, 200 },
    { 10,  80, 50, 10 },
    { 90,  80, 10, 80 },
    { 120, 10, 10, 80 },
    { 10,  180,100,10 },
    { 2*120, 2*80, 100,10 },
    { 2*130,2*30, 10, 200 },
    { 7*10, 2*80, 50, 10 },
    { 2*20, 2*80, 100, 80 },
    { 2*120,2*10, 10, 80 },
    { 2*50, 2*180,20,100 },
    { 6*80, 3*80, 10,100 },
    { 3*140,3*30, 10, 200 },
    { 3*10, 4*80, 50, 10 },
    { 3*90, 3*80, 10, 80 },
    { 2*120,3*10, 10, 120 },
    { 3*10, 5*180,100,10 },
};

bool pointInBounds(int pointX, int pointY, int wall[]) {
    int boundX =    wall[0];
    int boundY =    wall[1];
    int boundXLen = wall[2];
    int boundYLen = wall[3];

    if (pointX >= boundX && pointX <= boundX + boundXLen && pointY >= boundY && pointY <= boundY + boundYLen) {
        return true;
    }
    return false;
}

void clearScreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);
}

void drawDigit(SDL_Renderer* renderer, int x, int y, int digit, int font_size) {
    SDL_SetRenderDrawColor(renderer, rand() % 255, 255, rand() % 255, 255);
    switch (digit) {
    case 1:
        for (int i = 0; i < font_size; i++) {
            for (int j = 0; j < font_size; j++) {
                SDL_RenderDrawPoint(renderer, x + i, y + j + font_size * 1);
                SDL_RenderDrawPoint(renderer, x + i, y + j + font_size * 4);
                SDL_RenderDrawPoint(renderer, x + i + font_size * 2, y + j + font_size * 4);

                for (int k = 0; k < 5;k++) {
                    SDL_RenderDrawPoint(renderer, x + i + font_size * 1, y + j + font_size * k);
                }
            }
        }

        break;
    case 2:
        for (int i = 0; i < font_size; i++) {
            for (int j = 0; j < font_size; j++) {
                SDL_RenderDrawPoint(renderer, x + i + font_size * 2, y + j + font_size);
                SDL_RenderDrawPoint(renderer, x + i, y + j + font_size * 3);

                for (int k = 0; k < 3; k++) {
                    for (int l = 0; l < 5; l += 2)
                        SDL_RenderDrawPoint(renderer, x + i + font_size * k, y + j + font_size * l);
                }
            }
        }
        break;
    case 3:
        for (int i = 0; i < font_size; i++) {
            for (int j = 0; j < font_size; j++) {
                SDL_RenderDrawPoint(renderer, x + font_size * 2 + i, y + j + font_size * 1);
                SDL_RenderDrawPoint(renderer, x + font_size * 2 + i, y + j + font_size * 3);

                for (int k = 0; k < 3; k++) {
                    for (int l = 0; l < 5; l += 2)
                        SDL_RenderDrawPoint(renderer, x + i + font_size * k, y + j + font_size * l);
                }
            }
        }

        break;
    case 4:
        for (int i = 0; i < font_size; i++) {
            for (int j = 0; j < font_size; j++) {
                SDL_RenderDrawPoint(renderer, x + i + font_size * 1, y + j + font_size * 2);

                for (int k = 0; k < 3; k++) {
                    SDL_RenderDrawPoint(renderer, x + i, y + j + font_size * k);
                }

                for (int k = 0; k < 5; k++) {
                    SDL_RenderDrawPoint(renderer, x + i + font_size * 2, y + j + font_size * k);
                }
            }
        }

        break;
    case 5:
        for (int i = 0; i < font_size; i++) {
            for (int j = 0; j < font_size; j++) {
                SDL_RenderDrawPoint(renderer, x + i, y + j + font_size);
                SDL_RenderDrawPoint(renderer, x + i + font_size * 2, y + j + font_size * 3);

                for (int k = 0; k < 3; k++) {
                    for (int l = 0; l < 5; l += 2)
                        SDL_RenderDrawPoint(renderer, x + i + font_size * k, y + j + font_size * l);
                }
            }
        }

        break;
    case 6:
        for (int i = 0; i < font_size; i++) {
            for (int j = 0; j < font_size; j++) {
                SDL_RenderDrawPoint(renderer, x + i, y + j + font_size);
                SDL_RenderDrawPoint(renderer, x + i, y + j + font_size * 3);
                SDL_RenderDrawPoint(renderer, x + i + font_size * 2, y + j + font_size * 3);

                for (int k = 0; k < 3; k++) {
                    for (int l = 0; l < 5; l += 2)
                        SDL_RenderDrawPoint(renderer, x + i + font_size * k, y + j + font_size * l);
                }
            }
        }

        break;
    case 7:
        for (int i = 0; i < font_size; i++) {
            for (int j = 0; j < font_size; j++) {
                SDL_RenderDrawPoint(renderer, x + i + font_size * 2, y + j + font_size * 1);
                SDL_RenderDrawPoint(renderer, x + i + font_size * 1, y + j + font_size * 2);
                SDL_RenderDrawPoint(renderer, x + i, y + j + font_size * 3);
                SDL_RenderDrawPoint(renderer, x + i, y + j + font_size * 4);

                for (int k = 0; k < 3; k++) {
                    SDL_RenderDrawPoint(renderer, x + i + font_size * k, y + j);
                }
            }
        }

        break;
    case 8:
        for (int i = 0; i < font_size; i++) {
            for (int j = 0; j < font_size; j++) {
                SDL_RenderDrawPoint(renderer, x + i, y + j + font_size);
                SDL_RenderDrawPoint(renderer, x + i, y + j + font_size * 3);
                SDL_RenderDrawPoint(renderer, x + i + font_size * 2, y + j + font_size * 3);
                SDL_RenderDrawPoint(renderer, x + i + font_size * 2, y + j + font_size);

                for (int k = 0; k < 3; k++) {
                    for (int l = 0; l < 5; l += 2)
                        SDL_RenderDrawPoint(renderer, x + i + font_size * k, y + j + font_size * l);
                }
            }
        }

        break;
    case 9:
        for (int i = 0; i < font_size; i++) {
            for (int j = 0; j < font_size; j++) {
                SDL_RenderDrawPoint(renderer, x + i, y + j + font_size * 1);

                for (int k = 0; k < 2; k++) {
                    for (int l = 0; l < 4; l += 2)
                        SDL_RenderDrawPoint(renderer, x + i + font_size * k, y + j + font_size * l);
                }

                for (int k = 0; k < 5; k++) {
                    SDL_RenderDrawPoint(renderer, x + i + font_size * 2, y + j + font_size * k);
                }
            }
        }
        break;
    case 0:
        for (int i = 0; i < font_size; i++) {
            for (int j = 0; j < font_size; j++) {
                SDL_RenderDrawPoint(renderer, x + i + font_size * 1, y + j);
                SDL_RenderDrawPoint(renderer, x + i + font_size * 1, y + j + font_size * 4);

                for (int k = 0; k < 4; k += 2) {
                    for (int l = 0; l < 5; l++) {
                        SDL_RenderDrawPoint(renderer, x + i + font_size * k, y + j + font_size * l);
                    }
                }
            }
        }

        break;
    }
}

void drawNumber(SDL_Renderer* renderer, int x, int y, int number, int font_size) {
    std::string numStr = std::to_string(number);
    int i = 0;
    for (char digit : numStr) {
        int numericDigit = digit - '0'; // Convert char to integer
        drawDigit(renderer, x + i * 5 * font_size, y, numericDigit, font_size);
        i++;
    }
}

void drawPlayer(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    
    int size = 3;

    for(int i = -size; i <= size; i++) {
        for (int j = -size; j <= size; j++) {
            SDL_RenderDrawPoint(renderer, Player.x+i, Player.y+j);
        }
    }


    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (int i = -Marker.size; i <= Marker.size; i++) {
        for (int j = -Marker.size; j <= Marker.size; j++) {
            SDL_RenderDrawPoint(renderer, Marker.x+i, Marker.y+j);
        }
    }
}

void sendRays(SDL_Renderer* renderer) {
    int lenght = Player.ray_lenght;
    double ray_range = M_PI / 4;
    double ray_density = Player.ray_density;

    int x = 0;
    for (double j = -ray_range; j <= ray_range; j += 1/ray_density) { // Loop for all rays
        double xoff = cos(Player.view_angle + j);
        double yoff = sin(Player.view_angle + j);

        x += PIXEL_DISTANCE_HORIZONTAL;

        bool hitMarker = false;
        int len = 0;
        // Geting Lenght
        for (int i = 0; i < lenght; i++) {// Loop for one ray
            for (int index = 0; index <= WALL_AMOUNT; index++) { //collision check
                if (pointInBounds(Player.x + (i) * xoff, Player.y + (i) * yoff, allWalls[index]) ||
                    (Player.x + i * xoff < 0) ||
                    (Player.y + i * yoff < 0) || 
                    (Player.x + i * xoff > WINDOW_WIDTH) ||
                    (Player.y + i * yoff > WINDOW_HEIGHT)) {
                    
                    if (i < lenght) {
                        len = i;
                    }
                    i = lenght;
                }
            }
;
            if (pointInBounds(Player.x + (i)*xoff, Player.y + (i)*yoff, Marker.Markerspace)) {
                hitMarker = true;
            }
        }
        // Drawing
        //      Walls:
        for (int y =  (len * xoff); y <= WINDOW_HEIGHT - (len * xoff); y += 2) {
            len = fmin(len, 255);
            (hitMarker) ? SDL_SetRenderDrawColor(renderer, 255 - len, 0, 0, 255) : SDL_SetRenderDrawColor(renderer, 255 - len, 255 - len, 0, 255);

            SDL_RenderDrawPoint(renderer, x, y);
            SDL_RenderDrawPoint(renderer, x + 1, y);

            SDL_RenderDrawPoint(renderer, x + 2, y);
        }
        //      Floor:
        int color_val = 0;
        for (int y = WINDOW_HEIGHT; y > (WINDOW_HEIGHT - (len * xoff)); y -= 2) {
            
            color_val += FLOOR_LIGHT_DROP_OFF; // Floor color degreesing to look darker
            
            int reducedColor = fmax(0, 255 - color_val);

            SDL_SetRenderDrawColor(renderer, 0, reducedColor, reducedColor, 255);

            SDL_RenderDrawPoint(renderer, x, y);
            SDL_RenderDrawPoint(renderer, x + 1, y);
        }
        

    }
    
}

int sendRays2D(SDL_Renderer* renderer) {
    int lenght = Player.ray_lenght;
    double ray_range = M_PI / 4;
    double ray_density = Player.ray_density;

    for (double j = -ray_range; j <= ray_range; j += 1 / ray_density) {
        double xoff = cos(Player.view_angle + j);
        double yoff = sin(Player.view_angle + j);

        for (int i = 0; i < lenght; i++) {
            int len;
            for (int index = 0; index <= WALL_AMOUNT; index++) {
                if (pointInBounds(Player.x + (i)*xoff, Player.y + (i)*yoff, allWalls[index]) ||
                    (Player.x + i * xoff < 0) ||
                    (Player.y + i * yoff < 0) ||
                    (Player.x + i * xoff > WINDOW_WIDTH) ||
                    (Player.y + i * yoff > WINDOW_HEIGHT)) {
                    if (i < 1000) {
                        len = i;
                        //std::cout << j << " - " << len << std::endl;
                    }
                    i = 10000;
                }
            }
            SDL_SetRenderDrawColor(renderer, 255, i, 0, 255);
            SDL_RenderDrawPoint(renderer, Player.x + i * xoff, Player.y + i * yoff);
        }
    }
    return 0;
}

void drawMap(SDL_Renderer* renderer) {
    for (int y = 0; y <= WINDOW_HEIGHT; y++) {
        for (int x = 0; x <= WINDOW_WIDTH; x++) {
            for (int index = 0; index <= WALL_AMOUNT; index++) {
                if (pointInBounds(x, y, allWalls[index])) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }
    }
}

void displayAll(SDL_Renderer *renderer) {
    clearScreen(renderer);

    sendRays(renderer);

    // Drawing Debug information
    drawNumber(renderer, 0, 0, Player.x, 5);
    drawNumber(renderer, 0, 35, Player.y, 5);

    drawNumber(renderer, 0, 75, radAsDeg(Player.view_angle), 5);

    // 2D Part
    //drawPlayer(renderer);
    //drawMap(renderer);
    //sendRays2D(renderer);
}


int main(int argc, char* argv[]) {
    SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Window* window;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    clearScreen(renderer);

    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            
            double xoff = cos(Player.view_angle);
            double yoff = sin(Player.view_angle);
            
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
                // MOVEMENT
                if (event.key.keysym.sym == SDLK_w) {
                    Player.x += xoff * Player.speed;
                    Player.y += yoff * Player.speed;
                }
                if (event.key.keysym.sym == SDLK_s) {
                    Player.x -= xoff * Player.speed;
                    Player.y -= yoff * Player.speed;
                }
                // Viewing
                if (event.key.keysym.sym == SDLK_d) {
                    Player.view_angle += Player.look_speed;
                    if (Player.view_angle > 2*M_PI) { Player.view_angle = 0; }
                }
                if (event.key.keysym.sym == SDLK_a) {
                    Player.view_angle -= Player.look_speed;
                    if (Player.view_angle < 0) { Player.view_angle = 2*M_PI; }
                }

            }
        }
        Player.warpArountScreen();
        displayAll(renderer);
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}