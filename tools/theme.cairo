import math

s = 32
r = s * 0.75

top    = math.pi + (math.pi / 2.0)
bottom = math.pi / 2.0
left   = math.pi
right  = 0

lineColor = 0.1, 0.1, 0.8, 1
fillColor = 1, 1, 1, 1

def finalizeCorner(c, x, y):
	c.set_source_rgba(*lineColor)
	c.stroke_preserve()
	c.set_source_rgba(*fillColor)
	c.line_to(x, y)
	c.fill()

def drawBorder(c, x1, y1, x2, y2, xoff):
	c.set_source_rgba(*lineColor)
	c.move_to(x1 + xoff, 0)
	c.line_to(x1 + xoff, s)
	c.stroke()
	c.rectangle(x1, y1, x2, y2)
	c.set_source_rgba(*fillColor)
	c.fill()

c.set_line_width(1)

# Create the upper-left region.
c.arc(s, s, r, left, top)
finalizeCorner(c, s, s)

# Create the top border region.
# c.rectangle(s + (s - r), 0, r, s)
drawBorder(c, s + (s - r), 0, r, s, 0)

# Create the upper-right region.
c.arc(s * 2, s, r, top, right)
finalizeCorner(c, s * 2, s)

# Create the left border.
drawBorder(c, (s * 3) + (s - r), 0, r, s, 0)

# Create the right border.
drawBorder(c, (s * 4), 0, r, s, r)
c.fill()

# Create the bottom-left region.
c.arc(s * 6, 0, r, bottom, left)
finalizeCorner(c, s * 6, 0)

# Create the bottom border region.
drawBorder(c, (s * 6), 0, r, s, r)

# Create the bottom-right region.
c.arc(s * 7, 0, r, right, bottom)
finalizeCorner(c, s * 7, 0)
