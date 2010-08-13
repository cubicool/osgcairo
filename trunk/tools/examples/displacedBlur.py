import osgcairoutil

c.push_group();

c.set_line_width(1.0);
c.move_to(w / 2.0, 0.0);
c.line_to(w / 2.0, h);
c.stroke();

blur = osgcairoutil.displaced_blur(c, c.pop_group(), 10);

c.set_source(blur);
c.paint();

c.set_line_width(1.0);
c.set_source_rgb(1.0, 0.0, 0.0);
c.move_to(w / 2.0, 0.0);
c.line_to(w / 2.0, h);
c.stroke();

