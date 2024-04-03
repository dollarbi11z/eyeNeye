/*
						███████╗██╗   ██╗███████╗███╗   ██╗███████╗██╗   ██╗███████╗
						██╔════╝╚██╗ ██╔╝██╔════╝████╗  ██║██╔════╝╚██╗ ██╔╝██╔════╝
						█████╗   ╚████╔╝ █████╗  ██╔██╗ ██║█████╗   ╚████╔╝ █████╗  
						██╔══╝    ╚██╔╝  ██╔══╝  ██║╚██╗██║██╔══╝    ╚██╔╝  ██╔══╝  
						███████╗   ██║   ███████╗██║ ╚████║███████╗   ██║   ███████╗
						╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═══╝╚══════╝   ╚═╝   ╚══════╝


						**IMPORTANT** 
						
						Ensure you call: 

						eyeNeye::Free();

						Before program termination otherwise the file you have been modifying
						won't be cleaned up.
															
*/

#pragma once
#include <unordered_map>
#include <memory>
#include <fstream>
#include <string>

/* [SECTION]
*  var1=value
*  var2=value
*  var3=value
*
*  [SECTION2]
*  etc...
*/

struct INISECTION
{
	std::string sectionName;

	/* NAME, VALUE*/
	std::unordered_map <std::string, std::string> mapVariables;
};

/* These will be formatted like [User Settings] */
struct INIFILE
{
	std::unique_ptr<INISECTION> section = std::make_unique<INISECTION>();

	/* KEY, SECTION */
	std::unordered_map<std::string, INISECTION> mapSections;
};

class eyeNeye
{
private:
	static INIFILE*	_iniFile;

	static inline void Log_Error( const std::string& message );
	static inline void Log_Info( const std::string& message );

	static std::string FormatSectionKey( const std::string& line );
	static std::string FormatVariableId( const std::string& line );
	static std::string FormatVariableValue( const std::string& line );

public:
	eyeNeye();
	~eyeNeye();

	static INIFILE* ReadIniFile( const std::string& file);
	static const std::string& GetVariableValue( const std::string& section, const std::string& var );

	static bool WriteIniFile( const std::string& file );
	static bool EditVariableValue( const std::string& section, const std::string& variable, const std::string& newVal );

	static void Free();
};