; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Ginga"
#define MyAppVersion "1.0"
#define MyAppPublisher "Telemidia"
#define MyAppURL "github.com/telemidia/ginga"
#define MyAppExeName "gingagui.exe"
#define MyAppIcoName "icon.ico"

;[Registry]
;Root: HKCU; Subkey: "Environment"; ValueType:string; ValueName: "GST_PLUGIN_PATH_1_0"; \
;    ValueData: "C:\gstreamer\1.0\x86_64\lib"; Flags: preservestringtype

[Registry]

Root: HKCR; Subkey: ".ncl";                             ValueData: "{#MyAppName}";          Flags: uninsdeletevalue; ValueType: string;  ValueName: ""
Root: HKCR; Subkey: "{#MyAppName}";                     ValueData: "Program {#MyAppName}";  Flags: uninsdeletekey;   ValueType: string;  ValueName: ""
Root: HKCR; Subkey: "{#MyAppName}\DefaultIcon";         ValueData: "{app}\ginga\{#MyAppExeName},0";               ValueType: string;  ValueName: ""
;Root: HKCR; Subkey: "{#MyAppName}\DefaultIcon";         ValueData: "{app}\icon.ico";               ValueType: string;  ValueName: ""
Root: HKCR; Subkey: "{#MyAppName}\shell\open\command";  ValueData: """{app}\ginga\{#MyAppExeName}"" ""%1""";  ValueType: string;  ValueName: ""

[Icons]
Name: "{userdesktop}\{#MyAppName}"; Filename: "{app}\ginga\{#MyAppExeName}"; IconFilename: "{app}\{#MyAppIcoName}"; Tasks: desktopicon

