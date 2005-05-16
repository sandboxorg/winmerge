; *** Inno Setup version 4.2.2+ Polish messages ***
; Krzysztof Cynarski <krzysztof at cynarski.net>
;
; To download user-contributed translations of this file, go to:
;   http://www.jrsoftware.org/is3rdparty.php
;
; Note: When translating this text, do not add periods (.) to the end of
; messages that didn't have them already, because on those messages Inno
; Setup adds the periods automatically (appending a period would result in
; two periods being displayed).
;
; $jrsoftware: issrc/Files/Languages/Polish.isl,v 1.7 2004/04/07 20:19:51 jr Exp $

[LangOptions]
LanguageName=Polski
LanguageID=$0415
LanguageCodePage=1250

[Messages]

; *** Application titles
SetupAppTitle=Instalator
SetupWindowTitle=Instalacja - %1
UninstallAppTitle=Deinstalacja
UninstallAppFullTitle=Odinstaluj %1

; *** Misc. common
InformationTitle=Informacja
ConfirmTitle=Potwierdz
ErrorTitle=Blad

; *** SetupLdr messages
SetupLdrStartupMessage=Ten program zainstaluje aplikacje %1. Czy chcesz kontynuowac?
LdrCannotCreateTemp=Nie mozna utworzyc pliku tymczasowego. Instalacja przerwana
LdrCannotExecTemp=Nie mozna uruchomic pliku w tymczasowym folderze. Instalacja przerwana

; *** Startup error messages
LastErrorMessage=%1.%n%nBlad %2: %3
SetupFileMissing=W folderze Instalatora brakuje pliku %1. Prosze rozwiazac ten problem lub uzyskac nowa kopie Instalatora tego programu od producenta.
SetupFileCorrupt=Pliki skladowe Instalatora sa uszkodzone. Prosze uzyskac nowa kopie Instalatora od producenta.
SetupFileCorruptOrWrongVer=Pliki skladowe instalatora s� uszkodzone lub niezgodne z ta wersja Instalatora. Prosze rozwiazac ten problem lub uzyskac nowa kopie Instalatora od producenta.
NotOnThisPlatform=Ten program nie uruchomi sie pod systemem %1.
OnlyOnThisPlatform=Ten program wymaga systemu %1.
WinVersionTooLowError=Ten program wymaga %1 w wersji %2 lub p�zniejszej.
WinVersionTooHighError=Ten program nie moze byc zainstalowany w wersji %2 lub p�zniejszej systemu %1.
AdminPrivilegesRequired=Musisz miec uprawnienia administratora aby zainstalowac ten program.
PowerUserPrivilegesRequired=Musisz miec uprawnienia administratora lub uzytkownika zaawansowanego aby zainstalowac ten program.
SetupAppRunningError=Instalator wykryl, ze %1 jest aktualnie uruchomiony.%n%nZamknij wszystkie okienka tej aplikacji, a potem wybierz przycisk OK, aby kontynuowac, lub Anuluj, aby przerwac instalacje.
UninstallAppRunningError=Deinstalator wykryl, ze %1 jest aktualnie uruchomiony.%n%nZamknij teraz wszystkie okna tej aplikacji, a nastepnie wybierz przycisk OK, aby kontynuowac, lub Anuluj, aby przerwac deinstalacje.

; *** Misc. errors
ErrorCreatingDir=Instalator nie m�gl utworzyc folderu "%1"
ErrorTooManyFilesInDir=Nie mozna utworzyc pliku w folderze %1, poniewaz zawiera on za duzo plik�w

; *** Setup common messages
ExitSetupTitle=Zakoncz instalacje
ExitSetupMessage=Instalacja nie zostala zakonczona. Jesli zakonczysz j� teraz, aplikacja nie bedzie zainstalowana.%n%nJesli chcesz dokonczyc instalacje innym razem, uruchom ponownie Instalatora.%n%nZakonczyc instalacje?
AboutSetupMenuItem=&O Instalatorze...
AboutSetupTitle=O Instalatorze
AboutSetupMessage=%1 wersja %2%n%3%n%n Strona domowa %1:%n%4
AboutSetupNote=Polska wersja: Krzysztof Cynarski

