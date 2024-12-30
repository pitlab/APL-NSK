#pragma once

class Config
{
public:
	Config();

	template< typename ReturnType >
	ReturnType get( const char* _path ) const
	{
		return m_propTree.get< ReturnType >( _path );
	}

	template<>
	std::wstring get< std::wstring >( const char* _path ) const;

	template<>
	CString get< CString >( const char * _path ) const;

	template< typename ReturnType >
	ReturnType get( const char* _path, const ReturnType & _default ) const
	{
		return m_propTree.get< ReturnType >( _path, _default );
	}

	template<>
	std::wstring get< std::wstring >( const char* _path, const std::wstring & _default ) const;

	template<>
	CString get< CString >( const char* _path, const CString & _default ) const;

	template< typename DataType >
	void put( const char * _path, const DataType & _data )
	{
		m_propTree.put< DataType >( _path, _data );
		assert( m_propTree.get< DataType >( _path ) == _data );
	}

	template<>
	void put< std::wstring >( const char * _path, const std::wstring & _data );

	void saveToRegistry();
	void loadFromRegistry();

	void saveToXML( const string_type & _path );
	void loadFromXML( const string_type & _path );

private:

	boost::property_tree::ptree m_propTree;
};

Config& getConfig();