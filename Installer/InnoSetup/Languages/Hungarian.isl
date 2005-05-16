; *** Inno Setup version 4.2.2+ Hungarian messages ***
;
; To download user-contributed translations of this file, go to:
;   http://www.jrsoftware.org/is3rdparty.php
;
; Note: When translating this text, do not add periods (.) to the end of
; messages that didn't have them already, because on those messages Inno
; Setup adds the periods automatically (appending a period would result in
; two periods being displayed).
;
; $jrsoftware: issrc/Files/Default.isl,v 1.58 2004/04/07 20:17:13 jr Exp $

[LangOptions]
LanguageName=Magyar
LanguageID=$0409
LanguageCodePage=0
; If the language you are translating to requires special font faces or
; sizes, uncomment any of the following entries and change them accordingly.
;DialogFontName=
;DialogFontSize=8
;WelcomeFontName=Verdana
;WelcomeFontSize=12
;TitleFontName=Arial
;TitleFontSize=29
;CopyrightFontName=Arial
;CopyrightFontSize=8

[Messages]

; *** Application titles
SetupAppTitle=Telep�t�s
SetupWindowTitle=Telep�t�s - %1
UninstallAppTitle=Elt�vol�t�s
UninstallAppFullTitle=%1 Elt�vol�t�s

; *** Misc. common
InformationTitle=Inform�ci�
ConfirmTitle=Meger�s�t�s
ErrorTitle=Hiba

; *** SetupLdr messages
SetupLdrStartupMessage=Ezzel megkezded a %1 telep�t�s�t. K�v�nod folytatni?
LdrCannotCreateTemp=Nem lehet l�trehozni az ideiglenes f�jlokat. A telep�t�s megszakadt
LdrCannotExecTemp=Nem lehet futtatni az ideiglenes f�jlokat. A telep�t�s megszakadt

; *** Startup error messages
LastErrorMessage=%1.%n%nHiba %2: %3
SetupFileMissing=Ez a f�jl: '%1' hi�nyzik a telep�t�si k�nyvt�rb�l. K�rlek, ellen�rizd a hib�t, �s ha sz�ks�ges, szerezz be egy �j p�ld�nyt a programb�l.
SetupFileCorrupt=A telep�t� f�jlok hib�sak. K�rlek, szerezz be egy �j p�ld�nyt a programb�l.
SetupFileCorruptOrWrongVer=A telepit� f�jlok hib�sak, vagy nem kompatibilisak a Telep�t�vel. K�rlek, ellen�rizd a hib�t, �s ha sz�ks�ges, szerezz be egy �j p�ld�nyt a programb�l.
NotOnThisPlatform=Ez a program nem fog futni %1 rendszeren.
OnlyOnThisPlatform=Ezt a programot csak %1 rendszeren lehet futtatni.
WinVersionTooLowError=A programhoz a %1 rendszer sz�ks�ges legal�bb %2 verzi�sz�mmal.
WinVersionTooHighError=A program nem telep�thet� %1 rendszer %2, vagy �jabb verzi�j�n.
AdminPrivilegesRequired=A program telep�t�shez rendszergazdak�nt kell bejelentkezni.
PowerUserPrivilegesRequired=A program telep�t�shez rendszergazdak�nt, vagy a telep�t�sre jogosult felhaszn�l�k�nt sz�ks�ges bejelentkezni.
SetupAppRunningError=A Telep�t� a %1 program fut�s�t �rz�kelte.%n%nK�rlek, z�rj be minden �ppen fut� p�ld�nyt majd nyomd meg a Rendben gombot a folytat�shoz, vagy a M�gsem gombot a kil�p�shez.
UninstallAppRunningError=Az Elt�vol�t�s a %1 program fut�s�t �rz�kelte.%n%nK�rlek, z�rj be minden �ppen fut� p�ld�nyt majd nyomd meg a Rendben gombot a folytat�shoz, vagy a M�gsem gombot a kil�p�shez.

; *** Misc. errors
ErrorCreatingDir=A Telep�t� nem tudta l�trehozni a k�vetkez� k�nyvt�rat: "%1"
ErrorTooManyFilesInDir=Nem lehetett l�trehozni a(z) "%1" f�jlt, mivel a c�lk�nyvt�r t�l sok f�jlt tartalmaz

