; *** Inno Setup version 5.1.11+ Croatian messages ***

; To download user-contributed translations of this file, go to:
;   http://www.jrsoftware.org/is3rdparty.php

; Note: When translating this text, do not add periods (.) to the end of
; messages that didn't have them already, because on those messages Inno
; Setup adds the periods automatically (appending a period would result in
; two periods being displayed).
;
; Maintained by HasanOsmanagi�(hasan.osmanagic@public.carnet.hr)
; Based on translation v. 5.1.1 by: Krunoslav Kanjuh (krunoslav.kanjuh@zg.t-com.hr)

[LangOptions]
; The following three entries are very important. Be sure to read and 
; understand the '[LangOptions] section' topic in the help file.
LanguageName=Hrvatski
LanguageID=$041a
LanguageCodePage=1250

[Messages]

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
SetupAppTitle=Instalacija
SetupWindowTitle=Instalacija - %1
UninstallAppTitle=Deinstalacija
UninstallAppFullTitle=Deinstalacija %1

; *** Misc. common
InformationTitle=Informacija
ConfirmTitle=Potvrda
ErrorTitle=Gre�ka

; *** SetupLdr messages
SetupLdrStartupMessage=Po�ela je instalacija programa %1. Nastaviti?
LdrCannotCreateTemp=Ne mogu stvoriti privremenu datoteku. Instalacija je prekinuta
LdrCannotExecTemp=Ne mogu u�itati datoteku u privremenoj mapi. Instalacija je prekinuta

; *** Startup error messages
LastErrorMessage=%1.%n%nGre�ka %2: %3
SetupFileMissing=Nema datoteke %1 u instalacijskoj mapi. Molimo vas rije�ite problem ili nabavite novu kopiju programa
SetupFileCorrupt=Instalacijske datoteke sadr�e gre�ku. Nabavite novu kopiju programa
SetupFileCorruptOrWrongVer=Instalacijske datoteke sadr�e gre�ku, ili nisu kompatibilne s ovom verzijom instalera. Molimo vas rije�ite problem ili nabavite novu kopiju programa
NotOnThisPlatform=Ovaj program ne radi na %1
OnlyOnThisPlatform=Ovaj program se mora pokrenuti na %1
OnlyOnTheseArchitectures=Ovaj program mo�e biti instaliran na verzijama Windowsa predvi�enim za sljede�u procesorsku arhitekturu:%n%n%1
MissingWOW64APIs=Verzija Windowsa ne udovoljava zahtjevima za 64-bitnu instalaciju. Poku�ajte rije�iti problem instalacijom: Service Pack %1
WinVersionTooLowError=Ovaj program zahtjeva %1 verziju %2 ili noviju
WinVersionTooHighError=Ovaj program ne mo�e biti instaliran na %1 verziji %2 ili starijoj
AdminPrivilegesRequired=Za instaliranje morate imati administratorske ovlasti
PowerUserPrivilegesRequired=Za instaliranje morate imati administratorske ovlasti ili biti ovla�teni korisnik
SetupAppRunningError=Instaler nalazi neuga�en %1.%n%nMolimo zatvorite program i sve instance i potom pritisnite tipku 'Nastavi>' za nastavak ili 'Odustajem' za prekid
UninstallAppRunningError=Deinstaler nalazi neuga�en %1.%n%nMolimo zatvorite program i sve njegove instance i potom pritisnite tipku 'Nastavi>' za nastavak ili 'Odustajem' za prekid

; *** Misc. errors
ErrorCreatingDir=Ne mogu stvoriti mapu "%1"
ErrorTooManyFilesInDir=Ne mo�e se stvoriti nova datoteka u mapi "%1" jer ve� sadr�i previ�e datoteka

; *** Setup common messages
ExitSetupTitle=Prekid instalacije
ExitSetupMessage=Instalacija nije zavr�ena. Ako sad prekinete, program ne�e biti instaliran.%n%nInstalaciju uvijek mo�ete pokrenuti kasnije.%n%nPrekid instalacije?
AboutSetupMenuItem=&O Instaleru...
AboutSetupTitle=O Instaleru
AboutSetupMessage=%1 verzija %2%n%3%n%n%1 Wev strana:%n%4
AboutSetupNote=
TranslatorNote=

