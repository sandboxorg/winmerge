; *** Inno Setup version 5.1.11+ Spanish messages ***
;
; Maintained by Jorge Andres Brugger (jbrugger@gmx.net)
; Spanish.isl version 0.8 (20070328)
; Default.isl version 1.69
; 
; Thanks to Germ�n Giraldo, Jordi Latorre, Ximo Tamarit, Emiliano Llano, 
; Ram�n Verduzco, Graciela Garc�a and Carles Millan

[LangOptions]
; The following three entries are very important. Be sure to read and 
; understand the '[LangOptions] section' topic in the help file.
LanguageName=Espa<00F1>ol
LanguageID=$0c0a
LanguageCodePage=1252
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
SetupAppTitle=Instalar
SetupWindowTitle=Instalar - %1
UninstallAppTitle=Desinstalar
UninstallAppFullTitle=Desinstalar - %1

; *** Misc. common
InformationTitle=Informaci�n
ConfirmTitle=Confirmar
ErrorTitle=Error

; *** SetupLdr messages
SetupLdrStartupMessage=Este programa instalar� %1. �Desea continuar?
LdrCannotCreateTemp=Imposible crear archivo temporal. Instalaci�n interrumpida
LdrCannotExecTemp=Imposible ejecutar archivo en la carpeta temporal. Instalaci�n interrumpida

; *** Startup error messages
LastErrorMessage=%1.%n%nError %2: %3
SetupFileMissing=El archivo %1 no se encuentra en la carpeta de instalaci�n. Por favor, solucione el problema u obtenga una copia nueva del programa.
SetupFileCorrupt=Los archivos de instalaci�n est�n da�ados. Por favor, obtenga una copia nueva del programa.
SetupFileCorruptOrWrongVer=Los archivos de instalaci�n est�n da�ados, o son incompatibles con esta versi�n del programa de instalaci�n. Por favor, solucione el problema u obtenga una copia nueva del programa.
NotOnThisPlatform=Este programa no se ejecutar� en %1.
OnlyOnThisPlatform=Este programa debe ejecutarse en %1.
OnlyOnTheseArchitectures=Este programa s�lo puede instalarse en versiones de Windows dise�adas para las siguientes arquitecturas de procesadores:%n%n%1
MissingWOW64APIs=La versi�n de Windows que est� utilizando no cuenta con la funcionalidad requerida por el instalador para realizar una instalaci�n de 64-bits. Para solucionar este problema, por favor instale el Service Pack %1.
WinVersionTooLowError=Este programa requiere %1 versi�n %2 o posterior.
WinVersionTooHighError=Este programa no puede instalarse en %1 versi�n %2 o posterior.
AdminPrivilegesRequired=Debe iniciar la sesi�n como administrador para instalar este programa.
PowerUserPrivilegesRequired=Debe iniciar la sesi�n como administrador o como un miembro del grupo Usuarios Avanzados para instalar este programa.
SetupAppRunningError=El programa de instalaci�n ha detectado que %1 est� ejecut�ndose.%n%nPor favor, ci�rrelo ahora, luego haga clic en Aceptar para continuar, o en Cancelar para salir.
UninstallAppRunningError=El desinstalador ha detectado que %1 est� ejecut�ndose.%n%nPor favor, ci�rrelo ahora, luego haga clic en Aceptar para continuar, o en Cancelar para salir.

; *** Misc. errors
ErrorCreatingDir=El programa de instalaci�n no pudo crear la carpeta "%1"
ErrorTooManyFilesInDir=Imposible crear un archivo en la carpeta "%1" porque contiene demasiados archivos

; *** Setup common messages
ExitSetupTitle=Salir de la Instalaci�n
ExitSetupMessage=La instalaci�n no se ha completado a�n. Si cancela ahora, el programa no ser� instalado.%n%nPuede ejecutar nuevamente el programa de instalaci�n en otra ocasi�n para completarla.%n%n�Salir de la instalaci�n?
AboutSetupMenuItem=&Acerca de Instalar...
AboutSetupTitle=Acerca de Instalar
AboutSetupMessage=%1 versi�n %2%n%3%n%n%1 sitio Web:%n%4
AboutSetupNote=
TranslatorNote=Spanish translation maintained by Jorge Andres Brugger (jbrugger@gmx.net)

