#include "pch.h"
#include "MapaWysokosciowaDoc.h"

IMPLEMENT_DYNCREATE(CMapaWysokosciowaDoc, CDocument)

CMapaWysokosciowaDoc::CMapaWysokosciowaDoc()
{
}

CMapaWysokosciowaDoc::~CMapaWysokosciowaDoc()
{
}

BOOL CMapaWysokosciowaDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    return TRUE;
}