; *** Buttons
ButtonBack=< Na&zad
ButtonNext=&Nastavi >
ButtonInstall=&Instaliraj
ButtonOK=U redu
ButtonCancel=Odustajem
ButtonYes=&Da
ButtonYesToAll=Da za &sve
ButtonNo=&Ne
ButtonNoToAll=N&e za sve
ButtonFinish=&Izlaz
ButtonBrowse=&Odabir...
ButtonWizardBrowse=Oda&bir...
ButtonNewFolder=Stvori novu &mapu

; *** "Select Language" dialog messages
SelectLanguageTitle=Odabir jezika instalacije
SelectLanguageLabel=Odberite jezik koji �elite koristiti pri instalaciji

; *** Common wizard text
ClickNext=Pritisnite tipku 'Nastavi >' za nastavak ili 'Odustajem' za prekid instalacije
BeveledLabel=
BrowseDialogTitle=Odabir mape
BrowseDialogLabel=Odaberite mapu iz popisa koja slijedi te pritisnite tipku 'U redu'
NewFolderName=Nova mapa

; *** "Welcome" wizard page
WelcomeLabel1=Dobro do�li u instalaciju programa [name]
WelcomeLabel2=Instaler �e instalirati program [name/ver] na va�e ra�unalo.%n%nPreporu�amo da zatvorite sve programe prije nastavka

; *** "Password" wizard page
WizardPassword=Lozinka
PasswordLabel1=Instalacija je za�ti�ena lozinkom.
PasswordLabel3=Upi�ite lozinku. Lozinka razlikuje mala i velika slova
PasswordEditLabel=&Lozinka:
IncorrectPassword=Upisana je pogre�na lozinka. Poku�ajte ponovo

; *** "License Agreement" wizard page
WizardLicense=Ugovor o kori�tenju
LicenseLabel=Molimo vas, prije nastavka, pa�ljivo pro�itajte slijede�e:
LicenseLabel3=Molimo vas, pa�ljivo pro�itajte Ugovor o kori�tenju. Za nastavak instalacije morate prihvatiti Ugovor o kori�tenju.
LicenseAccepted=&Prihva�am Ugovor o kori�tenju
LicenseNotAccepted=&Ne prihva�am Ugovor o kori�tenju

; *** "Information" wizard pages
WizardInfoBefore=Napomena
InfoBeforeLabel=Molimo vas, prije nastavka, pro�itajte sljede�e:
InfoBeforeClickLabel=Kada budete spremni nastaviti instalaciju, pritisnite tipku 'Nastavi >'
WizardInfoAfter=Napomena
InfoAfterLabel=Molimo vas, prije nastavka, pro�itajte sljede�e:
InfoAfterClickLabel=Kada budete spremni nastaviti instalaciju pritisnite tipku 'Nastavi >'

; *** "User Information" wizard page
WizardUserInfo=Korisnik
UserInfoDesc=Upi�ite va�e podatke
UserInfoName=&Ime korisnika:
UserInfoOrg=&Organizacija:
UserInfoSerial=&Serijski broj:
UserInfoNameRequired=Morate upisati ime

