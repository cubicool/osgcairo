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

const unsigned int WIDTH      = 800;
const unsigned int HEIGHT     = 600;
const float        SCALE_STEP = 0.05f;

cairo_surface_t* createDistanceField(unsigned int size, int scanSize, int blockSize) {
	cairo_surface_t*      surface = cairo_image_surface_create(CAIRO_FORMAT_A8, size, size);
	cairo_t*              c       = cairo_create(surface);

	cairo_arc(c, size / 2.0f, size / 2.0f, size / 3.0f, 0.0f, 2.0f * osg::PI);
	cairo_fill(c);

	OSG_NOTICE << "Creating Distance Field... " << std::endl;

	osg::ElapsedTime t;

	cairo_surface_t* distanceField = osgCairo::createDistanceField(surface, scanSize, blockSize);

	OSG_NOTICE << "Took " << t.elapsedTime() << " seconds." << std::endl;

	// cairo_surface_write_to_png(surface, "surface.png");
	// cairo_surface_write_to_png(distanceField, "distanceField.png");

	cairo_surface_destroy(surface);
	cairo_destroy(c);

	return distanceField;
}

osg::Geode* createBackgroundGeode(unsigned int width, unsigned int height) {
	osg::Geode*      geode   = new osg::Geode();
	osgCairo::Image* image   = new osgCairo::Image();
	osg::Texture2D*  texture = new osg::Texture2D();

	if(image->allocateSurface(width, height, CAIRO_FORMAT_ARGB32)) {
		cairo_t* c = image->createContext();

		if(!cairo_status(c)) {
			cairo_set_source_rgba(c, 1.0f, 0.5f, 0.0f, 1.0f);
			cairo_paint(c);
			cairo_destroy(c);
		}

		image->dirty();
	}

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

	state->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
	state->setMode(GL_BLEND, osg::StateAttribute::ON);
	state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	geode->addDrawable(geometry);

	return geode;
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
	state->setAttributeAndModes(
		new osg::BlendFunc(osg::BlendFunc::ONE, osg::BlendFunc::ONE_MINUS_SRC_ALPHA)
	);

	geode->addDrawable(geometry);

	return geode;
}

class ScaleSet: public osgGA::GUIEventHandler {
public:
	osg::MatrixTransform* getMatrix(osgGA::GUIActionAdapter& aa) {
		osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);

		if(!view) return 0;
		
		osg::Camera* camera = dynamic_cast<osg::Camera*>(view->getSceneData());

		if(!camera) return 0;
		
		osg::MatrixTransform* matrix = dynamic_cast<osg::MatrixTransform*>(camera->getChild(0));

		if(!matrix) return 0;

		return matrix;
	}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
		static float scale = 1.0f;
		
		if(
			ea.getEventType() != osgGA::GUIEventAdapter::KEYDOWN ||
			(
				ea.getKey() != '=' &&
				ea.getKey() != '-'
			)
		) return false;

		osg::MatrixTransform* matrix = getMatrix(aa);
		
		if(!matrix) return false;

		osg::StateSet* state = matrix->getStateSet();
		osg::Uniform*  aMin  = state->getUniform("AlphaMin");
		osg::Uniform*  aMax  = state->getUniform("AlphaMax");

		if(ea.getKey() == '=') scale += SCALE_STEP;

		else if(ea.getKey() == '-') scale -= SCALE_STEP;
		
		// OSG_NOTICE << "New Scale: " << scale << std::endl;

		aMin->set(std::max(0.0f, 0.5f - 0.07f / (scale / 2.0f)));
		aMax->set(std::min(0.5f + 0.07f / (scale / 2.0f), 1.0f));

		matrix->setMatrix(osg::Matrix::scale(scale, scale, 1.0f));

		// OSG_NOTICE << "AlphaMin: " << std::max(0.0f, 0.5f - 0.07f / scale) << std::endl;
		// OSG_NOTICE << "AlphaMax: " << std::min(0.5f + 0.07f / scale, 1.0f) << std::endl;

		return true;
	}
};

osg::Camera* createOrthoCamera(float width, float height) {
	osg::Camera* camera = new osg::Camera();

	camera->getOrCreateStateSet()->setMode(
		GL_LIGHTING,
		osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
	);

	camera->setProjectionMatrixAsOrtho2D(
		-width / 2.0f,
		width / 2.0f,
		-height / 2.0f,
		height / 2.0f
	);

	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);

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

	osg::Geode*   geode   = createDistanceFieldGeode(distanceField);
	osg::Geode*   bg      = createBackgroundGeode(WIDTH, HEIGHT);
	osg::Camera*  camera  = createOrthoCamera(WIDTH, HEIGHT);
	osg::Program* program = new osg::Program();
	osg::Shader*  vert    = osg::Shader::readShaderFile(osg::Shader::VERTEX, vertPath);
	osg::Shader*  frag    = osg::Shader::readShaderFile(osg::Shader::FRAGMENT, fragPath);

	float scale  = 1.0f;
	float aMin   = std::max(0.0f, 0.5f - 0.07f / scale);
	float aMax   = std::min(0.5f + 0.07f / scale, 1.0f);
	float radius = 1.0f;
	float ol     = std::max(double(0.2f), double(0.5 - 0.5 / radius)); 
	float oaMax0 = std::max(double(0.0f), ol - 0.07 / scale);
	float oaMax1 = std::min(ol + 0.07 / scale, double(aMin));

	osg::Uniform* color      = new osg::Uniform("Color", osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	osg::Uniform* styleColor = new osg::Uniform("StyleColor", osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	osg::Uniform* alphaMin   = new osg::Uniform("AlphaMin", aMin);
	osg::Uniform* alphaMax   = new osg::Uniform("AlphaMax", aMax);
	osg::Uniform* alphaMax0  = new osg::Uniform("OutlineAlphaMax0", oaMax0);
	osg::Uniform* alphaMax1  = new osg::Uniform("OutlineAlphaMax1", oaMax1);
	osg::Uniform* texture    = new osg::Uniform(osg::Uniform::SAMPLER_2D, "Texture");

	texture->set(0);

	program->addShader(vert);
	program->addShader(frag);

	osg::MatrixTransform* matrix = new osg::MatrixTransform();

	matrix->getOrCreateStateSet()->setAttributeAndModes(program);
	matrix->getOrCreateStateSet()->addUniform(color);
	matrix->getOrCreateStateSet()->addUniform(styleColor);
	matrix->getOrCreateStateSet()->addUniform(alphaMin);
	matrix->getOrCreateStateSet()->addUniform(alphaMax);
	matrix->getOrCreateStateSet()->addUniform(alphaMax0);
	matrix->getOrCreateStateSet()->addUniform(alphaMax1);
	matrix->getOrCreateStateSet()->addUniform(texture);
	matrix->addChild(geode);

	camera->addChild(matrix);
	camera->addChild(bg);

	viewer.setSceneData(camera);
	viewer.setUpViewInWindow(50, 50, WIDTH, HEIGHT);
	viewer.addEventHandler(new ScaleSet());

	int r = viewer.run();

	cairo_surface_destroy(distanceField);

	return r;
}

