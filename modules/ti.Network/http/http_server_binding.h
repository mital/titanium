/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef _HTTP_SERVER_BINDING_H_
#define _HTTP_SERVER_BINDING_H_

#include <kroll/kroll.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/Thread.h>
#include <Poco/FileStream.h>

namespace ti
{
	class HTTPServerBinding : public StaticBoundObject
	{
	public:
		HTTPServerBinding(Host* host);
		virtual ~HTTPServerBinding();
		
		SharedValue GetSelf() { return self;}
		
	private:
		Host* host;
		SharedKObject global;
		SharedKMethod callback;
		std::string ipaddress;
		int port;
		Poco::Thread *thread;
		SharedValue self;
		Poco::Net::ServerSocket *socket;
		Poco::Net::HTTPServer *connection;
		
		static void Run(void*);
		
		DECLAREBOUNDMETHOD(Bind)
		DECLAREBOUNDMETHOD(Close)
		DECLAREBOUNDMETHOD(IsClosed)
		
		void Close();
		
	};
}

#endif
