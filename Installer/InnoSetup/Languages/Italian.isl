; *** Inno Setup versione 4.2.2+ lingua Italiana ***
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
; Italian.isl revisione 18 - 2004/04/28
;
; Tradotto da ale5000 - ale5000@tiscali.it - http://digilander.libero.it/kazaaita/
; Segnalatemi via e-mail eventuali errori o suggerimenti

[LangOptions]
LanguageName=Italiano
LanguageID=$0410
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
SetupAppTitle=Installazione
SetupWindowTitle=Installazione di %1
UninstallAppTitle=Disinstallazione
UninstallAppFullTitle=Disinstallazione di %1

; *** Misc. common
InformationTitle=Informazioni
ConfirmTitle=Conferma
ErrorTitle=Errore

; *** SetupLdr messages
SetupLdrStartupMessage=Questa � l'installazione di %1. Vuoi continuare?
LdrCannotCreateTemp=Impossibile creare i file temporanei. Installazione annullata
LdrCannotExecTemp=Impossibile eseguire i file nella cartella temporanea. Installazione annullata

; *** Startup error messages
LastErrorMessage=%1.%n%nErrore %2: %3
SetupFileMissing=File %1 non trovato nella cartella di installazione. Correggere il problema o richiedere una nuova copia del software.
SetupFileCorrupt=I file di installazione sono danneggiati. Richiedere una nuova copia del software.
SetupFileCorruptOrWrongVer=I file di installazione sono danneggiati, oppure sono incompatibili con questa versione del programma. Correggere il problema o richiedere una nuova copia del software.
NotOnThisPlatform=Questo programma non � compatibile con %1.
OnlyOnThisPlatform=Questo programma richiede per funzionare %1.
WinVersionTooLowError=Questo programma richiede %1 versione %2 o successiva.
WinVersionTooHighError=Questo programma non pu� essere installato su %1 versione %2 o successiva.
AdminPrivilegesRequired=Per installare questo programma si deve accedere al sistema come amministratore.
PowerUserPrivilegesRequired=Devi essere connesso come amministratore per poter installare questo programma.
SetupAppRunningError=%1 � attualmente in esecuzione.%n%nChiudere adesso tutte le istanze del programma e poi premere OK, oppure premere Annulla per uscire.
UninstallAppRunningError=%1 � attualmente in esecuzione.%n%nChiudere adesso tutte le istanze del programma e poi premere OK, oppure premere Annulla per uscire.

; *** Misc. errors
ErrorCreatingDir=La cartella "%1" non pu� essere creata
ErrorTooManyFilesInDir=Impossibile creare i file nella cartella "%1" in quanto contiene troppi file

; *** Setup common messages
ExitSetupTitle=Uscita dall'installazione
ExitSetupMessage=L'installazione non � completa. Uscendo dall'installazione in questo momento, il programma non sar� installato.%n%n� possibile eseguire l'installazione in un secondo tempo.%n%nUscire dall'installazione?
AboutSetupMenuItem=&Informazioni sull'installazione...
AboutSetupTitle=Informazioni sull'installazione
AboutSetupMessage=%1 versione %2%n%3%n%n%1 home page:%n%4
AboutSetupNote=

; *** Buttons
ButtonBack=< &Indietro
ButtonNext=&Avanti >
ButtonInstall=&Installa
ButtonOK=OK
ButtonCancel=Annulla
ButtonYes=&S�
ButtonYesToAll=S� a &tutto
ButtonNo=&No
ButtonNoToAll=N&o a tutto
ButtonFinish=&Fine
ButtonBrowse=&Sfoglia...
ButtonWizardBrowse=S&foglia...
ButtonNewFolder=&Crea nuova cartella

; *** "Select Language" dialog messages
SelectLanguageTitle=Selezionare la lingua dell'installazione
SelectLanguageLabel=Selezionare la lingua da utilizzare durante l'installazione:

; *** Common wizard text
ClickNext=Premere Avanti per continuare, o Annulla per uscire.
BeveledLabel=
BrowseDialogTitle=Sfoglia per cartelle
BrowseDialogLabel=Selezionare la cartella in questa lista, poi premere OK.
NewFolderName=Nuova cartella

; *** "Welcome" wizard page
WelcomeLabel1=Benvenuti nel programma di installazione di [name]
WelcomeLabel2=[name/ver] sar� installato sul computer.%n%nSi consiglia di chiudere tutte le applicazioni attive prima di procedere.

