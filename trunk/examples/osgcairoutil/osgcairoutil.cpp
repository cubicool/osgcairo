#include <osg/Math>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgCairo/Image>

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

		/*
		double lw = 1.0f / static_cast<double>(size / 2);
		
		image->scale(size, size);
		image->setLineWidth(lw);
		image->setSourceRGBA(1.0f, 1.0f, 1.0f, 1.0f);
		*/

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
