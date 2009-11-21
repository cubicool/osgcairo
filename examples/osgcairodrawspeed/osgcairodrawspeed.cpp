#include <osg/Math>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osgViewer/Viewer>
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
	
		if(_nextArc >= 0.5f) {
			_nextArc    = _lw * 2.0f;
			_nextRadian = 0.0f;

			image->setOperator(CAIRO_OPERATOR_CLEAR);
			image->paint();
			image->setOperator(CAIRO_OPERATOR_OVER);
		}

		else {
			double n = (osg::PI * 2.0f) * 0.25f;

			image->arc(
				0.5f,
				0.5f,
				_nextArc,
				_nextRadian,
				_nextRadian + n
			);

			image->stroke();

			if(_nextRadian >= (osg::PI * 2.0f)) {
				_nextRadian =  0.0f;
				_nextArc    += _lw * 2.0f;
			}

			else _nextRadian += n;
		}

		image->dirty();
	}
};

osg::Geode* createExample(unsigned int size) {
	osg::Geode*      geode = new osg::Geode();
	osgCairo::Image* image = new osgCairo::Image(CAIRO_FORMAT_ARGB32);
	
	if(image->allocateSurface(size, size) && image->createContext()) {
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
		
		image->scale(size, size);
		image->setLineWidth(lw);
		image->setSourceRGBA(1.0f, 1.0f, 1.0f, 1.0f);

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

	unsigned int size = 256;

	viewer.setSceneData(createExample(size));
	viewer.setUpViewInWindow(50, 50, size + 40, size + 40);
	
	return viewer.run();
}