; *** Setup common messages
ExitSetupTitle=Kil�p�s a Telep�t�b�l
ExitSetupMessage=A Telep�t�s nem fejez�d�tt be. Amennyiben most kil�psz, a program nem fog m�k�dni.%n%nTerm�szetesen k�s�bb b�rmikor �jrafuttathatod a Telep�t�t.%n%nKil�psz a Telep�t�b�l?
AboutSetupMenuItem=A &Telep�t�r�l...
AboutSetupTitle=A Telep�t�r�l
AboutSetupMessage=%1 verzi�: %2%n%3%n%n%1 Honlap:%n%4
AboutSetupNote=Magyar ford�t�s: M�rton Bal�zs

; *** Buttons
ButtonBack=< &Vissza
ButtonNext=&Tov�bb >
ButtonInstall=&Telep�t�s
ButtonOK=Rendben
ButtonCancel=M�gsem
ButtonYes=&Igen
ButtonYesToAll=Igen &Mindenre
ButtonNo=&Nem
ButtonNoToAll=N&em Mindenre
ButtonFinish=&Befejez�s
ButtonBrowse=&Tall�z�s...
ButtonWizardBrowse=T&all�z�s...
ButtonNewFolder=&�j k�nyvt�r l�trehoz�sa

; *** "Select Language" dialog messages
SelectLanguageTitle=V�laszd ki a Telep�t� nyelv�t
SelectLanguageLabel=V�laszd ki a telep�t�s alatt haszn�lni k�v�nt nyelvet:

; *** Common wizard text
ClickNext=Kattints a Tov�bb gombra a folytat�shoz, a M�gsem gombra a kil�p�shez.
BeveledLabel=
BrowseDialogTitle=K�nyvt�rak tall�z�sa
BrowseDialogLabel=V�lassz egy k�nyvt�rat a list�b�l, majd nyomd meg a Rendben gombot.
NewFolderName=�j k�nyvt�r

; *** "Welcome" wizard page
WelcomeLabel1=�dv�z�l a [name] Telep�t� Var�zsl�
WelcomeLabel2=Ezzel megkezded a [name/ver] telep�t�s�t a sz�m�t�g�pre.%n%nTan�csos bez�rni minden m�s fut� programot a folytat�s el�tt.

; *** "Password" wizard page
WizardPassword=Jelsz�
PasswordLabel1=A telep�t� program jelsz�val v�dett.
PasswordLabel3=K�rlek, add meg a jelsz�t, majd nyomd meg a Tov�bb gombot. A program �rz�keny a kis-nagybet�kre.
PasswordEditLabel=&Jelsz�:
IncorrectPassword=A megadott jelsz� nem megfelel�. K�rlek, add meg �jra.

; *** "License Agreement" wizard page
WizardLicense=Licensz Szerz�d�s
LicenseLabel=K�rlek, olvasd el a k�vetkez� fontos inform�ci�t folytat�s el�tt.
LicenseLabel3=K�rlek, olvasd el a k�vetkez� Licensz Szerz�d�st. A telep�t�s folytat�s�hoz el kell fogadnod a szerz�d�s felt�teleit.
LicenseAccepted=&Elfogadom a Szerz�d�st
LicenseNotAccepted=&Nem fogadom el a szerz�d�st

; *** "Information" wizard pages
WizardInfoBefore=Inform�ci�
InfoBeforeLabel=K�rlek, olvasd el a k�vetkez� fontos inform�ci�t folytat�s el�tt.
InfoBeforeClickLabel=Amennyiben k�szen �llsz a Telep�t�s folytat�s�ra, kattints a Tov�bb gombra.
WizardInfoAfter=Inform�ci�
InfoAfterLabel=K�rlek, olvasd el a k�vetkez� fontos inform�ci�t folytat�s el�tt.
InfoAfterClickLabel=Amennyiben k�szen �llsz a Telep�t�s folytat�s�ra, kattints a Tov�bb gombra.

; *** "User Information" wizard page
WizardUserInfo=Felhaszn�l�i adatok
UserInfoDesc=K�rlek, ad meg az adataidat.
UserInfoName=&Felhaszn�l�i n�v:
UserInfoOrg=&V�llalat:
UserInfoSerial=&Sz�ria sz�m:
UserInfoNameRequired=Meg kell adnod egy nevet.

