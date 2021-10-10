#include "./linalg.h"
#include <malloc.h>
#include <stdio.h>
#include <chrono>
#include <thread>

#define BALL_RADIUS 8
#define HEIGHT 80
#define WIDTH 80
#define FPS 30

int main()
{
  char *pixelArr = (char *)malloc(sizeof(char) * HEIGHT * WIDTH * 2);
  auto timeStart = std::chrono::high_resolution_clock::now();
  auto timeEnd = std::chrono::high_resolution_clock::now();
  using dTime = std::chrono::duration<double, std::milli>;
  std::chrono::duration<double, std::milli> targetDelta(1000 / FPS);

  linalg::vec<float, 2> ballPos(0.0f, 10.0f);
  linalg::vec<float, 2> velocity(3.0f, 1.0f);
  linalg::vec<float, 2> acceleration(0.0f, 1.0f);

  //Animation loop
  while (true)
  {
    velocity += acceleration;
    ballPos += velocity;

    if (ballPos.x - BALL_RADIUS < 0)
    { // LEFT BOUNCE
      ballPos.x = BALL_RADIUS;
      velocity.x *= -.95f;
    }
    else if (ballPos.x + BALL_RADIUS > WIDTH)
    { // RIGHT BOUNCE
      ballPos.x = WIDTH - BALL_RADIUS;
      velocity.x *= -.95f;
    }
    if (ballPos.y + BALL_RADIUS > HEIGHT)
    { // BOTTOM BOUNCE
      ballPos.y = HEIGHT - BALL_RADIUS;
      velocity *= linalg::vec<float, 2>(.95f, -.95f);
    }

    timeStart = std::chrono::high_resolution_clock::now();
    for (int y = 0; y < HEIGHT; y++)
    {
      for (int x = 0; x < WIDTH; x++)
      {
        if (linalg::distance(
          linalg::vec<int, 2>(x, y), linalg::vec<int, 2>(ballPos)) < BALL_RADIUS)
        {
          //Since console characters are about twice as tall as they are wide,
          //print two characters for every "pixel" (otherwise the circle will
          //be stretched vertically)
          pixelArr[2 * WIDTH * y + 2 * x] = 'X';
          pixelArr[2 * WIDTH * y + 2 * x + 1] = 'X';
        }
        else
        {
          pixelArr[2 * WIDTH * y + 2 * x] = ' ';
          pixelArr[2 * WIDTH * y + 2 * x + 1] = ' ';
        }
      }
      pixelArr[2 * WIDTH * (1 + y) + -1] = '\n';
    }
    fwrite(pixelArr, sizeof(char), WIDTH * HEIGHT * 2, stdout);

    timeEnd = std::chrono::high_resolution_clock::now();
    //Find the difference between the time it took to draw this time and the desired frame time,
    double frametimeTargetDiff = targetDelta.count() - dTime(timeEnd - timeStart).count();
    //if drawing too fast, wait the difference to maintain constant desired framerate
    if (frametimeTargetDiff > 0)
    {
      std::this_thread::sleep_for(
          std::chrono::duration<double, std::milli>(frametimeTargetDiff));
    }
  }
}