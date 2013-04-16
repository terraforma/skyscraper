# include "version.h"

namespace
{
	enum version {
		VERSION_MAJOR = 0, VERSION_MINOR = 2, VERSION_PATCH = 0
	};
};

namespace LibTerra {
	//
	//	Return an structure that details the current version.
	const tfVersion GetVersion()
	{
		return tfVersion(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	}
}
