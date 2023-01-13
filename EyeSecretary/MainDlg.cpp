#include "stdafx.h"
#include "MainDlg.h"
#include "DlgSettings.h"
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

const UINT ID_TIMER_TOPMOST     = 1;
const UINT INT_ALPHA_DEFAULT    = 24;
const COLORREF RGB_DEFAULT      = RGB(0, 255, 0);
const COLORREF RGB_KEY          = RGB(255, 0, 255);
const TCHAR STR_PATH_CONFIG[]   = _T("EyeSecretary.ini");


CMainDlg::CMainDlg() :
m_rgb(RGB_DEFAULT),
m_nAlpha(INT_ALPHA_DEFAULT)
{
    ReadConfig();
}

LRESULT CMainDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
    // center the dialog on the screen
    CenterWindow();

    // set icons
    HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
    ::SetWindowLong (m_hWnd, GWL_EXSTYLE, dwExStyle ^ WS_EX_LAYERED ^ WS_EX_TRANSPARENT ^ WS_EX_TOOLWINDOW);
    ::SetLayeredWindowAttributes(m_hWnd, RGB_KEY, m_nAlpha, LWA_COLORKEY | LWA_ALPHA);

    MONITORINFO mon = {};
    mon.cbSize = sizeof(mon);
    ::GetMonitorInfo(::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST), &mon);
    CRect rcWork = mon.rcWork;
    //::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN), SWP_SHOWWINDOW);
    ::SetWindowPos(m_hWnd, HWND_TOPMOST, rcWork.left, rcWork.top, rcWork.Width(), rcWork.Height(), SWP_SHOWWINDOW);

    InitExeName();
    InstallIcon(hIconSmall, IDR_MENU_TRAY, _T("WTLTrayIcon"), _T("小秘在这里"), _T("护眼秘书"));
    SetTimer(ID_TIMER_TOPMOST, 10); // 不要用OnIdle，因为OnIdle每几个ms就执行一次，更浪费，没必要
    return TRUE;
}

LRESULT CMainDlg::OnOK( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
    // TODO: Add validation code 
    EndDialog(wID);
    return 0;
}

LRESULT CMainDlg::OnCancel( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
    EndDialog(wID);
    return 0;
}

LRESULT CMainDlg::OnDestroy()
{
    //You should call SetMsgHandled(FALSE) or set bHandled = FALSE for the main window of your application
    KillTimer(ID_TIMER_TOPMOST);
    return 0;
}

LRESULT CMainDlg::OnPaint(HDC hdc)
{
    CPaintDC dc(m_hWnd);
    CRect  rc;
    CRgn   rgn, rgnExclude;
    CBrush br, brKey;

    br.CreateSolidBrush(m_rgb);
    brKey.CreateSolidBrush(RGB_KEY);
    GetClientRect(&rc);
    rgnExclude.CreateRectRgnIndirect(&m_rcExclude);
    rgn.CreateRectRgnIndirect(&rc);
    rgn.CombineRgn(HRGN(rgnExclude), RGN_DIFF);

    dc.SaveDC();
    dc.FillRgn(HRGN(rgn), HBRUSH(br));
    dc.FillRgn(HRGN(rgnExclude), HBRUSH(brKey));
    dc.RestoreDC(-1);

    return 0;
}

void Log(HWND hWnd)
{
    CString str;
    TCHAR szTitle[_MAX_PATH];
    TCHAR szClass[_MAX_PATH];

    ::GetWindowText(hWnd, szTitle, _MAX_PATH);
    ::GetClassName(hWnd, szClass, _MAX_PATH);
    str.Format(_T("wtl: %s,%s\n"), szTitle, szClass);
    OutputDebugString(str);
}

void Log(LPCTSTR psz)
{
    CString str;
    str.Format(_T("wtl: %s\n"), psz);
    OutputDebugString(str);
}

BOOL IsTopMost(HWND hWnd)
{
    if (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST)
    {
        return TRUE;
    }

    return FALSE;
}

void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch(nIDEvent)
    {
    case ID_TIMER_TOPMOST:
        {
            CRect rcOld = m_rcExclude;

            GetExcludeRect(::GetForegroundWindow(), m_rcExclude);

            if (rcOld != m_rcExclude)
            {
                Invalidate();
            }

            ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

            // 不应该调用SetForegroundWindow，因为其他窗口右键弹出菜单时，会使其菜单消失
            //HWND hWnd = ::GetForegroundWindow();

            //if ((hWnd != m_hWnd) && IsTopMost(hWnd))
            //{
            //    ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            //    ::SetForegroundWindow(m_hWnd);
            //    Log(hWnd);
            //    Log(m_hWnd);
            //}
        }
        break;
    default:
        break;
    }

}

