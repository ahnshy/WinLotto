/*
Module : HOOKWND.H
Purpose: Defines the interface for an MFC class to implement message hooking before CWnd gets there
Created: PJN / 24-02-1999
History: None

Copyright (c) 1999 by PJ Naughter.  
All rights reserved.

*/

#ifndef __HOOKWND_H__
#define __HOOKWND_H__

#ifndef __AFXMT_H__
#pragma message("CHookWnd class requires afxmt.h in your PCH")
#endif


//Class which implements message hooking before CWnd gets there
class CHookWnd : public CObject
{
public:
//Constructors / Destructors
  CHookWnd();
  ~CHookWnd();

//Other Methods
  BOOL IsHooked() const;
  BOOL FirstInChain() const;
  BOOL LastInChain() const;
  int  MiddleOfChain() const;
  int  SizeOfHookChain() const;

protected:
	DECLARE_DYNAMIC(CHookWnd);

  //Hooking & UnHooking
  void Hook(CWnd* pWnd); 
  void UnHook();

  //When you want to pass on the message from your WindowProc function
  //you can call this function just like MFC.
  LRESULT Default();

  //This is the function you need to override in your derived
  //version of CHookWnd so that you can handle the window messages
  virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);

  //The callback function through which all window messages
  //will be handled when they are hooked by CHookWnd 
  static LRESULT CALLBACK HookProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

  //Functions which manage the HWND -> CHookWnd* mapping
  static void      Remove(CHookWnd* pWnd);
  static void      RemoveAll(HWND hWnd);
  static void      Add(CHookWnd* pWnd);
  static CHookWnd* GetFirstHook(HWND hWnd);

//Data variables
  CWnd*     m_pOriginalWnd;     //The window being hooked
  WNDPROC   m_pOriginalWndProc; //The orignal window procedure
  CHookWnd* m_pNextHook;        //The next hook in the chain
  static CCriticalSection m_cs; //Critical section which is used to make the code thread safe
};



#endif //__HOOKWND_H__