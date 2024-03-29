/*
This contains a single class with two public functions:
Class name is FF7KNames
Constructor: FF7KNames(filepath, PSX)
	filepath: file path to location of kernel file to read
	PSX: boolean value indicating this is the PSX kernel
Method: GetSectionNames(empty_char[] , section)
	empty_char[]: Pointer to an empty array of chars that will be filled.
				  When this function is finished, the empty_char will be
				  filled with all texts in the kernel section.
	section: which kernel section's text to read
*/

#include <string.h>
#include <stdlib.h>
namespace FF7KERNELNamesC {

	char *FF7NameConsts[] = {"{VAR:ElementName}",
							 "{VAR:TargetLetter}",
							 "{VAR:AttackName}",
							 "{VAR:TargetName}",
							 "{VAR:Number}",
							 "{VAR:ItemName}",
							 "{VAR:CharacterName}"};
	char *FF7ColorConsts[] = {"{Color:Gray}",
                              "{Color:Blue}",
                              "{Color:Red}",
                              "{Color:Purple}",
                              "{Color:Green}",
                              "{Color:Cyan}",
                              "{Color:Yellow}",
                              "{Color:White}",
                              "{Color:Flash}",
							  "{Color:Rainbow}"};


	public ref class FF7KNames
	{
	public:
		FF7KNames(char*, bool);
		void GetSectionNames(char*, unsigned char);
	private:
		char ** CommandDesc;
		char ** AttackDesc;
		char ** ItemDesc;
		char ** WeaponDesc;
		char ** ArmorDesc;
		char ** AccessoryDesc;
		char ** MateriaDesc;
		char ** KeyItemDesc;
		char ** CommandNames;
		char ** AttackNames;
		char ** ItemNames;
		char ** WeaponNames;
		char ** ArmorNames;
		char ** AccessoryNames;
		char ** MateriaNames;
		char ** KeyItemNames;
		char ** BattleText;
		char ** SummonAttackNames;

		char ** OpenKERNELPieces;
		char * KERNELLoc;
		bool FoundFile;

		const char FF7ReplaceChar(unsigned char);
		char * FF7Names(const char*, unsigned char);
		void SeparateFiles();
		void SeparateKernel2();
		void UnLZS(unsigned char Compressed[], unsigned char UnCompressed[]);
		void DecodeLZSBlock(unsigned char CompressedBlock[], unsigned char UncompressedBlock[]);
		void Load_Item_Names();
		void Load_Item_Descs();
	};

	FF7KNames::FF7KNames(char * KERNELL, bool PSX) {
		CommandDesc = new char*[32];
		AttackDesc = new char*[256];
		ItemDesc = new char*[128];
		WeaponDesc = new char*[128];
		ArmorDesc = new char*[32];
		AccessoryDesc = new char*[32];
		MateriaDesc = new char*[96];
		KeyItemDesc = new char*[64];
		CommandNames = new char*[16];
		AttackNames = new char*[256];
		ItemNames = new char*[128];
		WeaponNames = new char*[128];
		ArmorNames = new char*[32];
		AccessoryNames = new char*[32];
		MateriaNames = new char*[96];
		KeyItemNames = new char*[64];
		BattleText = new char*[128];
		SummonAttackNames = new char*[16];

		OpenKERNELPieces = new char*[27];
		for (int x = 0; x< 27; x++)
			// they won't be this large, but there needs to be a limit to the size
			OpenKERNELPieces[x] = new char[10000];

		KERNELLoc = KERNELL;
		if (PSX)
		{
			SeparateFiles();
		}
		else
		{
			SeparateKernel2();
		}
	}

	void FF7KNames::GetSectionNames(char *StringArray, unsigned char Section)
	{
	}