; *** "Select Destination Location" wizard page
WizardSelectDir=Add meg a telep�t�si c�lt
SelectDirDesc=Hol legyen a(z) [name] telep�tve?
SelectDirLabel3=A Telep�t� a(z) [name] programot ide fogja telep�teni.
SelectDirBrowseLabel=A folytat�shoz nyomd meg a Tov�bb gombot. Amennyiben m�s k�nyvt�rat szeretn�l v�lasztani, kattints a Tall�z�s gombra.
DiskSpaceMBLabel=Legal�bb [mb] MB szabad hely sz�ks�ges.
ToUNCPathname=A Telep�t� nem tud UNC el�r�si �ton telep�teni. Amennyiben h�l�zatra szeretn�d telep�teni a programot, hozz l�tre egy h�l�zati meghajt�t.
InvalidPath=Teljes el�r�si utat kell megadnod, pl.:%n%nC:\APP%n%nvagy egy UNC el�r�si utat, pl.:%n%n\\server\share
InvalidDrive=A meghajt� vagy UNC el�r�si �t nem l�tezik. K�rlek, adj meg egy m�sikat.
DiskSpaceWarningTitle=Nincs elegend� szabad hely
DiskSpaceWarning=A Telep�t� legal�bb %1 KB szabad helyet ig�nyel a telep�t�shez, de a megadott meghajt�n csak %2 KB szabad hely van.%n%nEnnek ellen�re folytatni akarod?
DirNameTooLong=A k�nyvt�r neve vagy az el�r�si �t t�l hossz�.
InvalidDirName=A k�nyvt�r neve �rv�nytelen.
BadDirName32=A k�nyvt�r neve nem tartalmazhatja a k�vetkez� karaktereket:%n%n%1
DirExistsTitle=A k�nyvt�r m�r l�tezik
DirExists=Ez a k�nyvt�r %n%n(%1)%n%nm�r l�tezik. Ennek ellen�re szeretn�d ide telep�teni a programot?
DirDoesntExistTitle=A k�nyvt�r nem l�tezik
DirDoesntExist=Ez a k�nyvt�r:%n%n(%1)%n%n m�g nem l�tezik. Szeretn�d, ha most l�trehozn�m ezt a k�nyvt�rat?

; *** "Select Components" wizard page
WizardSelectComponents=V�laszd ki az �sszetev�ket
SelectComponentsDesc=Mely �sszetev�k legyenek telep�tve?
SelectComponentsLabel2=V�laszd ki a telep�teni k�v�nt �sszetev�ket; Hagyd �resen azokat az �sszetev�ket, amelyeket nem szeretn�l telep�teni. Amennyiben k�sz vagy, kattints a Tov�bb gombra.
FullInstallation=Teljes telep�t�s
; if possible don't translate 'Compact' as 'Minimal' (I mean 'Minimal' in your language)
CompactInstallation=Kompakt telep�t�s
CustomInstallation=Egyedi telep�t�s
NoUninstallWarningTitle=Az �sszetev� m�r l�tezik
NoUninstallWarning=A Telep�t�s �szlelte a k�vetkez� �sszetev�ket a sz�m�t�g�pen:%n%n%1%n%nAmennyiben �resen hagyod �ket, nem lesznek telep�tve.%n%nEnnek ellen�re folytatni akarod?
ComponentSize1=%1 KB
ComponentSize2=%1 MB
ComponentsDiskSpaceMBLabel=A jelenlegi �ssze�ll�t�s [mb] MB szabad helyet ig�nyel.

; *** "Select Additional Tasks" wizard page
WizardSelectTasks=V�laszd ki a tov�bbi feladatokat
SelectTasksDesc=Milyen tov�bbi feladatok legyenek v�grehajtva?
SelectTasksLabel2=V�laszd ki azokat a tov�bbi feladatokat, melyek a(z) [name] telep�t�sekor lesznek v�grehajtva, majd nyomd meg a Tov�bb gombot.

