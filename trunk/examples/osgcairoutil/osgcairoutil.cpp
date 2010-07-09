#include <osg/Math>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgCairo/Image>
#include <osgCairo/Util>

void drawPie(osgCairo::Surface* s) {
	double w   = s->getWidth();
	double h   = s->getHeight();
	double w2  = w / 2.0f;
	double h2  = h / 2.0f;
	double r   = h2 * 0.8f;
	double seg = (2.0f * osg::PI) / 8.0f;
	double top = osg::PI + (osg::PI / 2.0f);
	
	// each "tier" or level or an arc
	double lvl[] = { 0.0f, 0.25f, 0.50f, 0.75f, 1.0f };

	std::string atts[] = {
		"Milk",
		"Horns",
		"Beefiness",
		"Moo/HR",
		"Grazing",
		"Sleeping",
		"Cowpower!",
		"s p o t s"
	};
	
	double attl[] = {
		0.25,
		0,
		0.50,
		1.0,
		0,
		0.25,
		0.75,
		1.0
	};

	s->setLineWidth(((w + h) / 2.0) * 0.003);
	s->selectFontFace("SegoeUI", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	s->setFontSize(((w + h) / 2.0) * 0.05);
        
	/*
	s->setSourceRGBA(0.7, 0.7, 0.7, 1.0);
	s->rectangle(0, 0, w, h);
	s->fill();
	*/

	s->translate(w2, h2);
	s->rotate(-seg / 2.0);
	s->setSourceRGBA(1.0, 1.0, 1.0, 1.0);

	for(unsigned int i = 0; i < 8; i++) {
		// Pick a random level...
		s->moveTo(0.0, 0.0);
		s->lineTo(0.0, -r);
        	s->arc(0.0, 0.0, attl[i] * r, top, top + seg);
		s->lineTo(0.0, 0.0);
		s->setSourceRGBA(0.2, 0.8, 0.2, 0.5);
		s->fill();
		s->setSourceRGBA(1.0, 1.0, 1.0, 1.0);
	
        	// Do the various levels...
		s->moveTo(0.0, 0.0);
		s->lineTo(0.0, -r);
		s->stroke();
		
		for(unsigned int l = 0; l < 5; l++) {
			s->arc(0.0, 0.0, lvl[l] * r, top, top + seg);
			s->stroke();
		}

		osgCairo::TextExtents extents = s->textExtents(atts[i]);
		
		double arcsize = r * seg;
	
		// ------------------------------------
		s->save();
	
		double tr = extents.width / r;
		double aa = ((arcsize - extents.width) / 2.0) / r;

		s->arc(0.0, 0.0, h2 * 0.85, top + aa, top + aa + tr);
		s->setTolerance(0.01);

		osgCairo::Path path = s->copyPathFlat();
	
		s->newPath();
		s->textPath(atts[i]);

		osgCairo::util::mapPathOnto(s, path);
	
		s->setSourceRGBA(1.0, 1.0, 1.0, 1.0);
		s->setLineWidth(1.0);
        	s->strokePreserve();
		s->setSourceRGBA(0.8, 0.5, 0.1, 0.7);
		s->fill();
	
		s->restore();
		// ------------------------------------
	
        	// Pick a random level...
		s->moveTo(0.0, 0.0);
		s->lineTo(0.0, -r);
		s->arc(0.0, 0.0, r, top, top + seg);
		s->lineTo(0.0, 0.0);
		s->setSourceRGBA(1.0, 1.0, 1.0, 1.0);
		s->stroke();

		s->rotate(seg);
	}
}

osg::Geode* createExample(unsigned int size) {
	osg::Geode*      geode = new osg::Geode();
	osgCairo::Image* image = new osgCairo::Image();
	
	if(image->allocateSurface(size, size, CAIRO_FORMAT_ARGB32) && image->createContext()) {
		osg::Texture2D* texture = new osg::Texture2D();
		osg::Geometry*  geom    = osg::createTexturedQuadGeometry(
			osg::Vec3(0.0f, 0.0f, 0.0f),
			osg::Vec3(image->s(), 0.0f, 0.0f),
			osg::Vec3(0.0f, image->t(), 0.0f),
			0.0f,
			0.0f, 
			1.0f,
			1.0f
		);

		drawPie(image);

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
		state->setAttributeAndModes(
			new osg::BlendFunc(osg::BlendFunc::ONE, osg::BlendFunc::ONE_MINUS_SRC_ALPHA)
		);

		image->dirty();

		geode->addDrawable(geom);
	}

	return geode;
}

osg::Matrix createInvertedYOrthoProjectionMatrix(float width, float height) {
	osg::Matrix m = osg::Matrix::ortho2D(0.0f, width, 0.0f, height);
	osg::Matrix s = osg::Matrix::scale(1.0f, -1.0f, 1.0f);
	osg::Matrix t = osg::Matrix::translate(0.0f, -height, 0.0f);

	return t * s * m;
}

osg::Camera* createOrthoCamera(float width, float height) {
	osg::Camera* camera = new osg::Camera();

	camera->getOrCreateStateSet()->setMode(
		GL_LIGHTING,
		osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
	);

	camera->setProjectionMatrix(osg::Matrix::ortho2D(0.0, width, 0.0f, height));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);

	return camera;
}

osg::Camera* createInvertedYOrthoCamera(float width, float height) {
	osg::Camera* camera = createOrthoCamera(width, height);

	camera->setProjectionMatrix(createInvertedYOrthoProjectionMatrix(width, height));

	return camera;
}

int main(int, char**) {
	osgViewer::Viewer viewer;

	unsigned int size = 512;

	osg::Camera* camera = createInvertedYOrthoCamera(size, size);
	osg::Group*  group  = new osg::Group();

	camera->addChild(createExample(size));

	group->addChild(camera);
	group->addChild(osgDB::readNodeFile("cow.osg"));

	viewer.setSceneData(group);
	viewer.setUpViewInWindow(50, 50, size, size);
	viewer.addEventHandler(new osgViewer::StatsHandler());

	return viewer.run();
}
