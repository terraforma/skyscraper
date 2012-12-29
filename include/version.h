# ifndef   __SKYSCRAPER_INCLUDE_VERSION_H_
#  define  __SKYSCRAPER_INCLUDE_VERSION_H_ 1

namespace Skyscraper
{
	class SemVer;
	
	SemVer GetVersion();
};

class Skyscraper::SemVer
{
public :
	unsigned m_major, m_minor, m_patch;
};

# endif /* __SKYSCRAPER_INCLUDE_VERSION_H_ */
