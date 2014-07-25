#include <sstream>
#include <osg/Notify>
#include <osg/Math>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osgGA/TrackballManipulator>
#include <osgViewer/CompositeViewer>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgCairo/Version>
#include <osgCairo/Image>
#include <osgCairo/ReadFile>
#include <osgCairo/Util>

osg::Geometry* createGeometry(osg::Image* image, bool setBlendMode=true) {
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

osg::Geode* createExample_loadImages() {
	// We demonstrate using the standard osgDB::Options object here to tell
	// osgCairo to add an alpha channel to our GL_RGB PNG image. If you wanted
	// to you could simply save an alpha channel in your PNG, in which case
	// something like this isn't necessary. FUTHERMORE, if you dont' want (or
	// need) an alpha channel, just leave the image as-is.
	osg::ref_ptr<osgDB::ReaderWriter::Options> opts =
		new osgDB::ReaderWriter::Options("addAlphaToRGB")
	;

	osg::Geode*      geode = new osg::Geode();
	osgCairo::Image* image = osgCairo::readImageFile("img.png", opts.get());

	if(!image) return geode;

	int width  = image->s();
	int height = image->t();

	cairo_t* c = image->createContext();

	if(!cairo_status(c)) {
		osgCairo::roundedRectangleApply(
			c,
			10.0f,
			10.0f,
			image->s() - 20.0f,
			image->t() - 20.0f,
			20.0f
		);

		cairo_set_source_rgba(c, 1.0f, 1.0f, 1.0f, 0.5f);
		cairo_set_line_width(c, 40.0f);
		cairo_arc(
			c,
			width / 2.0f,
			height / 2.0f,
			60.0f,
			0.0f,
			osg::PI + (osg::PI / 2.0f)
		);

		cairo_stroke(c);
		cairo_destroy(c);
	}

	geode->addDrawable(createGeometry(image));

	return geode;
}

osg::Geode* createExample_simpleDrawing() {
	osg::Geode*      geode = new osg::Geode();
	osgCairo::Image* image = new osgCairo::Image();

	if(image->allocateSurface(256, 256, CAIRO_FORMAT_ARGB32)) {
		cairo_t* c = image->createContext();

		if(!cairo_status(c)) {
			cairo_scale(c, 256.0f, 256.0f);

			const osg::Vec4 colors[] = {
				osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
				osg::Vec4(1.0f, 0.0f, 0.0f, 0.8f),
				osg::Vec4(0.0f, 1.0f, 0.0f, 0.6f),
				osg::Vec4(0.0f, 0.0f, 1.0f, 0.4f),
				osg::Vec4(1.0f, 1.0f, 1.0f, 0.2f),
			};

			for(unsigned int i = 1; i <= 5; i++) {
				const osg::Vec4& co = colors[i - 1];

				cairo_set_source_rgba(c, co[0], co[1], co[2], co[3]);
				cairo_set_line_width(c, 0.05f);
				cairo_arc(c, 0.5f, 0.5f, i / 12.0f, 0.0f, osg::PI * 2);
				cairo_stroke(c);
			}

			// If we wanted to create a PNG image of our surface, we could do so here.
			// osgCairo::writeToPNG(image->getSurface(), "output.png");

			// osgCairo::gaussianBlur(image->getSurface(), 10.0f);

			cairo_destroy(c);

			image->dirty();
		}

		geode->addDrawable(createGeometry(image));
	}

	return geode;
}

osg::Camera* createCamera() {
	osg::Camera* camera = new osg::Camera();

	camera->getOrCreateStateSet()->setMode(
		GL_LIGHTING,
		osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
	);

	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);

	return camera;
}

