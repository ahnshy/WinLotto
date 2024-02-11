/*
Module : HOOKWND.CPP
Purpose: Defines the implementation for an MFC class to implement message hooking before CWnd gets there
Created: PJN / 24-02-1999
History: None

Copyright (c) 1999 by PJ Naughter.  
All rights reserved.

*/




/////////////////////////////////  Includes  //////////////////////////////////

#include "stdafx.h"
#include "HookWnd.h"




//////////////////////////////// Statics / Macros /////////////////////////////

static LPCTSTR g_pszHookWndData = _T("HookWnd_P");
CCriticalSection CHookWnd::m_cs;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif



////////////////////////////////// Implementation /////////////////////////////

IMPLEMENT_DYNAMIC(CHookWnd, CObject);

CHookWnd::CHookWnd()
{
  //Set up all the member variables to default values
  m_pOriginalWnd = NULL;
  m_pOriginalWndProc = NULL;  
  m_pNextHook = NULL;
}

CHookWnd::~CHookWnd()
{
  ASSERT(m_pOriginalWnd == NULL); //should have been already unhooked
  ASSERT(m_pOriginalWndProc == NULL);
}

void CHookWnd::UnHook()
{                  
  //Use a Crit section to make code thread-safe
  CSingleLock sl(&m_cs, TRUE);

  //Validate our variables
  ASSERT(m_pOriginalWnd); //Must be hooked to unhook

  //Remove the hook
  Remove(this);

  //Reset all the variables
  m_pOriginalWnd = NULL;
  m_pOriginalWndProc = NULL;
  m_pNextHook = NULL;
}

void CHookWnd::Hook(CWnd* pWnd)
{
  //Use a Crit section to make code thread-safe
  CSingleLock sl(&m_cs, TRUE);

  //Validate our variables
  ASSERT(m_pOriginalWnd == NULL);   //Must UnHook first
  ASSERT(pWnd);                     //Must be given a valid CWnd
  ASSERT(pWnd->m_hWnd);             //Must have a window handle
  ASSERT(::IsWindow(pWnd->m_hWnd)); //Must be a real window handle

  //Store away the hooked window
  m_pOriginalWnd = pWnd;

  //Install the hook
  Add(this);
}

//The default implementation of this passes the message onto the 
//next CHookWnd in the chain. If we are the last one in the chain
//then call the original window proc
LRESULT CHookWnd::WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  //If there is another item in the chain then pass it on
  if (m_pNextHook)
    lResult = m_pNextHook->WindowProc(nMsg, wParam, lParam);
  else
  {
    //Pass it back to the original window procedure. Normally it will be 
    //the AfxWndProc callback used by MFC 
    ASSERT(m_pOriginalWndProc);
    lResult = ::CallWindowProc(m_pOriginalWndProc, m_pOriginalWnd->m_hWnd, nMsg, wParam, lParam);
  }

  return lResult;
}

LRESULT CHookWnd::Default()
{
	//Pull out the current message
	MSG& currentMsg = AfxGetThreadState()->m_lastSentMsg;

	//Call CHookWnd::WindowProc directly instead of just WindowProc as
  //otherwise we would get infinite recursion on the virtual function
	return CHookWnd::WindowProc(currentMsg.message, currentMsg.wParam, currentMsg.lParam);
}

LRESULT CALLBACK CHookWnd::HookProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
  //Manage the MFC state if we are being used in a DLL
#ifdef _USRDLL
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

  //Similiar to AfxCallWindowProc we set up the thread state correctly
  //just in case anyone down the chain wants to use it
  MSG& currentMsg = AfxGetThreadState()->m_lastSentMsg;
  MSG previousMsg = currentMsg;
	currentMsg.hwnd = hWnd;
	currentMsg.message = nMsg;
	currentMsg.wParam = wParam;
	currentMsg.lParam = lParam;

  //Convert from the HWND to a CHookWnd and route the message through
  //its WindowProc. In effect what we are doing here is converting from
  //the old fashioned Windows SDK way of doing to the C++ (virtual functions)
  //way of doing things
  CHookWnd* pHook = GetFirstHook(hWnd);
  ASSERT(pHook);

  LRESULT lResult;
  if (nMsg == WM_NCDESTROY)
  {
    //If this is the last window message. Then unhook it and 
    //pass the message onto the original window proc
    WNDPROC wndProc = pHook->m_pOriginalWndProc;
    RemoveAll(hWnd);
    ASSERT(wndProc);
    lResult = ::CallWindowProc(wndProc, hWnd, nMsg, wParam, lParam);
  }
  else
  {
    //Just pass the message onto the C++ WindowProc function
    lResult = pHook->WindowProc(nMsg, wParam, lParam);
  }

  //Restore the MFC message state
	currentMsg = previousMsg;
 
  //Return the result
  return lResult; 
}

BOOL CHookWnd::IsHooked() const
{
  return (m_pOriginalWnd != NULL);
}

BOOL CHookWnd::FirstInChain() const
{
  ASSERT(IsHooked());

  //Lookup the HWND we're hooking and if the item found
  //is this one, there we are the first in the chain
  CHookWnd* pFirst = GetFirstHook(m_pOriginalWnd->m_hWnd);

  return (pFirst == this);
}