; *** "Select Start Menu Folder" wizard page
WizardSelectProgramGroup=V�laszd ki a Start Men� mapp�t
SelectStartMenuFolderDesc=Hol helyezze el a Telep�t� a parancsikonokat?
SelectStartMenuFolderLabel3=A Telep�t� a k�vetkez� Start Men� mapp�ban helyezi majd el a parancsikonokat.
SelectStartMenuFolderBrowseLabel=A folytat�shoz kattints a Tov�bb gombra. Amennyiben m�s mapp�t szeretn�l megadni, kattints a Tall�z�s gombra.
NoIconsCheck=&Ne hozzon l�tre parancsikonokat
MustEnterGroupName=Meg kell adnod egy mapp�t.
GroupNameTooLong=A mappa neve, vagy el�r�si �tja t�l hossz�.
InvalidGroupName=A mappa neve �rv�nytelen.
BadGroupName=A mappa neve nem tartalmazhatja a k�vetkez� karaktereket:%n%n%1
NoProgramGroupCheck2=N&e hozzon l�tre mapp�t a Start Men�ben

; *** "Ready to Install" wizard page
WizardReady=K�szen �llsz a Telep�t�sre
ReadyLabel1=A Telep�t� k�szen �ll a(z) [name] telep�t�s�re a sz�m�t�g�pre.
ReadyLabel2a=Kattints a Telep�t�s gombra a telep�t�s megkezd�s�hez, vagy a Vissza gombra a be�ll�t�said m�dos�t�s�hoz.
ReadyLabel2b=Kattints a Telep�t�s gombra a telep�t�s megkezd�s�hez.
ReadyMemoUserInfo=Felhaszn�l�i adatok:
ReadyMemoDir=Telep�t�s helye:
ReadyMemoType=Telep�t�s t�pusa:
ReadyMemoComponents=Kiv�lasztott komponensek:
ReadyMemoGroup=Start Men� mappa:
ReadyMemoTasks=Tov�bbi feladatok:

; *** "Preparing to Install" wizard page
WizardPreparing=Felk�sz�l�s a telep�t�sre
PreparingDesc=A Telep�t� felk�sz�l a(z) [name] telep�t�s�re a sz�m�t�g�pen.
PreviousInstallNotCompleted=A Telep�t�s/Elt�vol�t�s m�g nem fejez�d�tt be. A befejez�shez a sz�m�t�g�p �jraind�t�sa sz�ks�ges.%n%nA sz�m�t�g�p �jraind�t�sa ut�n futtasd �jra a Telep�t�t a(z) [name] telep�t�s�hez.
CannotContinue=A Telep�t�s nem folytat�dhat. Nyomd meg a M�gsem gombot a kil�p�shez.

; *** "Installing" wizard page
WizardInstalling=Telep�t�s
InstallingLabel=K�rlek v�rj, am�g a Telep�t� elv�gzi a(z) [name] telep�t�s�t a sz�m�t�g�pre.

; *** "Setup Completed" wizard page
FinishedHeadingLabel=A(z) [name] Telep�t� Var�zsl� el�k�sz�t�se
FinishedLabelNoIcons=A Telep�t� befejezte a(z) [name] telep�t�s�t a sz�m�t�g�pre.
FinishedLabel=A Telep�t� befejezte a(z) [name] telep�t�s�t a sz�m�t�g�pre. A program a telep�tett ikonokra kattintva futtathat�.
ClickFinish=A kil�p�shez nyomd meg a Befejez�s gombot.
FinishedRestartLabel=A(z) [name] telep�t�s�nek befejez�s�hez, a Telep�t�nek �jra kell ind�tania a sz�m�t�g�pet. Szeretn�d ezt most megtenni?
FinishedRestartMessage=A(z) [name] telep�t�s�nek befejez�s�hez, a Telep�t�nek �jra kell ind�tania a sz�m�t�g�pet.%n%nSzeretn�d ezt most megtenni?
ShowReadmeCheck=Igen, szeretn�m elolvasni a README f�jlt
YesRadio=&Igen, ind�tsa �jra most a sz�m�t�g�pet
NoRadio=&Nem, majd k�s�bb ind�tom �jra a sz�m�t�g�pet
; used for example as 'Run MyProg.exe'
RunEntryExec=A(z) %1 futtat�sa
; used for example as 'View Readme.txt'
RunEntryShellExec=A(z) %1 megtekint�se

