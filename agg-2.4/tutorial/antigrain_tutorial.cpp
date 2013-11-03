// ========================================================================== //
// This is a **very small** sample code to know how to use AGG in a very few 
// lines of code, which was really missing in the documentation.
//
// Copyright (C) 2013 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public 
// License v. 2.0. If a copy of the MPL was not distributed with this file, 
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

#define _USE_MATH_DEFINES
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
// Pixel format
#include "agg_pixfmt_rgb.h"
// Rendering-related headers.
#include "agg_renderer_scanline.h"
#include "agg_renderer_base.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
// Geometric transforms.
#include "agg_conv_transform.h"
#include "agg_trans_affine.h"
// Ellipse
#include "agg_ellipse.h"
#include "agg_path_storage.h"
#include "agg_conv_stroke.h"

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

struct ImagePainter
{
  ImagePainter(unsigned char *rgb_raw_data, int width, int height)
    : framebuffer(&rgb_raw_data[0], width, height, width*3)
    , pixel_format(framebuffer)
    , renderer_base(pixel_format)
  {
  }

  void drawLine(double x1, double y1, double x2, double y2, const double rgb[3],
                double penWidth)
  {
    // Vectorial geometry.
    agg::path_storage ps;
    ps.remove_all();
    ps.move_to(x1, y1);
    ps.line_to(x2, y2);
    // Convert vectorial geometry to pixels.
    agg::conv_stroke<agg::path_storage> pg(ps);
    pg.width(penWidth);
    aa_rasterizer.add_path(pg);
    // Draw.
    agg::render_scanlines_aa_solid(
      aa_rasterizer, scanline, renderer_base,
      agg::rgba(rgb[0], rgb[1], rgb[2]) );
  }
  void drawRectangle(double x1, double y1, double x2, double y2,
                     const double rgb[3], double penWidth)
  {
    // Vectorial geometry.
    agg::path_storage ps;
    ps.remove_all();
    ps.move_to(x1, y1);
    ps.line_to(x2, y1);
    ps.line_to(x2, y2);
    ps.line_to(x1, y2);
    ps.line_to(x1, y1);
    ps.close_polygon();
    // Convert vectorial geometry to pixels.
    agg::conv_stroke<agg::path_storage> pg(ps);
    pg.width(penWidth);
    aa_rasterizer.add_path(pg);
    // Draw.
    agg::render_scanlines_aa_solid(
      aa_rasterizer, scanline, renderer_base,
      agg::rgba(rgb[0], rgb[1], rgb[2]) );
  }
  void drawEllipse(double cx, double cy, double rx, double ry, double oriRadian,
                   const double rgb[3], double penWidth)
  {
    // Create vectorial ellipse.
    agg::ellipse ellipse;
    // Rotate and translate ellipse
    agg::trans_affine transform;
    ellipse.init(0, 0, rx, ry);
    transform.rotate(oriRadian);
    transform.translate(cx, cy);
    // Render ellipse.
    agg::conv_transform<agg::ellipse> ellipticPath(ellipse, transform);
    agg::conv_stroke<agg::conv_transform<agg::ellipse> > ellStroke(ellipticPath);
    ellStroke.width(penWidth);
    aa_rasterizer.add_path(ellStroke);
    agg::render_scanlines_aa_solid(
      aa_rasterizer, scanline, renderer_base,
      agg::rgba(rgb[0], rgb[1], rgb[2]) );
  }
  void drawCircle(double cx, double cy, double r, const double rgb[3],
                  double penWidth)
  { drawEllipse(cx, cy, r, r, 0., rgb, penWidth); }
  void fillRectangle(double x1, double y1, double x2, double y2,
                     const double rgb[3])
  {
    // Vectorial geometry.
    agg::path_storage ps;
    ps.remove_all();
    ps.move_to(x1, y1);
    ps.line_to(x2, y1);
    ps.line_to(x2, y2);
    ps.line_to(x1, y2);
    ps.line_to(x1, y1);
    ps.close_polygon();
    // Convert vectorial geometry to pixels.
    agg::trans_affine identity_transform;
    agg::conv_transform<agg::path_storage> pg(ps, identity_transform);
    aa_rasterizer.add_path(pg);
    // Draw.
    agg::render_scanlines_aa_solid(
      aa_rasterizer, scanline, renderer_base,
      agg::rgba(rgb[0], rgb[1], rgb[2]) );
  }
  void fillEllipse(double cx, double cy, double rx, double ry, double oriRadian,
                   const double rgb[3])
  {
    // Create vectorial ellipse.
    agg::ellipse ellipse(cx, cy, rx, ry);
    // Render ellipse.
    agg::trans_affine transform;
    ellipse.init(0, 0, rx, ry);
    transform.rotate(oriRadian);
    transform.translate(cx, cy);
    agg::conv_transform<agg::ellipse> ellipticPath(ellipse, transform);
    aa_rasterizer.add_path(ellipticPath);
    agg::render_scanlines_aa_solid(
      aa_rasterizer, scanline, renderer_base,
      agg::rgba(rgb[0], rgb[1], rgb[2]) );
  }
  void fillCircle(double cx, double cy, double r, const double rgb[3])
  { fillEllipse(cx, cy, r, r, 0., rgb); }

  // Some convenient typedefs.
  typedef agg::renderer_base<agg::pixfmt_rgb24> renderer_base_type;
  typedef agg::renderer_scanline_aa_solid<renderer_base_type> renderer_scanline_type;
  typedef agg::rasterizer_scanline_aa<> rasterizer_scanline_type;
  // Rendering buffer data structure to which raw image data is attached.
  agg::rendering_buffer framebuffer;
  // Pixel format of the frame buffer.
  agg::pixfmt_rgb24 pixel_format;
  // Renderer supporting low-level clipping.
  agg::renderer_base<agg::pixfmt_rgb24> renderer_base;
  // Anti-aliased rasterizer.
  agg::rasterizer_scanline_aa<> aa_rasterizer;
  // Use a packed scanline storage.
  agg::scanline_p8 scanline;
};

int main()
{
  // Image dimensions.
  const int w = 320;
  const int h = 200;
  const int c = 3;
  const int stride = w*c;

  // Raw data.
  std::vector<unsigned char> data(w*h*c, 255);

  ImagePainter painter(&data[0], w, h);
  
  double black[] = { 0, 0, 0 };
  double red[] = { 1, 0, 0 };
  double green[] = { 0, 1, 0 };
  double blue[] = { 0, 0, 1 };
  
  // Draw two filled ellipses.
  painter.fillEllipse(w/2, h/2, 50, 30, M_PI/6., red);
  painter.drawEllipse(w/2, h/2, 50, 30, M_PI/6., black, 5.);
  painter.fillEllipse(0, 0, 20, 50, M_PI/3., green);

  // Draw two ellipses.
  painter.drawCircle(30, 80, 25, blue, 3.);
  // Draw rectangle.
  painter.fillRectangle(10, 10, 300, 40, blue);
  painter.drawRectangle(10, 10, 300, 40, black, 2.);

  // Draw black line.
  painter.drawLine(0, 0, 100, 50, black, 2.);
  painter.drawLine(20, 50, 100, 60, black, 1.);


  write_ppm(&data[0], w, h, "agg_test.ppm");

  return 0;
}