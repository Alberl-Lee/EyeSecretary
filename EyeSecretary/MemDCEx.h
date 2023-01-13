/***************************************************
* @file   : MemDCEx.h
* @author : lixiongzhi
* @date   : 2014.04.28
* @brief  : 内存DC，防止界面闪烁（想立即显示的，可以手动调用Release，或者加上{}作用域）
****************************************************/
#pragma once

class CMemDCEx
{
public:
    CMemDCEx(HDC hdc, const RECT &rect)
    {
        m_rect    = rect;
        m_hdc     = hdc;
        m_hdcMem  = ::CreateCompatibleDC(hdc);
        m_hBmp    = ::CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
        m_hBmpOld = ::SelectObject(m_hdcMem, m_hBmp);
    }

    void Release()
    {
        ::BitBlt(m_hdc, m_rect.left, m_rect.top, (m_rect.right - m_rect.left), (m_rect.bottom - m_rect.top), 
            m_hdcMem, m_rect.left, m_rect.top, SRCCOPY);
        ::SelectObject(m_hdcMem, m_hBmpOld);   
        ::DeleteObject(m_hBmp); 
        ::DeleteDC(m_hdcMem);
        m_hdcMem = NULL;
        m_hBmp   = NULL;
    }

    ~CMemDCEx()	
    {	
        Release();	
    }

    operator HDC()
    {
        return m_hdcMem;
    }

private:	
    HDC         m_hdc;
    HDC         m_hdcMem;
    HBITMAP		m_hBmp;		
    HGDIOBJ		m_hBmpOld;	
    RECT        m_rect;
};