; *** "Select Destination Directory" wizard page
WizardSelectDir=Odabir odredi�ne mape
SelectDirDesc=Mapa instalacije programa.
SelectDirLabel3=[name] �e biti instaliran u mapu
SelectDirBrowseLabel=Za nastavak pritisnite tipku 'Nastavi >'. Za odabir druge mape, pritisnite tipku 'Odabir'
DiskSpaceMBLabel=Ovaj program zahtjeva minimalno [mb] MB slobodnog prostora na disku.
ToUNCPathname=Instaler ne mo�e instalirati na UNC datoteku. Ako poku�avate instalirati u mre�u, mre�ni disk mora biti mapiran
InvalidPath=Morate unijeti punu stazu zajedno sa slovnom oznakom diska; npr:%nC:\APP
InvalidDrive=Disk koji ste odabrali ne postoji. Odaberite neki drugi
DiskSpaceWarningTitle=Nedovoljno prostora na disku
DiskSpaceWarning=Instalacija zahtjeva bar %1 KB slobodnog prostora, a odabrani disk ima %2 KB na raspolaganju.%n%nNastaviti?
DirNameTooLong=Predugi naziv mape ili staze
InvalidDirName=Krivi naziv mape.
BadDirName32=Naziv mape ne smije, nakon to�ke, sadr�avati niti jedan od sljede�ih znakova:%n%n%1
DirExistsTitle=Mapa ve� postoji
DirExists=mapa:%n%n%1%n%nve� postoji. Instalirati u nju?
DirDoesntExistTitle=Mapa ne postoji
DirDoesntExist=mapa:%n%n%1%n%nne postoji. Stvoriti?

; *** "Select Components" wizard page
WizardSelectComponents=Odabir komponenata
SelectComponentsDesc=Koje komponente �elite instalirati?
SelectComponentsLabel2=Odaberite komponente koje �elite instalirati ili uklonite kva�icu uz komponente koje ne �elite
FullInstallation=Puna instalacija

; if possible don't translate 'Compact' as 'Minimal' (I mean 'Minimal' in your language)
CompactInstallation=Najmanja instalacija
CustomInstallation=Instalacija prema �elji
NoUninstallWarningTitle=Postoje�e komponente
NoUninstallWarning=Na va�em ra�unalu ve� postoje komponente:%n%n%1%n%nNeodabir tih komponenata ne dovodi do njihove deinstalacije.%n%nNastaviti?
ComponentSize1=%1 KB
ComponentSize2=%1 MB
ComponentsDiskSpaceMBLabel=Va� izbor zahtijeva najmanje [mb] MB prostora na disku

; *** "Select Additional Tasks" wizard page
WizardSelectTasks=Odaberite radnje
SelectTasksDesc=Koje radnje �elite da se izvr�e?
SelectTasksLabel2=Odaberite radnje koje �e se izvr�iti tokom instalacije programa [name]

; *** "Select Start Menu Folder" wizard page
WizardSelectProgramGroup=Odaberite programsku grupu
SelectStartMenuFolderDesc=Lokacija pre�ice programa
SelectStartMenuFolderLabel3=Instaler �e kreirati pre�icu programa u Po�etnom izborniku
SelectStartMenuFolderBrowseLabel=Za nastavak pritisnite tipku 'Nastavi >'. Ako �elite odabrati drugu mapu pritisnite tipku 'Odabir'
MustEnterGroupName=Morate unijeti naziv programske grupe
GroupNameTooLong=Predugi naziv mape ili staze
InvalidGroupName=Naziv mape je pogre�an
BadGroupName=Naziv programske grupe ne smije sadr�avati sljede�e znakove:%n%n%1
NoProgramGroupCheck2=&Ne stvaraj %1 programsku grupu

; *** "Ready to Install" wizard page
WizardReady=Spreman za instalaciju
ReadyLabel1=Instaler je spreman instalirati program [name] na va�e ra�unalo
ReadyLabel2a=Pritisnite tipku 'Instaliraj' za instalaciju programa ili 'Nazad' ako �elite provjeriti ili promjeniti postavke
ReadyLabel2b=Pritisnite tipku 'Instaliraj' za instalaciju programa
ReadyMemoUserInfo=Podaci o korisniku:
ReadyMemoDir=Odredi�ni mapa:
ReadyMemoType=Tip instalacije:
ReadyMemoComponents=Odabrane komponente:
ReadyMemoGroup=Programska grupa:
ReadyMemoTasks=Dodatni zadaci:

; *** "Preparing to Install" wizard page
WizardPreparing=Pripremam instalaciju
PreparingDesc=Instaler se priprema da program [name] instalira na va�e ra�unalo
PreviousInstallNotCompleted=Instalacija/deinstalacija prethodnog programa nije zavr�ena. Morate restartati ra�unalo kako bi zavr�ili instalaciju.%n%nNakon restartanja ra�unala, ponovno pokrenite instalaciju [name]
CannotContinue=Instaler ne mo�e nastaviti. Molimo pritisnite tipku 'Odustajem' za izlaz

