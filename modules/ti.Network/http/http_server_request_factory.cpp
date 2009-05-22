/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#include "http_server_request.h"
#include "http_server_request_factory.h"

namespace ti
{
	HttpServerRequestFactory::HttpServerRequestFactory(Host *host, SharedKMethod callback) :
		host(host),callback(callback)
	{
	}
	HttpServerRequestFactory::~HttpServerRequestFactory()
	{
	}
	Poco::Net::HTTPRequestHandler* HttpServerRequestFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request)
	{
		return new HttpServerRequest(host,callback,(Poco::Net::HTTPServerRequest &)request);
	}
}