; *** Buttons
ButtonBack=< &Atr�s
ButtonNext=&Siguiente >
ButtonInstall=&Instalar
ButtonOK=Aceptar
ButtonCancel=Cancelar
ButtonYes=&S�
ButtonYesToAll=S� a &Todo
ButtonNo=&No
ButtonNoToAll=N&o a Todo
ButtonFinish=&Finalizar
ButtonBrowse=&Examinar...
ButtonWizardBrowse=&Examinar...
ButtonNewFolder=&Crear Nueva Carpeta

; *** "Select Language" dialog messages
SelectLanguageTitle=Seleccione el Idioma de la Instalaci�n
SelectLanguageLabel=Seleccione el idioma a utilizar durante la instalaci�n:

; *** Common wizard text
ClickNext=Haga clic en Siguiente para continuar, o en Cancelar para salir de la instalaci�n.
BeveledLabel=
BrowseDialogTitle=Buscar Carpeta
BrowseDialogLabel=Seleccione una carpeta, y luego haga clic en Aceptar.
NewFolderName=Nueva Carpeta

; *** "Welcome" wizard page
WelcomeLabel1=Bienvenido al asistente de instalaci�n de [name]
WelcomeLabel2=Este programa instalar� [name/ver] en su sistema.%n%nSe recomienda que cierre todas las dem�s aplicaciones antes de continuar.

; *** "Password" wizard page
WizardPassword=Contrase�a
PasswordLabel1=Esta instalaci�n est� protegida por contrase�a.
PasswordLabel3=Por favor, ingrese la contrase�a, y haga clic en Siguiente para continuar. En las contrase�as se hace diferencia entre may�sculas y min�sculas.
PasswordEditLabel=&Contrase�a:
IncorrectPassword=La contrase�a ingresada no es correcta. Por favor, int�ntelo nuevamente.

; *** "License Agreement" wizard page
WizardLicense=Acuerdo de Licencia
LicenseLabel=Por favor, lea la siguiente informaci�n de importancia antes de continuar.
LicenseLabel3=Por favor, lea el siguiente acuerdo de licencia. Debe aceptar los t�rminos de este acuerdo antes de continuar con la instalaci�n.
LicenseAccepted=A&cepto el acuerdo
LicenseNotAccepted=&No acepto el acuerdo

; *** "Information" wizard pages
WizardInfoBefore=Informaci�n
InfoBeforeLabel=Por favor, lea la siguiente informaci�n de importancia antes de continuar.
InfoBeforeClickLabel=Cuando est� listo para continuar con la instalaci�n, haga clic en Siguiente.
WizardInfoAfter=Informaci�n
InfoAfterLabel=Por favor, lea la siguiente informaci�n de importancia antes de continuar.
InfoAfterClickLabel=Cuando est� listo para continuar, haga clic en Siguiente.

; *** "User Information" wizard page
WizardUserInfo=Informaci�n de Usuario
UserInfoDesc=Por favor, introduzca su informaci�n.
UserInfoName=Nombre de &Usuario:
UserInfoOrg=&Organizaci�n:
UserInfoSerial=N�mero de &Serie:
UserInfoNameRequired=Debe ingresar un nombre.

