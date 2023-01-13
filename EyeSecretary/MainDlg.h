// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include "trayiconimpl.h"

class CMainDlg : public CDialogImpl<CMainDlg>
    ,public CTrayIconImpl<CMainDlg>
{
public:
    CMainDlg();

	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP_EX(CMainDlg) // 添加事件处理时需把BEGIN_MSG_MAP_EX改成BEGIN_MSG_MAP，不然添加不了，添加完了再改回去
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_TIMER(OnTimer)
        MSG_WM_DESTROY(OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER(ID_TRAY_CLOSE, OnTrayClose)
        COMMAND_ID_HANDLER(ID_TRAY_OPTION, OnTrayOption)
        CHAIN_MSG_MAP(CTrayIconImpl<CMainDlg>)
	END_MSG_MAP()

    void SetColor(COLORREF rgb);
    void SetAlpha(int nAlpha);

    COLORREF GetColor();
    int GetAlpha();

private:
    void InitExeName();
    BOOL IsFilterExe(LPCTSTR pszPath);
    BOOL IsFilterExe(HWND hWnd);
    BOOL GetExcludeRect(HWND hWnd, CRect &rc);
    void WriteConfig();
    void ReadConfig();
    void GetPathIni(CString &strPath);

    CRect m_rcExclude;
    std::vector<CString> m_vctExeName;
    COLORREF    m_rgb;
    int         m_nAlpha;

public:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnPaint(HDC hdc);
    void OnTimer(UINT_PTR nIDEvent);
    LRESULT OnDestroy(void);
    LRESULT OnTrayClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnTrayOption(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
