#include <osg/Math>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osgGA/TrackballManipulator>
#include <osgViewer/CompositeViewer>
#include <osgDB/ReadFile>
#include <osgCairo/Image>

struct CairoPath: public osg::Referenced {
	CairoPath(cairo_path_t* p):
	path(p) {
	}

	~CairoPath() {
		cairo_path_destroy(path);
	}

	cairo_path_t* path;
};

osg::Geometry* createRoadsImageQuad(osg::Image* image) {
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

	return geom;
}

osg::Geometry* createRoadsGeometry(osgCairo::Image* roads) {
	static osg::Vec3::value_type h = 0.0f;

	CairoPath* cp = dynamic_cast<CairoPath*>(roads->getUserData());

	if(!cp) return 0;

	osg::ref_ptr<osg::Vec3Array> verts  = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();

	unsigned int numQuads = 0;

	for(int i = 0; i < cp->path->num_data; i += cp->path->data[i].header.length) {
		cairo_path_data_t* data = &cp->path->data[i];

		if(data->header.type == CAIRO_PATH_LINE_TO) {
			osg::Vec3 middle(
				data[1].point.x,
				h,
				data[1].point.y
			);

			verts->push_back(middle + osg::Vec3(-1.0f, 0.0f, -1.0f));
			verts->push_back(middle + osg::Vec3( 1.0f, 0.0f, -1.0f));
			verts->push_back(middle + osg::Vec3( 1.0f, 0.0f,  1.0f));
			verts->push_back(middle + osg::Vec3(-1.0f, 0.0f,  1.0f));

			h += 0.1f;
		
			numQuads++;
		}
	}

	if(!numQuads) return 0;

	osg::Vec4::value_type numQuadsFloat = static_cast<osg::Vec4::value_type>(numQuads);
	
	for(osg::Vec4::value_type i = 0.0f; i < numQuadsFloat; i += 1.0f) {
		osg::Vec4::value_type tint = i / numQuadsFloat;

		colors->push_back(osg::Vec4(tint, tint * 0.75f, 0.0f, 1.0f));
	}

	osg::notify(osg::NOTICE) << "Rendered " << numQuads << " quads." << std::endl;

	osg::Geometry* geometry = new osg::Geometry();

	geometry->setVertexArray(verts.get());
	geometry->setColorArray(colors.get());
	geometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, numQuads * 4));

	return geometry;
}

osgCairo::Image* createRoadsImage(unsigned int size) {
	osgCairo::Image* image = new osgCairo::Image();
	
	if(image->allocateSurface(size, size, CAIRO_FORMAT_A8)) {
		cairo_t* c = image->createContext();

		if(!cairo_status(c)) {
			// TODO: This is where we specify the "granularity" of the path; the
			// smaller this number, the MORE geometry will be generated later.
			cairo_set_tolerance(c, 0.01f);
			
			cairo_move_to(c, 0.0f, 0.0f);
			cairo_curve_to(c, size, 0.0f, size, size, 0.0f, size);
			cairo_curve_to(c, 0.0f, 0.0f, size, 0.0f, size, size);
			
			CairoPath* cp = new CairoPath(cairo_copy_path_flat(c));
			
			cairo_stroke(c);
			cairo_destroy(c);

			image->dirty();
			image->setUserData(cp);
		}
	}

	return image;
}

osgViewer::View* createView(
	osg::Geometry*        geometry,
	const osg::Vec4&      vp,
	osg::GraphicsContext* gc
) {
	osg::Geode*      geode = new osg::Geode();
	osgViewer::View* view  = new osgViewer::View();

	geode->addDrawable(geometry);

	view->setSceneData(geode);
	view->getCamera()->setGraphicsContext(gc);
	view->getCamera()->setProjectionMatrixAsPerspective(
		30.0f,
		static_cast<float>(vp[3] - 20.0f) / static_cast<float>(vp[2] - 20.0f),
		1.0f,
		1000.0f
	);

	view->getCamera()->getOrCreateStateSet()->setMode(
		GL_LIGHTING,
		osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
	);

	view->getCamera()->setViewport(new osg::Viewport(
		vp[0] + 10.0f,
		vp[1] + 10.0f,
		vp[2] - 20.0f,
		vp[3] - 20.0f
	));

	return view;
}

int main(int argc, char** argv) {
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
	
	unsigned int size = 512;
	
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

	osgCairo::Image* roads = createRoadsImage(size);

	viewer.addView(createView(
		createRoadsImageQuad(roads),
		osg::Vec4(0, 0, size, size),
		gc.get()
	));
	
	viewer.addView(createView(
		createRoadsGeometry(roads),
		osg::Vec4(size, 0, size, size),
		gc.get()
	));

	return viewer.run();
}
