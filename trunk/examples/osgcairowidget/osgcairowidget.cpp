#include <osg/BlendFunc>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgWidget/WindowManager>
#include <osgWidget/ViewerEventHandlers>
#include <osgWidget/Box>
#include <osgCairo/SurfaceImage>

typedef osgWidget::point_type point_type;

osgCairo::SurfaceImage* createButton(point_type w, point_type h) {
	osgCairo::SurfaceImage* image     = new osgCairo::SurfaceImage("button-WOOT!");
	osgCairo::Pattern*      gradient1 = new osgCairo::LinearPattern(0.0f, 0.0f, 0.0f, h);
	osgCairo::Pattern*      gradient2 = new osgCairo::LinearPattern(0.0f, 0.0f, 0.0f, h);
	osgCairo::Pattern*      gradient3 = new osgCairo::LinearPattern(0.0f, 0.0f, 0.0f, h);

	if(!image->allocateCairo(
		static_cast<unsigned int>(w),
		static_cast<unsigned int>(h)
	)) return 0;
	
	point_type lw = floor(w * 0.02f);
	point_type x  = floor(w * 0.1f) + 0.5f;
	point_type y  = floor(x * 2.0f);
	point_type r  = floor(w * 0.3f);
	point_type xx = floor(x + (lw * 1.5f)) + 0.5f;
	point_type yy = floor(xx * 2.0f);
	point_type rr = r - (r * 0.2f);

	image->setLineWidth(lw);

	image->save();
	gradient1->addColorStopRGBA(0.0f, 0.3f, 0.3f, 0.3f, 1.0f);
	gradient1->addColorStopRGBA(1.0f, 0.7f, 0.7f, 0.7f, 1.0f);
	image->setSource(gradient1);
	image->roundedRectangle(x, x, w - y, h - y, r);
	image->stroke();
	image->restore();

	image->save();
	gradient2->addColorStopRGBA(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	gradient2->addColorStopRGBA(1.0f, 0.8f, 0.5f, 0.1f, 0.5f);
	image->setSource(gradient2);
	image->roundedRectangle(xx, xx, w - yy, h - yy, rr);
	image->fill();
	image->restore();
	
	image->save();
	gradient3->addColorStopRGBA(0.0f, 0.7f, 0.7f, 0.7f, 1.0f);
	gradient3->addColorStopRGBA(1.0f, 0.3f, 0.3f, 0.3f, 1.0f);
	image->setSource(gradient3);
	image->roundedRectangle(xx, xx, w - yy, h - yy, rr);
	image->stroke();
	image->restore();

	/*
	image->setLineWidth(3.0f);

	image->save();
	gradient1->addColorStopRGBA(0.0f, 0.3f, 0.3f, 0.3f, 1.0f);
	gradient1->addColorStopRGBA(1.0f, 0.7f, 0.7f, 0.7f, 1.0f);
	image->setSource(gradient1);
	image->roundedRectangle(10.5f, 10.5f, w - 20.0f, h - 20.0f, 55.0f);
	image->stroke();
	image->restore();

	image->save();
	gradient2->addColorStopRGBA(0.0f, 0.9f, 0.9f, 0.9f, 0.4f);
	gradient2->addColorStopRGBA(1.0f, 0.4f, 0.4f, 0.4f, 0.4f);
	image->setSource(gradient2);
	image->roundedRectangle(15.5f, 15.5f, w - 30.0f, h - 30.0f, 45.0f);
	image->fill();
	image->restore();
	
	image->save();
	gradient3->addColorStopRGBA(0.0f, 0.7f, 0.7f, 0.7f, 1.0f);
	gradient3->addColorStopRGBA(1.0f, 0.3f, 0.3f, 0.3f, 1.0f);
	image->setSource(gradient3);
	image->roundedRectangle(15.5f, 15.5f, w - 30.0f, h - 30.0f, 45.0f);
	image->stroke();
	image->restore();
	*/

	image->writeToPNG("foo.png");
	image->flipVertical();
	image->dirty();

	delete gradient1;
	delete gradient2;
	delete gradient3;

	return image;
}

int main(int argc, char** argv) {
	osgViewer::Viewer viewer;

	point_type w = 64.0f;
	point_type h = 64.0f;

	osgWidget::WindowManager* wm  = new osgWidget::WindowManager(1280.0f, 1024.0f, 0xF000000);
	osgWidget::Box*           box = new osgWidget::Box("box", osgWidget::Box::HORIZONTAL);
	osgWidget::Widget*        wid = new osgWidget::Widget("cairo", w, h);

	wid->setImage(createButton(w, h), true);
	wid->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	wid->getOrCreateStateSet()->setAttributeAndModes(
		new osg::BlendFunc(osg::BlendFunc::ONE, osg::BlendFunc::ONE_MINUS_SRC_ALPHA)
	);

	box->getBackground()->setColor(0.0f, 0.0f, 0.0f, 0.0f);
	box->addWidget(wid);

	wm->addChild(box);
	wm->resizeAllWindows();

	viewer.addEventHandler(new osgWidget::MouseHandler(wm, viewer));
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());

	osg::Group*  group  = new osg::Group();
	osg::Camera* camera = wm->createParentOrthoCamera();
	osg::Node*   model  = osgDB::readNodeFile("spaceship.osg");

	model->setNodeMask(0x0F000000);

	camera->addChild(wm);

	group->addChild(camera);
	group->addChild(model);

	viewer.setSceneData(group);

	return viewer.run();
}
