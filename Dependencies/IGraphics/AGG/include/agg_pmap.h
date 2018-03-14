//----------------------------------------------------------------------------
//
// THIS FILE ADDED FOR IPLUG2
//
// class pixel_map
//
//----------------------------------------------------------------------------

#ifndef AGG_PMAP_INCLUDED
#define AGG_PMAP_INCLUDED

#include <stdio.h>

namespace agg
{
  class pixel_map
  {
  public:

    enum format_e
    {
      format_png = 0,
      format_jpg
    };

    pixel_map() {}
    virtual ~pixel_map() {}

    virtual void destroy() = 0;
    virtual void create(unsigned width, unsigned height, unsigned clear_val=255) = 0;

    virtual void clear(unsigned clear_val=255) = 0;

    virtual unsigned char* buf() = 0;

    virtual unsigned width() const = 0;
    virtual unsigned height() const = 0;

    virtual int row_bytes() const = 0;
    virtual unsigned bpp() const = 0;

  private:

    pixel_map(const pixel_map&);
    const pixel_map& operator = (const pixel_map&);
  };
}

#endif
