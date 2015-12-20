#include <fstream>
#include <string>
#include "DirectXMath.h" 

namespace Indecisive
{
	inline std::istream& operator>> (std::istream& s, Vector3& v)
	{
		return s >> v.x >> v.y >> v.z;
	};

	class IReadable
	{
	protected:
		std::string m_ErrorText;
		virtual void _Open(const std::string&) = 0;
		virtual void _Close() = 0;
		virtual void InternalRead(const std::string&) = 0;
	public:
		virtual bool CanRead(const std::string&) const = 0;
		virtual const std::string GetInfo() const = 0;
		void Read(const std::string& file)
		{
			try
			{
				InternalRead(file);
			}
			catch (const std::exception& ex)
			{
				// Extract error description
				m_ErrorText = ex.what();
				//DefaultLogger::get()->error(m_ErrorText);
				return;
			}
		};
		const std::string& GetErrorText() const { return m_ErrorText; };
	public: // static utilities
		static bool ExtensionCheck(const std::string& file, const char* ext0, const char* ext1 = NULL, const char* ext2 = NULL)
		{
			std::string::size_type pos = file.find_last_of('.');

			// No file extension - can't read
			if (pos == std::string::npos)
				return false;

			const char* ext_real = &file[pos + 1];
			if (!_stricmp(ext_real, ext0))
				return true;

			// Check for other, optional, file extensions
			if (ext1 && !_stricmp(ext_real, ext1))
				return true;

			if (ext2 && !_stricmp(ext_real, ext2))
				return true;

			return false;
		};
		/*static const std::string& GetExtension(const std::string&)
		{
		std::string::size_type pos = pFile.find_last_of('.');

		// no file extension at all
		if( pos == std::string::npos)
		return "";

		std::string ret = pFile.substr(pos+1);
		std::transform(ret.begin(),ret.end(),ret.begin(),::tolower); // thanks to Andy Maloney for the hint
		return ret;
		};
		*/
	};
};