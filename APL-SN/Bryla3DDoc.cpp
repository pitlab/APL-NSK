#include "pch.h"
#include "Bryla3DDoc.h"

IMPLEMENT_DYNCREATE(CBryla3DDoc, CDocument)

CBryla3DDoc::CBryla3DDoc() 
{
}

CBryla3DDoc::~CBryla3DDoc()
{
}

BOOL CBryla3DDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    return TRUE;
}