import cairo
import math
import osgcairoutil

c.arc(w / 2, 0, w / 4, 0, 2 * math.pi)

p = c.copy_path_flat()

c.new_path()
c.set_line_width(1)
c.select_font_face("Trebuchet MS", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_NORMAL)
c.set_font_size(20)

c.text_path("osgcairoutil osgCairo::util OSGCAIROUTIL")

osgcairoutil.map_path_onto(c, p)

c.fill()

