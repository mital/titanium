/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */
#include <kroll/kroll.h>
#include "filesystem_binding.h"
#include "file.h"
#include "async_copy.h"
#include "filesystem_utils.h"
#include "app_config.h"

#ifdef OS_OSX
#include <Cocoa/Cocoa.h>
#elif defined(OS_WIN32)
#include <windows.h>
#include <shlobj.h>
#include <process.h>
#endif

#include <Poco/TemporaryFile.h>
#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/LineEndingConverter.h>
#include <Poco/Exception.h>

namespace ti
{
	FilesystemBinding::FilesystemBinding(Host *host, SharedKObject global) : host(host), global(global), timer(0)
	{
		/**
		 * @tiapi(method=True,name=Filesystem.createTempFile) Creates a temporary file
		 * @tiresult(for=Filesystem.createTempFile,type=object) a File object referencing the temporary file
		 */
		this->SetMethod("createTempFile",&FilesystemBinding::CreateTempFile);
		/**
		 * @tiapi(method=True,name=Filesystem.createTempDirectory) Creates a temporary directory
		 * @tiresult(for=Filesystem.createTempDirectory,type=object) a File object referencing the temporary directory
		 */
		this->SetMethod("createTempDirectory",&FilesystemBinding::CreateTempDirectory);
		/**
		 * @tiapi(method=True,name=Filesystem.getFile) Returns a file path, optionally joining multiple arguments together in an OS specific way
		 * @tiresult(for=Filesystem.getFile,type=object) a File object referencing the file
		 */
		this->SetMethod("getFile",&FilesystemBinding::GetFile);
		/**
		 * @tiapi(method=True,returns=filestream,name=Filesystem.getFileStream) Returns a Filestream object
		 * @tiresult(for=Filesystem.getFileStream,type=object) a Filestream object referencing the file
		 */
		this->SetMethod("getFileStream",&FilesystemBinding::GetFileStream);
		/**
		 * @tiapi(method=True,name=Filesystem.getProgramsDirectory) Returns the programs directory of the current system
		 * @tiresult(for=Filesystem.getProgramsDirectory,type=object) a File object referencing the system programs directory
		 */
		this->SetMethod("getProgramsDirectory",&FilesystemBinding::GetProgramsDirectory);
		/**
		 * @tiapi(method=True,name=Filesystem.getApplicationDirectory) Returns the directory where the application resides
		 * @tiresult(for=Filesystem.getApplicationDirectory,type=object) a File object referencing the directory where the application resides
		 */
		this->SetMethod("getApplicationDirectory",&FilesystemBinding::GetApplicationDirectory);
		/**
		 * @tiapi(method=True,name=Filesystem.getApplicationDataDirectory) Returns the data directory of the application
		 * @tiresult(for=Filesystem.getApplicationDataDirectory,type=object) a File object referencing the data directory of the application
		 */
		this->SetMethod("getApplicationDataDirectory",&FilesystemBinding::GetApplicationDataDirectory);
		/**
		 * @tiapi(method=True,name=Filesystem.getRuntimeHomeDirectory) Returns the directory of where the Titanium runtime files are stored
		 * @tiresult(for=Filesystem.getRuntimeHomeDirectory,type=object) a File object referencing the directory where the Titanium runtime files are stored.
		 */
		this->SetMethod("getRuntimeHomeDirectory",&FilesystemBinding::GetRuntimeHomeDirectory);
		/**
		 * @tiapi(method=True,name=Filesystem.getResourcesDirectory) Returns the resources directory of the application
		 * @tiresult(for=Filesystem.getResourcesDirectory,type=object) a File object referencing the resources directory of the application
		 */
		this->SetMethod("getResourcesDirectory",&FilesystemBinding::GetResourcesDirectory);
		/**
		 * @tiapi(method=True,name=Filesystem.getDesktopDirectory) Returns the system desktop directory
		 * @tiresult(for=Filesystem.getDesktopDirectory,type=object) a File object referencing the system desktop directory
		 */
		this->SetMethod("getDesktopDirectory",&FilesystemBinding::GetDesktopDirectory);
		/**
		 * @tiapi(method=True,name=Filesystem.getDocumentsDirectory) Returns the system documents directory
		 * @tiresult(for=Filesystem.getDocumentsDirectory,type=object) a File object referencing the system documents directory
		 */
		this->SetMethod("getDocumentsDirectory",&FilesystemBinding::GetDocumentsDirectory);
		/**
		 * @tiapi(method=True,name=Filesystem.getUserDirectory) Returns the home directory of the current user
		 * @tiresult(for=Filesystem.getUserDirectory,type=object) a File object referencing the home directory of the current user
		 */
		this->SetMethod("getUserDirectory",&FilesystemBinding::GetUserDirectory);
		/**
		 * @tiapi(method=True,name=Filesystem.getLineEnding) Returns the line ending used by the operating system
		 * @tiresult(for=Filesystem.getLineEnding,type=string) the line ending used by the operating system
		 */
		this->SetMethod("getLineEnding",&FilesystemBinding::GetLineEnding);
		/**
		 * @tiapi(method=True,name=Filesystem.getSeparator) Returns the path separator used by the operating system
		 * @tiresult(for=Filesystem.getSeparator,type=string) the path separator used by the operating system
		 */
		this->SetMethod("getSeparator",&FilesystemBinding::GetSeparator);
		/**
		 * @tiapi(method=True,name=Filesystem.getRootDirectories) Returns the system root directory
		 * @tiresult(for=Filesystem.getRootDirectories,type=list) a File object referencing the system root directory
		 */
		this->SetMethod("getRootDirectories",&FilesystemBinding::GetRootDirectories);
		/**
		 * @tiapi(method=True,name=Filesystem.asyncCopy) Executes an async copy operation
		 * @tiarg(for=Filesystem.asyncCopy,name=paths,type=object) either a path or array of paths to copy from
		 * @tiarg(for=Filesystem.asyncCopy,name=destination,type=object) either a string or file object to copy to
		 * @tiarg(for=Filesystem.asyncCopy,name=callback,type=method) callback to invoke on each copy completion operation
		 * @tiresult(for=Filesystem.asyncCopy,type=object) async copy object
		 */
		this->SetMethod("asyncCopy",&FilesystemBinding::ExecuteAsyncCopy);

		/**
		 * @tiapi(property=True,immutable=True,name=Filesystem.MODE_READ, since=0.3, type=int) File read constant
		 */
		this->Set("MODE_READ", Value::NewInt(MODE_READ));
		/**
		 * @tiapi(property=True,immutable=True,name=Filesystem.MODE_WRITE, since=0.3, type=int) File write constant
		 */
		this->Set("MODE_WRITE", Value::NewInt(MODE_WRITE));
		/**
		 * @tiapi(property=True,immutable=True,name=Filesystem.MODE_APPEND, since=0.3, type=int) File append constant
		 */
		this->Set("MODE_APPEND", Value::NewInt(MODE_APPEND));
	}
	FilesystemBinding::~FilesystemBinding()
	{
		if (this->timer!=NULL)
		{
			this->timer->stop();
			delete this->timer;
			this->timer = NULL;
		}
	}
	void FilesystemBinding::CreateTempFile(const ValueList& args, SharedValue result)
	{
		try
		{
			Poco::TemporaryFile tempFile;
			tempFile.keepUntilExit();
			tempFile.createFile();

			ti::File* jsFile = new ti::File(tempFile.path());
			result->SetObject(jsFile);
		}
		catch (Poco::Exception& exc)
		{
			throw ValueException::FromString(exc.displayText());
		}
	}
	void FilesystemBinding::CreateTempDirectory(const ValueList& args, SharedValue result)
	{
		try
		{
			Poco::TemporaryFile tempDir;
			tempDir.keepUntilExit();
			tempDir.createDirectory();

			ti::File* jsFile = new ti::File(tempDir.path());
			result->SetObject(jsFile);
		}
		catch (Poco::Exception& exc)
		{
			throw ValueException::FromString(exc.displayText());
		}
	}
	void FilesystemBinding::ResolveFileName(const ValueList& args, std::string& filename)
	{
		if (args.at(0)->IsList())
		{
			// you can pass in an array of parts to join
			SharedKList list = args.at(0)->ToList();
			for (size_t c = 0; c < list->Size(); c++)
			{
				std::string arg = list->At(c)->ToString();
				filename = kroll::FileUtils::Join(filename.c_str(), arg.c_str(), NULL);
			}
		}
		else
		{
			// you can pass in vararg of strings which acts like  a join
			for (size_t c = 0; c < args.size(); c++)
			{
				std::string arg = FileSystemUtils::GetFileName(args.at(c));
				filename = kroll::FileUtils::Join(filename.c_str(), arg.c_str(), NULL);
			}
		}
		if (filename.empty())
		{
			throw ValueException::FromString("invalid file type");
		}
	}
	void FilesystemBinding::GetFile(const ValueList& args, SharedValue result)
	{
		std::string filename;
		this->ResolveFileName(args, filename);
		ti::File* file = new ti::File(filename);
		result->SetObject(file);
	}
	void FilesystemBinding::GetFileStream(const ValueList& args, SharedValue result)
	{
		std::string filename;
		this->ResolveFileName(args, filename);
		ti::FileStream* fs = new ti::FileStream(filename);
		result->SetObject(fs);
	}
	void FilesystemBinding::GetApplicationDirectory(const ValueList& args, SharedValue result)
	{
		ti::File* file = new ti::File(host->GetApplication()->path);
		result->SetObject(file);
	}
	void FilesystemBinding::GetApplicationDataDirectory(const ValueList& args, SharedValue result)
	{
		std::string appid = AppConfig::Instance()->GetAppID();
		std::string dir = FileUtils::GetApplicationDataDirectory(appid);
		ti::File* file = new ti::File(dir);
		result->SetObject(file);
	}
	void FilesystemBinding::GetRuntimeHomeDirectory(const ValueList& args, SharedValue result)
	{
		std::string dir = FileUtils::GetSystemRuntimeHomeDirectory();
		ti::File* file = new ti::File(dir);
		result->SetObject(file);
	}
	void FilesystemBinding::GetResourcesDirectory(const ValueList& args, SharedValue result)
	{
		ti::File* file = new ti::File(host->GetApplication()->GetResourcesPath());
		result->SetObject(file);
	}
	void FilesystemBinding::GetProgramsDirectory(const ValueList &args, SharedValue result)
	{
#ifdef OS_WIN32
		std::string dir;
		char path[MAX_PATH];
		if(SHGetSpecialFolderPath(NULL,path,CSIDL_PROGRAM_FILES,FALSE))
		{
			dir.append(path);
		}
#elif OS_OSX
		NSString *fullPath = @"/Applications";
		std::string dir = [fullPath UTF8String];
#elif OS_LINUX
		std::string dir = "/usr/local/bin"; //TODO: this might need to be configurable
#endif
		ti::File* file = new ti::File(dir);
		result->SetObject(file);
	}

