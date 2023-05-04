/*
 * WhiteChoco.c
 *
 *  Created on: Mar 1, 2010
 *      Author: Yama H
 */
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <commctrl.h>
#include "Scanner.h"	// C implementation of a scanner object to read a delimited file
#include "fcopy.h"

// Shop editor ID's
#define IDSC_LIST 				1000
#define IDSC_EDIT				1001
#define IDSC_COMBO				1100
#define IDSC_STATIC				1200

// Price editor ID's
#define IDPC_LIST				1300
#define	IDPC_NAMEEDIT			1301
#define IDPC_PRICEEDIT			1302
#define	IDPC_MMMEDIT			1303
#define	IDPC_STATIC1			1304
#define IDPC_STATIC2			1305
#define IDPC_GROUP				1306

// Other constants
#define IDC_TABCONTROL			9000
#define ID_FILE_EXIT 			9001
#define ID_FILE_LOAD_EXE 		9002
#define ID_FILE_APPLY_CHANGES 	9003
#define ID_FILE_RESTORE_BACKUP 	9004
#define ID_HELP_ABOUT 			9005
#define ID_FILE_SAVE_NAMES		9006
#define ID_FILE_LOAD_MNU		9007
#define ITEM_OFFSET 			0
#define EDGE_BUFFER				5
#define TAB_HEIGHT				20
#define EDITBOXHEIGHT			30
#define EDITBOXWIDTH			250
#define WEAPON_OFFSET 			128
#define ARMOR_OFFSET 			256
#define ACCESSORY_OFFSET 		288
#define MATERIA_OFFSET 			320
#define SHOPS_OFFSET 			416
#define TOTAL_NAMES 			496
#define TOTAL_NAMES_STRING 		"496"
#define LISTYSIZE				520
#define LISTXSIZE				150
#define SHOPS_NUMCOLS			13
#define MAX_NAME_LENGTH 		MAX_TOKEN_LENGTH	// modify in Scanner.h
#define TOTAL_SHOPS 			80
#define SHOPSIZE				84					// size of a single shop in bytes
#define PRICESIZE				4					// size of a single price in bytes

// FF7.exe constants
#define EXE_FILESIZE 			5882880				// size of ff7.exe in bytes
#define EXE_SHOP_OFFSET 		5381656				// address of first shop in the exe
#define EXE_MASTER_MULT1		3273039			// These are both locations of the
#define EXE_MASTER_MULT2		3273118			// master materia multiplier in the exe
#define EXE_ITEM_PRICE_OFFSET	5388376				// address of first item price in the exe
#define EXE_MAT_PRICE_OFFSET	5389912				// ""		""		materia		""
#define EXE_SIGNATURE			5390432				// Executable signature
#define EXE_SIG_LEN				0x3A				// Length of signature in bytes
//#define KERNEL2_FILESIZE		14151				// size of kernel2.bin in bytes

// SHOPMENU.MNU constants
#define MNU_FILESIZE			61892				// size of shopmenu.mnu in bytes
#define MNU_SHOP_OFFSET 		18196				// address of first shop in the mnu
#define MNU_MASTER_MULT1		0					// Unknown
#define MNU_MASTER_MULT2		0					// Unknown
#define MNU_ITEM_PRICE_OFFSET	26708				// address of first item price in the mnu
#define MNU_MAT_PRICE_OFFSET	28244				// ""		""		materia		""
#define MNU_SIGNATURE			17882				// This area should read "SUMTHIN"
//#define PSX_KERNEL_FILESIZE		22376				// size of KERNEL.BIN in bytes

// General Global Vars
const char g_szClassName[] = "WhiteChoco";
const char version[] = "White Choco v0.7b";
const char configfileName[] = "whitechoco.cfg";
char* gamefilePath;
byte* signature;
int mode = -1;
HINSTANCE hInstance;
HWND filePromptDialog, filePromptDialogInput, tabControl;
HMENU hFileMenu;
HFONT smallFont, defaultFont, largeFont;
char** itemNames;
byte* data;
/*
 * 0 when no file is loaded
 * 1 when ff7.exe is loaded
 * 2 when shopmenu.mnu is loaded
 * 3 when an international or modified version of ff7.exe is loaded
 */
short int gameFileLoaded = 0;
short int configFileLoaded = 0;
long int offset = 0;
long unsigned int filesize = EXE_FILESIZE;

// Shop editor Global vars
HWND shopSelector, shopNameEditBox, typeSelector, inventorySelector;
HWND* itemSelectors;
HWND* shopsTexts;
int shopIndex = -1;

// Price editor Global vars
HWND priceSelector, itemNameEditBox, priceEditBox, masterMultEditBox, masterMultGroupCtrl,
	itemNameLabel, priceLabel;
int priceIndex = -1;

