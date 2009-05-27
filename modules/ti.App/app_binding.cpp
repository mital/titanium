/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2008 Appcelerator, Inc. All Rights Reserved.
 */
#include <kroll/kroll.h>
#include <Poco/Environment.h>
#include "app_binding.h"
#include "app_config.h"
#include "Properties/properties_binding.h"

namespace ti
{
	AppBinding::AppBinding(Host *host, SharedKObject global) : host(host),global(global)
	{
		/**
		 * @tiapi(method=True,immutable=True,name=App.getID,since=0.2) Gets the application id
		 * @tiresult(for=App.getID,type=string) returns the id
		 */
		this->SetMethod("getID", &AppBinding::GetID);
		/**
		 * @tiapi(method=True,immutable=True,name=App.getName,since=0.2) Gets the application name
	     * @tiresult(for=App.getName,type=string) returns the name
		 */
		this->SetMethod("getName", &AppBinding::GetName);
		/**
		 * @tiapi(method=True,immutable=True,name=App.getVersion,since=0.2) Gets the application version
		 * @tiresult(for=App.getVersion,type=string) returns the version
		 */
		this->SetMethod("getVersion", &AppBinding::GetVersion);
		/**
		 * @tiapi(method=True,immutable=True,name=App.getPublisher,since=0.4) Gets the application publisher
	     * @tiresult(for=App.getPublisher,type=string) returns the publisher
		 */
		this->SetMethod("getPublisher", &AppBinding::GetPublisher);
		/**
		 * @tiapi(method=True,immutable=True,name=App.getURL,since=0.4) Gets the application url
	     * @tiresult(for=App.getURL,type=string) returns the url for the app
		 */
		this->SetMethod("getURL", &AppBinding::GetURL);
		/**
		 * @tiapi(method=True,immutable=True,name=App.getDescription,since=0.4) Gets the application description
	     * @tiresult(for=App.getDescription,type=string) returns the description for the app
		 */
		this->SetMethod("getDescription", &AppBinding::GetDescription);
		/**
		 * @tiapi(method=True,immutable=True,name=App.getCopyright,since=0.4) Gets the application copyright information
	     * @tiresult(for=App.getCopyright,type=string) returns the copyright for the app
		 */
		this->SetMethod("getCopyright", &AppBinding::GetCopyright);
		/**
		 * @tiapi(method=True,immutable=True,name=App.getGUID,since=0.2) Gets the application globally unique id
	     * @tiresult(for=App.getGUID,type=string) returns the unique id
		 */
		this->SetMethod("getGUID", &AppBinding::GetGUID);
		/**
		 * @tiapi(method=True,immutable=True,name=App.getStreamURL,since=0.4) Gets the application stream URL for the update channel
	     * @tiresult(for=App.getStreamURL,type=string) returns the stream URL
		 */
		this->SetMethod("getStreamURL", &AppBinding::GetStreamURL);
		/**
		 * @tiapi(method=True,immutable=True,name=App.appURLToPath,since=0.2) Returns the full path equivalent of an app: protocol path
		 * @tiresult(for=App.appURLToPath,type=string) returns the path
		 */
		this->SetMethod("appURLToPath", &AppBinding::AppURLToPath);
		
		/**
		 * @tiapi(property=True,immutable=True,type=string,name=App.path,since=0.2) Gets the full path to the application
		 */
#ifdef OS_OSX
		NSString *path = [[NSBundle mainBundle] bundlePath];
		this->Set("path",Value::NewString([path UTF8String]));
#else
		this->Set("path",Value::NewString(host->GetCommandLineArg(0)));
#endif

		/**
		 * @tiapi(property=True,immutable=True,type=string,name=App.version,since=0.2) The Titanium product version
		 */
		SharedValue version = Value::NewString(STRING(PRODUCT_VERSION));
		global->Set("version", version);

		/**
		 * @tiapi(property=True,immutable=True,type=string,name=App.platform,since=0.2) The Titanium platform
		 */
		SharedValue platform = Value::NewString(host->GetPlatform());
		global->Set("platform",platform);

		// skip the first argument which is the filepath to the
		// executable
		SharedKList argList = new StaticBoundList();
		for (int i = 1; i < Host::GetInstance()->GetCommandLineArgCount(); i++) {
			argList->Append(Value::NewString(Host::GetInstance()->GetCommandLineArg(i)));
		}
		SharedValue arguments = Value::NewList(argList);
		/**
		 * @tiapi(property=True,immutable=True,type=list,name=App.arguments,since=0.2) The command line arguments
		 */
		Set("arguments", arguments);

		/**
		 * @tiapi(method=True,immutable=True,name=App.exit,since=0.2) Exits the application
		 */
		this->SetMethod("exit",&AppBinding::Exit);

		/**
		 * @tiapi(method=True,name=App.loadProperties,since=0.2) Loads a properties list from a file path
		 * @tiarg(for=App.loadProperties,type=string,name=path) path to properties file
		 * @tiresult(for=App.loadProperties,type=list) returns the properties as a list
		 */
		this->SetMethod("loadProperties", &AppBinding::LoadProperties);

		/**
		 * @tiapi(method=True,name=App.stdout,since=0.4) Writes to stdout
		 * @tiarg(for=App.stdout,type=string,name=data) data to write
		 */
		this->SetMethod("stdout", &AppBinding::StdOut);
		/**
		 * @tiapi(method=True,name=App.stderr,since=0.4) Writes to stderr
		 * @tiarg(for=App.stderr,type=string,name=data) data to write
		 */
		this->SetMethod("stderr", &AppBinding::StdErr);
	}

