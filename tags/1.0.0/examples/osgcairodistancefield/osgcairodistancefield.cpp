#include <cmath>
#include <cstdlib>
#include <osg/ArgumentParser>
#include <osg/Texture2D>
#include <osg/MatrixTransform>
#include <osgGA/GUIEventHandler>
#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
#include <osgText/Text>
#include <osgCairo/Image>
#include <osgCairo/Util>

const unsigned int WIDTH      = 800;
const unsigned int HEIGHT     = 600;
const float        SCALE_STEP = 0.25f;

osg::Image* createDistanceFieldImage(unsigned int size, int scanSize, int blockSize) {
	cairo_surface_t*      surface = cairo_image_surface_create(CAIRO_FORMAT_A8, size, size);
	cairo_t*              c       = cairo_create(surface);
	cairo_font_options_t* fo      = cairo_font_options_create();

	cairo_font_options_set_hint_style(fo, CAIRO_HINT_STYLE_NONE);

	cairo_set_antialias(c, CAIRO_ANTIALIAS_NONE);
	cairo_set_font_options(c, fo);
	cairo_scale(c, size, size);
	cairo_select_font_face(c, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(c, 0.8f);
	cairo_translate(c, 0.25f, 0.8);
	cairo_text_path(c, "A");
	cairo_fill(c);

	OSG_NOTICE << "Creating Distance Field... " << std::endl;

	osg::ElapsedTime t;

	cairo_surface_t* distanceField = osgCairo::util::createDistanceField(surface, scanSize, blockSize);

	OSG_NOTICE << "Took " << t.elapsedTime() << " seconds." << std::endl;

	// osgCairo::util::writeToPNG(surface, "surface.png");
	// osgCairo::util::writeToPNG(distanceField, "distanceField.png");

	// Reset our size.
	size = cairo_image_surface_get_width(distanceField);

	cairo_font_options_destroy(fo);
	cairo_surface_destroy(surface);
	cairo_destroy(c);

	osgCairo::Image* image = new osgCairo::Image();

	if(image->allocateSurface(size, size, CAIRO_FORMAT_A8)) {
		c = image->createContext();

		if(!cairo_status(c)) {
			cairo_set_source_surface(c, distanceField, 0, 0);
			cairo_paint(c);
			cairo_destroy(c);

			cairo_surface_destroy(distanceField);
		}

		image->flipVertical();
		image->dirty();
	}

	return image;
}

osg::Geometry* createDistanceFieldGeometry(osg::Image* image) {
	osg::Texture2D* texture  = new osg::Texture2D();
	osg::Geometry*  geometry = osg::createTexturedQuadGeometry(
		osg::Vec3(-(image->s() / 2.0f), -(image->t() / 2.0f), -0.1f),
		osg::Vec3(image->s(), 0.0f, 0.0f),
		osg::Vec3(0.0f, image->t(), 0.0f),
		0.0f,
		0.0f, 
		1.0f,
		1.0f
	);
	
	texture->setImage(image);

	texture->setDataVariance(osg::Object::DYNAMIC);

	osg::StateSet* state = geometry->getOrCreateStateSet();

	state->setTextureAttributeAndModes(
		0,
		texture,
		osg::StateAttribute::ON
	);

	state->setMode(GL_BLEND, osg::StateAttribute::ON);
	state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	return geometry;
}

class ScaleSet: public osgGA::GUIEventHandler {
public:
	ScaleSet(osg::Camera* camera):
	_camera(camera) {
	}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
		static float scale = 1.0f;

		if(!_camera.valid()) return false;

		if(ea.getEventType() != osgGA::GUIEventAdapter::KEYDOWN) return false;
		
		osg::StateSet* state = _camera->getOrCreateStateSet();
		osg::Uniform*  aMin  = state->getUniform("AlphaMin");
		osg::Uniform*  aMax  = state->getUniform("AlphaMax");

		bool adjustViewMatrix = true;

		if(ea.getKey() == '=') scale += SCALE_STEP;

		else if(ea.getKey() == '-') scale -= SCALE_STEP;
		
		else if(ea.getKey() == ']') {
			scale            += SCALE_STEP;
			adjustViewMatrix  = false;
		}
		
		else if(ea.getKey() == '[') {
			scale            -= SCALE_STEP;
			adjustViewMatrix  = false;
		}

		else if(ea.getKey() == '\\') {
			const osg::Matrix& vm = _camera->getViewMatrix();

			OSG_NOTICE
				<< "ViewMatrix Scale: " << vm.getScale().x() << std::endl
				<< "Scale: " << scale << std::endl
			;

			return true;
		}

		else return false;

		if(adjustViewMatrix) {
			OSG_NOTICE << "New ViewMatrix Scale: " << scale << std::endl;
			
			osg::Matrix vm(osg::Matrix::scale(scale, scale, 1.0f));
		
			_camera->setViewMatrix(vm);
		}
		
		OSG_NOTICE << "New Scale: " << scale << std::endl;

		aMin->set(std::max(0.0f, 0.5f - 0.07f / scale));
		aMax->set(std::min(0.5f + 0.07f / scale, 1.0f));

		return true;
	}

private:
	osg::observer_ptr<osg::Camera> _camera;
};

osg::Camera* createOrthoCamera(float width, float height, int orderNum, bool center=false) {
	osg::Camera* camera = new osg::Camera();

	camera->getOrCreateStateSet()->setMode(
		GL_LIGHTING,
		osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
	);

	if(center) camera->setProjectionMatrixAsOrtho2D(
		-width / 2.0f,
		width / 2.0f,
		-height / 2.0f,
		height / 2.0f
	);

	else camera->setProjectionMatrixAsOrtho2D(0.0, width, 0.0, height);

	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);

	return camera;
}