/*
 * Translates a char of FF7Text to standard unicode.
 * Code - char to translate
 *
unsigned char translateFF7Text(unsigned char Code)
{
	if(Code < 96)
		return (char)(Code+32);
	else
	{
		switch((int)Code)
		{
			case 0x60 : return 'Ä';
			case 0x61 : return 'Á';
			case 0x62 : return 'Ç';
			case 0x63 : return 'É';
			case 0x64 : return 'Ñ';
			case 0x65 : return 'Ö';
			case 0x66 : return 'Ü';
			case 0x67 : return 'á';
			case 0x68 : return 'à';
			case 0x69 : return 'â';
			case 0x6A : return 'ä';
			case 0x6B : return 'ã';
			case 0x6C : return 'å';
			case 0x6D : return 'ç';
			case 0x6E : return 'é';
			case 0x6F : return 'è';
			case 0x70 : return 'ê';
			case 0x71 : return 'ë';
			case 0x72 : return 'í';
			case 0x73 : return 'ì';
			case 0x74 : return 'î';
			case 0x75 : return 'ï';
			case 0x76 : return 'ñ';
			case 0x77 : return 'ó';
			case 0x78 : return 'ò';
			case 0x79 : return 'ô';
			case 0x7A : return 'ö';
			case 0x7B : return 'õ';
			case 0x7C : return 'ú';
			case 0x7D : return 'ù';
			case 0x7E : return 'û';
			case 0x7F : return 'ü';
			case 0x80 : return 'Ð';
			case 0x81 : return '°';
			case 0x82 : return '¢';
			case 0x83 : return '£';
			case 0x84 : return 'Ù';
			case 0x85 : return 'Û';
			case 0x86 : return '¶';
			case 0x87 : return 'ß';
			case 0x88 : return '®';
			case 0x89 : return '©';
			case 0x8A : return '™';
			case 0x8B : return '´';
			case 0x8C : return '¨';
			case 0x8D : return '?';
			case 0x8E : return 'Æ';
			case 0x8F : return 'Ø';
			case 0x90 : return '8';
			case 0x91 : return '±';
			case 0x92 : return '=';
			case 0x93 : return '=';
			case 0x94 : return '¥';
			case 0x95 : return 'µ';
			case 0x96 : return '?';
			case 0x97 : return 'S';
			case 0x98 : return '?';
			case 0x99 : return 'p';
			case 0x9A : return ')';
			case 0x9B : return 'ª';
			case 0x9C : return 'º';
			case 0x9D : return 'O';
			case 0x9E : return 'æ';
			case 0x9F : return 'ø';
			case 0xA0 : return '¿';
			case 0xA1 : return '¡';
			case 0xA2 : return '¬';
			case 0xA3 : return 'v';
			case 0xA4 : return 'ƒ';
			case 0xA5 : return '˜';
			case 0xA6 : return '?';
			case 0xA7 : return '«';
			case 0xA8 : return '»';
			case 0xA9 : return '…';
			case 0xAA : return 'À';
			case 0xAB : return 'Ã';
			case 0xAC : return 'Õ';
			case 0xAD : return 'Œ';
			case 0xAE : return 'œ';
			case 0xAF : return '0';
			case 0xB0 : return '–';
			case 0xB1 : return '—';
			case 0xB2 : return (char)147;	// case 0xB2 : return '“'
			case 0xB3 : return (char)148;	// case 0xB3 : return '”'
			case 0xB4 : return '‘';
			case 0xB5 : return '’';
			case 0xB6 : return '÷';
			case 0xB7 : return '?';
			case 0xB8 : return 'ÿ';
			case 0xB9 : return 'Ÿ';
			case 0xBA : return '/';
			case 0xBB : return '¤';
			case 0xBC : return '‹';
			case 0xBD : return '›';
			case 0xBE : return '?';
			case 0xBF : return '?';
			case 0xC0 : return '¦';
			case 0xC1 : return '?';
			case 0xC2 : return '‚';
			case 0xC3 : return '„';
			case 0xC4 : return '‰';
			case 0xC5 : return 'Â';
			case 0xC6 : return 'Ê';
			case 0xC7 : return 'Á';
			case 0xC8 : return 'Ë';
			case 0xC9 : return 'È';
			case 0xCA : return 'í';
			case 0xCB : return 'î';
			case 0xCC : return 'ï';
			case 0xCD : return 'ì';
			case 0xCE : return 'Ó';
			case 0xCF : return 'Ô';
			case 0xD0 : return ' ';
			case 0xD1 : return 'Ò';
			case 0xD2 : return 'Ù';
			case 0xD3 : return 'Û';
			case 0xE1 : return '/t';
			case 0xE2 : return ',';
		}
	}
	return ' ';
}
int readKernel(hwnd, short int psx)
{
	byte* kerneldata;
	if(psx)
		kerneldata = (byte*)malloc(PSX_KERNEL_FILESIZE*sizeof(byte));
	else
		kerneldata = (byte*)malloc(KERNEL2_FILESIZE*sizeof(byte));
	OPENFILENAME kernel;
	kernel.lStructSize = sizeof(OPENFILENAME);
	kernel.hwndOwner = hwnd;
	kernel.hInstance = hInstance;
	kernel.lpstrFilter = NULL;
	kernel.lpstrCustomFilter = NULL;
	kernel.lpstrFile = NULL;
	kernel.nMaxFile = 0;
	kernel.lpstrFileTitle = NULL;
	if(psx)
	{
		kernel.lpstrInitialDir = "%USERPROFILE%";
		kernel.lpstrTitle = "Open KERNEL.BIN";
	}
	else
	{
		kernel.lpstrInitialDir =
			"%ProgramFiles%\\Square Soft, Inc\\Final Fantasy VII\\data\\kernel\\kernel2.bin";
		kernel.lpstrTitle = "Open Kernel2.bin";
	}
	kernel.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOREADONLYRETURN |
		OFN_LONGNAMES | OFN_HIDEREADONLY;
	GetOpenFileName(&kernel);
	FILE* kernel_bin = fopen(gamefilePath, "r");
	if(kernel_bin == NULL)
		return 1;
	if(psx)
	{
		if(fread(kerneldata, 1, PSX_KERNEL_FILESIZE, ff7) != PSX_KERNEL_FILESIZE) // check for a file too small
		{
			int tmpindex = 0;
			// Check file signature
			if(	kerneldata[tmpindex++] == 'z'	&& kerneldata[tmpindex++] == 0 &&
				kerneldata[tmpindex++] == 0 	&& kerneldata[tmpindex++] == 1)
			{
				MessageBox(hwnd,
					"Warning: filesize does not match, but file signature found.",
					"Warning", MB_OK | MB_ICONWARNING);
			}
			else return 2;
		}
	}
	else
	{
		if(fread(kerneldata, 1, KERNEL2_FILESIZE, kernel_bin) != KERNEL2_FILESIZE) // check for a file too small
		{
			int tmpindex = 0;
			// Check file signature
			if(	kerneldata[tmpindex++] == 'C'	&& kerneldata[tmpindex++] == '7' &&
				kerneldata[tmpindex++] == 0 	&& kerneldata[tmpindex++] == 0)
			{
				MessageBox(hwnd,
					"Warning: filesize does not match, but file signature found.",
					"Warning", MB_OK | MB_ICONWARNING);
			}
			else return 2;
		}
	}
	if(fgetc(kernel_bin) != EOF) // check for a file too large
	{
		int tmpindex;
		if(psx)
		{
			// Check file signature
			if(	kerneldata[tmpindex++] == 'z'	&& kerneldata[tmpindex++] == 0 &&
				kerneldata[tmpindex++] == 0 	&& kerneldata[tmpindex++] == 1)
			{
				MessageBox(hwnd,
					"Warning: filesize does not match, but file signature found.",
					"Warning", MB_OK | MB_ICONWARNING);
			}
			else return 2;
		}
		else
		{
			// Check file signature
			if(	kerneldata[tmpindex++] == 'C'	&& kerneldata[tmpindex++] == '7' &&
				kerneldata[tmpindex++] == 0 	&& kerneldata[tmpindex++] == 0)
			{
				MessageBox(hwnd,
					"Warning: filesize does not match, but file signature found.",
					"Warning", MB_OK | MB_ICONWARNING);
			}
			else return 2;
		}
	}
	byte functionplace;
	byte functionlength;
	byte functionloop;
}*/
/*
 * Scans or checks for file signature, return value depends on quickcheck
 * filesize - size of data[] array
 * quickcheck - 1 to check for signature at EXE_SIGNATURE, 0 to scan for it
 * returns 0 on success if quickcheck = 1, returns nonzero offset of file signature
 * on success if quickcheck = 0.
 */
long int scanFileSignature(long unsigned int filesize, short int quickcheck)
{
	int i = 0;
	int j;
	if(quickcheck)
		i = EXE_SIGNATURE;
	while(i < filesize)
	{
		j = 0;
		while(j < EXE_SIG_LEN)
		{
			if(data[i+j] != signature[j])
				break;
			j++;
			if(j == EXE_SIG_LEN)
				return (i - EXE_SIGNATURE);
		}
		if(quickcheck)
			return -1;
		i++;
	}
	return 0;
}
/*
 * Updates current shop data into FF7.exe in memory
 * hwnd - handle to the window
 */
int updateData(HWND hwnd)
{
	long unsigned int data_shopOffset = 0;
	long unsigned int data_masterMult1 = 0;
	long unsigned int data_masterMult2 = 0;
	long unsigned int data_itemPriceOffset = 0;
	long unsigned int data_matPriceOffset = 0;
	switch(gameFileLoaded)
	{
		case 0:
		{
			return 0;
		}
		case 1:
		{
			data_shopOffset = EXE_SHOP_OFFSET;
			data_masterMult1 = EXE_MASTER_MULT1;
			data_masterMult2 = EXE_MASTER_MULT2;
			data_itemPriceOffset = EXE_ITEM_PRICE_OFFSET;
			data_matPriceOffset = EXE_MAT_PRICE_OFFSET;
			break;
		}
		case 2:
		{
			data_shopOffset = MNU_SHOP_OFFSET;
			data_masterMult1 = MNU_MASTER_MULT1;
			data_masterMult2 = MNU_MASTER_MULT2;
			data_itemPriceOffset = MNU_ITEM_PRICE_OFFSET;
			data_matPriceOffset = MNU_MAT_PRICE_OFFSET;
			break;
		}
		case 3:
		{
			data_shopOffset = offset+EXE_SHOP_OFFSET;
			data_masterMult1 = 0;
			data_masterMult2 = 0;
			data_itemPriceOffset = offset+EXE_ITEM_PRICE_OFFSET;
			data_matPriceOffset = offset+EXE_MAT_PRICE_OFFSET;
			break;
		}
	}
	switch(mode)
	{
		case 0:	// Update Shops
		{
			if(shopIndex == -1)
				return 0;
			int tmpIndex = data_shopOffset+shopIndex*SHOPSIZE;
			data[tmpIndex] = (byte)SendMessage(typeSelector, CB_GETCURSEL, 0, 0);
			tmpIndex += 2;
			data[tmpIndex] = (byte)SendMessage(inventorySelector, CB_GETCURSEL, 0, 0);
			tmpIndex += 2;
			int itemIndex = 0;
			int itemNameIndex;
			while(itemIndex < 10)
			{
				itemNameIndex = SendMessage(itemSelectors[itemIndex], CB_GETCURSEL, 0, 0);
				if(itemNameIndex >= MATERIA_OFFSET)
				{
					data[tmpIndex] = (byte)1;
					itemNameIndex -= MATERIA_OFFSET;
				}
				else
					data[tmpIndex] = (byte)0;
				data[tmpIndex+5] = (byte)((int)(itemNameIndex/256));
				data[tmpIndex+4] = (byte)(itemNameIndex%256);
				tmpIndex += 8;
				itemIndex++;
			}
			break;
		}
		case 1: // Update Prices
		{
			char* tmpstr = (char*)malloc(50*sizeof(char));
			if(data_masterMult1 != 0)
			{
				SendMessage(masterMultEditBox, WM_GETTEXT, 50, (LPARAM)tmpstr);
				short unsigned int multiplier;
				sscanf(tmpstr, "%hu", &multiplier);
				data[data_masterMult1] = (byte)multiplier;
				data[data_masterMult2] = (byte)multiplier;
			}
			if(priceIndex == -1)
				return 0;
			int tmpIndex;
			if(priceIndex < MATERIA_OFFSET)
				tmpIndex = data_itemPriceOffset+priceIndex*PRICESIZE;
			else
				tmpIndex = data_matPriceOffset+(priceIndex-MATERIA_OFFSET)*PRICESIZE;
			SendMessage(priceEditBox, WM_GETTEXT, 50, (LPARAM)tmpstr);
			long unsigned int price;
			sscanf(tmpstr, "%lu", &price);
			int i = tmpIndex+4;
			while(tmpIndex < i)
			{
				data[tmpIndex++] = (byte)price%0x100;
				price -= price%0x100;
				price /= 0x100;
			}
			free(tmpstr);
			break;
		}
	}
	return 0;
}
/*
 * Saves changes to name table to winchoco.cfg.
 * hwnd - handle to the window
 */