[Setup] 
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{FD94E7C2-B90C-4869-B6C2-FB5DD69279B5}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableDirPage=no
DisableProgramGroupPage=yes
OutputBaseFilename=gingasetup
SetupIconFile={#SourcePath}\{#MyAppIcoName}
UninstallDisplayIcon={#SourcePath}\{#MyAppIcoName}
Compression=lzma
SolidCompression=yes
; Tell Windows Explorer to reload the environment
ChangesEnvironment=no
; Tell Windows Explorer to change its File Association
ChangesAssociations = yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]

; Ginga
;Source: "C:\tools\msys64\mingw64\bin\ginga.exe"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\gingagui.exe"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: {#SourcePath}\{#MyAppIcoName}; DestDir: "{app}\"; Flags: ignoreversion


; Ginga-gui GTK icons & depends
Source: "C:\tools\msys64\mingw64\share\icons\Adwaita\16x16\*"; DestDir: "{app}\ginga\share\icons\Adwaita\16x16"; Flags: ignoreversion recursesubdirs createallsubdirs
; This fix the bug of mising icons windows buttons
Source: "C:\tools\msys64\mingw64\share\icons\Adwaita\16x16\ui\window*"; DestDir: "{app}\ginga\share\icons\Adwaita\16x16\actions"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\tools\msys64\mingw64\share\icons\hicolor\16x16\*"; DestDir: "{app}\ginga\share\icons\hicolor\16x16"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\tools\msys64\mingw64\share\icons\hicolor\index.theme"; DestDir: "{app}\ginga\share\icons\hicolor"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\tools\msys64\mingw64\share\glib-2.0\*"; DestDir: "{app}\ginga\share\glib-2.0"; Flags: ignoreversion recursesubdirs createallsubdirs
;Source: "C:\tools\msys64\mingw64\share\icons\hicolor\*"; DestDir: "{app}\ginga\share\icons"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\tools\msys64\mingw64\share\ginga\*"; DestDir: "{app}\ginga\share\ginga"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\tools\msys64\mingw64\lib\gdk-pixbuf-2.0\*"; DestDir: "{app}\ginga\lib\gdk-pixbuf-2.0"; Flags: ignoreversion recursesubdirs createallsubdirs

; Nclua deps
Source: "C:\tools\msys64\mingw64\lib\lua\*"; DestDir: "{app}\lib\lua"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\tools\msys64\mingw64\share\lua\*"; DestDir: "{app}\share\lua"; Flags: ignoreversion recursesubdirs createallsubdirs

; Ginga deps
Source: "C:\tools\msys64\mingw64\bin\libatk-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libbz2-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libcairo-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libcairo-gobject-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
; Source: "C:\tools\msys64\mingw64\bin\libcroco-0.6-3.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libcurl-4.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
; Source: "C:\tools\msys64\mingw64\bin\libeay32.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libepoxy-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libexpat-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libffi-7.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libfontconfig-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libfreetype-6.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgcc_s_seh-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgdk_pixbuf-2.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgdk-3-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgio-2.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libglib-2.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgmodule-2.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgobject-2.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgraphite2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstapp-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstbase-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstreamer-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstvideo-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgtk-3-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libharfbuzz-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libiconv-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libicudt67.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libicuuc67.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libidn2-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libintl-8.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\liblzma-5.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libnclua-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libnghttp2-14.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\liborc-0.4-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libpango-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libpangocairo-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libpangoft2-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libpangowin32-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libpcre-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libpixman-1-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libpng16-16.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\librsvg-2-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libssp-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libpsl-5.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libthai-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libdatrie-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libbrotlidec.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libbrotlicommon.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
;Source: "C:\tools\msys64\mingw64\bin\libssh2-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libstdc++-6.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libunistring-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libwinpthread-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libxml2-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\lua53.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
;Source: "C:\tools\msys64\mingw64\bin\ssleay32.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\zlib1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion

; Gstreamer plugin libs
Source: "C:\tools\msys64\mingw64\lib\gstreamer-1.0\*"; DestDir: "{app}\ginga\lib\gstreamer-1.0"; Flags: ignoreversion recursesubdirs createallsubdirs

; Gstreamer plugin bin

Source: "C:\tools\msys64\mingw64\bin\avcodec-58.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\avfilter-7.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\avformat-58.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
;Source: "C:\tools\msys64\mingw64\bin\avresample-3.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\avutil-56.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
; Source: "C:\tools\msys64\mingw64\bin\liba52-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libass-9.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libbluray-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libcaca-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
; Source: "C:\tools\msys64\mingw64\bin\libcdio-18.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libcelt0-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libchromaprint.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
; Source: "C:\tools\msys64\mingw64\bin\libdaalabase-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
; Source: "C:\tools\msys64\mingw64\bin\libdaaladec-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
; Source: "C:\tools\msys64\mingw64\bin\libdaalaenc-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libdca-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libdvdcss-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libdvdnav-4.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libdvdread-8.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libfaac-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libfaad-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libFLAC-8.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libfluidsynth-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libfribidi-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
;Source: "C:\tools\msys64\mingw64\bin\libgfortran-4.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgme.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgmp-10.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgnutls-30.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgsm.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstadaptivedemux-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstaudio-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstbadaudio-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
;Source: "C:\tools\msys64\mingw64\bin\libgstbadbase-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
;Source: "C:\tools\msys64\mingw64\bin\libgstbadvideo-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstbasecamerabinsrc-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstcodecparsers-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstcontroller-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstfft-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstgl-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstmpegts-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstnet-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstopencv-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstpbutils-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstphotography-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstriff-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstrtp-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstrtsp-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgstsdp-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgsttag-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libgsturidownloader-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libHalf-2_5.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libhogweed-6.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libIex-2_5.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libIlmImf-2_5.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libIlmThread-2_5.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libImath-2_5.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libjasper-4.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libjpeg-8.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libmodplug-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libmp3lame-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libmpeg2-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libmpg123-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libnettle-8.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libogg-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopenal-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopenblas.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopencore-amrnb-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopencore-amrwb-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopencv_bgsegm430.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopencv_calib3d430.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopencv_core430.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopencv_flann430.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopencv_imgcodecs430.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopencv_imgproc430.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopencv_objdetect430.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopencv_video430.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopenh264.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopenjp2-7.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libopus-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libp11-kit-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libportaudio-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libquadmath-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libreadline8.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\librtmp-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
; Source: "C:\tools\msys64\mingw64\bin\libschroedinger-1.0-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libshout-3.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libsndfile-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libSoundTouch-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libsoup-2.4-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libspeex-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libsqlite3-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libsrtp2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libtag.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libtasn1-6.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libtermcap-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libtheora-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libtheoradec-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libtheoraenc-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libtiff-5.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libtwolame-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libvorbis-0.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libvorbisenc-2.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libvorbisidec-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libvpx-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libwavpack-1.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libwebp-7.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libwebpmux-3.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libx264-159.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\libx265.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\postproc-55.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\swresample-3.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\swscale-5.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\tbb.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion
Source: "C:\tools\msys64\mingw64\bin\xvidcore.dll"; DestDir: "{app}\ginga\"; Flags: ignoreversion

; NOTE: Don't use "Flags: ignoreversion" on any shared system files
[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\ginga\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\ginga\{#MyAppExeName}"; Tasks: desktopicon

