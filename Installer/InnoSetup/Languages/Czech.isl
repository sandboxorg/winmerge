; *** Inno Setup version 1.2.0 CZECH messages ***
;
;  TRANSLATION TO CZECH:
;  (c) Josef Planeta, planeta@iach.cz
;
;  Pokud bude nekdo provadet upravy/opravy tohoto prekladu, at mi prosim posle
;  emailem opravenou verzi.
;  20.9.1999: Dve drobne zmeny dle skacel@stereo.cz
;  22.9.1999: Upgrade prekladu na verzi 1.12beta10
;  27.9.1999: Upgrade prekladu na verzi 1.2.0 [a vyssi]
;  15.11.1999: Zmena v ConfirmUninstall dle skacel@stereo.cz
;  17.4.2000: Provedeny (temer vsechny) zmeny, ktere navrhl Vaclav Slavik <v.slavik@volny.cz>


[Messages]

; *** Application titles

SetupAppTitle=Instalace
SetupWindowTitle=Instalace - %1
UninstallAppTitle=Odinstalov�n�
UninstallAppFullTitle=%1 - Odinstalov�n�

; *** Icons
DefaultUninstallIconName=%1 - Odinstalov�n�

; *** Misc. common

InformationTitle=Informace
ConfirmTitle=Potvrzen�
ErrorTitle=Chyba
DirectoryOld=adres��
DirectoryNew=slo�ku
ProgramManagerOld=Program Manager
ProgramManagerNew=Start Menu

; *** SetupLdr messages

SetupLdrStartupMessage=Instalace programu %1. P�ejete si pokra�ovat?
LdrCannotCreateTemp=Nelze vytvo�it do�asn� soubor. Instalace se ukon��
LdrCannotExecTemp=Nelze spustit soubor v do�asn�m adres��i. Instalace se ukon��


; *** Startup error messages

LastErrorMessage=%1.%n%nChyba %2: %3
SetupFileMissing=Soubor %1 chyb� v instala�n�m adres��i. Pros�m, odstra�te probl�m nebo si opat�ete novou kopii programu.
SetupFileCorrupt=Instala�n� soubory jsou poru�eny. Pros�m, opat�ete si novou kopii programu.
SetupFileCorruptOrWrongVer=Instala�n� soubory jsou poru�eny nebo se neslu�uj� s touto verz� Instalace. Pros�m, odstra�te probl�m nebo si opat�ete novou kopii programu.
NotOnThisPlatform=Tento program nelze spustit pod %1.
OnlyOnThisPlatform=Tento program mus� b�t spu�t�n pod %1.
WinVersionTooLowError=Tento program vy�aduje %1 verze %2 nebo vy���.
WinVersionTooHighError=Tento program nem��e b�t nainstalov�n na %1 verze %2 nebo vy���.
AdminPrivilegesRequired=K instalaci tohoto programu mus�te b�t p�ihl�en jako administr�tor.


; *** Misc. errors

ErrorCreatingDir=Instala�n� program nemohl vytvo�it adres�� "%1"
ErrorTooManyFilesInDir=Nelze vytvo�it soubor v adres��i "%1" proto�e obsahuje p��li� mnoho soubor�
ErrorThunk=Thunk selhal; code %1.


; *** Setup common messages

ExitSetupTitle=Ukon�en� instalace
ExitSetupMessage=Instalace nebyla dokon�ena. Pokud nyn� skon��te, program nebude nainstalov�n.%n%nInstala�n� program m��ete spustit jindy k dokon�en� instalace.%n%nUkon�it instalaci?
AboutSetupMenuItem=O progr&amu...
AboutSetupTitle=O instalaci
AboutSetupMessage=%1 verze %2%n%3%n%n%1 domovsk� str�nka:%n%4
AboutSetupNote=


; *** Buttons

ButtonBack=< &Zp�t
ButtonNext=&Dal�� >
ButtonInstall=&Nainstalovat
ButtonOK=OK
ButtonCancel=Storno
ButtonYes=&Ano
ButtonNo=&Ne
ButtonFinish=&Dokon�it
ButtonBrowse=&Proch�zet...


; *** Common wizard text

ClickNext=Dal�� pro pokra�ov�n�, Storno ukon�� instalaci.


; *** "Welcome" wizard page