int saveNameTable(HWND hwnd)
{
	FILE* configfile = fopen(configfileName, "w");
	if (configfile == NULL)
		return 1;
	int i = 0;
	while(i < TOTAL_NAMES)
	{
		fprintf(configfile, itemNames[i++]);
		fprintf(configfile, "\r\n");
	}
	fclose(configfile);
	MessageBox(hwnd, "whitechoco.cfg saved successfully!", "Success!", MB_OK |
		MB_ICONINFORMATION);
	return 0;
}
/*
 * Loads the Master Materia Multiplier data into the window
 * hwnd - handle to the window
 */
int loadMMM(HWND hwnd)
{
	switch(gameFileLoaded)
	{
		case 0:
		case 2:
		case 3:
		{
			return 0;
		}
	}
	char* tmpstr = (char*)malloc(50*sizeof(char));
	sprintf(tmpstr, "%u", data[EXE_MASTER_MULT1]);
	SendMessage(masterMultEditBox, WM_SETTEXT, 0, (LPARAM)tmpstr);
	free(tmpstr);
	return 0;
}
/*
 * Loads data of currently selected item into window and displays it
 * hwnd - handle to the window
 */
int loadPrice(HWND hwnd)
{
	if(priceIndex == -1)
		return 0;
	SendMessage(itemNameEditBox, WM_SETTEXT, 0, (LPARAM)itemNames[priceIndex]);
	long unsigned int data_itemPriceOffset = 0;
	long unsigned int data_matPriceOffset = 0;
	switch(gameFileLoaded)
	{
		case 0:
		{
			return 0;
		}
		case 1:
		{
			data_itemPriceOffset = EXE_ITEM_PRICE_OFFSET;
			data_matPriceOffset = EXE_MAT_PRICE_OFFSET;
			break;
		}
		case 2:
		{
			data_itemPriceOffset = MNU_ITEM_PRICE_OFFSET;
			data_matPriceOffset = MNU_MAT_PRICE_OFFSET;
			break;
		}
		case 3:
		{
			data_itemPriceOffset = offset+EXE_ITEM_PRICE_OFFSET;
			data_matPriceOffset = offset+EXE_MAT_PRICE_OFFSET;
			break;
		}
	}
	int tmpIndex;
	if(priceIndex < MATERIA_OFFSET)
		tmpIndex = data_itemPriceOffset+priceIndex*PRICESIZE;
	else
		tmpIndex = data_matPriceOffset+(priceIndex-MATERIA_OFFSET)*PRICESIZE;
	long unsigned int price = data[tmpIndex] + data[tmpIndex+1]*0x100 +
		data[tmpIndex+2]*0x10000 + data[tmpIndex+3]*0x1000000;
	char* tmpstr = (char*)malloc(100*sizeof(char));
	sprintf(tmpstr, "%lu", price);
	SendMessage(priceEditBox, WM_SETTEXT, 0, (LPARAM)tmpstr);
	free(tmpstr);
	return 0;
}
/*
 * Loads data of currently selected shop into window and displays it
 * hwnd - handle to the window
 */
int loadShop(HWND hwnd)
{
	if(shopIndex == -1)
		return 0;
	SendMessage(shopNameEditBox, WM_SETTEXT, 0, (LPARAM)itemNames[shopIndex+SHOPS_OFFSET]);
	long unsigned int data_shopOffset = 0;
	switch(gameFileLoaded)
	{
		case 0:
		{
			return 0;
		}
		case 1:
		{
			data_shopOffset = EXE_SHOP_OFFSET;
			break;
		}
		case 2:
		{
			data_shopOffset = MNU_SHOP_OFFSET;
			break;
		}
		case 3:
		{
			data_shopOffset = offset+EXE_SHOP_OFFSET;
			break;
		}
	}
	int tmpIndex = data_shopOffset+shopIndex*SHOPSIZE;
	if(SendMessage(typeSelector, CB_SETCURSEL, (WPARAM)data[tmpIndex], 0) == CB_ERR)
	{
		char* tmpstr = (char*)malloc(100*sizeof(char));
		tmpstr = "Warning: Shop has an invalid type index of ";
		sprintf(&tmpstr[44], "%d", data[tmpIndex]);
		strcat(tmpstr, "! Defaulting to 0");
		MessageBox(hwnd, tmpstr, "Warning!", MB_OK | MB_ICONINFORMATION);
		SendMessage(typeSelector, CB_SETCURSEL, 0, 0); // default to x00
	}
	tmpIndex += 2;
	int itemIndex = 0;
	while(itemIndex < data[tmpIndex])
	{
		EnableWindow(itemSelectors[itemIndex], 1);
		itemIndex++;
	}
	while(itemIndex < 10)
	{
		EnableWindow(itemSelectors[itemIndex], 0);
		itemIndex++;
	}
	if(SendMessage(inventorySelector, CB_SETCURSEL, data[tmpIndex], 0) == CB_ERR)
	{
		SendMessage(inventorySelector, CB_SETCURSEL, 2, 0);
	}
	tmpIndex += 2;
	short int materia;
	itemIndex = 0;
	unsigned int itemNameIndex = 0;
	while(itemIndex < 10)
	{
		materia = data[tmpIndex];
		tmpIndex += 4;
		itemNameIndex = data[tmpIndex]+256*data[tmpIndex+1];
		if(materia)	itemNameIndex += MATERIA_OFFSET;
		if(SendMessage(itemSelectors[itemIndex], CB_SETCURSEL, itemNameIndex, 0) == CB_ERR)
		{
			MessageBox(hwnd, "Warning: Invalid item ID, defaulting to 0.", "Warning!",
				MB_OK | MB_ICONINFORMATION);
			SendMessage(itemSelectors[itemIndex], CB_SETCURSEL, 0, 0);
		}
		itemIndex++;
		tmpIndex += 4;
	}
	return 0;
}
/*
 * Prompt user to delete current ff7.exe and rename ff7.exe.bak to ff7.exe.
 * If user says no, or ff7.exe.bak doesn't exist, prompt the user to manually
 * specify a backup file.
 * hwnd - handle to the window
 */
