#include "pch.h"
#include "Sfera3DDoc.h"

IMPLEMENT_DYNCREATE(CSfera3DDoc, CDocument)

CSfera3DDoc::CSfera3DDoc()
{
    SetTitle(_T("Podgl¹d sfery 3D magnetometru"));
}

CSfera3DDoc::~CSfera3DDoc()
{
}

BOOL CSfera3DDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    return TRUE;
}