	AppBinding::~AppBinding()
	{
	}
	void AppBinding::GetID(const ValueList& args, SharedValue result)
	{
		result->SetString(AppConfig::Instance()->GetAppID().c_str());
	}
	void AppBinding::GetName(const ValueList& args, SharedValue result)
	{
		result->SetString(AppConfig::Instance()->GetAppName().c_str());
	}
	void AppBinding::GetVersion(const ValueList& args, SharedValue result)
	{
		result->SetString(AppConfig::Instance()->GetVersion().c_str());
	}
	void AppBinding::GetPublisher(const ValueList& args, SharedValue result)
	{
		result->SetString(AppConfig::Instance()->GetPublisher().c_str());
	}
	void AppBinding::GetCopyright(const ValueList& args, SharedValue result)
	{
		result->SetString(AppConfig::Instance()->GetCopyright().c_str());
	}
	void AppBinding::GetDescription(const ValueList& args, SharedValue result)
	{
		result->SetString(AppConfig::Instance()->GetDescription().c_str());
	}
	void AppBinding::GetURL(const ValueList& args, SharedValue result)
	{
		result->SetString(AppConfig::Instance()->GetURL().c_str());
	}
	void AppBinding::GetGUID(const ValueList& args, SharedValue result)
	{
		std::string guid = host->GetApplication()->guid;
		result->SetString(guid);
	}
	void AppBinding::Exit(const ValueList& args, SharedValue result)
	{
		host->Exit(args.size()==0 ? 0 : args.at(0)->ToInt());
	}

	static const char *kAppURLPrefix = "Resources";
	void AppBinding::AppURLToPath(const ValueList& args, SharedValue result)
	{
		//FIXME - use FileUtils for this... so we can a common implementation
		
		result->SetString("");

		if (args.size() < 0 || !args.at(0)->IsString())
			return;

//FIXME: take into consider the appid which is in the host position of the URL
		std::string url = std::string(args.at(0)->ToString());
		if (url.find("app://") == 0)
		{
			url = url.substr(6, url.length() - 6);
		}
		std::string path = Poco::Environment::get("KR_HOME", "");

		result->SetString(std::string(path + KR_PATH_SEP + kAppURLPrefix + KR_PATH_SEP + url).c_str());
	}

	void AppBinding::LoadProperties(const ValueList& args, SharedValue result)
	{
		if (args.size() >= 1 && args.at(0)->IsString()) {
			std::string file_path = args.at(0)->ToString();
			SharedKObject properties = new PropertiesBinding(file_path);
			result->SetObject(properties);
		}
	}

	void AppBinding::StdOut(const ValueList& args, SharedValue result)
	{
		for (size_t c=0;c<args.size();c++)
		{
			SharedValue arg = args.at(c);
			const char *s = arg->ToString();
			std::cout << s;
		}
		std::cout << std::endl;
	}

	void AppBinding::StdErr(const ValueList& args, SharedValue result)
	{
		for (size_t c=0;c<args.size();c++)
		{
			SharedValue arg = args.at(c);
			const char *s = arg->ToString();
			std::cerr << s;
		}
		std::cerr << std::endl;
	}

	void AppBinding::GetStreamURL(const ValueList& args, SharedValue result)
	{
		const SharedApplication app = this->host->GetApplication();
		std::string stream = app->stream;
		
		// TODO: switch to HTTPS once the ti.Network XHR supports it
		std::string url = "http://api.appcelerator.net/";
		if (stream == "production")
		{
			url+="p/v1";
		}
		else if (stream == "dev")
		{
			url+="d/v1";
		}
		else if (stream == "test")
		{
			url+="t/v1";
		}
		else
		{
			url+=stream;
			url+="/v1";
		}
		for (size_t c = 0; c < args.size(); c++)
		{
			SharedValue arg = args.at(c);
			if (arg->IsString())
			{
				url+="/";
				url+=arg->ToString();
			}
		}
		result->SetString(url);
	}

}
