#include <iostream>
#include <osg/Notify>
#include <osg/Math>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgCairo/ReadFile>

osg::Geometry* createPageGeometry(osg::Image* image) {
	static osg::Vec3 pos(0.0f, 0.0f, 0.0f);

	osg::Texture2D* texture = new osg::Texture2D();
	osg::Geometry*  geom    = osg::createTexturedQuadGeometry(
		pos,
		osg::Vec3(image->s(), 0.0f, 0.0f),
		osg::Vec3(0.0f, 0.0f, image->t()),
		0.0f,
		0.0f, 
		1.0f,
		1.0f
	);

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

	pos += osg::Vec3(image->s() + 5.0f, 0.0f, 0.0f);

	return geom;
}

int main(int argc, char** argv) {
	osgCairo::ImageVector pages;
	osgCairo::PDFOptions  opts;

	// These options will give good results if you want to use Texture2D objects;
	opts.width           = 1024;
	opts.height          = 1024;
	opts.keepAspectRatio = true;
	
	// These options will just use the actual page width/height, and expect you
	// to either accept image scaling or use TextureRectangle.
	// opts.keepPageSize = true;

	if(osgCairo::readFilePDF("file:///tmp/test.pdf", pages, opts)) {
		osgViewer::Viewer viewer;
		
		osg::Geode*  geode  = new osg::Geode();

		for(
			osgCairo::ImageVector::iterator page = pages.begin();
			page != pages.end();
			page++
		) {
			if(!page->valid()) continue;

			geode->addDrawable(createPageGeometry(page->get()));

			page->get()->flipVertical();
		}

		viewer.setSceneData(geode);

		return viewer.run();
	}

	else return 1;
}
