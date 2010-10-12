import cairo
import math
import osgcairoutil

surface = cairo.ImageSurface(cairo.FORMAT_A8, 400, 400)
cr      = cairo.Context(surface)

cr.set_line_width(40.0)
cr.arc(200.0, 200.0, 170.0, 0.0, 2 * math.pi)
cr.stroke()

emboss = osgcairoutil.create_embossed_surface(
	surface,
	30 * (math.pi / 180.0),
	30 * (math.pi / 180.0),
	3
)

emboss2 = cairo.ImageSurface.create_for_data(
	emboss.get_data(),
	cairo.FORMAT_A8,
	emboss.get_width(),
	emboss.get_height(),
	emboss.get_stride()
)

c.set_source_surface(emboss2, 0, 0)
c.paint()