; *** Buttons
ButtonBack=< &Wstecz
ButtonNext=&Dalej >
ButtonInstall=&Instaluj
ButtonOK=OK
ButtonCancel=Anuluj
ButtonYes=&Tak
ButtonYesToAll=Tak na &wszystkie
ButtonNo=&Nie
ButtonNoToAll=N&ie na wszystkie
ButtonFinish=&Zakoncz
ButtonBrowse=&Przegladaj...
ButtonWizardBrowse=P&rzegladaj...
ButtonNewFolder=&Utw�rz nowy folder

; *** "Select Language" dialog messages
SelectLanguageTitle=Wybierz jezyk instalacji
SelectLanguageLabel=Wybierz jezyk uzywany podczas instalacji:

; *** Common wizard text
ClickNext=Wybierz przycisk Dalej, aby kontynuowac, lub Anuluj, aby zakonczyc instalacje.
BeveledLabel=
BrowseDialogTitle=Wskaz folder
BrowseDialogLabel=Wybierz folder z ponizszej listy, a nastepnie wybierz przycisk OK.
NewFolderName=Nowy folder

; *** "Welcome" wizard page
WelcomeLabel1=Witamy w Kreatorze instalacji programu [name].
WelcomeLabel2=Instalator zainstaluje teraz program [name/ver] na Twoim komputerze.%n%nZalecane jest zamkniecie wszystkich innych uruchomionych program�w przed rozpoczeciem procesu instalacji.

; *** "Password" wizard page
WizardPassword=Haslo
PasswordLabel1=Ta instalacja jest zabezpieczona haslem.
PasswordLabel3=Podaj haslo, potem wybierz przycisk Dalej, aby kontynuowac. W haslach rozr�zniane sa duze i male litery.
PasswordEditLabel=&Haslo:
IncorrectPassword=Wprowadzone haslo nie jest poprawne. Spr�buj ponownie.

; *** "License Agreement" wizard page
WizardLicense=Umowa Licencyjna
LicenseLabel=Przed kontynuacja prosze przeczytac ponizsze wazne informacje.
LicenseLabel3=Prosze przeczytac tekst Umowy Licencyjnej. Musisz zgodzic sie na warunki tej umowy przed kontynuacja instalacji.
LicenseAccepted=&Akceptuje warunki umowy
LicenseNotAccepted=&Nie akceptuje warunk�w umowy

; *** "Information" wizard pages
WizardInfoBefore=Informacja
InfoBeforeLabel=Przed przejeciem do dalszego etapu instalacji, prosze przeczytac ponizsza informacje.
InfoBeforeClickLabel=Kiedy bedziesz gotowy do instalacji, kliknij przycisk Dalej.
WizardInfoAfter=Informacja
InfoAfterLabel=Przed przejeciem do dalszego etapu instalacji, prosze przeczytac ponizsza informacje.
InfoAfterClickLabel=Gdy bedziesz gotowy do zakonczenia instalacji, kliknij przycisk Dalej.

; *** "User Information" wizard page
WizardUserInfo=Dane Uzytkownika
UserInfoDesc=Prosze podac swoje dane.
UserInfoName=&Nazwisko:
UserInfoOrg=&Organizacja:
UserInfoSerial=Numer &seryjny:
UserInfoNameRequired=Musisz podac nazwisko.