; *** "Installing" wizard page
WizardInstalling=Instaliram
InstallingLabel=Pri�ekajte zavr�etak instalacije programa [name] na va�e ra�unalo

; *** "Setup Completed" wizard page
FinishedHeadingLabel=Program [name] je instaliran
FinishedLabelNoIcons=Instalacija programa [name] je zavr�ena
FinishedLabel=Instalacija programa [name] je zavr�ena. Program mo�ete pokrenuti preko instaliranih pre�ica
ClickFinish=Pritisnite tipku 'Izlaz' za izlaz iz Instalera
FinishedRestartLabel=Za zavr�etak instalacije programa [name], potrebno je ponovno pokrenuti ra�unalo. Pokrenuti odmah?
FinishedRestartMessage=Zavr�itak instalacije programa [name], zahtijeva ponovno pokretanje ra�unala.%n%nPokrenuti odmah?
ShowReadmeCheck=Da, �elim pro�itati datoteku Pro�itaj
YesRadio=&Da, �elim odmah ponovno pokrenuti ra�unalo
NoRadio=&Ne, kasnije �u ga pokrenuti

; used for example as 'Run MyProg.exe'
RunEntryExec=Pokreni %1
; used for example as 'View Readme.txt'
RunEntryShellExec=Pregledati %1

; *** "Setup Needs the Next Disk" stuff
ChangeDiskTitle=Potreban je sljede�i disk:
SelectDiskLabel2=Umetnite disketu %1 i pritisnite tipku 'U redu'.%n%nAko se datoteke s ove diskete nalaze na nekom drugom mediju %2 , ispravno upi�ite stazu do njega ili pritisnite tipku 'Odaberi'
PathLabel=&Staza:
FileNotInDir2=Datoteka "%1" ne postoji u "%2". Molimo vas umetnite odgovaraju�i disk ili odaberete drugi %3.
SelectDirectoryLabel=Molimo vas odaberite lokaciju sljede�eg diska

; *** Installation phase messages
SetupAborted=Instalacija nije zavr�ena.%n%nMolimo vas, rije�ite problem i opet pokrenite instalaciju
EntryAbortRetryIgnore=Pritisnite tipku 'Retry' za novi poku�aj, 'Ignore' za nastavak, ili 'Abort' za prekid instalacije

; *** Installation status messages
StatusCreateDirs=Stvaram mape...
StatusExtractFiles=Izdvajam datoteke...
StatusCreateIcons=Stvaram ikone...
StatusCreateIniEntries=Stvaram INI datoteke...
StatusCreateRegistryEntries=Stvaram podatke za registar...
StatusRegisterFiles=Registriram datoteke...
StatusSavingUninstall=Pohranjujem deinstalacijske podatke...
StatusRunProgram=Zavr�avam instalaciju...
StatusRollback=Poni�tavam promjene...

; *** Misc. errors
ErrorInternal2=Interna gre�ka: %1
ErrorFunctionFailedNoCode=%1 nije uspjelo
ErrorFunctionFailed=%1 nije uspjelo; code %2
ErrorFunctionFailedWithMessage=%1 nije uspjelo; code %2.%n%3
ErrorExecutingProgram=Ne mogu u�itati datoteku:%n%1

; *** Registry errors
ErrorRegOpenKey=Gre�ka pri otvaranju klju�a registra:%n%1\%2
ErrorRegCreateKey=Gre�ka pri stvaranju klju�a registra:%n%1\%2
ErrorRegWriteKey=Gre�ke pri pohrani u klju� registra:%n%1\%2

; *** INI errors
ErrorIniEntry=Gre�ka pri stvaranju INI podataka u datoteci "%1"