int restoreBackup(HWND hwnd)
{
	if(gameFileLoaded != 1 && gameFileLoaded != 3)
		return 0;
	FILE* tmp = fopen("ff7.exe.bak", "r");
	if(tmp == NULL || fgetc(tmp) == EOF)
		return 1;
	fclose(tmp);
	fcopy(gamefilePath, "ff7.exe.bak");
	if(MessageBox(hwnd, "Backup restored successfully.\nWould you like to open it?",
		"Confirmation", MB_YESNO | MB_ICONINFORMATION) == IDYES)
	{
		FILE* ff7 = fopen(gamefilePath, "r");
		fread(data, 1, EXE_FILESIZE, ff7);
		fclose(ff7);
		loadShop(hwnd);
	}
	return 0;
}
/*
 * Saves changes to ff7.exe while creating a backup, ff7.exe.bak
 * hwnd - handle to the window
 */
int saveExecutable(HWND hwnd)
{
	if(MessageBox(hwnd,
		"This will create a backup of your previous ff7.exe, called 'ff7.exe.bak', and create a new one with the changes specified.\nProceed?",
		"Confirmation", MB_YESNO | MB_ICONINFORMATION) == IDYES)
	{
		updateData(hwnd);
		fcopy("ff7.exe.bak", gamefilePath);
		FILE* ff7 = fopen(gamefilePath, "w");
		if(ff7 == NULL)
			return 1;
		fwrite(data, 1, filesize, ff7);
		fclose(ff7);
	}
	return 0;
}
/*
 * Saves changes to shopmenu.mnu
 * hwnd - handle to the window
 */
int saveMNU(HWND hwnd)
{
	if(MessageBox(hwnd,
		"Save changes to SHOPMENU.MNU?",
		"Confirmation", MB_YESNO | MB_ICONINFORMATION) == IDYES)
	{
		updateData(hwnd);
		FILE* ff7 = fopen(gamefilePath, "w");
		if(ff7 == NULL)
			return 1;
		fwrite(data, 1, MNU_FILESIZE, ff7);
		fclose(ff7);
	}
	return 0;
}
/*
 * Loads ff7.exe, reads data, and draws UI. Returns 0 upon success,
 * 1 upon failure, 2 upon invalid filesize error.
 * hwnd - handle to the window
 * psx - 0 if loading ff7.exe, nonzero if loading shopmenu.mnu
 */
int loadExecutable(HWND hwnd, short int psx)
{
	short int modified_exe = 0;
	OPENFILENAME ff7exec;
	ff7exec.lStructSize = sizeof(OPENFILENAME);
	ff7exec.hwndOwner = hwnd;
	ff7exec.hInstance = hInstance;
	ff7exec.lpstrFilter = NULL;
	ff7exec.lpstrCustomFilter = NULL;
	ff7exec.lpstrFile = gamefilePath;
	ff7exec.nMaxFile = 32000;
	ff7exec.lpstrFileTitle = NULL;
	if(psx)
	{
		ff7exec.lpstrInitialDir = "%USERPROFILE%";
		ff7exec.lpstrTitle = "Open SHOPMENU.MNU";
	}
	else
	{
		ff7exec.lpstrInitialDir =
			"%ProgramFiles%\\Square Soft, Inc\\Final Fantasy VII\\ff7.exe";
		ff7exec.lpstrTitle = "Open FF7.exe";
	}
	ff7exec.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOREADONLYRETURN |
		OFN_LONGNAMES | OFN_HIDEREADONLY;
	if(!GetOpenFileName(&ff7exec))
		return 0;
	FILE* ff7 = fopen(gamefilePath, "r");
	if(ff7 == NULL)
		return 1;
	if(psx)
	{
		filesize = fread(data, 1, MNU_FILESIZE, ff7);
		if(filesize != MNU_FILESIZE) // check for a file too small
		{
			int tmpindex = MNU_SIGNATURE;
			// Check file signature
			if(	data[tmpindex++] == 'S' && data[tmpindex++] == 'U' &&
				data[tmpindex++] == 'M' && data[tmpindex++] == 'T' &&
				data[tmpindex++] == 'H' && data[tmpindex++] == 'I' &&
				data[tmpindex++] == 'N')
			{
				MessageBox(hwnd,
					"Warning: filesize does not match, but file signature found.",
					"Warning", MB_OK | MB_ICONWARNING);
			}
			else return 2;
		}
	}
	else
	{
		fseek(ff7, 0, SEEK_END);
		filesize = ftell(ff7);
		rewind(ff7);
		if(filesize != EXE_FILESIZE) // check for a file too small
		{
			data = (byte*)realloc(data, filesize*sizeof(byte));
			if(fread(data, 1, filesize, ff7) != filesize)
				return 2;
			// Check file signature
			if(scanFileSignature(filesize, 1) == 0)	// Check for sig in the normal place
			{
				MessageBox(hwnd,
					"Warning: filesize does not match, but file signature found.",
					"Warning", MB_OK | MB_ICONWARNING);
			}
			else	// The sig isn't where it usually is--search for it and find an offset
			{
				offset = scanFileSignature(filesize, 0);
				if(offset == 0)	// sig not found
					return 2;
				char* tmpstr = malloc(30*sizeof(char));
				sprintf(tmpstr,
					"International or modified FF7.exe found.\nCalculated offset: %ld",
					offset);
				MessageBox(hwnd, tmpstr, "Warning", MB_OK | MB_ICONWARNING);
				free(tmpstr);
				modified_exe = 1;
			}
		}
		else if(fread(data, 1, filesize, ff7) != EXE_FILESIZE)
			return 2;
	}
	if(fgetc(ff7) != EOF && psx) // check for a file too large
	{
		int tmpindex = MNU_SIGNATURE;
		// Check file signature
		if(	data[tmpindex++] == 'S' && data[tmpindex++] == 'U' &&
			data[tmpindex++] == 'M' && data[tmpindex++] == 'T' &&
			data[tmpindex++] == 'H' && data[tmpindex++] == 'I' &&
			data[tmpindex++] == 'N')
		{
			MessageBox(hwnd,
				"Warning: filesize does not match, but file signature found.",
				"Warning", MB_OK | MB_ICONWARNING);
		}
		else return 2;
	}
	fclose(ff7);
	if(psx)
		gameFileLoaded = 2;
	else if(modified_exe)
		gameFileLoaded = 3;
	else
		gameFileLoaded = 1;
	switch(mode)
	{
		case 0:	// Shop Editor
		{
			loadShop(hwnd);
			break;
		}
		case 1:	// Price Editor
		{
			loadPrice(hwnd);
			loadMMM(hwnd);
			break;
		}
	}
	EnableMenuItem(hFileMenu, ID_FILE_APPLY_CHANGES, MF_ENABLED);
	if(mode == 0)
	{
		EnableWindow(inventorySelector, 1);
		EnableWindow(typeSelector, 1);
	}
	else if(mode == 1)
		EnableWindow(priceEditBox, 1);
	if(psx)
	{
		EnableWindow(masterMultEditBox, 0);
		EnableMenuItem(hFileMenu, ID_FILE_RESTORE_BACKUP, MF_DISABLED);
	}
	else
	{
		EnableWindow(masterMultEditBox, 1);
		EnableMenuItem(hFileMenu, ID_FILE_RESTORE_BACKUP, MF_ENABLED);
	}
	return 0;
}
/*
 * Builds table of names based upon given config file
 * configfile - file to read from, should be opened prior to calling and closed after
 */
int buildNameTable(FILE* configfile)
{
	if(configFileLoaded)
		return 0;
	Scanner_init(configfile);
	Scanner_removeDelimiters();
	int err;
	int i = 0;
	//int j = 0;
	while(i < TOTAL_NAMES)
	{
		if(!Scanner_hasNext())
			return 1;
		err = Scanner_next();
		if(err == 1) // end of file
			return -1;
		if(err == 2) // string too large
			return -2;
		if(err) // unknown error
			return -3;
		itemNames[i] = (char*)realloc(itemNames[i], MAX_NAME_LENGTH*sizeof(char));
		strcpy(itemNames[i], Scanner_token);
		i++;
	}
	configFileLoaded = 1;
	if(Scanner_hasNext())
		return 2;
	return 0;
}
/*
 * Changes/draws the editor displayed controls.
 * newMode - mode to be changed to:
 * -1 = Redraw Current Mode
 * 	0 = Shop Editing Mode
 * 	1 = Price Editing Mode
 * hwnd - handle to the window
 */
