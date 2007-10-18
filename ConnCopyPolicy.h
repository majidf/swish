// ConnCopyPolicy.h: Copy-policy class for converting from connection
// data structure to PIDL
//
//////////////////////////////////////////////////////////////////////

#ifndef CONNCOPYPOLICY_H
#define CONNCOPYPOLICY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HostPidlManager.h"

class CConnCopyPolicy  
{
public:
	static void init( LPITEMIDLIST* ) { /* No init needed */ }
    
    static HRESULT copy( LPITEMIDLIST* pTo, const HOSTPIDL *pFrom )
    {
		ATLASSERT(SUCCEEDED(m_PidlManager.IsValid((LPITEMIDLIST)pFrom)));
		return m_PidlManager.Create( pFrom->wszLabel, pFrom->wszUser, 
			pFrom->wszHost, pFrom->wszPath, pFrom->uPort, pTo );
    }

    static void destroy( LPITEMIDLIST* p ) 
    {
        m_PidlManager.Delete( *p ); 
    }

private:
    static CHostPidlManager m_PidlManager;
};

typedef CComEnumOnSTL<IEnumIDList, &IID_IEnumIDList, LPITEMIDLIST,
                      CConnCopyPolicy, std::vector<HOSTPIDL> >
		CEnumIDListImpl;

#endif // CONNCOPYPOLICY_H