; *** "Select Destination Location" wizard page
WizardSelectDir=Seleccione la Carpeta de Destino
SelectDirDesc=�D�nde debe instalarse [name]?
SelectDirLabel3=El programa instalar� [name] en la siguiente carpeta.
SelectDirBrowseLabel=Para continuar, haga clic en Siguiente. Si desea seleccionar una carpeta diferente, haga clic en Examinar.
DiskSpaceMBLabel=Se requieren al menos [mb] MB de espacio libre en el disco.
ToUNCPathname=No es posible realizar la instalaci�n en una ruta de acceso UNC. Si est� intentando instalar en una red, necesitar� mapear una unidad de la red.
InvalidPath=Debe ingresar una ruta completa con la letra de la unidad; por ejemplo:%n%nC:\APP%n%no una ruta de acceso UNC de la siguiente forma:%n%n\\servidor\compartido
InvalidDrive=La unidad o ruta de acceso UNC que seleccion� no existe o no es accesible. Por favor, seleccione otra.
DiskSpaceWarningTitle=Espacio Insuficiente en Disco
DiskSpaceWarning=La instalaci�n requiere al menos %1 KB de espacio libre, pero la unidad seleccionada s�lo cuenta con %2 KB disponibles.%n%n�Desea continuar de todas formas?
DirNameTooLong=El nombre de la carpeta o la ruta son demasiado largos.
InvalidDirName=El nombre de la carpeta no es v�lido.
BadDirName32=Los nombres de carpetas no pueden incluir los siguientes caracteres:%n%n%1
DirExistsTitle=La Carpeta Ya Existe
DirExists=La carpeta:%n%n%1%n%nya existe. �Desea realizar la instalaci�n en esa carpeta de todas formas?
DirDoesntExistTitle=La Carpeta No Existe
DirDoesntExist=La carpeta:%n%n%1%n%nno existe. �Desea crear esa carpeta?

; *** "Select Components" wizard page
WizardSelectComponents=Seleccione los Componentes
SelectComponentsDesc=�Qu� componentes deben instalarse?
SelectComponentsLabel2=Seleccione los componentes que desea instalar; desactive los componentes que no desea instalar. Haga clic en Siguiente cuando est� listo para continuar.
FullInstallation=Instalaci�n Completa
; if possible don't translate 'Compact' as 'Minimal' (I mean 'Minimal' in your language)
CompactInstallation=Instalaci�n Compacta
CustomInstallation=Instalaci�n Personalizada
NoUninstallWarningTitle=Componentes Existentes
NoUninstallWarning=El programa de instalaci�n ha detectado que los siguientes componentes ya est�n instalados en su sistema:%n%n%1%n%nQuitar la selecci�n a estos componentes no los desinstalar�.%n%n�Desea continuar de todos modos?
ComponentSize1=%1 KB
ComponentSize2=%1 MB
ComponentsDiskSpaceMBLabel=La selecci�n actual requiere al menos [mb] MB de espacio en disco.

; *** "Select Additional Tasks" wizard page
WizardSelectTasks=Seleccione las Tareas Adicionales
SelectTasksDesc=�Qu� tareas adicionales deben realizarse?
SelectTasksLabel2=Seleccione las tareas adicionales que desea que se realicen durante la instalaci�n de [name] y haga clic en Siguiente.

; *** "Select Start Menu Folder" wizard page
WizardSelectProgramGroup=Seleccione la Carpeta del Men� Inicio
SelectStartMenuFolderDesc=�D�nde deben colocarse los accesos directos del programa?
SelectStartMenuFolderLabel3=El programa de instalaci�n crear� los accesos directos del programa en la siguiente carpeta del Men� Inicio.
SelectStartMenuFolderBrowseLabel=Para continuar, haga clic en Siguiente. Si desea seleccionar una carpeta distinta, haga clic en Examinar.
MustEnterGroupName=Debe proporcionar un nombre de carpeta.
GroupNameTooLong=El nombre de la carpeta o la ruta son demasiado largos.
InvalidGroupName=El nombre de la carpeta no es v�lido.
BadGroupName=El nombre de la carpeta no puede incluir ninguno de los siguientes caracteres:%n%n%1
NoProgramGroupCheck2=&No crear una carpeta en el Men� Inicio

; *** "Ready to Install" wizard page
WizardReady=Listo para Instalar
ReadyLabel1=Ahora el programa est� listo para iniciar la instalaci�n de [name] en su sistema.
ReadyLabel2a=Haga clic en Instalar para continuar con el proceso, o haga clic en Atr�s si desea revisar o cambiar alguna configuraci�n.
ReadyLabel2b=Haga clic en Instalar para continuar con el proceso.
ReadyMemoUserInfo=Informaci�n del usuario:
ReadyMemoDir=Carpeta de Destino:
ReadyMemoType=Tipo de Instalaci�n:
ReadyMemoComponents=Componentes Seleccionados:
ReadyMemoGroup=Carpeta del Men� Inicio:
ReadyMemoTasks=Tareas Adicionales:

; *** "Preparing to Install" wizard page
WizardPreparing=Prepar�ndose para Instalar
PreparingDesc=El programa de instalaci�n est� prepar�ndose para instalar [name] en su sistema.
PreviousInstallNotCompleted=La instalaci�n/desinstalaci�n previa de un programa no se complet�. Deber� reiniciar el sistema para completar esa instalaci�n.%n%nUna vez reiniciado el sistema, ejecute el programa de instalaci�n nuevamente para completar la instalaci�n de [name].
CannotContinue=El programa de instalaci�n no puede continuar. Por favor, presione Cancelar para salir.

; *** "Installing" wizard page
WizardInstalling=Instalando
InstallingLabel=Por favor, espere mientras se instala [name] en su sistema.

; *** "Setup Completed" wizard page
FinishedHeadingLabel=Completando la instalaci�n de [name]
FinishedLabelNoIcons=El programa complet� la instalaci�n de [name] en su sistema.
FinishedLabel=El programa complet� la instalaci�n de [name] en su sistema. Puede ejecutar la aplicaci�n haciendo clic sobre el icono instalado.
ClickFinish=Haga clic en Finalizar para salir del programa de instalaci�n.
FinishedRestartLabel=Para completar la instalaci�n de [name], su sistema debe reiniciarse. �Desea reiniciarlo ahora?
FinishedRestartMessage=Para completar la instalaci�n de [name], su sistema debe reiniciarse.%n%n�Desea reiniciarlo ahora?
ShowReadmeCheck=S�, deseo ver el archivo L�AME
YesRadio=&S�, deseo reiniciar el sistema ahora
NoRadio=&No, reiniciar� el sistema m�s tarde
; used for example as 'Run MyProg.exe'
RunEntryExec=Ejecutar %1
; used for example as 'View Readme.txt'
RunEntryShellExec=Ver %1

; *** "Setup Needs the Next Disk" stuff
ChangeDiskTitle=El Programa de Instalaci�n Necesita el Siguiente Disco
SelectDiskLabel2=Por favor, inserte el Disco %1 y haga clic en Aceptar.%n%nSi los archivos pueden ser hallados en una carpeta diferente a la indicada abajo, introduzca la ruta correcta o haga clic en Examinar.
PathLabel=&Ruta:
FileNotInDir2=El archivo "%1" no se ha podido hallar en "%2". Por favor, inserte el disco correcto o seleccione otra carpeta.
SelectDirectoryLabel=Por favor, especifique la ubicaci�n del siguiente disco.

; *** Installation phase messages
SetupAborted=La instalaci�n no se ha completado.%n%nPor favor solucione el problema y ejecute nuevamente el programa de instalaci�n.
EntryAbortRetryIgnore=Haga clic en Reintentar para intentarlo de nuevo, en Omitir para continuar de todas formas, o en Anular para cancelar la instalaci�n.

; *** Installation status messages
StatusCreateDirs=Creando carpetas...
StatusExtractFiles=Extrayendo archivos...
StatusCreateIcons=Creando accesos directos...
StatusCreateIniEntries=Creando entradas INI...
StatusCreateRegistryEntries=Creando entradas de registro...
StatusRegisterFiles=Registrando archivos...
StatusSavingUninstall=Guardando informaci�n para desinstalar...
StatusRunProgram=Terminando la instalaci�n...
StatusRollback=Deshaciendo cambios...

; *** Misc. errors
ErrorInternal2=Error interno: %1
ErrorFunctionFailedNoCode=%1 fall�
ErrorFunctionFailed=%1 fall�; c�digo %2
ErrorFunctionFailedWithMessage=%1 fall�; c�digo %2.%n%3
ErrorExecutingProgram=Imposible ejecutar el archivo:%n%1

