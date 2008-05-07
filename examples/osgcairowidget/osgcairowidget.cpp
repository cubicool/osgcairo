#include <osg/BlendFunc>
#include <osg/Timer>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgWidget/WindowManager>
#include <osgWidget/ViewerEventHandlers>
#include <osgWidget/Box>
#include <osgWidget/Version>
#include <osgCairo/SurfaceImage>

typedef osgWidget::point_type point_type;   

osgCairo::SurfaceImage* createButton(point_type w, point_type h, bool gloss=false) {
	osgCairo::SurfaceImage* image = new osgCairo::SurfaceImage();

	osgCairo::Pattern* gradient1 = new osgCairo::LinearPattern(0.0f, 0.0f, 0.0f, h);
	osgCairo::Pattern* gradient2 = new osgCairo::LinearPattern(0.0f, 0.0f, 0.0f, h);
	osgCairo::Pattern* gradient3 = new osgCairo::LinearPattern(0.0f, 0.0f, 0.0f, h);

	osgCairo::Pattern* gradient4 = new osgCairo::RadialPattern(
		w / 2.0f,
		0.0f,
		0.0f,
		w / 2.0f,
		0.0f,
		h / 2.0f
	);

	if(!image->allocateImage(
		static_cast<unsigned int>(w),
		static_cast<unsigned int>(h)
	) || !image->createContext()) return 0;

	point_type lw = floor(w * 0.01f);
	point_type x  = floor(w * 0.1f) + 0.5f;
	point_type y  = floor(x * 2.0f);
	point_type r  = floor(w * 0.12f);
	point_type xx = floor(x + (lw * 1.5f)) + 0.5f;
	point_type yy = floor(xx * 2.0f);   
	point_type rr = r - (r * 0.2f);

	image->setLineWidth(lw);

	// The inner gradient
	image->save();

	if(gloss) {
		gradient2->addColorStopRGBA(0.0f, 0.4f, 0.4f, 0.2f, 0.1f);
		gradient2->addColorStopRGBA(1.0f, 0.4f, 0.9f, 0.2f, 0.5f);
	}

	else {
		gradient2->addColorStopRGBA(0.0f, 0.4f, 0.4f, 0.2f, 0.1f);
		gradient2->addColorStopRGBA(1.0f, 0.9f, 0.2f, 0.2f, 0.5f);
	}

	image->setSource(gradient2);
	image->roundedRectangle(xx, xx, w - yy, h - yy, rr);
	image->fill();
	image->restore();

	// The outer line
	image->save();
	gradient1->addColorStopRGBA(0.0f, 0.1f, 0.1f, 0.1f, 0.7f);
	gradient1->addColorStopRGBA(1.0f, 0.7f, 0.7f, 0.7f, 0.7f);
	image->setSource(gradient1);
	image->roundedRectangle(x, x, w - y, h - y, r);
	image->stroke();
	image->restore();

	// The inner line
	image->save();
	gradient3->addColorStopRGBA(0.0f, 0.7f, 0.7f, 0.7f, 0.7f);
	gradient3->addColorStopRGBA(1.0f, 0.1f, 0.1f, 0.1f, 0.7f);
	image->setSource(gradient3);
	image->roundedRectangle(xx, xx, w - yy, h - yy, rr);
	image->stroke();
	image->restore();

	// image->writeToPNG("osgcairowidget.png");
	// image->flipVertical();
	image->dirty();

	delete gradient1;
	delete gradient2;
	delete gradient3;
	delete gradient4;

	return image;
}

class CairoButton: public osgWidget::Widget {
	osg::ref_ptr<osg::Image> _normal;   
	osg::ref_ptr<osg::Image> _over;

public:
	CairoButton(point_type w, point_type h):
	osgWidget::Widget("cb", w, h),
	_normal (createButton(w, h)),
	_over   (createButton(w, h, true)) {
		setImage(_normal.get(), true);
		setColor(1.0f, 1.0f, 1.0f, 1.0f);
		setEventMask(osgWidget::EVENT_MASK_MOUSE_MOVE);
		getOrCreateStateSet()->setAttributeAndModes(new osg::BlendFunc(
			osg::BlendFunc::ONE,
			osg::BlendFunc::ONE_MINUS_SRC_ALPHA
		));
	}

	virtual bool mouseEnter(double, double, osgWidget::WindowManager*) {
		setImage(_over.get());

		return true;
	};

	virtual bool mouseLeave(double, double, osgWidget::WindowManager*) {
		setImage(_normal.get());

		return true;
	};
};

int main(int argc, char** argv) {
	/*
	if(!osgWidgetVersionMinimum(0, 1, 6)) {
		osg::notify(osg::FATAL)
			<< argv[0] << " needs osgWidget version 0.1.6 or higher; "
			<< "found " << osgWidgetGetVersion() << "." << std::endl
		;

		return 0;
	}
	*/

	osgViewer::Viewer viewer;

	point_type w = 256.0f;
	point_type h = 256.0f;

	osgWidget::WindowManager* wm = new osgWidget::WindowManager(
		&viewer,
		1280.0f,
		1024.0f,
		0xF000000
	);
	
	osgWidget::Box* box = new osgWidget::Box("box", osgWidget::Box::HORIZONTAL);

	box->getBackground()->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	box->getBackground()->setImage("../examples/osgcairowidget/bg.jpg");
	box->getBackground()->setTexCoord(0.0f, 1.0f, osgWidget::Widget::LOWER_LEFT);
	box->getBackground()->setTexCoord(5.0f, 1.0f, osgWidget::Widget::LOWER_RIGHT);
	box->getBackground()->setTexCoord(5.0f, 0.0f, osgWidget::Widget::UPPER_RIGHT);
	box->getBackground()->setTexCoord(0.0f, 0.0f, osgWidget::Widget::UPPER_LEFT);

	osg::Timer timer;

	box->addWidget(new CairoButton(w, h));
	box->addWidget(new CairoButton(w, h));
	box->addWidget(new CairoButton(w, h));
	box->addWidget(new CairoButton(w, h));
	box->addWidget(new CairoButton(w, h));

	osgWidget::warn() << "Created 5 buttons in " << timer.time_s() << " seconds." << std::endl;

	wm->addChild(box);
	wm->resizeAllWindows();

	viewer.addEventHandler(new osgWidget::MouseHandler(wm));
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());

	osg::Group*  group  = new osg::Group();
	osg::Camera* camera = wm->createParentOrthoCamera();

	camera->addChild(wm);

	group->addChild(camera);

	viewer.setSceneData(group);

	osgWidget::warn() << "Run called after " << timer.time_s() << " seconds." << std::endl;
	
	return viewer.run();
}
