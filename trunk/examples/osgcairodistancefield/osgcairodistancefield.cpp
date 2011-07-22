#include <cmath>
#include <cstdlib>
#include <osg/ArgumentParser>
#include <osg/Texture2D>
#include <osg/TextureRectangle>
#include <osg/BlendFunc>
#include <osg/MatrixTransform>
#include <osgGA/GUIEventHandler>
#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
#include <osgCairo/Image>
#include <osgCairo/Util>

/* NOTES!
 *
 * View Matrix/Shader Scale Ratio (4096 200 x)
 * 16 = 1:1
 * 32 = 2:1
 * 64 = 4:1
 *
 */

const unsigned int WIDTH      = 1024;
const unsigned int HEIGHT     = 768;
const float        SCALE_STEP = 0.25f;

cairo_surface_t* createDistanceField(unsigned int size, int scanSize, int blockSize) {
	cairo_surface_t*      surface = cairo_image_surface_create(CAIRO_FORMAT_A8, size, size);
	cairo_t*              c       = cairo_create(surface);

	cairo_arc(c, size / 2.0f, size / 2.0f, size / 3.0f, 0.0f, 2.0f * osg::PI);
	cairo_fill(c);

	OSG_NOTICE << "Creating Distance Field... " << std::endl;

	osg::ElapsedTime t;

	cairo_surface_t* distanceField = osgCairo::util::createDistanceField(surface, scanSize, blockSize);

	OSG_NOTICE << "Took " << t.elapsedTime() << " seconds." << std::endl;

	osgCairo::util::writeToPNG(surface, "surface.png");
	osgCairo::util::writeToPNG(distanceField, "distanceField.png");

	cairo_surface_destroy(surface);
	cairo_destroy(c);

	return distanceField;
}

