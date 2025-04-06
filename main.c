#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<errno.h>
#include<assert.h>

#define WIDTH 800
#define HEIGHT 600
#define COLOR_RED 0xFF0000FF 
#define COLOR_GREEN 0xFF00FF00
#define COLOR_BLUE  0xFFFF0000
#define COLOR_WHITE 0xFFFFFFFF
#define SEEDS_COUNT 10
#define BACKGROUND_COLOR 0xFF181818
#define SEED_MARKER_RADIUS 5
#define SEED_MARKER_COLOR COLOR_WHITE



#define OUTPUT_FILE_PATH "output.ppm"

typedef uint32_t Color32;
typedef struct{
  int x, y;
} Point;

static Color32 image[HEIGHT][WIDTH];
static Point seeds[SEEDS_COUNT];

void fill_image(Color32 color){
  for(size_t y = 0; y < HEIGHT; ++y){
    for(size_t x = 0; x < WIDTH; ++x){
      image[y][x] = color;
    }
  }
}

void fill_circle(int cx, int cy, int radius, uint32_t color){
  int x0 = cx - radius;
  int y0 = cy - radius;
  int x1 = cx + radius;
  int y1 = cy + radius;
  for(int x = x0; x<=x1; ++x){
    if(0<=x && x<WIDTH){
      for(int y = y0; y<= y1; ++y){
        if(0<=y && y<HEIGHT){
          int dx = cx - x;
          int dy = cy - y;
          if (dx*dx + dy*dy <= radius*radius){
            image[y][x] = color;
          }

        }
        
      }

    }
    
  }
}

void save_image_as_ppm(const char *file_path){
  FILE *f = fopen(file_path, "wb");
  if(f==NULL){
    fprintf(stderr, "ERROR: could write into file %s: %s\n", file_path, strerror(errno));
    exit(1);
  }
  fprintf(f, "P6\n%d %d 255\n", WIDTH, HEIGHT);
  for(size_t y = 0; y < HEIGHT; ++y){
    for(size_t x = 0; x<WIDTH; ++x){
      uint32_t pixel = image[y][x];
      uint8_t bytes[3] = {
        (pixel&0x0000FF)>>8*0,
        (pixel&0x00FF00)>>8*1,
        (pixel&0xFF0000)>>8*2,
      };
      fwrite(bytes, sizeof(bytes), 1, f);
      assert(!ferror(f));
    }
  }

  int ret = fclose(f);
  assert(ret==0);
}

void generate_random_seeds(void){
  for(size_t i = 0; i<SEEDS_COUNT; ++i){
    seeds[i].x = rand()%WIDTH;
    seeds[i].y = rand()%HEIGHT;
  }
}


int main(void){
  fill_image(BACKGROUND_COLOR);
  generate_random_seeds();
  for(size_t i = 0; i<SEEDS_COUNT; ++i){
    fill_circle(seeds[i].x, seeds[i]. y, SEED_MARKER_RADIUS, SEED_MARKER_COLOR);
  }
  save_image_as_ppm(OUTPUT_FILE_PATH);
  return 0;
}