; *** "Select Destination Location" wizard page
WizardSelectDir=Wybierz docelowa lokalizacje
SelectDirDesc=Gdzie ma byc zainstalowany program [name]?
SelectDirLabel3=Instalator zainstaluje program [name] do ponizszego folderu.
SelectDirBrowseLabel=Kliknij przycisk Nastepny, aby kontynuowac. Jesli chcesz okreslic inny folder, wybierz przycisk Przegladaj.
DiskSpaceMBLabel=Potrzeba przynajmniej [mb] MB wolnego miejsca na dysku.
ToUNCPathname=Instalator nie moze instalowac do sciezki UNC. Jesli pr�bujesz zainstalowac program na dysku sieciowym, najpierw zmapuj ten dysk.
InvalidPath=Musisz wprowadzic pelna sciezke wraz z liter� dysku, np.:%n%nC:\PROGRAM%n%nlub sciezke sieciowa (UNC) w formacie:%n%n\\serwer\udzial
InvalidDrive=Wybrany dysk lub udostepniony folder sieciowy nie istnieje. Prosze wybrac inny.
DiskSpaceWarningTitle=Niewystarczajaca ilosc wolnego miejsca na dysku
DiskSpaceWarning=Instalator wymaga co najmniej %1 KB wolnego miejsca na dysku. Wybrany dysk posiada tylko %2 KB dostepnego miejsca.%n%nCzy pomimo to chcesz kontynuowac?
DirNameTooLong=Nazwa folderu lub sciezki jest za dluga.
InvalidDirName=Niepoprawna nazwa folderu.
BadDirName32=Nazwa folderu nie moze zawierac zadnego z nastepujacych znak�w:%n%n%1
DirExistsTitle=Ten folder juz istnieje
DirExists=Folder%n%n%1%n%njuz istnieje. Czy pomimo to chcesz zainstalowac program w tym folderze?
DirDoesntExistTitle=Nie ma takiego folderu
DirDoesntExist=Folder:%n%n%1%n%nnie istnieje. Czy chcesz, aby zostal utworzony?

; *** "Select Components" wizard page
WizardSelectComponents=Zaznacz komponenty
SelectComponentsDesc=Kt�re komponenty maja byc zainstalowane?
SelectComponentsLabel2=Zaznacz komponenty, kt�re chcesz zainstalowac, odznacz te, kt�rych nie chcesz zainstalowac. Kliknij przycisk Dalej, aby kontynuowac.
FullInstallation=Instalacja pelna
; if possible don't translate 'Compact' as 'Minimal' (I mean 'Minimal' in your language)
CompactInstallation=Instalacja minimalna
CustomInstallation=Instalacja uzytkownika
NoUninstallWarningTitle=Zainstalowane komponenty
NoUninstallWarning=Instalator wykryl, ze w twoim komputerze s� juz zainstalowane nastepujace komponenty:%n%n%1%n%nOdznaczenie kt�regokolwiek z nich nie spowoduje ich odinstalacji.%n%nCzy pomimo tego chcesz kontynuowac?
ComponentSize1=%1 KB
ComponentSize2=%1 MB
ComponentsDiskSpaceMBLabel=Wybrane komponenty wymagaja co najmniej [mb] MB na dysku.

; *** "Select Additional Tasks" wizard page
WizardSelectTasks=Zaznacz dodatkowe zadania
SelectTasksDesc=Kt�re dodatkowe zadania maja byc wykonane?
SelectTasksLabel2=Zaznacz dodatkowe zadania, kt�re Instalator ma wykonac podczas instalacji programu [name], a nastepnie kliknij przycisk Dalej, aby kontynuowac.

; *** "Select Start Menu Folder" wizard page
WizardSelectProgramGroup=Wybierz folder Menu Start
SelectStartMenuFolderDesc=Gdzie maja byc umieszczone skr�ty do programu?
SelectStartMenuFolderLabel3=Instalator stworzy skr�ty do programu w ponizszym folderze Menu Start.
SelectStartMenuFolderBrowseLabel=Kliknij przycisk Nastepny, aby kontynuowac. Jesli chcesz okreslic inny folder, kliknij przycisk Przegladaj.
NoIconsCheck=&Nie tw�rz zadnych skr�t�w
MustEnterGroupName=Musisz wprowadzic nazwe folderu.
GroupNameTooLong=Nazwa folderu lub sciezki jest za dluga.
InvalidGroupName=Niepoprawna nazwa folderu.
BadGroupName=Nazwa folderu nie moze zawierac zadnego z nastepujacych znak�w:%n%n%1
NoProgramGroupCheck2=&Nie tw�rz folderu w Menu Start

; *** "Ready to Install" wizard page
WizardReady=Gotowy do rozpoczecia instalacji
ReadyLabel1=Instalator jest juz gotowy do rozpoczecia instalacji programu [name] na twoim komputerze.
ReadyLabel2a=Kliknij przycisk Instaluj, aby rozpoczac instalacje lub Wstecz, jesli chcesz przejrzec lub zmienic ustawienia.
ReadyLabel2b=Kliknij przycisk Instaluj, aby kontynuowac instalacje.
ReadyMemoUserInfo=Informacje uzytkownika:
ReadyMemoDir=Lokalizacja docelowa:
ReadyMemoType=Rodzaj instalacji:
ReadyMemoComponents=Wybrane komponenty:
ReadyMemoGroup=Folder w Menu Start:
ReadyMemoTasks=Dodatkowe zadania:

