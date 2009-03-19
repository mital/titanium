/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2008 Appcelerator, Inc. All Rights Reserved.
 */
#ifndef _UI_MODULE_H_
#define _UI_MODULE_H_

#include <kroll/kroll.h>

namespace ti {
	class UIBinding;
	class WindowBinding;
	class MenuItem;
	class TrayItem;
	class UserWindow;

	typedef SharedPtr<UserWindow> SharedUserWindow;
	typedef SharedPtr<UIBinding> SharedUIBinding;
}

#ifdef OS_WIN32
// A little disorganization; header include order is very sensitive in win32,
// and the build breaks if this below the other OS_ defines
#include "win32/win32_user_window.h"
#endif

#include <iostream>
#include "window_config.h"
#include "user_window.h"
#include "menu_item.h"
#include "tray_item.h"
#include "ui_binding.h"

#ifdef OS_LINUX
#include "gtk/ui_module_gtk.h"
#include "gtk/script_evaluator.h"

#elif defined(OS_OSX)
#include "osx/ui_module_osx.h"

#elif defined(OS_WIN32)
#include "javascript_module.h"
#include "win32/win32_ui_binding.h"
#endif


namespace ti {

	class UIModule : public kroll::Module
	{
		KROLL_MODULE_CLASS(UIModule)

		public:

		/*
		 * Function: GetResourcePath
		 * Get the real path to a resource given an app:// URL. This
		 * will access the appropriate function in the global object.
		 * This will also just return the URL if the URL passed is
		 * not app://
		 *
		 * Arguments:
		 *  url - the app:// URL to resolve (can also be http://, etc)
		 * Returns: The path to the resource on this system or a
		 *          NULL SharedString on failure.
		 */
		static SharedString GetResourcePath(const char *URL);

		/**
		 * can be called to determine if the URI passed is a local file
		 * URI vs remote scheme (such as HTTP).
		 *
		 */
		static bool IsResourceLocalFile(std::string string);

		static UIModule* GetInstance() { return instance_; }

		void Exiting(int exitcode);
		void Start();
		void LoadUIJavascript(JSContextRef context);

		static void SetMenu(SharedPtr<MenuItem> menu);
		static SharedPtr<MenuItem> GetMenu();
		static void SetContextMenu(SharedPtr<MenuItem> menu);
		static SharedPtr<MenuItem> GetContextMenu();
		static void SetIcon(SharedString icon_path);
		static SharedString GetIcon();
		static void AddTrayItem(SharedPtr<TrayItem>);
		static void ClearTrayItems();
		static void UnregisterTrayItem(TrayItem* item);
		

		protected:
		static SharedBoundObject global;
		static SharedPtr<MenuItem> app_menu;
		static SharedPtr<MenuItem> app_context_menu;
		static SharedString icon_path;
		static std::vector<SharedPtr<TrayItem> > tray_items;
		static UIModule* instance_;

		DISALLOW_EVIL_CONSTRUCTORS(UIModule);

	};
}

#ifdef OS_OSX
  #define TI_FATAL_ERROR(msg) \
  { \
	NSApplicationLoad();	\
	if (msg) NSRunCriticalAlertPanel (@"Application Error",	\
				[NSString stringWithUTF8String:msg],nil,nil,nil);	\
	[NSApp terminate:nil]; \
	 \
  }
#elif OS_WIN32
  #define TI_FATAL_ERROR(msg) \
  { \
	MessageBox(NULL,msg,"Application Error",MB_OK|MB_ICONERROR|MB_SYSTEMMODAL); \
	ExitProcess(1);\
  }
#elif OS_LINUX
  #define TI_FATAL_ERROR(msg) \
  { \
	GtkWidget* dialog = gtk_message_dialog_new (NULL,  \
	                                  GTK_DIALOG_MODAL, \
					  GTK_MESSAGE_ERROR,  \
	                                  GTK_BUTTONS_OK, \
	                                  msg); \
	gtk_dialog_run (GTK_DIALOG (dialog)); \
	gtk_widget_destroy (dialog); \
	exit(1); \
  }
#endif

#endif
