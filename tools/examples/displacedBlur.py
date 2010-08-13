import cairo
import osgcairoutil

lw = 2.0
nb = 10

c.push_group()

c.set_line_width(lw)
c.move_to(w / 2.0, 0.0)
c.line_to(w / 2.0, h)
c.stroke()

# p = c.pop_group()
# m = cairo.Matrix()
# m.translate((nb * r) / 2.0, (nb * r) / 2.0)
# p.set_matrix(m)

blur = osgcairoutil.displaced_blur(c, c.pop_group(), nb)

c.set_source(blur)
c.paint()

c.set_line_width(1.0)
c.set_source_rgb(1.0, 0.0, 0.0)
c.move_to(w / 2.0, 0.0)
c.line_to(w / 2.0, h)
c.stroke()