osg::Group* createGUI(osg::Geometry* geometry, osg::Vec3::value_type x, osg::Vec3::value_type y) {
	osg::MatrixTransform* mt    = new osg::MatrixTransform();
	osg::Geode*           geode = new osg::Geode();
	osgText::Text*        text  = new osgText::Text();

	text->setFont(osgText::readFontFile("arial.ttf"));
	text->setCharacterSize(20.0f);
	text->setFontResolution(20.0f, 20.0f);
	text->setPosition(osg::Vec3(x, -y, 0.0f));
	text->setAlignment(osgText::Text::LEFT_BOTTOM);
	text->setAxisAlignment(osgText::Text::XY_PLANE);
	text->setText(
		"+/-: increase, decrease global scale\n"
		"[/]: increase, decrease shader scale ONLY\n"
		"\\: print scale values"
	);

	geode->addDrawable(geometry);
	geode->addDrawable(text);

	mt->setMatrix(osg::Matrix::translate(x, y, 0.0f));
	mt->addChild(geode);

	return mt;
}

int main(int argc, char** argv) {
	osg::ArgumentParser args(&argc, argv);
	osgViewer::Viewer   viewer(args);

	osgDB::FilePathList& paths = osgDB::getDataFilePathList();
	
	paths.push_back("../examples/osgcairodistancefield");
	paths.push_back("examples/osgcairodistancefield");
	paths.push_back(".");

	std::string vertPath = "shaders/normal-vert.glsl";
	std::string fragPath = "shaders/normal-frag.glsl";

	while(args.read("--outline")) {
		fragPath = "shaders/outline-frag.glsl";
	}

	while(args.read("--shifted")) {
		vertPath = "shaders/shifted-vert.glsl";
		fragPath = "shaders/shifted-frag.glsl";
	}

	if(args.argc() != 4) {
		OSG_FATAL << "usage: osgcairodistancefield <surface_size> <scan_size> <block_size>" << std::endl;

		return 1;
	}

	vertPath = osgDB::findDataFile(vertPath);
	fragPath = osgDB::findDataFile(fragPath);

	osg::Image* image = createDistanceFieldImage(
		std::atoi(args[1]),
		std::atoi(args[2]),
		std::atoi(args[3])
	);
	
	osg::Geometry* geometry  = createDistanceFieldGeometry(image);
	osg::Camera*   camera    = createOrthoCamera(WIDTH, HEIGHT, 1, true);
	osg::Camera*   guiCamera = createOrthoCamera(WIDTH, HEIGHT, 0);
	osg::Program*  program   = new osg::Program();
	osg::Shader*   vert      = osg::Shader::readShaderFile(osg::Shader::VERTEX, vertPath);
	osg::Shader*   frag      = osg::Shader::readShaderFile(osg::Shader::FRAGMENT, fragPath);

	float scale  = 1.0f;
	float aMin   = std::max(0.0f, 0.5f - 0.07f / scale);
	float aMax   = std::min(0.5f + 0.07f / scale, 1.0f);
	float radius = 20.0f; //80.0f / 32.0f; // distanceFieldRadius
	float ol     = std::max(double(0.2f), double(0.5 - 0.5 / radius / 1.0f)); 
	float oaMax0 = std::max(double(0.0f), ol - 0.07 / scale);
	float oaMax1 = std::min(ol + 0.07 / scale, double(aMin));

	osg::Uniform* color      = new osg::Uniform("Color", osg::Vec4(1.0f, 0.7f, 0.0f, 1.0f));
	osg::Uniform* styleColor = new osg::Uniform("StyleColor", osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	osg::Uniform* alphaMin   = new osg::Uniform("AlphaMin", aMin);
	osg::Uniform* alphaMax   = new osg::Uniform("AlphaMax", aMax);
	osg::Uniform* alphaMax0  = new osg::Uniform("OutlineAlphaMax0", oaMax0);
	osg::Uniform* alphaMax1  = new osg::Uniform("OutlineAlphaMax1", oaMax1);
	osg::Uniform* texture    = new osg::Uniform(osg::Uniform::SAMPLER_2D, "Texture");

	texture->set(0);

	program->addShader(vert);
	program->addShader(frag);

	camera->getOrCreateStateSet()->setAttributeAndModes(program);
	camera->getOrCreateStateSet()->addUniform(color);
	camera->getOrCreateStateSet()->addUniform(styleColor);
	camera->getOrCreateStateSet()->addUniform(alphaMin);
	camera->getOrCreateStateSet()->addUniform(alphaMax);
	camera->getOrCreateStateSet()->addUniform(alphaMax0);
	camera->getOrCreateStateSet()->addUniform(alphaMax1);
	camera->getOrCreateStateSet()->addUniform(texture);
	
	osg::Geode* geode = new osg::Geode();

	geode->addDrawable(geometry);

	camera->addChild(geode);

	guiCamera->addChild(createGUI(geometry, image->s() / 2.0f, image->t() / 2.0f));

	osg::Group* group = new osg::Group();

	group->addChild(camera);
	group->addChild(guiCamera);

	viewer.setSceneData(group);
	viewer.setUpViewInWindow(50, 50, WIDTH, HEIGHT);
	viewer.addEventHandler(new ScaleSet(camera));

	return viewer.run();
}

