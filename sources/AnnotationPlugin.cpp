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

/*-------------------------------------------------------
	Functions
r-------------------------------------------------------*/

/* MyPluginIsEnabled
** ------------------------------------------------------
** Function to control if a menu item should be enabled.
** Return true to enable it, false not to enable it.
*/
ACCB1 ASBool ACCB2 MyPluginIsEnabled(void *clientData) {
    // always enabled.
    return true;

    // this code make it is enabled only if there is a open PDF document.
    /* return (AVAppGetActiveDoc() != NULL); */
}

ACCB1 void ACCB2 MyPluginCommandLoad() {
    // get this plugin's name for display
    ASAtom NameAtom = ASExtensionGetRegisteredName(gExtensionID);
    const char * name = ASAtomGetString(NameAtom);
    char str[256];

    // try to get front PDF document
    AVDoc avDoc = AVAppGetActiveDoc();

    if (avDoc == NULL) {
        // if no doc is loaded, make a message.
        strcat(str, "There is no PDF document loaded in Acrobat.");
    } else {

        // if a PDF is open, get its number of pages
        PDDoc pdDoc = AVDocGetPDDoc(avDoc);

        ASFile fileinfo = PDDocGetFile(pdDoc);

        int fileSize = ASFileGetEOF(fileinfo);

        int numPages = PDDocGetNumPages(pdDoc);
        sprintf(str, "Load - The active PDF document has %d pages.", numPages);

        ASFileSys fileSys = ASGetDefaultFileSys();
        ASPathName volatile fdfPathName = NULL;

        char annotationFile[256];
        sprintf(annotationFile, "/Users/user/Desktop/Annotations/");
        sprintf(annotationFile, "%s%d.fdf", annotationFile, fileSize);

		fdfPathName = ASFileSysCreatePathName(fileSys, ASAtomFromString("Cstring"), annotationFile, 0);

		ASInt32 errorCode;
		ASFile volatile fdfFile = NULL;
		errorCode = ASFileSysOpenFile(fileSys, fdfPathName, ASFILE_READ, (ASFile*)&fdfFile);

		// If we succeeded, save the doc to the file
		if (errorCode == 0) {
			char jsScript[512];
			char basePath[256];
			sprintf(basePath, "C:/Users/user/Desktop/Annotations/");
			sprintf(basePath, "%s%d.fdf", basePath, fileSize);
			sprintf(jsScript, "loadAnnot('%s');", basePath);
			AFExecuteThisScript(pdDoc, jsScript, NULL);
			ASFileClose(fdfFile);
		}
    }

    // display message
    // AVAlertNote(str);

    return;
}


ACCB1 void ACCB2 MyPluginCommandSave() {
    // get this plugin's name for display
    ASAtom NameAtom = ASExtensionGetRegisteredName(gExtensionID);
    const char * name = ASAtomGetString(NameAtom);
    char str[256];

    // try to get front PDF document
    AVDoc avDoc = AVAppGetActiveDoc();

    if (avDoc == NULL) {
        // if no doc is loaded, make a message.
        strcat(str, "There is no PDF document loaded in Acrobat.");
    } else {

        // if a PDF is open, get its number of pages
        PDDoc pdDoc = AVDocGetPDDoc(avDoc);

        ASFile fileinfo = PDDocGetFile(pdDoc);

        int fileSize = ASFileGetEOF(fileinfo);

        int numPages = PDDocGetNumPages(pdDoc);
        sprintf(str, "Save - The active PDF document has %d pages.", numPages);

        ASInt32 test;
        CosDoc fdfDoc = PDDocExportNotes(pdDoc, NULL, NULL, NULL, NULL, NULL, &test);

        ASFileSys fileSys = ASGetDefaultFileSys();
        ASPathName volatile fdfPathName = NULL;

        char annotationFile[256];
        sprintf(annotationFile, "/Users/user/Desktop/Annotations/");
        sprintf(annotationFile, "%s%d.fdf", annotationFile, fileSize);

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

        // Create/Open the file
        errorCode = ASFileSysOpenFile(fileSys, fdfPathName, ASFILE_CREATE | ASFILE_WRITE, (ASFile*)&fdfFile);

        // If we succeeded, save the doc to the file
        if (errorCode == 0) {
            CosDocSaveToFile(fdfDoc, fdfFile, cosSaveFullSave, &params);
            CosDocClose(fdfDoc);
            ASFileClose(fdfFile);
        } else {
            AVAlertNote("Error in saving the fdf file!");
            ASRaise(ASFileError(fileErrOpenFailed));
        }
    }

    // display message

    // AVAlertNote(str);

    return;
}