; *** File copying errors
FileAbortRetryIgnore=Pritisnite tipku 'Retry' za novi poku�aj, 'Ignore' za zanemarivanje ove datoteke (ne preporu�a se) ili 'Abort' za prekid instalacije
FileAbortRetryIgnore2=Pritisnite tipku 'Retry' za novi poku�aj, 'Ignore' za nastavak zanemaruju�i poruku (ne preporu�a se) ili 'Abort' za prekid instalacije
SourceIsCorrupted=Izvori�na datoteka je o�te�ena
SourceDoesntExist=Izvori�na datoteka "%1" ne postoji
ExistingFileReadOnly=Datoteka je ozna�ena "samo-za-�itanje".%n%nPritisnite tipku 'Retry' da uklonite oznaku "samo-za-�itanje" i poku�ajte ponovno, 'Ignore' za zanemarivanje ove datoteke, ili 'Abort' za prekid instalacije
ErrorReadingExistingDest=Pojavila se gre�ka pri poku�aju �itanja postoje�e datoteke:
FileExists=Datoteka ve� postoji.%n%nNatpisati?
ExistingFileNewer=Postoje�a datoteka je novija od instalacijske. Preporu�a se zadr�ati postoje�u datoteku.%n%nZadr�ati postoje�u datoteku?
ErrorChangingAttr=Pojavila se gre�ka pri poku�aju promjene atributa postoje�e datoteke
ErrorCreatingTemp=Pojavila se gre�ka pri poku�aju stvaranja datoteke u odredi�noj mapi
ErrorReadingSource=Pojavila se gre�ka pri poku�aju �itanja izvori�ne datoteke
ErrorCopying=Pojavila se gre�ka pri poku�aju kopiranja datoteke
ErrorReplacingExistingFile=Pojavila se gre�ka pri poku�aju zamjene datoteke
ErrorRestartReplace=Zamjena nakon ponovnog pokretanja nije uspjela
ErrorRenamingTemp=Pojavila se gre�ka pri poku�aju preimenovanja datoteke u odredi�noj mapi
ErrorRegisterServer=Ne mogu registrirati DLL/OCX: %1
ErrorRegSvr32Failed=Gre�ka u RegSvr32: gre�ka %1
ErrorRegisterTypeLib=Ne mogu registrirati datoteku library: %1

; *** Post-installation errors
ErrorOpeningReadme=Pojavila se gre�ka pri poku�aju otvaranja datoteke Pro�itaj
ErrorRestartingComputer=Instaler ne mo�e ponovno pokrenuti ra�unalo. Molimo vas, u�inite to sami

; *** Uninstaller messages
UninstallNotFound=Datoteka "%1" ne postoji. Deinstalacija prekinuta
UninstallOpenError=Datoteku "%1" ne mogu otvoriti. Deinstalacija nije mogu�a
UninstallUnsupportedVer=Deinstalacijska datoteka "%1" je u obliku koju ove verzija deinstalera ne prihva�a. Nije mogu�a deinstalacija
UninstallUnknownEntry=U deinstalacijskoj datoteci je prona�en nepoznat zapis (%1)
ConfirmUninstall=Ukloniti %1 i sve pripadne komponente?
UninstallOnlyOnWin64=Ova instalacija mo�e biti uklonjena samo na 64-bitnim Windows-ima
OnlyAdminCanUninstall=Ova instalacija mo�e biti uklonjena samo s administratorskim ovlastima
UninstallStatusLabel=Pri�ekajte dok %1 ne bude uklonjen s va�eg ra�unala
UninstalledAll=Program %1 je uspje�no uklonjen sa va�eg ra�unala
UninstalledMost=Deinstalacija programa %1 je zavr�ena.%n%nNeke elemente nije bilo mogu�e ukloniti. Molimo vas da to u�inite sami
UninstalledAndNeedsRestart=Za zavr�etak deinstalacije %1, va�e ra�unalo morate ponovno pokrenuti%n%nPokrenuti odmah?
UninstallDataCorrupted="%1" datoteka je o�te�ena. Deinstalacija nije mogu�a

