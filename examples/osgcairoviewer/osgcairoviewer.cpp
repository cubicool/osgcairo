#include <iostream>
#include <osg/Notify>
#include <osg/Math>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgCairo/Image>

osg::Geometry* createGroupCommon(osg::Image* image, bool setBlendMode=true) {
	static osg::Vec3 pos(50.0f, 50.0f, -0.8f);

	osg::Texture2D* texture = new osg::Texture2D();
	osg::Geometry*  geom    = osg::createTexturedQuadGeometry(
		pos,
		osg::Vec3(image->s(), 0.0f, 0.0f),
		osg::Vec3(0.0f, image->t(), 0.0f),
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

	if(setBlendMode) state->setAttributeAndModes(
		new osg::BlendFunc(osg::BlendFunc::ONE, osg::BlendFunc::ONE_MINUS_SRC_ALPHA)
	);

	image->dirty();

	pos += osg::Vec3(image->s() + 50.0f, 0.0f, 0.1f);

	return geom;
}

osg::Geode* createGroup4() {
	osg::Geode*      geode = new osg::Geode();
	osgCairo::Image* image = new osgCairo::Image(CAIRO_FORMAT_A8);
	
	if(image->allocateSurface(64, 64) && image->createContext()) {
		//image->setOriginBottomLeft();
		image->setLineWidth(2.0f);
		image->setSourceRGBA(1.0f, 1.0f, 1.0f, 1.0);
		image->moveTo(10.0f, 10.0f);
		image->lineTo(44.0f, 44.0f);
		image->stroke();
		image->gaussianBlur(10);

		geode->addDrawable(createGroupCommon(image, false));
	}

	return geode;
}

osg::Geode* createGroup3() {
	osg::Geode* geode = new osg::Geode();
	osg::Image* image = osgDB::readImageFile("../examples/osgcairoviewer/img.png");

	geode->addDrawable(createGroupCommon(image));

	unsigned char* newData = osgCairo::convertImageDataToCairoFormat(
		image,
		CAIRO_FORMAT_ARGB32
	);

	if(!newData) {
		osg::notify(osg::NOTICE) << "Couldn't convert img.png to ARGB32." << std::endl;

		return geode;
	}

	osgCairo::Image* cairoImage = new osgCairo::Image(
		image->s(),
		image->t(),
		CAIRO_FORMAT_ARGB32,
		newData
	);
	
	if(cairoImage->createContext()) {
		cairoImage->roundedCorners();
		cairoImage->setSourceRGBA(1.0f, 1.0f, 1.0f, 0.5f);
		cairoImage->setLineWidth(40.0f);
		cairoImage->arc(
			cairoImage->s() / 2.0f,
			cairoImage->t() / 2.0f,
			60.0f,
			0.0f,
			osg::PI + (osg::PI / 2.0f)
		);
		cairoImage->stroke();
	}

	geode->addDrawable(createGroupCommon(cairoImage));

	return geode;
}

osg::Geode* createGroup2() {
	osg::Geode*      geode = new osg::Geode();
	osgCairo::Image* image = new osgCairo::Image();
	
	if(image->allocateSurface(64, 64) && image->createContext()) {
		/*
		// Theme 1!
		image->setSourceRGBA(0.8f, 0.8f, 0.8f);
		image->setLineWidth(1.0f);
		image->roundedRectangle(2.5f, 2.5f, 59.0f, 59.0f, 10.0f);
		image->strokePreserve();
		image->setSourceRGBA(0.7f, 0.5f, 0.2f, 0.2f);
		image->fillPreserve();
		*/

		// Theme 2!
		image->setLineWidth(2.0f);
		image->roundedRectangle(2.0f, 2.0f, 59.0f, 59.0f, 12.0f);
		image->setSourceRGBA(1.0f, 1.0f, 1.0f, 1.0);
		image->fillPreserve();
		//image->setSourceRGBA(0.2f, 0.2f, 0.2f, 1.0f);
		//image->strokePreserve();
			
		geode->addDrawable(createGroupCommon(image));
	}

	return geode;
}

osg::Geode* createGroup1() {
	osg::Geode*      geode = new osg::Geode();
	osgCairo::Image* image = new osgCairo::Image();
	
	if(image->allocateSurface(256, 256) && image->createContext()) {
		image->scale(256.0f, 256.0f);
		//image->setSourceRGBA(0.2f, 0.2f, 0.4f, 1.0f);
		//image->rectangle(0.0f, 0.0f, 1.0f, 1.0f);
		//image->fill();
		image->setSourceRGBA(1.0f, 1.0f, 1.0f);
		image->setLineWidth(0.05f);
		image->arc(0.5f, 0.5f, 0.3f, 0.0f, osg::PI * 2);
		image->stroke();
		image->gaussianBlur(5);

		// If we wanted to create a PNG image of our surface, we could do so here.
		// image->writeToPNG("output.png");
		
		geode->addDrawable(createGroupCommon(image));
	}

	return geode;
}

osg::Camera* createOrthoCamera(unsigned int width, unsigned int height) {
	osg::Camera* camera = new osg::Camera();

	camera->getOrCreateStateSet()->setMode(
		GL_LIGHTING,
		osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
	);
	
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0, width, 0, height));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	
	return camera;
}

int main(int argc, char** argv) {
	osgViewer::Viewer viewer;

	osg::Camera* camera = createOrthoCamera(1280, 1024);
	osg::Geode*  cairo1 = createGroup1();
	osg::Geode*  cairo2 = createGroup2();
	osg::Geode*  cairo3 = createGroup3();
	osg::Geode*  cairo4 = createGroup4();

	if(camera && cairo1 && cairo2 && cairo3 && cairo4) {
		camera->addChild(cairo1);
		camera->addChild(cairo2);
		camera->addChild(cairo3);
		camera->addChild(cairo4);

		viewer.setSceneData(camera);

		return viewer.run();
	}

	return 1;
}
