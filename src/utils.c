//  MaCoPiX = Mascot Construnctive Pilot for X
//                                (ActX / Gtk+ Evolution)
//
//
//      utils.c  
//      Utility functions  for  MaCoPiX
//
//                            Copyright 2002-2007  K.Chimari
//                                     http://rosegray.sakura.ne.jp/
//
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
// 
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
//


#include "main.h"

#ifdef USE_WIN32
#include <windows.h>
#include <winnt.h>
#endif

#define BUFFSIZE 200

typedef struct{
  char *buf;
  char *ptr;
}ST_STR;





void copy_file(gchar *src, gchar *dest)
{
  FILE *src_fp, *dest_fp;
  gchar *buf;
  gint n_read;

  if(strcmp(src,dest)==0) return;
  
  buf=g_malloc0(sizeof(gchar)*1024);


  if ((src_fp = fopen(src, "rb")) == NULL) {
    g_print(_("Cannot open copy source file %s"),src);
    exit(1);
  }

  if ((dest_fp = fopen(dest, "wb")) == NULL) {
    g_print(_("Cannot open copy destination file %s"),dest);
    exit(1);
  }

  while (!feof(src_fp)){
    n_read = fread(buf, sizeof(gchar), sizeof(buf), src_fp);
    fwrite(buf, n_read, 1, dest_fp);
  }
  fclose(dest_fp);
  fclose(src_fp);

#ifndef USE_WIN32
  chmod(dest, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
#endif

  g_free(buf);
}
 

#ifdef USE_COMMON
void check_common_dir(void){
    struct stat st;

    if (access(COMMON_DIR, F_OK) == 0) {
	if (stat(COMMON_DIR, &st)<0){
	    fprintf(stderr,_("Cannot read status of directry %s .\n"),COMMON_DIR);
	    exit(1);
	}
	if (! S_ISDIR(st.st_mode)) {
	    fprintf(stderr,_("%s is not directory.\n"),COMMON_DIR);
	    exit(1);
	}
    }
    else{
	fprintf(stderr,_("Creating the common directory \"%s\"\n"),COMMON_DIR);
#ifdef USE_WIN32
	mkdir(COMMON_DIR);
	mkdir(COMMON_DIR PIXDIR);
	mkdir(COMMON_DIR SOUNDDIR);
#else
	mkdir(COMMON_DIR,(S_IRWXU|S_IRWXG|S_IRWXO));
	mkdir(COMMON_DIR PIXDIR,(S_IRWXU|S_IRWXG|S_IRWXO));
	mkdir(COMMON_DIR SOUNDDIR,(S_IRWXU|S_IRWXG|S_IRWXO));
#endif
    }

}

gboolean check_common_dir2(){
  struct stat st;
  mode_t modes;
  
  // Checking Access to PREFIXDIR
  if (stat(PREFIX_DIR, &st)<0){
    return(FALSE);
  }
  else{
    if ((S_ISDIR(st.st_mode)&&
	 (access(PREFIX_DIR, R_OK|W_OK|X_OK)==0))){
      if (stat(COMMON_DIR, &st)<0){
	// COMMON_DIR is NO-existing
	return(TRUE);
      }
      else{
	if ((S_ISDIR(st.st_mode)&&
	     (access(COMMON_DIR, R_OK|W_OK|X_OK)==0))){
	  // COMMON_DIR is existing and permision is OK
	  return(TRUE);
	}
	else{
	  // COMMON_DIR is existing and permision is NG
	  return(FALSE);
	}
      }
    }
    else{
      // PREFIX_DIR is not accesible
	return(FALSE);
    }
  }      
  
}
#endif

#ifdef USE_WIN32
gchar* get_win_home(void){
  gchar *WinPath; 

  WinPath=g_strconcat(getenv("APPDATA"),G_DIR_SEPARATOR_S,"MaCoPiX",NULL);
  if(access(WinPath,F_OK)!=0){
    mkdir(WinPath);
  }

  return(WinPath);
}
#endif

gchar* get_rc_dir(void){
#ifdef USE_WIN32
  return(get_win_home());
#else
  return(g_strconcat(g_get_home_dir(),G_DIR_SEPARATOR_S,USER_DIR,NULL));
#endif
}

#ifdef USE_WIN32
gchar* WindowsVersion()
{
  // Get OS Info for WinXP and 2000 or later
  // for Win9x, OSVERSIONINFO should be used instead of OSVERSIONINFOEX
  OSVERSIONINFOEX osInfo;
  gchar *windowsName;
  static gchar buf[1024];

  windowsName = NULL;
  
  osInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);

  GetVersionEx ((LPOSVERSIONINFO)&osInfo);

  switch (osInfo.dwMajorVersion)
  {
  case 4:
    switch (osInfo.dwMinorVersion)
      {
      case 0:
	if(osInfo.dwPlatformId == VER_PLATFORM_WIN32_NT){
	  windowsName = g_strdup("Windows NT 4.0");
	}
	else{
	  windowsName = g_strdup("Windows 95");
	}
	break;

      case 10:
	windowsName = g_strdup("Windows 98");
	break;

      case 90:
	windowsName = g_strdup("Windows Me");
	break;
      }
    break;

  case 5:
    switch (osInfo.dwMinorVersion)
      {
      case 0:
	windowsName = g_strdup("Windows 2000");
	break;
	    
      case 1:
	windowsName = g_strdup("Windows XP");
	break;
	
      case 2:
	windowsName = g_strdup("Windows Server 2003");
	break;

      }
    break;

  case 6:
    switch (osInfo.dwMinorVersion)
      {
      case 0:
	if(osInfo.wProductType == VER_NT_WORKSTATION)
	  windowsName = g_strdup("Windows Vista");
	else
	  windowsName = g_strdup("Windows Server 2008");
	break;
	
      case 1:
	if(osInfo.wProductType == VER_NT_WORKSTATION)
	  windowsName = g_strdup("Windows 7");
	else
	  windowsName = g_strdup("Windows Server 2008 R2");
	break;

      case 2:
	if(osInfo.wProductType == VER_NT_WORKSTATION)
	  windowsName = g_strdup("Windows 8");
	else
	  windowsName = g_strdup("Windows Server 2012");
	break;

      case 3:
	if(osInfo.wProductType == VER_NT_WORKSTATION)
	  windowsName = g_strdup("Windows 8.1");
	else
	  windowsName = g_strdup("Windows Server 2012 R2");
	break;
      }
    break;

  case 10:
    switch (osInfo.dwMinorVersion)
      {
      case 0:
	if(osInfo.wProductType == VER_NT_WORKSTATION)
	  windowsName = g_strdup("Windows 10");
	else
	  windowsName = g_strdup("Windows Server 2016");
	break;
      }	
    break;
  }

  if(!windowsName) windowsName = g_strdup("Windows UNKNOWN");
  
  //OutPut
  if(osInfo.wServicePackMajor!=0){
    g_snprintf(buf, sizeof(buf),
	       "Microsoft %s w/SP%d (%ld.%02ld.%ld)",
	       windowsName,
	       osInfo.wServicePackMajor,
	       osInfo.dwMajorVersion,
	       osInfo.dwMinorVersion,
	       osInfo.dwBuildNumber);
  }
  else{
    g_snprintf(buf, sizeof(buf),
	       "Microsoft %s (%ld.%02ld.%ld)",
	       windowsName,
	       osInfo.dwMajorVersion,
	       osInfo.dwMinorVersion,
	       osInfo.dwBuildNumber);
  }
  g_free(windowsName);

  return(buf);
}
#endif