; *** "Password" wizard page
WizardPassword=Password
PasswordLabel1=Questa installazione � protetta da password.
PasswordLabel3=Inserire la password, poi premere Avanti per continuare. Le password distinguono tra maiuscole e minuscole.
PasswordEditLabel=&Password:
IncorrectPassword=La password inserita non � corretta, riprovare.

; *** "License Agreement" wizard page
WizardLicense=Contratto di licenza
LicenseLabel=Leggere con attenzione le informazioni che seguono prima di procedere.
LicenseLabel3=Leggere il seguente contratto di licenza. � necessario accettare tutti i termini di questo accordo per procedere con l'installazione.
LicenseAccepted=&Accetto i termini del contratto di licenza
LicenseNotAccepted=&Non accetto i termini del contratto di licenza

; *** "Information" wizard pages
WizardInfoBefore=Informazioni
InfoBeforeLabel=Leggere le importanti informazioni che seguono prima di procedere.
InfoBeforeClickLabel=Quando sei pronto per proseguire, premi Avanti.
WizardInfoAfter=Informazioni
InfoAfterLabel=Leggere le importanti informazioni che seguono prima di procedere.
InfoAfterClickLabel=Quando sei pronto per proseguire, premi Avanti.

; *** "User Information" wizard page
WizardUserInfo=Informazioni utente
UserInfoDesc=Inserire le seguenti informazioni.
UserInfoName=&Nome:
UserInfoOrg=&Societ�:
UserInfoSerial=&Numero di serie:
UserInfoNameRequired=� necessario inserire un nome.

; *** "Select Destination Location" wizard page
WizardSelectDir=Selezione della cartella di destinazione
SelectDirDesc=Dove vuoi installare [name]?
SelectDirLabel3=[name] sar� installato nella seguente cartella.
SelectDirBrowseLabel=Per continuare, premere Avanti. Per selezionare un'altra cartella, premere Sfoglia.
DiskSpaceMBLabel=Il programma richiede almeno [mb] MB di spazio sul disco.
ToUNCPathname=Non � possibile installare su un percorso UNC. Se stai installando attraverso una rete, devi connettere la risorsa come unit� di rete.
InvalidPath=Devi inserire un percorso completo di lettera di unit�; per esempio:%n%nC:\APP%n%no un percorso UNC nella forma:%n%n\\server\share
InvalidDrive=L'unit� o lo share UNC selezionato non esiste o non � accessibile. Selezionarne un'altro.
DiskSpaceWarningTitle=Spazio su disco insufficiente
DiskSpaceWarning=L'installazione richiede almeno %1 KB di spazio libero per installare, ma l'unit� selezionata ha solo %2 KB disponibili.%n%nVuoi continuare comunque?
DirNameTooLong=Il nome della cartella o il percorso sono troppo lunghi.
InvalidDirName=Il nome della cartella non � valido.
BadDirName32=Il nome della cartella non pu� includere nessuno dei caratteri seguenti:%n%n%1
DirExistsTitle=Cartella gi� esistente
DirExists=La cartella:%n%n%1 esiste gi�.%n%nVuoi utilizzarla comunque?
DirDoesntExistTitle=Cartella inesistente
DirDoesntExist=La cartella:%n%n%1 non esiste.%n%nVuoi crearla?

; *** "Select Components" wizard page
WizardSelectComponents=Selezione componenti
SelectComponentsDesc=Quali componenti devono essere installati?
SelectComponentsLabel2=Selezionare i componenti da installare, deselezionare quelli che non vuoi installare. Premere Avanti per continuare.
FullInstallation=Installazione completa
; if possible don't translate 'Compact' as 'Minimal' (I mean 'Minimal' in your language)
CompactInstallation=Installazione compatta
CustomInstallation=Installazione personalizzata
NoUninstallWarningTitle=Componente esistente
NoUninstallWarning=L'installazione ha rilevato che i seguenti componenti sono gi� installati sul computer:%n%n%1%n%nDeselezionando questi componenti non verranno disinstallati.%n%nVuoi continuare comunque?
ComponentSize1=%1 KB
ComponentSize2=%1 MB
ComponentsDiskSpaceMBLabel=La selezione corrente richiede almeno [mb] MB di spazio su disco.