; *** Registry errors
ErrorRegOpenKey=Error al abrir la clave del registro:%n%1\%2
ErrorRegCreateKey=Error al crear la clave del registro:%n%1\%2
ErrorRegWriteKey=Error al escribir la clave del registro:%n%1\%2

; *** INI errors
ErrorIniEntry=Error al crear entrada INI en el archivo "%1".

; *** File copying errors
FileAbortRetryIgnore=Haga clic en Reintentar para intentarlo de nuevo, en Omitir para excluir este archivo (no recomendado), o en Anular para cancelar la instalaci�n.
FileAbortRetryIgnore2=Haga clic en Reintentar para intentarlo de nuevo, en Omitir para continuar de todas formas (no recomendado), o en Anular para cancelar la instalaci�n.
SourceIsCorrupted=El archivo de origen est� da�ado
SourceDoesntExist=El archivo de origen "%1" no existe
ExistingFileReadOnly=El archivo existente est� marcado como s�lo-lectura.%n%nHaga clic en Reintentar para quitar el atributo de s�lo-lectura e intentarlo de nuevo, en Omitir para excluir este archivo, o en Anular para cancelar la instalaci�n.
ErrorReadingExistingDest=Ocurri� un error mientras se intentaba leer el archivo:
FileExists=El archivo ya existe.%n%n�Desea sobreescribirlo?
ExistingFileNewer=El archivo existente es m�s reciente que el que est� tratando de instalar. Se recomienda que mantenga el archivo existente.%n%n�Desea mantener el archivo existente?
ErrorChangingAttr=Ocurri� un error al intentar cambiar los atributos del archivo:
ErrorCreatingTemp=Ocurri� un error al intentar crear un archivo en la carpeta de destino:
ErrorReadingSource=Ocurri� un error al intentar leer el archivo de origen:
ErrorCopying=Ocurri� un error al intentar copiar el archivo:
ErrorReplacingExistingFile=Ocurri� un error al intentar reemplazar el archivo existente:
ErrorRestartReplace=Fall� reintento de reemplazar:
ErrorRenamingTemp=Ocurri� un error al intentar renombrar un archivo en la carpeta de destino:
ErrorRegisterServer=Imposible registrar el DLL/OCX: %1
ErrorRegSvr32Failed=RegSvr32 fall� con el c�digo de salida %1
ErrorRegisterTypeLib=Imposible registrar la librer�a de tipos: %1

; *** Post-installation errors
ErrorOpeningReadme=Ocurri� un error al intentar abrir el archivo L�AME.
ErrorRestartingComputer=El programa de instalaci�n no pudo reiniciar el equipo. Por favor, h�galo manualmente.

; *** Uninstaller messages
UninstallNotFound=El archivo "%1" no existe. Imposible desinstalar.
UninstallOpenError=El archivo "%1" no pudo ser abierto. Imposible desinstalar
UninstallUnsupportedVer=El archivo de registro para desinstalar "%1" est� en un formato no reconocido por esta versi�n del desinstalador. Imposible desinstalar
UninstallUnknownEntry=Una entrada desconocida (%1) fue encontrada en el registro de desinstalaci�n
ConfirmUninstall=�Est� seguro que desea desinstalar completamente %1 y todos sus componentes?
UninstallOnlyOnWin64=Este programa s�lo puede ser desinstalado en Windows de 64-bits.
OnlyAdminCanUninstall=Este programa s�lo puede ser desinstalado por un usuario con privilegios administrativos.
UninstallStatusLabel=Por favor, espere mientras %1 es removido de su sistema.
UninstalledAll=%1 fue removido exitosamente de su sistema.
UninstalledMost=La desinstalaci�n de %1 ha sido completada.%n%nAlgunos elementos no pudieron eliminarse. Estos pueden ser eliminados manualmente.
UninstalledAndNeedsRestart=Para completar la desinstalaci�n de %1, su sistema debe reiniciarse.%n%n�Desea reiniciarlo ahora?
UninstallDataCorrupted=El archivo "%1" est� da�ado. No puede desinstalarse

