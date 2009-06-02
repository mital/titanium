/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */


#include "webkit_policy_delegate.h"
#include "win32_user_window.h"
#include <shellapi.h>
#include <string>

namespace ti {
	Win32WebKitPolicyDelegate::Win32WebKitPolicyDelegate(Win32UserWindow *window_)
		: window(window_), m_refCount(1), m_permissiveDelegate(false)
	{
	}

	// IUnknown
	HRESULT STDMETHODCALLTYPE Win32WebKitPolicyDelegate::QueryInterface(REFIID riid, void** ppvObject)
	{
		*ppvObject = 0;
		if (IsEqualGUID(riid, IID_IUnknown))
			*ppvObject = static_cast<IWebPolicyDelegate*>(this);
		else if (IsEqualGUID(riid, IID_IWebPolicyDelegate))
			*ppvObject = static_cast<IWebPolicyDelegate*>(this);
		else
			return E_NOINTERFACE;

		AddRef();
		return S_OK;
	}

	ULONG STDMETHODCALLTYPE Win32WebKitPolicyDelegate::AddRef(void)
	{
		return ++m_refCount;
	}

	ULONG STDMETHODCALLTYPE Win32WebKitPolicyDelegate::Release(void)
	{
		ULONG newRef = --m_refCount;
		if (!newRef)
			delete this;

		return newRef;
	}

	HRESULT STDMETHODCALLTYPE Win32WebKitPolicyDelegate::decidePolicyForNavigationAction(
		/*[in]*/ IWebView* /*webView*/,
		/*[in]*/ IPropertyBag* actionInformation,
		/*[in]*/ IWebURLRequest* request,
		/*[in]*/ IWebFrame* frame,
		/*[in]*/ IWebPolicyDecisionListener* listener)
	{
		/*
		BSTR url;
		request->URL(&url);

		int navType = 0;
		VARIANT var;
		if (SUCCEEDED(actionInformation->Read(WebActionNavigationTypeKey, &var, 0))) {
			V_VT(&var) = VT_I4;
			navType = V_I4(&var);
		}

		const char* typeDescription;
		switch (navType) {
			case WebNavigationTypeLinkClicked:
				typeDescription = "link clicked";
				break;
			case WebNavigationTypeFormSubmitted:
				typeDescription = "form submitted";
				break;
			case WebNavigationTypeBackForward:
				typeDescription = "back/forward";
				break;
			case WebNavigationTypeReload:
				typeDescription = "reload";
				break;
			case WebNavigationTypeFormResubmitted:
				typeDescription = "form resubmitted";
				break;
			case WebNavigationTypeOther:
				typeDescription = "other";
				break;
			default:
				typeDescription = "illegal value";
		}

		printf("Policy delegate: attempt to load %S with navigation type '%s'\n", url ? url : TEXT(""), typeDescription);

		SysFreeString(url);

		if (m_permissiveDelegate)
			listener->use();
		else
			listener->ignore();

		return S_OK;
		*/

		BSTR u;
		request->URL(&u);
		std::wstring u2(u);
		std::string url;
		url.assign(u2.begin(), u2.end());

		// if url matches a window config, then modify window as needed
		this->window->UpdateWindowForURL(url);

		SysFreeString(u);

		listener->use();

		return S_OK;
	}

    HRESULT STDMETHODCALLTYPE Win32WebKitPolicyDelegate::decidePolicyForNewWindowAction(
        /* [in] */ IWebView *webView,
        /* [in] */ IPropertyBag *actionInformation,
        /* [in] */ IWebURLRequest *request,
        /* [in] */ BSTR frameName,
        /* [in] */ IWebPolicyDecisionListener *listener)
    {
		std::wstring frame(frameName);
		transform(frame.begin(), frame.end(), frame.begin(), tolower);

		if(frame == L"ti:systembrowser")
		{
			BSTR u;
			request->URL(&u);
			std::wstring url(u);

			ShellExecuteW(NULL, L"open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
			listener->ignore();

			SysFreeString(u);
		}
		else
		{
			listener->use();
		}

		return S_OK;
    }

	HRESULT STDMETHODCALLTYPE Win32WebKitPolicyDelegate::decidePolicyForMIMEType(
		/* [in] */ IWebView *webView,
		/* [in] */ BSTR type,
		/* [in] */ IWebURLRequest *request,
		/* [in] */ IWebFrame *frame,
		/* [in] */ IWebPolicyDecisionListener *listener)
	{
		Logger::Get("UI.Win32WebKitPolicyDelegate")->Debug("decidePolicyForMIMEType() not implemented");
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE Win32WebKitPolicyDelegate::unableToImplementPolicyWithError(
		/* [in] */ IWebView *webView,
		/* [in] */ IWebError *error,
		/* [in] */ IWebFrame *frame)
	{
		Logger::Get("UI.Win32WebKitPolicyDelegate")->Debug("unableToImplementPolicyWithError() not implemented");
		return E_NOTIMPL;
	}

}
