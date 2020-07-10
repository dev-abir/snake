#ifndef SETTINGS_H
#define SETTINGS_H

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT SCREEN_WIDTH
#define TITLE "snake game"

// #define FRAME_CAP false // will automatically adjust FPS according to your monitor's refresh rate
#define SPEED 30 // equivalent to defining custom FPS [if you define this, then FRAME_CAP will become UNDEFINED]

#ifdef SPEED
#undef FRAME_CAP
#endif // SPEED

#define SNAKE_CELL_WIDTH 10
#define SNAKE_CELL_HEIGHT SNAKE_CELL_WIDTH
#define SNAKE_CELL_BORDER_WIDTH 1

#define FOOD_WIDTH SNAKE_CELL_WIDTH
#define FOOD_HEIGHT SNAKE_CELL_WIDTH

#endif // SETTINGS_H