; *** "Setup Needs the Next Disk" stuff
ChangeDiskTitle=A Telep�t�nek sz�ks�ge van a k�vetkez� lemezre
SelectDiskLabel2=K�rlek tedd be a %1. sz�m� lemezt �s nyomd meg a Rendben gombot.%n%nAmennyiben ezen a lemezen a f�jlok m�s el�r�si �ton tal�lhat�ak meg, mint a lenti, k�rlek add meg a helyes �tvonalat, vagy kattints a Tall�z�s gombra.
PathLabel=&�tvonal:
FileNotInDir2=A(z) "%1" nev� f�jl nem tal�lhat� meg a(z) "%2" �tvonalon. K�rlek, tedd be a megfelel� lemezt vagy add meg a helyes el�r�si �tvonalat.
SelectDirectoryLabel=K�rlek, add meg a k�vetkez� lemez el�r�si �tvonal�t.

; *** Installation phase messages
SetupAborted=A Telep�t�s nem fejez�d�tt be.%n%nK�rlek, jav�tsd ki a hib�t �s futtasd �jra a Telep�t�t.
EntryAbortRetryIgnore=Nyomd meg az �jra gombot az �jb�li pr�b�lkoz�shoz, a Kihagy gombot a folytat�shoz, vagy a M�gsem gombot a telep�t�s befejez�s�hez.

; *** Installation status messages
StatusCreateDirs=K�nyvt�rak l�trehoz�sa...
StatusExtractFiles=F�jlok kibont�sa...
StatusCreateIcons=Parancsikonok l�trehoz�sa...
StatusCreateIniEntries=INI bejegyz�sek l�trehoz�sa...
StatusCreateRegistryEntries=Regisztr�ci�s bejegyz�sek l�trehoz�sa...
StatusRegisterFiles=F�jlok regisztr�l�sa...
StatusSavingUninstall=Elt�vol�t�si inform�ci�k ment�se...
StatusRunProgram=Telep�t�s befejez�se...
StatusRollback=V�ltoztat�sok visszafejt�se...

; *** Misc. errors
ErrorInternal2=Bels� hiba: %1
ErrorFunctionFailedNoCode=A(z) %1 sikertelen volt
ErrorFunctionFailed=A(z) %1 sikertelen volt; k�d: %2
ErrorFunctionFailedWithMessage=A(z) %1 sikertelen volt; k�d: %2.%n%3
ErrorExecutingProgram=Nem lehet futtatni a k�vetkez� f�jlt:%n%1

; *** Registry errors
ErrorRegOpenKey=Nem lehetett megnyitni a k�vetkez� regisztr�ci�s bejegyz�st:%n%1\%2
ErrorRegCreateKey=Nem lehetett l�trehozni a k�vetkez� regisztr�ci�s bejegyz�st:%n%1\%2
ErrorRegWriteKey=Nem lehetett �rni a k�vetkez� regisztr�ci�s bejegyz�st:%n%1\%2

; *** INI errors
ErrorIniEntry=Nem lehetett l�trehozni az INI bejegyz�st a k�vetkez� f�jlban: "%1".

