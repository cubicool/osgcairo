// -*-c++-*- osgCairo - Copyright (C) 2009 Jeremy Moles

#include <osgDB/Registry>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgCairo/ReadFile>

class ReaderWriterCairo: public osgDB::ReaderWriter {
public:
	virtual const char* className() const {
		return "Cairo Image Reader/Writer";
	}

	virtual bool acceptsExtension(const std::string& ext) const {
		return osgDB::equalCaseInsensitive(ext, "cairo");
	}

	virtual ReadResult readImage(const std::string& file, const Options* opts) const {
		// First, check to see if we accept the extension.
		std::string ext = osgDB::getLowerCaseFileExtension(file);

		if(!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

		// Second, check to see if the file is in our OSG_FILE_PATH directory.
		std::string filename = osgDB::findDataFile(file, opts);

		if(filename.empty()) return ReadResult::FILE_NOT_FOUND;

		osgCairo::Image* image = osgCairo::readImageFile(filename);

		if(!image) return std::s
	}
};

// osgDB::RegisterReaderWriterProxy<ReaderWriterCairo> g_readerWriterCairoProxy;
REGISTER_OSGPLUGIN(cairo, ReaderWriterCairo)

