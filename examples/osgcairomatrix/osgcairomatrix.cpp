#include <osg/Math>
#include <osgCairo/Matrix>
#include <iostream>

int main(int, char**) {
	osgCairo::Matrix identity  = osgCairo::Matrix::identity();
	osgCairo::Matrix translate = osgCairo::Matrix::translate(20.0f, 20.0f);
	osgCairo::Matrix scale     = osgCairo::Matrix::scale(10.0f, 10.0f);
	osgCairo::Matrix rotate    = osgCairo::Matrix::rotate(osg::PI * 1.0f);

	std::cout << "Identity " << std::endl << identity;
	std::cout << "Translate " << std::endl << translate;
	std::cout << "Scale " << std::endl << scale;
	std::cout << "Rotate " << std::endl << rotate;
	
	osgCairo::Matrix m(1.0, 0.0f, 0.0f, 1.0f);

	// m.makeTranslate(20.0f, 20.0f);
	// m.makeScale(10.0f, 10.0f);
	// m.makeRotate(osg::PI * 1.0f);
	// m.makeInvert();
}