; *** File copying errors
FileAbortRetryIgnore=Nyomd meg az Ism�t gombot az �jrapr�b�lkoz�shoz, a Kihagy gombot a f�jl kihagy�s�hoz (nem javasolt), vagy a Befejez gombot a telep�t�s megszak�t�s�hoz.
FileAbortRetryIgnore2=Nyomd meg az Ism�t gombot az �jrapr�b�lkoz�shoz, a Kihagy gombot a f�jl kihagy�s�hoz (nem javasolt), vagy a Befejez gombot a telep�t�s megszak�t�s�hoz.
SourceIsCorrupted=A forr�s f�jl hib�s
SourceDoesntExist=Egy forr�s f�jl (%1) hi�nyzik
ExistingFileReadOnly=A l�tez� f�jl csak olvashat�k�nt van megjel�lve.%n%nKattints az �jra gombra ennek megsz�ntet�s�hez, a Kihagy gombra a f�jl kihagy�s�hoz, vagy a Befejez gombot a telep�t�s megszak�t�s�hoz.
ErrorReadingExistingDest=Hiba t�rt�nt a k�vetkez�, m�r l�tez� f�jl �r�sakor:
FileExists=A f�jl m�r l�tezik.%n%nSzeretn�d, hogy a Telep�t� fel�l�rja azt?
ExistingFileNewer=A telep�tett f�jl �jabb ann�l, amire a Telep�t� lecser�ln�. Javasolt, hogy tartsd meg az eredeti f�jlt.%n%nSzeretn�d ezt tenni?
ErrorChangingAttr=Hiba t�rt�nt a k�vetkez� f�jl attrib�tumainak megv�ltoztat�sakor:
ErrorCreatingTemp=Hiba t�rt�nt a k�vetkez� f�jl l�trehoz�sakor:
ErrorReadingSource=Hiba t�rt�nt a k�vetkez� forr�sf�jl olvas�sakor:
ErrorCopying=Hiba t�rt�nt a k�vetkez� f�jl olvas�sakor:
ErrorReplacingExistingFile=Hiba t�rt�nt a k�vetkez�, m�r l�tez� f�jl �r�sakor:
ErrorRestartReplace=RestartReplace failed:
ErrorRenamingTemp=Hiba t�rt�nt a k�vetkez� f�jl �tnevez�sekor:
ErrorRegisterServer=Nem lehetett regisztr�ni a k�vetkez� DLL/OCX modult: %1
ErrorRegisterServerMissingExport=A DllRegisterServer export�l�s nem t�mogatott
ErrorRegisterTypeLib=Nem lehetett regisztr�lni a k�vetkez� t�pus k�nyvt�rat: %1

; *** Post-installation errors
ErrorOpeningReadme=Hiba t�rt�nt a README f�jl megnyit�sa k�zben.
ErrorRestartingComputer=A Telep�t� nem tudta �jraind�tani a sz�m�t�g�pet. K�rlek, tedd ezt meg a hagyom�nyos �ton.

; *** Uninstaller messages
UninstallNotFound="%1" nev� f�jl nem l�tezik. Nem lehetett elt�vol�tani.
UninstallOpenError=A "%1" nev� f�jlt nme lehetett megnyitni. Nem lehetett elt�vol�tani.
UninstallUnsupportedVer=Az elt�vol�t�si napl� f�jl (%1) form�tuma nem �rtelmezhet� a Telep�t�nek eme verzi�j�val. Nem lehetett elt�vol�tani.
UninstallUnknownEntry=Ismeretlen bejegyz�s (%1) tal�lhat� az elt�vol�t�si napl�ban
ConfirmUninstall=Biztosan szeretn�d elt�vol�tani a(z) %1 programot �s annak minden �sszetev�j�t?
OnlyAdminCanUninstall=Ezt programot csak adminisztr�tori jogokkal rendelkez� felhaszn�l� t�vol�thatja el.
UninstallStatusLabel=K�rlek v�rj, am�g a(z) %1 programot elt�vol�tom a sz�m�t�g�pr�l.
UninstalledAll=A(z) %1 sikeresen el lett t�vol�tva a sz�m�t�g�pr�l.
UninstalledMost=A(z) %1 elt�vol�t�sa sikeres volt.%n%nN�h�ny elemet nem lehetett elt�vol�tani. Ezeket a hagyom�nyos m�don lehet t�r�lni.
UninstalledAndNeedsRestart=A(z) %1 teljes m�rt�k� elt�vol�t�s�hoz a sz�m�t�g�pet �jra kell ind�tani.%n%nSzeretn�d ezt most megtenni?
UninstallDataCorrupted=A(z) "%1" nev� f�jl hib�s. Nem lehet elt�vol�tani.

; *** Uninstallation phase messages
ConfirmDeleteSharedFileTitle=Elt�vol�tsam a megosztott f�jlt?
ConfirmDeleteSharedFile2=A rendszer jelz�se szerint a k�vetkez� megosztott f�jl m�r egy�ltal�n nincs haszn�latban. Szeretn�d, ha az Elt�vol�t� elt�vol�tan� a sz�m�t�g�pr�l?%n%nAmennyiben elt�vol�t�sra ker�l, esetleg m�s programok nem fognak m�k�dni. Amennyiben bizonytalan vagy, v�laszd a Nem gombot. Semmi baj nem t�rt�nhet, ha a f�jl a sz�m�t�g�pen marad.
SharedFileNameLabel=F�jln�v:
SharedFileLocationLabel=Hely:
WizardUninstalling=Elt�vol�t�si �llapot
StatusUninstalling=A(z) %1 elt�vol�t�sa...

