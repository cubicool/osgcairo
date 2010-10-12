#include <osg/Math>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osgGA/TrackballManipulator>
#include <osgViewer/CompositeViewer>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osgCairo/Image>

osg::Geometry* createGeometry(osg::Image* image) {
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

	pos += osg::Vec3(image->s() + 10.0f, 0.0f, 0.1f);

	return geom;
}

/*
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
		osgCairo::util::roundedCorners(c, image->s(), image->t(), 10.0f, 10.0f);
	
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
*/

osgCairo::Image* createRoadsImage(unsigned int size, unsigned int numCurves) {
	osgCairo::Image* image = new osgCairo::Image();
	
	if(image->allocateSurface(size, size, CAIRO_FORMAT_A8)) {
		cairo_t* c = image->createContext();

		if(!cairo_status(c)) {
			std::vector<osg::Vec2> curves(numCurves * 3);

			for(unsigned int i = 0; i < numCurves * 3; i++) {
				points[i    ].set(std::rand() % size, std::rand() % size);
				points[i + 1].set(std::rand() % size, std::rand() % size);
				points[i + 2].set(std::rand() % size, std::rand() % size);
			}
			
			cairo_destroy(c);

			image->dirty();
		}
	}

	return image;
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
	
	paths.push_back("../examples/osgcairoroads/");
	paths.push_back("examples/osgcairoroads/");
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
	
	unsigned int size = 256;
	
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();

	traits->x                = 100;
	traits->y                = 100;
	traits->width            = size * 2;
	traits->height           = size;
	traits->windowDecoration = true;
	traits->doubleBuffer     = true;
	traits->sharedContext    = 0;
	traits->windowName       = "osgcairoroads";

	osg::ref_ptr<osg::GraphicsContext> gc =
		osg::GraphicsContext::createGraphicsContext(traits.get())
	;
	
	if(gc.valid()) {
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	osgViewer::CompositeViewer viewer;

	osgCairo::Image* roads    = createRoadsImage(size, 5);
	osg::Geometry*   geometry = createGeometry(roads);
	osg::Geode*      geode    = new osg::Geode();

	geode->addDrawable(geometry);

	// Setup our ORTHOGRAPHIC view of the Texture that made the roads.
	{
		osg::Camera*     camera = createCamera();
		osgViewer::View* view   = new osgViewer::View();

		camera->addChild(geode);
		camera->setProjectionMatrixAsOrtho2D(0, size, 0, size);
		camera->setViewMatrix(osg::Matrix::identity());

		view->setSceneData(camera);
		view->getCamera()->setViewport(new osg::Viewport(0, 0, size, size));
		view->getCamera()->setGraphicsContext(gc.get());
		view->getCamera()->setProjectionMatrixAsPerspective(
			30.0f,
			static_cast<float>(size) / static_cast<float>(size),
			1.0f,
			1000.0f
		);

		viewer.addView(view);
	}

	/*
	// Setup our PERSPECTIVE view into the scene.
	{
		osg::Camera*     camera = createCamera();
		osgViewer::View* view   = new osgViewer::View();

		camera->addChild(osgDB::readNodeFile("cow.osg"));
		camera->setProjectionMatrixAsPerspective(
			30.0f,
			static_cast<float>(size) / static_cast<float>(size),
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
		view->getCamera()->setViewport(new osg::Viewport(0, size, size * 2, size));
		view->getCamera()->setGraphicsContext(gc.get());
		view->getCamera()->setProjectionMatrixAsPerspective(
			30.0f,
			static_cast<float>(size) / static_cast<float>(size),
			1.0f,
			1000.0f
		);

		viewer.addView(view);
	}
	*/

	return viewer.run();
}