WizardWelcome=V�tejte
WelcomeLabel=V�tejte v instala�n�m programu pro [name]. Program nainstaluje [name/ver] na v� po��ta�.%n%nNe� budete pokra�ovat, doporu�ujeme uzav��t v�echny spu�t�n� aplikace. P�edejdete t�m mo�n�m konflikt�m b�hem instalace.

; *** "Password" wizard page

WizardPassword=Heslo
PasswordLabel=Tato instalace je chr�n�na heslem. Pros�m zadejte heslo.%n%nHesla rozli�uj� velk� a mal� znaky.
PasswordEditLabel=&Heslo:
IncorrectPassword=Zadan� heslo nen� spr�vn�. Pros�m zadejte jej znovu.


; *** "License Agreement" wizard page

WizardLicense=Licen�n� ujedn�n�
LicenseLabel1=Pros�me p�e�t�te si pozorn� n�sleduj�c� licen�n� ujedn�n�. K zobrazen� cel�ho textu pou�ijte posuvnou li�tu nebo kl�vesu Page Down.
LicenseLabel2=Souhlas�te se v�emi ��stmi licen�n�ho ujedn�n�? Pokud zvol�te Ne, instalace se ukon��. Abyste mohli nainstalovat [name] mus�te souhlasit s t�mto ujedn�n�m.


; *** "Information" wizard pages

WizardInfoBefore=Informace
InfoBeforeLabel=P�ed t�m ne� budete pokra�ovat, �t�te pros�m nejprve n�sleduj�c� d�le�itou informaci.
InfoBeforeClickLabel=Stiskem Dal�� pokra�ujte v instalaci.
WizardInfoAfter=Informace
InfoAfterLabel=P�ed t�m ne� budete pokra�ovat, p�e�t�te si pros�m nejprve n�sleduj�c� d�le�itou informaci.
InfoAfterClickLabel=Stiskem Dal�� pokra�ujte v instalaci.

; *** "Select Destination Directory" wizard page

WizardSelectDir=Vyberte c�lov� adres��

; the %1 below is changed to either DirectoryOld or DirectoryNew
; depending on whether the user is running Windows 3.x, or 95 or NT 4.0

SelectFolderLabel=Vyberte %1 kam chcete nainstalovat [name] :
DiskSpaceMBLabel=Aplikace vy�aduje nejm�n� [mb] MB m�sta na disku.
ToUNCPathname=Nelze instalovat do cesty UNC. Pokud se pokou��te instalovat do s�t�, mus�te namapovat s�ov� disk.
InvalidPath=Mus�te zadat celou cestu s p�smenem disku, nap��klad:%nC:\APP
InvalidDrive=Vybran� disk neexistuje. Pros�m, vyberte jin�.
PathTooLong=��st zadan� cesty obsahuje p��li� mnoho znak�. Platn� n�zev adres��e nesm� p�es�hnout d�lku 8 znak�, ale m��e t� zahrnovat p��ponu t�� znak�.
DiskSpaceWarningTitle=Na disku nen� dost m�sta
DiskSpaceWarning=Instalace vy�aduje nejm�n� %1 KB voln�ho m�sta, ale na vybran�m disku je dostupn� pouze %2 KB.%n%nChcete p�esto pokra�ovat?
BadDirName32=N�zev adres��e nem��e obsahovat ��dn� z n�sleduj�c�ch znak�:%n%n%1
BadDirName16=N�zev adres��e nem��e obsahovat mezery nebo n�kter� z n�sleduj�c�ch znak�:%n%n%1
DirExistsTitle=Adres�� ji� existuje
DirExists=Adres�� %1 ji� existuje.%n%nChcete p�esto do tohoto adres��e instalovat?
DirDoesntExistTitle=Adres�� neexistuje
DirDoesntExist=Adres��:%n%n%1%n%nneexistuje. Chcete aby byl adres�� vytvo�en?


; *** "Select Program Group" wizard page

WizardSelectProgramGroup=Vyberte programovou skupinu

; the %1 below is changed to either ProgramManagerOld or ProgramManagerNew
; depending on whether the user is running Windows 3.x, or 95 or NT 4.0