int main(int, char**) {
	osgDB::FilePathList& paths = osgDB::getDataFilePathList();

	paths.push_back("../examples/osgcairoviewer/");
	paths.push_back("examples/osgcairoviewer/");
	paths.push_back("./");

	// Setup a single graphics context; I got this code from the osgcompositeviewer
	// example.
	osg::GraphicsContext::WindowingSystemInterface* wsi =
		osg::GraphicsContext::getWindowingSystemInterface()
	;

	if(!wsi) {
		osg::notify(osg::NOTICE)
			<< "No WindowSystemInterface available, cannot create windows."
			<< std::endl
		;

		return 1;
	}

	unsigned int width  = 256 * 3 + 40;
	unsigned int height = 276;

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();

	traits->x                = 100;
	traits->y                = 100;
	traits->width            = width;
	traits->height           = height * 2;
	traits->windowDecoration = true;
	traits->doubleBuffer     = true;
	traits->sharedContext    = 0;
	traits->windowName       = "osgcairoviewer";

	osg::ref_ptr<osg::GraphicsContext> gc =
		osg::GraphicsContext::createGraphicsContext(traits.get())
	;

	if(gc.valid()) {
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Now we setup the viewer. The size of the osg::Views is hard-coded for now, since
	// I know all the objects are going to be 256x256.
	osgViewer::CompositeViewer viewer;

	// Here's our main scene data; it will be viewed in two different ways, but it's the
	// same data both times.
	osg::ref_ptr<osg::Geode> ex1 = createExample_loadImages();
	osg::ref_ptr<osg::Geode> ex2 = createExample_simpleDrawing();
	osg::ref_ptr<osg::Node>  cow = osgDB::readNodeFile("cow.osg");

	// Setup our ORTHOGRAPHIC view into the scene.
	{
		osg::Camera*     camera = createCamera();
		osg::Group*      group  = new osg::Group();
		osgViewer::View* view   = new osgViewer::View();

		camera->addChild(ex1.get());
		camera->addChild(ex2.get());
		camera->setProjectionMatrixAsOrtho2D(0, width, 0, height);
		camera->setViewMatrix(osg::Matrix::identity());

		group->addChild(cow);
		group->addChild(camera);

		view->setSceneData(group);
		view->setCameraManipulator(new osgGA::TrackballManipulator());
		view->getCamera()->setViewport(new osg::Viewport(0, 0, width, height));
		view->getCamera()->setGraphicsContext(gc.get());
		view->getCamera()->setProjectionMatrixAsPerspective(
			30.0f,
			static_cast<float>(width) / static_cast<float>(height),
			1.0f,
			1000.0f
		);

		viewer.addView(view);
	}

	// Setup our PERSPECTIVE view into the scene.
	{
		osg::Camera*          camera = createCamera();
		osg::MatrixTransform* matrix = new osg::MatrixTransform();
		osg::Group*           group  = new osg::Group();
		osgViewer::View*      view   = new osgViewer::View();

		matrix->addChild(ex1.get());
		matrix->addChild(ex2.get());
		matrix->setMatrix(osg::Matrix::rotate(osg::PI_4, 0.0f, 1.0f, 0.0f));

		camera->addChild(matrix);
		camera->setProjectionMatrixAsPerspective(
			30.0f,
			static_cast<float>(width) / static_cast<float>(height),
			-1.0f,
			1.0f
		);

		const osg::BoundingSphere& bs  = matrix->getBound();
		const osg::Vec3&           c   = bs.center();
		const osg::Vec3&           eye = osg::Vec3(c.x(), c.y(), bs.radius() * 2.0f);

		camera->setViewMatrixAsLookAt(eye, bs.center(), osg::Vec3(0.0f, 1.0f, 0.0f));

		group->addChild(cow);
		group->addChild(camera);

		view->setSceneData(group);
		view->setCameraManipulator(new osgGA::TrackballManipulator());
		view->getCamera()->setViewport(new osg::Viewport(0, height, width, height));
		view->getCamera()->setGraphicsContext(gc.get());
		view->getCamera()->setProjectionMatrixAsPerspective(
			30.0f,
			static_cast<float>(width) / static_cast<float>(height),
			1.0f,
			1000.0f
		);

		viewer.addView(view);
	}

	return viewer.run();
}
