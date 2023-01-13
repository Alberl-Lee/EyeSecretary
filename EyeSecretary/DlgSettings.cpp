#include "StdAfx.h"
#include "DlgSettings.h"
#include "MainDlg.h"

CDlgSettings::CDlgSettings( CMainDlg *dlgParent )
: m_dlgParent(dlgParent),
m_rgbOld(m_dlgParent->GetColor()),
m_nAlphaOld(m_dlgParent->GetAlpha())
{
}

LRESULT CDlgSettings::OnOK( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
    // TODO: Add validation code 
    EndDialog(wID);
    return 0;
}

LRESULT CDlgSettings::OnCancel( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
    m_dlgParent->SetColor(m_rgbOld);
    m_dlgParent->SetAlpha(m_nAlphaOld);
    EndDialog(wID);
    return 0;
}

LRESULT CDlgSettings::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
    if (! m_dlgParent)
    {
        return TRUE;
    }

    CenterWindow();

    CButton btnAutoID  = GetDlgItem(IDC_CHECK_AUTO_ID_WND);
    CButton btnTopmost = GetDlgItem(IDC_CHECK_TOPMOST);
    m_trackbar = GetDlgItem(IDC_SLIDER_ALPHA);

    btnAutoID.SetCheck(TRUE);
    btnTopmost.SetCheck(TRUE);
    m_btnColor.SubclassWindow(GetDlgItem(IDC_BUTTON_COLOR));
    m_btnColor.SetTrackSelection(TRUE);
    m_btnColor.SetDefaultColor(m_dlgParent->GetColor());
    m_trackbar.SetRange(0, 255, FALSE);
    m_trackbar.SetPos(m_dlgParent->GetAlpha());

    CString str;
    str.Format(_T("%d"), m_dlgParent->GetAlpha());
    SetDlgItemText(IDC_STATIC_ALPHA, str);
    return TRUE;
}

LRESULT CDlgSettings::OnSelChange( LPNMHDR pnmh )
{
    NMCOLORBUTTON *pNm = (NMCOLORBUTTON *)pnmh;
    m_dlgParent->SetColor(pNm->clr);
    return TRUE;
}

LRESULT CDlgSettings::OnHScroll(int code, short pos, HWND hwndCtl)
{
    switch(::GetDlgCtrlID(hwndCtl))
    {
    case IDC_SLIDER_ALPHA:
        {
            CString str;

            m_dlgParent->SetAlpha(m_trackbar.GetPos());
            str.Format(_T("%d"), m_trackbar.GetPos());
            SetDlgItemText(IDC_STATIC_ALPHA, str);
        }
        break;
    }
    return 0;
}
