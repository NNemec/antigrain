#define _USE_MATH_DEFINES
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "../agg2d/agg2d.h"

// Writing the buffer to a .PPM file, assuming it has 
// RGB-structure, one byte per color component
//--------------------------------------------------
bool write_ppm(const unsigned char* buf, 
               unsigned width, 
               unsigned height, 
               const char* file_name)
{
  FILE* fd = fopen(file_name, "wb");
  if(fd)
  {
    fprintf(fd, "P6 %d %d 255 ", width, height);
    fwrite(buf, 1, width * height * 3, fd);
    fclose(fd);
    return true;
  }
  return false;
}


int main()
{
  // Image dimensions.
  const int w = 640;
  const int h = 480;
  const int c = 3;
  const int stride = w*c;

  // Raw data.
  std::vector<unsigned char> data(w*h*c, 255);

  Agg2D painter;
  painter.attach(&data[0], w, h, stride);
  //painter.clearAll(0, 0, 0, 255); // This is buggy.
  painter.rotate(M_PI/6.);
  painter.addEllipse(w/2, h/2, 100, 300, Agg2D::CCW);
  painter.fillColor(255, 0, 0);

  // Something is clearly missing to render the image...

  write_ppm(&data[0], w, h, "agg_test.ppm");

  return 0;
}