int changeMode(int newMode, HWND hwnd)
{
	if(newMode == mode)
		return 0;
	if(newMode != -1)
	{
		switch(mode)
		{
			case 0:
			{
				DestroyWindow(shopSelector);
				DestroyWindow(shopNameEditBox);
				DestroyWindow(typeSelector);
				DestroyWindow(inventorySelector);
				int i = 0;
				while(i < SHOPS_NUMCOLS)
				{
					if(i < 10)
						DestroyWindow(itemSelectors[i]);
					DestroyWindow(shopsTexts[i++]);
				}
				shopIndex = -1;
				break;
			}
			case 1:
			{
				DestroyWindow(priceSelector);
				DestroyWindow(itemNameEditBox);
				DestroyWindow(priceEditBox);
				DestroyWindow(masterMultEditBox);
				DestroyWindow(masterMultGroupCtrl);
				DestroyWindow(itemNameLabel);
				DestroyWindow(priceLabel);
				priceIndex = -1;
				break;
			}
		}
	}
	else
		newMode = mode;
	switch(newMode)
	{
		case 0:	// Time to build the shop editor controls
		{
			// Create a listbox on the left side of the window, to contain a list of all of
			// the shops
			int staticTextWidth = 95;
			int ycoord = TAB_HEIGHT+EDGE_BUFFER;
			shopSelector = CreateWindowEx(0, WC_LISTBOX, 0, WS_VISIBLE | WS_CHILD |
				WS_TABSTOP | WS_VSCROLL | WS_BORDER | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS
				| LBS_NOTIFY, EDGE_BUFFER, ycoord, LISTXSIZE, LISTYSIZE, hwnd,
				(HMENU)IDSC_LIST, hInstance, 0);
			// Change listbox font
			SendMessage(shopSelector, WM_SETFONT, (WPARAM)smallFont, FALSE);
			// Create Static texts
			shopsTexts[SHOPS_NUMCOLS-1] = CreateWindowEx(0, WC_STATIC, "Shop Name:",
				WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, LISTXSIZE+EDGE_BUFFER, ycoord,
				staticTextWidth, LISTYSIZE/SHOPS_NUMCOLS, hwnd, (HMENU)IDSC_STATIC,
				hInstance, 0);
			SendMessage(shopsTexts[SHOPS_NUMCOLS-1], WM_SETFONT, (WPARAM)largeFont, FALSE);
			ycoord += LISTYSIZE/SHOPS_NUMCOLS;
			shopsTexts[SHOPS_NUMCOLS-2] = CreateWindowEx(0, WC_STATIC, "Shop Type:",
				WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, LISTXSIZE+EDGE_BUFFER, ycoord,
				staticTextWidth, LISTYSIZE/SHOPS_NUMCOLS, hwnd, (HMENU)IDSC_STATIC-1,
				hInstance, 0);
			SendMessage(shopsTexts[SHOPS_NUMCOLS-2], WM_SETFONT, (WPARAM)largeFont, FALSE);
			ycoord += LISTYSIZE/SHOPS_NUMCOLS;
			shopsTexts[SHOPS_NUMCOLS-3] = CreateWindowEx(0, WC_STATIC, "Inventory:",
				WS_VISIBLE | WS_CHILD | WS_GROUP | SS_RIGHT, LISTXSIZE+EDGE_BUFFER, ycoord,
				staticTextWidth, LISTYSIZE/SHOPS_NUMCOLS, hwnd, (HMENU)IDSC_STATIC-2,
				hInstance, 0);
			SendMessage(shopsTexts[SHOPS_NUMCOLS-3], WM_SETFONT, (WPARAM)largeFont, FALSE);
			int itemNo = 1;
			char* itemNoString = (char*)malloc(10*sizeof(char));
			while(ycoord < 495 && itemNo <= 10)
			{
				ycoord += LISTYSIZE/SHOPS_NUMCOLS;
				strcpy(itemNoString, "Item ");
				itemNoString[5] = (char)((int)(itemNo/10)+48);
				if(itemNo < 10) 	itemNoString[6] = (char)(itemNo+48);
				else 				itemNoString[6] = (char)(itemNo+38);
				itemNoString[7] = ':';
				itemNoString[8] = '\0';
				shopsTexts[itemNo-1] = CreateWindowEx(0, WC_STATIC, itemNoString, WS_VISIBLE |
					WS_CHILD | WS_GROUP | SS_RIGHT, LISTXSIZE+EDGE_BUFFER, ycoord,
					staticTextWidth, LISTYSIZE/SHOPS_NUMCOLS, hwnd,
					(HMENU)IDSC_STATIC+itemNo, hInstance, 0);
				SendMessage(shopsTexts[itemNo-1], WM_SETFONT, (WPARAM)largeFont, FALSE);
				itemNoString = (char*)realloc(itemNoString, 10*sizeof(char));
				itemNo++;
			}
			free(itemNoString);
			// Create Input Box for Shop Name
			ycoord = TAB_HEIGHT+EDGE_BUFFER;
			int xcoord = LISTXSIZE+EDGE_BUFFER+staticTextWidth;
			shopNameEditBox = CreateWindowEx(WS_EX_CLIENTEDGE |
				WS_EX_STATICEDGE, WC_EDIT, 0, WS_VISIBLE | WS_CHILD | WS_TABSTOP |
				ES_AUTOHSCROLL, xcoord, ycoord, EDITBOXWIDTH, EDITBOXHEIGHT, hwnd,
				(HMENU)IDSC_EDIT, hInstance, 0);
			SendMessage(shopNameEditBox, WM_SETFONT, (WPARAM)smallFont, FALSE);
			// Create Comboboxes:
			itemNo = 0;
			ycoord += LISTYSIZE/SHOPS_NUMCOLS;
			typeSelector = CreateWindowEx(WS_EX_CLIENTEDGE, WC_COMBOBOX, 0, WS_VISIBLE |
				WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_DISABLED, xcoord, ycoord,
				EDITBOXWIDTH, EDITBOXWIDTH+EDITBOXHEIGHT, hwnd, (HMENU)(IDSC_COMBO-1),
				hInstance, 0);
			SendMessage(typeSelector, CB_ADDSTRING, 0, (LPARAM)"x00 Item Shop");
			SendMessage(typeSelector, CB_ADDSTRING, 0, (LPARAM)"x01 Weapon Shop");
			SendMessage(typeSelector, CB_ADDSTRING, 0, (LPARAM)"x02 Item Shop");
			SendMessage(typeSelector, CB_ADDSTRING, 0, (LPARAM)"x03 Materia Shop");
			SendMessage(typeSelector, CB_ADDSTRING, 0, (LPARAM)"x04 General Store");
			SendMessage(typeSelector, CB_ADDSTRING, 0, (LPARAM)"x05 Vegetable Store");
			SendMessage(typeSelector, CB_ADDSTRING, 0, (LPARAM)"x06 Accessory Shop");
			SendMessage(typeSelector, CB_ADDSTRING, 0, (LPARAM)"x07 Tool Shop");
			SendMessage(typeSelector, CB_ADDSTRING, 0, (LPARAM)"x08 Hotel Shop");
			ycoord += LISTYSIZE/SHOPS_NUMCOLS;
			inventorySelector = CreateWindowEx(WS_EX_CLIENTEDGE, WC_COMBOBOX, 0, WS_DISABLED
				| WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS, xcoord, ycoord,
				EDITBOXWIDTH, EDITBOXWIDTH+EDITBOXHEIGHT, hwnd, (HMENU)(IDSC_COMBO-2),
				hInstance, 0);
			SendMessage(inventorySelector, CB_ADDSTRING, 0, (LPARAM)"0");
			SendMessage(inventorySelector, CB_ADDSTRING, 0, (LPARAM)"1");
			SendMessage(inventorySelector, CB_ADDSTRING, 0, (LPARAM)"2");
			SendMessage(inventorySelector, CB_ADDSTRING, 0, (LPARAM)"3");
			SendMessage(inventorySelector, CB_ADDSTRING, 0, (LPARAM)"4");
			SendMessage(inventorySelector, CB_ADDSTRING, 0, (LPARAM)"5");
			SendMessage(inventorySelector, CB_ADDSTRING, 0, (LPARAM)"6");
			SendMessage(inventorySelector, CB_ADDSTRING, 0, (LPARAM)"7");
			SendMessage(inventorySelector, CB_ADDSTRING, 0, (LPARAM)"8");
			SendMessage(inventorySelector, CB_ADDSTRING, 0, (LPARAM)"9");
			SendMessage(inventorySelector, CB_ADDSTRING, 0, (LPARAM)"10");
			ycoord += LISTYSIZE/SHOPS_NUMCOLS;
			while(itemNo < 10)
			{
				itemSelectors[itemNo] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_COMBOBOX, 0,
					WS_VISIBLE | WS_DISABLED | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST |
					CBS_HASSTRINGS, xcoord, ycoord, EDITBOXWIDTH, 400, hwnd,
					(HMENU)(IDSC_COMBO+itemNo), hInstance, 0);
				ycoord += LISTYSIZE/SHOPS_NUMCOLS;
				itemNo++;
			}
			// Fill shopSelector and itemSelectors with itemNames
			int i = 0;
			int j = 0;
			while(i < TOTAL_NAMES)
			{
				if(i >= SHOPS_OFFSET)
				{
					SendMessage(shopSelector, LB_ADDSTRING, 0, (LPARAM)itemNames[i]);
				}
				if(i < SHOPS_OFFSET)
				{
					j = 0;
					while(j < 10)
					{
						SendMessage(itemSelectors[j++], CB_ADDSTRING, 0,
							(LPARAM)itemNames[i]);
					}
				}
				i++;
			}
			mode = newMode;
			if(gameFileLoaded)
			{
				EnableWindow(inventorySelector, 1);
				EnableWindow(typeSelector, 1);
			}
			break;
		}
		case 1:	// Time to build the price editor controls
		{
			// Create Price Selector List box and apply font
			int ycoord = EDGE_BUFFER+TAB_HEIGHT;
			priceSelector = CreateWindowEx(0, WC_LISTBOX, 0, WS_VISIBLE | WS_CHILD |
				WS_TABSTOP | WS_VSCROLL | WS_BORDER | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS
				| LBS_NOTIFY, EDGE_BUFFER, ycoord, LISTXSIZE, LISTYSIZE, hwnd,
				(HMENU)IDPC_LIST, hInstance, 0);
			SendMessage(priceSelector, WM_SETFONT, (WPARAM)smallFont, FALSE);
			// Create static texts and apply fonts
			int staticTextWidth = 145;
			itemNameLabel = CreateWindowEx(0, WC_STATIC, "Item Name:", WS_VISIBLE |
				WS_CHILD | WS_GROUP | SS_RIGHT | SS_CENTERIMAGE, LISTXSIZE+EDGE_BUFFER,
				ycoord, staticTextWidth, EDITBOXHEIGHT, hwnd, (HMENU)IDPC_STATIC1,
				hInstance, 0);
			SendMessage(itemNameLabel, WM_SETFONT, (WPARAM)largeFont, FALSE);
			ycoord += 2*EDITBOXHEIGHT;
			priceLabel = CreateWindowEx(0, WC_STATIC, "Item Price:", WS_VISIBLE | WS_CHILD
				| WS_GROUP | SS_RIGHT | SS_CENTERIMAGE, LISTXSIZE+EDGE_BUFFER, ycoord,
				staticTextWidth, EDITBOXHEIGHT, hwnd, (HMENU)IDPC_STATIC2, hInstance, 0);
			SendMessage(priceLabel, WM_SETFONT, (WPARAM)largeFont, FALSE);
			// Create edit boxes and apply fonts
			ycoord = EDGE_BUFFER+TAB_HEIGHT;
			int xcoord = EDGE_BUFFER+LISTXSIZE+staticTextWidth;
			itemNameEditBox = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, 0, WS_VISIBLE |
				WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, xcoord, ycoord,
				EDITBOXWIDTH, EDITBOXHEIGHT, hwnd, (HMENU)IDPC_NAMEEDIT, hInstance, 0);
			SendMessage(itemNameEditBox, WM_SETFONT, (WPARAM)smallFont, FALSE);
			ycoord += 2*EDITBOXHEIGHT;
			priceEditBox = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, 0, WS_VISIBLE |
				WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER | WS_DISABLED,
				xcoord, ycoord, EDITBOXWIDTH, EDITBOXHEIGHT, hwnd, (HMENU)IDPC_PRICEEDIT,
				hInstance, 0);
			SendMessage(priceEditBox, WM_SETFONT, (WPARAM)defaultFont, FALSE);
			// Create Master Materia Multiplier Edit Group and apply fonts
			ycoord += 2*EDITBOXHEIGHT;
			masterMultGroupCtrl = CreateWindowEx(0, WC_BUTTON, "Master Materia Multiplier",
				WS_VISIBLE | WS_CHILD | BS_CENTER | BS_GROUPBOX, xcoord, ycoord,
				EDITBOXWIDTH, EDITBOXHEIGHT+4*EDGE_BUFFER, hwnd, (HMENU)IDPC_GROUP,
				hInstance, 0);
			SendMessage(masterMultGroupCtrl, WM_SETFONT, (WPARAM)defaultFont, FALSE);
			masterMultEditBox = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, 0, WS_VISIBLE |
				WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER | WS_DISABLED,
				xcoord+EDGE_BUFFER, ycoord+3*EDGE_BUFFER, EDITBOXWIDTH-2*EDGE_BUFFER,
				EDITBOXHEIGHT, hwnd, (HMENU)IDPC_MMMEDIT, hInstance, 0);
			SendMessage(masterMultEditBox, WM_SETFONT, (WPARAM)defaultFont, FALSE);
			// Fill priceSelector with itemNames
			int i = 0;
			while(i < SHOPS_OFFSET)
			{
				SendMessage(priceSelector, LB_ADDSTRING, 0, (LPARAM)itemNames[i]);
				i++;
			}
			// We can actually try to load the multiplier data now, too
			mode = newMode;
			loadMMM(hwnd);
			if(gameFileLoaded)
				EnableWindow(priceEditBox, 1);
			if(gameFileLoaded == 1)
				EnableWindow(masterMultEditBox, 1);
			break;
		}
		default:
		{
			changeMode(-1, hwnd);	// crap, put things back the way they were
			return 0;
		}
	}
	return 0;
}
/*
 * Window Procedure, instantiates window.
 * Standard window parameters
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
		case WM_NOTIFY:
		{
			switch(wParam)
			{
				case IDC_TABCONTROL:
				{
					switch((*((NMHDR*)lParam)).code)	// lolrly?
					{
						case TCN_SELCHANGE:
						{
							updateData(hwnd);
							changeMode(SendMessage(tabControl, TCM_GETCURSEL, 0, 0), hwnd);
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case WM_CREATE:
		{
			HMENU hMenu, hHelpMenu;
			HICON hIcon, hIconSm;
			hMenu = CreateMenu();
			hFileMenu = CreatePopupMenu();
			AppendMenu(hFileMenu, MF_STRING, ID_FILE_LOAD_EXE, "&Load FF7.exe");
			AppendMenu(hFileMenu, MF_STRING, ID_FILE_LOAD_MNU, "Load SHOPMENU.&MNU");
			AppendMenu(hFileMenu, MF_SEPARATOR, 0, 0);
			AppendMenu(hFileMenu, MF_STRING | MF_GRAYED, ID_FILE_APPLY_CHANGES,
				"&Apply Changes");
			AppendMenu(hFileMenu, MF_STRING, ID_FILE_SAVE_NAMES, "&Save Names");
			AppendMenu(hFileMenu, MF_STRING | MF_GRAYED, ID_FILE_RESTORE_BACKUP,
				"&Restore Backup");
			AppendMenu(hFileMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hFileMenu, "&File");
			hHelpMenu = CreatePopupMenu();
			AppendMenu(hHelpMenu, MF_STRING, ID_HELP_ABOUT, "&About");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hHelpMenu, "&Help");
			SetMenu(hwnd, hMenu);
			hIcon = LoadImage(NULL, "whitechoco_l.ico", IMAGE_ICON, 256, 256,
				LR_LOADFROMFILE);
			if(hIcon)
				SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			else
				MessageBox(hwnd, "Could not load large icon!", "Error", MB_OK |
					MB_ICONERROR);
			hIconSm = LoadImage(NULL, "whitechoco_s.ico", IMAGE_ICON, 16, 16,
				LR_LOADFROMFILE);
			if(hIconSm)
				SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
			else
				MessageBox(hwnd, "Could not load small icon!", "Error", MB_OK |
					MB_ICONERROR);
		}
		break;
		case WM_SYSKEYDOWN:
		{
			switch(wParam)
			{
				case 'S':
				case 's':
				{
					if(saveNameTable(hwnd))
					{
						MessageBox(hwnd, "Error saving whitechoco.cfg.", "Error",
							MB_OK | MB_ICONERROR);
					}
				}
				break;
				case 'A':
				case 'a':
				{
					switch(gameFileLoaded)
					{
						case 1:
						case 3:
						{
							if(saveExecutable(hwnd))
							{
								MessageBox(hwnd, "Error saving ff7.exe.", "Error",
									MB_OK | MB_ICONERROR);
							}
							break;
						}
						case 2:
						{
							if(saveMNU(hwnd))
							{
								MessageBox(hwnd, "Error saving SHOPMENU.MNU.", "Error",
									MB_OK | MB_ICONERROR);
							}
							break;
						}
					}
				}
				break;
				case 'B':
				case 'b':
				{
					if(restoreBackup(hwnd))
					{
						MessageBox(hwnd, "Error backing up ff7.exe.", "Error",
							MB_OK | MB_ICONERROR);
					}
				}
				break;
				case 'L':
				case 'l':
				{
					int err = loadExecutable(hwnd, 0);
					if(err == 2)
					{
						MessageBox(hwnd, "Invalid filesize and file signature.", "Error",
							MB_OK | MB_ICONERROR);
					}
					else if(err)
					{
						MessageBox(hwnd, "Error loading ff7.exe.", "Error",
							MB_OK | MB_ICONERROR);
					}
					break;
				}
				case 'M':
				case 'm':
				{
					int err = loadExecutable(hwnd, 1);
					if(err == 2)
					{
						MessageBox(hwnd, "Invalid filesize and file signature.", "Error",
							MB_OK | MB_ICONERROR);
					}
					else if(err)
					{
						MessageBox(hwnd, "Error loading SHOPMENU.MNU.", "Error",
							MB_OK | MB_ICONERROR);
					}
					break;
				}
			}
		}
		break;
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDPC_NAMEEDIT:
				{
					switch(HIWORD(wParam))
					{
						case EN_CHANGE:
						{
							if(priceIndex != -1)
							{
								itemNames[priceIndex] =
									realloc(itemNames[priceIndex],
										MAX_NAME_LENGTH*sizeof(char));
								SendMessage(itemNameEditBox, WM_GETTEXT, MAX_NAME_LENGTH,
									(LPARAM)itemNames[priceIndex]);
								// Couldn't find a more efficient way to modify a string
								SendMessage(priceSelector, LB_DELETESTRING,
									(WPARAM)priceIndex, 0);
								SendMessage(priceSelector, LB_INSERTSTRING,
									(WPARAM)priceIndex, (LPARAM)itemNames[priceIndex]);
								SendMessage(priceSelector, LB_SETCURSEL,
									(WPARAM)priceIndex, 0);
							}
							break;
						}
					}
					break;
				}
				case IDPC_PRICEEDIT:
				{
					switch(HIWORD(wParam))
					{
						case EN_CHANGE:
						{
							char* tmpstr = (char*)malloc(50*sizeof(char));
							SendMessage(priceEditBox, WM_GETTEXT, 50, (LPARAM)tmpstr);
							double price;
							sscanf(tmpstr, "%lf", &price);
							if(price > 0xFFFFFFFF)
								SendMessage(priceEditBox, WM_SETTEXT, 0,
									(LPARAM)"4294967040");
							break;
						}
					}
					break;
				}
				case IDPC_MMMEDIT:
				{
					switch(HIWORD(wParam))
					{
						case EN_CHANGE:
						{
							char* tmpstr = (char*)malloc(50*sizeof(char));
							SendMessage(masterMultEditBox, WM_GETTEXT, 50, (LPARAM)tmpstr);
							short unsigned int price;
							sscanf(tmpstr, "%hu", &price);
							if(price > 0xFF)
								SendMessage(masterMultEditBox, WM_SETTEXT, 0, (LPARAM)"255");
							break;
						}
					}
					break;
				}
				case IDPC_LIST:
				{
					switch(HIWORD(wParam))
					{
						case LBN_SELCHANGE:
						{
							updateData(hwnd);
							priceIndex = SendMessage(priceSelector, LB_GETCURSEL, 0, 0);
							if(loadPrice(hwnd))
							{
								MessageBox(hwnd, "Error refreshing values.", "Error",
									MB_OK | MB_ICONERROR);
							}
							break;
						}
					}
					break;
				}
				case (IDSC_COMBO-2): // inventory selector
				{
					switch(HIWORD(wParam))
					{
						case CBN_SELCHANGE:
						{
							int itemIndex = 0;
							int tmp = SendMessage(inventorySelector, CB_GETCURSEL, 0, 0);
							while(itemIndex < tmp)
							{
								EnableWindow(itemSelectors[itemIndex], 1);
								itemIndex++;
							}
							while(itemIndex < 10)
							{
								EnableWindow(itemSelectors[itemIndex], 0);
								itemIndex++;
							}
							break;
						}
					}
					break;
				}
				case IDSC_EDIT:
				{
					switch(HIWORD(wParam))
					{
						case EN_CHANGE:
						{
							if(shopIndex != -1)
							{
								itemNames[shopIndex+SHOPS_OFFSET] =
									realloc(itemNames[shopIndex+SHOPS_OFFSET],
										MAX_NAME_LENGTH*sizeof(char));
								SendMessage(shopNameEditBox, WM_GETTEXT, MAX_NAME_LENGTH,
									(LPARAM)itemNames[shopIndex+SHOPS_OFFSET]);
								// Couldn't find a more efficient way to modify a string
								SendMessage(shopSelector, LB_DELETESTRING, (WPARAM)shopIndex,
									0);
								SendMessage(shopSelector, LB_INSERTSTRING, (WPARAM)shopIndex,
									(LPARAM)itemNames[shopIndex+SHOPS_OFFSET]);
								SendMessage(shopSelector, LB_SETCURSEL,
									(WPARAM)shopIndex, 0);
							}
							break;
						}
					}
					break;
				}
				case IDSC_LIST:
				{
					switch(HIWORD(wParam))
					{
						case LBN_SELCHANGE:
						{
							updateData(hwnd);
							shopIndex = SendMessage(shopSelector, LB_GETCURSEL, 0, 0);
							if(loadShop(hwnd))
							{
								MessageBox(hwnd, "Error refreshing values.", "Error",
									MB_OK | MB_ICONERROR);
							}
							break;
						}
					}
					break;
				}
				case ID_FILE_EXIT:
				{
					if (MessageBox(hwnd, "Are you sure you would like to quit?",
							"Quit", MB_YESNO | MB_ICONINFORMATION) == IDYES)
						DestroyWindow(hwnd);
					break;
				}
				case ID_FILE_LOAD_EXE:
				{
					UpdateWindow(hwnd);
					int err;
					err = loadExecutable(hwnd, 0);
					if(err == 2)
					{
						MessageBox(hwnd, "Invalid filesize and file signature.", "Error",
							MB_OK | MB_ICONERROR);
					}
					else if(err)
					{
						MessageBox(hwnd, "Error loading ff7.exe.", "Error",
							MB_OK | MB_ICONERROR);
					}
					break;
				}
				case ID_FILE_LOAD_MNU:
				{
					int err = loadExecutable(hwnd, 1);
					if(err == 2)
					{
						MessageBox(hwnd, "Invalid filesize and file signature.", "Error",
							MB_OK | MB_ICONERROR);
					}
					else if(err)
					{
						MessageBox(hwnd, "Error loading SHOPMENU.MNU.", "Error",
							MB_OK | MB_ICONERROR);
					}
					break;
				}
				case ID_FILE_APPLY_CHANGES:
				{
					switch(gameFileLoaded)
					{
						case 1:
						case 3:
						{
							if(saveExecutable(hwnd))
							{
								MessageBox(hwnd, "Error saving ff7.exe.", "Error",
									MB_OK | MB_ICONERROR);
							}
							break;
						}
						case 2:
						{
							if(saveMNU(hwnd))
							{
								MessageBox(hwnd, "Error saving SHOPMENU.MNU.", "Error",
									MB_OK | MB_ICONERROR);
							}
							break;
						}
					}
					break;
				}
				case ID_FILE_SAVE_NAMES:
				{
					if(saveNameTable(hwnd))
					{
						MessageBox(hwnd, "Error saving whitechoco.cfg.", "Error",
							MB_OK | MB_ICONERROR);
					}
					break;
				}
				case ID_FILE_RESTORE_BACKUP:
				{
					if(restoreBackup(hwnd))
					{
						MessageBox(hwnd, "Error backing up ff7.exe.", "Error",
							MB_OK | MB_ICONERROR);
					}
					break;
				}
				case ID_HELP_ABOUT:
				{
					char* abtmsg =
						"White Choco\nMade by Yama H (aka titeguy3)\n\nA tool for editing shop data in FFVII";
					MessageBox(hwnd, abtmsg, version, MB_OK | MB_ICONINFORMATION);
					break;
				}
			}
			break;
		}
		case WM_CLOSE:
		{
			if (MessageBox(hwnd, "Are you sure you would like to quit?",
					"Quit", MB_YESNO | MB_ICONINFORMATION) == IDYES)
				DestroyWindow(hwnd);
			break;
		}
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        default:
        {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
    return 0;
}
/*
 * Win32 Main Method
 * hInstance - handle to this instance
 * hPrevInstance - unused
 * lpCmdLine - command line arguments, in long pointer string format (unused)
 * nCmdShow - unused
 */
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
	// Instantiate file signature
	char* signatureString = malloc(EXE_SIG_LEN*3*sizeof(char));
	signatureString[0] = '\0';
	strcat(signatureString, "00 00 00 00 9A 01 33 00 00 00 33 00 80 02 33 00 00 00 5E 00 ");
	strcat(signatureString, "C6 01 CF 00 00 00 2D 01 80 02 B2 00 00 00 00 00 00 00 00 00 ");
	strcat(signatureString, "00 00 66 00 80 02 33 00 B8 01 5E 00 C8 00 CF 00 ED 00");
	signature = (byte*)malloc(EXE_SIG_LEN*sizeof(byte));
	int i = 0;
	short unsigned int bytecast;
	while(i < EXE_SIG_LEN)
	{
		sscanf(&signatureString[i*3], "%2hX", &bytecast);
		signature[i] = (byte)bytecast;
		i++;
	}
	WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    hInstance = hInst;
	itemNames = (char**)malloc(TOTAL_NAMES*sizeof(char*));
	gamefilePath = (char*)malloc(32000*sizeof(char));
	strcpy(gamefilePath, "%ProgramFiles%\\Square Soft, Inc\\Final Fantasy VII\\ff7.exe");
	itemSelectors = (HWND*)malloc(10*sizeof(HWND));
	shopsTexts = (HWND*)malloc(SHOPS_NUMCOLS*sizeof(HWND));
	data = (byte*)malloc(EXE_FILESIZE*sizeof(byte));
	smallFont = CreateFont(-11, 0, 0, 0, 400, FALSE,
		FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, 0, "Times New Roman");
	defaultFont = CreateFont(-11, 0, 0, 0, 400, FALSE, FALSE, FALSE, 1, 400, 0, 0, 0,
		"Ms Shell Dlg 2");
	largeFont = CreateFont(0, 0, 0, 0, 400, FALSE,
		FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, 0, "Times New Roman");
	i = 0;
	while(i < TOTAL_NAMES)
	{
		itemNames[i++] = (char*)malloc(1); // Just allocate something so realloc doesnt crash
	}
    // Register the window class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION); // Do this in WM_CREATE
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName  = NULL; 							// Do this in WM_CREATE
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION); // Do this in WM_CREATE
    // Report error if registration fails for some reason
    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    // Create window
    hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW | WS_EX_STATICEDGE,
    	g_szClassName, "White Choco Shop Editor",
    	WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_DLGFRAME | WS_GROUP | WS_SYSMENU,
    	CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 0, 0, hInstance, 0);
    // Report error if window creation fails for some reason
	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	// Create tab control and add entries
	tabControl = CreateWindowEx(0, WC_TABCONTROL, 0, WS_VISIBLE | WS_CHILD | WS_TABSTOP, 0,
			0, 800, 600, hwnd, (HMENU)IDC_TABCONTROL, hInstance, 0);
	SendMessage(tabControl, WM_SETFONT, (WPARAM)defaultFont, FALSE);
	TCITEM shopsTab;
	shopsTab.mask = TCIF_TEXT;
	shopsTab.pszText = "Shops";
	shopsTab.cchTextMax = 6;
	TCITEM pricesTab;
	pricesTab.mask = TCIF_TEXT;
	pricesTab.pszText = "Prices";
	pricesTab.cchTextMax = 7;
	SendMessage(tabControl, TCM_INSERTITEM, 0, (LPARAM)&shopsTab);
	SendMessage(tabControl, TCM_INSERTITEM, 1, (LPARAM)&pricesTab);
	FILE* configfile = fopen(configfileName, "r");
	char* errmsg = (char*)malloc(100*sizeof(char));
	if(configfile == NULL)
	{
		strcpy(errmsg,
			"Error finding whitechoco.cfg, make sure it's in the program directory.");
		MessageBox(hwnd, errmsg, "Error", MB_OK | MB_ICONERROR);
		perror("");
		DestroyWindow(hwnd);
	}
	int err = buildNameTable(configfile);
	fclose(configfile);
	if(err == 1)
	{
		strcpy(errmsg,
			"Error: whitechoco.cfg has too few entries; makes sure it has exactly ");
		strcat(errmsg, TOTAL_NAMES_STRING);
		strcat(errmsg, ".");
		MessageBox(hwnd, errmsg, "Error", MB_OK | MB_ICONERROR);
		DestroyWindow(hwnd);
	}
	else if(err == 2)
	{
		strcpy(errmsg,
			"Error: whitechoco.cfg has too many entries; makes sure it has exactly ");
		strcat(errmsg, TOTAL_NAMES_STRING);
		strcat(errmsg, ".");
		MessageBox(hwnd, errmsg, "Error", MB_OK | MB_ICONERROR);
		DestroyWindow(hwnd);
	}
	else if(err == -2)
	{
		strcpy(errmsg, "Error: String too large in whitechoco.cfg");
		MessageBox(hwnd, errmsg, "Error", MB_OK | MB_ICONERROR);
		DestroyWindow(hwnd);
	}
	else if(err)
	{
		strcpy(errmsg, "Unexpected Error reading whitechoco.cfg");
		MessageBox(hwnd, errmsg, "Error", MB_OK | MB_ICONERROR);
		DestroyWindow(hwnd);
	}
	free(errmsg);
    // Show and update window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    // Let's make the Prices tab be our default
    changeMode(0, hwnd);
    // Message loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