BOOL CHookWnd::LastInChain() const
{
  ASSERT(IsHooked());

  //Lookup the HWND we're hooking, then traverse to the end 
  //of the chain and see is that the same one as us
  const CHookWnd* pTemp = GetFirstHook(m_pOriginalWnd->m_hWnd);
  while (pTemp->m_pNextHook)
    pTemp = pTemp->m_pNextHook;

  return (pTemp == this);
}

BOOL CHookWnd::MiddleOfChain() const
{
  ASSERT(IsHooked());
  return (!FirstInChain() && !LastInChain() && (SizeOfHookChain() > 1));
}

int CHookWnd::SizeOfHookChain() const
{
  ASSERT(IsHooked());

  //Run along the linked list to accumulate the size of the   
  int nSize = 1;
  const CHookWnd* pTemp = GetFirstHook(m_pOriginalWnd->m_hWnd);
  while (pTemp->m_pNextHook)
  {
    ++nSize;
    pTemp = pTemp->m_pNextHook;
  }

  return nSize;
}

void CHookWnd::Add(CHookWnd* pWnd)
{
  //Validate out parameters
  ASSERT(pWnd);                                     //Must be given a valid CWnd
  ASSERT(pWnd->m_pOriginalWnd);                     //Must be assigned already
  ASSERT(pWnd->m_pOriginalWnd->m_hWnd);             //Must have a window handle
  ASSERT(::IsWindow(pWnd->m_pOriginalWnd->m_hWnd)); //Must be a real window handle

  //Lookup the HWND we're hooking and if it is found
  //assign it into the NextHook pointer into
  pWnd->m_pNextHook = GetFirstHook(pWnd->m_pOriginalWnd->m_hWnd);

  //Store the pWnd away by using ::SetProp. This differs from DiLascia's implementation
  //as he uses an MFC CMap. It does mean that the code is simpler. This idea is taken
  //from the book "Windows++" written by DiLascia. It's a wonder actually why he didn't
  //use this method
  VERIFY(::SetProp(pWnd->m_pOriginalWnd->m_hWnd, g_pszHookWndData, pWnd));

  if (pWnd->m_pNextHook)
  {
    //Already subclassed, so just copy the wndproc into the current instance
    pWnd->m_pOriginalWndProc = pWnd->m_pNextHook->m_pOriginalWndProc;
  }
  else
  {
    //Not found, subclass the window and store away the original wndproc
    pWnd->m_pOriginalWndProc = (WNDPROC) ::SetWindowLong(pWnd->m_pOriginalWnd->m_hWnd, GWL_WNDPROC, (LONG) HookProc);
  }

  ASSERT(pWnd->m_pOriginalWndProc);
}

void CHookWnd::Remove(CHookWnd* pWnd)
{
  //Validate out parameters
  ASSERT(pWnd);
  ASSERT(pWnd->m_pOriginalWnd);
  HWND hWnd = pWnd->m_pOriginalWnd->GetSafeHwnd();
  ASSERT(hWnd);
  ASSERT(::IsWindow(hWnd));

  //Convert from the SDK HWND to the C++ CHookWnd
  CHookWnd* pHook = GetFirstHook(hWnd);
  ASSERT(pHook); //must have been found

  //The item found is the one we were looking for. This
  //means that it is the first item in the chain. Just replace
  //and make the next item in the chain (if any) the first item
  if (pHook == pWnd)
  {
    if (pHook->m_pNextHook)
    {
      //Store the pWnd away by using ::SetProp. This differs from DiLascia's implementation
      //as he uses an MFC CMap. It does mean that the code is simpler. This idea is taken
      //from the book "Windows++".
      VERIFY(::SetProp(pHook->m_pNextHook->m_pOriginalWnd->m_hWnd, g_pszHookWndData, pHook->m_pNextHook));
    }
    else
    {
      //No next hook, but it was the item found. This means that it is the last item
      //in the chain. In this case we remove the window property 
      VERIFY(::RemoveProp(pHook->m_pOriginalWnd->m_hWnd, g_pszHookWndData));

      //Also restore the original window proc
      ::SetWindowLong(pHook->m_pOriginalWnd->m_hWnd, GWL_WNDPROC, (LONG) pHook->m_pOriginalWndProc);
    }
  }
  else
  {
    //The item is in the middle of the chain, Just remove it from the linked list
    while (pHook->m_pNextHook != pWnd)
      pHook = pHook->m_pNextHook;
    ASSERT(pHook);
    ASSERT(pHook->m_pNextHook == pWnd);
    pHook->m_pNextHook = pWnd->m_pNextHook;
  }
}

void CHookWnd::RemoveAll(HWND hWnd)
{
  ASSERT(hWnd);             //Must have a window handle
  ASSERT(::IsWindow(hWnd)); //Must be a real window handle

  //Remove all the chain of CHookWnd's hanging off the HWND
  CHookWnd* pHook;
  do
  {
    pHook = GetFirstHook(hWnd);
    if (pHook)
      pHook->UnHook();
  }
  while (pHook);
}

CHookWnd* CHookWnd::GetFirstHook(HWND hWnd)
{
  //Get the CHookWnd* back by using the ::GetProp call 
  CHookWnd* pHook = (CHookWnd*) ::GetProp(hWnd, g_pszHookWndData);
  if (pHook == NULL)
    return NULL;

  //Make sure we are not getting trash back
  ASSERT(pHook->IsKindOf(RUNTIME_CLASS(CHookWnd)));

  return pHook;
}