; *** "Select Additional Tasks" wizard page
WizardSelectTasks=Selezione processi addizionali
SelectTasksDesc=Quali processi addizionali vuoi avviare?
SelectTasksLabel2=Selezionare i processi addizionali da eseguire installando [name], poi premere Avanti.

; *** "Select Start Menu Folder" wizard page
WizardSelectProgramGroup=Selezione della cartella nel Menu Avvio/Start
SelectStartMenuFolderDesc=Dove vuoi che l'installazione inserisca i collegamenti al programma?
SelectStartMenuFolderLabel3=Saranno creati i collegamenti al programma nella seguente cartella del Menu Avvio/Start.
SelectStartMenuFolderBrowseLabel=Per continuare, premere Avanti. Per selezionare un'altra cartella, premere Sfoglia.
NoIconsCheck=&Non creare nessuna icona
MustEnterGroupName=Devi inserire il nome della cartella.
GroupNameTooLong=Il nome della cartella o il percorso sono troppo lunghi.
InvalidGroupName=Il nome della cartella non � valido.
BadGroupName=Il nome della cartella non pu� includere nessuno dei caratteri seguenti:%n%n%1
NoProgramGroupCheck2=&Non creare una cartella nel Menu Avvio/Start

; *** "Ready to Install" wizard page
WizardReady=Pronto per l'installazione
ReadyLabel1=Il programma � pronto per iniziare l'installazione di [name] sul computer.
ReadyLabel2a=Premere Installa per continuare con l'installazione, o premere Indietro per rivedere o modificare le impostazioni.
ReadyLabel2b=Premere Installa per procedere con l'installazione.
ReadyMemoUserInfo=Informazioni utente:
ReadyMemoDir=Cartella di destinazione:
ReadyMemoType=Tipo di installazione:
ReadyMemoComponents=Componenti selezionati:
ReadyMemoGroup=Cartella del menu Avvio/Start:
ReadyMemoTasks=Processi addizionali:

; *** "Preparing to Install" wizard page
WizardPreparing=Preparazione all'installazione
PreparingDesc=Preparazione all'installazione di [name] sul computer.
PreviousInstallNotCompleted=L'installazione/disinstallazione precedente del programma non � stata completata. � necessario riavviare il computer per completare l'installazione.%n%nAl successivo riavvio del sistema eseguire di nuovo l'installazione di [name].
CannotContinue=L'installazione non pu� continuare. Premere Annulla per uscire.

; *** "Installing" wizard page
WizardInstalling=Installazione in corso
InstallingLabel=Attendere il completamento dell'installazione di [name] sul computer.

; *** "Setup Completed" wizard page
FinishedHeadingLabel=Completamento dell'installazione guidata di [name]
FinishedLabelNoIcons=L'installazione di [name] � stata completata con successo.
FinishedLabel=L'installazione di [name] � stata completata con successo. L'applicazione pu� essere eseguita selezionando le icone installate.
ClickFinish=Premere Fine per uscire dall'installazione.
FinishedRestartLabel=Per completare l'installazione di [name], � necessario riavviare il computer. Vuoi riavviare ora?
FinishedRestartMessage=Per completare l'installazione di [name], � necessario riavviare il computer.%n%nVuoi riavviare ora?
ShowReadmeCheck=S�, desidero visualizzare il file LEGGIMI ora
YesRadio=&Si, riavvia il computer adesso
NoRadio=&No, riavvio il computer pi� tardi
; used for example as 'Run MyProg.exe'
RunEntryExec=Lancia %1
; used for example as 'View Readme.txt'
RunEntryShellExec=Visualizza %1

; *** "Setup Needs the Next Disk" stuff
ChangeDiskTitle=L'installazione necessita del disco successivo
SelectDiskLabel2=Inserire il disco %1 e premere OK.%n%nSe i file su questo disco possono essere trovati in una cartella diversa da quella visualizzata sotto, inserire il percorso corretto o premere Sfoglia.
PathLabel=&Percorso:
FileNotInDir2=Il file "%1" non pu� essere individuato  in "%2". Inserire il disco corretto o selezionare un'altra cartella.
SelectDirectoryLabel=Specificare il percorso del prossimo disco.

; *** Installation phase messages
SetupAborted=L'installazione non � stata completata.%n%nCorreggere il problema e rieseguire nuovamente l'installazione.
EntryAbortRetryIgnore=Premere Riprova per ritentare nuovamente, Ignora per procedere in ogni caso, o Interrompi per terminare l'installazione.

