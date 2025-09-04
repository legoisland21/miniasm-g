#include <cmath>
#include <iostream>
#include <vector>
#include "include/miniasm.h"

using namespace std;

AsciiTable game(64, 32);
vector<Register> registers;

vector<vector<int>> map = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,1,1,0,1,0,1},
    {1,0,1,0,0,0,0,1,0,1},
    {1,0,1,1,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,1,0,1},
    {1,1,1,0,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

int mapWidth = map[0].size();
int mapHeight = map.size();

vector<vector<int>> wallColor = {
    {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
    {BLUE, 0, 0, 0, 0, 0, 0, 0, 0, BLUE},
    {BLUE, 0, RED, 0, GREEN, GREEN, 0, RED, 0, BLUE},
    {BLUE, 0, MAGENTA, 0, 0, 0, 0, CYAN, 0, BLUE},
    {BLUE, 0, YELLOW, YELLOW, YELLOW, YELLOW, 0, MAGENTA, 0, BLUE},
    {BLUE, 0, 0, 0, 0, 0, 0, CYAN, 0, BLUE},
    {BLUE, BLUE, BLUE, 0, GREEN, GREEN, 0, RED, 0, BLUE},
    {BLUE, 0, 0, 0, 0, 0, 0, 0, 0, BLUE},
    {BLUE, 0, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, 0, BLUE},
    {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE}
};

struct Player {
    double x = 2.5, y = 2.5;
    double dirX = -1, dirY = 0;
    double planeX = 0, planeY = 0.66;
} player;

double moveSpeed = 1;
double rotSpeed = 0.5;

char getShade(double distance) {
    if(distance <= 1.0) return '#';
    else if(distance <= 2.0) return 'X';
    else if(distance <= 3.0) return '+';
    else if(distance <= 4.0) return '.';
    else return ' ';
}

int main() {
    int FPS = game.calculateDelta(60);
    enableANSI();
    clearWindow();

    while(true) {
        game.clear(' ', BLACK);
        for(int x = 0; x < game.getWidth(); x++) {
            double cameraX = 2.0 * x / double(game.getWidth()) - 1;
            double rayDirX = player.dirX + player.planeX * cameraX;
            double rayDirY = player.dirY + player.planeY * cameraX;

            int mapX = int(player.x);
            int mapY = int(player.y);

            double sideDistX;
            double sideDistY;
            double deltaDistX = (rayDirX == 0) ? 1e30 : abs(1 / rayDirX);
            double deltaDistY = (rayDirY == 0) ? 1e30 : abs(1 / rayDirY);
            double perpWallDist;

            int stepX, stepY;
            bool hit = false;
            int side;

            if(rayDirX < 0) { stepX = -1; sideDistX = (player.x - mapX) * deltaDistX; }
            else { stepX = 1; sideDistX = (mapX + 1.0 - player.x) * deltaDistX; }
            if(rayDirY < 0) { stepY = -1; sideDistY = (player.y - mapY) * deltaDistY; }
            else { stepY = 1; sideDistY = (mapY + 1.0 - player.y) * deltaDistY; }

            while(!hit) {
                if(sideDistX < sideDistY) { sideDistX += deltaDistX; mapX += stepX; side = 0; }
                else { sideDistY += deltaDistY; mapY += stepY; side = 1; }
                if(map[mapY][mapX] > 0) hit = true;
            }

            if(side == 0) perpWallDist = (mapX - player.x + (1 - stepX) / 2) / rayDirX;
            else perpWallDist = (mapY - player.y + (1 - stepY) / 2) / rayDirY;

            int lineHeight = int(game.getHeight() / perpWallDist);
            int drawStart = max(0, -lineHeight / 2 + game.getHeight() / 2);
            int drawEnd = min(game.getHeight() - 1, lineHeight / 2 + game.getHeight() / 2);

            int color = wallColor[mapY][mapX];
            if(color == 0) color = WHITE;

            for(int y = drawStart; y <= drawEnd; y++) {
                char shade = getShade(perpWallDist);
                game.setPixel(x, y, shade, color, BLACK);
            }
        }

        game.render();
        wait(FPS);
        
        int input = getInputNs();
        if(input != -1) {
            if(input == 'w') {
                if(map[int(player.y)][int(player.x + player.dirX * moveSpeed)] == 0) player.x += player.dirX * moveSpeed;
                if(map[int(player.y + player.dirY * moveSpeed)][int(player.x)] == 0) player.y += player.dirY * moveSpeed;
            }
            if(input == 's') {
                if(map[int(player.y)][int(player.x - player.dirX * moveSpeed)] == 0) player.x -= player.dirX * moveSpeed;
                if(map[int(player.y - player.dirY * moveSpeed)][int(player.x)] == 0) player.y -= player.dirY * moveSpeed;
            }
            if(input == 'a') {
                double oldDirX = player.dirX;
                player.dirX = player.dirX * cos(rotSpeed) - player.dirY * sin(rotSpeed);
                player.dirY = oldDirX * sin(rotSpeed) + player.dirY * cos(rotSpeed);
                double oldPlaneX = player.planeX;
                player.planeX = player.planeX * cos(rotSpeed) - player.planeY * sin(rotSpeed);
                player.planeY = oldPlaneX * sin(rotSpeed) + player.planeY * cos(rotSpeed);
            }
            if(input == 'd') {
                double oldDirX = player.dirX;
                player.dirX = player.dirX * cos(-rotSpeed) - player.dirY * sin(-rotSpeed);
                player.dirY = oldDirX * sin(-rotSpeed) + player.dirY * cos(-rotSpeed);
                double oldPlaneX = player.planeX;
                player.planeX = player.planeX * cos(-rotSpeed) - player.planeY * sin(-rotSpeed);
                player.planeY = oldPlaneX * sin(-rotSpeed) + player.planeY * cos(-rotSpeed);
            }
        }
    }
}