void pop_debug_print(const gchar *format, ...)
{
#ifdef POP_DEBUG
        va_list args;
        gchar buf[BUFFSIZE];

        va_start(args, format);
        g_vsnprintf(buf, sizeof(buf), format, args);
        va_end(args);

        fprintf(stderr,"%s", buf);
	fflush(stderr);
#endif
}



gchar* my_dirname(const gchar *file_name){
  return(g_path_get_dirname(file_name));
}

gchar* my_basename(const gchar *file_name){
  return(g_path_get_basename(file_name));
}

gboolean my_main_iteration(gboolean may_block){
#ifdef USE_WIN32
  return(FALSE);
#else
  return(g_main_context_iteration(NULL, may_block));
#endif
}

void my_signal_connect(GtkWidget *widget, 
		       const gchar *detailed_signal,
		       void *func,
		       gpointer data)
{
  g_signal_connect(G_OBJECT(widget),
		   detailed_signal,
		   G_CALLBACK(func),
		   data);
}


#ifndef USE_WIN32
#ifndef USE_OSX
gchar* GetCurrentWMName(GtkWidget *widget){
  GdkScreen *screen = gtk_widget_get_screen(widget);
  //GdkScreen *screen = gdk_screen_get_default();

  return((gchar *)gdk_x11_screen_get_window_manager_name(screen));
}
#endif
#endif

gdouble GetCurrentResolution(void){
  GdkScreen *screen = gdk_screen_get_default();

  return(gdk_screen_get_resolution(screen));
}