; *** "Preparing to Install" wizard page
WizardPreparing=Przygotowanie do instalacji
PreparingDesc=Instalator przygotowuje instalacje programu [name] na Twoim komputerze.
PreviousInstallNotCompleted=Instalacja (usuniecie) poprzedniej wersji programu nie zostala zakonczona. Bedziesz musial ponownie uruchomic komputer, aby zakonczyc instalacje. %n%nPo ponownym uruchomieniu komputera uruchom ponownie instalatora, aby zakonczyc instalacje aplikacji [name].
CannotContinue=Instalator nie moze kontynuowac. Kliknij przycisk Anuluj, aby przerwac instalacje.


; *** "Installing" wizard page
WizardInstalling=Instalacja
InstallingLabel=Poczekaj, az instalator zainstaluje aplikacje [name] na Twoim komputerze.

; *** "Setup Completed" wizard page
FinishedHeadingLabel=Zakonczono instalacje programu [name]
FinishedLabelNoIcons=Instalator zakonczyl instalacje programu [name] na Twoim komputerze.
FinishedLabel=Instalator zakonczyl instalacje programu [name] na Twoim komputerze. Aplikacja moze byc uruchomiona poprzez uzycie zainstalowanych skr�t�w.
ClickFinish=Kliknij przycisk Zakoncz, aby zakonczyc instalacje.
FinishedRestartLabel=Aby zakonczyc instalacje programu [name], Instalator musi ponownie uruchomic Tw�j komputer. Czy chcesz teraz wykonac restart komputera?
FinishedRestartMessage=Aby zakonczyc instalacje programu [name], Instalator musi ponownie uruchomic Tw�j komputer.%n%nCzy chcesz teraz wykonac restart komputera?
ShowReadmeCheck=Tak, chce przeczytac dodatkowe informacje
YesRadio=&Tak, teraz uruchom ponownie
NoRadio=&Nie, sam zrestartuje p�zniej
; used for example as 'Run MyProg.exe'
RunEntryExec=Uruchom %1
; used for example as 'View Readme.txt'
RunEntryShellExec=Pokaz %1

; *** "Setup Needs the Next Disk" stuff
ChangeDiskTitle=Instalator potrzebuje nastepnej dyskietki
SelectDiskLabel2=Prosze wlozyc dyskietke %1 i kliknac przycisk OK.%n%nJesli pokazany ponizej folder nie okresla polozenia plik�w z tej dyskietki, wprowadz poprawna sciezke lub kliknij przycisk Przegladaj.
PathLabel=S&ciezka:
FileNotInDir2=Plik "%1" nie zostal znaleziony na dyskietce "%2". Prosze wlozyc wlasciwa dyskietke lub wybrac inny folder.
SelectDirectoryLabel=Prosze okreslic lokalizacje nastepnej dyskietki.

; *** Installation phase messages
SetupAborted=Instalacja nie zostala zakonczona.%n%nProsze rozwiazac problem i ponownie rozpoczac instalacje.
EntryAbortRetryIgnore=Mozesz ponowic nieudana czynnosc, zignorowac ja (nie zalecane) lub przerwac instalacje.

; *** Installation status messages
StatusCreateDirs=Tworzenie folder�w...
StatusExtractFiles=Dekompresja plik�w...
StatusCreateIcons=Tworzenie ikon aplikacji...
StatusCreateIniEntries=Tworzenie zapis�w w plikach INI...
StatusCreateRegistryEntries=Tworzenie zapis�w w rejestrze...
StatusRegisterFiles=Rejestracja plik�w...
StatusSavingUninstall=Zachowanie informacji deinstalatora...
StatusRunProgram=Konczenie instalacji...
StatusRollback=Cofanie zmian...

; *** Misc. errors
ErrorInternal2=Wewnetrzny blad: %1
ErrorFunctionFailedNoCode=Blad podczas wykonywania %1
ErrorFunctionFailed=Blad podczas wykonywania %1; kod %2
ErrorFunctionFailedWithMessage=Blad podczas wykonywania %1; kod %2.%n%3
ErrorExecutingProgram=Nie mozna uruchomic:%n%1

