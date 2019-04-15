#ifndef __PROPERTY_WINDOW_H
#define __PROPERTY_WINDOW_H

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <common/array.h>
#include <common/assotiativearray.h>
#include <common/string.h>
#include <common/aastring.h>

enum X_PROP_EDITOR_TYPE
{
	XPET_TEXT,
	XPET_FILE,
	XPET_COMBO,

	XPET__LAST
};

struct X_PROP_FIELD
{
	const char *szKey;
	const char *szName;
	X_PROP_EDITOR_TYPE editorType;
	const void *pEditorData;
	const char *szHelp;
};

class CPropertyWindow
{
protected:
	INT_PTR CALLBACK dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK PropDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK EditorDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK ClassListWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	void onSelChanged();
	void onPropListChanged();
	void filterClassList(const char *szFilter);
	void initEditor(X_PROP_EDITOR_TYPE type, const void *pData, const char *szValue);
public:
	CPropertyWindow(HINSTANCE hInstance, HWND hMainWnd);
	~CPropertyWindow();

	void show();
	void hide();
	bool isVisible();

	void clearClassList();
	void addClass(const char *szClassName);

	void clearProps();
	void addPropField(const X_PROP_FIELD *pField, const char *szValue = NULL);
	void setPropFieldValue(const char *szKey, const char *szValue);

	void setClassName(const char *szClassName);
	void allowClassChange(bool bAllow);

	class ICallback
	{
	public:
		virtual void onClassChanged(const char *szNewClassName) = 0;
		virtual bool onPropertyChanged(const char *szKey, const char *szValue) = 0;
	};

	void setCallback(ICallback *pCallback);

protected:

	HINSTANCE m_hInstance;
	HWND m_hDlgWnd; // tab control handle
	HWND m_hTabControl; // tab control handle
	HWND m_hCurrentTab = NULL; // tab dialog handle
	Array<HWND> m_hPropTabs;

	HWND m_hClassListWnd = NULL;
	HWND m_hPropListWnd = NULL;

	WNDPROC m_pfnClassListOldWndproc;

	Array<String> m_asClassList;

	HWND m_phEditors[XPET__LAST];

	struct prop_s
	{
		X_PROP_FIELD field;
		String sValue;
	};

	struct edt_kv
	{
		const char *szKey;
		const char *szValue;
	};

	AssotiativeArray<AAString, prop_s> m_aPropFields;
	X_PROP_EDITOR_TYPE m_editorActive = XPET__LAST;

	ICallback *m_pCallback = NULL;
};

#endif