; The custom messages below aren't used by Setup itself, but if you make
; use of them in your scripts, you'll want to translate them.

[CustomMessages]
;Inno Setup Built-in Custom Messages
NameAndVersion=%1 - Verzi�sz�m: %2
AdditionalIcons=Tov�bbi ikonok:
OptionalFeatures=Optional Features:
CreateDesktopIcon=Ikon l�trehoz�sa az &Asztalon
CreateQuickLaunchIcon=Ikon l�trehoz�sa a &Gyorsind�t�s pulton
ProgramOnTheWeb=%1 a weben
UninstallProgram=A(z) %1 elt�vol�t�sa
LaunchProgram=A(z)%1 &ind�t�sa
AssocFileExtension=A(z) %1 &t�rs�t�sa a %2 kiterjeszt�s� f�jlokkal
AssocingFileExtension=A(z) %1 &t�rs�t�sa a %2 kiterjeszt�s� f�jlokkal...

;Things we can also localize
CompanyName=Thingamahoochie Software

;Types
TypicalInstallation=Szok�sos Telep�t�s
FullInstallation=TeljesTelep�t�s
CompactInstallation=Kompakt Telep�t�s
CustomInstallation=Egyedi Telep�t�s

;Components
AppCoreFiles=A WinMerge k�reg f�jljai
ApplicationRuntimes=Az alkalmaz�s fut�sidej� f�jljai
UsersGuide=Felhaszn�l�i k�zik�nyv
Filters=Sz�r�k
Plugins=B�v�tm�nyek (L�sd a Plugins.txt f�jlt)

;Localization Components
Languages=Nyelvek
BulgarianLanguage=Bolg�r men�k �s p�rbesz�dablakok
CatalanLanguage=Katal�n men�k �s p�rbesz�dablakok
ChineseSimplifiedLanguage=K�nai (Egyszer�s�tett) men�k �s p�rbesz�dablakok
ChineseTraditionalLanguage=Kynai (Hagyom�nyos) men�k �s p�rbesz�dablakok
CzechLanguage=Cseh men�k �s p�rbesz�dablakok
DanishLanguage=D�n men�k �s p�rbesz�dablakok
DutchLanguage=Holland men�k �s p�rbesz�dablakok
FrenchLanguage=Francia men�k �s p�rbesz�dablakok
GermanLanguage=N�met men�k �s p�rbesz�dablakok
HungarianLanguage=Magyar men�k �s p�rbesz�dablakok
ItalianLanguage=Olasz men�k �s p�rbesz�dablakok
JapaneseLanguage=Jap�n men�k �s p�rbesz�dablakok
KoreanLanguage=Koreai men�k �s p�rbesz�dablakok
NorwegianLanguage=Norv�g men�k �s p�rbesz�dablakok
PolishLanguage=Lengyel men�k �s p�rbesz�dablakok
PortugueseLanguage=Portug�l (Braz�liai) men�k �s p�rbesz�dablakok
RussianLanguage=Orosz men�k �s p�rbesz�dablakok
SlovakLanguage=Szlov�k men�k �s p�rbesz�dablakok
SpanishLanguage=Spanyol men�k �s p�rbesz�dablakok
SwedishLanguage=Sv�d men�k �s p�rbesz�dablakok
TurkishLanguage=t�r�k men�k �s p�rbesz�dablakok

;Tasks
ExplorerContextMenu=Integr�l�s az Int�z� &context-men�j�hez
IntegrateTortoiseCVS=Integr�l�s a &TortoiseCVS programmal
IntegrateDelphi4=Borland� Delphi &4 bin�ris f�jl t�mogat�s
UpdatingCommonControls=Updating the System's Common Controls

;Icon Labels
ReadMe=Olvass el
UsersGuide=Felhaszn�l�i k�zik�nyv
UpdatingCommonControls=Updating the System's Common Controls
ViewStartMenuFolder=A WinMerge Start Men� mapp�j�nak &megtekint�se

;Code Dialogs
DeletePreviousStartMenu=A Telep�t�s �szlelte, hogy megv�ltoztattad a Start Men� hely�t err�l: "%s" erre: "%s". Szeretn�d elt�vol�tani a kor�bbi Start Men� mapp�t?
