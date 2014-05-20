#ifndef SUKEY_FLAGS_SOURCE
#define SUKEY_FLAGS_SOURCE
#endif

#include <string>
#include <vector>
#include "flags.h"
#include "util.h"

//DEFINE_string(flagfile,"","从文件中加载参数");

namespace FLAGS_NAMESPACE
{
	using std::string;
	using std::vector;

	void FLAGS_DLL_DECL (*gflags_exitfunc)(int) = &exit;

	class CommandLineFlag
	{
	public:
	private:
		friend class FlagRregistry;
	};

	class FlagRegistry
	{
	public:
		CommandLineFlag* SplitArgumentLocked(const char* argument,
                                       string* key, const char** v,
                                       string* error_message);
		bool SetFlagLocked(CommandLineFlag* flag, const char* value,
                     FlagSettingMode set_mode, string* msg);
	
	};

	class CommandLineFlagParser
	{
	public:
		string ProcessFlagFileLocked(const string& flagval,FlagSettingMode set_mode);
		string ProcessOptionsFromStringLocked(const string& contentdata,FlagSettingMode set_mode);
	private:
		FlagRegistry* const registry_;
	};

	static void ParseFlagList(const char* value, vector<string>* flags)
	{
		for(const char *p = value;p&&*p;value=p)
		{
			p = strchr(value,',');
			size_t len;
			if(p)
			{
				len = p-value;
				p++;
			}
			else
			{
				len = strlen(value);
			}
			flags->push_back(string(value,len));
		}
	}

	#define PFATAL(s)  do { perror(s); gflags_exitfunc(1); } while (0)

	static string ReadFileIntoString(const char* filename) {
		const int kBufSize = 8092;
		char buffer[kBufSize];
		string s;
		FILE* fp;
		if ((errno = SafeFOpen(&fp, filename, "r")) != 0) PFATAL(filename);
		size_t n;
		while ( (n=fread(buffer, 1, kBufSize, fp)) > 0 ) {
			if (ferror(fp))  PFATAL(filename);
			s.append(buffer, n);
		}
		fclose(fp);
		return s;
	}

	string CommandLineFlagParser::ProcessFlagFileLocked(const string &flagval, FlagSettingMode set_mode)
	{
		if(flagval.empty())
			return "";
		string msg;
		vector<string> filename_list;
		ParseFlagList(flagval.c_str(),&filename_list);
		for(size_t i=0; i<filename_list.size();++i)
		{
			const char* file = filename_list[i].c_str();
			msg += ProcessOptionsFromStringLocked(ReadFileIntoString(file),set_mode);
		}
		return msg;
	}

	string CommandLineFlagParser::ProcessOptionsFromStringLocked(const string& contentdata, FlagSettingMode set_mode) 
	{
		string retval;
		const char* flagfile_contents = contentdata.c_str();
		bool flags_are_relevant = true;   // set to false when filenames don't match
		bool in_filename_section = false;

		const char* line_end = flagfile_contents;
		for (; line_end; flagfile_contents = line_end + 1) {
			while (*flagfile_contents && isspace(*flagfile_contents))
				++flagfile_contents;
			line_end = strchr(flagfile_contents, '\n');
			size_t len = line_end ? line_end - flagfile_contents
														: strlen(flagfile_contents);
			string line(flagfile_contents, len);

			if (line.empty() || line[0] == '#') 
			{
			} 
			else if (line[0] == '-') 
			{    // flag
				in_filename_section = false;  // instead, it was a flag-line
				if (!flags_are_relevant)      // skip this flag; applies to someone else
					continue;

				const char* name_and_val = line.c_str() + 1;    // skip the leading -
				if (*name_and_val == '-')
					name_and_val++;                               // skip second - too
				string key;
				const char* value;
				string error_message;
			 // CommandLineFlag* flag = registry_->SplitArgumentLocked(name_and_val,
																														//   &key, &value,
																													 //   &error_message);//TODO
				//if (flag == NULL) 
				//{
			 // } 
				//else if (value == NULL) 
				//{
			 // } 
				//else 
				//{
				//	int a=0;
					//retval += ProcessSingleOptionLocked(flag, value, set_mode);TODO
				//}

			} else {                        // a filename!
				if (!in_filename_section) {   // start over: assume filenames don't match
					in_filename_section = true;
					flags_are_relevant = false;
				}

				// Split the line up at spaces into glob-patterns
				const char* space = line.c_str();   // just has to be non-NULL
				for (const char* word = line.c_str(); *space; word = space+1) {
					if (flags_are_relevant)     // we can stop as soon as we match
						break;
					space = strchr(word, ' ');
					if (space == NULL)
						space = word + strlen(word);
					const string glob(word, space - word);
					// We try matching both against the full argv0 and basename(argv0)
					if (glob == ProgramInvocationName()       // small optimization
							|| glob == ProgramInvocationShortName()
	#if defined(HAVE_FNMATCH_H)
							|| fnmatch(glob.c_str(), ProgramInvocationName(),      FNM_PATHNAME) == 0
							|| fnmatch(glob.c_str(), ProgramInvocationShortName(), FNM_PATHNAME) == 0
	#elif defined(HAVE_SHLWAPI_H)
							|| PathMatchSpec(glob.c_str(), ProgramInvocationName())
							|| PathMatchSpec(glob.c_str(), ProgramInvocationShortName())
	#endif
							) {
						flags_are_relevant = true;
					}
				}
			}
		}
		return retval;
}


	static const char* argv0 = "UNKNOWN"; 
	const char* GetArgv0()           { return argv0; }
	const char* ProgramInvocationName() 
	{             // like the GNU libc fn
		return GetArgv0();
	}
	const char* ProgramInvocationShortName() {        // like the GNU libc fn
		const char* slash = strrchr(argv0, '/');
	#ifdef OS_WINDOWS
		if (!slash)  slash = strrchr(argv0, '\\');
	#endif
		return slash ? slash + 1 : argv0;
}
}