; *** Uninstallation phase messages
ConfirmDeleteSharedFileTitle=�Eliminar Archivo Compartido?
ConfirmDeleteSharedFile2=El sistema indica que el siguiente archivo compartido no es usado por ning�n otro programa. �Desea eliminar este archivo compartido?%n%nSi hay programas que usan este archivo y el mismo es eliminado, esos programas pueden dejar de funcionar correctamente. Si no est� seguro, elija No. Dejar el archivo en su sistema no producir� ning�n da�o.
SharedFileNameLabel=Archivo:
SharedFileLocationLabel=Ubicaci�n:
WizardUninstalling=Estado de la Desinstalaci�n
StatusUninstalling=Desinstalando %1...

; The custom messages below aren't used by Setup itself, but if you make
; use of them in your scripts, you'll want to translate them.

[CustomMessages]
;Inno Setup Built-in Custom Messages
NameAndVersion=%1 versi�n %2
AdditionalIcons=Iconos adicionales:
OptionalFeatures=Optional Features:
CreateDesktopIcon=Crear un icono en el &escritorio
CreateQuickLaunchIcon=Crear un icono de Inicio R�pido
ProgramOnTheWeb=%1 en la Web
UninstallProgram=Desinstalar %1
LaunchProgram=Ejecutar %1
AssocFileExtension=&Asociar %1 con la extensi�n de archivo %2
AssocingFileExtension=Asociando %1 con la extensi�n de archivo %2...

;Things we can also localize
CompanyName=Thingamahoochie Software

;Types
TypicalInstallation=Typical Installation
FullInstallation=Full Installation
CompactInstallation=Compact Installation
CustomInstallation=Custom Installation

;Components
AppCoreFiles=WinMerge Core Files
ApplicationRuntimes=Application Runtimes
Filters=Filters
Plugins=Plugins

;Localization Components
Languages=Languages
BulgarianLanguage=Bulgarian menus and dialogs
CatalanLanguage=Catalan menus and dialogs
ChineseSimplifiedLanguage=Chinese (Simplified) menus and dialogs
ChineseTraditionalLanguage=Chinese (Traditional) menus and dialogs
CroatianLanguage=Croatian menus and dialogs
CzechLanguage=Czech menus and dialogs
DanishLanguage=Danish menus and dialogs
DutchLanguage=Dutch menus and dialogs
FrenchLanguage=French menus and dialogs
GermanLanguage=German menus and dialogs
GreekLanguage=Greek menus and dialogs
HungarianLanguage=Hungarian menus and dialogs
ItalianLanguage=Italian menus and dialogs
JapaneseLanguage=Japanese menus and dialogs
KoreanLanguage=Korean menus and dialogs
NorwegianLanguage=Norwegian menus and dialogs
PolishLanguage=Polish menus and dialogs
PortugueseBrazilLanguage=Portuguese (Brazilian) menus and dialogs
PortugueseLanguage=Portuguese menus and dialogs
RomanianLanguage=Romanian menus and dialogs
RussianLanguage=Russian menus and dialogs
SlovakLanguage=Slovak menus and dialogs
SlovenianLanguage=Slovenian menus and dialogs
SpanishLanguage=Spanish menus and dialogs
SwedishLanguage=Swedish menus and dialogs
TurkishLanguage=Turkish menus and dialogs
UkrainianLanguage=Ukrainian menus and dialogs

;Tasks
ExplorerContextMenu=&Enable Explorer context menu integration
IntegrateTortoiseCVS=Integrate with &TortoiseCVS
IntegrateTortoiseSVN=Integrate with T&ortoiseSVN
IntegrateClearCase=Integrate with Rational &ClearCase
IntegrateDelphi4=Borland� Delphi &4 Binary File Support
UpdatingCommonControls=Updating the System's Common Controls

;Icon Labels
ReadMe=Read Me
UsersGuide=User's Guide
UpdatingCommonControls=Updating the System's Common Controls
ViewStartMenuFolder=&View the WinMerge Start Menu Folder
PluginsText=Plugins

;Code Dialogs
DeletePreviousStartMenu=The installer has detected that you changed the location of your start menu from "%s" to "%s". Would you like to delete the previous start menu folder?

; Project file description
ProjectFileDesc=WinMerge Project file
