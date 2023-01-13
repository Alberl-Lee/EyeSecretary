#pragma once
#include "ColorButton.h"

class CMainDlg;
class CDlgSettings :
    public CDialogImpl<CDlgSettings>
{
public:
    explicit CDlgSettings(CMainDlg *dlgParent);

    enum { IDD = IDD_DIALOG_SETTINGS };

    BEGIN_MSG_MAP_EX(CDlgSettings) 
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MSG_WM_HSCROLL(OnHScroll)
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        NOTIFY_HANDLER_EX(IDC_BUTTON_COLOR, CPN_SELCHANGE, OnSelChange)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    LRESULT OnOK( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );
    LRESULT OnCancel( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );
    LRESULT OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ );
    LRESULT OnSelChange(LPNMHDR pnmh);
    CColorButton m_btnColor;

private:
    CMainDlg *m_dlgParent;
    COLORREF m_rgbOld;
    int m_nAlphaOld;
    CTrackBarCtrl m_trackbar;

public:
    LRESULT OnHScroll(int code, short pos, HWND hwndCtl);
};
