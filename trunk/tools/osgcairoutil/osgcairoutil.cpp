#include <pycairo.h>
#include <osgCairo/Util>

extern "C" {

static Pycairo_CAPI_t* Pycairo_CAPI;

static PyObject* py_map_path_onto(PyObject* self, PyObject* args) {
	PyObject* cr   = 0;
	PyObject* path = 0;

	if(!PyArg_ParseTuple(
		args, "O!O!",
		&PycairoContext_Type, &cr,
		&PycairoPath_Type, &path
	)) return 0;

	if(!cr || !path) return 0;

	osgCairo::util::mapPathOnto(
		((PycairoContext*)(cr))->ctx,
		((PycairoPath*)(path))->path
	);

	Py_RETURN_NONE;
}

static PyObject* py_gaussian_blur(PyObject* self, PyObject* args) {
	PyObject* surf      = 0;
	double    radius    = 0.0f;
	double    deviation = 0.0f;

	if(!PyArg_ParseTuple(
		args, "O!|dd",
		&PycairoSurface_Type, &surf,
		&radius,
		&deviation
	)) return 0;

	if(!surf) return 0;

	osgCairo::util::gaussianBlur(((PycairoSurface*)(surf))->surface, radius, deviation);

	Py_RETURN_NONE;
}

static PyObject* py_rounded_rectangle(PyObject* self, PyObject* args) {
	PyObject* cr = 0;
	double    x  = 0.0f;
	double    y  = 0.0f;
	double    w  = 0.0f;
	double    h  = 0.0f;
	double    r  = 10.0f;

	if(!PyArg_ParseTuple(
		args, "O!dddd|d",
		&PycairoContext_Type, &cr,
		&x,
		&y,
		&w,
		&h,
		&r
	)) return 0;

	if(!cr) return 0;

	osgCairo::util::roundedRectangle(
		((PycairoContext*)(cr))->ctx,
		x,
		y,
		w,
		h,
		r
	);

	Py_RETURN_NONE;
}

static PyObject* py_rounded_corners(PyObject* self, PyObject* args) {
	PyObject* cr = 0;
	double    w  = 0.0f;
	double    h  = 0.0f;
	double    s  = 0.01f;
	double    r  = 0.075f;

	if(!PyArg_ParseTuple(
		args, "O!dd|dd",
		&PycairoContext_Type, &cr,
		&w,
		&h,
		&s,
		&r
	)) return 0;

	if(!cr) return 0;

	osgCairo::util::roundedCorners(
		((PycairoContext*)(cr))->ctx,
		w,
		h,
		s,
		r
	);

	Py_RETURN_NONE;
}

static PyMethodDef module_methods[] = {
	{ 
		"map_path_onto", py_map_path_onto, METH_VARARGS,
		"Map a new path onto an older path to achieve twisted text, etc."
	},
	{
		"gaussian_blur", py_gaussian_blur, METH_VARARGS,
		"Do a permanent ARGB32 cairo_surface_t blur using a variable kernel size."
	},
	{
		"rounded_rectangle", py_rounded_rectangle, METH_VARARGS,
		"..."
	},
	{
		"rounded_corners", py_rounded_corners, METH_VARARGS,
		"..."
	},
	{ 0, 0, 0, 0 }
};

PyMODINIT_FUNC init_osgcairoutil(void) {
	Pycairo_IMPORT;

	Py_InitModule3(
		"_osgcairoutil",
		module_methods,
		"foo"
	);
}

}
