import cairo
import math

c.set_antialias(cairo.ANTIALIAS_SUBPIXEL)

c.rectangle(0, 0, 512, 512)
c.set_source_rgb(0.7, 0.7, 0.7)
c.fill()
c.set_line_width(1)

w2 = w / 2
h2 = h / 2

shadow = createShadow(w2 + 2, h2 + 2, 45.5, 54.5)

c.set_source(shadow)
c.arc(w2 + 2, h2 + 2, 54.5, 0, 2 * math.pi)
c.fill()

c.set_source_rgb(1.0, 1.0, 1.0)
c.arc(w2, h2, 51.5, 0, 2 * math.pi)
c.fill_preserve()
c.clip()

drawRadialGradientCircle(
	c,
	w2, h2 - 20, 10.5, 45.5,
	(0.0, 1.0, 1.0, 1.0),
	(0.0, 1.0, 1.0, 0.1)
)

# c.set_operator(cairo.OPERATOR_IN)

c.fill()