; *** Registry errors
ErrorRegOpenKey=Blad podczas otwierania klucza rejestru:%n%1\%2
ErrorRegCreateKey=Blad podczas tworzenia klucza rejestru:%n%1\%2
ErrorRegWriteKey=Blad podczas zapisu do klucza rejestru:%n%1\%2

; *** INI errors
ErrorIniEntry=Blad podczas tworzenia pozycji w pliku INI: "%1".

; *** File copying errors
FileAbortRetryIgnore=Mozesz ponowic nieudana czynnosc, zignorowac ja, aby ominac ten plik (nie zalecane), lub przerwac instalacje.
FileAbortRetryIgnore2=Mozesz ponowic nieudana czynnosc, zignorowac ja (nie zalecane) lub przerwac instalacje.
SourceIsCorrupted=Plik zr�dlowy jest uszkodzony
SourceDoesntExist=Plik zr�dlowy "%1" nie istnieje
ExistingFileReadOnly=Istniejacy plik jest oznaczony jako tylko-do-odczytu.%n%nMozesz ponowic (aby usunac oznaczenie) zignorowac (aby ominac ten plik) lub przerwac instalacje.
ErrorReadingExistingDest=Wystapil blad podczas pr�by odczytu istniejacego pliku:
FileExists=Plik juz istnieje.%n%nCzy chcesz, aby Instalator zamienil go na nowy?
ExistingFileNewer=Istniejacy plik jest nowszy niz ten, kt�ry Instalator pr�buje skopiowac. Zalecanym jest zachowanie istniejacego pliku.%n%nCzy chcesz zachowac istniejacy plik?
ErrorChangingAttr=Podczas pr�by zmiany atrybut�w istniejacego pliku wystapil blad:
ErrorCreatingTemp=Podczas pr�by utworzenia pliku w folderze docelowym wystapil blad:
ErrorReadingSource=Podczas pr�by odczytu pliku zr�dlowego wystapil blad:
ErrorCopying=Podczas pr�by kopiowania pliku wystapil blad:
ErrorReplacingExistingFile=Podczas pr�by zamiany istniejacego pliku wystapil blad:
ErrorRestartReplace=Blad RestartReplace:
ErrorRenamingTemp=Podczas pr�by zmiany nazwy pliku w folderze docelowym wystapil blad:
ErrorRegisterServer=Nie mozna zarejestrowac DLL/OCX: %1
ErrorRegisterServerMissingExport=Eksportowana funkcja DllRegisterServer nie zostala znaleziony
ErrorRegisterTypeLib=Nie moge zarejestrowac biblioteki typ�w: %1

; *** Post-installation errors
ErrorOpeningReadme=Wystapil blad podczas pr�by otwarcia pliku README.
ErrorRestartingComputer=Instalator nie m�gl zrestartowac tego komputera. Prosze zrobic to samodzielnie.

; *** Uninstaller messages
UninstallNotFound=Plik "%1" nie istnieje. Nie mozna go odinstalowac.
UninstallOpenError=Plik "%1" nie m�gl byc otwarty. Nie mozna odinstalowac
UninstallUnsupportedVer=Ta wersja programu deinstalacyjnego nie rozpoznaje formatu logu deinstalacji. Nie mozna odinstalowac
UninstallUnknownEntry=W logu deinstalacji wystapila nie znana pozycja (%1)
ConfirmUninstall=Czy na pewno chcesz usunac program %1 i wszystkie jego skladniki?
OnlyAdminCanUninstall=Ta instalacja moze byc odinstalowana tylko przez uzytkownika z prawami administratora.
UninstallStatusLabel=Poczekaj az program %1 zostanie usuniety z Twojego komputera.
UninstalledAll=%1 zostal usuniety z Twojego komputera.
UninstalledMost=Odinstalowywanie programu %1 zakonczone.%n%nNiekt�re elementy nie mogly byc usuniete. Mozesz je usunac recznie.
UninstalledAndNeedsRestart=Tw�j komputer musi byc ponownie uruchomiony, aby zakonczyc odinstalowywanie %1.%n%nCzy chcesz teraz ponownie uruchomic komputer?
UninstallDataCorrupted=Plik "%1" jest uszkodzony. Nie mozna odinstalowac