IconsLabel=Instalace p�id� ikony program� do programov� skupiny %1.
NoIconsCheck=Nevytv��et ��dn� ikony
MustEnterGroupName=Mus�te zadat n�zev skupiny.
BadGroupName=N�zev skupiny nem��e obsahovat ��dn� z n�sleduj�c�ch znak�:%n%n%1


; *** "Ready to Install" wizard page

WizardReady=Instalace je p�ipravena
ReadyLabel1=Nyn� se [name] nainstaluje na v� po��ta�...
ReadyLabel2a=Pokra�ujte kliknut�m na Instalovat nebo klikn�te Zp�t, pokud chcete zm�nit nastaven�.
ReadyLabel2b=V instalaci pokra�ujte klikem na Instalovat.


; *** "Setup Completed" wizard page

WizardFinished=Instalace byla dokon�ena
FinishedLabelNoIcons=Instalace aplikace [name] na v� po��ta� byla dokon�ena.
FinishedLabel=Instalace aplikace [name] na v� po��ta� byla dokon�ena. Aplikace m��ete spustit pomoc� nainstalovan�ch ikon.
ClickFinish=Kliknut�m na Dokon�it dokon��te instalaci.
FinishedRestartLabel=K dokon�en� instalace [name] se mus� restartovat v� po��ta�. Chcete nyn� prov�st restart?
ShowReadmeCheck=Ano, chci vid�t soubor README
YesRadio=&Ano, restartovat po��ta� hned
NoRadio=&Ne, restartuji po��ta� pozd�ji


; *** "Setup Needs the Next Disk" stuff

ChangeDiskTitle=Instalace vy�aduje dal�� disketu
SelectDirectory=Vyberte adres��

; the %2 below is changed to either SDirectoryOld or SDirectoryNew
; depending on whether the user is running Windows 3.x, or 95 or NT 4.0

SelectDiskLabel=Pros�m, vlo�te Disk %1 a klikn�te OK.%n%nPokud mohou b�t soubory na tomto disku nalezeny v jin�m %2 ne� v n�e uveden�m, zadejte spr�vnou cestu nebo klikn�te na Nalistovat.
PathLabel=&Cesta:

; the %3 below is changed to either SDirectoryOld or SDirectoryNew
; depending on whether the user is running Windows 3.x, or 95 or NT 4.0

FileNotInDir=Soubor "%1" nelze naj�t v "%2". Pros�m vlo�te spr�vn� disk nebo vyberte jin� %3.
SelectDirectoryLabel=Pros�m, zadejte um�st�n� dal��ho disku.

; *** Installation phase messages

SetupAborted=Instalace nebyla dokon�ena.%n%nPros�m, odstra�te probl�m a spus�te instalaci znovu.
EntryAbortRetryIgnore=Klikn�te Znovu pro opakov�n�, Ignoruj pro pokra�ov�n� nebo Storno k ukon�en� instalace.


; *** Installation status messages

StatusCreateDirs=Vytv���m adres��e...
StatusExtractFiles=Rozbaluji soubory...
StatusCreateIcons=Vytv���m ikony program�...
StatusCreateIniEntries=Vytv���m z�znamy v INI...
StatusCreateRegistryEntries=Vytv���m z�znamy v registrech...
StatusRegisterFiles=Registruji soubory...
StatusSavingUninstall=Ukl�d�m informace k odinstalov�n�...


; *** Misc. errors

ErrorInternal=Vnit�n� chyba %1
ErrorFunctionFailedNoCode=%1 selhal
ErrorFunctionFailed=%1 selhal; code %2
ErrorFunctionFailedWithMessage=%1 selhal; code %2.%n%3
ErrorExecutingProgram=Nen� mo�n� spustit soubor:%n%1


; *** DDE errors

ErrorDDEExecute=DDE: DDE: Chyba b�hem prov�d�n� transakce (code: %1)
ErrorDDECommandFailed=DDE: P��kaz nebyl �sp�n�
ErrorDDERequest=DDE: Chyba b�hem po�adavku na transakci (code: %1)


; *** Registry errors

ErrorRegOpenKey=Chyba p�i otev�r�n� registr�:%n%1\%2
ErrorRegCreateKey=Chyba p�i vytv��en� registr�:%n%1\%2
ErrorRegWriteKey=Chyba p�i z�pisu do registr�:%n%1\%2


