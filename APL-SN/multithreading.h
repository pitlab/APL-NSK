#pragma once

class SectionLocker;
class TrySectionLocker;

class CriticalSection
{
public:
	CriticalSection()
	{
		::InitializeCriticalSection( &m_section );
	}

	~CriticalSection()
	{
		::DeleteCriticalSection( &m_section );
	}

private:
	CriticalSection( const CriticalSection & );
	CriticalSection& operator=( const CriticalSection & );

	CRITICAL_SECTION m_section;

	friend class SectionLocker;
	friend class TrySectionLocker;
};

class SectionLocker
{
public:
	SectionLocker( CriticalSection & _section ) : m_section( _section )
	{
		::EnterCriticalSection( &m_section.m_section );
	}

	~SectionLocker()
	{
		::LeaveCriticalSection( &m_section.m_section );
	}

private:
	CriticalSection& m_section;
};

class TrySectionLocker
{
public:
	TrySectionLocker( CriticalSection & _section ) : m_section( _section ),
		m_isLocked( ::TryEnterCriticalSection( &m_section.m_section ) )
	{
	}

	void unlock()
	{
		if( m_isLocked )
		{
			m_isLocked = false;
			::LeaveCriticalSection( &m_section.m_section );
		}
	}

	~TrySectionLocker()
	{
		if( m_isLocked )
			::LeaveCriticalSection( &m_section.m_section );
	}

	bool isLocked() const
	{
		return m_isLocked ? true : false;
	}

private:

	CriticalSection& m_section;
	BOOL			m_isLocked;
};