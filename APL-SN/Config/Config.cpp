#include "StdAfx.h"
#include "Config.h"
#include "registry_parser.hpp"
#include <boost/property_tree/xml_parser.hpp>
#include "helpers/unicode.h"

Config config;

Config::Config()
{
}

template<>
std::wstring Config::get< std::wstring >( const char* _path ) const
{
	return toWstring( get< std::string >( _path ) );
}

template<>
CString Config::get< CString >( const char * _path ) const
{
	return CString( toWstring( get< std::string >( _path ) ).c_str() );
}

template<>
std::wstring Config::get< std::wstring >( const char* _path, const std::wstring & _default ) const
{
	return toWstring( get< std::string >( _path, toString( _default ) ) );
}

template<>
CString Config::get< CString >( const char* _path, const CString & _default ) const
{
	return CString( toWstring( get< std::string >( _path , toString( _default ) ) ).c_str() );
}

template<>
void Config::put< std::wstring >( const char * _path, const std::wstring & _data )
{
	put< std::string >( _path, toString( _data ) );
}

void Config::saveToRegistry()
{
	boost::property_tree::write_registry( HKEY_CURRENT_USER, "SOFTWARE\\Pitlab\\APL-GS\\Settings", m_propTree );
}

void Config::loadFromRegistry()
{
	boost::property_tree::read_registry( HKEY_CURRENT_USER, "SOFTWARE\\Pitlab\\APL-GS\\Settings", m_propTree );
}

void Config::saveToXML( const string_type & _path )
{
	std::ofstream outStream( toString( _path ).c_str() );
	boost::property_tree::write_xml( outStream, m_propTree );
}

void Config::loadFromXML( const string_type & _path )
{
	std::ifstream inStream( toString( _path ).c_str() );
	boost::property_tree::read_xml( inStream, m_propTree );
}