; *** INI errors

ErrorIniEntry=Chyba p�i vytv��en� INI z�znamu v souboru %1.


; *** File copying errors

FileAbortRetryIgnore=Klikn�te na Znovu pro opakov�n�, Ignoruj pro vynech�n� tohoto souboru (nedoporu�uje se) nebo Storno k ukon�en� instalace.
FileAbortRetryIgnore2=Klikn�te na Znovu pro opakov�n�, Ignoruj k pokra�ov�n� (nedoporu�uje se) nebo Storno k ukon�en� instalace.
SourceIsCorrupted=Zdrojov� soubor je poru�en
SourceDoesntExist=Zdrojov� soubor "%1" neexistuje
ExistingFileReadOnly=Existuj�c� soubor je ozna�en jako read-only.%n%nKlikn�te Znovu pro odstran�n� atributu read-only a nov�mu opakov�n�, Ignoruj pro vynech�n� tohoto souboru, nebo Storno k ukon�en� instalace.
ErrorReadingExistingDest=Do�lo k chyb� p�i pokusu p�e��st ji� existuj�c� soubor:
FileExists=Soubor ji� existuje.%n%nChcete aby jej instalace p�epsala?
ExistingFileNewer=P�vodn� soubor je nov�j�� ne� ten, kter� se bude instalovat. Doporu�uje se zachovat p�vodn� soubor.%n%nChcete zachovat p�vodn� soubor?
ErrorChangingAttr=Do�lo k chyb� p�i pokusu zm�nit atributy existuj�c�ho souboru:
ErrorCreatingTemp=Do�lo k chyb� p�i pokusu vytvo�it soubor v c�lov�m adres��i:
ErrorReadingSource=Do�lo k chyb� p�i pokusu p�e��st zdrojov� soubor:
ErrorCopying=Do�lo k chyb� p�i pokusu kop�rovat soubor:
ErrorReplacingExistingFile=Do�lo k chyb� p�i pokusu nahradit existuj�c� soubor:
ErrorRestartReplace=RestartReplace selhal:
ErrorRenamingTemp=Do�lo k chyb� p�i pokusu p�ejmenovat soubor v c�lov�m adres��i:
ErrorRegisterServer=Nelze zaregistrovat DLL/OCX: %1
ErrorRegisterServerMissingExport=DllRegisterServer export nebyl nalezen
ErrorRegisterTypeLib=Nelze zaregistrovat typ knihovny: %1


; *** Post-installation errors

ErrorOpeningReadme=Vyskytla se chyba p�i pokusu otev��t README soubor.
ErrorRestartingComputer=Setup nemohl restartovat po��ta�. Pros�m, restartujte manu�ln�.


; *** Uninstaller messages

UninstallNotFound=Soubor "%1" neexistuje. Nelze odinstalovat.
ConfirmUninstall=Jste si jist(a), �e chcete odstranit %1 v�etn� v�ech sou��st�?
OnlyAdminCanUninstall=Tato instalace m��e b�t odinstalov�na pouze u�ivatelem - administr�torem.
UninstalledAll=Program %1 byl �sp�n� odstran�n z va�eho po��ta�e.
UninstalledMost=Odinstalov�n� %1 je dokon�eno.%n%nN�kter� ��sti nemohly b�t odstran�ny. Mohou b�t odstran�ny manu�ln�.
UninstallDataCorrupted=Soubor "%1" je poru�en. Nelze odinstalovat


; *** Uninstallation phase messages

ConfirmDeleteSharedFileTitle=Odstranit sd�len� soubor?
ConfirmDeleteSharedFile=Syst�m ukazuje, �e n�sleduj�c� sd�len� soubor ji� nen� d�le pou��v�n ��dn�mi programy. Chcete odinstalovat tento sd�len� soubor?%n%n%1%n%nPokud n�kter� programy tento soubor pou��vaj�, po jeho odstran�n� nemus� pracovat spr�vn�. Pokud si nejste jist, vyberte Ne. Ponech�n� souboru v syst�mu nevyvol� ��dnou �kodu.


; The custom messages below aren't used by Setup itself, but if you make
; use of them in your scripts, you'll want to translate them.