; *** Uninstallation phase messages
ConfirmDeleteSharedFileTitle=Brisanje dijeljene datoteke?
ConfirmDeleteSharedFile2=Sistem ukazuje da sljede�e dijeljene datoteke ne koristi niti jedan program. �elite li da Deintaler ukloni te dijeljene datoteke?%n%nAko neki programi ipak koriste te datoteke, a one su obrisane, ti programi ne�e ispravno raditi. Ako niste sigurni, odaberite Ne. Ostavljanje datoteka ne�e uzrokovati �tetu va�em sistemu
SharedFileNameLabel=Datoteka:
SharedFileLocationLabel=Staza:
WizardUninstalling=Deinstalacija
StatusUninstalling=Deinstaliram %1...

; The custom messages below aren't used by Setup itself, but if you make
; use of them in your scripts, you'll want to translate them.

[CustomMessages]
;Inno Setup Built-in Custom Messages
NameAndVersion=%1 verzija %2
AdditionalIcons=Dodatne ikone:
OptionalFeatures=Po volji:
CreateDesktopIcon=Stvori ikonu na &desktopu
CreateQuickLaunchIcon=Stvori ikonu u brzom izborniku
ProgramOnTheWeb=%1 je na Web-u
UninstallProgram=Deinstaliraj %1
LaunchProgram=Pokreni %1
AssocFileExtension=Pridru&�i %1 s %2 ekstenzijom datoteke
AssocingFileExtension=Pridru�ujem %1 s %2 ekstenzijom datoteke

;Things we can also localize
CompanyName=Thingamahoochie Software

;Types
TypicalInstallation=Uobi�ajena instalacija
FullInstallation=Puna instalacija
CompactInstallation=Najmanja instalacija
CustomInstallation=Izborna instalacija

;Components
AppCoreFiles=WinMerge temeljne datoteke
ApplicationRuntimes=Datoteke izvo�enja (Runtimes)
UsersGuide=Vodi� za korisnika
Filters=Filtri
Plugins=Dodaci

;Localization Components
Languages=Jezici
BulgarianLanguage=Bulgarian menus and dialogs
CatalanLanguage=Catalan menus and dialogs
ChineseSimplifiedLanguage=Chinese (Simplified) menus and dialogs
ChineseTraditionalLanguage=Chinese (Traditional) menus and dialogs
CroatianLanguage=Hrvatski izbornik i dijalozi
CzechLanguage=Czech menus and dialogs
DanishLanguage=Danish menus and dialogs
DutchLanguage=Dutch menus and dialogs
FrenchLanguage=French menus and dialogs
GermanLanguage=German menus and dialogs
HungarianLanguage=Hungarian menus and dialogs
ItalianLanguage=Italian menus and dialogs
JapaneseLanguage=Japanese menus and dialogs
KoreanLanguage=Korean menus and dialogs
NorwegianLanguage=Norwegian menus and dialogs
PolishLanguage=Polish menus and dialogs
PortugueseBrazilLanguage=Portuguese (Brazilian) menus and dialogs
PortugueseLanguage=Portuguese menus and dialogs
RussianLanguage=Russian menus and dialogs
SlovakLanguage=Slovak menus and dialogs
SpanishLanguage=Spanish menus and dialogs
SwedishLanguage=Swedish menus and dialogs
TurkishLanguage=Turkish menus and dialogs

;Tasks
ExplorerContextMenu=&Integracija u Explorer kontekst izbornik
IntegrateTortoiseCVS=Integracija s &TortoiseCVS
IntegrateTortoiseSVN=Integracija s T&ortoiseSVN
IntegrateClearCase=Integracija s Rational &ClearCase
IntegrateDelphi4=Rad s Borland� Delphi &4 Binarnim datotekama
UpdatingCommonControls=Obnova System's Common kontrola

;Icon Labels
ReadMe=Pro�itaj
UsersGuide=Vodi� za korisnika
UpdatingCommonControls=Obnova System's Common kontrola
ViewStartMenuFolder=&Pregled WinMerge po�etne mape
PluginsText=Dodaci

;Code Dialogs
DeletePreviousStartMenu=Instaler je otkrio promjenu lokacije Po�etnog izbornika od "%s" u "%s". Obrisati stari Po�etni izbornik?

; Project file description
ProjectFileDesc=WinMerge Usporedbe
