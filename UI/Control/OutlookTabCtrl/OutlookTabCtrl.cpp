//==========================================================
// Author: Baradzenka Aleh (baradzenka@gmail.com)
//==========================================================
// 
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <cassert>
#include <algorithm>
#include "OutlookTabCtrl.h"
/////////////////////////////////////////////////////////////////////////////
#pragma comment (lib, "Gdiplus.lib")
/////////////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4355)   // 'this' : used in base member initializer list.
#undef max
#undef min
#undef new
#undef small
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
struct OutlookTabCtrl::Private : IRecalc
{	struct VirtualWindow : CDC
	{	VirtualWindow(CWnd *wnd)
		{	assert(wnd && ::IsWindow(wnd->m_hWnd));
			pwnd = wnd;
			pdc = pwnd->BeginPaint(&ps/*out*/);
			pwnd->GetClientRect(&rect/*out*/);
			if(CreateCompatibleDC(pdc) && bitmap.CreateCompatibleBitmap(pdc,rect.Width(),rect.Height()))
			{	SelectObject(&bitmap);
				SetBkMode(TRANSPARENT);
			}
		}
		~VirtualWindow()
		{	if(bitmap.m_hObject)
				pdc->BitBlt(0,0,rect.Width(),rect.Height(), this, 0,0, SRCCOPY);
			pwnd->EndPaint(&ps);
		}

	private:
		CWnd *pwnd;
		PAINTSTRUCT ps;
		CDC *pdc;
		CRect rect;
		CBitmap bitmap;
	};

public:
	Private(OutlookTabCtrl &owner);
	~Private();

private:
	OutlookTabCtrl &o;

private:   // OutlookTabCtrl::IRecalc.
	int GetBorderWidth(OutlookTabCtrl const *ctrl, IRecalc *base) override;
	int GetCaptionHeight(OutlookTabCtrl const *ctrl, IRecalc *base) override;
	int GetSplitterHeight(OutlookTabCtrl const *ctrl, IRecalc *base) override;
	int GetStripeHeight(OutlookTabCtrl const *ctrl, IRecalc *base) override;
	int GetButtonHeight(OutlookTabCtrl const *ctrl, IRecalc *base) override;
	int GetMinButtonWidth(OutlookTabCtrl const *ctrl, IRecalc *base) override;
	int GetMenuButtonWidth(OutlookTabCtrl const *ctrl, IRecalc *base) override;

private:
	ULONG_PTR m_gdiPlusToken;

public:
	Ability *m_pAbilityMngr;
	Notify *m_pNotifyMngr;
	ToolTip *m_pToolTipMngr;
	Draw *m_pDrawMngr;
	IRecalc *m_pRecalcMngr;
		// 
	Layout m_Layout;
	ButtonsAlign m_ButtonsAlign;
	CaptionAlign m_CaptionAlign;
		// 
	Gdiplus::Bitmap *m_pBitmapStripeNorm, *m_pBitmapStripeDis;
	Gdiplus::Bitmap *m_pBitmapButtonNorm, *m_pBitmapButtonDis;
	CSize m_szImageStripe, m_szImageButton;
	COLORREF m_clrImageStripeTransp, m_clrImageButtonTransp;
	CFont m_FontCaption, m_FontStripes, m_FontButtons;
	HCURSOR m_hCurDrag;
		// 
	bool m_bShowBorder;
	bool m_bShowCaption;
	bool m_bActiveSplitter;
	bool m_bShowMenuButton;
	bool m_bHideEmptyButtonsBar;
	bool m_bShowButtonText;
	bool m_bShowButtonSeparator;

public:
	struct Item
	{	HWND wnd;
		struct { int stripe, button; } image;
		CString text;
		__int64 data;
			// 
		CRect rect;
		bool button;
		bool visible;
		bool disabled;
			// 
		void operator<<(CArchive &ar);   // save.
		void operator>>(CArchive &ar);   // load.
	};
	std::vector<Item *> m_items;
	typedef std::vector<Item *>::iterator i_item;
	typedef std::vector<Item *>::const_iterator ci_item;
	typedef std::vector<Item *>::reverse_iterator ri_item;
		// 
	HANDLE m_hCurItem;
	HANDLE m_hHighlightedArea;
	HANDLE m_hPushedArea;   // area (splitter,button menu,stripe or button) selected between [WM_LBUTTONDOWN,WM_LBUTTONUP].
		// 
	CToolTipCtrl *m_pToolTip;
		// 
	CRect m_rcCaption, m_rcWindows, m_rcSplitter, m_rcButtonMenu;
	CRect m_rcStripes, m_rcButtons;
		// 
	HANDLE m_hTopVisibleStripe, m_hBottomVisibleStripe;
		// 
	UINT_PTR m_iTimerId;
		// 
	CPoint m_ptSplitterDrag;
	int m_iSplitterDragStartCountStripes;
		// 
	static const UINT HTSplitter;   // hit-test value for splitter area.
	static HANDLE HANDLE_SPLITTER;   // handle splitter area (like item).
	static HANDLE HANDLE_MENUBUTTON;   // handle menu button area (like item).

public:
	Item *HandleToItem(HANDLE h);
	Item const *HandleToItem(HANDLE h) const;
	HANDLE InsertItem(i_item before, HWND Wnd, TCHAR const *text, int imageStripe, int imageButton);
	bool LoadImage(HMODULE moduleRes/*or null*/, UINT resID, bool pngImage, Gdiplus::Bitmap **bmp/*out*/) const;
	bool CreateImageList(Gdiplus::Bitmap *bmp, int imageWidth, COLORREF clrMask/*or CLR_NONE*/, COLORREF clrBack/*or CLR_NONE*/, CImageList *imageList/*out*/) const;
	void PrepareRecalc();
	void Recalc();   // recalculate control.
	void UpdateToolTips();
	HANDLE HitTestEx(CPoint point) const;
	HANDLE GetItemByWindowID(int id) const;
	void SetFocusToChild();
	void MoveChangedWindow(HWND wnd, CRect const *rc, bool redraw);
	bool LoadStateInner(CArchive *ar);
	bool SaveStateInner(CArchive *ar) const;
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// OutlookTabCtrl.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
const UINT OutlookTabCtrl::Private::HTSplitter = 0x0000fff0;   // hit-test value for splitter area.
HANDLE OutlookTabCtrl::Private::HANDLE_SPLITTER = reinterpret_cast<HANDLE>(1);   // handle splitter area (like item).
HANDLE OutlookTabCtrl::Private::HANDLE_MENUBUTTON = reinterpret_cast<HANDLE>(2);   // handle menu button area (like item).
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(OutlookTabCtrl,CWnd)
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(OutlookTabCtrl, CWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CAPTURECHANGED()
	ON_WM_SETCURSOR()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONDBLCLK()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// 
OutlookTabCtrl::OutlookTabCtrl() :
	p( *new Private(*this) )
{
}
// 
OutlookTabCtrl::~OutlookTabCtrl()
{	delete &p;
}
/////////////////////////////////////////////////////////////////////////////
// 
OutlookTabCtrl::Private::Private(OutlookTabCtrl &owner) : o(owner)
{	m_pAbilityMngr = nullptr;
	m_pNotifyMngr = nullptr;
	m_pToolTipMngr = nullptr;
	m_pDrawMngr = nullptr;
	m_pRecalcMngr = this;
		// 
	m_Layout = Layout1;
	m_ButtonsAlign = ButtonsAlignRight;
	m_CaptionAlign = CaptionAlignTop;
		// 
	m_pBitmapStripeNorm = m_pBitmapStripeDis = m_pBitmapButtonNorm = m_pBitmapButtonDis = nullptr;
	m_szImageStripe.cx = m_szImageButton.cx = 0;
	m_szImageStripe.cy = m_szImageButton.cy = 0;
	m_clrImageStripeTransp = m_clrImageButtonTransp = CLR_NONE;
	m_hCurDrag = nullptr;
		// 
	m_bShowBorder = true;
	m_bShowCaption = true;
	m_bActiveSplitter = true;
	m_bShowMenuButton = true;
	m_bHideEmptyButtonsBar = false;
	m_bShowButtonText = true;
	m_bShowButtonSeparator = true;
		// 
	m_gdiPlusToken = 0;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiPlusToken/*out*/,&gdiplusStartupInput,nullptr);
}
//
OutlookTabCtrl::Private::~Private()
{	o.DestroyWindow();
		// 
	::delete m_pBitmapStripeNorm;
	::delete m_pBitmapStripeDis;
	::delete m_pBitmapButtonNorm;
	::delete m_pBitmapButtonDis;
	if(m_hCurDrag) 
		::DestroyCursor(m_hCurDrag);
	if(m_gdiPlusToken)
		Gdiplus::GdiplusShutdown(m_gdiPlusToken);
}
/////////////////////////////////////////////////////////////////////////////
// 
BOOL OutlookTabCtrl::Create(LPCTSTR /*lpszClassName*/, LPCTSTR /*lpszWindowName*/, DWORD style, const RECT& rect, CWnd* parentWnd, UINT id, CCreateContext* /*context*/)
{	return Create(parentWnd,style,rect,id);
}
// 
bool OutlookTabCtrl::Create(CWnd *parent, DWORD style, RECT const &rect, UINT id)
{	p.m_hCurItem = nullptr;
	p.m_hHighlightedArea = nullptr;
	p.m_hPushedArea = nullptr;
		// 
	p.m_pToolTip = nullptr;
	p.m_hTopVisibleStripe = p.m_hBottomVisibleStripe = nullptr;
	p.m_iTimerId = 0;
		// 
	const CString className = AfxRegisterWndClass(CS_DBLCLKS,::LoadCursor(nullptr,IDC_ARROW),nullptr,nullptr);
	if( !CWnd::Create(className,_T(""),style | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,rect,parent,id) )
		return false;
		// 
	CFont *font = CFont::FromHandle( static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT)) );
	if(!p.m_FontCaption.m_hObject)
		SetCaptionFont(font);
	if(!p.m_FontStripes.m_hObject) 
		SetStripesFont(font);
	if(!p.m_FontButtons.m_hObject) 
		SetButtonsFont(font);
		// 
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::OnDestroy()
{	if(p.m_pToolTipMngr && p.m_pToolTip) 
		p.m_pToolTipMngr->DestroyToolTip(p.m_pToolTip);
	DeleteAllItems();
		// 
	CWnd::OnDestroy();
}
/////////////////////////////////////////////////////////////////////////////
// Don't use CWnd::PreTranslateMessage to call CToolTipCtrl::RelayEvent.
//  If TabCtrl is in a Regular MFC DLL, then CWnd::PreTranslateMessage is not called. 
// 
LRESULT OutlookTabCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{	if(p.m_pToolTip && p.m_pToolTip->m_hWnd)
		if(message==WM_LBUTTONDOWN || message==WM_LBUTTONUP || message==WM_MBUTTONDOWN || message==WM_MBUTTONUP ||   // All messages required for TTM_RELAYEVENT.
			message==WM_MOUSEMOVE || message==WM_NCMOUSEMOVE || message==WM_RBUTTONDOWN || message==WM_RBUTTONUP)
		{
			// Don't use AfxGetCurrentMessage(). If TabCtrl is in a Regular MFC DLL, then we get an empty MSG. 
			MSG msg = {m_hWnd,message,wParam,lParam,0,{0,0}};
			p.m_pToolTip->RelayEvent(&msg);
		}
	return CWnd::WindowProc(message,wParam,lParam);
}
/////////////////////////////////////////////////////////////////////////////
// 
HANDLE OutlookTabCtrl::AddItem(HWND wnd, TCHAR const *text, int imageStripe, int imageButton)
{	return p.InsertItem( p.m_items.end(), wnd,text,imageStripe,imageButton);
}
// 
HANDLE OutlookTabCtrl::InsertItem(HANDLE before, HWND wnd, TCHAR const *text, int imageStripe, int imageButton)
{	assert( IsItemExist(before) );
		// 
	Private::i_item i_before = p.m_items.begin() + GetItemIndexByHandle(before);
	return p.InsertItem(i_before,wnd,text,imageStripe,imageButton);
}
// 
HANDLE OutlookTabCtrl::Private::InsertItem(i_item before, HWND wnd, TCHAR const *text, int imageStripe, int imageButton)
{	assert(wnd && ::IsWindow(wnd) && ::GetParent(wnd)==o.m_hWnd);
	assert(text);
	assert(imageStripe>=-1 && imageButton>=-1);
	assert(GetItemByWindowID(::GetDlgCtrlID(wnd))==nullptr);   // window with this ID has been added.
	assert( ::GetDlgCtrlID(wnd) );   // ID==0 - this is error.
		// 
	Item *item = new (std::nothrow) Item;
	if(!item)
		return nullptr;
		// 
	item->wnd = wnd;
	item->image.stripe = imageStripe;
	item->image.button = imageButton;
	item->text = text;
		// 
	item->button = (before!=m_items.end() ? (*before)->button : (m_items.empty() ? false : m_items.back()->button));
	item->visible = true;
	item->disabled = false;
	m_items.insert(before,item);
		// 
	if(!m_hCurItem)
		m_hCurItem = o.GetTopVisibleAndEnableItem();
	else
		::ShowWindow(wnd,SW_HIDE);
		// 
	return item;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::RemoveItem(HANDLE before, HANDLE src)
{	assert(IsItemExist(before) && IsItemExist(src));
		// 
	p.m_items.erase( p.m_items.begin()+GetItemIndexByHandle(src) );
	p.m_items.insert( p.m_items.begin()+GetItemIndexByHandle(before), p.HandleToItem(src));
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::DeleteItem(HANDLE item)
{	assert( IsItemExist(item) );
	delete p.HandleToItem(item);
	p.m_items.erase( p.m_items.begin()+GetItemIndexByHandle(item) );
	if(p.m_hCurItem==item)
		p.m_hCurItem = GetTopVisibleAndEnableItem();
	if(p.m_hTopVisibleStripe==item)
		p.m_hTopVisibleStripe = nullptr;
	if(p.m_hBottomVisibleStripe==item)
		p.m_hBottomVisibleStripe = nullptr;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::DeleteAllItems()
{	for(Private::i_item i=p.m_items.begin(); i!=p.m_items.end(); ++i)
		delete *i;
	p.m_items.clear();
	p.m_hCurItem = nullptr;
	p.m_hTopVisibleStripe = p.m_hBottomVisibleStripe = nullptr;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::SetAbilityManager(Ability *ptr/*or null*/)
{	p.m_pAbilityMngr = ptr;
}
OutlookTabCtrl::Ability *OutlookTabCtrl::GetAbilityManager() const
{	return p.m_pAbilityMngr;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::SetNotifyManager(Notify *ptr/*or null*/)
{	p.m_pNotifyMngr = ptr;
}
OutlookTabCtrl::Notify *OutlookTabCtrl::GetNotifyManager() const
{	return p.m_pNotifyMngr;
}
/////////////////////////////////////////////////////////////////////////////
//
void OutlookTabCtrl::SetToolTipManager(ToolTip *ptr/*or null*/)
{	p.m_pToolTipMngr = ptr;
}
OutlookTabCtrl::ToolTip *OutlookTabCtrl::GetToolTipManager() const
{	return p.m_pToolTipMngr;
}
/////////////////////////////////////////////////////////////////////////////
//
void OutlookTabCtrl::SetDrawManager(Draw *ptr/*or null*/)
{	p.m_pDrawMngr = ptr;
}
OutlookTabCtrl::Draw *OutlookTabCtrl::GetDrawManager() const
{	return p.m_pDrawMngr;
}
/////////////////////////////////////////////////////////////////////////////
//
void OutlookTabCtrl::SetRecalcManager(OutlookTabCtrl::IRecalc *ptr/*or null*/)
{	p.m_pRecalcMngr = (ptr ? ptr : &p);
}
OutlookTabCtrl::IRecalc *OutlookTabCtrl::GetRecalcManager() const
{	return p.m_pRecalcMngr;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::SetCaptionAlign(CaptionAlign align)
{	p.m_CaptionAlign = align;
}
// 
OutlookTabCtrl::CaptionAlign OutlookTabCtrl::GetCaptionAlign() const
{	return p.m_CaptionAlign;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::SetLayout(Layout layout)
{	p.m_Layout = layout;
}
// 
OutlookTabCtrl::Layout OutlookTabCtrl::GetLayout() const
{	return p.m_Layout;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::SetButtonsAlign(ButtonsAlign align)
{	p.m_ButtonsAlign = align;
}
// 
OutlookTabCtrl::ButtonsAlign OutlookTabCtrl::GetButtonsAlign() const
{	return p.m_ButtonsAlign;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::CreateStripesImages(HMODULE moduleRes/*or null*/, UINT resNormalID/*or 0*/, UINT resDisableID/*or 0*/, 
	bool pngImage, int imageWidth, COLORREF clrTransp/*=CLR_NONE*/)
{
	if(p.m_pBitmapStripeNorm)
	{	::delete p.m_pBitmapStripeNorm;
		p.m_pBitmapStripeNorm = nullptr;
	}
	if(p.m_pBitmapStripeDis)
	{	::delete p.m_pBitmapStripeDis;
		p.m_pBitmapStripeDis = nullptr;
	}
		// 
	bool res = true;
	if(resNormalID)
		if( !p.LoadImage(moduleRes,resNormalID,pngImage,&p.m_pBitmapStripeNorm/*out*/) )
			res = false;
	if(resDisableID)
		if( !p.LoadImage(moduleRes,resDisableID,pngImage,&p.m_pBitmapStripeDis/*out*/) )
			res = false;
		// 
	if(p.m_pBitmapStripeNorm)
		p.m_szImageStripe.SetSize(imageWidth, p.m_pBitmapStripeNorm->GetHeight() );
	else if(p.m_pBitmapStripeDis)
		p.m_szImageStripe.SetSize(imageWidth, p.m_pBitmapStripeDis->GetHeight() );
	else
		p.m_szImageStripe.cx = p.m_szImageStripe.cy = 0;
	p.m_clrImageStripeTransp = clrTransp;
		// 
	return res;
}
// 
void OutlookTabCtrl::GetStripesImages(Gdiplus::Bitmap **norm/*out,or null*/, Gdiplus::Bitmap **disable/*out,or null*/) const
{	if(norm)
		*norm = p.m_pBitmapStripeNorm;
	if(disable)
		*disable = p.m_pBitmapStripeDis;
} 
//
bool OutlookTabCtrl::GetStripesImageList(COLORREF clrDstBack/*or CLR_NONE*/, CImageList *normal/*out,or null*/, CImageList *disable/*out,or null*/) const
{	if(normal)
		if(!p.m_pBitmapStripeNorm || !p.CreateImageList(p.m_pBitmapStripeNorm, p.m_szImageStripe.cx, p.m_clrImageStripeTransp, clrDstBack, normal/*out*/) )
			return false;
	if(disable)
		if(!p.m_pBitmapStripeDis || !p.CreateImageList(p.m_pBitmapStripeDis, p.m_szImageStripe.cx, p.m_clrImageStripeTransp, clrDstBack, disable/*out*/) )
			return false;
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::CreateButtonsImages(HMODULE moduleRes/*or null*/, UINT resNormalID/*or 0*/, UINT resDisableID/*or 0*/, 
	bool pngImage, int imageWidth, COLORREF clrTransp/*=CLR_NONE*/)
{
	if(p.m_pBitmapButtonNorm)
	{	::delete p.m_pBitmapButtonNorm;
		p.m_pBitmapButtonNorm = nullptr;
	}
	if(p.m_pBitmapButtonDis)
	{	::delete p.m_pBitmapButtonDis;
		p.m_pBitmapButtonDis = nullptr;
	}
		// 
	bool res = true;
	if(resNormalID)
		if( !p.LoadImage(moduleRes,resNormalID,pngImage,&p.m_pBitmapButtonNorm/*out*/) )
			res = false;
	if(resDisableID)
		if( !p.LoadImage(moduleRes,resDisableID,pngImage,&p.m_pBitmapButtonDis/*out*/) )
			res = false;
		// 
	if(p.m_pBitmapButtonNorm)
		p.m_szImageButton.SetSize(imageWidth, p.m_pBitmapButtonNorm->GetHeight() );
	else if(p.m_pBitmapButtonDis)
		p.m_szImageButton.SetSize(imageWidth, p.m_pBitmapButtonDis->GetHeight() );
	else
		p.m_szImageButton.cx = p.m_szImageButton.cy = 0;
	p.m_clrImageButtonTransp = clrTransp;
		// 
	return res;
}
// 
void OutlookTabCtrl::GetButtonsImages(Gdiplus::Bitmap **normal/*out,or null*/, Gdiplus::Bitmap **disable/*out,or null*/) const
{	if(normal)
		*normal = p.m_pBitmapButtonNorm;
	if(disable)
		*disable = p.m_pBitmapButtonDis;
}
//
bool OutlookTabCtrl::GetButtonsImageList(COLORREF clrDstBack/*or CLR_NONE*/, CImageList *normal/*out,or null*/, CImageList *disable/*out,or null*/) const
{	if(normal)
		if(!p.m_pBitmapButtonNorm || !p.CreateImageList(p.m_pBitmapButtonNorm, p.m_szImageButton.cx, p.m_clrImageButtonTransp, clrDstBack, normal/*out*/) )
			return false;
	if(disable)
		if(!p.m_pBitmapButtonDis || !p.CreateImageList(p.m_pBitmapButtonDis, p.m_szImageButton.cx, p.m_clrImageButtonTransp, clrDstBack, disable/*out*/) )
			return false;
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::GetImagesSize(CSize *szStripe/*out,or null*/, CSize *szButton/*out,or null*/) const
{	if(szStripe)
	{	szStripe->cx = p.m_szImageStripe.cx;
		szStripe->cy = p.m_szImageStripe.cy;
	}
	if(szButton)
	{	szButton->cx = p.m_szImageButton.cx;
		szButton->cy = p.m_szImageButton.cy;
	}
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::GetImagesTranspColor(COLORREF *clrStripe/*out,or null*/, COLORREF *clrButton/*out,or null*/) const
{	if(clrStripe)
		*clrStripe = p.m_clrImageStripeTransp;
	if(clrButton)
		*clrButton = p.m_clrImageButtonTransp;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::SetItemText(HANDLE item, TCHAR const *text)
{	assert( IsItemExist(item) );
	p.HandleToItem(item)->text = text;
}
// 
CString OutlookTabCtrl::GetItemText(HANDLE item) const
{	assert( IsItemExist(item) );
	return p.HandleToItem(item)->text;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::SetItemImage(HANDLE item, int imageStripe, int imageButton)
{	assert( IsItemExist(item) );
	p.HandleToItem(item)->image.stripe = imageStripe;
	p.HandleToItem(item)->image.button = imageButton;
}
// 
void OutlookTabCtrl::GetItemImage(HANDLE item, int *imageStripe/*out,or null*/, int *imageButton/*out,or null*/) const
{	assert( IsItemExist(item) );
	if(imageStripe)
		*imageStripe = p.HandleToItem(item)->image.stripe;
	if(imageButton)
		*imageButton = p.HandleToItem(item)->image.button;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::SetItemWindow(HANDLE item, HWND wnd)
{	assert( IsItemExist(item) );
	assert(wnd && ::IsWindow(wnd) && ::GetParent(wnd)==m_hWnd);
	assert(::GetDlgCtrlID(p.HandleToItem(item)->wnd)==::GetDlgCtrlID(wnd) ||   // window with the same id.
		p.GetItemByWindowID(::GetDlgCtrlID(wnd))==nullptr);   // window with this ID has been added.
	p.HandleToItem(item)->wnd = wnd;
}
// 
HWND OutlookTabCtrl::GetItemWindow(HANDLE item) const
{	assert( IsItemExist(item) );
	return p.HandleToItem(item)->wnd;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::SetItemData(HANDLE item, __int64 data)
{	assert( IsItemExist(item) );
	p.HandleToItem(item)->data = data;
}
// 
__int64 OutlookTabCtrl::GetItemData(HANDLE item) const
{	assert( IsItemExist(item) );
	return p.HandleToItem(item)->data;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::SetCaptionFont(CFont *font)
{	assert(font);
		// 
	LOGFONT logfont;
	return (font->GetLogFont(&logfont/*out*/) ? SetCaptionFont(&logfont) : false);
}
// 
bool OutlookTabCtrl::SetCaptionFont(LOGFONT const *logfont)
{	assert(logfont);
		// 
	if(p.m_FontCaption.m_hObject)
		p.m_FontCaption.DeleteObject();
	return p.m_FontCaption.CreateFontIndirect(logfont)!=0;
}
// 
CFont *OutlookTabCtrl::GetCaptionFont()
{	return &p.m_FontCaption;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::SetStripesFont(CFont *font)
{	assert(font);
		// 
	LOGFONT logfont;
	return (font->GetLogFont(&logfont/*out*/) ? SetStripesFont(&logfont) : false);
}
// 
bool OutlookTabCtrl::SetStripesFont(LOGFONT const *logfont)
{	assert(logfont);
		// 
	if(p.m_FontStripes.m_hObject) 
		p.m_FontStripes.DeleteObject();
	return p.m_FontStripes.CreateFontIndirect(logfont)!=0;
}
// 
CFont *OutlookTabCtrl::GetStripesFont()
{	return &p.m_FontStripes;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::SetButtonsFont(CFont *font)
{	assert(font);
		// 
	LOGFONT logfont;
	return (font->GetLogFont(&logfont) ? SetButtonsFont(&logfont) : false);
}
// 
bool OutlookTabCtrl::SetButtonsFont(LOGFONT const *logfont)
{	assert(logfont);
		// 
	if(p.m_FontButtons.m_hObject) 
		p.m_FontButtons.DeleteObject();
	return p.m_FontButtons.CreateFontIndirect(logfont)!=0;
}
// 
CFont *OutlookTabCtrl::GetButtonsFont()
{	return &p.m_FontButtons;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::SetCursor(UINT drag)
{	return SetCursor( AfxGetResourceHandle(), drag);
}
// 
bool OutlookTabCtrl::SetCursor(HMODULE module, UINT drag)
{	if(p.m_hCurDrag)
	{	::DestroyCursor(p.m_hCurDrag);
		p.m_hCurDrag = nullptr;
	}
		// 
	if(module && drag)
		if((p.m_hCurDrag = ::LoadCursor(module,MAKEINTRESOURCE(drag)))==nullptr)
			return false;
		// 
	return true;
}
// 
HCURSOR OutlookTabCtrl::GetCursor() const
{	return p.m_hCurDrag;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::Update()
{	p.Recalc();
	Invalidate(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::OnSize(UINT nType, int cx, int cy)
{	CWnd::OnSize(nType, cx, cy);
	Update();
}
/////////////////////////////////////////////////////////////////////////////
// 
int OutlookTabCtrl::GetBorderWidth() const { return p.m_pRecalcMngr->GetBorderWidth(this,&p); }
int OutlookTabCtrl::GetCaptionHeight() const { return p.m_pRecalcMngr->GetCaptionHeight(this,&p); }
int OutlookTabCtrl::GetSplitterHeight() const { return p.m_pRecalcMngr->GetSplitterHeight(this,&p); }
int OutlookTabCtrl::GetStripeHeight() const { return p.m_pRecalcMngr->GetStripeHeight(this,&p); }
int OutlookTabCtrl::GetButtonHeight() const { return p.m_pRecalcMngr->GetButtonHeight(this,&p); }
int OutlookTabCtrl::GetMinButtonWidth() const { return p.m_pRecalcMngr->GetMinButtonWidth(this,&p); }
int OutlookTabCtrl::GetMenuButtonWidth() const { return p.m_pRecalcMngr->GetMenuButtonWidth(this,&p); }
// 
/////////////////////////////////////////////////////////////////////////////
// 
int OutlookTabCtrl::Private::GetBorderWidth(OutlookTabCtrl const * /*ctrl*/, IRecalc * /*base*/)
{	return 1;
}
// 
int OutlookTabCtrl::Private::GetCaptionHeight(OutlookTabCtrl const * /*ctrl*/, IRecalc * /*base*/)
{	CWindowDC dc(nullptr);
	CFont *pOldFont = dc.SelectObject(&const_cast<CFont &>(m_FontCaption));
	const int height = dc.GetTextExtent(_T("H"),1).cy;
	dc.SelectObject(pOldFont);
	return 4 + height + 4;
}
// 
int OutlookTabCtrl::Private::GetSplitterHeight(OutlookTabCtrl const * /*ctrl*/, IRecalc * /*base*/)
{	return (m_bActiveSplitter ? 6 : 4);
}
// 
int OutlookTabCtrl::Private::GetStripeHeight(OutlookTabCtrl const * /*ctrl*/, IRecalc * /*base*/)
{	CWindowDC dc(nullptr);
	CFont *pOldFont = dc.SelectObject(&const_cast<CFont &>(m_FontStripes));
	const int height = dc.GetTextExtent(_T("H"),1).cy;
	dc.SelectObject(pOldFont);
		// 
	return (3 + std::max<int>(m_szImageStripe.cy,height) + 3);
}
// 
int OutlookTabCtrl::Private::GetButtonHeight(OutlookTabCtrl const * /*ctrl*/, IRecalc * /*base*/)
{	CWindowDC dc(nullptr);
	CFont *pOldFont = dc.SelectObject(&const_cast<CFont &>(m_FontButtons));
	const int height = dc.GetTextExtent(_T("H"),1).cy;
	dc.SelectObject(pOldFont);
		// 
	return (3 + std::max<int>(m_szImageButton.cy,height) + 3);
}
// 
int OutlookTabCtrl::Private::GetMinButtonWidth(OutlookTabCtrl const * /*ctrl*/, IRecalc * /*base*/)
{	return (6 + m_szImageButton.cx + 6);
}
// 
int OutlookTabCtrl::Private::GetMenuButtonWidth(OutlookTabCtrl const * /*ctrl*/, IRecalc * /*base*/)
{	return (3 + m_szImageButton.cx + 3);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::OnPaint()
{	if(!p.m_pDrawMngr)
	{	CPaintDC dc(this);
		return;
	}
		// 
	Private::VirtualWindow virtwnd(this);
	if( !virtwnd.GetSafeHdc() )
	{	CPaintDC dc(this);
		return;
	}
		// 
	p.m_pDrawMngr->DrawBegin(this,&virtwnd);
		// 
	if(p.m_bShowBorder)
		if(GetBorderWidth() > 0)
		{	CRect rc;
			GetClientRect(&rc);
			p.m_pDrawMngr->DrawBorder(this,&virtwnd,&rc);
		}
		// 
	if(p.m_bShowCaption)
	{	CFont *oldFont = static_cast<CFont*>( virtwnd.SelectObject(&p.m_FontCaption) );
		p.m_pDrawMngr->DrawCaption(this,&virtwnd,&p.m_rcCaption);
		virtwnd.SelectObject(oldFont);
	}
		// 
	p.m_pDrawMngr->DrawSplitter(this,&virtwnd,&p.m_rcSplitter);
	if(p.m_bShowMenuButton) 
		p.m_pDrawMngr->DrawButtonMenu(this,&virtwnd,&p.m_rcButtonMenu);
	const bool visibleButtonsBar = IsButtonsAreaVisible();
	if(visibleButtonsBar) 
		p.m_pDrawMngr->DrawButtonsBackground(this,&virtwnd,&p.m_rcButtons);
		// 
	if( !p.m_items.empty() )
	{	CRgn rgn;
		rgn.CreateRectRgn(0,0,0,0);
			// 
		CFont *oldFont = static_cast<CFont*>( virtwnd.SelectObject(&p.m_FontStripes) );
			// draw stripes.
		Private::i_item i;
		for(i=p.m_items.begin(); i!=p.m_items.end(); ++i)
		{	if((*i)->button)
				break;
			if(!(*i)->visible)
				continue;
				// 
			rgn.SetRectRgn(&(*i)->rect);
			virtwnd.SelectClipRgn(&rgn,RGN_COPY);
				// 
			const bool drawSeparator = visibleButtonsBar ||
				((p.m_Layout==Layout1 || p.m_Layout==Layout4) && *i!=p.m_hBottomVisibleStripe) ||
				((p.m_Layout==Layout2 || p.m_Layout==Layout3) && *i!=p.m_hTopVisibleStripe);
			p.m_pDrawMngr->DrawStripe(this,&virtwnd,*i,drawSeparator);
		}
			// draw buttons.
		virtwnd.SelectObject(&p.m_FontButtons);
		rgn.SetRectRgn(&p.m_rcButtons);
		virtwnd.SelectClipRgn(&rgn,RGN_COPY);
			// 
		for(; i!=p.m_items.end(); ++i)
			if((*i)->visible)
				p.m_pDrawMngr->DrawButton(this,&virtwnd,*i);
			// 
		virtwnd.SelectClipRgn(nullptr,RGN_COPY);
		virtwnd.SelectObject(oldFont);
	}
		// 
	if(!p.m_hCurItem)
		p.m_pDrawMngr->DrawEmptyWindowsArea(this,&virtwnd,&p.m_rcWindows);
		// 
	p.m_pDrawMngr->DrawEnd(this,&virtwnd);
}
/////////////////////////////////////////////////////////////////////////////
// 
OutlookTabCtrl::ItemState OutlookTabCtrl::GetItemState(HANDLE item) const
{	ItemState state;
	state.selected = (p.m_hCurItem==item);
	state.highlighted = (p.m_hHighlightedArea==item);
	state.pushed = (p.m_hPushedArea==item);
	return state;
}
/////////////////////////////////////////////////////////////////////////////
//
OutlookTabCtrl::ItemState OutlookTabCtrl::GetMenuButtonState() const
{	return GetItemState(Private::HANDLE_MENUBUTTON);
}
/////////////////////////////////////////////////////////////////////////////
//
bool OutlookTabCtrl::IsAnyStripeOrButtonPushed() const
{	return p.m_hPushedArea && p.m_hPushedArea!=Private::HANDLE_SPLITTER;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Recalculate control.
// 
void OutlookTabCtrl::Private::Recalc()
{	PrepareRecalc();
		// 
	const int countStripes = o.GetNumberVisibleStripes();
	const int countButtons = o.GetNumberVisibleButtons();
		// 
	const int stripeHeight = o.GetStripeHeight();
	const int minButtonWidth = o.GetMinButtonWidth();
		// 
	const bool visibleButtonsBar = o.IsButtonsAreaVisible();
	const int allStripesHeight = (countStripes*stripeHeight + (countStripes/*pixels for borders*/-(visibleButtonsBar ? 0 : 1)));
	const int allButtonsHeight = (visibleButtonsBar ? o.GetButtonHeight() : 0);
		// 
	o.GetClientRect(&m_rcCaption);
		// 
	if(m_bShowBorder)
	{	const int width = o.GetBorderWidth();
		if(width>0)
			m_rcCaption.DeflateRect(width,width);
	}
		// 
	m_rcSplitter = m_rcStripes = m_rcButtons = m_rcButtonMenu = m_rcWindows = m_rcCaption;
		// 
	if(m_bShowCaption)
		if(m_CaptionAlign==CaptionAlignTop)
			m_rcSplitter.top = m_rcStripes.top = m_rcButtons.top = m_rcButtonMenu.top = m_rcWindows.top = 
				m_rcCaption.bottom = m_rcCaption.top + o.GetCaptionHeight();
		else	// Bottom.
			m_rcSplitter.bottom = m_rcStripes.bottom = m_rcButtons.bottom= m_rcButtonMenu.bottom = 
				m_rcWindows.bottom = m_rcCaption.top = m_rcCaption.bottom - o.GetCaptionHeight();
		// 
	m_rcButtons.right = m_rcButtonMenu.left = m_rcButtonMenu.right - (m_bShowMenuButton ? o.GetMenuButtonWidth() : 0);
		// 
	switch(m_Layout)
	{	case Layout1:
			m_rcStripes.bottom = m_rcButtons.top = m_rcButtonMenu.top = m_rcButtons.bottom - allButtonsHeight;
			m_rcSplitter.bottom = m_rcStripes.top = m_rcStripes.bottom - allStripesHeight;
			m_rcWindows.bottom = m_rcSplitter.top = m_rcSplitter.bottom - o.GetSplitterHeight();
			break;
		case Layout2:
			m_rcButtons.bottom = m_rcStripes.top = m_rcButtonMenu.bottom = m_rcStripes.bottom - allStripesHeight;
			m_rcSplitter.bottom = m_rcButtons.top = m_rcButtonMenu.top = m_rcButtons.bottom - allButtonsHeight;
			m_rcWindows.bottom = m_rcSplitter.top = m_rcSplitter.bottom - o.GetSplitterHeight();
			break;
		case Layout3:
			m_rcButtons.bottom = m_rcStripes.top = m_rcButtonMenu.bottom = m_rcButtons.top + allButtonsHeight;
			m_rcSplitter.top = m_rcStripes.bottom = m_rcStripes.top + allStripesHeight;
			m_rcWindows.top = m_rcSplitter.bottom = m_rcSplitter.top + o.GetSplitterHeight();
			break;
		case Layout4:
			m_rcButtons.top = m_rcStripes.bottom = m_rcButtonMenu.top = m_rcStripes.top + allStripesHeight;
			m_rcSplitter.top = m_rcButtons.bottom = m_rcButtonMenu.bottom = m_rcButtons.top + allButtonsHeight;
			m_rcWindows.top = m_rcSplitter.bottom = m_rcSplitter.top + o.GetSplitterHeight();
			break;
	}
		// 
	int iCountFoundStripes = 0;
	int iCountFoundButtons = 0;
		// 
	for(i_item i=m_items.begin(); i!=m_items.end(); ++i)
	{	Item *item = *i;
		if(!item->visible)
			continue;
			// 
		if(!item->button)	// this is stripe.
		{	item->rect = m_rcStripes;
				// 
			if(m_Layout==Layout1 || m_Layout==Layout4)
			{	item->rect.top = m_rcStripes.top + (stripeHeight + 1/*separator*/)*iCountFoundStripes;
				item->rect.bottom = std::min<int>(item->rect.top + (stripeHeight + 1/*separator*/), m_rcStripes.bottom);
			}
			else	// Layout2 or Layout3.
				if(visibleButtonsBar)
				{	item->rect.top = m_rcStripes.top + (stripeHeight + 1/*separator*/)*iCountFoundStripes;
					item->rect.bottom = std::min<int>(item->rect.top + (stripeHeight + 1/*separator*/), m_rcStripes.bottom);
				}
				else
				{	item->rect.top = m_rcStripes.top + (item!=m_hTopVisibleStripe ? ((stripeHeight + 1/*separator*/)*iCountFoundStripes-1) : 0);
					item->rect.bottom = item->rect.top + (stripeHeight + (item!=m_hTopVisibleStripe ? 1/*separator*/ : 0));
				}
			iCountFoundStripes ++;
		}
		else	// this is button.
		{	item->rect = m_rcButtons;
				// 
			const int buttonWidth = std::max<int>(minButtonWidth, m_rcButtons.Width()/countButtons);
			if(m_ButtonsAlign==ButtonsAlignLeft)
			{	item->rect.right = (iCountFoundButtons==0 ? m_rcButtons.right : m_rcButtons.right-iCountFoundButtons*buttonWidth);
				item->rect.left = (iCountFoundButtons!=countButtons-1 || buttonWidth*countButtons>m_rcButtons.Width() ?
					item->rect.right-buttonWidth : m_rcButtons.left);
			}
			else	// ButtonsAlign::Right.
			{	item->rect.left = (iCountFoundButtons==0 ? m_rcButtons.left : m_rcButtons.left+iCountFoundButtons*buttonWidth);
				item->rect.right = (iCountFoundButtons!=countButtons-1 || buttonWidth*countButtons>m_rcButtons.Width() ?
					item->rect.left+buttonWidth : m_rcButtons.right);
			}
			iCountFoundButtons ++;
		}
	}
		// 
	const HANDLE hOldCurItem = m_hCurItem;
		// 
	if(!m_hCurItem || !HandleToItem(m_hCurItem)->visible || HandleToItem(m_hCurItem)->disabled)
		m_hCurItem = o.GetTopVisibleAndEnableItem();
		// 
	if(m_hCurItem)
	{	MoveChangedWindow(HandleToItem(m_hCurItem)->wnd,&m_rcWindows,true);
		if( !::IsWindowVisible(HandleToItem(m_hCurItem)->wnd) )
			::ShowWindow(HandleToItem(m_hCurItem)->wnd,SW_SHOW);
	}
	if(hOldCurItem!=m_hCurItem)
		if(hOldCurItem)
			::ShowWindow(HandleToItem(hOldCurItem)->wnd,SW_HIDE);
		// 
	UpdateToolTips();
}
/////////////////////////////////////////////////////////////////////////////
// Correct order items and assign top and bottom visible stripes.
// 
void OutlookTabCtrl::Private::PrepareRecalc()
{	m_hTopVisibleStripe = m_hBottomVisibleStripe = nullptr;
		// 
	for(i_item i=m_items.begin(); i!=m_items.end(); ++i)
		if(!(*i)->visible)
			(*i)->button = false;
		else
			if(!(*i)->button)   // this is stripe.
			{	if(!m_hTopVisibleStripe)
					m_hTopVisibleStripe = *i;
				m_hBottomVisibleStripe = *i;
			}
			else   // this is the first button.
			{	for(++i; i!=m_items.end(); ++i)
					(*i)->button = true;
				break;
			}
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::Private::UpdateToolTips()
{	if(m_pToolTipMngr)
	{	if(!m_pToolTip)
			m_pToolTip = m_pToolTipMngr->CreateToolTip(&o);
			// 
		if(m_pToolTip && m_pToolTip->m_hWnd)
		{	int c = m_pToolTip->GetToolCount();
			for(; c>0; --c)
				m_pToolTip->DelTool(&o,c);
				// 
			c = 1;
			for(i_item i=m_items.begin(); i!=m_items.end(); ++i)
			{	Item const *item = *i;
				if(item->button && item->visible && !item->text.IsEmpty()) 
					if( m_pToolTipMngr->HasButtonTooltip(&o,*i) )
						if( m_pToolTip->AddTool(&o,item->text,item->rect,c) )
							++c;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::CanVisibleItemPush() const
{	return GetNumberVisibleStripes()>0;
}
// 
void OutlookTabCtrl::PushVisibleItem()
{	assert(p.m_hPushedArea==nullptr);
		// 
	for(Private::ri_item ri=p.m_items.rbegin(); ri!=p.m_items.rend(); ++ri)
		if((*ri)->visible && !(*ri)->button)
		{	(*ri)->button = true;
			break;
		}
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::CanVisibleItemPop() const
{	return GetNumberVisibleButtons()>0;
}
// 
void OutlookTabCtrl::PopVisibleItem()
{	assert(p.m_hPushedArea==nullptr);
		// 
	for(Private::i_item i=p.m_items.begin(); i!=p.m_items.end(); ++i)
		if((*i)->visible && (*i)->button)
		{	(*i)->button = false;
			break;
		}
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::SetNumberVisibleItemsInStripeState(int count)
{	assert(count>=0 && count<=GetTotalNumberVisibleItems());
		// 
	for(Private::i_item i=p.m_items.begin(); i!=p.m_items.end(); ++i)
	{	(*i)->button = count<=0;
		if((*i)->visible)
			--count;
	}
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::SetItemsOrder(std::vector<HANDLE> const &items)
{	assert(items.size()==p.m_items.size());
		// 
	std::vector<Private::Item *> itemsNew;
	for(std::vector<HANDLE>::const_iterator i=items.begin(), e=items.end(); i!=e; ++i)
	{	Private::Item *item = p.HandleToItem(*i);
		assert(std::find(p.m_items.begin(),p.m_items.end(),item) != p.m_items.end());
		itemsNew.push_back(item);
	}
	p.m_items = itemsNew;
}
/////////////////////////////////////////////////////////////////////////////
// 
int OutlookTabCtrl::GetNumberVisibleStripes() const
{	int countStripes = 0;
	for(Private::ci_item i=p.m_items.begin(); i!=p.m_items.end(); ++i)
	{	if((*i)->button)
			break;
		if((*i)->visible)
			++countStripes;
	}
	return countStripes;
}
/////////////////////////////////////////////////////////////////////////////
// 
int OutlookTabCtrl::GetNumberVisibleButtons() const
{	int countbuts = 0;
	for(Private::ri_item ri=p.m_items.rbegin(); ri!=p.m_items.rend(); ++ri)
	{	if(!(*ri)->button)
			break;
		if((*ri)->visible)
			++countbuts;
	}
	return countbuts;
}
/////////////////////////////////////////////////////////////////////////////
// 
int OutlookTabCtrl::GetTotalNumberVisibleItems() const
{	return GetNumberVisibleStripes() + GetNumberVisibleButtons();
}
/////////////////////////////////////////////////////////////////////////////
// 
int OutlookTabCtrl::GetNumberStripes() const
{	int countStripes = 0;
	for(Private::ci_item i=p.m_items.begin(); i!=p.m_items.end(); ++i)
	{	if((*i)->button)
			break;
		++countStripes;
	}
	return countStripes;
}
/////////////////////////////////////////////////////////////////////////////
// 
int OutlookTabCtrl::GetNumberButtons() const
{	int countbuts = 0;
	for(Private::ri_item ri=p.m_items.rbegin(); ri!=p.m_items.rend(); ++ri)
	{	if(!(*ri)->button)
			break;
		++countbuts;
	}
	return countbuts;
}
/////////////////////////////////////////////////////////////////////////////
// 
int OutlookTabCtrl::GetTotalNumberItems() const
{	return static_cast<int>( p.m_items.size() );
}
/////////////////////////////////////////////////////////////////////////////
// 
HANDLE OutlookTabCtrl::GetItemHandleByIndex(int idx) const
{	assert(idx>=0 && idx<GetTotalNumberItems());
	return p.m_items[idx];
}
/////////////////////////////////////////////////////////////////////////////
// 
HANDLE OutlookTabCtrl::GetTopVisibleItem() const
{	for(Private::ci_item i=p.m_items.begin(); i!=p.m_items.end(); ++i)
		if((*i)->visible)
			return *i;
	return nullptr;
}
/////////////////////////////////////////////////////////////////////////////
// 
HANDLE OutlookTabCtrl::GetBottomVisibleItem() const
{	for(Private::ri_item ri=p.m_items.rbegin(); ri!=p.m_items.rend(); ++ri)
		if((*ri)->visible)
			return *ri;
	return nullptr;
}
/////////////////////////////////////////////////////////////////////////////
// 
HANDLE OutlookTabCtrl::GetTopVisibleAndEnableItem() const
{	for(Private::ci_item i=p.m_items.begin(); i!=p.m_items.end(); ++i)
		if((*i)->visible && !(*i)->disabled)
			return *i;
	return nullptr;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::SelectItem(HANDLE item)
{	assert(p.m_hPushedArea==nullptr);
	assert( IsItemExist(item) );
		// 
	if(p.m_hCurItem==item || !p.HandleToItem(item)->visible || p.HandleToItem(item)->disabled)
		return;
		// 
	::MoveWindow(p.HandleToItem(item)->wnd,p.m_rcWindows.left,p.m_rcWindows.top,p.m_rcWindows.Width(),p.m_rcWindows.Height(),TRUE);
	::ShowWindow(p.HandleToItem(item)->wnd,SW_SHOW);
	if(p.m_hCurItem)
		::ShowWindow(p.HandleToItem(p.m_hCurItem)->wnd,SW_HIDE);
		// 
	p.m_hCurItem = item;
	Invalidate(FALSE);
}
// 
HANDLE OutlookTabCtrl::GetSelectedItem() const
{	return p.m_hCurItem;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::ShowItem(HANDLE item, bool show)
{	assert( IsItemExist(item) );
	p.HandleToItem(item)->visible = show;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::IsItemVisible(HANDLE item) const
{	assert( IsItemExist(item) );
	return p.HandleToItem(item)->visible;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::DisableItem(HANDLE item, bool disable)
{	assert( IsItemExist(item) );
	p.HandleToItem(item)->disabled = disable;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::IsItemDisabled(HANDLE item) const
{	assert( IsItemExist(item) );
	return p.HandleToItem(item)->disabled;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::IsItemInStripeState(HANDLE item) const
{	assert( IsItemExist(item) );
	return !p.HandleToItem(item)->button;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::IsItemInButtonState(HANDLE item) const
{	assert( IsItemExist(item) );
	return p.HandleToItem(item)->button;
}
/////////////////////////////////////////////////////////////////////////////
// 
HANDLE OutlookTabCtrl::Private::GetItemByWindowID(int id) const
{	for(ci_item i=m_items.begin(); i!=m_items.end(); ++i)
		if(::GetDlgCtrlID((*i)->wnd)==id)
			return *i;
	return nullptr;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::Private::SetFocusToChild()
{	if(m_hCurItem && 
		HandleToItem(m_hCurItem)->visible && !HandleToItem(m_hCurItem)->disabled &&
		::GetFocus()!=HandleToItem(m_hCurItem)->wnd)
	{
		::SetFocus(HandleToItem(m_hCurItem)->wnd);
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::Private::MoveChangedWindow(HWND wnd, CRect const *rcNew, bool redraw)
{	CRect rcOld;
	::GetWindowRect(wnd,&rcOld/*out*/);
	::MapWindowPoints(HWND_DESKTOP,o.m_hWnd,reinterpret_cast<POINT*>(&rcOld),2);
	if(*rcNew!=rcOld)
		::MoveWindow(wnd,rcNew->left,rcNew->top,rcNew->Width(),rcNew->Height(),redraw);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::ShowBorder(bool show)
{	p.m_bShowBorder = show;
}
// 
bool OutlookTabCtrl::IsBorderVisible() const
{	return p.m_bShowBorder;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::ShowCaption(bool show)
{	p.m_bShowCaption = show;
}
// 
bool OutlookTabCtrl::IsCaptionVisible() const
{	return p.m_bShowCaption;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::ActivateSplitter(bool active)
{	p.m_bActiveSplitter = active;
}
// 
bool OutlookTabCtrl::IsSplitterActive() const
{	return p.m_bActiveSplitter;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::ShowMenuButton(bool show)
{	p.m_bShowMenuButton = show;
}
// 
bool OutlookTabCtrl::IsMenuButtonVisible() const
{	return p.m_bShowMenuButton;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::HideEmptyButtonsArea(bool hide)
{	p.m_bHideEmptyButtonsBar = hide;
}
// 
bool OutlookTabCtrl::IsEmptyButtonsAreaHide() const
{	return p.m_bHideEmptyButtonsBar;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::ShowButtonText(bool show)
{	p.m_bShowButtonText = show;
}
// 
bool OutlookTabCtrl::IsButtonTextVisible() const
{	return p.m_bShowButtonText;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::ShowButtonSeparator(bool show)
{	p.m_bShowButtonSeparator = show;
}
// 
bool OutlookTabCtrl::IsButtonSeparatorVisible() const
{	return p.m_bShowButtonSeparator;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::IsButtonsAreaVisible() const
{	return p.m_bShowMenuButton || !p.m_bHideEmptyButtonsBar || GetNumberVisibleButtons()>0;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
HANDLE OutlookTabCtrl::HitTest(CPoint point) const
{	for(Private::ci_item i=p.m_items.begin(); i!=p.m_items.end(); ++i)
		if((*i)->visible)
			if( (*i)->rect.PtInRect(point) ) 
				return *i;
	return nullptr;
}
/////////////////////////////////////////////////////////////////////////////
// 
HANDLE OutlookTabCtrl::Private::HitTestEx(CPoint point) const
{	if( m_rcSplitter.PtInRect(point) )
		return HANDLE_SPLITTER;
	if( m_rcButtonMenu.PtInRect(point) )
		return HANDLE_MENUBUTTON;
		//
	HANDLE item = o.HitTest(point);
	if(item)
	{	if( o.IsItemDisabled(item) )
			return nullptr;
		return item;
	}
	return nullptr;
}
/////////////////////////////////////////////////////////////////////////////
// 
int OutlookTabCtrl::GetItemIndexByHandle(HANDLE item) const
{	for(Private::ci_item i=p.m_items.begin(); i!=p.m_items.end(); ++i)
		if(*i==p.HandleToItem(item)) 
			return static_cast<int>(i-p.m_items.begin());
	return -1;
}
/////////////////////////////////////////////////////////////////////////////
// 
int OutlookTabCtrl::GetVisibleItemIndexByHandle(HANDLE item) const
{	int idx=0;
	for(Private::ci_item i=p.m_items.begin(); i!=p.m_items.end(); ++i)
	{	if(!(*i)->visible)
			continue;
		if(*i==p.HandleToItem(item)) 
			return idx;
		idx++;
	}
	return -1;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::IsItemExist(HANDLE item) const
{	return GetItemIndexByHandle(item)!=-1;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
CRect OutlookTabCtrl::GetItemRect(HANDLE item) const
{	assert( IsItemExist(item) );
	return p.HandleToItem(item)->rect;
}
// 
CRect OutlookTabCtrl::GetCaptionRect() const 
{	return p.m_rcCaption;
}
// 
CRect OutlookTabCtrl::GetSplitterRect() const
{	return p.m_rcSplitter;
}
// 
CRect OutlookTabCtrl::GetStripesAreaRect() const
{	return p.m_rcStripes;
}
// 
CRect OutlookTabCtrl::GetButtonsAreaRect() const
{	return p.m_rcButtons;
}
// 
CRect OutlookTabCtrl::GetMenuButtonRect() const
{	return p.m_rcButtonMenu;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::OnNcLButtonDown(UINT nHitTest, CPoint point)
{	if(p.m_bActiveSplitter && nHitTest==Private::HTSplitter)
	{	p.m_ptSplitterDrag = point;
		ScreenToClient(&p.m_ptSplitterDrag);
		p.m_iSplitterDragStartCountStripes = GetNumberVisibleStripes();
			// 
		p.m_hPushedArea = Private::HANDLE_SPLITTER;
		SetCapture();
	}
	CWnd::OnNcLButtonDown(nHitTest, point);
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{	if(p.m_bActiveSplitter && nHitTest==Private::HTSplitter)
	{	if(GetNumberVisibleButtons()>0)	// there are buttons.
			SetNumberVisibleItemsInStripeState( GetTotalNumberVisibleItems() );
		else	// there are not buttons.
			SetNumberVisibleItemsInStripeState(0);
		Update();
	}
	CWnd::OnNcLButtonDblClk(nHitTest, point);
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{	p.SetFocusToChild();
		// 
	const HANDLE hPushedArea = p.HitTestEx(point);
		// 
	if(hPushedArea)
	{	p.m_hPushedArea = hPushedArea;
		Invalidate(FALSE);
		SetCapture();
	}
		// 
	CWnd::OnLButtonDown(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
//
void OutlookTabCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{	if(p.m_hPushedArea)
	{	const HANDLE hArea = p.HitTestEx(point);
			// 
		bool changedSelect = false;
			// 
		if(hArea==p.m_hPushedArea)
			changedSelect = (hArea!=Private::HANDLE_SPLITTER && (hArea==Private::HANDLE_MENUBUTTON || p.m_hCurItem!=hArea));   // select new item.
		p.m_hPushedArea = nullptr;
		ReleaseCapture();
			// 
		if(changedSelect)
		{	if(hArea==Private::HANDLE_MENUBUTTON)
			{	if(p.m_pNotifyMngr)
					p.m_pNotifyMngr->OnMenuButtonClicked(this,&p.m_rcButtonMenu);
			}
			else
				if(!p.m_pAbilityMngr || p.m_pAbilityMngr->CanSelect(this,hArea))
				{	SelectItem(hArea);
					::SetFocus(p.HandleToItem(hArea)->wnd);
						// 
					if(p.m_pNotifyMngr)
						p.m_pNotifyMngr->OnSelectionChanged(this);
				}
		}
		Invalidate(FALSE);
	}
		// 
	CWnd::OnLButtonUp(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::OnMButtonDown(UINT nFlags, CPoint point)
{	p.SetFocusToChild();
		// 
	CWnd::OnMButtonDown(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{	p.SetFocusToChild();
		// 
	CWnd::OnRButtonDown(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::OnRButtonUp(UINT nFlags,CPoint point)
{	if(p.m_pNotifyMngr)
		p.m_pNotifyMngr->OnRightButtonReleased(this,point);
		// 
	CWnd::OnRButtonUp(nFlags,point);
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
{	if(p.m_hPushedArea==Private::HANDLE_SPLITTER)
	{	const float fdelta = static_cast<float>(point.y - p.m_ptSplitterDrag.y) / static_cast<float>(GetStripeHeight() + 1/*border*/);
		int delta = static_cast<int>(fdelta<0 ? fdelta-0.5f : fdelta+0.5f);
		if(p.m_Layout==Layout3 || p.m_Layout==Layout4)
			delta = -delta;
			// 
		if(!IsMenuButtonVisible() && IsEmptyButtonsAreaHide())
			if(p.m_iSplitterDragStartCountStripes == GetTotalNumberVisibleItems() ||
				GetTotalNumberVisibleItems() - (p.m_iSplitterDragStartCountStripes-delta) == 1)
				if(delta)
					(delta<0 ? --delta : ++delta);
			// 
		int needCountStripes = p.m_iSplitterDragStartCountStripes - delta;
		if(needCountStripes<0)
			needCountStripes = 0;
		if(needCountStripes>GetTotalNumberVisibleItems())
			needCountStripes = GetTotalNumberVisibleItems();
			// 
		if(needCountStripes!=GetNumberVisibleStripes())
		{	SetNumberVisibleItemsInStripeState(needCountStripes);
			Update();
		}
	}
	else
	{	const HANDLE hHighlightedArea = p.HitTestEx(point);
			// 
		if(hHighlightedArea)
		{	const bool changeHover = p.m_hHighlightedArea==nullptr;
				// 
			if(hHighlightedArea!=p.m_hHighlightedArea)
			{	p.m_hHighlightedArea = hHighlightedArea;
				Invalidate(FALSE);
			}
				// 
			if(changeHover)
				p.m_iTimerId = SetTimer(1,10,nullptr);
		}
	}
		// 
	CWnd::OnMouseMove(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
//
void OutlookTabCtrl::OnTimer(UINT_PTR nIDEvent)
{	assert(p.m_iTimerId);
		// 
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
		// 
	HANDLE item = p.HitTestEx(pt);
		// 
	if(item==nullptr || item==Private::HANDLE_SPLITTER)
	{	KillTimer(p.m_iTimerId);
		p.m_iTimerId = 0;
		p.m_hHighlightedArea = nullptr;
		Invalidate(FALSE);
	}
		// 
	CWnd::OnTimer(nIDEvent);
}
/////////////////////////////////////////////////////////////////////////////
//
void OutlookTabCtrl::OnCaptureChanged(CWnd *pWnd)
{	if(pWnd!=this)
	{	if(p.m_iTimerId)
		{	KillTimer(p.m_iTimerId);
			p.m_iTimerId = 0;
		}
			// 
		p.m_hHighlightedArea = nullptr;
		p.m_hPushedArea = nullptr;
		Invalidate(FALSE);
	}
		// 
	CWnd::OnCaptureChanged(pWnd);
}
/////////////////////////////////////////////////////////////////////////////
// 
LRESULT OutlookTabCtrl::OnNcHitTest(CPoint point)
{	CPoint pt(point);
	ScreenToClient(&pt);
	if( p.m_rcSplitter.PtInRect(pt) )
		return static_cast<LRESULT>(Private::HTSplitter);
	return CWnd::OnNcHitTest(point);
}
/////////////////////////////////////////////////////////////////////////////
// 
BOOL OutlookTabCtrl::OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message)
{
#ifdef _AFX_USING_CONTROL_BARS
	if( CMFCPopupMenu::GetActiveMenu()==nullptr )
#endif
		if(p.m_bActiveSplitter && (nHitTest & 0x0000ffff)==Private::HTSplitter)
		{	::SetCursor(p.m_hCurDrag ? p.m_hCurDrag : ::LoadCursor(nullptr,IDC_SIZENS));
			return TRUE;
		}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
OutlookTabCtrl::Private::Item *OutlookTabCtrl::Private::HandleToItem(HANDLE h)
{	return static_cast<Item *>(h);
}
// 
OutlookTabCtrl::Private::Item const *OutlookTabCtrl::Private::HandleToItem(HANDLE h) const
{	return static_cast<Item const *>(h);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::SaveState(CWinApp *app, TCHAR const *section, TCHAR const *entry) const
{	assert(app && section && entry);
		// 
	CMemFile file;
	CArchive ar(&file,CArchive::store);
	if( !SaveState(&ar) )
		return false;
	ar.Flush();
	ar.Close();
		// 
	const UINT uDataSize = static_cast<UINT>( file.GetLength() );
	BYTE *pData = file.Detach();
	const bool res = app->WriteProfileBinary(section,entry,pData,uDataSize)!=0;
	free(pData);
	return res;
}
// 
bool OutlookTabCtrl::LoadState(CWinApp *app, TCHAR const *section, TCHAR const *entry)
{	assert(app && section && entry);
		//
	bool res = false;
	BYTE *pData = nullptr;
	UINT uDataSize;
		// 
	try
	{	if( app->GetProfileBinary(section,entry,&pData,&uDataSize) )
		{	CMemFile file(pData,uDataSize);
			CArchive ar(&file,CArchive::load);
			res = LoadState(&ar);
		}
	}
	catch(CMemoryException* pEx)
	{	pEx->Delete();
	}
	if(pData)
		delete [] pData;
		// 
	return res;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::LoadState(CArchive *ar)
{	try
	{	return p.LoadStateInner(ar);
	}
	catch(CMemoryException* pEx)
	{	pEx->Delete();
	}
	catch(CArchiveException* pEx)
	{	pEx->Delete();
	}
	catch(...)
	{
	}
	return false;
}
// 
bool OutlookTabCtrl::SaveState(CArchive *ar) const
{	try
	{	return p.SaveStateInner(ar);
	}
	catch(CMemoryException* pEx)
	{	pEx->Delete();
	}
	catch(CArchiveException* pEx)
	{	pEx->Delete();
	}
	catch(...)
	{
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::Private::LoadStateInner(CArchive *ar)
{	if(m_hCurItem)
		::ShowWindow(HandleToItem(m_hCurItem)->wnd,SW_HIDE);
		// 
	int countItems, id, curItem;
	std::vector<Item *> items;
		// 
	*ar >> countItems;
	assert(countItems==o.GetTotalNumberItems());   // not same number items (in the control and in the registry).
	*ar >> curItem;
		// 
	for(int i=0; i<countItems; ++i)
	{	*ar >> id;
			// 
		const HANDLE item = GetItemByWindowID(id);   // search for an element whose window has an id.
		assert(item);
		if(!item)
			return false;
			// 
		items.push_back( HandleToItem(item) );
		*items.back() >> *ar;
	}
		// 
	m_items = items;
	m_hCurItem = (curItem>=0 ? m_items[curItem] : nullptr);
		// 
	Recalc();
	o.Invalidate(FALSE);
		// 
	return true;
}
// 
bool OutlookTabCtrl::Private::SaveStateInner(CArchive *ar) const
{	*ar << o.GetTotalNumberItems();   // total number (visible + invisible).
	*ar << o.GetItemIndexByHandle(m_hCurItem);
		// 
	for(ci_item i=m_items.begin(); i!=m_items.end(); ++i)
	{	*ar << ::GetDlgCtrlID((*i)->wnd);
		**i << *ar;
	}
		// 
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrl::Private::Item::operator<<(CArchive &ar)   // save.
{	ar << static_cast<BYTE>(button);
	ar << static_cast<BYTE>(visible);
}
// 
void OutlookTabCtrl::Private::Item::operator>>(CArchive &ar)	// load.
{	BYTE val;
	ar >> val;
	button = val!=0;
	ar >> val;
	visible = val!=0;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrl::Private::LoadImage(HMODULE moduleRes/*or null*/, UINT resID, bool pngImage, Gdiplus::Bitmap **bmp/*out*/) const
{	assert(resID);
	assert(bmp);
		// 
	*bmp = nullptr;
		// 
	if(!moduleRes)
		moduleRes = AfxFindResourceHandle(MAKEINTRESOURCE(resID),(pngImage ? _T("PNG") : RT_BITMAP));
	if(moduleRes)
	{	if(!pngImage)   // bmp.
			*bmp = ::new (std::nothrow) Gdiplus::Bitmap(moduleRes,MAKEINTRESOURCEW(resID));
		else   // png.
		{	HRSRC rsrc = ::FindResource(moduleRes,MAKEINTRESOURCE(resID),_T("PNG"));
			if(rsrc)
			{	HGLOBAL rsrcMem = ::LoadResource(moduleRes,rsrc);
				if(rsrcMem)
				{	const void *rsrcBuffer = ::LockResource(rsrcMem);
					if(rsrcBuffer)
					{	const UINT rsrcSize = static_cast<UINT>( ::SizeofResource(moduleRes,rsrc) );
						HGLOBAL streamMem = ::GlobalAlloc(GMEM_MOVEABLE,rsrcSize);
						if(streamMem)
						{	void *streamBuffer = ::GlobalLock(streamMem);
							if(streamBuffer)
							{	memcpy(streamBuffer,rsrcBuffer,rsrcSize);
								::GlobalUnlock(streamBuffer);
									// 
								IStream *stream = nullptr;
								if(::CreateStreamOnHGlobal(streamMem,FALSE,&stream/*out*/)==S_OK)
								{	*bmp = ::new (std::nothrow) Gdiplus::Bitmap(stream,FALSE);
									stream->Release();
								}
							}
							::GlobalFree(streamMem);
						}
						::UnlockResource(rsrcMem);
					}
					::FreeResource(rsrcMem);
				}
			}
		}
	}
	if(*bmp && (*bmp)->GetLastStatus()!=Gdiplus::Ok)
	{	::delete *bmp;
		*bmp = nullptr;
		return false;
	}
	return (*bmp)!=nullptr;
}
/////////////////////////////////////////////////////////////////////////////
//
bool OutlookTabCtrl::Private::CreateImageList(Gdiplus::Bitmap *bmp, int imageWidth, 
	COLORREF clrMask/*or CLR_NONE*/, COLORREF clrBack/*or CLR_NONE*/, CImageList *imageList/*out*/) const
{
	assert(bmp);
	assert(imageWidth>0);
	assert(imageList);
		// 
	bool res = false;
	const Gdiplus::Rect rect(0,0,bmp->GetWidth(),bmp->GetHeight());
	if( imageList->Create(imageWidth,bmp->GetHeight(),ILC_COLOR24 | ILC_MASK,1,0) )
	{	Gdiplus::BitmapData bmpData;
		if(bmp->LockBits(&rect,Gdiplus::ImageLockModeRead,PixelFormat32bppARGB,&bmpData/*out*/)==Gdiplus::Ok)
		{	const int sizeBuffer = abs(bmpData.Stride) * static_cast<int>(bmpData.Height);
			char *buffer = ::new (std::nothrow) char[sizeBuffer];
			if(buffer)
			{	memcpy(buffer,bmpData.Scan0,sizeBuffer);
					// 
				CBitmap imageListBitmap;
				if( imageListBitmap.CreateBitmap(rect.Width,rect.Height,1,32,nullptr) )
				{	const UINT maskRGB = (clrMask & 0x0000ff00) | (clrMask & 0xff)<<16 | (clrMask & 0x00ff0000)>>16;
					const UINT pixelNumber = bmpData.Width * bmpData.Height;
					UINT32 *ptr = reinterpret_cast<UINT32 *>(buffer);
					for(UINT32 *e=ptr+pixelNumber; ptr!=e; ++ptr)
					{	const unsigned char a = static_cast<unsigned char>(*ptr >> 24);
						if(a==0)
							*ptr = maskRGB;
						else if(a==255)
						{	if(clrMask!=CLR_NONE)
								if((*ptr & 0x00ffffff)==maskRGB)
									*ptr = maskRGB;
						}
						else   // a!=255.
							if(clrBack!=CLR_NONE)
							{	const UINT _a = 255u - a;
								const UINT r = ((*ptr & 0xff) * a + (clrBack>>16 & 0xff) * _a) / 255u;
								const UINT g = ((*ptr>>8 & 0xff) * a + (clrBack>>8 & 0xff) * _a) / 255u;
								const UINT b = ((*ptr>>16 & 0xff) * a + (clrBack & 0xff) * _a) / 255u;
								*ptr = r | (g<<8) | (b<<16);
							}
					}
					imageListBitmap.SetBitmapBits(pixelNumber*4,buffer);
						// 
					res = imageList->Add(&imageListBitmap,clrMask & 0x00ffffff)!=-1;
				}
				::delete [] buffer;
			}
			bmp->UnlockBits(&bmpData);
		}
	}
	if(!res && imageList->m_hImageList)
		imageList->DeleteImageList();
	return res;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// OutlookTabCtrlCustom1.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
const int OutlookTabCtrlCustom1::m_iMenuImageWidth = 8;
const int OutlookTabCtrlCustom1::m_iMenuImageHeight = 13;
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom1::Install(OutlookTabCtrl *ctrl)
{	ctrl->SetToolTipManager(this);
	ctrl->SetDrawManager(this);
	ctrl->SetRecalcManager(this);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
CToolTipCtrl *OutlookTabCtrlCustom1::CreateToolTip(OutlookTabCtrl *ctrl)
{
	#ifdef AFX_TOOLTIP_TYPE_ALL   // for MFC Feature Pack.
		CToolTipCtrl *tooltip = nullptr;
		return (CTooltipManager::CreateToolTip(tooltip/*out*/,ctrl,AFX_TOOLTIP_TYPE_TAB) ? tooltip : nullptr);
	#else
		CToolTipCtrl *toolTip = nullptr;
		try
		{	toolTip = new CToolTipCtrl;
		}
		catch(std::bad_alloc &)
		{	return nullptr;
		}
		if( !toolTip->Create(ctrl,TTS_ALWAYSTIP | TTS_NOPREFIX) )
		{	delete toolTip;
			return nullptr;
		}
			// 
		DWORD dwClassStyle = ::GetClassLong(toolTip->m_hWnd,GCL_STYLE);
		dwClassStyle |= 0x00020000/*CS_DROPSHADOW*/;   // enables the drop shadow effect.
		::SetClassLong(toolTip->m_hWnd,GCL_STYLE,dwClassStyle);
		return toolTip;
	#endif
}
// 
void OutlookTabCtrlCustom1::DestroyToolTip(CToolTipCtrl *tooltip)
{	
	#ifdef AFX_TOOLTIP_TYPE_ALL   // for MFC Feature Pack.
		CTooltipManager::DeleteToolTip(tooltip);
	#else
		delete tooltip;
	#endif
}
/////////////////////////////////////////////////////////////////////////////
// 
bool OutlookTabCtrlCustom1::HasButtonTooltip(OutlookTabCtrl const *ctrl, HANDLE item)
{	if( !ctrl->IsButtonTextVisible() )
		return true;
		// 
	CRect rc = ctrl->GetItemRect(item);
		// 
	if( ctrl->IsButtonSeparatorVisible() )
	{	const bool separator = (ctrl->GetButtonsAlign()==OutlookTabCtrl::ButtonsAlignRight ?
			ctrl->GetTotalNumberVisibleItems()-ctrl->GetVisibleItemIndexByHandle(item) < ctrl->GetNumberVisibleButtons() : 
			ctrl->GetVisibleItemIndexByHandle(item) != ctrl->GetTotalNumberVisibleItems()-1);
		rc.DeflateRect(2+separator,0,2,0);
	}
		// 
	Gdiplus::Bitmap *pBmpNorm, *pBmpDis;
	ctrl->GetButtonsImages(&pBmpNorm/*out*/,&pBmpDis/*out*/);
	int imageButton;
	ctrl->GetItemImage(item,nullptr,&imageButton/*out*/);
	const bool drawIcon = (imageButton!=-1 && 
			((!ctrl->IsItemDisabled(item) && pBmpNorm) || 
			(ctrl->IsItemDisabled(item) && pBmpDis)));
	if(drawIcon)
	{	CSize szImage;
		ctrl->GetImagesSize(nullptr,&szImage);
		rc.left += GetButtonContentLeftMargin() + szImage.cx + GetButtonImageTextGap();
	}
	else
		rc.left += GetButtonContentLeftMargin();
		// 
	rc.right -= 2;
	const CString text = ctrl->GetItemText(item);
	CFont *font = const_cast<OutlookTabCtrl *>(ctrl)->GetButtonsFont();
	return GetTextSize(font,text).cx > rc.Width();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom1::DrawBorder(OutlookTabCtrl const * /*ctrl*/, CDC *dc, CRect const *rect)
{	const COLORREF clr = GetBorderColor();
	dc->Draw3dRect(rect,clr,clr);
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom1::DrawCaption(OutlookTabCtrl const *ctrl, CDC *dc, CRect const *rect)
{	dc->FillSolidRect(rect,GetCaptionColor());
		// 
	HANDLE item = ctrl->GetSelectedItem();
		// 
	if(item)
	{	CRect rc(*rect);
		rc.left += GetCaptionTextLeftMargin();
		rc.right -= 2;
			// 
		const CString text = ctrl->GetItemText(item);
		dc->SetTextColor( GetCaptionTextColor() );
		dc->DrawText(text,rc,DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);
	}
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom1::DrawEmptyWindowsArea(OutlookTabCtrl const * /*ctrl*/, CDC *dc, CRect const *rect)
{	dc->FillSolidRect(rect, GetEmptyWindowsAreaColor() );
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom1::DrawSplitterDots(CDC *dc, CRect const *rect, int count, int size, COLORREF color) const
{	int x = rect->CenterPoint().x - (size*(count+count-1))/2;
	const int y = rect->CenterPoint().y - size/2;
		// 
	for(; count-->0; )
	{	dc->FillSolidRect(x,y,size,size,color);
		x += 2 * size;
	}
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom1::DrawSplitter(OutlookTabCtrl const *ctrl, CDC *dc, CRect const *rect)
{	dc->FillSolidRect(rect, GetSplitterBackColor() );
		// 
	if( ctrl->IsSplitterActive() )
		DrawSplitterDots(dc,rect,6,2, GetSplitterDotsColor() );
		// 
	CPen pen(PS_SOLID,1, GetSeparationLineColor() );
		// 
	CPen *pPenOld = dc->SelectObject(&pen);
	dc->MoveTo(rect->left,rect->top);
	dc->LineTo(rect->right,rect->top);
	dc->MoveTo(rect->left,rect->bottom-1);
	dc->LineTo(rect->right,rect->bottom-1);
	dc->SelectObject(pPenOld);
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom1::DrawStripe(OutlookTabCtrl const *ctrl, CDC *dc, HANDLE item, bool drawSeparator)
{	CRect rc = ctrl->GetItemRect(item);
		// 
		// draw separator (border).
	if(drawSeparator)
	{	CPen penSeparationLine(PS_SOLID,1, GetSeparationLineColor() );
		CPen *pPenOld = dc->SelectObject(&penSeparationLine);
		const OutlookTabCtrl::Layout layout = ctrl->GetLayout();
			// 
		if(layout==OutlookTabCtrl::Layout1 || layout==OutlookTabCtrl::Layout4)
		{	rc.bottom--;
			dc->MoveTo(rc.left,rc.bottom);
			dc->LineTo(rc.right,rc.bottom);
		}
		else	// Layout2 or Layout3.
		{	dc->MoveTo(rc.left,rc.top);
			dc->LineTo(rc.right,rc.top);
			rc.top++;
		}
		dc->SelectObject(pPenOld);
	}
		// 
		// fill background.
	const OutlookTabCtrl::ItemState state = ctrl->GetItemState(item);
	DrawBackground(ctrl,dc,&state,&rc);
		// 
		// draw icon.
	Gdiplus::Bitmap *pBmpNorm, *pBmpDis;
	ctrl->GetStripesImages(&pBmpNorm/*out*/,&pBmpDis/*out*/);
	int imageStripe;
	ctrl->GetItemImage(item,&imageStripe/*out*/,nullptr);
	const bool bDisabled = ctrl->IsItemDisabled(item);
		// 
	rc.left += GetStripeContentLeftMargin();
	if(imageStripe!=-1 && 
		((!bDisabled && pBmpNorm) || (bDisabled && pBmpDis)))
	{
		CSize szImage;
		ctrl->GetImagesSize(&szImage/*out*/,nullptr);
		COLORREF clrTransp;
		ctrl->GetImagesTranspColor(&clrTransp/*out*/,nullptr);
			// 		
		Gdiplus::Bitmap *bmp = (!bDisabled ? pBmpNorm : pBmpDis);
		const CPoint pt( rc.left, rc.top+(rc.Height()-szImage.cy)/2);
		DrawImage(dc, bmp, pt, imageStripe, szImage, clrTransp);
		rc.left += szImage.cx + GetStripeImageTextGap();
	}
		// 
		// draw text.
	rc.right -= 2;
	const CString text = ctrl->GetItemText(item);
	dc->SetTextColor(!ctrl->IsItemDisabled(item) ? GetStripeTextColor(&state) : GetDisabledStripeTextColor(&state));
	dc->DrawText(text,rc,DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom1::DrawButton(OutlookTabCtrl const *ctrl, CDC *dc, HANDLE item)
{	CRect rc = ctrl->GetItemRect(item);
		// 
		// draw separator.
	if( ctrl->IsButtonSeparatorVisible() )
	{	const bool separator = (ctrl->GetButtonsAlign()==OutlookTabCtrl::ButtonsAlignRight ?
			ctrl->GetTotalNumberVisibleItems()-ctrl->GetVisibleItemIndexByHandle(item) < ctrl->GetNumberVisibleButtons() : 
			ctrl->GetVisibleItemIndexByHandle(item) != ctrl->GetTotalNumberVisibleItems()-1);
			// 
		if(separator)
		{	CPen penSeparationLine(PS_SOLID,1, GetSeparationLineColor() );
			CPen *pPenOld = dc->SelectObject(&penSeparationLine);
			dc->MoveTo(rc.left,rc.top+6);
			dc->LineTo(rc.left,rc.bottom-6);
			dc->SelectObject(pPenOld);
		}
		rc.DeflateRect(2+separator,0,2,0);
	}
		// 
		// fill background.
	const OutlookTabCtrl::ItemState state = ctrl->GetItemState(item);
	DrawBackground(ctrl,dc,&state,&rc);
		// 
	Gdiplus::Bitmap *pBmpNorm, *pBmpDis;
	ctrl->GetButtonsImages(&pBmpNorm/*out*/,&pBmpDis/*out*/);
	int imageButton;
	ctrl->GetItemImage(item,nullptr,&imageButton/*out*/);
		// 
	const bool bDisabled = ctrl->IsItemDisabled(item);
	const bool drawIcon = (imageButton!=-1 && ((!bDisabled && pBmpNorm) || (bDisabled && pBmpDis)));
		// 
	if( ctrl->IsButtonTextVisible() )
	{		// draw icon.
		rc.left += GetButtonContentLeftMargin();
		if(drawIcon)
		{	CSize szImage;
			ctrl->GetImagesSize(nullptr,&szImage/*out*/);
			COLORREF clrTransp;
			ctrl->GetImagesTranspColor(nullptr,&clrTransp/*out*/);
				// 
			Gdiplus::Bitmap *bmp = (!bDisabled ? pBmpNorm : pBmpDis);
			const CPoint pt( rc.left, (rc.top+rc.bottom-szImage.cy)/2 );
			DrawImage(dc, bmp, pt, imageButton, szImage, clrTransp);
			rc.left += szImage.cx + GetButtonImageTextGap();
		}
			// 
			// draw text.
		rc.right -= 2;
		dc->SetTextColor(!ctrl->IsItemDisabled(item) ? GetButtonTextColor(&state) : GetDisabledButtonTextColor(&state));
		const CString text = ctrl->GetItemText(item);
		dc->DrawText(text,rc,DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);
	}
	else	// draw icon only.
		if(drawIcon)
		{	CSize szImage;
			ctrl->GetImagesSize(nullptr,&szImage/*out*/);
			COLORREF clrTransp;
			ctrl->GetImagesTranspColor(nullptr,&clrTransp/*out*/);
				// 
			Gdiplus::Bitmap *bmp = (!bDisabled ? pBmpNorm : pBmpDis);
			const CPoint pt( (rc.left+rc.right-szImage.cx)/2, (rc.top+rc.bottom-szImage.cy)/2 );
			DrawImage(dc, bmp, pt, imageButton, szImage, clrTransp);
		}
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom1::DrawButtonsBackground(OutlookTabCtrl const *ctrl, CDC *dc, CRect const *rect)
{	static const OutlookTabCtrl::ItemState state = {false,false,false};
	DrawBackground(ctrl,dc,&state,rect);
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom1::DrawButtonMenu(OutlookTabCtrl const *ctrl, CDC *dc, CRect const *rect)
{	static CBrush brButtonMenu;
		// 
	if(!brButtonMenu.m_hObject)
	{	static const WORD wImageBits[m_iMenuImageHeight] = {0x33,0x99,0xCC,0x99,0x33,0xFF,0xFF,0xFF,0xFF,0xFF,0xC1,0xE3,0xF7};
		CBitmap bmp;
		if( bmp.CreateBitmap(m_iMenuImageWidth,m_iMenuImageHeight,1,1,wImageBits) )
			brButtonMenu.CreatePatternBrush(&bmp);
	}
		// 
	if(brButtonMenu.m_hObject)
	{	const OutlookTabCtrl::ItemState state = ctrl->GetMenuButtonState();
		DrawBackground(ctrl,dc,&state,rect);
			// 
		const int x = rect->left + (rect->Width() - m_iMenuImageWidth)/2;
		const int y = rect->top + (rect->Height() - m_iMenuImageHeight)/2;
			// 
		CDC dc_mem;
		CBitmap bmp_mem;
		if(dc_mem.CreateCompatibleDC(dc) && bmp_mem.CreateCompatibleBitmap(dc,m_iMenuImageWidth,m_iMenuImageHeight))
		{	const COLORREF clrTransp = RGB(20,40,80);
				// 
			dc_mem.SelectObject(&bmp_mem);
			CRect rc(0,0,m_iMenuImageWidth,m_iMenuImageHeight);
			dc_mem.FillSolidRect(&rc,clrTransp);
			dc_mem.SetTextColor( GetMenuButtonImageColor() );
			dc_mem.SetBrushOrg(0,0);
			dc_mem.FillRect(&rc,&brButtonMenu);
			::TransparentBlt(dc->m_hDC,x,y,m_iMenuImageWidth,m_iMenuImageHeight,dc_mem.m_hDC,0,0,m_iMenuImageWidth,m_iMenuImageHeight,clrTransp);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom1::GetHighlightStateOfItem(OutlookTabCtrl const *ctrl, OutlookTabCtrl::ItemState const *state, bool *selectLight/*out*/, bool *selectDark/*out*/)
{	const bool bPushed = ctrl->IsAnyStripeOrButtonPushed();
		// 
	if(selectLight)
		*selectLight = (!bPushed && ((state->selected && !state->highlighted) || (!state->selected && state->highlighted))) || (bPushed && state->selected);
	if(selectDark)
		*selectDark = state->highlighted && ((!bPushed && state->selected) || (bPushed && state->pushed));
}
/////////////////////////////////////////////////////////////////////////////
// 
COLORREF OutlookTabCtrlCustom1::MixingColors(COLORREF src, COLORREF dst, int percent) const
{	const int ipercent = 100 - percent;
	return RGB(
		(GetRValue(src) * percent + GetRValue(dst) * ipercent) / 100,
		(GetGValue(src) * percent + GetGValue(dst) * ipercent) / 100,
		(GetBValue(src) * percent + GetBValue(dst) * ipercent) / 100);
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom1::DrawBackground(OutlookTabCtrl const *ctrl, CDC *dc, OutlookTabCtrl::ItemState const *state, CRect const *rect)
{	bool selectLight, selectDark;
	GetHighlightStateOfItem(ctrl,state,&selectLight/*out*/,&selectDark/*out*/);
		// 
	if(selectLight)
	{	static COLORREF clr = MixingColors(::GetSysColor(COLOR_BTNSHADOW), MixingColors(::GetSysColor(COLOR_HIGHLIGHT),::GetSysColor(COLOR_WINDOW),22), 20);
		dc->FillSolidRect(rect,clr);
	}
	else if(selectDark)
	{	static COLORREF clr = MixingColors(::GetSysColor(COLOR_BTNSHADOW), MixingColors(::GetSysColor(COLOR_HIGHLIGHT),::GetSysColor(COLOR_WINDOW),55), 20);
		dc->FillSolidRect(rect,clr);
	}
	else
	{	static COLORREF clr = ::GetSysColor(COLOR_BTNFACE);
		dc->FillSolidRect(rect,clr);
	}
}
/////////////////////////////////////////////////////////////////////////////
//
CSize OutlookTabCtrlCustom1::GetTextSize(CFont *font, CString const &text) const
{	CWindowDC dc(nullptr);
		// 
	CFont *pOldFont = dc.SelectObject(font);
	const CSize sz = dc.GetTextExtent(text);
	dc.SelectObject(pOldFont);
	return sz;
}
/////////////////////////////////////////////////////////////////////////////
//
void OutlookTabCtrlCustom1::DrawImage(CDC *dc, Gdiplus::Bitmap *bmp, CPoint const &ptDst, int image, CSize const &szSrc, COLORREF clrTransp) const
{	Gdiplus::Graphics gr(dc->m_hDC);
	if(clrTransp==CLR_NONE)
		gr.DrawImage(bmp, ptDst.x,ptDst.y, image*szSrc.cx,0,szSrc.cx,szSrc.cy, Gdiplus::UnitPixel);
	else   // draw with color key.
	{	const Gdiplus::Rect rcDest(ptDst.x,ptDst.y,szSrc.cx,szSrc.cy);
		const Gdiplus::Color clrTr(GetRValue(clrTransp),GetGValue(clrTransp),GetBValue(clrTransp));
		Gdiplus::ImageAttributes att;
		att.SetColorKey(clrTr,clrTr,Gdiplus::ColorAdjustTypeBitmap);
		gr.DrawImage(bmp, rcDest, image*szSrc.cx,0,szSrc.cx,szSrc.cy, Gdiplus::UnitPixel, &att);
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
int OutlookTabCtrlCustom1::GetBorderWidth(OutlookTabCtrl const *ctrl, IRecalc *base) { return base->GetBorderWidth(ctrl,nullptr); }
int OutlookTabCtrlCustom1::GetCaptionHeight(OutlookTabCtrl const *ctrl, IRecalc *base) { return base->GetCaptionHeight(ctrl,nullptr); }
int OutlookTabCtrlCustom1::GetSplitterHeight(OutlookTabCtrl const *ctrl, IRecalc *base) { return base->GetSplitterHeight(ctrl,nullptr); }
int OutlookTabCtrlCustom1::GetStripeHeight(OutlookTabCtrl const *ctrl, IRecalc *base) { return base->GetStripeHeight(ctrl,nullptr); }
int OutlookTabCtrlCustom1::GetButtonHeight(OutlookTabCtrl const *ctrl, IRecalc *base) { return std::max(3+m_iMenuImageHeight+3, base->GetButtonHeight(ctrl,nullptr)); }
int OutlookTabCtrlCustom1::GetMinButtonWidth(OutlookTabCtrl const *ctrl, IRecalc *base) { return base->GetMinButtonWidth(ctrl,nullptr); }
int OutlookTabCtrlCustom1::GetMenuButtonWidth(OutlookTabCtrl const * /*ctrl*/, IRecalc * /*base*/) { return 5 + m_iMenuImageWidth + 5; }
// 
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// OutlookTabCtrlCustom2.
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom2::DrawBackground(OutlookTabCtrl const *ctrl, CDC *dc, OutlookTabCtrl::ItemState const *state, CRect const *rect)
{	bool selectLight, selectDark;
	GetHighlightStateOfItem(ctrl,state,&selectLight/*out*/,&selectDark/*out*/);
		// 
	if(selectLight)
		dc->FillSolidRect(rect, RGB(255,226,155) );
	else if(selectDark)
		dc->FillSolidRect(rect, RGB(255,207,123) );
	else
		dc->FillSolidRect(rect, RGB(240,240,240) );
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// OutlookTabCtrlCustom3.
/////////////////////////////////////////////////////////////////////////////
//
void OutlookTabCtrlCustom3::DrawSplitter(OutlookTabCtrl const *ctrl, CDC *dc, CRect const *rect)
{	DrawGradient(dc,rect,RGB(127,127,127),RGB(71,71,71));
	if( ctrl->IsSplitterActive() )
		DrawSplitterDots(dc, rect, 7,2,1, RGB(10,10,10),RGB(230,230,230));
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom3::DrawBackground(OutlookTabCtrl const *ctrl, CDC *dc, OutlookTabCtrl::ItemState const *state, CRect const *rect)
{	bool selectLight, selectDark;
	GetHighlightStateOfItem(ctrl,state,&selectLight/*out*/,&selectDark/*out*/);
		// 
	if(selectLight)
		DrawGradient(dc,rect,RGB(248,230,182),RGB(241,189,111));
	else if(selectDark)
		DrawGradient(dc,rect,RGB(247,227,171),RGB(225,142,24));
	else
		DrawGradient(dc,rect,RGB(255,255,255),RGB(213,209,201));
}
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom3::DrawGradient(CDC *dc, CRect const *rc, COLORREF clrTop, COLORREF clrBottom) const
{	GRADIENT_RECT gRect = {0,1};
	TRIVERTEX vert[2] = 
	{	{rc->left,rc->top,static_cast<COLOR16>((GetRValue(clrTop) << 8)),static_cast<COLOR16>((GetGValue(clrTop) << 8)),static_cast<COLOR16>((GetBValue(clrTop) << 8)),0},
		{rc->right,rc->bottom,static_cast<COLOR16>((GetRValue(clrBottom) << 8)),static_cast<COLOR16>((GetGValue(clrBottom) << 8)),static_cast<COLOR16>((GetBValue(clrBottom) << 8)),0}
	};
	::GradientFill(dc->m_hDC,vert,2,&gRect,1,GRADIENT_FILL_RECT_V);
}
/////////////////////////////////////////////////////////////////////////////
//
void OutlookTabCtrlCustom3::DrawSplitterDots(CDC *dc, CRect const *rect, int count, int size, int offset, COLORREF clrTopDot, COLORREF clrBottomDot) const
{	int x = rect->CenterPoint().x - (size*(count+count-1))/2;
	const int y = rect->CenterPoint().y - size/2;
		// 
	for(; count-->0; )
	{	dc->FillSolidRect(x+offset,y+offset,size,size,clrBottomDot);
		dc->FillSolidRect(x,y,size,size,clrTopDot);
		x += 2 * size;
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// OutlookTabCtrlCustom4.
/////////////////////////////////////////////////////////////////////////////
// 
void OutlookTabCtrlCustom4::DrawBackground(OutlookTabCtrl const *ctrl, CDC *dc, OutlookTabCtrl::ItemState const *state, CRect const *rect)
{	bool selectLight, selectDark;
	GetHighlightStateOfItem(ctrl,state,&selectLight/*out*/,&selectDark/*out*/);
		// 
	if(selectLight)
		dc->FillSolidRect(rect, RGB(91,113,153) );
	else if(selectDark)
		dc->FillSolidRect(rect, RGB(77,96,130) );
	else
		dc->FillSolidRect(rect, RGB(54,78,111) );
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////










