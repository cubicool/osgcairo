// -*-c++-*- osgCairo - Copyright (C) 2006 Jeremy Moles

#include <osgCairo/Canvas>

namespace osgCairo {

Canvas::Canvas(CairoFormat format):
_format        (format),
_width         (0),
_height        (0),
_textureWidth  (0),
_textureHeight (0) {
}

Canvas::Canvas(
	unsigned int         width,
	unsigned int         height,
	unsigned int         textureWidth,
	unsigned int         textureHeight,
	CairoFormat          format,
	const unsigned char* data
):
_format          (format),
_width           (width),
_height          (height),
_textureWidth    (textureWidth),
_textureHeight   (textureHeight) {
	allocateSurface(_width, _height, _textureWidth, _textureHeight, data);
}

Canvas::Canvas(const Canvas& c, const osg::CopyOp& co):
osg::Object      (c, co),
_format          (c._format),
_width           (c._width),
_height          (c._height),
_textureWidth    (c._textureWidth),
_textureHeight   (c._textureHeight) {
}

bool Canvas::allocateSurface(
	unsigned int         width,
	unsigned int         height,
	unsigned int         textureWidth,
	unsigned int         textureHeight,
	const unsigned char* data
) {
	_width         = width;
	_height        = height;
	_textureWidth  = textureWidth;
	_textureHeight = textureHeight;

	unsigned int numRows = _height / _textureHeight;
	unsigned int numCols = _width / _textureWidth;

	if(_height % _textureHeight) numRows++;

	if(_width % _textureWidth) numCols++;

	/*
	_textures = gcnew TextureArray(numRows, numCols);

	for(unsigned int r = 0; r < numRows; r++) {
		for(unsigned int c = 0; c < numCols; c++) {
			_textures[r, c] = gcnew TextureSurface(graphicsDevice, textureWidth, textureHeight);
		}
	}
	*/

	return false;
}

/*
	for(int r = 0; r < _textures->GetLength(0); r++) {
		double y = r * _textureHeight;

		for(int c = 0; c < _textures->GetLength(1); c++) {
			TextureSurface^ t = _textures[r, c];

			double x = c * _textureWidth;

			t->SetSourceSurface(this, -x, -y);
			t->SetOperator(Operator::Source);
			t->Paint();
			t->UpdateData();
		}
	}
*/

} // namespace osgCairo
