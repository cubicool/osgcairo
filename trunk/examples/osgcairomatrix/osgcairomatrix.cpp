#include <osg/Math>
#include <osgCairo/Matrix>
#include <cstdio>

void printMatrix(const char* message, const osgCairo::Matrix& m) {
	std::printf(
		"%s\n"
		"%.4f %.4f\n"
		"%.4f %.4f\n"
		"%.4f %.4f\n\n",
		message,
		m.xx, m.yx,
		m.xy, m.yy,
		m.x0, m.y0
	);
}

int main(int, char**) {
	osgCairo::Matrix identity  = osgCairo::Matrix::identity();
	osgCairo::Matrix translate = osgCairo::Matrix::translate(20.0f, 20.0f);
	osgCairo::Matrix scale     = osgCairo::Matrix::scale(10.0f, 10.0f);
	osgCairo::Matrix rotate    = osgCairo::Matrix::rotate(osg::PI * 1.0f);
	
	printMatrix("Identity Matrix:", identity);
	printMatrix("Translation Matrix:", translate);
	printMatrix("Scale Matrix:", scale);
	printMatrix("Rotation Matrix:", rotate);
	
	osgCairo::Matrix m(1.0, 0.0f, 0.0f, 1.0f);

	// m.makeTranslate(20.0f, 20.0f);
	// m.makeScale(10.0f, 10.0f);
	// m.makeRotate(osg::PI * 1.0f);
	// m.makeInvert();

	printMatrix("m! (translated, scaled, rotated, and inverted)", m);
}