	void FilesystemBinding::GetDesktopDirectory(const ValueList& args, SharedValue result)
	{
		std::string dir;

#ifdef OS_WIN32
		char path[MAX_PATH];
		if(SHGetSpecialFolderPath(NULL,path,CSIDL_DESKTOP,FALSE))
		{
			dir.append(path);
		}
#elif OS_OSX
		NSString *fullPath = [@"~/Desktop" stringByExpandingTildeInPath];
		dir = [fullPath UTF8String];
#elif OS_LINUX
		// TODO
#endif
		if (!dir.empty())
		{
			ti::File* file = new ti::File(dir);
			result->SetObject(file);
		}
	}
	void FilesystemBinding::GetDocumentsDirectory(const ValueList& args, SharedValue result)
	{
		std::string dir;

#ifdef OS_WIN32
		char path[MAX_PATH];
		if(SHGetSpecialFolderPath(NULL,path,CSIDL_PERSONAL,FALSE))
		{
			dir.append(path);
		}
#elif OS_OSX
		NSString *fullPath = [@"~/Documents" stringByExpandingTildeInPath];
		dir = [fullPath UTF8String];
#elif OS_LINUX
		// TODO
#endif
		if (dir.empty())
		{
			ti::File* file = new ti::File(dir);
			result->SetObject(file);
		}
	}
	void FilesystemBinding::GetUserDirectory(const ValueList& args, SharedValue result)
	{
		try
		{
			std::string dir = Poco::Path::home().c_str();
			ti::File* file = new ti::File(dir);
			result->SetObject(file);
		}
		catch (Poco::Exception& exc)
		{
			throw ValueException::FromString(exc.displayText());
		}
	}
	void FilesystemBinding::GetLineEnding(const ValueList& args, SharedValue result)
	{
		try
		{
			result->SetString(Poco::LineEnding::NEWLINE_LF.c_str());
		}
		catch (Poco::Exception& exc)
		{
			throw ValueException::FromString(exc.displayText());
		}
	}
	void FilesystemBinding::GetSeparator(const ValueList& args, SharedValue result)
	{
		try
		{
			std::string sep;
			sep += Poco::Path::separator();
			result->SetString(sep.c_str());
		}
		catch (Poco::Exception& exc)
		{
			throw ValueException::FromString(exc.displayText());
		}
	}
	void FilesystemBinding::GetRootDirectories(const ValueList& args, SharedValue result)
	{
		try
		{
			Poco::Path path;

			std::vector<std::string> roots;
			path.listRoots(roots);

			SharedPtr<StaticBoundList> rootList = new StaticBoundList();

			for(size_t i = 0; i < roots.size(); i++)
			{
				ti::File* file = new ti::File(roots.at(i));
				SharedValue value = Value::NewObject((SharedKObject) file);
				rootList->Append(value);
			}

			SharedKList list = rootList;
			result->SetList(list);
		}
		catch (Poco::Exception& exc)
		{
			throw ValueException::FromString(exc.displayText());
		}
	}
	void FilesystemBinding::ExecuteAsyncCopy(const ValueList& args, SharedValue result)
	{
		if (args.size()!=3)
		{
			throw ValueException::FromString("invalid arguments - this method takes 3 arguments");
		}
		std::vector<std::string> files;
		if (args.at(0)->IsString())
		{
			files.push_back(args.at(0)->ToString());
		}
		else if (args.at(0)->IsList())
		{
			SharedKList list = args.at(0)->ToList();
			for (unsigned int c = 0; c < list->Size(); c++)
			{
				SharedValue v = list->At(c);
				std::string s(FileSystemUtils::GetFileName(v));
				files.push_back(s);
			}
		}
		else if (args.at(0)->IsObject())
		{
			SharedKObject bo = args.at(0)->ToObject();
			SharedPtr<File> file = bo.cast<File>();
			if (file.isNull())
			{
				throw ValueException::FromString("invalid type passed as first argument");
			}
			files.push_back(file->GetFilename());
		}
		SharedValue v = args.at(1);
		std::string destination(FileSystemUtils::GetFileName(v));
		SharedKMethod method = args.at(2)->ToMethod();
		SharedKObject copier = new ti::AsyncCopy(this,host,files,destination,method);
		result->SetObject(copier);
		asyncOperations.push_back(copier);
		// we need to create a timer thread that can cleanup operations
		if (timer==NULL)
		{
			this->SetMethod("_invoke",&FilesystemBinding::DeletePendingOperations);
			timer = new Poco::Timer(100,100);
			Poco::TimerCallback<FilesystemBinding> cb(*this, &FilesystemBinding::OnAsyncOperationTimer);
			timer->start(cb);
		}
		else
		{
			this->timer->restart(100);
		}
	}
	void FilesystemBinding::DeletePendingOperations(const ValueList& args, SharedValue result)
	{
		KR_DUMP_LOCATION
		if (asyncOperations.size()==0)
		{
			result->SetBool(true);
			return;
		}
		std::vector<SharedKObject>::iterator iter = asyncOperations.begin();

		while (iter!=asyncOperations.end())
		{
			SharedKObject c = (*iter);
			SharedValue v = c->Get("running");
			bool running = v->ToBool();
			if (!running)
			{
				asyncOperations.erase(iter);
				break;
			}
			iter++;
		}

		// return true to pause the timer
		result->SetBool(asyncOperations.size()==0);
	}
	void FilesystemBinding::OnAsyncOperationTimer(Poco::Timer &timer)
	{
#ifdef OS_OSX
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
#endif
		KR_DUMP_LOCATION
		ValueList args = ValueList();
		SharedKMethod m = this->Get("_invoke")->ToMethod();
		SharedValue result = host->InvokeMethodOnMainThread(m, args);
		if (result->ToBool())
		{
			timer.restart(0);
		}
#ifdef OS_OSX
		[pool release];
#endif
	}
}