; *** Installation status messages
StatusCreateDirs=Creazione cartelle...
StatusExtractFiles=Estrazione file...
StatusCreateIcons=Creazione icone...
StatusCreateIniEntries=Creazione voci del file INI...
StatusCreateRegistryEntries=Creazione voci del registro...
StatusRegisterFiles=Registrazione file...
StatusSavingUninstall=Salvataggio delle informazioni di disinstallazione...
StatusRunProgram=Conclusione dell'installazione...
StatusRollback=Recupero delle modifiche...

; *** Misc. errors
ErrorInternal2=Errore interno %1
ErrorFunctionFailedNoCode=%1 fallito
ErrorFunctionFailed=%1 fallito; codice %2
ErrorFunctionFailedWithMessage=%1 fallito; codice %2.%n%3
ErrorExecutingProgram=Impossibile eseguire il file:%n%1

; *** Registry errors
ErrorRegOpenKey=Errore di apertura della chiave del registro:%n%1\%2
ErrorRegCreateKey=Errore di creazione della chiave del registro:%n%1\%2
ErrorRegWriteKey=Errore di scrittura della chiave del registro:%n%1\%2

; *** INI errors
ErrorIniEntry=Errore nella creazione delle voci INI nel file "%1".

; *** File copying errors
FileAbortRetryIgnore=Premere Riprova per tentare di nuovo, Ignora per saltare questo file (sconsigliato), o Interrompi per terminare l'installazione.
FileAbortRetryIgnore2=Premere Riprova per tentare di nuovo, Ignora per proseguire comunque (sconsigliato), o Interrompi per terminare l'installazione.
SourceIsCorrupted=Il file sorgente � danneggiato
SourceDoesntExist=Il file sorgente "%1" non esiste
ExistingFileReadOnly=Il file esistente ha attributi di sola lettura.%n%nPremere Riprova per rimuovere l'attributo di sola lettura e ritentare, Ignora per saltare questo file, o Interrompi per terminare l'installazione.
ErrorReadingExistingDest=Si � verificato un errore durante la lettura del file esistente:
FileExists=Il file esiste gi�.%n%nDesiderate sovrascriverlo?
ExistingFileNewer=Il file esistente � pi� recente di quello che si sta installando. Si raccomanda di mantenere il file esistente.%n%nVuoi mantenere il file esistente?
ErrorChangingAttr=Si � verificato un errore durante il tentativo di modifica dell'attributo del file esistente:
ErrorCreatingTemp=Si � verificato un errore durante la creazione di un file nella cartella di destinazione:
ErrorReadingSource=Si � verificato un errore durante la lettura del file sorgente:
ErrorCopying=Si � verificato un errore durante la copia di un file:
ErrorReplacingExistingFile=Si � verificato un errore durante la sovrascrittura del file esistente:
ErrorRestartReplace=Errore durante fase di riavvio-sostituzione:
ErrorRenamingTemp=Si � verificato un errore durante il tentativo di rinominare un file nella cartella di destinazione:
ErrorRegisterServer=Impossibile registrare la DLL/OCX: %1
ErrorRegisterServerMissingExport=DllRegisterServer esportazione mancante
ErrorRegisterTypeLib=Impossibile registrare il tipo di libreria: %1

; *** Post-installation errors
ErrorOpeningReadme=Si � verificato un errore durante l'apertura del file LEGGIMI.
ErrorRestartingComputer=L'installazione non � riuscita a riavviare il computer. Riavviare manualmente.

; *** Uninstaller messages
UninstallNotFound=Il file "%1" non esiste. Impossibile disinstallare.
UninstallOpenError=Il file "%1" non pu� essere aperto. Impossibile disinstallare
UninstallUnsupportedVer=Il file log di disinstallazione "%1" � in un formato non riconosciuto da questa versione del programma. Impossibile disinstallare
UninstallUnknownEntry=Trovata una voce sconosciuta (%1) nel file di log della disinstallazione
ConfirmUninstall=Sei sicuro di voler rimuovere completamente %1 e tutti i suoi componenti?
OnlyAdminCanUninstall=Questa applicazione pu� essere disinstallata solo da un utente con privilegi di Amministratore.
UninstallStatusLabel=Attendere fino a che %1 � stato rimosso dal computer.
UninstalledAll=%1 � stato rimosso con successo dal computer.
UninstalledMost=%1 disinstallazione completata.%n%nAlcuni elementi non possono essere rimossi. Dovranno essere rimossi manualmente.
UninstalledAndNeedsRestart=Per completare la disinstallazione di %1, � necessario riavviare il sistema.%n%nVuoi riavviare adesso?
UninstallDataCorrupted="%1" file � danneggiato. Impossibile disinstallare

