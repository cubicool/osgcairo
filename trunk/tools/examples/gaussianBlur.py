import math
import cairo
import osgcairoutil

surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, w, h);

cr = cairo.Context(surface)

cr.set_line_width(1.0)

for i in range(5):
	cr.arc(w / 2, h / 2, i * 10, 0, 2 * math.pi)
	cr.stroke()

osgcairoutil.gaussian_blur(surface, 3.0, 2.7)

c.set_source_surface(surface, 0, 0)
c.paint()

