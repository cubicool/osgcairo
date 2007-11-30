#include <osg/Notify>
#include <osg/Math>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osgViewer/Viewer>
#include <osgCairo/SurfaceImage>

osg::Geode* createGroup() {
	osg::Geode*             geode   = new osg::Geode();
	osg::Texture2D*         texture = new osg::Texture2D();
	osgCairo::SurfaceImage* image   = new osgCairo::SurfaceImage("testing");
	
	if(image->allocateCairo(256, 256)) {
		image->scale(256.0f, 256.0f);
		image->setSourceRGBA(0.2f, 0.2f, 0.4f, 1.0f);
		image->rectangle(0.0f, 0.0f, 1.0f, 1.0f);
		image->fill();
		image->setSourceRGBA(1.0f, 0.7f, 0.3f);
		image->setLineWidth(0.05);
		image->arc(0.5f, 0.5f, 0.3f, 0.0f, osg::PI * 2);
		image->stroke();
		image->writeToPNG("output.png");
		
		osg::Geometry* geom = osg::createTexturedQuadGeometry(
			osg::Vec3(10.0f, 10.0f, 0.0f),
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
	
		geode->addDrawable(geom);

		image->dirty();
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

	osg::Camera* camera = createOrthoCamera(1280.0f, 1024.0f);
	osg::Geode*  cairo  = createGroup();
	
	camera->addChild(cairo);

	if(camera) {
		viewer.setSceneData(camera);

		return viewer.run();
	}

	else osg::notify(osg::WARN) << "Couldn't create the orthographic camera." << std::endl;

	return 0;
}
