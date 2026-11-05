#pragma once
#include <afxwin.h>
#include "sys_def_wspolny.h"

class CBryla3DDoc :    public CDocument
{
    DECLARE_DYNCREATE(CBryla3DDoc)

public:
    CBryla3DDoc();
    virtual ~CBryla3DDoc();

    virtual BOOL OnNewDocument();
};

