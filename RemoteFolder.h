// RemoteFolder.h : Declaration of the CRemoteFolder

#ifndef REMOTEFOLDER_H
#define REMOTEFOLDER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "resource.h"       // main symbols

#define INITGUID
#include <propkey.h>

#include "RemotePidlManager.h"
#include "HostFolder.h"

#define _ATL_DEBUG_QI

// IRemoteFolder
[
	object,
	uuid("b816a83b-5022-11dc-9153-0090f5284f85"),
	helpstring("IRemoteFolder Interface"),
	pointer_default(unique)
]
__interface IRemoteFolder : IUnknown
{
};


// CRemoteFolder
[
	coclass,
	default(IRemoteFolder),
	threading(apartment),
	vi_progid("Swish.RemoteFolder"),
	progid("Swish.RemoteFolder.1"),
	version(1.0),
	registration_script("RemoteFolder.rgs"),
	uuid("b816a83c-5022-11dc-9153-0090f5284f85"),
	helpstring("RemoteFolder Class")
]
class ATL_NO_VTABLE CRemoteFolder :
	public IRemoteFolder,
	// The IShellFolder2-specific detail-handling methods are not compatible
	// with Win 9x/NT but it supports all those of IShellDetails which are
	public IShellFolder2, 
	// IPersistFolder2 needed for Details expando
    public IPersistFolder2, 
	public IExtractIcon
//	public IShellDetails // This is compatible with 9x/NT unlike IShellFolder2
{
public:
	CRemoteFolder() : m_pidl(NULL), m_pidlRoot(NULL)
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	void FinalRelease()
	{
	}

	// Init function - call right after constructing a CRemoteFolder object
    HRESULT _init( CHostFolder* pParentFolder, LPCITEMIDLIST pidl )
    {
        m_pHostFolder = pParentFolder;

		if (m_pHostFolder != NULL)
            m_pHostFolder->AddRef();

        m_pidl = m_PidlManager.Copy ( pidl );

        return S_OK;
    }

	// Init function - call right after constructing a CRemoteFolder object
    HRESULT _init( CRemoteFolder* pParentFolder, LPCITEMIDLIST pidl )
    {
        m_pParentFolder = pParentFolder;

		if (m_pParentFolder != NULL)
            m_pParentFolder->AddRef();

        m_pidl = m_PidlManager.Copy ( pidl );

        return S_OK;
    }

    // IPersist
    STDMETHOD(GetClassID)( CLSID* );

	// IPersistFolder
    STDMETHOD(Initialize)( LPCITEMIDLIST pidl );

	// IPersistFolder2
	STDMETHOD(GetCurFolder)( LPITEMIDLIST *ppidl );

	// IShellFolder
    STDMETHOD(BindToObject)( PCUIDLIST_RELATIVE pidl, LPBC, REFIID, void** );
	STDMETHOD(EnumObjects)( HWND, DWORD, LPENUMIDLIST* );
    STDMETHOD(CreateViewObject)( HWND, REFIID, void** );
    STDMETHOD(GetAttributesOf) ( UINT, LPCITEMIDLIST*, LPDWORD );
    STDMETHOD(GetUIObjectOf)
		( HWND, UINT, LPCITEMIDLIST*, REFIID, LPUINT, void** );
	STDMETHOD(CompareIDs)
		( LPARAM, LPCITEMIDLIST, LPCITEMIDLIST );
    STDMETHOD(BindToStorage)( LPCITEMIDLIST, LPBC, REFIID, void** )
        { return E_NOTIMPL; }
    STDMETHOD(GetDisplayNameOf)( PCUITEMID_CHILD, SHGDNF, LPSTRRET );
    STDMETHOD(ParseDisplayName)
		( HWND, LPBC, LPOLESTR, LPDWORD, LPITEMIDLIST*, LPDWORD )
        { return E_NOTIMPL; }
    STDMETHOD(SetNameOf)( HWND, LPCITEMIDLIST, LPCOLESTR, DWORD, LPITEMIDLIST* )
        { return E_NOTIMPL; }

	// IShellFolder2
	STDMETHOD(EnumSearches)( IEnumExtraSearch **ppEnum );
	STDMETHOD(GetDefaultColumn)( DWORD, ULONG *pSort, ULONG *pDisplay );
	STDMETHOD(GetDefaultColumnState)( UINT iColumn, SHCOLSTATEF *pcsFlags );		STDMETHOD(GetDefaultSearchGUID)( GUID *pguid )
		{ return E_NOTIMPL; }
	STDMETHOD(GetDetailsEx)( PCUITEMID_CHILD pidl, const SHCOLUMNID *pscid, 
							 VARIANT *pv );
	STDMETHOD(MapColumnToSCID)( UINT iColumn, SHCOLUMNID *pscid );

	// IExtractIcon
	STDMETHOD(Extract)( LPCTSTR pszFile, UINT nIconIndex, HICON *phiconLarge, 
						HICON *phiconSmall, UINT nIconSize );
	STDMETHOD(GetIconLocation)( UINT uFlags, LPTSTR szIconFile, UINT cchMax, 
								int *piIndex, UINT *pwFlags );

	// IShellDetails
	STDMETHOD(GetDetailsOf)( PCUITEMID_CHILD pidl, UINT iColumn, 
							 LPSHELLDETAILS pDetails );
	STDMETHOD(ColumnClick)( UINT iColumn );

private:
    CRemotePidlManager m_PidlManager;
	PIDLIST_ABSOLUTE   m_pidlRoot; // Absolute pidl of this folder object
    CHostFolder*       m_pHostFolder;
	CRemoteFolder*     m_pParentFolder;
    PIDLIST_RELATIVE   m_pidl;

	CString _GetLongNameFromPIDL( PCUITEMID_CHILD pidl, BOOL fCanonical );
	CString _GetLabelFromPIDL( PCUITEMID_CHILD pidl );
	HRESULT _FillDetailsVariant( PCWSTR szDetail, VARIANT *pv );
	HRESULT _FillDateVariant( CTime dtDate, VARIANT *pv );
	HRESULT _FillUI8Variant( ULONGLONG ull, VARIANT *pv );
};

// Remote folder listing column property IDs
enum PID_SWISH_REMOTE {
	PID_SWISH_REMOTE_GROUP = PID_FIRST_USABLE,
	PID_SWISH_REMOTE_PERMISSIONS
};

// PKEYs for custom swish details/properties
// Swish remote folder FMTID GUID {b816a851-5022-11dc-9153-0090f5284f85}
DEFINE_PROPERTYKEY(PKEY_SwishRemoteGroup, 0xb816a851, 0x5022, 0x11dc, \
				   0x91, 0x53, 0x00, 0x90, 0xf5, 0x28, 0x4f, 0x85, \
				   PID_SWISH_REMOTE_GROUP);
DEFINE_PROPERTYKEY(PKEY_SwishRemotePermissions, 0xb816a851, 0x5022, 0x11dc, \
				   0x91, 0x53, 0x00, 0x90, 0xf5, 0x28, 0x4f, 0x85, \
				   PID_SWISH_REMOTE_PERMISSIONS);

#endif // REMOTEFOLDER_H