void CMainDlg::InitExeName()
{
    m_vctExeName.push_back(_T("notepad.exe"));

    m_vctExeName.push_back(_T("wps.exe"));
    m_vctExeName.push_back(_T("wpp.exe"));
    m_vctExeName.push_back(_T("et.exe"));

    m_vctExeName.push_back(_T("EXCEL.EXE"));
    m_vctExeName.push_back(_T("WINWORD.EXE"));
    m_vctExeName.push_back(_T("POWERPNT.EXE"));
    m_vctExeName.push_back(_T("OUTLOOK.EXE"));
    m_vctExeName.push_back(_T("ONENOTE.EXE"));
    m_vctExeName.push_back(_T("VISIO.EXE"));
    m_vctExeName.push_back(_T("GROOVE.EXE"));

    m_vctExeName.push_back(_T("MSDEV.EXE"));
    m_vctExeName.push_back(_T("devenv.exe"));
}

BOOL CMainDlg::IsFilterExe( LPCTSTR pszPath )
{
    CString str;

    for (std::vector<CString>::const_iterator it = m_vctExeName.begin();
        it != m_vctExeName.end();
        ++it)
    {
        str = pszPath;
        str = str.Right( (*it).GetLength() );

        if (! (*it).Compare(str))
        {
            return TRUE;
        }    
    }

    return FALSE;
}

BOOL CMainDlg::IsFilterExe( HWND hWnd )
{
    BOOL  bRet = FALSE;
    TCHAR szPath[_MAX_PATH];
    DWORD dwPID;
    ::GetWindowThreadProcessId(hWnd, &dwPID);
    HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPID);

    //if (::GetModuleFileNameEx(hProcess, NULL, szPath, _MAX_PATH))
    if (::GetProcessImageFileName(hProcess, szPath, _MAX_PATH))
    {
        bRet = IsFilterExe(szPath);
    }

    ::CloseHandle(hProcess);
    return bRet;
}

BOOL CMainDlg::GetExcludeRect( HWND hWnd, CRect &rc )
{
    if (IsFilterExe(hWnd))
    {
        ::GetWindowRect(hWnd, &rc);
        return TRUE;
    }  

    rc.SetRectEmpty();
    return FALSE;
}

LRESULT CMainDlg::OnTrayClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: 在此添加命令处理程序代码
    EndDialog(IDOK);
    return 0;
}

LRESULT CMainDlg::OnTrayOption(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: 在此添加命令处理程序代码
    CDlgSettings dlg(this);

    if (IDOK == dlg.DoModal())
    {
        WriteConfig();
    }

    return 0;
}

void CMainDlg::SetColor( COLORREF rgb )
{
    m_rgb = rgb;
    Invalidate();
}

void CMainDlg::SetAlpha( int nAlpha )
{
    m_nAlpha = nAlpha;
    ::SetLayeredWindowAttributes(m_hWnd, RGB_KEY, m_nAlpha, LWA_COLORKEY | LWA_ALPHA);
    Invalidate();
}

COLORREF CMainDlg::GetColor()
{
    return m_rgb;
}

int CMainDlg::GetAlpha()
{
    return m_nAlpha;
}

void CMainDlg::WriteConfig()
{
    CString strPath, strTmp;

    GetPathIni(strPath);
    strTmp.Format(_T("%ld"), m_rgb);
    ::WritePrivateProfileString(_T("UI"),_T("Color"), strTmp, strPath); 
    strTmp.Format(_T("%d"), m_nAlpha);
    ::WritePrivateProfileString(_T("UI"),_T("Alpha"), strTmp, strPath); 
}

void CMainDlg::ReadConfig()
{
    CString strPath, strTmp;

    GetPathIni(strPath);
    m_rgb = ::GetPrivateProfileInt(_T("UI"),_T("Color"), RGB_DEFAULT, strPath); 
    m_nAlpha = ::GetPrivateProfileInt(_T("UI"),_T("Alpha"), INT_ALPHA_DEFAULT, strPath); 
}

void CMainDlg::GetPathIni( CString &strPath )
{
    TCHAR szExePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szExePath, _MAX_PATH);
    ::PathRemoveFileSpec(szExePath);
    ::PathAppend(szExePath, STR_PATH_CONFIG);
    strPath = szExePath;
}
