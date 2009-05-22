/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef _HTTP_SERVER_RESPONSE_H_
#define _HTTP_SERVER_RESPONSE_H_

#include <kroll/kroll.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace ti
{
	class HttpServerResponse : public StaticBoundObject
	{
	public:
		HttpServerResponse(Poco::Net::HTTPServerResponse &response);
		virtual ~HttpServerResponse();
		
	private:
		Poco::Net::HTTPServerResponse& response;

		DECLAREBOUNDMETHOD(SetStatus)
		DECLAREBOUNDMETHOD(SetReason)
		DECLAREBOUNDMETHOD(SetStatusAndReason)
		DECLAREBOUNDMETHOD(SetContentType)
		DECLAREBOUNDMETHOD(SetContentLength)
		DECLAREBOUNDMETHOD(AddCookie)
		DECLAREBOUNDMETHOD(SetHeader)
		DECLAREBOUNDMETHOD(Write)

	};
}

#endif
