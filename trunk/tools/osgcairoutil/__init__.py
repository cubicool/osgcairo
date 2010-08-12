try:
	from _osgcairoutil import *

except ImportError, e:
	print "You need to compile the osgcairoutil module. Go into the directory"
	print "osgcairoutil and use make or make-win.bat, accordingly (depending"
	print "if you are on Linux or Windows)."
	
	print e

	import sys

	sys.exit(1)
