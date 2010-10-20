import cairo
import math
import osgcairoutil

text     = "EMBOSS"
textSize = 100
surface  = cairo.ImageSurface(cairo.FORMAT_A8, w, h)
cr       = cairo.Context(surface)

cr.select_font_face("Sans", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_BOLD)
cr.set_font_size(textSize)

extents = cr.text_extents(text)

cr.move_to(((w - extents[2]) / 2.0) - extents[0], (h - extents[1]) / 2.0)
cr.show_text(text)

emboss = osgcairoutil.create_embossed_surface(
	surface,
	30 * (math.pi / 180.0),
	30 * (math.pi / 180.0)
)

c.set_source_surface(emboss, 0, 0)
c.paint()


