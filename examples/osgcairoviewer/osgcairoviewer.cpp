#include <sstream>
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

	pos += osg::Vec3(image->s() + 10.0f, 0.0f, 0.1f);

	return geom;
}

osg::Geode* createExample_loadSVG() {
	osg::ref_ptr<osgDB::Options> opts = new osgDB::Options("256x256 ");

	osg::Geode* geode = new osg::Geode();
	osg::Image* image = osgDB::readImageFile("img.svg", opts.get());

	if(!image) return geode;

	osgCairo::CairoSurface* surface = static_cast<osgCairo::CairoSurface*>(
		opts->getPluginData("surface")
	);

	if(!surface) osg::notify(osg::NOTICE)
		<< "You did not choose to save the CairoSurface object when you loaded "
		<< "the SVG file. It is also possible your version of the SVG loader "
		<< "doesn't support this feature." << std::endl
	;

	else free(surface);

	/*
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
	*/

	geode->addDrawable(createGroupCommon(image));

	return geode;
}

osg::Geode* createExample_loadImages() {
	// We demonstrate using the standard osgDB::Options object here to tell
	// osgCairo to add an alpha channel to our GL_RGB PNG image. If you wanted
	// to you could simply save an alpha channel in your PNG, in which case
	// something like this isn't necessary. FUTHERMORE, if you dont' want (or
	// need) an alpha channel, just leave the image as-is.
	osg::ref_ptr<osgDB::Options> opts = new osgDB::Options("addAlphaToRGB");

	osg::Geode*      geode = new osg::Geode();
	osgCairo::Image* image = osgCairo::readImageFile("img.png", opts.get());

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
	osgCairo::Image* image = new osgCairo::Image(CAIRO_FORMAT_ARGB32);
	
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

	osgDB::Input input;

	std::istringstream options("x 100 y 200");

	input.attach(&options);

	int x, y;

	input.read("x", x);
	input.read("y", y);

	osg::notify(osg::NOTICE) << "x: " << x << " y: " << y << " save: " << input.read("saveSurface") << std::endl;

	/*
	osgViewer::Viewer viewer;

	unsigned int width  = 256 * 3 + 40;
	unsigned int height = 276;

	osg::Camera* camera = createOrthoCamera(width, height);
	osg::Geode*  geode1 = createExample_simpleDrawing();
	osg::Geode*  geode2 = createExample_loadImages();
	osg::Geode*  geode3 = createExample_loadSVG();

	camera->addChild(geode1);
	camera->addChild(geode2);
	camera->addChild(geode3);

	viewer.setUpViewInWindow(50, 50, width, height);
	viewer.setSceneData(camera);

	return viewer.run();
	*/

	return 0;
}