[CustomMessages]
;Inno Setup Built-in Custom Messages
AdditionalIcons=Additional Icons:
CreateDesktopIcon=Create a &Desktop Icon
CreateQuickLaunchIcon=Create a &Quick Launch Icon
ProgramOnTheWeb=%1 on the Web
UninstallProgram=Uninstall %1
LaunchProgram=&Launch %1
AssocFileExtension=&Associate %1 with the %2 file extension
AssocingFileExtension=Associating %1 with the %2 file extension...


;Things we can also localize
CompanyName=Thingamahoochie Software

;Types
TypicalInstallation=Typical Installation
FullInstallation=Full Installation
CompactInstallation=Compact Installation
CustomInstallation=Custom Installation

;Components
AppCoreFile=%1 Core Files
ApplicationRuntimes=Application Runtimes
UsersGuide=User's Guide
Filters=Filters
Plugins=Plugins (enhance core behavior)

;Localization Components
BulgarianLanguage=Bulgarian menus and dialogs
CatalanLanguage=Catalan menus and dialogs
ChineseSimplifiedLanguage=Chinese(Simplified) menus and dialogs
ChineseTraditionalLanguage=Chinese (Traditional) menus and dialogs
CzechLanguage=Czech menus and dialogs
DanishLanguage=Danish menus and dialogs
DutchLanguage=Dutch menus and dialogs
FrenchLanguage=French menus and dialogs
GermanLanguage=German menus and dialogs
ItalianLanguage=Italian menus and dialogs
KoreanLanguage=Korean menus and dialogs
NorwegianLanguage=Norwegian menus and dialogs
PolishLanguage=Polish menus and dialogs
PortugueseLanguage=Portuguese (Brazillian) menus and dialogs
RussianLanguage=Russian menus and dialogs
SlovakLanguage=Slovak menus and dialogs
SpanishLanguage=Spanish menus and dialogs
SwedishLanguage=Swedish menus and dialogs

;Tasks
ExplorerContextMenu=&Enable Explorer context menu integration
IntegrateTortoiseCVS=Integrate with &TortoiseCVS
IntegrateDelphi4=Borland� Delphi &4 Binary File Support
UpdatingCommonControls=Updating the System's Common Controls

;Icon Labels
ReadMe=Read Me
UsersGuide=User's Guide
UpdatingCommonControls=Updating the System's Common Controls
ViewStartMenuFolder=&View the %1 Start Menu Folder

;Things we can also localize
CompanyName=Thingamahoochie Software

;Types
TypicalInstallation=Typical Installation
FullInstallation=Full Installation
CompactInstallation=Compact Installation
CustomInstallation=Custom Installation

;Components
AppCoreFile=%1 Core Files
ApplicationRuntimes=Application Runtimes
UsersGuide=User's Guide
Filters=Filters
Plugins=Plugins (enhance core behavior)

;Localization Components
BulgarianLanguage=Bulgarian menus and dialogs
CatalanLanguage=Catalan menus and dialogs
ChineseSimplifiedLanguage=Chinese(Simplified) menus and dialogs
ChineseTraditionalLanguage=Chinese (Traditional) menus and dialogs
CzechLanguage=Czech menus and dialogs
DanishLanguage=Danish menus and dialogs
DutchLanguage=Dutch menus and dialogs
FrenchLanguage=French menus and dialogs
GermanLanguage=German menus and dialogs
ItalianLanguage=Italian menus and dialogs
KoreanLanguage=Korean menus and dialogs
NorwegianLanguage=Norwegian menus and dialogs
PolishLanguage=Polish menus and dialogs
PortugueseLanguage=Portuguese (Brazillian) menus and dialogs
RussianLanguage=Russian menus and dialogs
SlovakLanguage=Slovak menus and dialogs
SpanishLanguage=Spanish menus and dialogs
SwedishLanguage=Swedish menus and dialogs

;Tasks
ExplorerContextMenu=&Enable Explorer context menu integration
IntegrateTortoiseCVS=Integrate with &TortoiseCVS
IntegrateDelphi4=Borland� Delphi &4 Binary File Support
UpdatingCommonControls=Updating the System's Common Controls

;Icon Labels
ReadMe=Read Me
UsersGuide=User's Guide
UpdatingCommonControls=Updating the System's Common Controls
ViewStartMenuFolder=&View the %1 Start Menu Folder