osg::Geode* createDistanceFieldGeode(cairo_surface_t* distanceField) {
	osg::Geode*      geode   = new osg::Geode();
	osgCairo::Image* image   = new osgCairo::Image();
	osg::Texture2D*  texture = new osg::Texture2D();
	unsigned int     size    = cairo_image_surface_get_width(distanceField);

	if(image->allocateSurface(size, size, CAIRO_FORMAT_A8)) {
		cairo_t* c = image->createContext();

		if(!cairo_status(c)) {
			cairo_set_source_surface(c, distanceField, 0, 0);
			cairo_paint(c);
			cairo_destroy(c);
		}

		image->flipVertical();
		image->dirty();
	}

	osg::Geometry*  geometry = osg::createTexturedQuadGeometry(
		osg::Vec3(-(image->s() / 2.0f), -(image->t() / 2.0f), 0.0f),
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

	state->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
	state->setMode(GL_BLEND, osg::StateAttribute::ON);
	state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	geode->addDrawable(geometry);

	return geode;
}

osg::Geode* createGUIGeode(cairo_surface_t* distanceField, unsigned int guiWidth) {
	osg::Geode*            geode   = new osg::Geode();
	osgCairo::Image*       image   = new osgCairo::Image();
	osg::TextureRectangle* texture = new osg::TextureRectangle();
	unsigned int           size    = cairo_image_surface_get_width(distanceField);

	if(image->allocateSurface(guiWidth, size + 20, CAIRO_FORMAT_ARGB32)) {
		cairo_t* c = image->createContext();

		if(!cairo_status(c)) {
			// Draw the texture and the box around the texture.
			cairo_save(c);
			
			cairo_set_source_rgba(c, 0.2f, 0.2f, 0.2f, 0.8f);
			cairo_paint(c);

			cairo_set_source_rgba(c, 1.0f, 1.0f, 1.0f, 1.0f);
			cairo_set_line_width(c, 1.0f);
			cairo_translate(c, 9.0f, 9.0f);
			cairo_rectangle(c, 0.5f, 0.5f, size + 2.0f, size + 2.0f);
			cairo_stroke(c);

			cairo_set_source_surface(c, distanceField, 1, 1);
			cairo_paint(c);

			cairo_restore(c);

			cairo_font_options_t* fo = cairo_font_options_create();

			cairo_font_options_set_hint_style(fo, CAIRO_HINT_STYLE_FULL);

			float fontSize = 15.0f;

			cairo_set_antialias(c, CAIRO_ANTIALIAS_SUBPIXEL);
			cairo_set_font_options(c, fo);
			cairo_select_font_face(c, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
			cairo_set_font_size(c, fontSize);
			cairo_set_source_rgba(c, 1.0f, 1.0f, 1.0f, 1.0f);

			const char* help[] = {
				"The generated texture is to the left.",
				"",
				"Use the U and I keys to adjust the ViewMatrix scale.",
				"Use the J and K keys to adjust the Shader scale.",
				"Use the O and P kess to adjust both scales at once."
			};

			cairo_translate(c, size + 20.0f, 10.0f);

			for(unsigned int i = 0; i < 5; i++) {
				cairo_translate(c, 0, fontSize);
				cairo_text_path(c, help[i]);
				cairo_fill(c);
			}

			cairo_font_options_destroy(fo);
			cairo_destroy(c);
		}

		image->flipVertical();
		image->dirty();
	}

	osg::Geometry*  geometry = osg::createTexturedQuadGeometry(
		osg::Vec3(0.0f, 0.0f, 0.0f),
		osg::Vec3(image->s(), 0.0f, 0.0f),
		osg::Vec3(0.0f, image->t(), 0.0f),
		0.0f,
		0.0f, 
		image->s(),
		image->t()
	);
	
	texture->setImage(image);
	texture->setDataVariance(osg::Object::DYNAMIC);

	osg::StateSet* state = geometry->getOrCreateStateSet();

	state->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
	state->setMode(GL_BLEND, osg::StateAttribute::ON);
	state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	state->setAttributeAndModes(
		new osg::BlendFunc(osg::BlendFunc::ONE, osg::BlendFunc::ONE_MINUS_SRC_ALPHA)
	);

	geode->addDrawable(geometry);

	return geode;
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

		OSG_NOTICE << "AlphaMin: " << std::max(0.0f, 0.5f - 0.07f / scale) << std::endl;
		OSG_NOTICE << "AlphaMax: " << std::min(0.5f + 0.07f / scale, 1.0f) << std::endl;

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
	camera->setRenderOrder(osg::Camera::POST_RENDER, orderNum);

	return camera;
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

	cairo_surface_t* distanceField = createDistanceField(
		std::atoi(args[1]),
		std::atoi(args[2]),
		std::atoi(args[3])
	);

	osg::Geode*   geode     = createDistanceFieldGeode(distanceField);
	osg::Camera*  camera    = createOrthoCamera(WIDTH, HEIGHT, 0, true);
	osg::Camera*  guiCamera = createOrthoCamera(WIDTH, HEIGHT, 1);
	osg::Program* program   = new osg::Program();
	osg::Shader*  vert      = osg::Shader::readShaderFile(osg::Shader::VERTEX, vertPath);
	osg::Shader*  frag      = osg::Shader::readShaderFile(osg::Shader::FRAGMENT, fragPath);

	float scale  = 1.0f;
	float aMin   = std::max(0.0f, 0.5f - 0.07f / scale);
	float aMax   = std::min(0.5f + 0.07f / scale, 1.0f);
	float radius = 1.0f;
	float ol     = std::max(double(0.2f), double(0.5 - 0.5 / radius)); 
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
	camera->addChild(geode);

	guiCamera->addChild(createGUIGeode(distanceField, WIDTH));

	osg::Group* group = new osg::Group();

	group->addChild(camera);
	group->addChild(guiCamera);

	viewer.setSceneData(group);
	viewer.setUpViewInWindow(50, 50, WIDTH, HEIGHT);
	viewer.addEventHandler(new ScaleSet(camera));

	int r = viewer.run();

	cairo_surface_destroy(distanceField);

	return r;
}