; *** Uninstallation phase messages
ConfirmDeleteSharedFileTitle=Rimuovere il file condiviso?
ConfirmDeleteSharedFile2=Il sistema indica che il seguente file condiviso non � pi� usato da nessun programma. Vuoi rimuovere questo file condiviso?%n%nSe qualche programma usasse questo file, potrebbe non funzionare pi� correttamente. Se non sei sicuro, scegli No. Lasciare il file nel sistema non pu� causare danni.
SharedFileNameLabel=Nome del file:
SharedFileLocationLabel=Posizione:
WizardUninstalling=Stato della disinstallazione
StatusUninstalling=Disinstallazione in corso di %1...

; The custom messages below aren't used by Setup itself, but if you make
; use of them in your scripts, you'll want to translate them.

[CustomMessages]
;Inno Setup Built-in Custom Messages
NameAndVersion=%1 versione %2
AdditionalIcons=Icone aggiuntive:
CreateDesktopIcon=Crea un'icona sul &desktop
CreateQuickLaunchIcon=Crea un'icona nella barra &Avvio veloce
ProgramOnTheWeb=%1 sul Web
UninstallProgram=Disinstalla %1
LaunchProgram=Lancia %1
AssocFileExtension=&Associa %1 con l'estensione %2
AssocingFileExtension=Associazione di %1 con l'estensione %2 in corso...

;Things we can also localize
CompanyName=Thingamahoochie Software

;Translated by Andrea Decorte <klenje@adriacom.it>
;Types
TypicalInstallation=Installazione tipica
FullInstallation=Installazione completa
CompactInstallation=Installazione compatta
CustomInstallation=Installazione personalizzata

;Components
AppCoreFiles=File base di WinMerge
ApplicationRuntimes=Runtime dell'applicazione
UsersGuide=Guida per l'utente
Filters=Filtri
Plugins=Plugin (Vedi file Plugins.txt)

;Localization Components
Languages=Lingue
BulgarianLanguage=Menu e finestre di dialogo in bulgaro
CatalanLanguage=Menu e finestre di dialogo in catalano
ChineseSimplifiedLanguage=Menu e finestre di dialogo in cinese (semplificato)
ChineseTraditionalLanguage=Menu e finestre di dialogo in cinese (tradizionale)
CzechLanguage=Menu e finestre di dialogo in ceco
DanishLanguage=Menu e finestre di dialogo in danese
DutchLanguage=Menu e finestre di dialogo in olandese
FrenchLanguage=Menu e finestre di dialogo in francese
GermanLanguage=Menu e finestre di dialogo in tedesco
HungarianLanguage=Hungarian menus and dialogs
ItalianLanguage=Menu e finestre di dialogo in italiano
JapaneseLanguage=Menu e finestre di dialogo in giapponese
KoreanLanguage=Menu e finestre di dialogo in coreano
NorwegianLanguage=Menu e finestre di dialogo in norvegese
PolishLanguage=Menu e finestre di dialogo in polacco
PortugueseLanguage=Menu e finestre di dialogo in portoghese (brasiliano)
RussianLanguage=Menu e finestre di dialogo in russo
SlovakLanguage=Menu e finestre di dialogo in slovacco
SpanishLanguage=Menu e finestre di dialogo in spagnolo
SwedishLanguage=Menu e finestre di dialogo in svedese

;Tasks
ExplorerContextMenu=&Attiva l'integrazione con il menu contestuale di Explorer
IntegrateTortoiseCVS=Integrazione con &TortoiseCVS
IntegrateDelphi4=Supporto per i file binari di Borland� Delphi &4
UpdatingCommonControls=Aggiornamento dei controlli comuni del sistema in corso...

;Icon Labels
ReadMe=Leggimi
UsersGuide=Guida per l'utente
UpdatingCommonControls=Aggiornamento dei controlli comuni del sistema in corso...
ViewStartMenuFolder=&Visualizza cartella del menu Start di WinMerge

;Code Dialogs
DeletePreviousStartMenu=L'installer ha rilevato che il tuo menu Start � stato spostato da "%s" a "%s". Desideri eliminare la precedente cartella del menu Start?
