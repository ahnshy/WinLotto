/////////////////////////////////////////////////////////////////////////
// #20240211 Work Ahnshy :: HtmlParser class
// CHtmlParser class
// Bug Report :: ahnshy@naver.com

#include "stdafx.h"
#include "HtmlParser.h"

#include <comdef.h>
#include <mshtml.h>
#pragma warning(disable : 4146)	//see Q231931 for explaintation

#import <mshtml.tlb> no_auto_exclude
//using namespace MSHTML;

#define _CRT_SECURE_NO_WARNINGS

CHtmlParser::CHtmlParser()
{
	Initialize();
}

CHtmlParser::~CHtmlParser()
{
	::CoUninitialize();
}
BOOL CHtmlParser::Initialize()
{
	::CoInitialize(NULL);
	return TRUE;
}

long CHtmlParser::GetWinsNumber(CString strPath, MapWins& wins)
{
	wins.clear();

	CFile f;

	if (f.Open(strPath, CFile::modeRead|CFile::shareDenyNone))
	{
		CStringA strText;
		f.Read(strText.GetBuffer(f.GetLength()), f.GetLength());
		strText.ReleaseBuffer(f.GetLength());
		f.Close();

		MSHTML::IHTMLDocument2Ptr pDoc;
		MSHTML::IHTMLDocument3Ptr pDoc3;
		MSHTML::IHTMLElementCollectionPtr pCollection;
		MSHTML::IHTMLElementCollectionPtr pChildCollection;
		MSHTML::IHTMLElementPtr pElement;
		MSHTML::IHTMLElementPtr pTdElement;

		HRESULT hr = CoCreateInstance(CLSID_HTMLDocument, NULL, CLSCTX_INPROC_SERVER, IID_IHTMLDocument2, (void**)&pDoc);

		USES_CONVERSION_EX;
		SAFEARRAY* pArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);
		VARIANT *pParam = NULL;
		//bstr_t bsData = A2BSTR(strText);
		hr = SafeArrayAccessData(pArray, (LPVOID*)&pParam);
		pParam->vt = VT_BSTR;
		pParam->bstrVal = A2BSTR(strText);

		hr = pDoc->write(pArray);
		hr = pDoc->close();

		SafeArrayDestroy(pArray);

		pDoc3 = pDoc;
		pCollection = pDoc3->getElementsByTagName(_T("tr"));
		long lCol = 0;
		CString strBuffer;
		for (long i=3; i<pCollection->length; i++)
		{
			lCol = 0;
			
			pElement = pCollection->item(i, (long)0);
			if(pElement != NULL)
			{
				strBuffer.Empty();
				pChildCollection = pElement->Getchildren();
				if (!pChildCollection)
					continue;

				for (long lCol = 0, lColCount = 0 ; lCol < pChildCollection->length ; lCol++)
				{
					pTdElement = pChildCollection->item(lCol, (long)0);
					variant_t varSpan = pTdElement->getAttribute(_T("bgcolor"), 0);
					if (varSpan.vt == VT_BSTR && varSpan.bstrVal != NULL && _tcsicmp((LPCTSTR)varSpan.bstrVal, _T("#ccffff")) == 0)
						continue;

					variant_t var = pTdElement->getAttribute(L"align", 0);					
					if (lColCount < 2  || (var.vt == VT_NULL || var.bstrVal == NULL))
					{
						if (pTdElement != NULL)
						{
							strBuffer.Append((LPCTSTR)pTdElement->GetinnerText());
							strBuffer.Append(_T(","));
							lColCount++;
						}
					}
				}
				CWinsItem* pItem = new CWinsItem();
				pItem->Parse(strBuffer, _T(","));
				wins.insert(make_pair(pItem->GetRound(), pItem));
			}
		}
	}

	return 0;
}