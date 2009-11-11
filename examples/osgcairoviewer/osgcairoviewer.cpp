#include <iostream>
#include <osg/Notify>
#include <osg/Math>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osgViewer/Viewer>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgCairo/Image>
#include <osgCairo/ReadFile>

osg::Geometry* createGroupCommon(osg::Image* image, bool setBlendMode=true) {
	static osg::Vec3 pos(10.0f, 10.0f, -0.8f);

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

osg::Geode* createExample_loadImages() {
	osg::Geode*      geode = new osg::Geode();
	osgCairo::Image* image = osgCairo::readImageFile("img.png");

	if(!image) return geode;

	int width  = image->s();
	int height = image->t();

	if(image->createContext()) {
		image->roundedCorners(width, height);
		image->setSourceRGBA(1.0f, 1.0f, 1.0f, 0.5f);
		image->setLineWidth(40.0f);
		image->arc(
			width / 2.0f,
			height / 2.0f,
			60.0f,
			0.0f,
			osg::PI + (osg::PI / 2.0f)
		);
		image->stroke();
	}

	geode->addDrawable(createGroupCommon(image));

	return geode;
}

osg::Geode* createExample_simpleDrawing() {
	osg::Geode*      geode = new osg::Geode();
	osgCairo::Image* image = new osgCairo::Image(CAIRO_FORMAT_RGB24);
	
	if(image->allocateSurface(256, 256) && image->createContext()) {
		image->scale(256.0f, 256.0f);
	
		const osg::Vec4 colors[] = {
			osg::Vec4(0.0f, 0.0f, 0.0f, 0.6f),
			osg::Vec4(1.0f, 0.0f, 0.0f, 0.7f),
			osg::Vec4(0.0f, 1.0f, 0.0f, 0.8f),
			osg::Vec4(0.0f, 0.0f, 1.0f, 0.9f),
			osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f),
		};

		for(unsigned int i = 1; i <= 5; i++) {
			image->setSourceRGBA(colors[i - 1]);
			image->setLineWidth(0.05f);
			image->arc(0.5f, 0.5f, i / 12.0f, 0.0f, osg::PI * 2);
			image->stroke();
		}

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
	osgDB::FilePathList& paths = osgDB::getDataFilePathList();
	
	paths.push_back("../examples/osgcairoviewer/");
	paths.push_back("examples/osgcairoviewer/");
	paths.push_back("./");

	osgViewer::Viewer viewer;

	unsigned int width  = 960;
	unsigned int height = 320;

	osg::Camera* camera = createOrthoCamera(width, height);
	osg::Geode*  geode1 = createExample_simpleDrawing();
	osg::Geode*  geode2 = createExample_loadImages();

	camera->addChild(geode1);
	camera->addChild(geode2);

	viewer.setUpViewInWindow(50, 50, width, height);
	viewer.setSceneData(camera);

	return viewer.run();
}
