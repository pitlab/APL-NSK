#pragma once
#include <afxwin.h>
#include "sys_def_wspolny.h"

class CSfera3DDoc :    public CDocument
{
    DECLARE_DYNCREATE(CSfera3DDoc)

public:
    CSfera3DDoc();
    virtual ~CSfera3DDoc();

    virtual BOOL OnNewDocument();
};

