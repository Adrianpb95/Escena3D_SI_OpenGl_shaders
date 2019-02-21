/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Copyright © 2014+ Ángel Rodríguez Ballesteros                              *
 *                                                                             *
 *  Distributed under the Boost Software License, version  1.0                 *
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt                 *
 *                                                                             *
 *  Started by Ángel on april of 2014                                          *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef COLOR_BUFFER_HEADER
#define COLOR_BUFFER_HEADER

    namespace example
    {

        class Color_Buffer
        {
        protected:

            size_t width;
            size_t height;

        public:

            Color_Buffer(size_t width, size_t height)
            :
                width (width ),
                height(height)
            {
            }

        public:

            size_t get_width () const
            {
                return (width);
            }

            size_t get_height () const
            {
                return (height);
            }

            int offset_at (int x, int y) const
            {
                return (y * width + x);
            }

        public:

            virtual int  bits_per_color () const = 0;
            virtual void set_color      (int r, int  g, int b) = 0;
            virtual void set_pixel      (int x, int  y) = 0;
            virtual void set_pixel      (size_t offset) = 0;
            virtual void gl_draw_pixels (int raster_x, int raster_y) const = 0;

        };

    }

#endif