; *** Uninstallation phase messages
ConfirmDeleteSharedFileTitle=Usunac plik wsp�ldzielony?
ConfirmDeleteSharedFile2=System wykryl, ze nastepujacy plik nie jest juz uzywany przez zaden program. Czy chcesz odinstalowac ten plik wsp�ldzielony?%n%nJesli inne programy nadal uzywaja tego pliku, a zostanie on usuniety, moga one przestac dzialac prawidlowo. Jesli nie jestes pewny, wybierz przycisk Nie. Pozostawienie tego pliku w Twoim systemie nie spowoduje zadnych szk�d.
SharedFileNameLabel=Nazwa pliku:
SharedFileLocationLabel=Polozenie:
WizardUninstalling=Stan deinstalacji
StatusUninstalling=Deinstalacja %1...

; The custom messages below aren't used by Setup itself, but if you make
; use of them in your scripts, you'll want to translate them.

[CustomMessages]
;Inno Setup Built-in Custom Messages
NameAndVersion=%1 wersja %2
AdditionalIcons=Dodatkowe ikony:
OptionalFeatures=Optional Features:
CreateDesktopIcon=Utw�rz ikone na &pulpicie
CreateQuickLaunchIcon=Utw�rz ikone na pasku &szybkiego uruchamiania
ProgramOnTheWeb=Strona WWW programu %1
UninstallProgram=Deinstalacja programu %1
LaunchProgram=Uruchom program %1
AssocFileExtension=&Przypisz program %1 do rozszerzenia pliku %2
AssocingFileExtension=Przypisywanie programu %1 do rozszerzenia pliku %2...

;Things we can also localize
CompanyName=Thingamahoochie Software

;Types
TypicalInstallation=Typowa instalacja
FullInstallation=Pelna instalacja
CompactInstallation=Ograniczona instalacja
CustomInstallation=Instalacja uzytkownika

;Components
AppCoreFiles=WinMerge ? pliki zasadnicze
ApplicationRuntimes=Moduly wykonywalne
UsersGuide=Przewodnik uzytkownika
Filters=Filtry
Plugins=Wtyczki (Zob. Plugins.txt)

;Localization Components
Languages=Jezyki
BulgarianLanguage=Bulgarski interfejs
CatalanLanguage=Katalonski interfejs
ChineseSimplifiedLanguage=Chinski (uproszczony) interfejs
ChineseTraditionalLanguage=Chinski (tradycyjny) interfejs
CzechLanguage=Czeski interfejs
DanishLanguage=Dunski interfejs
DutchLanguage=Holenderski interfejs
FrenchLanguage=Francuski interfejs
GermanLanguage=Niemiecki interfejs
HungarianLanguage=Wegierski interfejs
ItalianLanguage=Wloski interfejs
JapaneseLanguage=Japonski interfejs
KoreanLanguage=Koreanski interfejs
NorwegianLanguage=Norweski interfejs
PolishLanguage=Polski interfejs
PortugueseLanguage=Portugalski (Brazylijski) interfejs
RussianLanguage=Rosyjski interfejs
SlovakLanguage=Slowacki interfejs
SpanishLanguage=Hiszpanski interfejs
SwedishLanguage=Szwedzki interfejs
TurkishLanguage=turecki interfejs

;Tasks
ExplorerContextMenu=&Wlacz integracje z menu Eksploratora
IntegrateTortoiseCVS=Integracja z &TortoiseCVS
IntegrateDelphi4=Obsluga plik�w binarnych Borland� Delphi &4
UpdatingCommonControls=Aktualizacja wsp�lnych kontrolek systemowych SCC

;Icon Labels
ReadMe=Czytaj to
UsersGuide=Przewodnik uzytkownika
UpdatingCommonControls=Aktualizacja wsp�lnych kontrolek systemowych
ViewStartMenuFolder=&Podglad katalogu menu Start programu WinMerge

;Code Dialogs
DeletePreviousStartMenu=Instalator wykryl, ze zmieniles lokalizacje menu startowego z "%s" na "%s". Czy usunac poprzedni katalog menu startowego?
