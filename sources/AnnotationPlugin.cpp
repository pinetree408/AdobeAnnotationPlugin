/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/
/** 
\file BasicPlugin.cpp

  - This file implements the functionality of the BasicPlugin.
*********************************************************************/


// Acrobat Headers.
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/
// This plug-in's name, you should specify your own unique name here.
#pragma  message ("Please specify your own UNIQUE plug-in name. Remove this message if you have already done so")
const char* MyPluginExtensionName = "ADBE:Annotation_Plugin";

/* A convenient function to add a menu item for your plugin.
*/
ACCB1 ASBool ACCB2 PluginMenuItem(char* MyMenuItemTitle, char* MyMenuItemName);

/*-------------------------------------------------------
	Functions
r-------------------------------------------------------*/

/* MyPluginSetmenu
** ------------------------------------------------------
**
** Function to set up menu for the plugin.
** It calls a convenient function PluginMenuItem.
** Return true if successful, false if failed.
*/
ACCB1 ASBool ACCB2 MyPluginSetmenu()
{
	// Add a new menu item under Acrobat SDK submenu.
	// The new menu item name is "ADBE:BasicPluginMenu", title is "Basic Plugin".
	// Of course, you can change it to your own.
	return PluginMenuItem("Basic Plugin", "ADBE:BasicPluginMenu"); 
}
	

/**		BasicPlugin project is an Acrobat plugin sample with the minimum code 
	to provide an environment for plugin developers to get started quickly.
	It can help Acrobat APIs' code testing, too.  
		This file implements the functionality of the BasicPlugin. It adds a 
	new menu item that will show a message of some simple information about 
	the plugin and front PDF document. Users can modify and add code in this 
	file only to make a simple plugin of their own.   
		
		  MyPluginCommand is the function to be called when executing a menu.
	This is the entry point for user's code, just add your code inside.

	@see ASExtensionGetRegisteredName
	@see AVAppGetActiveDoc
	@see PDDocGetNumPages
*/ 
ACCB1 void ACCB2 MyPluginCommandSave(void *clientData)
{
	// get this plugin's name for display
	ASAtom NameAtom = ASExtensionGetRegisteredName (gExtensionID);
	const char * name = ASAtomGetString(NameAtom);
	char str[256];
	sprintf(str,"This menu item is added by plugin %s.\n", name);

	// try to get front PDF document 
	AVDoc avDoc = AVAppGetActiveDoc();

	if(avDoc==NULL) {
		// if no doc is loaded, make a message.
		strcat(str,"There is no PDF document loaded in Acrobat.");
	}
	else {

		// ASFile fileinfo = PDDocGetFile(pdDoc);
		// ASFileGetEOF();

		// if a PDF is open, get its number of pages
		PDDoc pdDoc = AVDocGetPDDoc (avDoc);

		ASFile fileinfo = PDDocGetFile(pdDoc);

		int fileSize = ASFileGetEOF(fileinfo);

		int numPages = PDDocGetNumPages (pdDoc);
		sprintf(str, "%sThe active PDF document has %d pages.", str, numPages);

		ASInt32 test;
		CosDoc fdfDoc = PDDocExportNotes(pdDoc, NULL, NULL, NULL, NULL, NULL, &test);

		ASFileSys fileSys = ASGetDefaultFileSys();
		//ASFileSys fileSys = NULL;
		ASPathName volatile fdfPathName = NULL;
		//save annotations

		char annotationFile[256];
		sprintf(annotationFile, "/Users/HCIL/Desktop/Annotations/");
		sprintf(annotationFile, "%s%d.fdf", annotationFile, fileSize);

		//fdfPathName = ASFileSysCreatePathName(fileSys, ASAtomFromString("Cstring"), "/Users/HCIL/Desktop/Annotations/test.fdf", 0);
		fdfPathName = ASFileSysCreatePathName(fileSys, ASAtomFromString("Cstring"), annotationFile, 0);
		ASInt32 errorCode;
		ASFile volatile fdfFile = NULL;
		CosDocSaveParamsRec params;

		// Initialize save params
		memset(&params, 0, sizeof(CosDocSaveParamsRec));
		params.size = sizeof(CosDocSaveParamsRec);
		params.header = "%FDF-1.2";

		if (fdfDoc == NULL) {
			// if there were no annotations, create a blank doent
			fdfDoc = CosDocCreate(0);
		}

		AVAlertNote(annotationFile);

		// Create/Open the file
		errorCode = ASFileSysOpenFile(fileSys, fdfPathName, ASFILE_CREATE | ASFILE_WRITE, (ASFile*)&fdfFile);

		// If we succeeded, save the doc to the file
		if (errorCode == 0) {
			CosDocSaveToFile(fdfDoc, fdfFile, cosSaveFullSave, &params);
			CosDocClose(fdfDoc);
			ASFileClose(fdfFile);
		}
		else {
			AVAlertNote("Error in saving the fdf file!");
			ASRaise(ASFileError(fileErrOpenFailed));
		}
	}

	// display message

	AVAlertNote(str);

	return;
}

