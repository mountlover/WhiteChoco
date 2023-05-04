Public Class FF7KERNELNames

    Private CommandDesc(31) As String        'Section 9
    Private AttackDesc(255) As String        'Section 10
    Private ItemDesc(127) As String          'Section 11
    Private WeaponDesc(127) As String        'Section 12
    Private ArmorDesc(31) As String          'Section 13
    Private AccessoryDesc(31) As String      'Section 14
    Private MateriaDesc(95) As String        'Section 15
    Private KeyItemDesc(63) As String        'Section 16
    Private CommandNames(31) As String       'Section 17
    Private AttackNames(255) As String       'Section 18
    Private ItemNames(127) As String         'Section 19
    Private WeaponNames(127) As String       'Section 20
    Private ArmorNames(31) As String         'Section 21
    Private AccessoryNames(31) As String     'Section 22
    Private MateriaNames(95) As String       'Section 23
    Private KeyItemNames(63) As String       'Section 24
    Private BattleText(127) As String        'Section 25
    Private SummonAttackNames(15) As String  'Section 26

    Private OpenKERNELPieces(26)() As Byte
    Private KERNELLoc As String
    Private FoundFile As Boolean

    Private Function FF7CharacterReplace(ByVal CharacterCode As Byte) As Char
        If CharacterCode < 96 Then
            FF7CharacterReplace = Chr(CharacterCode + 32)
        Else
            Select Case Hex(CharacterCode)
                Case "60" : FF7CharacterReplace = "Ä"
                Case "61" : FF7CharacterReplace = "Á"
                Case "62" : FF7CharacterReplace = "Ç"
                Case "63" : FF7CharacterReplace = "É"
                Case "64" : FF7CharacterReplace = "Ñ"
                Case "65" : FF7CharacterReplace = "Ö"
                Case "66" : FF7CharacterReplace = "Ü"
                Case "67" : FF7CharacterReplace = "á"
                Case "68" : FF7CharacterReplace = "à"
                Case "69" : FF7CharacterReplace = "â"
                Case "6A" : FF7CharacterReplace = "ä"
                Case "6B" : FF7CharacterReplace = "ã"
                Case "6C" : FF7CharacterReplace = "å"
                Case "6D" : FF7CharacterReplace = "ç"
                Case "6E" : FF7CharacterReplace = "é"
                Case "6F" : FF7CharacterReplace = "è"
                Case "70" : FF7CharacterReplace = "ê"
                Case "71" : FF7CharacterReplace = "ë"
                Case "72" : FF7CharacterReplace = "í"
                Case "73" : FF7CharacterReplace = "ì"
                Case "74" : FF7CharacterReplace = "î"
                Case "75" : FF7CharacterReplace = "ï"
                Case "76" : FF7CharacterReplace = "ñ"
                Case "77" : FF7CharacterReplace = "ó"
                Case "78" : FF7CharacterReplace = "ò"
                Case "79" : FF7CharacterReplace = "ô"
                Case "7A" : FF7CharacterReplace = "ö"
                Case "7B" : FF7CharacterReplace = "õ"
                Case "7C" : FF7CharacterReplace = "ú"
                Case "7D" : FF7CharacterReplace = "ù"
                Case "7E" : FF7CharacterReplace = "û"
                Case "7F" : FF7CharacterReplace = "ü"
                Case "80" : FF7CharacterReplace = "Ð"
                Case "81" : FF7CharacterReplace = "°"
                Case "82" : FF7CharacterReplace = "¢"
                Case "83" : FF7CharacterReplace = "£"
                Case "84" : FF7CharacterReplace = "Ù"
                Case "85" : FF7CharacterReplace = "Û"
                Case "86" : FF7CharacterReplace = "¶"
                Case "87" : FF7CharacterReplace = "ß"
                Case "88" : FF7CharacterReplace = "®"
                Case "89" : FF7CharacterReplace = "©"
                Case "8A" : FF7CharacterReplace = "™"
                Case "8B" : FF7CharacterReplace = "´"
                Case "8C" : FF7CharacterReplace = "¨"
                Case "8D" : FF7CharacterReplace = "≠"
                Case "8E" : FF7CharacterReplace = "Æ"
                Case "8F" : FF7CharacterReplace = "Ø"
                Case "90" : FF7CharacterReplace = "∞"
                Case "91" : FF7CharacterReplace = "±"
                Case "92" : FF7CharacterReplace = "≤"
                Case "93" : FF7CharacterReplace = "≥"
                Case "94" : FF7CharacterReplace = "¥"
                Case "95" : FF7CharacterReplace = "µ"
                Case "96" : FF7CharacterReplace = "∂"
                Case "97" : FF7CharacterReplace = "Σ"
                Case "98" : FF7CharacterReplace = "Π"
                Case "99" : FF7CharacterReplace = "π"
                Case "9A" : FF7CharacterReplace = "⌡"
                Case "9B" : FF7CharacterReplace = "ª"
                Case "9C" : FF7CharacterReplace = "º"
                Case "9D" : FF7CharacterReplace = "Ω"
                Case "9E" : FF7CharacterReplace = "æ"
                Case "9F" : FF7CharacterReplace = "ø"
                Case "A0" : FF7CharacterReplace = "¿"
                Case "A1" : FF7CharacterReplace = "¡"
                Case "A2" : FF7CharacterReplace = "¬"
                Case "A3" : FF7CharacterReplace = "√"
                Case "A4" : FF7CharacterReplace = "ƒ"
                Case "A5" : FF7CharacterReplace = "≈"
                Case "A6" : FF7CharacterReplace = "∆"
                Case "A7" : FF7CharacterReplace = "«"
                Case "A8" : FF7CharacterReplace = "»"
                Case "A9" : FF7CharacterReplace = "…"
                Case "AA" : FF7CharacterReplace = "À"
                Case "AB" : FF7CharacterReplace = "Ã"
                Case "AC" : FF7CharacterReplace = "Õ"
                Case "AD" : FF7CharacterReplace = "Œ"
                Case "AE" : FF7CharacterReplace = "œ"
                Case "AF" : FF7CharacterReplace = ""
                Case "B0" : FF7CharacterReplace = "–"
                Case "B1" : FF7CharacterReplace = "—"
                Case "B2" : FF7CharacterReplace = Chr(147)
                Case "B3" : FF7CharacterReplace = Chr(148)
                    'These two just don't work.
                    'System always wants to replace these characters with '"' characters
                    'Case "B2" : FF7CharacterReplace = "“"
                    'Case "B3" : FF7CharacterReplace = "”"
                Case "B4" : FF7CharacterReplace = "‘"
                Case "B5" : FF7CharacterReplace = "’"
                Case "B6" : FF7CharacterReplace = "÷"
                Case "B7" : FF7CharacterReplace = "◊"
                Case "B8" : FF7CharacterReplace = "ÿ"
                Case "B9" : FF7CharacterReplace = "Ÿ"
                Case "BA" : FF7CharacterReplace = "⁄"
                Case "BB" : FF7CharacterReplace = "¤"
                Case "BC" : FF7CharacterReplace = "‹"
                Case "BD" : FF7CharacterReplace = "›"
                Case "BE" : FF7CharacterReplace = "ﬁ"
                Case "BF" : FF7CharacterReplace = "ﬂ"
                Case "C0" : FF7CharacterReplace = "■"
                Case "C1" : FF7CharacterReplace = "▪"
                Case "C2" : FF7CharacterReplace = "‚"
                Case "C3" : FF7CharacterReplace = "„"
                Case "C4" : FF7CharacterReplace = "‰"
                Case "C5" : FF7CharacterReplace = "Â"
                Case "C6" : FF7CharacterReplace = "Ê"
                Case "C7" : FF7CharacterReplace = "Á"
                Case "C8" : FF7CharacterReplace = "Ë"
                Case "C9" : FF7CharacterReplace = "È"
                Case "CA" : FF7CharacterReplace = "í"
                Case "CB" : FF7CharacterReplace = "î"
                Case "CC" : FF7CharacterReplace = "ï"
                Case "CD" : FF7CharacterReplace = "ì"
                Case "CE" : FF7CharacterReplace = "Ó"
                Case "CF" : FF7CharacterReplace = "Ô"
                Case "D0" : FF7CharacterReplace = " "
                Case "D1" : FF7CharacterReplace = "Ò"
                Case "D2" : FF7CharacterReplace = "Ù"
                Case "D3" : FF7CharacterReplace = "Û"
                Case "E1" : FF7CharacterReplace = vbTab
                Case "E2" : FF7CharacterReplace = ","
            End Select
        End If
    End Function

    Private Function FF7Names(ByVal NameArray() As Byte, ByVal NameIndex As Byte) As String
        Dim StartPlace As UShort
        ' These three varibles are for displaying descriptions correctly when it encounters a "F9h"
        Dim functionplace As Byte
        Dim functionlength As Byte
        Dim functionloop As Byte

        If NameIndex * 2 >= NameArray.Length - 1 Then Return vbCrLf & vbTab
        StartPlace = BitConverter.ToUInt16(NameArray, NameIndex * 2) 'NameArray(NameIndex * 2 + 1) * 256 + NameArray(NameIndex * 2)
        If StartPlace > NameArray.Length - 1 Then
            Return ""
        End If
        FF7Names = ""
        While NameArray(StartPlace) <> 255
            If NameArray(StartPlace) < 212 Then
                FF7Names &= FF7CharacterReplace(NameArray(StartPlace))
            Else
                Select Case NameArray(StartPlace)
                    Case 254
                        Select Case NameArray(StartPlace + 1)
                            Case 210
                                FF7Names &= "{Color:Gray}"
                            Case 211
                                FF7Names &= "{Color:Blue}"
                            Case 212
                                FF7Names &= "{Color:Red}"
                            Case 213
                                FF7Names &= "{Color:Purple}"
                            Case 214
                                FF7Names &= "{Color:Green}"
                            Case 215
                                FF7Names &= "{Color:Cyan}"
                            Case 216
                                FF7Names &= "{Color:Yellow}"
                            Case 217
                                FF7Names &= "{Color:White}"
                            Case 218
                                FF7Names &= "{Color:Flash}"
                            Case 219
                                FF7Names &= "{Color:Rainbow}"
                        End Select
                        StartPlace += 1
                    Case 249
                        StartPlace += 1
                        functionplace = (NameArray(StartPlace) And Val("&H3F")) + 2
                        functionlength = (NameArray(StartPlace) >> 6) * 2 + 4
                        For functionloop = 0 To functionlength - 1
                            If NameArray(StartPlace - functionplace + functionloop) < 212 Then
                                If NameArray(StartPlace - functionplace + functionloop) = 255 Then MsgBox("Problem")
                                FF7Names &= FF7CharacterReplace(NameArray(StartPlace - functionplace + functionloop))
                            ElseIf NameArray(StartPlace - functionplace + functionloop) = 255 Then
                                Exit For
                            End If
                        Next
                    Case 248
                        FF7Names &= "{BOX:"
                        Select Case NameArray(StartPlace + 1)
                            ' Eventually other cases need to be discovered
                            Case 2
                                FF7Names &= "RED"
                            Case Else
                                FF7Names &= Hex(NameArray(StartPlace + 1))
                        End Select
                        FF7Names &= "}"
                        StartPlace += 1
                    Case 179
                        FF7Names &= ChrW(8220)
                    Case 178
                        FF7Names &= ChrW(8221)
                    Case Is > 223
                        FF7Names &= "{VAR:"
                        Select Case NameArray(StartPlace)
                            Case 234
                                FF7Names &= "CharacterName"
                            Case 235
                                FF7Names &= "ItemName"
                            Case 236
                                FF7Names &= "Number"
                            Case 237
                                FF7Names &= "TargetName"
                            Case 238
                                FF7Names &= "AttackName"
                            Case 239
                                FF7Names &= "TargetLetter"
                            Case 240
                                FF7Names &= "ElementName"
                            Case Else
                                FF7Names &= NameArray(StartPlace)
                        End Select
                        FF7Names &= "}"
                        StartPlace += 2
                End Select

            End If
            StartPlace += 1
        End While
    End Function


    Public Sub New(ByVal KernelLocation As String, ByVal PSX As Boolean)
        KERNELLoc = KernelLocation
        If PSX Then
            SeparateFiles()
        Else
            SeparateKernel2()
        End If

        If Not FoundFile Then
            Throw New ApplicationException("File Not Found")
        End If
    End Sub

    Public Sub GetSectionNames(ByRef StringArray As String(), ByVal Section As Byte)
        Select Case Section
            Case 9
                If CommandDesc(0) Is Nothing Then Load_Command_Desc("9")
                StringArray = CommandDesc
            Case 10
                If AttackDesc(0) Is Nothing Then Load_Attack_Desc("10")
                StringArray = AttackDesc
            Case 11
                If ItemDesc(0) Is Nothing Then Load_Item_Desc("11")
                StringArray = ItemDesc
            Case 12
                If WeaponDesc(0) Is Nothing Then Load_Weapon_Desc("12")
                StringArray = WeaponDesc
            Case 13
                If ArmorDesc(0) Is Nothing Then Load_Armor_Desc("13")
                StringArray = ArmorDesc
            Case 14
                If AccessoryDesc(0) Is Nothing Then Load_Accessory_Desc("14")
                StringArray = AccessoryDesc
            Case 15
                If MateriaDesc(0) Is Nothing Then Load_Materia_Desc("15")
                StringArray = MateriaDesc
            Case 16
                If KeyItemDesc(0) Is Nothing Then Load_KeyItem_Desc("16")
                StringArray = KeyItemDesc
            Case 17
                If CommandNames(0) Is Nothing Then Load_Command_Names("17")
                StringArray = CommandNames
            Case 18
                If AttackNames(0) Is Nothing Then Load_Attack_Names("18")
                StringArray = AttackNames
            Case 19
                If ItemNames(0) Is Nothing Then Load_Item_Names("19")
                StringArray = ItemNames
            Case 20
                If WeaponNames(0) Is Nothing Then Load_Weapon_Names("20")
                StringArray = WeaponNames
            Case 21
                If ArmorNames(0) Is Nothing Then Load_Armor_Names("21")
                StringArray = ArmorNames
            Case 22
                If AccessoryNames(0) Is Nothing Then Load_Accessory_Names("22")
                StringArray = AccessoryNames
            Case 23
                If MateriaNames(0) Is Nothing Then Load_Materia_Names("23")
                StringArray = MateriaNames
            Case 24
                If KeyItemNames(0) Is Nothing Then Load_KeyItem_Name("24")
                StringArray = KeyItemNames
            Case 25
                If BattleText(0) Is Nothing Then Load_Battle_Text("25")
                StringArray = BattleText
            Case 26
                If SummonAttackNames(0) Is Nothing Then Load_Summon_Attack_Names("26")
                StringArray = SummonAttackNames
            Case Else
                StringArray = Nothing
        End Select
    End Sub


    Private Function GUnZip(ByRef byteCompressed() As Byte) As Byte()
        Dim objMemStream As New System.IO.MemoryStream(byteCompressed)
        Dim objGZipStream As New System.IO.Compression.GZipStream(objMemStream, IO.Compression.CompressionMode.Decompress)
        Dim sizebytes(3) As Byte
        objMemStream.Position = objMemStream.Length - 4
        objMemStream.Read(sizebytes, 0, 4)
        Dim iOutputsize As Integer = BitConverter.ToInt32(sizebytes, 0)
        objMemStream.Position = 0
        Dim decompressedbytes(iOutputsize - 1) As Byte
        objGZipStream.Read(decompressedbytes, 0, iOutputsize)
        objGZipStream.Dispose()
        objMemStream.Dispose()
        Return decompressedbytes
    End Function

    Private Sub SeparateFiles()
        FoundFile = My.Computer.FileSystem.FileExists(KERNELLoc)
        If Not FoundFile Then Exit Sub
        Dim CompressedSize As Short
        Dim UncompressedSize As Short
        Dim CompressedFileData() As Byte
        Dim FileNumber As Byte
        Dim NextFilePos As Short
        Dim FileData As Short
        'Dim ToRun As String
        Dim KERNELDATA(FileLen(KERNELLoc)) As Byte
        Dim Compressedfile As String
        'Dim pid As Integer

        KERNELDATA = My.Computer.FileSystem.ReadAllBytes(KERNELLoc)
        'ToRun = ""
        NextFilePos = 0

        For FileNumber = 0 To 26
            Compressedfile = KERNELLoc & Convert.ToString(FileNumber)
            CompressedSize = KERNELDATA(NextFilePos + 1) * 256 + KERNELDATA(NextFilePos)
            UncompressedSize = KERNELDATA(NextFilePos + 3) * 256 + KERNELDATA(NextFilePos + 2)

            NextFilePos += 6
            ReDim CompressedFileData(CompressedSize - 1)
            For FileData = 0 To CompressedSize - 1
                CompressedFileData(FileData) = KERNELDATA(NextFilePos + FileData)
            Next

            'My.Computer.FileSystem.WriteAllBytes(Compressedfile, GUnZip(CompressedFileData), False)
            'KERNELPieces(FileNumber) = CompressedFileData
            OpenKERNELPieces(FileNumber) = GUnZip(CompressedFileData)
            NextFilePos += CompressedSize
        Next
    End Sub

    Private Sub SeparateKernel2()
        Dim kernel2 As String
        Dim CurrentFile As Byte

        kernel2 = KERNELLoc

        With My.Computer.FileSystem
            FoundFile = .FileExists(kernel2)
            If Not FoundFile Then Exit Sub

            Dim CompressedFileData() As Byte
            Dim UncompressedFileData() As Byte
            Dim CompressedFileSize As UInteger
            Dim UnCompressedFileLoc As UInteger

            Dim CurrentFileData() As Byte
            Dim CurrentFileSize As UInteger
            Dim CurrentFileLoop
            Dim DataLoop As UInteger
            Dim KERNEL2DATA() As Byte

            If .FileExists(kernel2) Then
                'RadioButtonPC.Enabled = True
                ReDim KERNEL2DATA(FileLen(kernel2))
                KERNEL2DATA = .ReadAllBytes(kernel2)

                CompressedFileSize = BitConverter.ToUInt32(KERNEL2DATA, 0)
                'CompressedFileSize = KERNEL2DATA(3) * CUInt(16777216) + KERNEL2DATA(2) * 65536 + KERNEL2DATA(1) * 256 + KERNEL2DATA(0)

                ReDim CompressedFileData(CompressedFileSize - 1)
                ReDim UncompressedFileData(CompressedFileSize * 2)

                For DataLoop = 0 To CompressedFileSize - 1
                    CompressedFileData(DataLoop) = KERNEL2DATA(DataLoop + 4)
                Next

                Call UnLZS(CompressedFileData, UncompressedFileData)

                UnCompressedFileLoc = 0
                CurrentFileSize = 0

                For CurrentFile = 9 To 26
                    CurrentFileSize = BitConverter.ToUInt32(UncompressedFileData, UnCompressedFileLoc)
                    'UncompressedFileData(UnCompressedFileLoc + 3) * CUInt(16777216) + UncompressedFileData(UnCompressedFileLoc + 2) * 65536 + UncompressedFileData(UnCompressedFileLoc + 1) * 256 + UncompressedFileData(UnCompressedFileLoc)
                    ReDim CurrentFileData(CurrentFileSize - 1)
                    UnCompressedFileLoc += 4
                    For CurrentFileLoop = 0 To CurrentFileSize - 1
                        CurrentFileData(CurrentFileLoop) = UncompressedFileData(UnCompressedFileLoc + CurrentFileLoop)
                    Next
                    OpenKERNELPieces(CurrentFile) = CurrentFileData
                    UnCompressedFileLoc += CurrentFileLoop
                Next
            End If
        End With
    End Sub

    Private Sub UnLZS(ByRef CompressedFileData() As Byte, ByRef UnCompressedFileData() As Byte)
        Dim FilePos As UInteger
        Dim UCPosition As UInteger
        Dim Datablock() As Byte
        Dim datablocksize As Byte
        Dim datablockloop As Byte
        FilePos = 0

        While FilePos < CompressedFileData.Length

            datablocksize = 0

            For datablockloop = 0 To 7
                If (CompressedFileData(FilePos) And (1 << datablockloop)) Then
                    datablocksize += 1
                Else
                    datablocksize += 2
                End If
            Next

            If CompressedFileData.Length < (FilePos + datablocksize) Then
                datablocksize = CompressedFileData.Length - FilePos - 1
            End If

            ReDim Datablock(datablocksize)

            For datablockloop = 0 To datablocksize
                Datablock(datablockloop) = CompressedFileData(FilePos + datablockloop)
            Next

            Call DecodeLZSBlock(Datablock, UnCompressedFileData, FilePos, UCPosition)

        End While
        ReDim Preserve UnCompressedFileData(UCPosition)
    End Sub

    Private Sub DecodeLZSBlock(ByRef CompressedData() As Byte, ByRef UnCompressedFileData() As Byte, ByRef position As UInteger, ByRef uncompressedposition As UInteger)
        Dim Compressedblocks As Byte
        Dim compressedlocation As Integer
        Dim compressedlength As Short
        Dim compressedposition As UInteger
        Dim compressloop As Short
        Dim dataloop As Byte

        Compressedblocks = CompressedData(0)
        compressedposition = 1

        For dataloop = 0 To 7
            If compressedposition = CompressedData.Length Then
                uncompressedposition -= 1
                position += compressedposition
                Exit Sub
            End If

            If (Compressedblocks And (1 << dataloop)) Then
                UnCompressedFileData(uncompressedposition) = CompressedData(compressedposition)
                compressedposition += 1
                uncompressedposition += 1
            Else
                compressedlocation = uncompressedposition - ((uncompressedposition - 18 - (CompressedData(compressedposition) + 256 * (CompressedData(compressedposition + 1) >> 4))) And 4095)
                compressedlength = 3 + (CompressedData(compressedposition + 1) And 15)
                For compressloop = 0 To compressedlength - 1
                    If compressedlocation + compressloop < 0 Then
                        UnCompressedFileData(uncompressedposition) = 0
                    Else
                        UnCompressedFileData(uncompressedposition) = UnCompressedFileData(compressedlocation + compressloop)
                    End If
                    uncompressedposition += 1
                Next
                compressedposition += 2
            End If
        Next
        position += compressedposition
    End Sub


    Private Sub Load_Command_Names(ByVal KERNEL17 As String)
        Dim CommandNameData(OpenKernelpieces(17).Length - 1) As Byte
        Dim CommandNameRecord As Byte

        CommandNameData = OpenKernelpieces(17)
        For CommandNameRecord = 0 To 31
            CommandNames(CommandNameRecord) = FF7Names(CommandNameData, CommandNameRecord)
        Next
    End Sub

    Private Sub Load_Command_Desc(ByVal KERNEL9 As String)
        Dim CommandDescData(OpenKernelpieces(9).Length - 1) As Byte
        Dim CommandDescRecord As Byte

        CommandDescData = OpenKernelpieces(9)
        For CommandDescRecord = 0 To 31
            CommandDesc(CommandDescRecord) = FF7Names(CommandDescData, CommandDescRecord)
        Next

    End Sub

    Private Sub Load_Attack_Names(ByVal KERNEL18 As String)
        Dim AttackNameData(OpenKernelpieces(18).Length - 1) As Byte
        Dim AttackNameRecord As Short

        AttackNameData = OpenKernelpieces(18)
        For AttackNameRecord = 0 To 255
            AttackNames(AttackNameRecord) = FF7Names(AttackNameData, AttackNameRecord)
        Next

    End Sub

    Private Sub Load_Attack_Desc(ByVal KERNEL10 As String)
        'Dim AttackDescData(FileLen(KERNEL10)) As Byte
        Dim AttackDescData(OpenKernelpieces(10).Length - 1) As Byte
        Dim AttackDescRecord As Short

        'AttackDescData = My.Computer.FileSystem.ReadAllBytes(KERNEL10)
        AttackDescData = OpenKernelpieces(10)
        For AttackDescRecord = 0 To 255
            AttackDesc(AttackDescRecord) = FF7Names(AttackDescData, AttackDescRecord)
        Next

    End Sub

    Private Sub Load_Item_Names(ByVal KERNEL19 As String)
        Dim ItemNameData(OpenKernelpieces(19).Length - 1) As Byte
        Dim ItemNameRecord As Byte

        ItemNameData = OpenKernelpieces(19)
        For ItemNameRecord = 0 To 127
            ItemNames(ItemNameRecord) = FF7Names(ItemNameData, ItemNameRecord)
        Next

    End Sub

    Private Sub Load_Item_Desc(ByVal KERNEL11 As String)
        'Dim ItemDescData(FileLen(KERNEL11)) As Byte
        Dim ItemDescData(OpenKernelpieces(11).Length - 1) As Byte
        Dim ItemDescRecord As Byte

        'ItemDescData = My.Computer.FileSystem.ReadAllBytes(KERNEL11)
        ItemDescData = OpenKernelpieces(11)
        For ItemDescRecord = 0 To 127
            ItemDesc(ItemDescRecord) = FF7Names(ItemDescData, ItemDescRecord)
        Next

    End Sub

    Private Sub Load_Weapon_Names(ByVal KERNEL20 As String)
        Dim WeaponNameData(OpenKernelpieces(20).Length - 1) As Byte
        Dim WeaponNameRecord As Byte

        WeaponNameData = OpenKernelpieces(20)
        For WeaponNameRecord = 0 To 127
            WeaponNames(WeaponNameRecord) = FF7Names(WeaponNameData, WeaponNameRecord)
        Next

    End Sub

    Private Sub Load_Weapon_Desc(ByVal KERNEL12 As String)
        'Dim WeaponDescData(FileLen(KERNEL12)) As Byte
        Dim WeaponDescData(OpenKernelpieces(12).Length - 1) As Byte
        Dim WeaponDescRecord As Byte

        'WeaponDescData = My.Computer.FileSystem.ReadAllBytes(KERNEL12)
        WeaponDescData = OpenKernelpieces(12)
        For WeaponDescRecord = 0 To 127
            WeaponDesc(WeaponDescRecord) = FF7Names(WeaponDescData, WeaponDescRecord)
        Next

    End Sub

    Private Sub Load_Armor_Names(ByVal KERNEL21 As String)
        Dim ArmorNameData(OpenKernelpieces(21).Length - 1) As Byte
        Dim ArmorNameRecord As Byte

        ArmorNameData = OpenKernelpieces(21)
        For ArmorNameRecord = 0 To 31
            ArmorNames(ArmorNameRecord) = FF7Names(ArmorNameData, ArmorNameRecord)
        Next

    End Sub

    Private Sub Load_Armor_Desc(ByVal KERNEL13 As String)
        Dim Armordescdata(OpenKernelpieces(13).Length - 1) As Byte
        Dim ArmorDescRecord As Byte

        Armordescdata = OpenKernelpieces(13)
        For ArmorDescRecord = 0 To 31
            ArmorDesc(ArmorDescRecord) = FF7Names(Armordescdata, ArmorDescRecord)
        Next

    End Sub

    Private Sub Load_Accessory_Names(ByVal KERNEL22 As String)
        Dim AccessoryNameData(OpenKernelpieces(22).Length - 1) As Byte
        Dim AccessoryNameRecord As Byte

        AccessoryNameData = OpenKernelpieces(22)
        For AccessoryNameRecord = 0 To 31
            AccessoryNames(AccessoryNameRecord) = FF7Names(AccessoryNameData, AccessoryNameRecord)
        Next

    End Sub

    Private Sub Load_Accessory_Desc(ByVal KERNEL14 As String)
        Dim AccessoryDescData(OpenKernelpieces(14).Length - 1) As Byte
        Dim AccessoryDescRecord As Byte

        AccessoryDescData = OpenKernelpieces(14)
        For AccessoryDescRecord = 0 To 31
            AccessoryDesc(AccessoryDescRecord) = FF7Names(AccessoryDescData, AccessoryDescRecord)
        Next

    End Sub

    Private Sub Load_Materia_Names(ByVal KERNEL23 As String)
        Dim MateriaNameData(OpenKernelpieces(23).Length - 1) As Byte
        Dim MateriaNameRecord As Byte

        MateriaNameData = OpenKernelpieces(23)
        For MateriaNameRecord = 0 To 95
            MateriaNames(MateriaNameRecord) = FF7Names(MateriaNameData, MateriaNameRecord)
        Next

    End Sub

    Private Sub Load_Materia_Desc(ByVal KERNEL15 As String)
        Dim MateriaDescData(OpenKernelpieces(15).Length - 1) As Byte
        Dim MateriaDescRecord As Byte

        MateriaDescData = OpenKernelpieces(15)
        For MateriaDescRecord = 0 To 95
            MateriaDesc(MateriaDescRecord) = FF7Names(MateriaDescData, MateriaDescRecord)
        Next

    End Sub

    Private Sub Load_KeyItem_Desc(ByVal KERNEL16 As String)
        Dim KeyItemDescData(OpenKernelpieces(16).Length - 1) As Byte
        Dim KeyItemDescRecord As Byte
        'Dim KeyItemDesc(63) As String

        KeyItemDescData = OpenKernelpieces(16)
        For KeyItemDescRecord = 0 To 63
            KeyItemDesc(KeyItemDescRecord) = FF7Names(KeyItemDescData, KeyItemDescRecord)
        Next
    End Sub

    Private Sub Load_KeyItem_Name(ByVal KERNEL24 As String)
        Dim KeyItemNameData(OpenKernelpieces(24).Length - 1) As Byte
        Dim KeyItemNameRecord As Byte
        'Dim KeyItemName(63) As String

        KeyItemNameData = OpenKernelpieces(24)
        For KeyItemNameRecord = 0 To 63
            KeyItemNames(KeyItemNameRecord) = FF7Names(KeyItemNameData, KeyItemNameRecord)
        Next
    End Sub

    Private Sub Load_Battle_Text(ByVal KERNEL25 As String)
        Dim BattleTextData(OpenKernelpieces(25).Length - 1) As Byte
        'Dim BattleText(127) As String
        Dim BattleTextRecord As Byte

        BattleTextData = OpenKernelpieces(25)
        For BattleTextRecord = 0 To 127
            BattleText(BattleTextRecord) = FF7Names(BattleTextData, BattleTextRecord)
        Next

    End Sub

    Private Sub Load_Summon_Attack_Names(ByVal KERNEL26 As String)
        Dim SummonAttackNamesData(OpenKernelpieces(26).Length - 1) As Byte
        'Dim SummonAttackNames(15) As String
        Dim SummonAttackNamesRecord As Byte

        SummonAttackNamesData = OpenKernelpieces(26)
        For SummonAttackNamesRecord = 0 To 15
            SummonAttackNames(SummonAttackNamesRecord) = FF7Names(SummonAttackNamesData, SummonAttackNamesRecord)
        Next

    End Sub

End Class
