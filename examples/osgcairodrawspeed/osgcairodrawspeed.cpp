#include <osg/Math>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgCairo/Image>

class DrawImage: public osg::Drawable::UpdateCallback {
	double _nextRadian;
	double _nextArc;
	double _lw;

public:
	DrawImage(double lw):
	_nextRadian (0.0f),
	_nextArc    (lw * 2.0f),
	_lw         (lw) {
	}

	virtual void update(osg::NodeVisitor*, osg::Drawable* drawable) {
		osg::StateSet* ss = drawable->getStateSet();
		
		if(!ss) return;

		osg::StateAttribute* sa = ss->getTextureAttribute(0, osg::StateAttribute::TEXTURE);

		if(!sa) return;

		osg::Texture* tex = sa->asTexture();

		if(!tex) return;

		osgCairo::Image* image = dynamic_cast<osgCairo::Image*>(tex->getImage(0));

		if(!image) return;
	
		cairo_t* c = image->createContext();

		if(cairo_status(c)) return;

		cairo_scale(c, image->s(), image->t());
		cairo_set_line_width(c, _lw);
		cairo_set_source_rgba(c, 1.0f, 1.0f, 1.0f, 1.0f);

		if(_nextArc >= 0.5f) {
			_nextArc    = _lw * 2.0f;
			_nextRadian = 0.0f;

			cairo_set_operator(c, CAIRO_OPERATOR_CLEAR);
			cairo_paint(c);
			cairo_set_operator(c, CAIRO_OPERATOR_OVER);
		}

		else {
			double n = (osg::PI * 2.0f) * 0.25f;

			cairo_arc(
				c,
				0.5f,
				0.5f,
				_nextArc,
				_nextRadian,
				_nextRadian + n
			);

			cairo_stroke(c);

			if(_nextRadian >= (osg::PI * 2.0f)) {
				_nextRadian =  0.0f;
				_nextArc    += _lw * 2.0f;
			}

			else _nextRadian += n;
		}

		cairo_destroy(c);

		image->dirty();
	}
};

osg::Geode* createExample(unsigned int size) {
	osg::Geode*      geode = new osg::Geode();
	osgCairo::Image* image = new osgCairo::Image();
	
	if(image->allocateSurface(size, size, CAIRO_FORMAT_ARGB32)) {
		osg::Texture2D* texture = new osg::Texture2D();
		osg::Geometry*  geom    = osg::createTexturedQuadGeometry(
			osg::Vec3(0.0f, 0.0f, 0.0f),
			osg::Vec3(image->s(), 0.0f, 0.0f),
			osg::Vec3(0.0f, 0.0f, image->t()),
			0.0f,
			0.0f, 
			1.0f,
			1.0f
		);

		double lw = 1.0f / static_cast<double>(size / 2);
	
		texture->setImage(image);
		texture->setDataVariance(osg::Object::DYNAMIC);
        
		osg::StateSet* state = geom->getOrCreateStateSet();
        
		state->setTextureAttributeAndModes(
			0,
			texture,
			osg::StateAttribute::ON
		);
        
		state->setMode(GL_BLEND, osg::StateAttribute::ON);
		state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

		geom->setUpdateCallback(new DrawImage(lw));

		geode->addDrawable(geom);
	}

	return geode;
}

int main(int, char**) {
	osgViewer::Viewer viewer;

	unsigned int size = 512;

	viewer.setSceneData(createExample(size));
	viewer.setUpViewInWindow(50, 50, size + 40, size + 40);
	viewer.addEventHandler(new osgViewer::StatsHandler());

	return viewer.run();
}