ACCB1 void ACCB2 MyPluginCommandLoad(void *clientData)
{
	// get this plugin's name for display
	ASAtom NameAtom = ASExtensionGetRegisteredName(gExtensionID);
	const char * name = ASAtomGetString(NameAtom);
	char str[256];
	sprintf(str, "This menu item is added by plugin test %s.\n", name);

	// try to get front PDF document 
	AVDoc avDoc = AVAppGetActiveDoc();

	if (avDoc == NULL) {
		// if no doc is loaded, make a message.
		strcat(str, "There is no PDF document loaded in Acrobat.");
	}
	else {

		// if a PDF is open, get its number of pages
		PDDoc pdDoc = AVDocGetPDDoc(avDoc);

		ASFile fileinfo = PDDocGetFile(pdDoc);

		int fileSize = ASFileGetEOF(fileinfo);


		int numPages = PDDocGetNumPages(pdDoc);
		sprintf(str, "%sThe active PDF document has %d pages.", str, numPages);

		//ASInt32 test;
		//CosDoc fdfDoc = PDDocExportNotes(pdDoc, NULL, NULL, NULL, NULL, NULL, &test);

		ASFileSys fileSys = ASGetDefaultFileSys();
		ASPathName volatile fdfPathName = NULL;
		//save annotations

		char annotationFile[256];
		sprintf(annotationFile, "/Users/HCIL/Desktop/Annotations/");
		sprintf(annotationFile, "%s%d.fdf", annotationFile, fileSize);

		//fdfPathName = ASFileSysCreatePathName(fileSys, ASAtomFromString("Cstring"), "/Users/HCIL/Desktop/Annotations/test.fdf", 0);
		fdfPathName = ASFileSysCreatePathName(fileSys, ASAtomFromString("Cstring"), annotationFile, 0);
		CosDocOpenParamsRec params;

		// Initialize save params
		memset(&params, 0, sizeof(CosDocOpenParamsRec));
		params.size = sizeof(CosDocOpenParamsRec);
		params.fileSys = fileSys;
		params.pathName = fdfPathName;
		params.headerString = "%FDF-1.2";

		CosDoc testDoc = CosDocOpenWithParams(&params);

		PDDocImportCosDocNotes(pdDoc, testDoc, NULL, NULL, NULL, NULL, NULL, NULL);

		CosDocClose(testDoc);

	}

	// display message
	AVAlertNote(str);

	return;
}

/* MyPluginIsEnabled
** ------------------------------------------------------
** Function to control if a menu item should be enabled.
** Return true to enable it, false not to enable it.
*/
ACCB1 ASBool ACCB2 MyPluginIsEnabled(void *clientData)
{
	// always enabled.
	return true;
	
	// this code make it is enabled only if there is a open PDF document. 
	/* return (AVAppGetActiveDoc() != NULL); */
}

