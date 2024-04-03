#include "eyeNeye.h"
#include <iostream>
#include <sstream>
#include <format>

INIFILE* eyeNeye::_iniFile;

eyeNeye::eyeNeye()
{ 
	_iniFile = nullptr;
}

eyeNeye::~eyeNeye()
{ }

inline void eyeNeye::Log_Error( const std::string& message )
{
	std::cerr << "[eyeNeye] [ERROR] " << message << std::endl;
}

inline void eyeNeye::Log_Info( const std::string& message )
{
	std::cout << "[eyeNeye] [INFO] " << message << std::endl;
}

std::string eyeNeye::FormatSectionKey( const std::string& line )
{
	uint32_t start = line.find( '[' );
	uint32_t end = line.find( ']' );

	std::string sub = line.substr( start + 1, end - 1 );
	std::string ret;

	for( uint32_t i = 0; i < strlen( sub.c_str() ); i++ )
	{
		ret.insert( ret.begin() + i, tolower( sub[i] ) );
	}

	return ret;
}

std::string eyeNeye::FormatVariableId( const std::string& line )
{
	uint32_t nameEndPos = line.find( '=' );

	if( nameEndPos != std::string::npos )
		return line.substr( 0, nameEndPos - 1 );

	return std::string();
}

std::string eyeNeye::FormatVariableValue( const std::string& line )
{
	uint32_t nameEndPos = line.find( '=' );

	if( nameEndPos != std::string::npos )
		return line.substr( nameEndPos + 1 );

	return std::string();
}

INIFILE* eyeNeye::ReadIniFile( const std::string& file )
{ 
	std::ifstream configFile( file );

	std::string sectionKey;
	std::string line;

	std::string currentSection;

	_iniFile = new INIFILE();

	if( !configFile.is_open() )
	{
		Log_Error( "Failed to open file: " + std::string( file ) );
		return nullptr;
	}

	while( std::getline(configFile, line ) )
	{
		/* Ignore the line if comments */
		if( line.front() == ';' )
			continue;

		if( line.front() == '[' )
		{
			currentSection = FormatSectionKey( line );
			_iniFile->mapSections.emplace( currentSection, line );
		}
		else if( !currentSection.empty() )
		{
			size_t equalsPos = line.find( '=' );
			if( equalsPos != std::string::npos )
			{
				std::string key = FormatVariableId(line.substr( 0, equalsPos ));
				std::string value = FormatVariableValue(line.substr( equalsPos + 1 ));

				_iniFile->mapSections.at( currentSection ).mapVariables.emplace( key, value );
			}
		}
	}

	Log_Info( "Successfully read ini file." );

	configFile.close();
	return _iniFile;
}

const std::string& eyeNeye::GetVariableValue( const std::string& section, const std::string& var )
{
	if( _iniFile->mapSections.count( section ) != 0 )
	{
		if( _iniFile->mapSections.at( section ).mapVariables.count( var ) != 0 )
		{
			Log_Info( std::format( "Retrieved Variable: {}, value: {}", var, section ));
			return _iniFile->mapSections.at( section ).mapVariables.at( var );
		}
		else
		{
			Log_Error( "Unable to find variable: " + var );
			return std::string();
		}
	}
	
	Log_Error( "Unable to find section: " + section );
	return std::string();
}

bool eyeNeye::EditVariableValue( const std::string& section, const std::string& variable, const std::string& newVal )
{
	if( _iniFile->mapSections.count( section ) != 0 )
	{
		uint32_t nameEndPos = variable.find( '=' );
		_iniFile->mapSections.at( section ).mapVariables.at( variable ) = newVal;

		Log_Info( std::format( "Updated Variable Value: {} to {}", variable, newVal ) );
		return true;
	}

	Log_Error( "Failed to find section/variable." );
	return false;
}

bool eyeNeye::WriteIniFile( const std::string& file )
{
	std::stringstream stream( std::ios::out );
	std::ofstream writer;

	writer.open( file );

	if( !writer.good() )
	{
		Log_Error( "Failed to open file for writing." );
		return false;
	}

	for( const auto& [key, section] : _iniFile->mapSections )
	{
		stream << section.sectionName << '\n';

		for( const auto& [varID, var] : section.mapVariables )
		{
			stream << std::format( "{}={}", varID, var ) << '\n';
		}
	}
	writer << stream.str();
	return true;
}

void eyeNeye::Free()
{ 
	if( _iniFile != nullptr )
	{
		delete _iniFile;
		_iniFile = nullptr;
	}
}