	const char FF7KNames::FF7ReplaceChar(unsigned char Code)
	{
		if (Code < 96)
            return (char) (Code + 32);
		else
		{
			switch ((int) Code)
			{
                case 0x60: 
					return 'Ä';
			    case 0x61: 
					return 'Á';
                case 0x62: 
					return 'Ç';
                case 0x63: 
					return 'É';
                case 0x64: 
					return 'Ñ';
                case 0x65: 
					return 'Ö';
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
                case 0x8D : return '≠';
                case 0x8E : return 'Æ';
                case 0x8F : return 'Ø';
                case 0x90 : return '∞';
                case 0x91 : return '±';
                case 0x92 : return '≤';
                case 0x93 : return '≥';
                case 0x94 : return '¥';
                case 0x95 : return 'µ';
                case 0x96 : return '∂';
                case 0x97 : return 'Σ';
                case 0x98 : return 'Π';
                case 0x99 : return 'π';
                case 0x9A : return '⌡';
                case 0x9B : return 'ª';
                case 0x9C : return 'º';
                case 0x9D : return 'Ω';
                case 0x9E : return 'æ';
                case 0x9F : return 'ø';
                case 0xA0 : return '¿';
                case 0xA1 : return '¡';
                case 0xA2 : return '¬';
                case 0xA3 : return '√';
                case 0xA4 : return 'ƒ';
                case 0xA5 : return '≈';
                case 0xA6 : return '∆';
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
                case 0xB2 : return (char) 147;
                case 0xB3 : return (char) 148;
                    //These two just don't work.
                    //System always wants to replace these characters with ''' characters
                    //case 0xB2 : return '“'
                    //case 0xB3 : return '”'
                case 0xB4 : return '‘';
                case 0xB5 : return '’';
                case 0xB6 : return '÷';
                case 0xB7 : return '◊';
                case 0xB8 : return 'ÿ';
                case 0xB9 : return 'Ÿ';
                case 0xBA : return '⁄';
                case 0xBB : return '¤';
                case 0xBC : return '‹';
                case 0xBD : return '›';
                case 0xBE : return 'ﬁ';
                case 0xBF : return 'ﬂ';
                case 0xC0 : return '■';
                case 0xC1 : return '▪';
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
	}


	
	char *FF7KNames::FF7Names(const char *NameArray, unsigned char NameIndex)
	{
		char GoodName[100];
		char *NewChar;
		char LoopChar;
		unsigned short StartPlace;
		unsigned char functionplace, functionloop, functionlength;

		StartPlace = (int)NameArray[NameIndex * 2] + (int)NameArray[NameIndex*2 + 1] * 256;

		while((int)NameArray[StartPlace] != 255)
		{
			if ((int)NameArray[StartPlace] < 212)
				*NewChar =FF7ReplaceChar((unsigned char)NameArray[StartPlace]);
			else
			{
				switch ((int)NameArray[StartPlace])
				{
					case 254:
					{
                        switch ((int)NameArray[StartPlace + 1])
						{
							case 210 :
                                NewChar = FF7ColorConsts[0];
							case 211:
                                NewChar = FF7ColorConsts[1];
							case 212:
                                NewChar = FF7ColorConsts[2];
							case 213:
                                NewChar = FF7ColorConsts[3];
							case 214:
                                NewChar = FF7ColorConsts[4];
							case 215:
                                NewChar = FF7ColorConsts[5];
							case 216:
                                NewChar = FF7ColorConsts[6];
							case 217:
                                NewChar = FF7ColorConsts[7];
							case 218:
                                NewChar = FF7ColorConsts[8];
							case 219:
                                NewChar = FF7ColorConsts[9];
						}
						StartPlace++;
						break;
					}
					case 249:
					{
						StartPlace++;
                        functionplace = ((int)NameArray[StartPlace] & 0x3F) + 2;
                        functionlength = ((int)NameArray[StartPlace] >> 6) * 2 + 4;
                        for (functionloop = 0; functionloop < functionlength; functionloop++)
						{
                            if ((int)NameArray[StartPlace - functionplace + functionloop] < 212)
							{
								LoopChar = FF7ReplaceChar((int)NameArray[StartPlace - functionplace + functionloop]);
                                strcat(NewChar, &LoopChar);
							}
							else if ((int)NameArray[StartPlace - functionplace + functionloop] == 255)
								{break;}
                        }
						break;
					}
					case 248:
					{
                        NewChar = "{BOX:";
                        switch ((int)NameArray[StartPlace + 1])
						{
                            // Eventually other cases need to be discovered
						case 2 :
                                strcat(NewChar, "RED");
						default :
                                itoa((int)NameArray[StartPlace + 1],NewChar,16);
						}
                        strcat(NewChar, "}");
						StartPlace++;
						break;
					}
					case 247:
					case 246:
					case 245:
					case 244:
					case 243:
					case 242:
					case 241:
					case 233:
					case 232:
					case 231:
					case 230:
					case 229:
					case 228:
					case 227:
					case 226:
					case 225:
					case 224:
					{
						strcpy(NewChar,"{VAR:");
						strcat(NewChar,&NameArray[StartPlace]);
						strcat(NewChar,"}");
						StartPlace += 2;
						break;
					}
					case 240:
					{
						NewChar = FF7NameConsts[0];
						StartPlace += 2;
						break;
					}
					case 239:
					{
						NewChar = FF7NameConsts[1];
						StartPlace += 2;
						break;
					}
					case 238:
					{
						NewChar = FF7NameConsts[2];
						StartPlace += 2;
						break;
					}
					case 237:
					{
						NewChar = FF7NameConsts[3];
						StartPlace += 2;
						break;
					}
					case 236:
					{
						NewChar = FF7NameConsts[4];
						StartPlace += 2;
						break;
					}
					case 235:
					{
						NewChar = FF7NameConsts[5];
						StartPlace += 2;
						break;
					}
					case 234:
					{
						NewChar = FF7NameConsts[6];
						StartPlace += 2;
						break;
					}
				}
			}
			strcat(GoodName, NewChar);
			StartPlace++;
		}

		return GoodName;
	}

	void FF7KNames::Load_Item_Descs() //Section 11
	{		
        const char *ItemDescData = OpenKERNELPieces[11];
        int ItemDescRecord;

		ItemDesc = new char*[128];
        for (ItemDescRecord = 0; ItemDescRecord < 128; ItemDescRecord++)
			ItemDesc[ItemDescRecord] = FF7Names(ItemDescData, ItemDescRecord);

	}

	void FF7KNames::Load_Item_Names() //Section 19
	{		
        const char *ItemNameData = OpenKERNELPieces[19];
        int ItemNameRecord;

		ItemNames = new char*[128];
        for (ItemNameRecord = 0; ItemNameRecord < 128; ItemNameRecord++)
			ItemNames[ItemNameRecord] = FF7Names(ItemNameData, ItemNameRecord);

	}

}