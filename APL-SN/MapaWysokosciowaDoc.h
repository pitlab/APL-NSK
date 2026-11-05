#pragma once
#include <afxwin.h>

class CMapaWysokosciowaDoc :     public CDocument
{
    DECLARE_DYNCREATE(CMapaWysokosciowaDoc)

public:
    CMapaWysokosciowaDoc();
    virtual ~CMapaWysokosciowaDoc();

    virtual BOOL OnNewDocument();


};

